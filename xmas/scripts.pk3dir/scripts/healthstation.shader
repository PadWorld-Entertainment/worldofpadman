models/mapobjects/pad_healthstation/HS_ring

{
	cull none
	
	{
		map textures/pad_gfx02/padmapyel3.tga
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity
		
	}
	
	{
		map models/mapobjects/pad_healthstation/HS_ring.tga
		rgbGen lightingDiffuse
		alphaFunc Ge128
	}
}

models/mapobjects/pad_healthstation/HS_base

{
	cull none
	
	{
		map textures/pad_gfx02/padmapyel3.tga
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity
		
	}
	
	{
		map models/mapobjects/pad_healthstation/HS_base.tga
		rgbGen lightingDiffuse
		alphaFunc Ge128
	}
}


models/mapobjects/pad_healthstation/star

{
	
	{
	map models/mapobjects/pad_healthstation/star.tga
	rgbGen identity
	}
}