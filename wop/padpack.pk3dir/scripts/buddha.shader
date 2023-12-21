models/mapobjects/buddha/buddha_g
{
	{
		map models/mapobjects/buddha/buddha_g.tga
		rgbGen identityLighting
	}
	{
		map models/mapobjects/buddha/enviromap.tga
		blendfunc filter
		rgbGen identityLighting
		tcGen environment
	}

	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

models/mapobjects/buddha/buddha_j
{
	{
		map models/mapobjects/buddha/buddha_j.tga
		rgbGen identityLighting
	}
	{
		map models/mapobjects/buddha/enviromap2.tga
		blendfunc filter
		rgbGen identityLighting
		tcGen environment
	}

	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

models/mapobjects/buddha/buddha_m
{
	surfaceparm alphashadow

	{
		map models/mapobjects/buddha/effect.tga
		blendfunc add
		tcGen environment
		tcMod scroll .3 1
		tcMod rotate 100
		rgbGen wave sin .5 1 0 .3
	}
	{
		map models/mapobjects/buddha/buddha_m.tga
		blendfunc blend
		rgbGen wave sin 0 1 0 .3
		alphaGen wave sin 0 1 0 .3
	}
}

models/mapobjects/buddha/buddha_f
{
	deformVertexes wave 100 sin 0 .7 0 .2
	//cull disable
	{
		map models/mapobjects/buddha/buddha_d.tga
		rgbGen vertex
	}
	{
		map models/mapobjects/buddha/buddha_f.tga
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen	wave sin -.5 1.5 1 .1
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
}
