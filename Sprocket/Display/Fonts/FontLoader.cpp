#include "FontLoader.h"
#include "Character.h"
#include "Tokenize.h"
#include "Log.h"

#include <fstream>
#include <exception>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Sprocket {
namespace {

Model2D GetModel(const Quad& textureQuad,
                 double width,
                 double height)
{
    auto x = textureQuad.position.x;
    auto y = textureQuad.position.y;
    auto w = textureQuad.width;
    auto h = textureQuad.height;
    Vertex2DBuffer buffer{
        {{0.0f,  0.0f  }, {x,     y    }},
        {{width, 0.0f  }, {x + w, y    }},
        {{0.0f,  height}, {x,     y + h}},
        {{width, height}, {x + w, y + h}}      
    };
    return Model2D(buffer);
}

}

std::pair<GlyphMap, float> ParseFntFile(
    const std::string& fntFile,
    const Texture& atlas)
{
    std::ifstream file;
    file.open(fntFile);

    bool error = false;

    GlyphMap glyphs;
    float size;

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 5) == "info ") {
            std::vector<std::string> charAttrs = Tokenize(line.substr(5));
            for (const auto& attr : charAttrs) {
                std::vector<std::string> keyVal = Tokenize(attr, "=");
                if (keyVal.size() != 2) {
                    SPKT_LOG_WARN("Failed to parse font attr '{}'", attr);
                    throw std::exception("Bad Font Parse");
                }

                if (keyVal[0] == "size") {
                    size = std::stof(keyVal[1]);
                    break;
                }
            }
            continue;
        }

        if (line.substr(0, 5) != "char ") {
            continue;
        }

        // Parameters to be populated
        Character c;
        c.id = -1;
        Maths::vec2 texTopLeft = {-1.0, -1.0};
        c.width = -1;
        c.height = -1;
        c.xOffset = -100; // Offsets can be negative, but surely not this big.
        c.yOffset = -100;
        c.advance = -1;

        std::vector<std::string> charAttrs = Tokenize(line.substr(5));
        for (const auto& attr : charAttrs) {
            std::vector<std::string> keyVal = Tokenize(attr, "=");
            if (keyVal.size() != 2) {
                SPKT_LOG_WARN("Failed to parse font attr '{}'", attr);
                throw std::exception("Bad Font Parse");
            }

            if (keyVal[0] == "id") {
                c.id = std::stoi(keyVal[1]);
            }
            else if (keyVal[0] == "x") {
                texTopLeft.x = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "y") {
                texTopLeft.y = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "width") {
                c.width = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "height") {
                c.height = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "xoffset") {
                c.xOffset = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "yoffset") {
                c.yOffset = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "xadvance") {
                c.advance = std::stod(keyVal[1]);
            }
        }

        bool invalid = false;
        if (c.id == -1 ||
            texTopLeft == Maths::vec2{-1.0, -1.0} ||
            c.width == -1 ||
            c.height == -1 ||
            c.xOffset == -100 ||
            c.yOffset == -100 ||
            c.advance == -1) {
            invalid = true;
        }

        if (invalid) {
            SPKT_LOG_WARN("Could not parse line '{}'", line);
            throw std::exception("Bad Font Parse");
        }      
    
        c.textureQuad.position = { texTopLeft.x / (float)atlas.Width(),
                                 texTopLeft.y / (float)atlas.Height() };
        c.textureQuad.width = c.width / (float)atlas.Width();
        c.textureQuad.height = c.height / (float)atlas.Height();

        c.model = GetModel(c.textureQuad, c.width, c.height);

        glyphs.insert(std::make_pair(c.id, c));
    }

    return {glyphs, size};
}
    
}