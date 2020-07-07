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
#include "texture-font.h"
#include "utf8-utils.h"
#include "Maths.h"

#define HRES  64
#define HRESf 64.f
#define DPI   72

namespace Sprocket {

// ------------------------------------------------- texture_font_load_face ---
static int
texture_font_load_face(std::shared_ptr<texture_font_t> self, float size,
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

    /* Initialize library */
    error = FT_Init_FreeType(library);
    if(error) {
        // TODO: Print error 
        goto cleanup;
    }

    /* Load face */
    error = FT_New_Face(*library, self->filename.c_str(), 0, face);

    if(error) {
        // TODO: Print error 
        goto cleanup_library;
    }

    /* Select charmap */
    error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
    if(error) {
        // TODO: Print error 
        goto cleanup_face;
    }

    /* Set char size */
    error = FT_Set_Char_Size(*face, (int)(size * HRES), 0, DPI * HRES, DPI);

    if(error) {
        // TODO: Print error 
        goto cleanup_face;
    }

    /* Set transform matrix */
    FT_Set_Transform(*face, &matrix, NULL);

    return 1;

cleanup_face:
    FT_Done_Face( *face );
cleanup_library:
    FT_Done_FreeType( *library );
cleanup:
    return 0;
}

// ------------------------------------------------------ texture_glyph_new ---
std::shared_ptr<TextureGlyph> texture_glyph_new()
{
    auto self = std::make_shared<TextureGlyph>();
    self->codepoint  = -1;
    self->width     = 0;
    self->height    = 0;
    self->rendermode = RenderMode::RENDER_NORMAL;
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

// ---------------------------------------------- texture_glyph_get_kerning ---
float
texture_glyph_get_kerning( const std::shared_ptr<TextureGlyph> self,
                           const char * codepoint )
{
    size_t i;
    uint32_t ucodepoint = utf8_to_utf32( codepoint );

    assert( self );
    for( i=0; i< self->kerning.size(); ++i )
    {
        auto kerning = self->kerning[i];
        if( kerning->codepoint == ucodepoint )
        {
            return kerning->kerning;
        }
    }
    return 0;
}


// ------------------------------------------ texture_font_generate_kerning ---
void
texture_font_generate_kerning( std::shared_ptr<texture_font_t>  self,
                               FT_Library *library, FT_Face *face )
{
    size_t i, j;
    FT_UInt glyph_index, prev_index;
    FT_Vector kerning;

    assert( self );

    /* For each glyph couple combination, check if kerning is necessary */
    /* Starts at index 1 since 0 is for the special backgroudn glyph */
    for( i=1; i < self->glyphs.size(); ++i )
    {
        auto glyph = self->glyphs[i];
        glyph_index = FT_Get_Char_Index( *face, glyph->codepoint );
        glyph->kerning.clear();

        for( j=1; j < self->glyphs.size(); ++j )
        {
            auto prev_glyph = self->glyphs[j];
            prev_index = FT_Get_Char_Index( *face, prev_glyph->codepoint );
            FT_Get_Kerning( *face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );
            // printf("%c(%d)-%c(%d): %ld\n",
            //       prev_glyph->codepoint, prev_glyph->codepoint,
            //       glyph_index, glyph_index, kerning.x);
            if( kerning.x )
            {
                auto k = std::make_shared<Kerning>();
                k->codepoint = prev_glyph->codepoint;
                k->kerning = kerning.x / (float)(HRESf*HRESf);
                glyph->kerning.push_back(k);
            }
        }
    }
}

// ------------------------------------------------------ texture_font_init ---
static int
texture_font_init(std::shared_ptr<texture_font_t> self)
{
    FT_Library library;
    FT_Face face;
    FT_Size_Metrics metrics;

    assert(self->atlas);
    assert(self->size > 0);

    self->height = 0;
    self->ascender = 0;
    self->descender = 0;
    self->rendermode = RenderMode::RENDER_NORMAL;
    self->outline_thickness = 0.0;
    self->hinting = 1;
    self->kerning = 1;
    self->filtering = 1;

    // FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
    // FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
    self->lcd_weights[0] = 0x10;
    self->lcd_weights[1] = 0x40;
    self->lcd_weights[2] = 0x70;
    self->lcd_weights[3] = 0x40;
    self->lcd_weights[4] = 0x10;

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
    self->ascender = (metrics.ascender >> 6) / 100.0;
    self->descender = (metrics.descender >> 6) / 100.0;
    self->height = (metrics.height >> 6) / 100.0;
    self->linegap = self->height - self->ascender + self->descender;
    FT_Done_Face( face );
    FT_Done_FreeType( library );

    return 0;
}

// --------------------------------------------- texture_font_new_from_file ---
std::shared_ptr<texture_font_t> 
texture_font_new_from_file(std::shared_ptr<texture_atlas_t> atlas, const float pt_size,
        const char *filename)
{
    auto self = std::make_shared<texture_font_t>();

    assert(filename);

    self->atlas = atlas;
    self->size  = pt_size;
    self->filename = filename;

    if (texture_font_init(self)) {
        return nullptr;
    }

    return self;
}

std::shared_ptr<TextureGlyph>
texture_font_find_glyph( std::shared_ptr<texture_font_t> self,
                         const char * codepoint )
{
    size_t i;
    TextureGlyph *glyph;
    uint32_t ucodepoint = utf8_to_utf32( codepoint );

    for( i = 0; i < self->glyphs.size(); ++i )
    {
        auto glyph = self->glyphs[i];
        // If codepoint is -1, we don't care about outline type or thickness
        if( (glyph->codepoint == ucodepoint) &&
            ((ucodepoint == -1) ||
             ((glyph->rendermode == self->rendermode) &&
              (glyph->outline_thickness == self->outline_thickness)) ))
        {
            return glyph;
        }
    }

    return nullptr;
}

int texture_font_load_glyph(
    std::shared_ptr<texture_font_t> self,
    const char* codepoint )
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

    Sprocket::Maths::ivec4 region;
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
    // WARNING: We use texture-atlas depth to guess if user wants
    //          LCD subpixel rendering

    if( self->rendermode != RenderMode::RENDER_NORMAL)
    {
        flags |= FT_LOAD_NO_BITMAP;
    }
    else
    {
        flags |= FT_LOAD_RENDER;
    }

    if( !self->hinting )
    {
        flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
    }
    else
    {
        flags |= FT_LOAD_FORCE_AUTOHINT;
    }

    if( self->atlas->depth == 3 )
    {
        FT_Library_SetLcdFilter( library, FT_LCD_FILTER_LIGHT );
        flags |= FT_LOAD_TARGET_LCD;

        if( self->filtering )
        {
            FT_Library_SetLcdFilterWeights( library, self->lcd_weights );
        }
    }

    error = FT_Load_Glyph( face, glyph_index, flags );
    if( error )
    {
        // TODO: Print error 
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    if( self->rendermode == RenderMode::RENDER_NORMAL)
    {
        slot            = face->glyph;
        ft_bitmap       = slot->bitmap;
        ft_glyph_top    = slot->bitmap_top;
        ft_glyph_left   = slot->bitmap_left;
    }
    else
    {
        FT_Stroker stroker;
        FT_BitmapGlyph ft_bitmap_glyph;

        error = FT_Stroker_New( library, &stroker );

        if( error )
        {
            // TODO: Print error 
            goto cleanup_stroker;
        }

        FT_Stroker_Set(stroker,
                        (int)(self->outline_thickness * HRES),
                        FT_STROKER_LINECAP_ROUND,
                        FT_STROKER_LINEJOIN_ROUND,
                        0);

        error = FT_Get_Glyph( face->glyph, &ft_glyph);

        if( error )
        {
            // TODO: Print error 
            goto cleanup_stroker;
        }

        if( self->rendermode == RenderMode::RENDER_OUTLINE_EDGE )
            error = FT_Glyph_Stroke( &ft_glyph, stroker, 1 );
        else if ( self->rendermode == RenderMode::RENDER_OUTLINE_POSITIVE )
            error = FT_Glyph_StrokeBorder( &ft_glyph, stroker, 0, 1 );
        else if ( self->rendermode == RenderMode::RENDER_OUTLINE_NEGATIVE )
            error = FT_Glyph_StrokeBorder( &ft_glyph, stroker, 1, 1 );

        if( error )
        {
            // TODO: Print error 
            goto cleanup_stroker;
        }

        if( self->atlas->depth == 1 )
            error = FT_Glyph_To_Bitmap( &ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        else
            error = FT_Glyph_To_Bitmap( &ft_glyph, FT_RENDER_MODE_LCD, 0, 1);

        if( error )
        {
            // TODO: Print error 
            goto cleanup_stroker;
        }

        ft_bitmap_glyph = (FT_BitmapGlyph) ft_glyph;
        ft_bitmap       = ft_bitmap_glyph->bitmap;
        ft_glyph_top    = ft_bitmap_glyph->top;
        ft_glyph_left   = ft_bitmap_glyph->left;

cleanup_stroker:
        FT_Stroker_Done( stroker );

        if( error )
        {
            FT_Done_Face( face );
            FT_Done_FreeType( library );
            return 0;
        }
    }

    struct {
        int left;
        int top;
        int right;
        int bottom;
    } padding = { 0, 0, 1, 1 };

    if( self->padding != 0 )
    {
        padding.top += self->padding;
        padding.left += self->padding;
        padding.right += self->padding;
        padding.bottom += self->padding;
    }

    size_t src_w = ft_bitmap.width/self->atlas->depth;
    size_t src_h = ft_bitmap.rows;

    size_t tgt_w = src_w + padding.left + padding.right;
    size_t tgt_h = src_h + padding.top + padding.bottom;

    region = texture_atlas_get_region( self->atlas, tgt_w, tgt_h );

    if ( region.x < 0 )
    {
        fprintf( stderr, "Texture atlas is full (line %d)\n",  __LINE__ );
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    std::size_t x = region.x;
    std::size_t y = region.y;

    unsigned char *buffer = (unsigned char*)calloc( tgt_w * tgt_h * self->atlas->depth, sizeof(unsigned char) );

    unsigned char *dst_ptr = buffer + (padding.top * tgt_w + padding.left) * self->atlas->depth;
    unsigned char *src_ptr = ft_bitmap.buffer;
    for (std::size_t i = 0; i < src_h; i++ )
    {
        //difference between width and pitch: https://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#FT_Bitmap
        memcpy( dst_ptr, src_ptr, ft_bitmap.width);
        dst_ptr += tgt_w * self->atlas->depth;
        src_ptr += ft_bitmap.pitch;
    }

    texture_atlas_set_region( self->atlas, x, y, tgt_w, tgt_h, tgt_w * self->atlas->depth, buffer);

    free( buffer );

    auto glyph = texture_glyph_new();
    glyph->codepoint = utf8_to_utf32( codepoint );
    glyph->width    = tgt_w;
    glyph->height   = tgt_h;
    glyph->rendermode = self->rendermode;
    glyph->outline_thickness = self->outline_thickness;
    glyph->offset_x = ft_glyph_left;
    glyph->offset_y = ft_glyph_top;
    glyph->s0       = x/(float)self->atlas->width;
    glyph->t0       = y/(float)self->atlas->height;
    glyph->s1       = (x + glyph->width)/(float)self->atlas->width;
    glyph->t1       = (y + glyph->height)/(float)self->atlas->height;

    // Discard hinting to get advance
    FT_Load_Glyph( face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
    slot = face->glyph;
    glyph->advance_x = slot->advance.x / HRESf;
    glyph->advance_y = slot->advance.y / HRESf;

    self->glyphs.push_back(glyph);

    if( self->rendermode != RenderMode::RENDER_NORMAL)
        FT_Done_Glyph( ft_glyph );

    texture_font_generate_kerning( self, &library, &face );

    FT_Done_Face( face );
    FT_Done_FreeType( library );

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