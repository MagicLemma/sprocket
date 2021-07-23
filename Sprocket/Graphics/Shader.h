#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace spkt {

class shader
{
    std::string d_vert_source;
    std::string d_frag_source;

    std::uint32_t d_program_id;
    std::uint32_t d_vert_shader_id;
    std::uint32_t d_frag_shader_id;

    std::uint32_t uniform_location(const std::string& name) const;

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

public:
    shader(std::string_view vert_shader_file, std::string_view frag_shader_file);
    ~shader();

    bool reload();
    
    std::string& vertex_source() { return d_vert_source; }
    std::string& fragment_source() { return d_frag_source; }

    void bind() const;
    void unbind() const;

    // Shader Uniform Setters
    void load(const std::string& name, int value) const;
    void load(const std::string& name, float value) const;
    void load(const std::string& name, const glm::vec2& vector) const;
    void load(const std::string& name, const glm::vec3& vector) const;
    void load(const std::string& name, const glm::vec4& vector) const;
    void load(const std::string& name, const glm::quat& quat) const;
    void load(const std::string& name, const glm::mat4& matrix, int count = 1) const;
};

using shader_ptr = std::unique_ptr<shader>;

// HELPER FUNCTIONS
std::string array_name(std::string_view uniformName, std::size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}