models/weapons2/spraypistol/f_spraypistolflash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/spraypistol/f_spraypistolflash.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/spraypistol/f_spraypistolflash.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}


models/weapons2/spraypistol/spraypistol
{
	qer_editorimage models/weapons2/spraypistol/spraypistol.tga

	nopicmip
	nomipmaps
	{
		map textures/pad_gfx02/tinpad2d.tga
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity
		
	}
	{
		map models/weapons2/spraypistol/spraypistol.tga
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
}


models/weapons2/spraypistol/cartridge_red
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map textures/pad_gfx02/padmapyel2.tga
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen lightingdiffuse
		
        }
	{
		map models/weapons2/spraypistol/padcart_r.tga
		alphaFunc ge128
		rgbGen lightingdiffuse

  	}
}


models/weapons2/spraypistol/cartridge_blue
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map textures/pad_gfx02/padmapyel2.tga
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen lightingdiffuse
		
        }
	{
		map models/weapons2/spraypistol/padcart_b.tga
		alphaFunc ge128
		rgbGen lightingdiffuse

  	}
}


models/weapons2/spraypistol/cartridge_neutral
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map textures/pad_gfx02/padmapyel2.tga
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen lightingdiffuse
		
        }
	{
		map models/weapons2/spraypistol/padcart_n.tga
		alphaFunc ge128
		rgbGen lightingdiffuse

  	}
}

