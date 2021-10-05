#version 410 core
layout (location = 0) out vec4 out_colour;

in vec2  o_line_begin;
in vec2  o_line_end;
in vec4  o_line_begin_colour;
in vec4  o_line_end_colour;
in float o_line_thickness;

uniform float u_width;
uniform float u_height;

float cross2d(vec2 a, vec2 b)
{
    return a.x * b.y - b.x * a.y;
}

void main()
{   
    vec2 pixel = vec2(gl_FragCoord.x, u_height - gl_FragCoord.y);

    vec2 A = o_line_end - o_line_begin;
    vec2 B = pixel - o_line_begin;
    float lengthA = length(A);

    float distance_from_line = abs(cross2d(A, B)) / lengthA;

    float ratio_along = dot(A, B) / (lengthA * lengthA);

    if (distance_from_line < o_line_thickness) {
        if (ratio_along > 0 && ratio_along < 1) {
            out_colour = mix(o_line_begin_colour, o_line_end_colour, ratio_along);
        } else if (ratio_along <= 0 && distance(o_line_begin, pixel) < o_line_thickness) {
            out_colour = o_line_begin_colour;
        } else if (ratio_along >= 1 && distance(o_line_end, pixel) < o_line_thickness) {
            out_colour = o_line_end_colour;
        } else {
            out_colour = vec4(0.0);
        }
    }
}