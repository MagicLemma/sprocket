"""
Plugin for generating Lua code.
"""

def main(reg):
    
    @reg.compmethod
    def signature(ctx):
        return ", ".join(attr["name"] for attr in ctx.comp['attributes'])