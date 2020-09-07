import json
import os
import os.path as op

from Datamatic import SchemaValidator

from Datamatic import definitions
from Datamatic import Gen_Loader
from Datamatic import lua_backend
from Datamatic import lua_bindings
from Datamatic import anvil_inspector

sprocket = os.path.abspath(os.path.dirname(__file__))
sprocket_base = os.path.dirname(sprocket)

components_h = os.path.join(sprocket, "Scene", "Components.h")
lua_cpp = os.path.join(sprocket, "Scripting", "LuaComponents.cpp")
lua = os.path.join(sprocket, "Scripting", "Sprocket_Components.lua")
inspector = os.path.join(sprocket_base, "Anvil", "Panels", "Inspector.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

SchemaValidator.validate(spec)

#loader_src = op.join(sprocket, "Scene", "Loader.dm.cpp")
#loader_dst = op.join(sprocket, "Scene", "Loader.cpp")
#Gen_Loader.generate(spec, loader_src, loader_dst)

#script_h_src = op.join(sprocket, "Scripting", "LuaComponents.dm.h")
#script_h_dst = op.join(sprocket, "Scripting", "LuaComponents.h")
#Gen_Loader.generate(spec, script_h_src, script_h_dst)

script_cpp_src = op.join(sprocket, "Scripting", "LuaComponents.dm.cpp")
script_cpp_dst = op.join(sprocket, "Scripting", "LuaComponents.cpp")
Gen_Loader.generate(spec, script_cpp_src, script_cpp_dst)

#lua_backend.generate_cpp(spec, lua_cpp)