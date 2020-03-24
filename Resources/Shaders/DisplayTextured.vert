
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 transform;
uniform mat4 projection;

out vec2 pass_textureCoord;
out vec2 pass_position;
out float width;
out float height;

void main()
{
    vec2 dimensions = (transform * vec4(1.0, 1.0, 0.0, 0.0)).xy;
    width = dimensions.x;
    height = dimensions.y;

    gl_Position = projection * transform * vec4(position, 0.0, 1.0);
    pass_textureCoord = textureCoord;
    pass_position = (transform * vec4(position, 0.0, 0.0)).xy;
}