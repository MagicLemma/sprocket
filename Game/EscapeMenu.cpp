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
    static bool showVolume = false;

    bool mainActive = true;
    bool mainDraggable = false;
    vec4 mainRegion{0.0f, 0.0f, w * 0.3f, h};
    if (d_ui.StartPanel("Main", &mainRegion, &mainActive, &mainDraggable)) {

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

        buttonRegion.y += 2 * 60;
        static float value1 = 27.0f;
        d_ui.Slider("Slider", buttonRegion, &value1, 0, 100);

        buttonRegion.y += 60;
        static float value2 = 84.0f;
        d_ui.Dragger("Dragger", buttonRegion, &value2, 0.1f);

        buttonRegion.y += 60;
        static bool check = false;
        d_ui.Checkbox("Checkbox", buttonRegion, &check);

        buttonRegion.y += 60;
        d_ui.Checkbox("Volume Panel", buttonRegion, &showVolume);
        
        d_ui.EndPanel();
    }
    
    static vec4 shape{w/2 - 200, 100, 400, 500};
    bool draggable = true;
    if (d_ui.StartPanel("VolumePanel", &shape, &showVolume, &draggable)) {
        d_ui.Text("Volume", {0, 0, 400, 100});

        float volume = Sprocket::Audio::GetMasterVolume();
        d_ui.Slider("Master Volume", {10, 100, 400 - 20, 50}, &volume, 0.0, 100.0);
        Sprocket::Audio::SetMasterVolume(volume);
        
        d_ui.EndPanel();
    }

    static vec4 shape2{w/2 + 300, 100, 400, 500};
    bool draggable2 = true;
    if (d_ui.StartPanel("BUTTONS! :D", &shape2, &draggable2, &draggable)) {
        d_ui.Text("BUTTONS! :D", {0, 0, 400, 100});
        vec4 buttonQuad{10, 100, 400 - 20, 50};
        d_ui.Button("Button 1", buttonQuad);
        buttonQuad.y += 60;
        d_ui.Button("Button 2", buttonQuad);
        buttonQuad.y += 60;
        d_ui.Button("Button 3", buttonQuad);
        buttonQuad.y += 60;
        d_ui.Button("Button 4", buttonQuad);
        buttonQuad.y += 60;
        d_ui.Button("Button 5", buttonQuad);
        buttonQuad.y += 60;
        buttonQuad.z = buttonQuad.w;

        static bool valA = false;
        d_ui.Checkbox("A", buttonQuad, &valA);
        buttonQuad.x += buttonQuad.z + 10.0f;

        static bool valB = true;
        d_ui.Checkbox("B", buttonQuad, &valB);
        buttonQuad.x += buttonQuad.z + 10.0f;

        d_ui.EndPanel();
    }

    d_ui.EndFrame();

    d_core.window->SetCursorVisibility(true);
}