import json
import os

from Datamatic import definitions
from Datamatic import serialiser
from Datamatic import lua_backend
from Datamatic import lua_bindings
from Datamatic import anvil_inspector

sprocket = os.path.abspath(os.path.dirname(__file__))
sprocket_base = os.path.dirname(sprocket)

components_h = os.path.join(sprocket, "Scene", "Components.h")
serialiser_cpp = os.path.join(sprocket, "Scene", "Loader.cpp")
lua_h = os.path.join(sprocket, "Scripting", "LuaComponents.h")
lua_cpp = os.path.join(sprocket, "Scripting", "LuaComponents.cpp")
lua = os.path.join(sprocket, "Scripting", "Sprocket_Components.lua")
inspector = os.path.join(sprocket_base, "Anvil", "Panels", "Inspector.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

definitions.generate(spec, components_h)
serialiser.generate(spec, serialiser_cpp)
lua_backend.generate_header(spec, lua_h)
lua_backend.generate_cpp(spec, lua_cpp)
lua_bindings.generate(spec, lua)
anvil_inspector.generate(spec, inspector)