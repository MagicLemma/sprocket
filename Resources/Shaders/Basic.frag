#version 400 core

in vec2 d_texture;
in vec3 surfaceNormal;
in vec3 toLightVector;

layout(location = 0) out vec4 out_Colour;

uniform sampler2D textureSampler;
uniform vec3 lightColour;

void main()
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToLight = normalize(toLightVector);

    float dotProd = dot(unitToLight, unitNormal);
    float brightness = max(dotProd, 0.2);

    vec3 diffuse = brightness * lightColour;

    out_Colour = vec4(diffuse, 1.0) * texture(textureSampler, d_texture);
}