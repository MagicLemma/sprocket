#version 400 core

in vec2 d_texture;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

layout(location = 0) out vec4 out_Colour;

uniform sampler2D textureSampler;

// Lighting Information
uniform vec3 lightColour;
uniform vec3 lightAttenuation;

// Texture/Lighting Information
uniform float shineDamper;
uniform float reflectivity;

void main()
{
    // Surface information
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToLight = normalize(toLightVector);
    vec3 unitToCamera = normalize(toCameraVector);
    vec3 reflectedLightDirection = reflect(-unitToLight, unitNormal);

    // Colour prior to lighting
    vec4 colour = texture(textureSampler, d_texture);

    // Attenuation calculation
    float d = length(toLightVector);
    float attenuation = lightAttenuation.x + lightAttenuation.y * d + lightAttenuation.z * d * d;

    // Diffuse lighting calculation
    float diffuseFactor = dot(unitToLight, unitNormal);
    diffuseFactor = max(diffuseFactor, 0.05);
    vec4 diffuseLight = vec4(diffuseFactor * lightColour, 1.0);

    // Specular lighting calculation
    float specularFactor = dot(reflectedLightDirection, unitToCamera);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shineDamper);
    vec4 specularLight = vec4(specularFactor * reflectivity * lightColour, 1.0);

    out_Colour = (diffuseLight * colour + specularLight) / attenuation;
}