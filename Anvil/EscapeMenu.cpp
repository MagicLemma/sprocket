#include "EscapeMenu.h"

namespace Sprocket {

EscapeMenu::EscapeMenu(const CoreSystems& core,
                       EditorLayer* editorLayer) 
    : Layer(core)
    , d_editorLayer(editorLayer)
    , d_ui(core.window)
{
    SimpleUITheme theme;
    theme.backgroundColour = FromHex(0x2C3A47);
    theme.baseColour       = FromHex(0x1B9CFC);
    theme.hoveredColour    = FromHex(0x25CCF7);
    theme.clickedColour    = FromHex(0x55E6C1);
    d_ui.SetTheme(theme);
}

void EscapeMenu::OnEvent(Event& event)
{
    if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        if (!e->IsConsumed() && e->Key() == Keyboard::ESC) {
            d_editorLayer->d_paused = !d_editorLayer->d_paused;
            e->Consume();
        }
    }

    d_ui.OnEvent(event);
    if (d_editorLayer->d_paused) { event.Consume(); }
}

void EscapeMenu::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);

    if (d_editorLayer->d_paused) {
        d_ui.StartFrame();

        static Maths::vec4 panelQuad{50, 50, 200, 50};
        static bool x = true;
        if (d_ui.StartPanel("Main", &panelQuad, &x, &x, &x)) {
            Maths::ivec4 region = {10, 10, 180, 30};
            d_ui.Text("Paused", 36.0f, region);
            d_ui.EndPanel();
        }

        d_ui.EndFrame();
    }
}

}