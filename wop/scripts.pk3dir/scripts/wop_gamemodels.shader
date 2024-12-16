// =================
// LOADiNGSTATiON
// =================

//23:19 06/04/2003//pad health station v2 SLoB
//2 main textures - pad_hs_base, pad_hs_crossring
//
models/mapobjects/pad_healthstation/pad_hs_dome
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base


	{
		map models/mapobjects/pad_healthstation/pad_hs_base
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
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		blendfunc add
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		rgbGen wave sin .5 .5 0 .35
		blendFunc add
	}
}

models/mapobjects/pad_healthstation/pad_hs_fin
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base
	{
		map models/mapobjects/pad_healthstation/pad_hs_base
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
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		blendFunc add

	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}

}

models/mapobjects/pad_healthstation/pad_hs_ring
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base
	{
		map models/mapobjects/pad_healthstation/pad_hs_base
                rgbGen lightingdiffuse
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		blendFunc add

	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight
		rgbGen wave sin .5 .5 0 .35
		blendFunc add
	}
}

models/mapobjects/pad_healthstation/pad_hs_crossring
{
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_crossring
	cull disable
	{
		map models/mapobjects/pad_healthstation/pad_hs_crossring
		rgbGen lightingdiffuse
	}
	{
	        map models/mapobjects/pad_healthstation/red02
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .2 0 .2
               	tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                rgbGen lightingdiffuse
        }
        {
                map models/mapobjects/pad_healthstation/red
		blendfunc GL_ONE GL_ONE
                tcMod turb 0 .1 0 .3
                tcmod scale .2 .2
                tcMod scroll .1 .09
                rgbGen lightingdiffuse
        }
}


station/ring
{
	cull disable
	{
		map models/mapobjects/pad_healthstation/ringy
		tcGen environment
		tcMod turb 0 0.15 0 0.3
                tcmod rotate 333
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ONE
	}
}


// =================
// LOLLIPOP
// =================

models/ctl/foil
{
	cull disable
	{
		map models/ctl/foil
		blendfunc add
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/ctl/r_lollipop
{
	cull disable
	{
		map models/ctl/r_lollipop
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/ctl/r_sticker_ribbon
{
	cull disable
	{
		map models/ctl/r_chromic
		rgbGen identity
		alphaFunc GE128
	}
	{
		map models/ctl/r_chromic
		tcGen environment
		depthFunc equal
	}
	{
		map models/ctl/r_sticker_ribbon
		depthFunc equal
		alphaFunc GE128
	}
}

models/ctl/b_lollipop
{
	cull disable
	{
		map models/ctl/b_lollipop
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/ctl/b_sticker_ribbon
{
	cull disable
	{
		map models/ctl/b_chromic
		rgbGen identity
		alphaFunc GE128
	}
	{
		map models/ctl/b_chromic
		tcGen environment
		depthFunc equal
	}
	{
		map models/ctl/b_sticker_ribbon
		depthFunc equal
		alphaFunc GE128
	}
}

models/ctl/n_lollipop
{
	cull disable
	{
		map models/ctl/n_lollipop
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/ctl/n_sticker_ribbon
{
	cull disable
	{
		map models/ctl/n_chromic
		rgbGen identity
		alphaFunc GE128
	}
	{
		map models/ctl/n_chromic
		tcGen environment
		depthFunc equal
	}
	{
		map models/ctl/n_sticker_ribbon
		depthFunc equal
		alphaFunc GE128
	}
}

// =================
// LOLLIPOP HOLO
// =================

models/ctl/r_lollipop_holo
{
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/r_holo_lollipop.png
		blendfunc add
		rgbGen identity
		depthFunc equal
	}
	{
		map models/ctl/r_holo_glow.png
		blendFunc add
		tcgen vector ( 0 0.01 0 ) ( 0 0 -.01 0)
		tcMod scroll 0 -0.4
		depthFunc equal
	}
}

models/ctl/r_ribbon_holo
{
	cull none
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/r_holo_lollipop.png
		blendfunc add
		rgbGen identity
		depthFunc equal
	}
	{
		map models/ctl/r_holo_glow.png
		blendFunc add
		tcgen vector ( 0 0.01 0 ) ( 0 0 -.01 0)
		tcMod scroll 0 -0.4
		depthFunc equal
	}
}

models/ctl/r_label_holo
{
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/r_holo_lollipop.png
		blendfunc add
		depthFunc equal
	}
}

models/ctl/b_lollipop_holo
{
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/b_holo_lollipop.png
		blendfunc add
		rgbGen identity
		depthFunc equal
	}
	{
		map models/ctl/b_holo_glow.png
		blendFunc add
		tcgen vector ( 0 0.01 0 ) ( 0 0 -.01 0)
		tcMod scroll 0 -0.4
		depthFunc equal
	}
}

models/ctl/b_ribbon_holo
{
	cull none
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/b_holo_lollipop.png
		blendfunc add
		rgbGen identity
		depthFunc equal
	}
	{
		map models/ctl/b_holo_glow.png
		blendFunc add
		tcgen vector ( 0 0.01 0 ) ( 0 0 -.01 0)
		tcMod scroll 0 -0.4
		depthFunc equal
	}
}

models/ctl/b_label_holo
{
	{
		map models/ctl/holo_interlace.png
		blendFunc GL_ZERO GL_ONE
		alphaFunc GE128
		tcgen vector ( 0 0.04 0 ) ( 0 0 -.04 0)
		tcMod scroll 0 -0.2
		depthWrite
	}
	{
		map models/ctl/b_holo_lollipop.png
		blendfunc add
		depthFunc equal
	}
}

// =================
// BiG BALLOON
// =================

models/special/ballon
{
	nomipmaps
	{
		map models/special/ballon
		rgbGen entity
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
        rgbGen entity //lightingDiffuse //vertex //identity //makes effect darker
	}
}

models/special/box
{
	nopicmip
	nomipmaps
	qer_editorimage models/special/box
	{
		map models/special/box
		rgbGen lightingdiffuse
	}

}

models/special/bow
{
	cull none
	{
		map textures/pad_gfx02/padmaprail4
		tcGen environment
		rgbGen lightingdiffuse
	}
}

// =================
// FT ICEBLOCK
// =================

models/iceblock
{
	cull none
	{
        map models/iceblock
		tcGen environment
		blendfunc GL_ONE GL_ONE
    }
}

// =================
// SyC TELEPORTER
// =================

//pad_teleporter/shader for Padmod by SLoB
//models/mapobjects/pad_teleporter/newpadtp.md3

models/mapobjects/pad_teleporter/pad_teleporter_blue
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr
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
		map models/mapobjects/pad_teleporter/pad_teleporter_bl_glow
		blendFunc add
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_bl_glow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}

}

models/mapobjects/pad_teleporter/pad_teleporter_orange
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr
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
		map models/mapobjects/pad_teleporter/pad_teleporter_or_glow
		blendFunc add
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_or_glow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
}

