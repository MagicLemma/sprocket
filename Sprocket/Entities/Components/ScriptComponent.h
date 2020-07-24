#pragma once
#include <string>

namespace Sprocket {

struct ScriptComponent
{
    std::string script;
    bool        active = true;
};
    
}