import json
import os

from Datamatic import definitions
from Datamatic import serialiser

sprocket = os.path.abspath(os.path.dirname(__file__))

components_h = os.path.join(sprocket, "Scene", "Components.h")
serialiser_cpp = os.path.join(sprocket, "Scene", "Serialiser.cpp")

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

serialiser.generate(spec, components_h)