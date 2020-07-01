#include "FontLoader.h"
#include "Character.h"
#include "Tokenize.h"
#include "Log.h"

#include <fstream>
#include <exception>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Sprocket {

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
        int id = -1;
        Maths::vec2 texTopLeft = {-1.0, -1.0};
        double width = -1;
        double height = -1;
        double xOffset = -100; // Offsets can be negative, but surely not this big.
        double yOffset = -100;
        double advance = -1;

        std::vector<std::string> charAttrs = Tokenize(line.substr(5));
        for (const auto& attr : charAttrs) {
            std::vector<std::string> keyVal = Tokenize(attr, "=");
            if (keyVal.size() != 2) {
                SPKT_LOG_WARN("Failed to parse font attr '{}'", attr);
                throw std::exception("Bad Font Parse");
            }

            if (keyVal[0] == "id") {
                id = std::stoi(keyVal[1]);
            }
            else if (keyVal[0] == "x") {
                texTopLeft.x = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "y") {
                texTopLeft.y = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "width") {
                width = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "height") {
                height = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "xoffset") {
                xOffset = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "yoffset") {
                yOffset = std::stod(keyVal[1]);
            }
            else if (keyVal[0] == "xadvance") {
                advance = std::stod(keyVal[1]);
            }
        }

        bool invalid = false;
        if (id == -1 ||
            texTopLeft == Maths::vec2{-1.0, -1.0} ||
            width == -1 ||
            height == -1 ||
            xOffset == -100 ||
            yOffset == -100 ||
            advance == -1) {
            invalid = true;
        }

        if (invalid) {
            SPKT_LOG_WARN("Could not parse line '{}'", line);
            throw std::exception("Bad Font Parse");
        }      
    
        Quad textureQuad;
        textureQuad.position = { texTopLeft.x / (float)atlas.Width(),
                                 texTopLeft.y / (float)atlas.Height() };
        textureQuad.width = width / (float)atlas.Width();
        textureQuad.height = height / (float)atlas.Height(); 

        glyphs.insert(std::make_pair(id, Character(
            id, textureQuad, width, height, xOffset, yOffset, advance)));
    }

    return {glyphs, size};
}
    
}