#include "PostProcessor.h"
#include "Loader.h"

namespace Sprocket {
namespace {

std::shared_ptr<Model2D> getQuad()
{
    Vertex2DBuffer v{
        {Maths::vec2{-1.0f, -1.0f}, Maths::vec2{0.0f, 0.0f}},
        {Maths::vec2{1.0f, -1.0f}, Maths::vec2{1.0f, 0.0f}},
        {Maths::vec2{1.0f, 1.0f}, Maths::vec2{1.0f, 1.0f}},
        {Maths::vec2{1.0f, 1.0f}, Maths::vec2{1.0f, 1.0f}},
        {Maths::vec2{-1.0f, 1.0f}, Maths::vec2{0.0f, 1.0f}},
        {Maths::vec2{-1.0f, -1.0f}, Maths::vec2{0.0f, 0.0f}}
    };
    return Loader::loadModel2D(v);
}

}

PostProcessor::PostProcessor(int width, int height)
    : d_width(width)
    , d_height(height)
{}

void PostProcessor::addEffect(std::shared_ptr<Effect> effect)
{
    d_effects.push_back(effect);
}

void PostProcessor::bind() const
{
    d_effects[0]->bindForWrite();
}

void PostProcessor::unbind() const
{
    d_effects[0]->unbindForWrite();
}

void PostProcessor::draw()
{
    for (int i = 0; i != d_effects.size(); ++i) {
        
        // Final effect, so draw to screen.
        if (i + 1 == d_effects.size()) {
            d_effects[i]->draw();
        } else {
            d_effects[i]->draw(d_effects[i+1]);
        }
    }
}

void PostProcessor::setScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;
    for (auto& effect : d_effects) {
        effect->setScreenSize(width, height);
    }
}
    
}