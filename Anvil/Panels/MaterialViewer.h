#pragma once
#include <Sprocket.h>

namespace Sprocket {

class EditorLayer;

class MaterialViewer
{
    MaterialManager* d_materialManager;

public:
    MaterialViewer(MaterialManager* materialManager);

    void Show(EditorLayer& editor);

};

}