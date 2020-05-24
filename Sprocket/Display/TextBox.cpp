#include "TextBox.h"

namespace Sprocket {

TextBox::TextBox(float width,
                 float height,
                 const std::string& message)
    : Widget(width, height)
    , d_text({message})
{
}

void TextBox::updateImpl(Window* window, DisplayRenderer* renderer)
{
    Text text = d_text;
    text.position = toScreenCoords(text.position);
    renderer->draw(text);
}

}