// =================
// BOASTER
// =================

models/weapons2/boaster/boaster
{
	qer_editorimage models/weapons2/boaster/boaster

	nopicmip
	nomipmaps
	{
		map models/weapons2/boaster/tubew
		blendfunc GL_ONE GL_ZERO
		tcMod scroll .07 .0
		rgbGen lightingdiffuse

	}
	{
		map models/weapons2/boaster/boaster
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
}

models/weapons2/boaster/boaster_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/boaster/f_boaster
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/boaster/f_boaster
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
		clampmap models/weapons2/betty/f_betty
		blendfunc GL_ONE GL_ONE
		tcmod rotate 360
	}
	{
		clampmap models/weapons2/betty/f_betty
		blendfunc GL_ONE GL_ONE
		tcmod rotate -129
	}
}

bettyMuzzle_ft
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/betty/f_betty_ft
		blendfunc GL_ONE GL_ONE
		tcmod rotate 360
	}
	{
		clampmap models/weapons2/betty/f_betty_ft
		blendfunc GL_ONE GL_ONE
		tcmod rotate -129
	}
}

models/weapons2/betty/betty
{
	nopicmip
	nomipmaps
	{
		map models/weapons2/betty/fir02
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .2 0 .2
		tcmod scale .4 .4
		tcMod scroll .09 -.1.1
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/betty/fir01
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .1 0 .3
		tcmod scale .2 .2
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/betty/betty
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
}

bettySkin_ft
{
	nopicmip
	nomipmaps
	{
		map models/weapons2/betty/fir02_ft
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .2 0 .2
		tcmod scale .4 .4
		tcMod scroll .09 -.1.1
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/betty/fir01_ft
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .1 0 .3
		tcmod scale .2 .2
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/betty/betty
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
}

// =================
// NiPPER
// =================

models/weapons2/nipper/f_nipper
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/nipper/f_nipper
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/nipper/f_nipper
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
	        map models/weapons2/nipper/water01
		blendfunc GL_ONE GL_ZERO
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                rgbGen lightingdiffuse
        }
        {
                map models/weapons2/nipper/water02
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09
                rgbGen lightingdiffuse

        }

	{
		map models/weapons2/nipper/nipper2_trans
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

}

models/weapons2/nipper/nipper
{
	nopicmip
	nomipmaps

	{
	        map models/weapons2/nipper/water01
		blendfunc GL_ONE GL_ZERO
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                rgbGen lightingdiffuse
        }
        {
                map models/weapons2/nipper/water02
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09
                rgbGen lightingdiffuse

        }

	{
		map models/weapons2/nipper/nipper
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
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
	qer_editorimage models/weapons2/pumper/pumper

	nopicmip
	nomipmaps
	{
		map models/weapons2/pumper/pumper
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/pumper/tubes
{
	qer_editorimage models/weapons2/pumper/tubes

	nopicmip
	nomipmaps
	{
		map models/weapons2/pumper/tubes
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
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
		clampmap models/weapons2/pumper/f_pumper
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/pumper/f_pumper
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
	qer_editorimage models/weapons2/splasher/splasher

	nopicmip
	nomipmaps
	{
		map textures/pad_gfx02/tinpad2d
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
	{
		map models/weapons2/splasher/splasher
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
}

models/weapons2/splasher/new_frontface
{
	qer_editorimage models/weapons2/splasher/new_frontface

	nopicmip
	nomipmaps
	{
		map models/weapons2/splasher/new_frontface
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
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
		clampmap models/weapons2/splasher/f_splasher
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/splasher/f_splasher
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }

}

models/weapons2/splasher/splasherwater
{
	{

                map textures/pad_gfx02/padmapplas
		blendfunc GL_ONE GL_ZERO
                tcMod turb 0 .6 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                //rgbGen lightingdiffuse //identity
        }
        {
                map textures/pad_gfx02/padmapblue2
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .7 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09
                //rgbGen lightingdiffuse
        }
        {
		map textures/pad_gfx02/padmapblue
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
		clampmap models/weapons2/bubbleg/f_bubbleg
		blendfunc GL_ONE GL_ONE
		tcmod rotate 360
	}
	{
		clampmap models/weapons2/bubbleg/f_bubbleg
		blendfunc GL_ONE GL_ONE
		tcmod rotate -129
	}

}

bubblegMuzzle_ft
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/bubbleg/f_bubbleg_ft
		blendfunc GL_ONE GL_ONE
		tcmod rotate 360
	}
	{
		clampmap models/weapons2/bubbleg/f_bubbleg_ft
		blendfunc GL_ONE GL_ONE
		tcmod rotate -129
	}

}

models/weapons2/bubbleg/bubbleg
{
	nopicmip
	nomipmaps
	{
		map models/weapons2/bubbleg/bub02
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .2 0 .2
		tcMod scroll .09 -.1.1
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/bubbleg/bub01
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .1 0 .3
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/bubbleg/bubbleg
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
}

bubblegSkin_ft
{
	nopicmip
	nomipmaps
	{
		map models/weapons2/bubbleg/bub02_ft
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .2 0 .2
		tcMod scroll .09 -.1.1
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/bubbleg/bub01_ft
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .1 0 .3
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/bubbleg/bubbleg
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
}

// =================
// BALLOONY
// =================

models/weapons2/balloony/balloony
{
	qer_editorimage models/weapons2/balloony/balloony

	nopicmip
	nomipmaps
	{
		map models/weapons2/balloony/balloony
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/balloony/stripemove
{
	qer_editorimage models/weapons2/balloony/stripemove_l2

	nopicmip
	nomipmaps

	{
		map models/weapons2/balloony/stripemove
		//blendfunc add
		tcMod scroll .6 .0
		rgbGen lightingdiffuse

	}
	{
		map models/weapons2/balloony/stripemove_l2
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen lightingdiffuse

	}

}

models/weapons2/balloony/tubemove
{
	nopicmip
	nomipmaps
	qer_editorimage models/weapons2/balloony/balloony

	deformVertexes bulge .11 .08 3

	{
		map models/weapons2/balloony/balloony
		rgbGen lightingdiffuse

	}

}


models/weapons2/balloony/balloony_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/balloony/f_balloony
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/balloony/f_balloony
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
		clampmap models/weapons2/imperius/f_imperius
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/imperius/f_imperius
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
	        map models/weapons2/imperius/watpurp02
		blendfunc GL_ONE GL_ZERO
                tcMod turb 0 .2 0 .2
                tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                rgbGen lightingdiffuse
        }
        {
                map models/weapons2/imperius/watpurp
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09
                rgbGen lightingdiffuse

        }

	{
		map models/weapons2/imperius/imperius
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

}


// =================
// PUNCHY
// =================

models/weapons2/punchy/punchy_teeth
{
	nopicmip
	nomipmaps
	cull none
	qer_editorimage models/weapons2/punchy/teeth
	{
                map models/weapons2/punchy/teeth
		alphaFunc GE128
		depthWrite
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/invispad
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
                map models/weapons2/punchy/punchy
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/invispad
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
                map models/weapons2/punchy/eyes
		rgbGen lightingdiffuse
        }
	{
		map textures/pad_gfx02/tinpad3
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
		clampmap models/weapons2/spraypistol/f_spraypistolflash
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
        }
        {
		clampmap models/weapons2/spraypistol/f_spraypistolflash
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
                //tcMod stretch sin .8 0.10 0 .7
        }

}


models/weapons2/spraypistol/spraypistol
{
	qer_editorimage models/weapons2/spraypistol/spraypistol

	nopicmip
	nomipmaps
	{
		map textures/pad_gfx02/tinpad2d
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
	{
		map models/weapons2/spraypistol/spraypistol
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
}

models/weapons2/spraypistol/cartridge_red
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
                map models/weapons2/spraypistol/redpaint
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity

        }
	{
		map models/weapons2/spraypistol/padcart_r
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3
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
                map models/weapons2/spraypistol/bluepaint
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity

        }
	{
		map models/weapons2/spraypistol/padcart_b
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3
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
                map models/weapons2/spraypistol/greenpaint
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity

        }
	{
		map models/weapons2/spraypistol/padcart_n
		alphaFunc ge128
		rgbGen identity

  	}
	{
		map textures/pad_gfx02/tinpad3
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
	qer_editorimage models/weapons2/kma97/kma97

	nopicmip
	nomipmaps
	{
		map models/weapons2/kma97/kma97
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

