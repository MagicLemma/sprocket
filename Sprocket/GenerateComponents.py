import json
import os

from Datamatic import definitions
from Datamatic import serialiser
from Datamatic import lua_bindings

sprocket = os.path.abspath(os.path.dirname(__file__))

components_h = os.path.join(sprocket, "Scene", "Components.h")
serialiser_cpp = os.path.join(sprocket, "Scene", "Serialiser.cpp")
lua_h = os.path.join(sprocket, "Scripting", "LuaComponents.h")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

definitions.generate(spec, components_h)
serialiser.generate(spec, serialiser_cpp)
lua_bindings.generate(spec, lua_h)