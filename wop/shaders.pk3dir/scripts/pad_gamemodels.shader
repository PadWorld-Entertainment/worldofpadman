// =================
// LOADiNGSTATiON
// =================

//23:19 06/04/2003//pad health station v2 SLoB
//2 main textures - pad_hs_base.tga, pad_hs_crossring.tga
//
models/mapobjects/pad_healthstation/pad_hs_dome
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base.tga
	
	
	{
		map models/mapobjects/pad_healthstation/pad_hs_base.tga
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
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
		blendfunc add
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
		rgbGen wave sin .5 .5 0 .35
		blendFunc add
	}
}

models/mapobjects/pad_healthstation/pad_hs_fin
{   	                 
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base.tga
	{
		map models/mapobjects/pad_healthstation/pad_hs_base.tga
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
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
		blendFunc add
		
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35
		
	}
	
}

models/mapobjects/pad_healthstation/pad_hs_ring
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_base.tga
	{
		map models/mapobjects/pad_healthstation/pad_hs_base.tga
                rgbGen lightingdiffuse
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
		blendFunc add
		
	}
	{
		map models/mapobjects/pad_healthstation/pad_hs_baselight.tga
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
	qer_editorimage models/mapobjects/pad_healthstation/pad_hs_crossring.tga
	cull disable 
	{
		map models/mapobjects/pad_healthstation/pad_hs_crossring.tga
		rgbGen lightingdiffuse
	}
	{
	        map models/mapobjects/pad_healthstation/red02.tga
		blendfunc GL_ONE GL_ONE                   
                tcMod turb 0 .2 0 .2
               	tcmod scale .4 .4
                tcMod scroll .09 -.1.1
                rgbGen lightingdiffuse
        }	
        {
                map models/mapobjects/pad_healthstation/red.tga
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
		map models/mapobjects/pad_healthstation/ringy.tga
		tcGen environment
		tcMod turb 0 0.15 0 0.3
                tcmod rotate 333
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ONE
	}
}


// =================
// LOLLYPOP
// =================

//shaders by doomdragon for WoP capture the lollipop gamemode
models/ctl/foil
{
	cull disable
	{
		map models/ctl/foil.tga
		blendfunc add
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment 
	}
}

models/ctl/r_lollipop
{
	cull disable
	{
		map models/ctl/r_lollipop.tga
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment 
	}
}

models/ctl/r_sticker_ribbon
{
	cull disable
	{
		map models/ctl/r_chromic.tga
		rgbGen identity
		alphaFunc GE128
	}
	{
		map models/ctl/r_chromic.tga
		tcGen environment 
		depthFunc equal
	}
	{
		map models/ctl/r_sticker_ribbon.tga
		depthFunc equal
		alphaFunc GE128
	}
}

models/ctl/b_lollipop
{
	cull disable
	{
		map models/ctl/b_lollipop.tga
		rgbGen identity
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment 
	}
}

models/ctl/b_sticker_ribbon
{
	cull disable
	{
		map models/ctl/b_chromic.tga
		rgbGen identity
		alphaFunc GE128
	}
	{
		map models/ctl/b_chromic.tga
		tcGen environment 
		depthFunc equal
	}
	{
		map models/ctl/b_sticker_ribbon.tga
		depthFunc equal
		alphaFunc GE128
	}
}

// =================
// BiGBALLOON
// =================

models/special/ballon
{
	nomipmaps
	{
		map models/special/ballon.tga
		rgbGen entity  
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE 
		tcGen environment
        rgbGen entity //lightingDiffuse //vertex //identity //makes effect darker
	}
}

models/special/box
{	
	nopicmip
	nomipmaps
	qer_editorimage models/special/box.tga
	{
		map models/special/box.tga
		rgbGen lightingdiffuse
	}

}

models/special/bow
{	
	cull none
	{
		map textures/pad_gfx02/padmaprail4.tga
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
        map models/iceblock.tga
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
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
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
		map models/mapobjects/pad_teleporter/pad_teleporter_bl_glow.tga
		blendFunc add			
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_bl_glow.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35	
		
	}
	
}

models/mapobjects/pad_teleporter/pad_teleporter_orange
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
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
		map models/mapobjects/pad_teleporter/pad_teleporter_or_glow.tga
		blendFunc add			
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_or_glow.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35	
		
	}
}

