"""
Plugin for the Inspector
"""

def main(reg):
    @reg.compmethod
    def inspector_guizmo_settings(ctx):
        if ctx.comp["name"] == "Transform3DComponent":
            return "spkt::ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);"
        return ""

    @reg.attrmethod
    def inspector_display(ctx):
        attr = ctx.attr
        name = attr["name"]
        display = attr["display_name"]
        cpp_type = attr["type"]

        data = attr.get("custom", {})
        cpp_subtype = data.get("Subtype")
        limits = data.get("Limits")

        if cpp_type == "std::string":
            if cpp_subtype == "File":
                filt = data.get("Filter")
                return f'spkt::ImGuiXtra::File("{display}", editor.window(), &c.{name}, "{filt}")'
            return f'spkt::ImGuiXtra::TextModifiable(c.{name})'
        if cpp_type == "float":
            if limits := data.get("Limits"):
                a, b = limits
                return f'ImGui::SliderFloat("{display}", &c.{name}, {a}, {b})'
            return f'ImGui::DragFloat("{display}", &c.{name}, 0.01f)'
        if cpp_type == "glm::vec2":
            return f'ImGui::DragFloat2("{display}", &c.{name}.x, 0.1f)'
        if cpp_type == "glm::vec3":
            if cpp_subtype == "Colour":
                return f'ImGui::ColorEdit3("{display}", &c.{name}.r)'
            return f'ImGui::DragFloat3("{display}", &c.{name}.x, 0.1f)'
        if cpp_type == "glm::vec4":
            if cpp_subtype == "Colour":
                return f'ImGui::ColorEdit4("{display}", &c.{name}.r)'
            return f'ImGui::DragFloat4("{display}", &c.{name}.x, 0.1f)'
        if cpp_type == "glm::quat":
            return f'spkt::ImGuiXtra::Euler("{display}", &c.{name})'
        if cpp_type == "bool":
            return f'ImGui::Checkbox("{display}", &c.{name})'
        if cpp_type == "ecs::Identifier":
            return f'ImGui::Text("{display}: %llu", c.{name})'
        
        # Things like vectors and matrices and queues will get ignored for now
        return ""