// ioQuake3-U -- GX2 ubershader, vertex stage.
//
// Compiled at runtime by CafeGLSL (glslcompiler.rpl). CafeGLSL requires
// modern GLSL with ALL locations/bindings explicit (see its README), and has
// NO uniform-register support: uniforms must live in explicit uniform blocks
// and the app must bind with GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK).
// Uniform block data is fetched raw from memory by the GPU, so the app
// byte-swaps each 32-bit word to little-endian before upload (same as the
// WiiU-GX2-Shader-Examples texture sample does with _swapF32).
//
// One ubershader pair serves every ioq3 draw; per-draw behavior is selected
// by u_Mode in the fragment stage.
#version 450

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord0;
layout(location = 2) in vec2 in_TexCoord1;
layout(location = 3) in vec4 in_Color;

layout(binding = 0) uniform VSBlock
{
	mat4 u_ModelViewProjection;
};

layout(location = 0) out vec2 v_TexCoord0;
layout(location = 1) out vec2 v_TexCoord1;
layout(location = 2) out vec4 v_Color;

void main()
{
	gl_Position = u_ModelViewProjection * vec4(in_Position, 1.0);
	v_TexCoord0 = in_TexCoord0;
	v_TexCoord1 = in_TexCoord1;
	v_Color = in_Color;
}
