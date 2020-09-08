import json
import os.path as op

from Datamatic import SchemaValidator

from Datamatic import Gen_Loader
from Datamatic import anvil_inspector

from Datamatic import Plugin
from Datamatic.Plugins import Lua

sprocket = op.abspath(op.dirname(__file__))
sprocket_base = op.dirname(sprocket)

#inspector = op.join(sprocket_base, "Anvil", "Panels", "Inspector.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

SchemaValidator.validate(spec)

components_h_src = op.join(sprocket, "Scene", "Components.dm.h")
components_h_dst = op.join(sprocket, "Scene", "Components.h")
Gen_Loader.generate(spec, components_h_src, components_h_dst)

loader_src = op.join(sprocket, "Scene", "Loader.dm.cpp")
loader_dst = op.join(sprocket, "Scene", "Loader.cpp")
Gen_Loader.generate(spec, loader_src, loader_dst)

script_h_src = op.join(sprocket, "Scripting", "LuaComponents.dm.h")
script_h_dst = op.join(sprocket, "Scripting", "LuaComponents.h")
Gen_Loader.generate(spec, script_h_src, script_h_dst)

script_cpp_src = op.join(sprocket, "Scripting", "LuaComponents.dm.cpp")
script_cpp_dst = op.join(sprocket, "Scripting", "LuaComponents.cpp")
Gen_Loader.generate(spec, script_cpp_src, script_cpp_dst)

script_lua_src = op.join(sprocket, "Scripting", "Sprocket_Components.dm.lua")
script_lua_dst = op.join(sprocket, "Scripting", "Sprocket_Components.lua")
Gen_Loader.generate(spec, script_lua_src, script_lua_dst)