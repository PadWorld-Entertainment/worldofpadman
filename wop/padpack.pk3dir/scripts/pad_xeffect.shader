textures/pad_garden/neon01
{
	qer_editorimage textures/pad_enteflare/neon01
	surfaceparm nomarks
	q3map_flareShader flareShader
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01_blend
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_crytek/yb2
{
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/yb2
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_effect/skinwin
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/skinwin
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/skinwin
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_effect/blacky
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/blacky
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/blacky
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_effect/blatt
{
	cull disable
	surfaceparm alphashadow
//	deformVertexes autoSprite
	surfaceparm trans
	{
		map textures/pad_effect/blatt
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}
}


textures/pad_effect/colabox
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/colabox
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/colabox
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}



textures/pad_effect/blacky01
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/blacky01
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/blacky01
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}
