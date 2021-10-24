#include "font.h"

#include <Sprocket/Core/log.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cassert>
#include <algorithm>

namespace spkt {

static constexpr int HRES = 64;
static constexpr float HRESf = 64.0f;
static constexpr int DPI = 72;

namespace {

uint32_t to_utf32(const char* c)
{
    uint32_t result = -1;

    if (!c) {
        return result;
    }

    if ((c[0] & 0x80) == 0x0) {
        result = c[0];
    }

    if ((c[0] & 0xC0) == 0xC0) {
        result = ((c[0] & 0x3F) << 6 ) | (c[1] & 0x3F);
    }

    if ((c[0] & 0xE0) == 0xE0) {
        result = ((c[0] & 0x1F) << (6 + 6)) | ((c[1] & 0x3F) << 6) | (c[2] & 0x3F);
    }

    if ((c[0] & 0xF0) == 0xF0) {
        result = ((c[0] & 0x0F) << (6 + 6 + 6)) | ((c[1] & 0x3F) << (6 + 6)) | ((c[2] & 0x3F) << 6) | (c[3] & 0x3F);
    }

    if ((c[0] & 0xF8) == 0xF8) {
        result = ((c[0] & 0x07) << (6 + 6 + 6 + 6)) | ((c[1] & 0x3F) << (6 + 6 + 6)) | ((c[2] & 0x3F) << (6 + 6)) | ((c[3] & 0x3F) << 6) | (c[4] & 0x3F);
    }

    return result;
}

bool load_face(const std::string& filename, float size, FT_Library *library, FT_Face *face)
{
    FT_Error error;
    FT_Matrix matrix = {
        (int)((1.0/HRES) * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((1.0)      * 0x10000L)};

    assert(library);
    assert(size);

    error = FT_Init_FreeType(library);
    if (error) {
        return false;
    }

    error = FT_New_Face(*library, filename.c_str(), 0, face);
    if (error) {
        FT_Done_FreeType(*library);
        return false;
    }

    error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
    if (error) {
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return false;
    }

    error = FT_Set_Char_Size(*face, (int)(size * HRES), 0, DPI * HRES, DPI);
    if (error) {
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return false;
    }

    FT_Set_Transform(*face, &matrix, NULL);
    return true;
}

void generate_kerning(
    const glyph_map& glyphs,
    kerning_map& kernings,
    FT_Library* library,
    FT_Face* face)
{
    if (!FT_HAS_KERNING(*face)) {
        return;
    }

    kernings.clear();
    for (const auto [lcp, leftGlyph] : glyphs) {
        FT_UInt leftIndex = FT_Get_Char_Index(*face, (FT_ULong)lcp);

        for (const auto [rcp, rightGlyph] : glyphs) {
            FT_UInt rightIndex = FT_Get_Char_Index(*face, (FT_ULong)rcp);
        
            FT_Vector kerning;
            FT_Get_Kerning(*face, leftIndex, rightIndex, FT_KERNING_UNFITTED, &kerning);
            if (kerning.x) {
                auto key = std::make_pair(lcp, rcp);
                kernings.emplace(key, kerning.x / (HRESf * HRESf));
            }
        }
    }
}

}

font::font(const std::string& filename)
    : d_atlas(1024, 1024)
    , d_filename(filename)
{
}

Glyph font::get_glyph(char c, float size)
{
    auto& font = d_font_data[size];

    uint32_t ucodepoint = to_utf32(&c);
    auto it = font.glyphs.find(ucodepoint);
    if (it != font.glyphs.end()) {
        return it->second;
    }

    // If we could not find it, attempt to load it
    load_glyph(c, size);
    it = font.glyphs.find(ucodepoint);
    if (it != font.glyphs.end()) {
        return it->second;
    }

    return Glyph(); // Empty glyph
}

bool font::load_glyph(char c, float size)
{
    uint32_t codepoint = to_utf32(&c);
    
    FT_Library library;
    FT_Face face;
    if (!load_face(d_filename, size, &library, &face)) {
        return false;
    }

    FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong)codepoint);
    FT_Int32 flags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;
    if (FT_Load_Glyph(face, glyph_index, flags)) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return false;
    }

    FT_GlyphSlot slot = face->glyph;
    FT_Bitmap bitmap  = slot->bitmap;
    auto region = d_atlas.get_region(bitmap.width, bitmap.rows);
    if (region.x < 0) {
        spkt::log::error("Texture atlas is full!");
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return false;
    }

    d_atlas.set_region(region, bitmap.buffer);

    auto& font = d_font_data[size];
    Glyph& glyph = font.glyphs[codepoint];

    glyph.codepoint = codepoint;
    glyph.width     = slot->bitmap.width;
    glyph.height    = slot->bitmap.rows;
    glyph.offset    = {slot->bitmap_left, slot->bitmap_top};
    glyph.texture.x = region.x / (float)d_atlas.width();
    glyph.texture.y = region.y / (float)d_atlas.height();
    glyph.texture.z = glyph.width / (float)d_atlas.width();
    glyph.texture.w = glyph.height / (float)d_atlas.height();
    glyph.advance = glm::vec2{slot->advance.x, slot->advance.y} / HRESf;

    generate_kerning(font.glyphs, font.kernings, &library, &face);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return true;
}

float font::get_kerning(char left, char right, float size)
{
    auto& font = d_font_data[size];

    uint32_t l = to_utf32(&left);
    uint32_t r = to_utf32(&right);

    auto it = font.kernings.find(std::make_pair(l, r));
    if (it != font.kernings.end()) {
        return it->second;
    }

    return 0.0f;
}

float font::text_width(std::string_view text, float size)
{
    float width = 0;
    for (int i = 0; i != text.size(); ++i) {
        auto glyph = get_glyph(text[i], size);

        if (i > 0) {
            width += get_kerning(text[i-1], text[i], size);
        }

        width += glyph.advance.x;
    }

    Glyph first = get_glyph(text.front(), size);
    width -= first.offset.x;

    Glyph last = get_glyph(text.back(), size);
    width += last.width;
    width += last.offset.x;
    width -= last.advance.x;

    return width;
}

}