#version 450 core

layout(location = 0) out vec4 out_colour;

in Data
{
    vec3 world_position;
    vec2 texture_coords;

    vec3 world_normal;
    vec3 world_tangent;
    vec3 world_bitangent;

    vec3 normal;
    vec3 tangent;
    vec3 bitangent;

    vec4 light_space_pos;
    vec3 to_camera;
} p_data;

// Material Info
uniform sampler2D texture_sampler;
uniform float     u_metallic;
uniform float     u_roughness;

// Lighting Information
uniform vec3  u_light_pos[5];
uniform vec3  u_light_colour[5];
uniform float u_light_brightness[5];

uniform vec3  u_sun_direction;
uniform vec3  u_sun_colour;
uniform float u_sun_brightness;

uniform vec3  u_ambience_colour;
uniform float u_ambience_brightness;

// Shadows
uniform sampler2D shadow_map;

// Takes a value between 
float cutoff(float value, float low, float high) {
    return clamp((value - low) / (high - low), 0.0, 1.0);
}

void main()
{
    // Surface information
    vec3 unit_normal = normalize(p_data.world_normal);
    vec3 unit_to_camera = normalize(p_data.to_camera);

    // Colour prior to lighting
    vec4 colour = texture(texture_sampler, p_data.texture_coords);

    // Ambience
    vec4 ambience = vec4(u_ambience_brightness * u_ambience_colour, 1.0);

    // Lighting calculation
    vec4 total_diffuse = vec4(0.0);
    vec4 total_specular = vec4(0.0);

    float sun_brightness = u_sun_brightness * max(-u_sun_direction.y, 0.0);
    sun_brightness *= cutoff(abs(dot(u_sun_direction, unit_normal)), 0.15, 0.25);

    // Sun diffuse light
    vec3 unit_sun_direction = normalize(u_sun_direction);
    float sun_diffuse_factor = dot(-unit_sun_direction, unit_normal);
    sun_diffuse_factor = max(sun_diffuse_factor, 0.0);
    total_diffuse += sun_diffuse_factor * sun_brightness * vec4(u_sun_colour, 1.0);

    // Sun specular light
    vec3 sun_reflected_light_dir = reflect(unit_sun_direction, unit_normal);
    float sun_specular_factor = dot(sun_reflected_light_dir, unit_to_camera);
    sun_specular_factor = max(sun_specular_factor, 0.0);
    total_specular += sun_specular_factor * sun_brightness * vec4(u_sun_colour, 1.0);
    
    // Point Lights
    for (int i = 0; i != 5; i++) {
        vec3 to_light = u_light_pos[i] - p_data.world_position;
        vec3 unit_to_light = normalize(to_light);
        vec3 reflected_light_direction = reflect(-unit_to_light, unit_normal);

        // Attenuation calculation
        float d = length(to_light);
        float attenuation = 1.0 / (d * d);

        // Diffuse lighting calculation
        float diffuse_factor = dot(unit_to_light, unit_normal);
        diffuse_factor = max(diffuse_factor, 0.0) * attenuation;
        total_diffuse += u_light_brightness[i] * vec4(diffuse_factor * u_light_colour[i], 1.0);

        // Specular lighting calculation
        float specular_factor = dot(reflected_light_direction, unit_to_camera);
        specular_factor = max(specular_factor, 0.0);
        specular_factor = pow(specular_factor, u_roughness) * attenuation;
        total_specular = total_specular + vec4(specular_factor * u_metallic * u_light_colour[i], 1.0);
    }

    // Shadows
    vec3 proj_coords = p_data.light_space_pos.xyz / p_data.light_space_pos.w;
    proj_coords = 0.5 * proj_coords + 0.5;
    float current_depth = proj_coords.z;
    float d = dot(p_data.world_normal, -u_sun_direction);
    //float bias = max(0.005 * (1.0 - d), 0.001);
    float bias = 0.0;
    
    float shadow = 0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y= -1; y <= 1; ++y) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if (proj_coords.z > 1.0) { shadow = 0.0; }

    out_colour = (ambience + (1.0 - shadow) * (total_diffuse + total_specular)) * colour;
}