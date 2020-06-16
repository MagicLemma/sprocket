#include "EscapeMenu.h"

void SetButtonAttrs(std::shared_ptr<Sprocket::Button> button)
{
    button->Base().colour = {0.15625f, 0.15625f, 0.15625f};

    button->ButtonNormal().colour = {0.926f, 0.496f, 0.0f};
    button->ButtonHovered().colour = {0.926f, 0.63281f, 0.3242f};
    button->ButtonClicked().colour = {0.324f, 0.90625f, 0.5352f};
}

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
                       WorldLayer* worldLayer,
                       EditorUI* editorUi) 
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_editorUi(editorUi)
    , d_displayRenderer(core.window)
    , d_container(
        (float)core.window->Width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
{
    using namespace Sprocket;

    d_container.Position({10.0f, 10.0f});
    d_container.Base().colour = {0.07f, 0.07f, 0.07f};
    d_container.Base().roundness = 0.081f;
    d_container.AddProperty<Draggable>();

    auto topSlider = d_container.Add<Slider>(300.0f, 50.0f);
    SetSliderAttrs(topSlider);

    auto cameraSwitchButton = d_container.Add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );

    SetButtonAttrs(cameraSwitchButton);

    cameraSwitchButton->SetUnclickCallback([&]() {  //unclick
        switch (d_worldLayer->d_mode) {
            case Mode::PLAYER: {
                d_worldLayer->d_mode = Mode::EDITOR;
            } break;
             case Mode::EDITOR: {
                d_worldLayer->d_mode = Mode::PLAYER;
            } break;
        }
    });

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

    if (d_worldLayer->d_paused) {
        d_container.OnEvent(event);
        event.Consume();
    }
}

void EscapeMenu::OnUpdate(double dt)
{
    d_displayRenderer.OnUpdate();

    if (d_worldLayer->d_paused) {
        d_container.OnUpdate(&d_displayRenderer);
        d_core.window->SetCursorVisibility(true);
    }
}