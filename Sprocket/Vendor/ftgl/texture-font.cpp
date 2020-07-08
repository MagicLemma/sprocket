/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include "texture-font.h"
#include "utf8-utils.h"
#include "Maths.h"

#define HRES  64
#define HRESf 64.f
#define DPI   72

namespace Sprocket {
namespace {

bool texture_font_load_face(std::shared_ptr<texture_font_t> self, float size,
                            FT_Library *library, FT_Face *face)
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

    error = FT_New_Face(*library, self->filename.c_str(), 0, face);
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

void texture_font_generate_kerning(std::shared_ptr<texture_font_t> self,
                                   FT_Library *library, FT_Face *face )
{
    // For each glyph couple combination, check if kerning is necessary
    // Starts at index 1 since 0 is for the special backgroudn glyph
    for (std::size_t i = 1; i < self->glyphs.size(); ++i) {
        auto glyph = self->glyphs[i];
        FT_UInt glyph_index = FT_Get_Char_Index( *face, glyph->codepoint );
        glyph->kerning.clear();

        for (std::size_t j = 1; j < self->glyphs.size(); ++j) {
            auto prev_glyph = self->glyphs[j];
            FT_UInt prev_index = FT_Get_Char_Index( *face, prev_glyph->codepoint );
            FT_Vector kerning;
            FT_Get_Kerning( *face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );

            if (kerning.x) {
                Kerning k;
                k.codepoint = prev_glyph->codepoint;
                k.kerning = kerning.x / (float)(HRESf*HRESf);
                glyph->kerning.push_back(k);
            }
        }
    }
}

int texture_font_init(std::shared_ptr<texture_font_t> self)
{
    FT_Library library;
    FT_Face face;
    FT_Size_Metrics metrics;

    assert(self->atlas);
    assert(self->size > 0);

    self->height = 0;
    self->outline_thickness = 0.0;
    self->hinting = 1;
    self->padding = 1;

    if (!texture_font_load_face(self, self->size * 100.f, &library, &face))
        return -1;

    self->underline_position = face->underline_position / (float)(HRESf*HRESf) * self->size;
    self->underline_position = roundf( self->underline_position );
    if( self->underline_position > -2 )
    {
        self->underline_position = -2.0;
    }

    self->underline_thickness = face->underline_thickness / (float)(HRESf*HRESf) * self->size;
    self->underline_thickness = roundf( self->underline_thickness );
    if( self->underline_thickness < 1 )
    {
        self->underline_thickness = 1.0;
    }

    metrics = face->size->metrics;
    self->height = (metrics.height >> 6) / 100.0;
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return 0;
}

}


std::shared_ptr<TextureGlyph> texture_glyph_new()
{
    auto self = std::make_shared<TextureGlyph>();
    self->codepoint  = -1;
    self->width     = 0;
    self->height    = 0;
    self->outline_thickness = 0.0;
    self->offset_x  = 0;
    self->offset_y  = 0;
    self->advance_x = 0.0;
    self->advance_y = 0.0;
    self->s0        = 0.0;
    self->t0        = 0.0;
    self->s1        = 0.0;
    self->t1        = 0.0;
    return self;
}

float texture_glyph_get_kerning(const std::shared_ptr<TextureGlyph> self,
                                const char* codepoint )
{
    uint32_t ucodepoint = utf8_to_utf32(codepoint);
    auto& k = self->kerning;

    auto it = std::find_if(k.begin(), k.end(), [&](const auto x) {
        return x.codepoint == ucodepoint;
    });

    if (it != k.end()) {
        SPKT_LOG_INFO("Found kerning!");
        return it->kerning;
    }

    return 0;
}

std::shared_ptr<texture_font_t> texture_font_new_from_file(
    FontAtlas* atlas,
    float size,
    const std::string& filename)
{
    auto self = std::make_shared<texture_font_t>();

    self->atlas = atlas;
    self->size  = size;
    self->filename = filename;

    if (texture_font_init(self)) {
        return nullptr;
    }

    return self;
}

std::shared_ptr<TextureGlyph>
texture_font_find_glyph(std::shared_ptr<texture_font_t> self,
                        const char * codepoint)
{
    uint32_t ucodepoint = utf8_to_utf32( codepoint );

    for (std::size_t i = 0; i < self->glyphs.size(); ++i) {
        auto glyph = self->glyphs[i];
        if (glyph->codepoint == ucodepoint) {
            return glyph;
        }
    }

    return nullptr;
}

int texture_font_load_glyph(
    std::shared_ptr<texture_font_t> self,
    const char* codepoint)
{
    FT_Library library;
    FT_Error error;
    FT_Face face;
    FT_Glyph ft_glyph;
    FT_GlyphSlot slot;
    FT_Bitmap ft_bitmap;

    FT_UInt glyph_index;
    FT_Int32 flags = 0;
    int ft_glyph_top = 0;
    int ft_glyph_left = 0;

    Maths::ivec4 region;
    size_t missed = 0;

    if (!texture_font_load_face(self, self->size, &library, &face)) {
        return 0;
    }

    /* Check if codepoint has been already loaded */
    if (texture_font_find_glyph(self, codepoint)) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return 1;
    }

