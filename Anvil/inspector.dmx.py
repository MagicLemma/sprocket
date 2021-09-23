"""
Plugin for the Inspector
"""

def main(reg):

    @reg.attrmethod
    def inspector_display(ctx):
        attr = ctx.attr
        name = attr["name"]
        display = attr["display_name"]
        cpp_type = attr["type"]

        data = attr.get("custom", {})

        if cpp_type == "glm::quat":
            return f'spkt::ImGuiXtra::Euler("{display}", &c.{name})'
        if cpp_type == "bool":
            return f'ImGui::Checkbox("{display}", &c.{name})'
        if cpp_type == "ecs::Identifier":
            return f'ImGui::Text("{display}: %llu", c.{name})'
        
        # Things like vectors and matrices and queues will get ignored for now
        return ""