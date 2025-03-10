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

    mat3 tangent_space;

    vec4 light_space_pos;
    vec3 to_camera;
} p_data;

// Material Info
uniform sampler2D u_albedo_map;
uniform sampler2D u_normal_map;
uniform sampler2D u_metallic_map;
uniform sampler2D u_roughness_map;

uniform float u_use_albedo_map;
uniform float u_use_normal_map;
uniform float u_use_metallic_map;
uniform float u_use_roughness_map;

uniform vec3  u_albedo;
uniform float u_metallic;
uniform float u_roughness;

// Lighting Information
const int MAX_NUM_LIGHTS = 50;
uniform vec3  u_light_pos[MAX_NUM_LIGHTS];
uniform vec3  u_light_colour[MAX_NUM_LIGHTS];
uniform float u_light_brightness[MAX_NUM_LIGHTS];

uniform vec3  u_sun_direction;
uniform vec3  u_sun_colour;
uniform float u_sun_brightness;

uniform vec3  u_ambience_colour;
uniform float u_ambience_brightness;

// Shadows
uniform sampler2D shadow_map;

const float PI = 3.14159265359;

vec3 fresnel_schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distribution_ggx(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float geometry_schlick_ggx(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometry_schlick_ggx(NdotV, roughness);
    float ggx1  = geometry_schlick_ggx(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 calculate_light(
    vec3 F0, vec3 N, vec3 V, vec3 L, vec3 H,
    vec3 albedo, vec3 radiance, float metallic, float roughness)
{
    // cook-torrance brdf
    float NDF = distribution_ggx(N, H, roughness);
    float G = geometry_smith(N, V, L, roughness);
    vec3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    // add to radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main()
{
    vec3 albedo = u_use_albedo_map > 0.5 ? texture(u_albedo_map, p_data.texture_coords).xyz : u_albedo;

    float metallic = u_use_metallic_map > 0.5 ? texture(u_metallic_map, p_data.texture_coords).r : u_metallic;
    float roughness = u_use_roughness_map > 0.5 ? texture(u_roughness_map, p_data.texture_coords).r : u_roughness;
 
    vec3 N = normalize(p_data.world_normal);
    if (u_use_normal_map > 0.5) {
        N = texture(u_normal_map, p_data.texture_coords).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(p_data.tangent_space * N);
    }

    vec3 V = normalize(p_data.to_camera);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Shadows
    vec3 proj_coords = p_data.light_space_pos.xyz / p_data.light_space_pos.w;
    proj_coords = 0.5 * proj_coords + 0.5;
    float current_depth = proj_coords.z;
    float d = dot(p_data.world_normal, -u_sun_direction);
    
    float shadow = 0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y= -1; y <= 1; ++y) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if (shadow > 0.0) { shadow = 1.0; }
    if (proj_coords.z > 1.0) { shadow = 0.0; }
    
    // reflectance
    vec3 Lo = vec3(0.0);

    // sun
    if (1.0 - shadow > 0.0) {
        vec3 L = normalize(-u_sun_direction);
        vec3 H = normalize(V + L);
        vec3 radiance = u_sun_brightness * u_sun_colour;
        Lo += calculate_light(F0, N, V, L, H, albedo, radiance, metallic, roughness);
    }

    // point lights
    for (int i = 0; i != MAX_NUM_LIGHTS; ++i)
    {
        vec3 L = normalize(u_light_pos[i] - p_data.world_position);
        vec3 H = normalize(V + L);
        float dist = length(u_light_pos[i] - p_data.world_position);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = u_light_brightness[i] * u_light_colour[i] * attenuation;
        Lo += calculate_light(F0, N, V, L, H, albedo, radiance, metallic, roughness);
    }

    vec3 ambience = u_ambience_brightness * u_ambience_colour * albedo;// * ao;
    vec3 colour = Lo + ambience;

    // Gamma Correction
    colour = colour / (colour + vec3(1.0));
    colour = pow(colour, vec3(1.0/2.2));

    out_colour = vec4(colour, 1.0);

}