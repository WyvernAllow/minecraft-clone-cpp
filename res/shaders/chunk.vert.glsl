#version 430 core
layout(location=0) in vec3 a_pos;
layout(location=1) in vec3 a_normal;
layout(location=2) in vec2 a_atlas_offset;

out vec3 o_pos;
out vec3 o_normal;
out vec2 o_atlas_offset;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    gl_Position = u_proj * u_view * vec4(a_pos, 1.0);
    
    o_normal = a_normal;
    o_pos = a_pos;
    o_atlas_offset = a_atlas_offset;
}