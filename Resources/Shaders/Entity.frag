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

uniform vec3  u_sun_direction;
uniform vec3  u_sun_colour;
uniform float u_sun_brightness;

uniform vec3  u_ambience_colour;
uniform float u_ambience_brightness;

// Texture/Lighting Information
uniform float u_shine_dampner;
uniform float u_reflectivity;

// Highlighting
uniform float u_brightness;

void main()
{
    // Surface information
    vec3 unit_normal = normalize(p_surface_normal);
    vec3 unit_to_camera = normalize(p_to_camera_vector);

    // Colour prior to lighting
    vec4 colour = texture(texture_sampler, p_texture_coords);

    // Ambience
    vec4 ambience = vec4(u_ambience_brightness * u_ambience_colour, 1.0);

    // Lighting calculation
    vec4 total_diffuse = vec4(0.0);
    vec4 total_specular = vec4(0.0);

    // Sun diffuse light
    vec3 unit_sun_direction = normalize(u_sun_direction);
    float sun_diffuse_factor = dot(-unit_sun_direction, unit_normal);
    sun_diffuse_factor = max(sun_diffuse_factor, 0.0);
    total_diffuse += sun_diffuse_factor * u_sun_brightness * vec4(u_sun_colour, 1.0);

    // Sun specular light
    vec3 sun_reflected_light_dir = reflect(unit_sun_direction, unit_normal);
    float sun_specular_factor = dot(sun_reflected_light_dir, unit_to_camera);
    sun_specular_factor = max(sun_specular_factor, 0.0);
    total_specular += sun_specular_factor * u_sun_brightness * vec4(u_sun_colour, 1.0);

    // Point Lights
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

    out_colour = ambience + total_diffuse * colour + total_specular;
    
    if (u_brightness > 1) {
        out_colour += u_brightness * vec4(0.1, 0.1, 0.1, 1.0);
    }
}