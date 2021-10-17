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

const float pi = 3.1415926535897932384626433832795;

// Rotates the given vec2 anti-clockwise by the given amount of radians.
vec2 rot(vec2 v, float rad)
{
    float s = sin(rad);
	float c = cos(rad);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

// Returns 0 for vectors on positive x axis, 0.5, for negative x axis, increasing
// rotating clockwise.
float rotation_value(vec2 v)
{
    if (v.y >= 0) { // Top right
        float cos_theta = normalize(-v).x;
        float theta = acos(cos_theta) + pi;
        return theta / (2 * pi);
    }
    else {
        float cos_theta = normalize(v).x;
        float theta = acos(cos_theta);
        return theta / (2 * pi);
    }
}

void main()
{   
    vec2 pixel = vec2(gl_FragCoord.x, u_height - gl_FragCoord.y);
    vec2 to_pixel = pixel - o_circle_centre;
    float from_centre = length(to_pixel);
    
    if (o_circle_inner_radius < from_centre && from_centre < o_circle_outer_radius) {
        float angle_offset = o_circle_angle / (2 * pi);
        out_colour = mix(
            o_circle_begin_colour,
            o_circle_end_colour,
            rotation_value(rot(to_pixel, -o_circle_angle))
        );
        return;
    }
}