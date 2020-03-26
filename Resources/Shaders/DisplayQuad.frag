#version 400 core
out vec4 out_Colour;

uniform sampler2D textureSampler;
uniform vec3 colour;
uniform float opacity;
uniform float roundness;
uniform float greyscale;

in vec2 pass_position;
in vec2 pass_textureCoord;
in float width;
in float height;

float distSquare(vec2 p1, vec2 p2) {
    return dot(p1 - p2, p1 - p2);
}

float getAlpha(float r) {

    if ((pass_position.x < r) && (pass_position.y < r)) {
        if (distSquare(pass_position, vec2(r, r)) > r*r) {
            return 0.0;
        }
    }
    
    if ((pass_position.x < r) && (pass_position.y > height - r)) {
        if (distSquare(pass_position, vec2(r, height - r)) > r*r) {
            return 0.0;
        }
    }

    if ((pass_position.x > width - r) && (pass_position.y < r)) {
        if (distSquare(pass_position, vec2(width - r, r)) > r*r) {
            return 0.0;
        }
    }
    
    if ((pass_position.x > width - r) && (pass_position.y > height - r)) {
        if (distSquare(pass_position, vec2(width - r, height - r)) > r*r) {
            return 0.0;
        }
    }

    return 1.0;
}

void main()
{
    float maxRoundness = min(width/2, height/2);
    float r = roundness * maxRoundness;

    float alpha = getAlpha(r);

    vec3 finalColour = colour * texture(textureSampler, pass_textureCoord).xyz;
    if (greyscale != 0) {
        float grey = finalColour.r * 0.3 + finalColour.g * 0.59 + finalColour.b * 0.11;
        finalColour = vec3(grey, grey, grey);
    }
    out_Colour = vec4(finalColour, opacity * alpha);
}