models/mapobjects/pad_teleporter/pad_teleporter_yellow
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr
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
		map models/mapobjects/pad_teleporter/pad_teleporter_ye_glow
		blendFunc add
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_ye_glow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
}

models/mapobjects/pad_teleporter/pad_teleporter_pink
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr
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
		map models/mapobjects/pad_teleporter/pad_teleporter_pi_glow
		blendFunc add
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_pi_glow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
}

models/mapobjects/pad_teleporter/pad_teleporter_purple
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr
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
		map models/mapobjects/pad_teleporter/pad_teleporter_pu_glow
		blendFunc add
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_pu_glow
		blendFunc add
		rgbGen wave sin .5 .5 0 .35

	}
}

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
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padspherebl
	q3map_surfacelight 200

	cull disable
    	{
		map models/mapobjects/pad_teleporter/padspherebl
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherebl01
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherebl02
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll -.1 .1
		rgbGen lightingdiffuse
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


// =================
// WEAPON MARKER
// =================

//Weapon marker model used for marking the weapon positions in the padmod maps :)
//model design by SLoB -shader by SLoB - Skins by Ente 2003
// 5 weapon marker shaders

// black/red design with blue glow and blue cone 
models/markers/black_bg
{
	nopicmip
	qer_editorimage models/markers/black_bg
	{
		map models/markers/black_bg
	}
	{
		map models/markers/blueglow
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

// blue/red design with blue glow and blue cone 
models/markers/blue_bg
{
	nopicmip
	qer_editorimage models/markers/blue_bg
	{
		map models/markers/blue_bg
	}
	{
		map models/markers/blueglow
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

// orange/green design with green glow and green cone 
models/markers/green_gg
{
	nopicmip
	qer_editorimage models/markers/green_gg
	{
		map models/markers/green_gg
	}
	{
		map models/markers/greenglow
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

// orange/blue design with blue glow and blue cone 
models/markers/orange_bg
{
	nopicmip
	qer_editorimage models/markers/orange_bg
	{
		map models/markers/orange_bg
	}
	{
		map models/markers/blueglow
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

// purple/pink design with pink glow and blue cone 
models/markers/purple_pg
{
	nopicmip
	qer_editorimage models/markers/purple_pg
	{
		map models/markers/purple_pg
	}
	{
		map models/markers/pinkglow
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

// Conefade - Blue & Green

models/markers/bluecone
{
	nopicmip
	cull none
	qer_editorimage models/markers/beam_blue
	{
		map models/markers/beam_blue
		tcMod Scroll .3 0
		blendFunc add
	}
}

models/markers/greencone
{
	nopicmip
	cull none
	qer_editorimage models/markers/beam_green
	{
		map models/markers/beam_green
		tcMod Scroll .3 0
		blendFunc add
	}
}
