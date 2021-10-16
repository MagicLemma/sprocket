#version 410 core
layout (location = 0) out vec4 out_colour;

in vec2  o_circle_centre;
in float o_circle_inner_radius;
in float o_circle_outer_radius;
in vec4  o_circle_begin_colour;
in vec4  o_circle_end_colour;
in float o_circle_angle;

uniform float u_width;
uniform float u_height;

void main()
{   
    vec2 pixel = vec2(gl_FragCoord.x, u_height - gl_FragCoord.y);
    
    if (distance(pixel, o_circle_centre) < o_circle_outer_radius) {
        out_colour = o_circle_begin_colour;
        return;
    }
}