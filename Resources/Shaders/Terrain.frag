#version 400 core

in vec2 d_texture;
in vec3 surfaceNormal;
in vec3 toLightVectors[5];
in vec3 toCameraVector;

layout(location = 0) out vec4 out_Colour;

uniform sampler2D textureSampler;

// Lighting Information
uniform vec3 lightColours[5];
uniform vec3 lightAttenuations[5];

uniform vec3  u_sun_direction;
uniform vec3  u_sun_colour;
uniform float u_sun_brightness;

// Texture/Lighting Information
uniform float shineDamper;
uniform float reflectivity;

void main()
{
    // Surface information
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToCamera = normalize(toCameraVector);

    // Colour prior to lighting
    vec4 colour = texture(textureSampler, d_texture);

    // Lighting calculation
    vec4 totalDiffuse = vec4(0.0);
    vec4 totalSpecular = vec4(0.0);
    
    // Sun diffuse light
    vec3 unit_sun_direction = normalize(u_sun_direction);
    float sun_diffuse_factor = dot(-unit_sun_direction, unitNormal);
    sun_diffuse_factor = max(sun_diffuse_factor, 0.0);
    totalDiffuse += sun_diffuse_factor * u_sun_brightness * vec4(u_sun_colour, 1.0);

    // Sun specular light
    vec3 sun_reflected_light_dir = reflect(unit_sun_direction, unitNormal);
    float sun_specular_factor = dot(sun_reflected_light_dir, unitToCamera);
    sun_specular_factor = max(sun_specular_factor, 0.0);
    totalSpecular += sun_specular_factor * u_sun_brightness * vec4(u_sun_colour, 1.0);

    for (int i = 0; i != 5; i++) {
        vec3 unitToLight = normalize(toLightVectors[i]);
        vec3 reflectedLightDirection = reflect(-unitToLight, unitNormal);

        // Attenuation calculation
        float d = length(toLightVectors[i]);
        float attenuation = lightAttenuations[i].x + lightAttenuations[i].y * d + lightAttenuations[i].z * d * d;

        // Diffuse lighting calculation
        float diffuseFactor = dot(unitToLight, unitNormal);
        diffuseFactor = max(diffuseFactor, 0.0) / attenuation;
        totalDiffuse = totalDiffuse + vec4(diffuseFactor * lightColours[i], 1.0);

        // Specular lighting calculation
        float specularFactor = dot(reflectedLightDirection, unitToCamera);
        specularFactor = max(specularFactor, 0.0);
        specularFactor = pow(specularFactor, shineDamper) / attenuation;
        totalSpecular = totalSpecular + vec4(specularFactor * reflectivity * lightColours[i], 1.0);
    }

    // Ambient lighting calculation
    totalDiffuse = max(totalDiffuse, 0.05);

    out_Colour = totalDiffuse * colour + totalSpecular;
}