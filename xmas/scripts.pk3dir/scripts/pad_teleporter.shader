//pad_teleporter/shader for Padmod by SLoB
//models/mapobjects/pad_teleporter/newpadtp.md3

models/mapobjects/pad_teleporter/pad_baseglow
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/padlogo
	{
		map models/mapobjects/pad_teleporter/padlogo
		rgbGen lightingDiffuse
	}
	{
		map models/mapobjects/pad_teleporter/pad_baseglow
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/mapobjects/pad_teleporter/padlogo
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
	{
		map models/mapobjects/pad_teleporter/pad_baseglow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
}

models/mapobjects/pad_teleporter/padsphere_orange
{
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padsphereor
	q3map_surfacelight 200

	cull disable
    	{
		map models/mapobjects/pad_teleporter/padsphereor
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapsphere01
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapsphere02
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
       	}


}


models/mapobjects/pad_teleporter/padsphere_blue
{
	{
		map textures/pad_gfx02/tinpad2c
                //map textures/sfx/specular
		blendfunc GL_ONE GL_ONE
		tcGen environment
	}
}


models/mapobjects/pad_teleporter/padsphere_purple
{
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padspherepu
	q3map_surfacelight 200

	cull disable
    	{
		map models/mapobjects/pad_teleporter/padspherepu
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherepu01
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherepu02
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
       	}


}


models/mapobjects/pad_teleporter/padsphere_pink
{
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padspherepi
	q3map_surfacelight 200

	cull disable
    	{
		map models/mapobjects/pad_teleporter/padspherepi
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherepi01
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherepi02
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
       	}


}


models/mapobjects/pad_teleporter/padsphere_yellow
{
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padsphereyel
	q3map_surfacelight 200

	cull disable
    	{
		map models/mapobjects/pad_teleporter/padsphereyel
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapsphereyel01
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapsphereyel02
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
       	}


}

models/mapobjects/pad_teleporter/pad_base
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_base
	{
		map models/mapobjects/pad_teleporter/pad_base
		alphaFunc ge128
		rgbGen lightingDiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}



}


models/mapobjects/pad_teleporter/snow
{
	cull none
	{
		map models/mapobjects/pad_teleporter/snow
		blendfunc add
		tcMod scroll -.25 0
	}
}

models/mapobjects/pad_teleporter/pad_teleporter_base
{

	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_base
		rgbGen lightingdiffuse
		alphaFunc Ge128
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}