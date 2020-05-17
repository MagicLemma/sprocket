#version 400 core

in vec2  p_texture_coords;
in vec3  p_surface_normal;
in vec3  p_to_camera_vector;
in vec3  p_to_light_vector[5];

layout(location = 0) out vec4 out_colour;

uniform sampler2D texture_sampler;
uniform sampler2D specular_sampler;
uniform sampler2D normal_sampler;

// Lighting Information
uniform vec3 u_light_colour[5];
uniform vec3 u_light_attenuation[5];

// Texture/Lighting Information
uniform float u_shine_dampner;
uniform float u_reflectivity;

void main()
{
    // Surface information
    vec3 unit_normal = normalize(p_surface_normal);
    vec3 unit_to_camera = normalize(p_to_camera_vector);

    // Colour prior to lighting
    vec4 colour = texture(texture_sampler, p_texture_coords);

    // Lighting calculation
    vec4 total_diffuse = vec4(0.0);
    vec4 total_specular = vec4(0.0);

    for (int i = 0; i != 5; i++) {
        vec3 unit_to_light = normalize(p_to_light_vector[i]);
        vec3 reflected_light_direction = reflect(-unit_to_light, unit_normal);

        // Attenuation calculation
        float d = length(p_to_light_vector[i]);
        float attenuation = u_light_attenuation[i].x + u_light_attenuation[i].y * d + u_light_attenuation[i].z * d * d;

        // Diffuse lighting calculation
        float diffuse_factor = dot(unit_to_light, unit_normal);
        diffuse_factor = max(diffuse_factor, 0.0) / attenuation;
        total_diffuse = total_diffuse + vec4(diffuse_factor * u_light_colour[i], 1.0);

        // Specular lighting calculation
        float specular_factor = dot(reflected_light_direction, unit_to_camera);
        float reflectivity = texture(specular_sampler, p_texture_coords).r;
        specular_factor = max(specular_factor, 0.0);
        specular_factor = pow(specular_factor, u_shine_dampner) / attenuation;
        total_specular = total_specular + vec4(specular_factor * u_reflectivity * u_light_colour[i], 1.0);
    }

    // Ambient lighting calculation
    total_diffuse = max(total_diffuse, 0.1);

    out_colour = total_diffuse * colour + total_specular;
}