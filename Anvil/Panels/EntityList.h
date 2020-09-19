#pragma once
#include <string>

namespace Sprocket {

class EditorLayer;

class EntityList
{
    std::string d_search;

public:
    void Show(EditorLayer& editor);

};

}