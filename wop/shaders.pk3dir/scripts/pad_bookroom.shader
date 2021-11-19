textures/pad_bookroom/fenstergitter
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/fenstergitter.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/fenstergitter003
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/fenstergitter003.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/vinesss
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/vinesss.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/goldstab001
{
	qer_editorimage textures/pad_bookroom/goldstab001.tga
	{
		map textures/pad_bookroom/goldstab001.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_bookroom/goldstab002
{
	qer_editorimage textures/pad_bookroom/goldstab002.tga
	{
		map textures/pad_bookroom/goldstab002.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/goldstab003
{
	qer_editorimage textures/pad_bookroom/goldstab003.tga
	{
		map textures/pad_bookroom/goldstab003.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_bookroom/goldstab004
{
	qer_editorimage textures/pad_bookroom/goldstab004.tga
	{
		map textures/pad_bookroom/goldstab004.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_bookroom/goldstab005
{
	qer_editorimage textures/pad_bookroom/goldstab005.tga
	{
		map textures/pad_bookroom/goldstab005.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/goldstab006
{
	qer_editorimage textures/pad_bookroom/goldstab006.tga
	{
		map textures/pad_bookroom/goldstab006.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_bookroom/bleifenster001
{
	qer_editorimage textures/pad_bookroom/bleifenster001.tga
	surfaceparm trans
        cull none
        qer_trans 0.6
        {
		map textures/pad_bookroom/bleifenster001.tga
		blendfunc gl_dst_color gl_zero
	}
}

textures/pad_bookroom/bleifenster002
{
	qer_editorimage textures/pad_bookroom/bleifenster002.tga
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
        cull none
        qer_trans 0.6
        {
		map textures/pad_bookroom/bleifenster002.tga
                alphaFunc GE128
		blendfunc gl_dst_color gl_zero
	}
}


textures/pad_bookroom/globusmass
{
	qer_editorimage textures/pad_bookroom/globusmass.tga
	{
		map textures/pad_bookroom/globusmass.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_bookroom/lightyy
{
    q3map_surfacelight 5
    surfaceparm nolightmap
    {
        map textures/pad_bookroom/lightyy.tga
    }
}


textures/pad_bookroom/telepaddy
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_bookroom/telepaddy.tga
		tcGen environment
                tcMod turb 0 0.25 0 0.2
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_bookroom/blobbypad
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided

	deformVertexes wave 30 sin 0 5 0 0.2 
	deformVertexes wave 100 sin 0 4 0 0.1 
	tessSize 48
	qer_trans 0.5
	{
		map textures/pad_bookroom/blobbypad.tga
		tcGen environment
                tcMod turb 0 0.35 0 0.25
                tcmod scroll -0.6 -0.8
	}
	{
		map textures/pad_bookroom/blobbypad.tga
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_bookroom/bomarble01
{
	qer_editorimage textures/pad_bookroom/bomarble01.tga
	{
		map textures/pad_bookroom/bomarble01.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble01_dark
{
	qer_editorimage textures/pad_bookroom/bomarble01_dark.tga
	{
		map textures/pad_bookroom/bomarble01_dark.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble01b
{
	qer_editorimage textures/pad_bookroom/bomarble01b.tga
	{
		map textures/pad_bookroom/bomarble01b.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble01blue
{
	qer_editorimage textures/pad_bookroom/bomarble01blue.tga
	{
		map textures/pad_bookroom/bomarble01blue.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble01blue_dark
{
	qer_editorimage textures/pad_bookroom/bomarble01blue_dark.tga
	{
		map textures/pad_bookroom/bomarble01blue_dark.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble01b_blue
{
	qer_editorimage textures/pad_bookroom/bomarble01b_blue.tga
	{
		map textures/pad_bookroom/bomarble01b_blue.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_bookroom/bomarble02
{
	qer_editorimage textures/pad_bookroom/bomarble02.tga
	{
		map textures/pad_bookroom/bomarble02.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
		blendfunc add
		rgbGen identity
		tcGen environment 
	}
	{
		map $lightmap 
		blendfunc filter
		rgbGen identity
	}
}



textures/pad_bookroom/flystars
{
       qer_editorimage textures/pad_bookroom/flystars.tga
       surfaceparm trans	
       surfaceparm pointlight
       surfaceparm nomarks	
       surfaceparm nodamage        
       surfaceparm nonsolid
        surfaceparm nolightmap
        deformVertexes move 2 3 1.5  sin 0 5 0 0.3
        //deformVertexes move .6 3.3 0  sin 0 5 0 0.4
        deformVertexes wave 20 sin 0 8 0 .2
        tessSize 16
        cull none
{
	map $lightmap
	rgbGen identity	
	blendfunc gl_zero gl_one
}
{
	
	map textures/pad_bookroom/flystars.tga
                tcMod Scroll -5 0.1
                tcMod turb .3 .25 0 .1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
{
 	map textures/pad_bookroom/flystars2.tga
                tcMod Scroll 4 -0.5
                tcMod turb .1 .25 0 -.1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
}


textures/pad_bookroom/pad_flag_blue
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_bookroom/pad_flag_blue.tga
                alphaFunc GE128
		depthWrite
		rgbGen vertex
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/pad_flag_red
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_bookroom/pad_flag_red.tga
                alphaFunc GE128
		depthWrite
		rgbGen vertex
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/beam_pink
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_bookroom/beam_pink.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}

textures/pad_bookroom/beam_yellow
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_bookroom/beam_yellow.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_bookroom/portal
{

	portal
	surfaceparm nolightmap
	deformVertexes wave 150 sin 0 1 0 .5


	{
		map textures/pad_bookroom/portal_sfx3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	//	This blend function is not working on RagePro drivers Mac only
	//	when it is used on top of portals
		depthWrite
	}




	{
		map textures/pad_bookroom/portal_sfx.tga
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 .5 .2 .5
	}

	{
		map textures/pad_bookroom/portalfog.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen portal 1024
		rgbGen identityLighting	
//		tcmod turb sin 0 .5 0 1
		tcmod rotate .1 //.1
		tcmod scroll .01 .03
	}
}


textures/pad_bookroom/hotlava
{
	qer_editorimage textures/pad_bookroom/hotlava.tga
	q3map_globaltexture
	surfaceparm trans
	//surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	q3map_surfacelight 80
	cull disable
	
	tesssize 128
	cull disable
	//deformVertexes wave 100 sin 3 2 .1 0.1
	
	{
		map textures/pad_bookroom/hotlava.tga
		tcMod turb 0 .2 0 .1
	}
}


textures/pad_bookroom/rostgitter
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     surfaceparm latticesteps
     tessSize 64
    
     
        {
                map textures/pad_bookroom/rostgitter.tga
                alphaFunc GE128
		depthWrite
		rgbGen vertex
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/doc_poe_kid
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/doc_poe_kid.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}

textures/pad_bookroom/freud_hari_curie
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/freud_hari_curie.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/zierkante
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_bookroom/zierkante.tga
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_bookroom/floor_pad03
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/floor_pad03.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/tep002
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/tep002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/tep004
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/tep004.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/dekostoff001
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/dekostoff001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/dekostoff002
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/dekostoff002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/dekostoff002c
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/dekostoff002c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/decke005b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/decke005b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan001c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan001c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan003b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan003b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan005b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan005b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan006
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan006.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan007
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan007.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/platte02big
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/platte02big.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/reg001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/reg001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/reg002b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/reg002b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/reg002c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/reg002c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/reg003b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/reg003b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/schach001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/schach001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/windowboard2
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/windowboard2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/windownutsmall
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/windownutsmall.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodkante001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodkante001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodkante002
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodkante002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodkante003
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodkante003.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodkante004bigc
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodkante004bigc.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodkante005
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodkante005.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodsockel
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodsockel.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodsockelc
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodsockelc.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier001b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier001b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier002
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier003
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier003.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier005
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier005.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_bookroom/woodzier005b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier005b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier005c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier005c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier006
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier006.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier006b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier006b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier007
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier007.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier008c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier008c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier011c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier011c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier012
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier012.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier001b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier001b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier015
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier015.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier018
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier018.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier019b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier019b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier019
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier019.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/wookkante008
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/wookkante008.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woood001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woood001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woood002
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woood002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woood001old2
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woood001old2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_wallin/wallcol26
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_wallin/wallcol26.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_wood/wood034
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_wood/wood034.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_wood/wood039
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_wood/wood039.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_wood/wood047
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_wood/wood047.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_bookroom/gelaenderstre
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelaenderstre.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/tep001
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/tep001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/wookkante007
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/wookkante007.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/wookkante007b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/wookkante007b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/woodzier003b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/woodzier003b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_bookroom/gelan001b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/gelan001b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_bookroom/frost-bite
{
        qer_editorimage textures/pad_bookroom/frost_bite.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_bookroom/frost_color.tga
	q3map_sun	0.754360 0.959199 1.000000 300 50 55
	q3map_surfacelight 300

        skyparms env/frost-bite512 - -
//       {
//		map textures/pad_bookroom/frost-bite.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}