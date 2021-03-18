from Datamatic.Plugins import Plugin, compmethod, attrmethod
from Datamatic import Types

class Inspector(Plugin):

    @compmethod
    def GuizmoSettings(comp, flags):
        if comp["Name"] == "Transform3DComponent":
            return "ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);"
        return ""

    @attrmethod
    def Display(attr, flags):
        name = attr["Name"]
        display = attr["DisplayName"]
        cpp_type = attr["Type"]

        data = attr.get("Custom", {})
        cpp_subtype = data.get("Subtype")
        limits = data.get("Limits")

        if cpp_type == "std::string":
            if cpp_subtype == "File":
                filt = data.get("Filter")
                return f'ImGuiXtra::File("{display}", editor.GetWindow(), &c.{name}, "{filt}")'
            return f'ImGuiXtra::TextModifiable(c.{name})'
        if cpp_type == "float":
            if limits is not None:
                a, b = [Types.Float(x) for x in limits]
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
            return f'ImGuiXtra::Euler("{display}", &c.{name})'
        if cpp_type == "bool":
            return f'ImGui::Checkbox("{display}", &c.{name})'
        if cpp_type == "ecs::Identifier":
            return f'ImGui::Text("{display}: %llu", static_cast<std::uint64_t>(c.{name}))'
        
        # Things like vectors and matrices and queues will get ignored for now
        return ""