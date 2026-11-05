#version 450

layout(location = 0) out vec2 frag_tex_coord;

void main() {
    // Fullscreen triangle trick: 3 vertices cover the screen without a vertex buffer
    vec2 uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    gl_Position = vec4(uv * 2.0 - 1.0, 0.0, 1.0);
    frag_tex_coord = uv;
}
