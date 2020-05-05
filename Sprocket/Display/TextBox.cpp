#include "TextBox.h"

namespace Sprocket {

TextBox::TextBox(float width,
                 float height,
                 const std::string& message)
    : Widget(width, height)
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
    Text text = d_text;
    text.position = toScreenCoords(text.position);
    renderer->draw(text);
}

}