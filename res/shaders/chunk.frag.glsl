#version 430 core

out vec4 frag_color;

in vec3 o_pos;
in vec3 o_normal;
in vec2 o_atlas_offset;

uniform sampler2D u_sampler;

vec3 light_dir = vec3(0.5, 1.0, 0.0);
vec3 ambient = vec3(0.5, 0.5, 0.5);

void main()
{
    vec2 uv = vec2(dot(o_normal.xzy, o_pos.zxx), o_pos.y + o_normal.y * o_pos.z);

    uv.y = 1.0 - uv.y;
    vec2 final_uv = fract(uv) * vec2(16.0 / 256.0, 16.0 / 256.0) + o_atlas_offset / 16;

    vec3 object_color = texture(u_sampler, final_uv).xyz;
    float diff = max(dot(normalize(light_dir), o_normal), 0.0);

    frag_color = vec4((ambient + vec3(diff)) * object_color, 1.0);
}