models/mapobjects/pad_teleporter/pad_teleporter_yellow
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
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
		map models/mapobjects/pad_teleporter/pad_teleporter_ye_glow.tga
		blendFunc add			
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_ye_glow.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35	
		
	}
}

models/mapobjects/pad_teleporter/pad_teleporter_pink
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
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
		map models/mapobjects/pad_teleporter/pad_teleporter_pi_glow.tga
		blendFunc add			
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_pi_glow.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35	
		
	}
}

models/mapobjects/pad_teleporter/pad_teleporter_purple
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_blgr.tga
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
		map models/mapobjects/pad_teleporter/pad_teleporter_pu_glow.tga
		blendFunc add			
	}
	{
		map models/mapobjects/pad_teleporter/pad_teleporter_pu_glow.tga
		blendFunc add
		rgbGen wave sin .5 .5 0 .35	
		
	}
}

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
	nopicmip
	nomipmaps
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_teleporter/padspherebl.tga
	q3map_surfacelight 200

	cull disable 
    	{
		map models/mapobjects/pad_teleporter/padspherebl.tga
               	blendfunc GL_ONE GL_ONE
		tcMod scale 2 2	
               	tcMod scroll .2 .2
		rgbGen lightingdiffuse
       }

	{
               	map textures/pad_gfx02/padmapspherebl01.jpg
		blendfunc GL_ONE GL_ONE
		tcGen environment
               	tcMod scroll .1 -.1
		rgbGen lightingdiffuse
       	}

	{
               	map textures/pad_gfx02/padmapspherebl02.jpg
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


// =================
// WEAPONMARKER
// =================

//Weapon marker model used for marking the weapon positions in the padmod maps :)
//model design by SLoB -shader by SLoB - Skins by Ente 2003
//5 weapon marker shaders

//4 blue versions with blue glow
models/mapobjects/pad_weaponmarker/pad_wepm_blue_bg
{	
	nopicmip
	qer_editorimage models/mapobjects/pad_weaponmarker/pad_wepm_blue_bg.tga
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_blue_bg.tga
		
		
	}
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_blueglow.tga
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

models/mapobjects/pad_weaponmarker/pad_wepm_black_bg
{	
	nopicmip
	qer_editorimage models/mapobjects/pad_weaponmarker/pad_wepm_black_bg.tga
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_black_bg.tga
		
		
	}
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_blueglow.tga
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

models/mapobjects/pad_weaponmarker/pad_wepm_orange_bg
{	
	nopicmip
	qer_editorimage models/mapobjects/pad_weaponmarker/pad_wepm_orange_bg.tga
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_orange_bg.tga
		
		
	}
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_blueglow.tga
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

models/mapobjects/pad_weaponmarker/pad_wepm_green_bg
{	
	nopicmip
	qer_editorimage models/mapobjects/pad_weaponmarker/pad_wepm_green_bg.tga
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_green_bg.tga
		
		
	}
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_pinkglow.tga
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}

//1 green version with green glow
models/mapobjects/pad_weaponmarker/pad_wepm_green_gg
{	
	nopicmip
	qer_editorimage models/mapobjects/pad_weaponmarker/pad_wepm_green_gg.tga
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_green_gg.tga
		
		
	}
	{
		map models/mapobjects/pad_weaponmarker/pad_wepm_greenglow.tga
		rgbGen wave sin .05 .9 0 .4
		blendFunc add
	}
}


//Conefade - Blue & Green

models/mapobjects/pad_weaponmarker/pad_wepm_bluecone
{
	nopicmip
	cull none
	qer_editorimage models/mapobjects/pad_weaponmarker/beam_blue.tga
	{
		map models/mapobjects/pad_weaponmarker/beam_blue.tga
                tcMod Scroll .3 0
                blendFunc add
        }
     
}

models/mapobjects/pad_weaponmarker/pad_wepm_greencone
{
	nopicmip
	cull none
	qer_editorimage models/mapobjects/pad_weaponmarker/beam_green.tga
	{
		map models/mapobjects/pad_weaponmarker/beam_green.tga
                tcMod Scroll .3 0
                blendFunc add
        }
     
}

