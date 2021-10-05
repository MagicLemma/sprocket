#version 410 core
layout (location = 0) in vec2 position;

layout (location = 1) in vec2  line_begin;
layout (location = 2) in vec2  line_end;
layout (location = 3) in vec4  line_begin_colour;
layout (location = 4) in vec4  line_end_colour;
layout (location = 5) in float line_thickness;

out vec2  o_line_begin;
out vec2  o_line_end;
out vec4  o_line_begin_colour;
out vec4  o_line_end_colour;
out float o_line_thickness;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);

    o_line_begin = line_begin;
    o_line_end = line_end;
    o_line_begin_colour = line_begin_colour;
    o_line_end_colour = line_end_colour;
    o_line_thickness = line_thickness;
} 