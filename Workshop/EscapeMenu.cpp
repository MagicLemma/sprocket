#include "EscapeMenu.h"

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer,
                       EditorUI* editorUi) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_editorUi(editorUi)
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

    if (d_worldLayer->d_paused) {
        d_core.window->SetCursorVisibility(true);

        d_ui.OnUpdate(dt);

        d_ui.StartFrame();

        static Maths::vec4 panelQuad{50, 50, 200, 200};
        static bool active = true;
        static bool draggable = true;
        static bool clickable = true;
        if (d_ui.StartPanel("Main", &panelQuad, &active, &draggable, &clickable)) {

            if (d_ui.Button("Toggle Mode", {10, 10, 180, 50})) {
                switch (d_worldLayer->d_mode) {
                    case Mode::OBSERVER: {
                        SPKT_LOG_INFO("Mode switched to Player");
                        d_worldLayer->d_mode = Mode::PLAYER;

                        d_worldLayer->d_observerCamera->Get<ScriptComponent>().active = false;
                        d_worldLayer->d_editorCamera->Get<ScriptComponent>().active = false;
                        d_worldLayer->d_activeCamera = d_worldLayer->d_playerCamera;

                        d_worldLayer->d_playerMovement.Enable(true);
                        d_worldLayer->d_mouseRequired = false;
                        d_worldLayer->d_selector.Enable(false);

                    } break;
                    case Mode::PLAYER: {
                        SPKT_LOG_INFO("Mode switched to Editor");
                        d_worldLayer->d_mode = Mode::EDITOR;

                        d_worldLayer->d_observerCamera->Get<ScriptComponent>().active = false;
                        d_worldLayer->d_editorCamera->Get<ScriptComponent>().active = true;
                        d_worldLayer->d_activeCamera = d_worldLayer->d_editorCamera;

                        d_worldLayer->d_playerMovement.Enable(false);
                        d_worldLayer->d_mouseRequired = true;
                        d_worldLayer->d_selector.Enable(true);

                    } break;
                    case Mode::EDITOR: {
                        SPKT_LOG_INFO("Mode switched to Observer");
                        d_worldLayer->d_mode = Mode::OBSERVER;

                        d_worldLayer->d_observerCamera->Get<ScriptComponent>().active = true;
                        d_worldLayer->d_editorCamera->Get<ScriptComponent>().active = false;
                        d_worldLayer->d_activeCamera = d_worldLayer->d_observerCamera;

                        d_worldLayer->d_playerMovement.Enable(false);
                        d_worldLayer->d_mouseRequired = false;
                        d_worldLayer->d_selector.Enable(false);

                    } break;
                }
            }

            d_ui.EndPanel();
        }

        d_ui.EndFrame();
    }
}