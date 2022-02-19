// =================
// PADSHiELD and PADSHARD
// =================

models/powerups/armor/pad_shield_front

{
	{
		map textures/pad_gfx02/padmapred
		tcGen environment
                rgbGen identity
	}

}


models/powerups/armor/pad_shield_back

{

	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
                rgbGen identity
	}
}

models/powerups/armor/pad_shield_logo

{

	{
		map textures/pad_gfx02/padmapyel3c
		tcGen environment
                rgbGen identity
	}
}



models/powerups/armor/pad_shard_bott
{
	nopicmip
	nomipmaps
	{
		map textures/pad_gfx02/padmapyel3c
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}

}

models/powerups/armor/pad_shard_top
{
	nopicmip
	nomipmaps
	{
		map textures/pad_gfx02/padmapred
		tcGen environment
                rgbGen identity
	}
}


// =================
// KiLLERDUCKS
// =================

models/weapons2/killerducks/teeth
{
	nopicmip
	nomipmaps
	cull none
	surfaceparm nolightmap
	{
		map models/weapons2/killerducks/teeth
		alphaFunc ge128
		rgbGen lightingdiffuse

	}
}

models/weapons2/killerducks/key
{
	nopicmip
	nomipmaps
	cull none
	surfaceparm nolightmap
	{
		map models/weapons2/killerducks/key
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaFunc ge128
		//depthWrite
		rgbGen lightingdiffuse
	}
}


models/weapons2/killerducks/duckwheel
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		map models/weapons2/killerducks/duckwheel
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/weapons2/killerducks/duckbody
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		map models/weapons2/killerducks/duckbody
                blendfunc GL_ONE GL_ZERO
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
               	rgbGen lightingdiffuse
	}
}

//just add cull disable for the maskties only to save on polys :)
models/weapons2/killerducks/duckbody_maskties
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		map models/weapons2/killerducks/duckbody
                blendfunc GL_ONE GL_ZERO
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
               	rgbGen lightingdiffuse
	}
}

//powerup model

models/weapons2/killerducks/killerduck_up
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
}

//just add cull disable for the maskties only to save on polys :)
models/weapons2/killerducks/killerduck_up_maskties
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
}

//Yellow FLARE used for Atoms effect in the Ring
models/powerups/instant/yellflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/yellflare
            	blendFunc GL_ONE GL_ONE
        }
}

// =================
// PUPPETMASTER
// =================

// ToDo!?!
// "powerups/berserkerPunchy" -> punchy skin/shader für berserker-powerup (falls nicht vorhanden bekommt die punchy die berserkerAura )
// "powerups/padpowerPunchy" -> extra skin/shader für punchy bei padpower (falls nicht vorhanden wird normaler padpower-shader benutzt)

powerups/berserkerAura
{
	//vq3-quad: wave 100 sin 3 0 0 0
//	deformVertexes wave 400 sawtooth 1.6 1.5 0 4
	deformVertexes wave 400 inversesawtooth 0.1 3 0 2
	{
		//map $whiteimage
		//map textures/pad_gfx02/invispad
		map textures/effects/berserkerAura
		blendfunc add
		tcMod turb 0 0.15 0 0.25
		//rgbGen entity
		rgbGen const ( 1 0 0 )
		alphaGen entity
		tcGen environment
	}
}

deadfadeSkin
{
	{
//		map $whiteimage
		animmap 10 textures/effects/death1 textures/effects/death2 textures/effects/death3
		rgbGen const ( 0.411765 0.580392 0.992157 )
		alphaGen entity
		blendfunc blend
	}
}




