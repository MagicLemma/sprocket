"""
Plugin for generating Lua code.
"""

def main(reg):

    @reg.compmethod
    def lua_dimension(ctx):
        return str(len(ctx.comp["attributes"]))
    
    @reg.compmethod
    def lua_sig(ctx):
        return ", ".join(attr['name'] for attr in ctx.comp['attributes'])

    @reg.compmethod
    def lua_arglist(ctx):
        return ", ".join(f"x{i}" for i in range(len(ctx.comp["attributes"])))