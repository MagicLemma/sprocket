def wrap(cpp_type, expression):
    if "spkt::entity" in cpp_type:
        return f"transform_entity(id_remapper, {expression})"
    return expression


def main(reg):

    @reg.attrmethod
    def parse_value_from_spec(ctx):
        name = ctx.attr["name"]
        cpp_type = ctx.attr["type"]
        expression = f"spec[\"{name}\"].as<{cpp_type}>()"
        return wrap(cpp_type, expression)

    @reg.attrmethod
    def parse_value_copy(ctx):
        name = ctx.attr["name"]
        cpp_type = ctx.attr["type"]
        expression = f"source_comp.{name}"
        return wrap(cpp_type, expression)

    @reg.compmethod
    @reg.attrmethod
    def is_savable(ctx):
        if ctx.attr is None:
            return "true" if ctx.comp["flags"].get("SAVABLE") else "false"
        return "true" if ctx.attr["flags"].get("SAVABLE") else "false"

    @reg.compmethod
    @reg.attrmethod
    def is_scriptable(ctx):
        if ctx.attr is None:
            return "true" if ctx.comp["flags"].get("SCRIPTABLE") else "false"
        return "true" if ctx.attr["flags"].get("SCRIPTABLE") else "false"

    @reg.attrmethod
    def get_metadata(ctx):
        if "metadata" in ctx.attr:
            pairs = (f'{{ "{k}", "{v}" }}' for k, v in ctx.attr["metadata"].items())
            return "{" + ", ".join(pairs) + "}"
        return "{}"
