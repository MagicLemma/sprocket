#ifdef DATAMATIC_BLOCK SCRIPTABLE=true
{{Comp.Name}} = Class(function(self, {{Comp.Lua.Sig}})
    self.{{Attr.Name}} = {{Attr.Name}}
end)

{{Comp.Lua.Getter}}
{{Comp.Lua.Setter}}
{{Comp.Lua.Adder}}

#endif