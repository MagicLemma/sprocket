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
    vec2 pixel = vec2(gl_FragCoord.x, u_height - gl_FragCoord.y);

    if (distance(pixel, o_line_begin) < 10) {
        out_colour = vec4(0.0, 0.0, 1.0, 1.0);
    } else if (distance(pixel, o_line_end) < 10) {
        out_colour = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        out_colour = vec4(0.0);;
    }
}