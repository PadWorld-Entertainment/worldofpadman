//pad_teleporter/shader for Padmod by SLoB
//models/mapobjects/pad_teleporter/newpadtp.md3

models/mapobjects/pad_teleporter/pad_baseglow
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/padlogo.tga
	{
		map models/mapobjects/pad_teleporter/padlogo.tga
		rgbGen lightingDiffuse
	}
	{
		map models/mapobjects/pad_teleporter/pad_baseglow.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/mapobjects/pad_teleporter/padlogo.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35
		
	}
	{
		map models/mapobjects/pad_teleporter/pad_baseglow.tga
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
	q3map_lightimage models/mapobjects/pad_teleporter/padsphereor.tga
	q3map_surfacelight 200

	cull disable 
    	{
		map models/mapobjects/pad_teleporter/padsphereor.tga
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2	
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapsphere01.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapsphere02.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
       	}


}


models/mapobjects/pad_teleporter/padsphere_blue
{
	{
		map textures/pad_gfx02/tinpad2c.tga
                //map textures/sfx/specular.tga
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
	q3map_lightimage models/mapobjects/pad_teleporter/padspherepu.tga
	q3map_surfacelight 200

	cull disable 
    	{
		map models/mapobjects/pad_teleporter/padspherepu.tga
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2	
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherepu01.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherepu02.jpg
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
	q3map_lightimage models/mapobjects/pad_teleporter/padspherepi.tga
	q3map_surfacelight 200

	cull disable 
    	{
		map models/mapobjects/pad_teleporter/padspherepi.tga
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2	
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherepi01.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherepi02.jpg
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
	q3map_lightimage models/mapobjects/pad_teleporter/padsphereyel.tga
	q3map_surfacelight 200

	cull disable 
    	{
		map models/mapobjects/pad_teleporter/padsphereyel.tga
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2	
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapsphereyel01.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapsphereyel02.jpg
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
	qer_editorimage models/mapobjects/pad_teleporter/pad_base.tga
	{
		map models/mapobjects/pad_teleporter/pad_base.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_teleporter/snow.tga
		blendfunc add
		tcMod scroll -.25 0
	}
}

models/mapobjects/pad_teleporter/pad_teleporter_base
{

	{
		map textures/pad_gfx02/padmapyel3.tga
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
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}