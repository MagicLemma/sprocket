#include "EscapeMenu.h"

void SetSliderAttrs(std::shared_ptr<Sprocket::Slider> slider)
{
    slider->Base().colour = {0.15625f, 0.15625f, 0.15625f};
    slider->Bar().colour = {0.3f, 0.3f, 0.3f};
    slider->Picker().colour = {0.926f, 0.496f, 0.0f};

    slider->Bar().width = slider->Base().width * 0.9f;
    slider->Bar().height = slider->Base().height * 0.1f;
    slider->Bar().position.x = slider->Base().width * (1.0f - 0.9f) / 2.0f;
    slider->Bar().position.y = slider->Base().height * (1.0f - 0.1f) / 2.0f;

    slider->Picker().width = slider->Base().width * 0.02f;
    slider->Picker().height = slider->Base().height * 0.8f;
    slider->Picker().position.x = slider->Base().width * (1.0f - 0.02f) / 2.0f;
    slider->Picker().position.y = slider->Base().height * (1.0f - 0.8f) / 2.0f;
}

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_displayRenderer(core.window)
    , d_container(
        (float)core.window->Width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_ui(core.window)
{
    using namespace Sprocket;

    SimpleUITheme theme;
    theme.baseColour = {0.926f, 0.496f, 0.0f, 1.0f};
    theme.hoveredColour = {0.926f, 0.63281f, 0.3242f, 1.0f};
    theme.clickedColour = {0.324f, 0.90625f, 0.5352f, 1.0f};
    d_ui.SetTheme(theme);

    d_container.Position({10.0f, 10.0f});
    d_container.Base().colour = {0.07f, 0.07f, 0.07f};
    d_container.Base().roundness = 0.081f;
    d_container.AddProperty<Draggable>();

    auto topSlider = d_container.Add<Slider>(300.0f, 50.0f);
    SetSliderAttrs(topSlider);


    auto textBox = d_container.Add<TextBox>(
        300.0f, 50.0f, "Text box!"
    );
    textBox->TextData().position = {10.0, 10.0};
    d_text = textBox.get();
    
    textBox->Base().colour = {0.15625f, 0.15625f, 0.15625f};
    textBox->TextData().font = Sprocket::Font::CALIBRI;

    topSlider->SetCallback([textBox](float val) {
        textBox->TextData().size = 24.0f + val * 60.0f;
    });

    topSlider->SetValue(0.0f);
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
        d_container.OnEvent(event);
        event.Consume();
    }
}

void EscapeMenu::OnUpdate(double dt)
{
    using namespace Sprocket::Maths;

    d_displayRenderer.OnUpdate();
    d_ui.OnUpdate(dt);

    if (!d_worldLayer->d_paused) {
        return; // Layer not active
    }

    auto* window = d_worldLayer->d_core.window;
    float w = (float)window->Width();
    float h = (float)window->Height();

    d_ui.StartFrame();
    d_ui.Quad(w * 0.35f, 0.0f, w * 0.3f, h, vec4{0.0, 0.0, 0.0, 0.8});

    if (d_ui.Button(1, "Toggle Dev UI", w * 0.375f, 100, w * 0.25f, 50)) {
        switch (d_worldLayer->d_mode) {
            case Mode::PLAYER: {
                d_worldLayer->d_mode = Mode::EDITOR;
            } break;
            case Mode::EDITOR: {
                d_worldLayer->d_mode = Mode::PLAYER;
            } break;
        }
    }

    if (d_ui.Button(2, "Volume", w * 0.375f, 175, w * 0.25f, 50)) {
        SPKT_LOG_INFO("Clicked!");
    }
    

    d_ui.EndFrame();

    d_container.OnUpdate(&d_displayRenderer);
    d_core.window->SetCursorVisibility(true);
}