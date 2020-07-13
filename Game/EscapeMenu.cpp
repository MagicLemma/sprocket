#include "EscapeMenu.h"
#include "Palette.h"

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
{
    using namespace Sprocket;

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_ui.SetTheme(theme);

    Sprocket::Audio::SetMasterVolume(0);
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
    auto background = SPACE_DARK * 0.1f;
    background.a = 1.0f;
    d_ui.Quad(background, {0.0f, 0.0f, w * 0.3f, h});

    d_ui.Text("Menu", {0.0f, 0.0f, w * 0.3f, 100});

    vec4 buttonRegion = {w * 0.025f, 100, w * 0.25f, 50};

    if (d_ui.Button("Toggle Dev UI", buttonRegion)) {
        switch (d_worldLayer->d_mode) {
            case Mode::PLAYER: {
                d_worldLayer->d_mode = Mode::EDITOR;
            } break;
            case Mode::EDITOR: {
                d_worldLayer->d_mode = Mode::PLAYER;
            } break;
        }
    }

    buttonRegion.y += 60;
    if (d_ui.Button("Button 1", buttonRegion)) {
        SPKT_LOG_INFO("Clicked 1!");
    }
    
    buttonRegion.y += 60;
    if (d_ui.Button("Button 2", buttonRegion)) {
        SPKT_LOG_INFO("Clicked 2!");
    }
    
    buttonRegion.y += 60;
    if (d_ui.Button("Button 3", buttonRegion)) {
        SPKT_LOG_INFO("Clicked 3!");
    }

    buttonRegion.y += 60;
    if (d_ui.Button("Button 4", buttonRegion)) {
        SPKT_LOG_INFO("Clicked 4!");
    }

    buttonRegion.y += 60;
    if (d_ui.Button("Button 5", buttonRegion)) {
        SPKT_LOG_INFO("Clicked 5!");
    }

    buttonRegion.y += 60;
    static bool showVolume = false;
    if (d_ui.Button("Volume Panel", buttonRegion)) {
        showVolume = !showVolume;
    }

    static vec4 shape{w/2 - 200, 200, 400, h - 200};
    d_ui.StartPanel("VolumePanel", &shape, &showVolume);

        float volume = Sprocket::Audio::GetMasterVolume();
        d_ui.Slider("Master Volume", {10, 10, w * 0.25f, 50}, &volume, 0.0, 100.0);
        Sprocket::Audio::SetMasterVolume(volume);

    d_ui.EndPanel();

    buttonRegion.y += 2 * 60;
    static float value1 = 27.0f;
    d_ui.Slider("Value 1", buttonRegion, &value1, 0, 100);

    buttonRegion.y += 60;
    static float value2 = 84.0f;
    d_ui.Dragger("Value 2", buttonRegion, &value2, 0.1f);
    

    d_ui.EndFrame();

    d_core.window->SetCursorVisibility(true);
}