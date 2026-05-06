#version 450

layout(set = 0, binding = 0) uniform sampler2D u_TextureMap;
layout(push_constant) uniform PushConstants {
    float greyscale;
} pc;

layout(location = 0) in vec2 frag_tex_coord;
layout(location = 0) out vec4 out_color;

const vec3 LUMA = vec3(0.2125, 0.7154, 0.0721);

void main() {
    vec4 color = texture(u_TextureMap, frag_tex_coord);
    float y = dot(color.rgb, LUMA);
    color.rgb = mix(color.rgb, vec3(y), pc.greyscale);
    out_color = color;
}
