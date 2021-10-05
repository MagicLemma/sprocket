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

    vec2 line = o_line_end - o_line_begin;
    vec2 to_pixel = pixel - o_line_begin;
    float angle = dot(line, to_pixel) / (length(line) * length(to_pixel));

    float distance_from_line = length(to_pixel) * sin(acos(angle));

    if (distance(pixel, o_line_begin) < 10) {
        out_colour = vec4(0.0, 0.0, 1.0, 1.0);
    } else if (distance(pixel, o_line_end) < 10) {
        out_colour = vec4(1.0, 0.0, 0.0, 1.0);
    } else if (distance_from_line < 5) {
        out_colour = vec4(0.0, 1.0, 0.0, 1.0);
    }
}