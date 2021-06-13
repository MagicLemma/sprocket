""" Plugin for generating Lua code. """

def main(reg):
    @reg.compmethod
    def lua_unpack(ctx, objname):
        return ", ".join(f"{objname}.{attr['name']}" for attr in ctx.comp["attributes"])