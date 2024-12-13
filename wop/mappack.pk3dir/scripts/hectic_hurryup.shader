textures/hectic_hurryup/blacksky
{
	qer_editorimage textures/colors/black.jpg
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/colors/white.jpg
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 100

        {
		map textures/colors/black.jpg
	}
}

textures/hectic_hurryup/darksky
{
	qer_editorimage textures/colors/black.jpg
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky

        {
		map textures/colors/black.jpg
	}
}

textures/hectic_hurryup/void
{
	qer_editorimage textures/pad_jail/pad_jail_dunkelheit.tga
	surfaceparm	trans
	surfaceparm	nonsolid
	surfaceparm	fog
	surfaceparm	nolightmap

	fogparms ( .0 .0 .0 ) 1000
}

textures/hectic_hurryup/portal
{
	qer_editorimage textures/common/qer_portal.tga
	q3map_lightimage textures/effects/envmapora.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	//q3map_surfacelight 500
	portal
	{
		map textures/effects/envmapora.tga
		blendfunc blend
		rgbGen identityLighting
		depthWrite
		alphaGen portal 700
	}
	{
		map textures/effects/envmapora.tga
		blendfunc add
		tcgen environment
	}
}

textures/hectic_hurryup/randlicht
{
    qer_editorimage textures/colors/white.jpg
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/colors/white.jpg
    }
}