punchySkins/padPowerpunchy_teeth
{
	nopicmip
	nomipmaps
	cull none
	{
                map punchySkins/padPowerTeeth
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

// =================
// POWERUPS
// =================

// (1) PadPower Shader by SLoB 2002

models/powerups/instant/quad
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		map textures/pad_gfx02/tinpad2d
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
}

// (2) Speedy Shader by SLoB 2002

models/powerups/instant/cone
{
	{
		map textures/pad_gfx02/padmappurp01
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}
}
models/powerups/instant/hrocket
{
	{
		map textures/pad_gfx02/padmapyel3b
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity

	}
}

models/powerups/instant/wings
{
	{
		map textures/pad_gfx02/padmapyel3b
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}
}


models/powerups/instant/fuseflare
{
    	deformVertexes autoSprite
    	surfaceparm trans
    	surfaceparm nomarks
    	surfaceparm nolightmap
    	cull none
        {
            	clampmap models/powerups/instant/fuseflare
            	blendFunc GL_ONE GL_ONE
        }
}

models/powerups/instant/fuse
{

	deformVertexes autoSprite2
    	surfaceparm trans
    	surfaceparm nomarks
    	surfaceparm nolightmap
    	cull none

	{
		map models/powerups/instant/fuse
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
}

// (3) Climber Shader by SLoB 2002
models/powerups/instant/wwwings
{
	{
		map textures/pad_gfx02/padmappurp
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                rgbGen identity
	}
}

models/powerups/instant/plunger
{
	{
		map textures/pad_gfx02/padmappurp
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                //rgbGen identity
		rgbGen lightingDiffuse
	}
}



// (4) Floater Shader by SLoB 2002
models/powerups/instant/floater
{
	q3map_surfacelight 250
	{
		map textures/pad_gfx02/padmapblue
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                rgbGen lightingDiffuse //makes effect darker
	}
}

// (5) Jumper Shader by SLoB 2002
models/powerups/instant/jumper
{
	{
		map textures/pad_gfx02/padmaprail2
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                rgbGen identity
	}
}

models/powerups/instant/jboot
{
	{
		map textures/pad_gfx02/padmaprail3
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                //rgbGen identity
		rgbGen lightingDiffuse
	}
}

// (6) Revival Shader by SLoB 2002
models/powerups/instant/regen
{
	{
		map textures/pad_gfx02/padmapred
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}
}


// (7) VisionLess Shader by SLoB 2002
models/powerups/instant/padinvisible
{
	{
		map textures/pad_gfx02/tinpad2c
                //map textures/sfx/specular
		blendfunc GL_ONE GL_ONE
		tcGen environment
	}
}



//Flare Effects for Pad Item Rings by SLoB 2002

//INVIS-WHITE FLARE used for Atoms effect in the Ring
models/powerups/instant/invisflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/invisflare
            	blendFunc GL_ONE GL_ONE
        }
}

//RED FLARE used for Atoms effect in the Ring
models/powerups/instant/redflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/redflare
            	blendFunc GL_ONE GL_ONE
        }
}

//Purple FLARE used for Atoms effect in the Ring
models/powerups/instant/purpflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/purpflare
            	blendFunc GL_ONE GL_ONE
        }
}

//Blue FLARE used for Atoms effect in the Ring
models/powerups/instant/flightflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/flightflare
            	blendFunc GL_ONE GL_ONE
        }
}

//Green FLARE used for Atoms effect in the Ring
models/powerups/instant/greenflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/greenflare
            	blendFunc GL_ONE GL_ONE
        }
}


powerups/padpower
{
	cull disable
	{
//		map textures/pad_gfx02/tinpad
		map textures/pad_gfx02/tinpad2d
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		rgbGen entity
	}
}

sprites/star01
{
	{
		map sprites/star01
		blendfunc blend
		alphaGen vertex
	}
}

sprites/star02
{
	{
		map sprites/star02
		blendfunc blend
		alphaGen vertex
	}
}

sprites/star03
{
	{
		map sprites/star03
		blendfunc blend
		alphaGen vertex
	}
}

powerups/invisibility
{
	{
		map textures/pad_gfx02/invispad
                //map textures/sfx/specular
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 0.15 0 0.25
		tcGen environment
	}
}



// =================
// WATERBOTTLES
// =================

//bottom is same for all ammo :)

models/powerups/ammo/bottom
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/bottom
		rgbGen lightingDiffuse
	}
		//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

//top is same for all ammo :)

models/powerups/ammo/top
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/top
		rgbGen lightingDiffuse
	}
		//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_nipper
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_nipper

		rgbGen lightingDiffuse
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_balloony
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_balloony

		rgbGen lightingDiffuse
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_bubbleg
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_bubbleg

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_boaster
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_boaster

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_betty
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_betty

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_imperius
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_imperius

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_pumper
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_pumper

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

