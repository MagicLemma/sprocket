#version 410 core
layout (location = 0) out vec4 out_colour;

in vec2  o_line_begin;
in vec2  o_line_end;
in vec4  o_line_colour;
in float o_line_thickness;

uniform float u_width;
uniform float u_height;

void main()
{    
    out_colour = vec4(1.0, 0.0, 0.0, 0.5);
}