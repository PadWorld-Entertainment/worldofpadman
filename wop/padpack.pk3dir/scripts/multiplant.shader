models/mapobjects/multiplant/fern
{
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map models/mapobjects/multiplant/fern.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/bannanaleaf
{
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map models/mapobjects/multiplant/bannanaleaf.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
//		depthFunc equal
	}
}

models/mapobjects/multiplant/batleaf
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/batleaf.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}


	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/batleaf2
{
	surfaceparm alphashadow
	cull none
	// nopicmip
	// tesssize 32
	deformVertexes wave 100 sin 3 5 .1 0.1

	{
		map models/mapobjects/multiplant/batleaf2.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
	{
		map models/mapobjects/multiplant/batleaf2veins.tga
//		blendFunc GL_ONE GL_ONE
		blendFunc blend
		rgbGen wave sin 0 1 0 .33
//		rgbGen identity
	}


	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/leaf1
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/leaf1.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/leaf2
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/leaf2.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/leaf3
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/leaf3.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/pleaf1
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/pleaf1.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/pleaf2
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/pleaf2.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/pleaf3
{
	surfaceparm alphashadow
	cull none

	{
		map models/mapobjects/multiplant/pleaf3.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}

models/mapobjects/multiplant/palmfrond
{
	surfaceparm alphashadow
	cull none
	nopicmip
	{
		map models/mapobjects/multiplant/palmfrond.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}
models/mapobjects/multiplant/invisible
{
	surfaceparm alphashadow
	surfaceparm nolightmap
	{
		map models/mapobjects/multiplant/invisible.tga
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
}