    flags = 0;
    ft_glyph_top = 0;
    ft_glyph_left = 0;
    glyph_index = FT_Get_Char_Index(face, (FT_ULong)utf8_to_utf32(codepoint));

    flags |= FT_LOAD_RENDER;

    if (!self->hinting)
    {
        flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
    }
    else
    {
        flags |= FT_LOAD_FORCE_AUTOHINT;
    }

    error = FT_Load_Glyph( face, glyph_index, flags );
    if (error) {
        // TODO: Print error 
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return 0;
    }

    slot            = face->glyph;
    ft_bitmap       = slot->bitmap;
    ft_glyph_top    = slot->bitmap_top;
    ft_glyph_left   = slot->bitmap_left;

    struct {
        int left;
        int top;
        int right;
        int bottom;
    } padding = { 0, 0, 1, 1 };

    if (self->padding != 0) {
        padding.top += self->padding;
        padding.left += self->padding;
        padding.right += self->padding;
        padding.bottom += self->padding;
    }

    std::size_t src_w = ft_bitmap.width;
    std::size_t src_h = ft_bitmap.rows;

    std::size_t tgt_w = src_w + padding.left + padding.right;
    std::size_t tgt_h = src_h + padding.top + padding.bottom;

    region = self->atlas->GetRegion(tgt_w, tgt_h);

    if (region.x < 0) {
        SPKT_LOG_ERROR("Texture atlas is full!");
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    std::size_t x = region.x;
    std::size_t y = region.y;

    std::vector<unsigned char> buffer;
    buffer.resize(tgt_w * tgt_h);
    
    unsigned char *dst_ptr = buffer.data() + (padding.top * tgt_w + padding.left);
    unsigned char *src_ptr = ft_bitmap.buffer;
    for (std::size_t i = 0; i < src_h; i++ )
    {
        std::memcpy( dst_ptr, src_ptr, ft_bitmap.width);
        dst_ptr += tgt_w;
        src_ptr += ft_bitmap.pitch;
    }

    self->atlas->SetRegion({x, y, tgt_w, tgt_h}, buffer);

    auto glyph = texture_glyph_new();
    glyph->codepoint = utf8_to_utf32( codepoint );
    glyph->width    = tgt_w;
    glyph->height   = tgt_h;
    glyph->outline_thickness = self->outline_thickness;
    glyph->offset_x = ft_glyph_left;
    glyph->offset_y = ft_glyph_top;
    glyph->s0       = x/(float)self->atlas->Width();;
    glyph->t0       = y/(float)self->atlas->Height();
    glyph->s1       = (x + glyph->width)/(float)self->atlas->Width();
    glyph->t1       = (y + glyph->height)/(float)self->atlas->Height();

    // Discard hinting to get advance
    FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
    slot = face->glyph;
    glyph->advance_x = slot->advance.x / HRESf;
    glyph->advance_y = slot->advance.y / HRESf;

    self->glyphs.push_back(glyph);

    texture_font_generate_kerning( self, &library, &face );

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return 1;
}

std::shared_ptr<TextureGlyph> texture_font_get_glyph(
    std::shared_ptr<texture_font_t> self,
    const char* codepoint )
{
    auto glyph = texture_font_find_glyph(self, codepoint);
    if (glyph != nullptr) {
        return glyph;
    }

    if (texture_font_load_glyph(self, codepoint)) {
        return texture_font_find_glyph(self, codepoint);
    }

    return nullptr;
}

}