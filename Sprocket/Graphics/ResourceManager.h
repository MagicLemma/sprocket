#pragma once
#include "Model3D.h"
#include "Model2D.h"
#include "Texture.h"
#include "CubeMap.h"
#include "FrameBuffer.h"

#include <unordered_map>
#include <string>
#include <array>
#include <mutex>

namespace Sprocket {

enum class VBOType
{
    VERTEX_BUFFER = 0,
    INDEX_BUFFER = 1
};

enum class TextureType
{
    FLAT = 0,
    CUBE = 1
};

class ResourceManager
{
    // Mutex to protect accessing ResourceManager across different threads.
    std::mutex d_mutex;

    // Buffers
    std::vector<unsigned int> d_vaoList;
    std::vector<unsigned int> d_rboList;
    std::vector<unsigned int> d_vboList;
    std::vector<unsigned int> d_fboList;
    std::vector<unsigned int> d_texList;

    unsigned int createVAO();
    unsigned int createVBO(VBOType type);
    unsigned int createTEX(TextureType type);
    unsigned int createFBO();
    unsigned int createRBO();

    void loadToVBOs(const Vertex3DBuffer& vertices,
                    const IndexBuffer& indices);

    // Cached Resources
    std::unordered_map<std::string, Model3D> d_model3Ds;
    std::unordered_map<std::string, Texture> d_textures;

public:
    ~ResourceManager(); // Clean up all buffers

    Model3D loadModel3D(const std::string& objFile); // Caches
    Model3D loadModel3D(const Vertex3DBuffer& vertices,
                        const IndexBuffer& indices);

    Model2D loadModel2D(const Vertex2DBuffer& vertices);

    Texture loadTexture(const std::string& pngFile); // Caches

    CubeMap loadCubeMap(const std::array<std::string, 6>& faceFiles);

    FrameBuffer loadFrameBuffer(int width, int height, float scale);
};

}