#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float opacity;

void main()
{
    FragColor = vec4(texture(ourTexture, TexCoord).rgb, opacity);;
}