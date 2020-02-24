#include "3D/ObjParser.h"
#include "Utility/Maths.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <string>

namespace Sprocket {

namespace {
    
std::vector<std::string> tokenize(const std::string& str,
                                  const std::string& delimiter = " ")
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delimiter, prev);
        if (pos == std::string::npos) {
            pos = str.length();
        }

        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) {
            tokens.push_back(token);
        }

        prev = pos + delimiter.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

Maths::vec2 loadVec2(const std::string& line)
{
    std::istringstream iss(line);
    float x, y;
    iss >> x >> y;
    return Maths::vec2(x, y);
}

Maths::vec3 loadVec3(const std::string& line)
{
    std::istringstream iss(line);
    float x, y, z;
    iss >> x >> y >> z;
    return Maths::vec3(x, y, z);
}

std::tuple<int, int, int> parseObjVertex(const std::string& vertex)
{
    std::vector<std::string> vertexData = tokenize(vertex, "/");
    int posIdx = std::stoi(vertexData[0]) - 1;
    int texIdx = std::stoi(vertexData[1]) - 1;
    int norIdx = std::stoi(vertexData[2]) - 1;
    return {posIdx, texIdx, norIdx};
}

}

std::pair<VertexBuffer, IndexBuffer> parseObjFile(const std::string& objFile)
{
    std::ifstream file;
    file.open(objFile);

    // First we will process all position, texture and normal
    // data into vectors. We will then construct the
    // VertexBuffer and IndexBuffer when looping over the faces
    // data in the OBJ file.
    std::vector<Maths::vec3> tempPositions;
    std::vector<Maths::vec2> tempTexCoords;
    std::vector<Maths::vec3> tempNormals;
    
    VertexBuffer vertices;
    IndexBuffer  indices;
    
    // Maps pos/tex/nor strings to the index of the corresponding
    // Vertex in the VertexBuffer.
    std::unordered_map<std::string, unsigned int> seenFaces;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            tempPositions.push_back(loadVec3(line.substr(2)));
        }
        else if (line.substr(0, 3) == "vt ") {
            tempTexCoords.push_back(loadVec2(line.substr(3)));
        }
        else if (line.substr(0, 3) == "vn ") {
            tempNormals.push_back(loadVec3(line.substr(3)));
        }

        // Although this is all one loop, all face lines come after all other
        // lines, so when we start entering this branch, all the temp vectors
        // will be populated.
        else if (line.substr(0, 2) == "f ") {
            std::vector<std::string> face = tokenize(line.substr(2));

            for (const auto& vertex : face) {
                auto it = seenFaces.find(vertex);
                if (it != seenFaces.end()) {
                    // We have seen this face before, so no need to increase
                    // the VertexBuffer; we can just push its index to the
                    // IndexBuffer.
                    indices.push_back(it->second);
                }
                else {
                    // We have not need this face before, so we need to
                    // create a new Vertex object, add it to VertexBuffer,
                    // cache its index in seenFaces, and push the index to
                    // the IndexBuffer.  
                    auto [posIdx, texIdx, norIdx] = parseObjVertex(vertex);
                    Maths::vec3 position = tempPositions[posIdx];
                    Maths::vec2 texCoord = tempTexCoords[texIdx];
                    Maths::vec3 normal   = tempNormals[norIdx];

                    Vertex newVertex = {
                        {position.x, position.y, position.z},
                        {texCoord.x, texCoord.y},
                        {normal.x, normal.y, normal.z}
                    };

                    unsigned int newVertexIdx = (unsigned int)vertices.size();

                    indices.push_back(newVertexIdx);
                    vertices.push_back(newVertex);
                    seenFaces[vertex] = newVertexIdx;
                }
            }
        }
    }
    
    return {vertices, indices};
}

}