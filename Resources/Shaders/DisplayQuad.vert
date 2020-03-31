#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 vertexColour;

uniform mat4 transform;
uniform mat4 projection;

uniform vec3 colour;

out vec2 pass_position;
out vec2 pass_textureCoord;
out vec3 pass_vertexColour;
out float width;
out float height;

void main()
{
    vec2 dimensions = (transform * vec4(1.0, 1.0, 0.0, 0.0)).xy;
    width = dimensions.x;
    height = dimensions.y;

    gl_Position = projection * transform * vec4(position, 0.0, 1.0);
    pass_position = (transform * vec4(position, 0.0, 0.0)).xy;
    pass_textureCoord = textureCoord;

    if (vertexColour.r >= 0) {
        pass_vertexColour = vertexColour;
    }
    else {
        pass_vertexColour = colour;
    }
}