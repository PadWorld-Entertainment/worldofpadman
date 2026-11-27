// ioQuake3-U -- GX2 ubershader, fragment stage. See q3uber_vert.glsl for the
// CafeGLSL constraints (explicit bindings, uniform blocks only).
//
// u_Mode selects behavior per draw. Components are FLOATS (0.0/1.0/...)
// compared against 0.5-style thresholds -- float uniforms through the
// uniform block are the bulletproof path; integer uniform handling in the
// experimental CafeGLSL is an avoidable risk.
//   x tex0On     -- sample s_Base into the color, else vec4(1)
//   y tex1On     -- sample s_Lightmap and combine per tex1Blend
//   z tex1Blend  -- 0 = modulate (c *= lm), 1 = add (c += lm)
//   w atestMode  -- 0 none, 1 discard a<=0, 2 discard a>=0.5,
//                   3 discard a<u_AlphaRef.x
// Alpha test is done with `discard` because R700/Latte has no
// fixed-function alpha test stage.
#version 450

layout(location = 0) in vec2 v_TexCoord0;
layout(location = 1) in vec2 v_TexCoord1;
layout(location = 2) in vec4 v_Color;

layout(binding = 0) uniform PSBlock
{
	vec4 u_Mode;
	vec4 u_AlphaRef;
};

// Set once per frame (not per-draw like PSBlock above), independent of the
// world/entity/2D pass -- this is the r_gamma brightness curve normally
// supplied by a hardware gamma ramp, which Wii U/GX2 has no API for (see
// [[wiiu-gx2-missing-gamma-correction]] project memory). x = 1.0/r_gamma.
layout(binding = 1) uniform GammaBlock
{
	vec4 u_InvGamma;
};

layout(binding = 0) uniform sampler2D s_Base;
layout(binding = 1) uniform sampler2D s_Lightmap;

layout(location = 0) out vec4 out_Color;

void main()
{
	vec4 color = v_Color;

	if (u_Mode.x > 0.5) {
		color *= texture(s_Base, v_TexCoord0);
	}

	if (u_Mode.y > 0.5) {
		vec4 lm = texture(s_Lightmap, v_TexCoord1);
		if (u_Mode.z > 0.5) {
			color += lm;
		} else {
			color *= lm;
		}
	}

	if (u_Mode.w > 0.5 && u_Mode.w < 1.5) {
		if (color.a <= 0.0) discard;
	} else if (u_Mode.w > 1.5 && u_Mode.w < 2.5) {
		if (color.a >= 0.5) discard;
	} else if (u_Mode.w > 2.5) {
		if (color.a < u_AlphaRef.x) discard;
	}

	color.rgb = pow(color.rgb, vec3(u_InvGamma.x));

	out_Color = color;
}
