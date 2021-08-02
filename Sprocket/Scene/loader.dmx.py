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