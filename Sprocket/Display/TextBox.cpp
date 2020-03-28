#include "TextBox.h"

namespace Sprocket {

TextBox::TextBox(float width,
                 float height,
                 const std::string& message)
    : Widget(width, height)
    , d_background({{0.0, 0.0}, width, height})
    , d_text({message})
{
}

void TextBox::updateImpl(Window* window)
{
}

bool TextBox::handleEventImpl(Window* window, const Event& event)
{
    return false;
}

void TextBox::drawImpl(DisplayRenderer* renderer) const
{
    renderer->draw(toScreenCoords(d_background));
    Text text = d_text;
    text.position = toScreenCoords(text.position + Maths::vec2{0.0, 0.5 * d_background.height});
    renderer->draw(text);
}

}