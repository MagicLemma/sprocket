#include "EscapeMenu.h"

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
{
    using namespace Sprocket;

    SimpleUITheme theme;
    theme.backgroundColour = FromHex(0x2C3A47);
    theme.baseColour       = FromHex(0x1B9CFC);
    theme.hoveredColour    = FromHex(0x25CCF7);
    theme.clickedColour    = FromHex(0x55E6C1);
    d_ui.SetTheme(theme);
}

void EscapeMenu::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;

    if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        if (!e->IsConsumed() && e->Key() == Keyboard::ESC) {
            d_worldLayer->d_paused = !d_worldLayer->d_paused;
            e->Consume();
        }
    }

    d_ui.OnEvent(event);

    if (d_worldLayer->d_paused) {
        event.Consume();
    }
}

void EscapeMenu::OnUpdate(double dt)
{
    using namespace Sprocket;
    
    d_ui.OnUpdate(dt);

    if (d_worldLayer->d_paused) {
        d_core.window->SetCursorVisibility(true);
        d_ui.StartFrame();

        static Maths::vec4 panelQuad{50, 50, 200, 50};
        static bool active = true;
        static bool draggable = true;
        static bool clickable = true;
        if (d_ui.StartPanel("Main", &panelQuad, &active, &draggable, &clickable)) {
            Maths::ivec4 region = {10, 10, 180, 30};
            d_ui.Text("Paused", 36.0f, region);
            d_ui.EndPanel();
        }

        d_ui.EndFrame();
    }
}