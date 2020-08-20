import json
import os

from Datamatic import definitions

sprocket = os.path.abspath(os.path.dirname(__file__))

components_h = os.path.join(sprocket, "Scene", "Components.h")
serialiser_cpp = os.path.join(sprocket, "Scene", "Serialiser.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

definitions.generate(spec, components_h)