#version 410 core
layout (location = 0) in vec2 position;

layout (location = 1) in vec2  circle_centre;
layout (location = 2) in float circle_inner_radius;
layout (location = 3) in float circle_outer_radius;
layout (location = 4) in vec4  circle_begin_colour;
layout (location = 5) in vec4  circle_end_colour;
layout (location = 6) in float circle_angle;

out vec2  o_circle_centre;
out float o_circle_inner_radius;
out float o_circle_outer_radius;
out vec4  o_circle_begin_colour;
out vec4  o_circle_end_colour;
out float o_circle_angle;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    
    o_circle_centre = circle_centre;
    o_circle_inner_radius = circle_inner_radius;
    o_circle_outer_radius = circle_outer_radius;
    o_circle_begin_colour = circle_begin_colour;
    o_circle_end_colour = circle_end_colour;
    o_circle_angle = circle_angle;
} 