models/powerups/ammo/waterbottle_splasher
{
	nopicmip
	nomipmaps
	{
		map models/powerups/ammo/waterbottle_splasher

		rgbGen lightingDiffuse //makes effect darker
        }
	//slight sheen effect
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
}

// =================
// BAMBAM
// =================

models/powerups/instant/bambam
{
	{
		map textures/pad_gfx02/padmappurp02
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}
}

models/powerups/instant/bamflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/bamflare
            	blendFunc GL_ONE GL_ONE
        }
}

models/powerups/instant/bambam_teeth
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		map textures/pad_gfx02/padmapblue4
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
}

models/weapons2/bambam/bambam_zzz
{
	cull disable
	surfaceparm nolightmap
	deformVertexes autoSprite
	{
		map models/weapons2/bambam/bambam_zzz
		rgbGen identity
		alphaFunc GE128
	}
}

models/weapons2/bambam/bambam_redfx
{
	{
        map models/weapons2/bambam/redpainti
	tcGen environment
	tcMod turb .1 0 .3 .2
        tcMod scroll .1 .09
		rgbGen identity
	}
}

models/weapons2/bambam/bambam_bluefx
{
	{
        map models/weapons2/bambam/bluepainti
	tcGen environment
	tcMod turb .1 0 .3 .2
        tcMod scroll .1 .09
	rgbGen identity
	}
}

models/weapons2/bambam/bambam_burnoutfx
{
	{
        map models/weapons2/bambam/burnout
	tcGen environment
	tcMod turb 0 .5 0 .3
        tcMod scroll .1 .09
	rgbGen lightingdiffuse
	}
}


models/weapons2/bambam/bambam
{

	nopicmip
	nomipmaps
	{
                map textures/pad_gfx02/padmapblack
		tcGen environment
		tcMod turb 0 .5 0 .3
               		 tcMod scroll .1 .09
		rgbGen lightingdiffuse
		        	}
	{
		map models/weapons2/bambam/bambam
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

bambamBall
{
	{
		map models/weaponsfx/bambamball
		blendFunc add
	}
}


models/weaponsfx/bambamdrop
{
	cull none
	{
		map models/weaponsfx/bambamdrop
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}

models/weaponsfx/bambamdrop_blue
{
	cull none
	{
		map models/weaponsfx/bambamdrop_blue
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}

models/weaponsfx/bambamdrop_red
{
	cull none
	{
		map models/weaponsfx/bambamdrop_red
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}


// =================
// BOOMIES
// =================

models/powerups/instant/boomies
{
	{
		map textures/pad_gfx02/padmaplil
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity

	}
}

models/powerups/instant/blueflare
{
    		deformVertexes autoSprite
    		surfaceparm trans
    		surfaceparm nomarks
    		surfaceparm nolightmap
    		cull none
        {
            	clampmap models/powerups/instant/blueflare
            	blendFunc GL_ONE GL_ONE
        }
}

models/powerups/instant/boomies_arc
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
		map textures/pad_gfx02/padmappurp01
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
                rgbGen identity

	}
}


models/weapons2/boomies/boomies_red
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
                map models/weapons2/spraypistol/redpaint
		tcGen environment
		tcMod turb 0 .5 0 .3
                tcMod scroll .1 .09
		rgbGen identity
	}
	{
		map models/weapons2/boomies/boomies_red
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


models/weapons2/boomies/boomies_blue
{
	nopicmip
	nomipmaps
	surfaceparm nolightmap
	cull disable
	{
                map models/weapons2/spraypistol/bluepaint
		tcGen environment
		tcMod turb 0 .5 0 .3
               		 tcMod scroll .1 .09
		rgbGen identity
		        	}
	{
		map models/weapons2/boomies/boomies_blue
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

boomiesCore
{
	cull none
	{
		map models/weaponsfx/boomiescore
		blendFunc add
		rgbGen entity
	}
}

models/weaponsfx/boomiessphere
{
	{
		map models/weaponsfx/boomiessphere
		blendFunc add
		rgbGen entity
		tcMod scroll 0.5 0.7
	}
}
