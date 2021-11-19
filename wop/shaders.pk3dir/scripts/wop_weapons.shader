// =================
// BOASTER
// =================

models/weapons2/boaster/boaster
{
	qer_editorimage models/weapons2/boaster/boaster.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/boaster/tubew.tga
		blendfunc GL_ONE GL_ZERO
		tcMod scroll .07 .0
		rgbGen lightingdiffuse
		
	}
	{
		map models/weapons2/boaster/boaster.tga
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

models/weapons2/boaster/boaster_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/boaster/f_boaster.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/boaster/f_boaster.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

// =================
// BETTY
// =================

models/weapons2/betty/betty_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/betty/f_betty.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/betty/f_betty.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

models/weapons2/betty/betty
{
	nopicmip
	nomipmaps
	
	{
	        map models/weapons2/betty/fir02.tga
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1         
                rgbGen lightingdiffuse
        }	
        {
                map models/weapons2/betty/fir01.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09         
                rgbGen lightingdiffuse
		
        }
	
	{
		map models/weapons2/betty/betty.tga
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

// =================
// NiPPER
// =================

models/weapons2/nipper/f_nipper
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/nipper/f_nipper.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/nipper/f_nipper.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

models/weapons2/nipper/nipper2_trans
{
	nopicmip
	nomipmaps
	
	{
	        map models/weapons2/nipper/water01.tga
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1         
                rgbGen lightingdiffuse
        }	
        {
                map models/weapons2/nipper/water02.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09         
                rgbGen lightingdiffuse
		
        }
	
	{
		map models/weapons2/nipper/nipper2_trans.tga
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

models/weapons2/nipper/nipper
{
	nopicmip
	nomipmaps
	
	{
	        map models/weapons2/nipper/water01.tga
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1         
                rgbGen lightingdiffuse
        }	
        {
                map models/weapons2/nipper/water02.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09         
                rgbGen lightingdiffuse
		
        }
	
	{
		map models/weapons2/nipper/nipper.tga
		//alphaFunc ge128
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

// =================
// PUMPER
// =================

models/weapons2/pumper/pumper
{
	qer_editorimage models/weapons2/pumper/pumper.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/pumper/pumper.tga
		//alphaFunc ge128
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

models/weapons2/pumper/tubes
{
	qer_editorimage models/weapons2/pumper/tubes.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/pumper/tubes.tga
		//alphaFunc ge128
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


models/weapons2/pumper/f_pumper
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/pumper/f_pumper.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/pumper/f_pumper.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

// =================
// SPLASHER
// =================

models/weapons2/splasher/splasher
{
	qer_editorimage models/weapons2/splasher/splasher.tga

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
		map models/weapons2/splasher/splasher.tga
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

models/weapons2/splasher/new_frontface
{
	qer_editorimage models/weapons2/splasher/new_frontface.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/splasher/new_frontface.tga
		//alphaFunc ge128
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

models/weapons2/splasher/splasher_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/splasher/f_splasher.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/splasher/f_splasher.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

models/weapons2/splasher/splasherwater
{	                 
	{
		
                map textures/pad_gfx02/padmapplas.jpg
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .6 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1         
                //rgbGen lightingdiffuse //identity             
        }	
        {
                map textures/pad_gfx02/padmapblue2.jpg
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .7 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09         
                //rgbGen lightingdiffuse
        }	
        {
		map textures/pad_gfx02/padmapblue.jpg
                blendFunc GL_ONE GL_ONE
		tcGen environment
		tcMod scroll -.3 0
		//rgbGen lightingDiffuse
	}     	
}



// =================
// BUBBLE G.
// =================

models/weapons2/bubbleg/bubbleg_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/bubbleg/f_bubbleg.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/bubbleg/f_bubbleg.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}


models/weapons2/bubbleg/bubbleg
{
	nopicmip
	nomipmaps
	
	{
	        map models/weapons2/bubbleg/watbub02.tga
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .2 0 .2

                tcMod scroll .09 -.1.1         
                rgbGen lightingdiffuse
        }	
        {
                map models/weapons2/bubbleg/watbub.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .1 0 .3

                tcMod scroll .1 .09         
                rgbGen lightingdiffuse
		
        }
	
	{
		map models/weapons2/bubbleg/bubbleg.tga
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

// =================
// BALLOONY
// =================

models/weapons2/balloony/balloony
{
	qer_editorimage models/weapons2/balloony/balloony.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/balloony/balloony.tga
		//alphaFunc ge128
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

models/weapons2/balloony/stripemove
{
	qer_editorimage models/weapons2/balloony/stripemove_l2.tga

	nopicmip
	nomipmaps

	{
		map models/weapons2/balloony/stripemove.tga
		//blendfunc add
		tcMod scroll .6 .0
		rgbGen lightingdiffuse
		
	}
	{
		map models/weapons2/balloony/stripemove_l2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen lightingdiffuse
		
	}
	
}

models/weapons2/balloony/tubemove
{
	nopicmip
	nomipmaps
	qer_editorimage models/weapons2/balloony/balloony.tga

	deformVertexes bulge .11 .08 3
	
	{
		map models/weapons2/balloony/balloony.tga
		rgbGen lightingdiffuse
		
	}
	
}


models/weapons2/balloony/balloony_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/balloony/f_balloony.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/balloony/f_balloony.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}


// =================
// iMPERiUS
// =================

models/weapons2/imperius/imperius_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/imperius/f_imperius.jpg
		blendfunc GL_ONE GL_ONE         
                tcmod rotate 360              
        }
        {
		clampmap models/weapons2/imperius/f_imperius.jpg
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }
     
}

models/weapons2/imperius/imperius
{
	nopicmip
	nomipmaps
	
	{
	        map models/weapons2/imperius/watpurp02.tga
		blendfunc GL_ONE GL_ZERO                   
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1         
                rgbGen lightingdiffuse
        }	
        {
                map models/weapons2/imperius/watpurp.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09         
                rgbGen lightingdiffuse
		
        }
	
	{
		map models/weapons2/imperius/imperius.tga
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


// =================
// PUNCHY
// =================

models/weapons2/punchy/punchy_teeth
{
	nopicmip
	nomipmaps
	cull none
	qer_editorimage models/weapons2/punchy/teeth.tga
	{
                map models/weapons2/punchy/teeth.tga
		alphaFunc GE128
		depthWrite
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/invispad.jpg
		blendfunc GL_ONE GL_ONE
		tcmod scale .3 .3
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/punchy/punchy
{
	nopicmip
	nomipmaps
	
	{
                map models/weapons2/punchy/punchy.tga
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/invispad.jpg
		blendfunc GL_ONE GL_ONE
		tcmod scale .3 .3
		tcGen environment
              	rgbGen lightingdiffuse
	}
}

models/weapons2/punchy/eyes
{
	nopicmip
	nomipmaps
	
	{
                map models/weapons2/punchy/eyes.tga
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .3 .3
		tcGen environment
                rgbGen lightingdiffuse
	}
}


// =================
// SPRAYPiSTOL
// =================

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
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map models/weapons2/spraypistol/redpaint.jpg
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity
		
        }
	{
		map models/weapons2/spraypistol/padcart_r.tga
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/spraypistol/cartridge_blue
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map models/weapons2/spraypistol/bluepaint.jpg
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity
		
        }
	{
		map models/weapons2/spraypistol/padcart_b.tga
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/spraypistol/cartridge_neutral
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map models/weapons2/spraypistol/greenpaint.jpg
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity
		
        }
	{
		map models/weapons2/spraypistol/padcart_n.tga
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

// =================
// KMA97
// =================

models/weapons2/kma97/kma97
{
	qer_editorimage models/weapons2/kma97/kma97.tga

	nopicmip
	nomipmaps
	{
		map models/weapons2/kma97/kma97.tga
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

