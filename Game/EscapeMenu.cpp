#include "EscapeMenu.h"

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
{
    using namespace Sprocket;

    SimpleUITheme theme;
    theme.baseColour = {0.926f, 0.496f, 0.0f, 1.0f};
    theme.hoveredColour = {0.926f, 0.63281f, 0.3242f, 1.0f};
    theme.clickedColour = {0.324f, 0.90625f, 0.5352f, 1.0f};
    d_ui.SetTheme(theme);
}

void EscapeMenu::OnEvent(Sprocket::Event& event)
{
    if (auto e = event.As<Sprocket::KeyboardButtonPressedEvent>()) {
        if (!e->IsConsumed() && e->Key() == Sprocket::Keyboard::ESC) {
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
    using namespace Sprocket::Maths;

    d_ui.OnUpdate(dt);

    if (!d_worldLayer->d_paused) {
        return; // Layer not active
    }

    auto* window = d_worldLayer->d_core.window;
    float w = (float)window->Width();
    float h = (float)window->Height();

    d_ui.StartFrame();
    d_ui.Quad(0, 0.0f, w * 0.3f, h, Sprocket::Maths::vec4{0.0, 0.0, 0.0, 0.8});

    if (d_ui.Button(1, "Toggle Dev UI", w * 0.025f, 100, w * 0.25f, 50)) {
        switch (d_worldLayer->d_mode) {
            case Mode::PLAYER: {
                d_worldLayer->d_mode = Mode::EDITOR;
            } break;
            case Mode::EDITOR: {
                d_worldLayer->d_mode = Mode::PLAYER;
            } break;
        }
    }

    if (d_ui.Button(2, "ButtonWABCDEF", w * 0.025f, 175, w * 0.25f, 50)) {
        SPKT_LOG_INFO("Clicked!");
    }

    float volume = Sprocket::Audio::GetMasterVolume();
    d_ui.Slider(5, "Volume", w * 0.025f, 325, w * 0.25f, 50, &volume, 0.0, 100.0);
    Sprocket::Audio::SetMasterVolume(volume);

    d_ui.EndFrame();

    d_core.window->SetCursorVisibility(true);
}