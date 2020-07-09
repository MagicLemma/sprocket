#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

#include "FontAtlas.h"

namespace Sprocket {

struct HashPair { 
    template <class T1, class T2> 
    size_t operator()(const std::pair<T1, T2>& p) const
    { 
        auto hash1 = std::hash<T1>{}(p.first); 
        auto hash2 = std::hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
};

using KerningMap = std::unordered_map<
    std::pair<uint32_t, uint32_t>,
    float,
    HashPair
>;

struct Glyph
{
    uint32_t codepoint = -1;

    std::size_t width  = 0;
    std::size_t height = 0;

    int offset_x = 0;
    int offset_y = 0;

    float advance_x = 0.0f;
    float advance_y = 0.0f;

    float s0 = 0.0f;
    float t0 = 0.0f;
    float s1 = 0.0f;
    float t1 = 0.0f;
};

class Font
{
    FontAtlas d_atlas;

    std::string d_filename;
    float d_size;
    
    std::vector<std::shared_ptr<Glyph>> d_glyphs;
    int d_padding;

    KerningMap d_kernings;

    std::shared_ptr<Glyph> FindGlyph(char c);
    bool LoadGlyph(char c);

public:
    Font(std::size_t width, std::size_t height);
    bool Load(const std::string& filename, float size);

    std::shared_ptr<Glyph> GetGlyph(char c);

    float GetKerning(char left, char right);

    void Bind() const { d_atlas.Bind(); }
    void Unbind() const { d_atlas.Unbind(); }

    float Size() const { return d_size; }
};

}
