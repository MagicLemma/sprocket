import json
import os.path as op
from pathlib import Path

from Datamatic import SchemaValidator
from Datamatic import Gen_Loader
from Datamatic import Plugin
from Datamatic.Plugins import Lua, Inspector

sprocket = op.abspath(op.dirname(__file__))
sprocket_base = op.dirname(sprocket)

with open("ComponentSpec.json") as specfile:
    spec = json.loads(specfile.read())

SchemaValidator.validate(spec)

for file in Path(sprocket_base).glob("**/*.dm.*"):
    Gen_Loader.generate(spec, str(file))

print("Done!")