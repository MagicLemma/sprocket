#include <Sprocket.h>
#include "WorldLayer.h"
#include "EditorUI.h"
#include "EscapeMenu.h"
#include "Palette.h"

int main()
{
    Sprocket::Window window("Game");

    Sprocket::App app;
    auto worldLayer = app.Add<WorldLayer>(&window);
    auto editorUi = app.Add<EditorUI>(worldLayer.get());
    app.Add<EscapeMenu>(worldLayer.get());

    Sprocket::RunOptions options;
    options.showFramerate = true;

    return Sprocket::Run(app, window, options);
}