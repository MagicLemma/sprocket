#include "PostProcessor.h"

namespace Sprocket {
namespace {

Vertex2DBuffer GetQuad()
{
    return Vertex2DBuffer{
        {
            Maths::vec3{-1.0f, -1.0f, 0.0f},
            Maths::vec2{0.0f, 0.0f}
        }, {
            Maths::vec3{1.0f, -1.0f, 0.0f},
            Maths::vec2{1.0f, 0.0f}
        }, {
            Maths::vec3{1.0f, 1.0f, 0.0f},
            Maths::vec2{1.0f, 1.0f}
        }, {
            Maths::vec3{1.0f, 1.0f, 0.0f},
            Maths::vec2{1.0f, 1.0f}
        }, {
            Maths::vec3{-1.0f, 1.0f, 0.0f},
            Maths::vec2{0.0f, 1.0f}
        }, {
            Maths::vec3{-1.0f, -1.0f, 0.0f},
            Maths::vec2{0.0f, 0.0f}
        }
    };
}

}

PostProcessor::PostProcessor(int width, int height)
    : d_width(width)
    , d_height(height)
    , d_quad(GetQuad())
{}

void PostProcessor::AddEffect(std::shared_ptr<Effect> effect)
{
    d_effects.push_back(effect);
}

void PostProcessor::Bind() const
{
    d_effects[0]->BindForWrite();
}

void PostProcessor::Unbind() const
{
    d_effects[0]->UnbindForWrite();
}

void PostProcessor::Draw()
{
    d_quad.Bind();
    for (int i = 0; i != d_effects.size(); ++i) {
        
        // Final effect, so draw to screen.
        if (i + 1 == d_effects.size()) {
            d_effects[i]->Draw();
        } else {
            d_effects[i]->Draw(d_effects[i+1]);
        }
    }
    d_quad.Unbind();
}

void PostProcessor::SetScreenSize(int width, int height)
{
    d_width = width;
    d_height = height;
    for (auto& effect : d_effects) {
        effect->SetScreenSize(width, height);
    }
}
    
}