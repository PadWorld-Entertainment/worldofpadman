textures/pad_attic/leiter
{
        qer_editorimage textures/pad_attic/leiter.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_attic/leiter.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/pad_attic/clockrad
{
        qer_editorimage textures/pad_attic/clockrad.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_attic/clockrad.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/pad_attic/clockpendel
{
        qer_editorimage textures/pad_attic/clockpendel.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_attic/clockpendel.tga
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/pad_attic/darkred
{
	qer_editorimage textures/pad_attic/darkred.tga
	{
		map textures/pad_attic/darkred.tga
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

textures/pad_attic/darkblue
{
	qer_editorimage textures/pad_attic/darkblue.tga
	{
		map textures/pad_attic/darkblue.tga
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


textures/pad_attic/pad_jail_fly1
{
       qer_editorimage textures/pad_attic/pad_jail_fly1.tga
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
	
	map textures/pad_attic/pad_jail_fly1.tga
                tcMod Scroll -5 0.1
                tcMod turb .3 .25 0 .1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
{
 	map textures/pad_attic/pad_jail_fly2.tga
                tcMod Scroll 4 -0.5
                tcMod turb .1 .25 0 -.1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
}

textures/pad_attic/pad_jail_fly2
{
       qer_editorimage textures/pad_attic/pad_jail_fly1.tga
       surfaceparm trans	
       surfaceparm pointlight
       surfaceparm nomarks	
       surfaceparm nodamage        
       surfaceparm nonsolid
        surfaceparm nolightmap
        deformVertexes move 1 2.5 2  sin 0 6 0 0.4
        //deformVertexes move .6 3.3 0  sin 0 5 0 0.4
        deformVertexes wave 10 sin 0 4 0 .4
        tessSize 16
        cull none
{
	map $lightmap
	rgbGen identity	
	blendfunc gl_zero gl_one
}
{
	
	map textures/pad_attic/pad_jail_fly2.tga
                tcMod Scroll -4 0.3
                tcMod turb .2 .3 0 -.1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
{
 	map textures/pad_attic/pad_jail_fly1.tga
                tcMod Scroll 6 -0.4
                tcMod turb .2 .25 0 -.2
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
 }


textures/pad_attic/birdcage
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_attic/birdcage.tga
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



textures/pad_attic/lampdraht
{
	qer_editorimage textures/pad_attic/lampdraht.tga
	surfaceparm trans
	surfaceparm nomarks
             surfaceparm nonsolid		
	surfaceparm nolightmap
	surfaceparm alphashadow
	cull none
	q3map_surfacelight 50
        nopicmip
		{
		map textures/pad_attic/lampdraht.tga 
blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
	}
}


textures/pad_attic/flare
{
    deformVertexes autoSprite
    surfaceparm trans
    surfaceparm nomarks
    surfaceparm nonsolid		
    surfaceparm nolightmap
    cull none
          {
            clampmap textures/pad_attic/flare.tga
            blendFunc GL_ONE GL_ONE
          }
}

textures/pad_attic/flareyel
{
    deformVertexes autoSprite
    surfaceparm trans
    surfaceparm nomarks
    surfaceparm nonsolid		
    surfaceparm nolightmap
    cull none
          {
            clampmap textures/pad_attic/flareyel.tga
            blendFunc GL_ONE GL_ONE
          }
}

textures/pad_attic/milchglass_1
{
	qer_editorimage textures/pad_attic/greyblue.tga
	cull back
	{
		map textures/pad_attic/greyblue.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
}

textures/pad_attic/milchglass_2
{
	qer_editorimage textures/pad_attic/greyblue.tga
	cull back
	{
		map textures/pad_attic/greyblue.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
	{
		map texturespad_attic/greyblue.tga
		blendfunc filter
	}
}

textures/pad_attic/milchglass_3
{
	qer_editorimage textures/pad_attic/greyblue.tga
	cull back
	{
		map textures/pad_attic/greyblue.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
	{
		map textures/pad_attic/greyblue.tga
		blendfunc filter
	}
	{
		map textures/pad_attic/greyblue.tga
		blendfunc blend
	}
}


textures/pad_attic/balken
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/balken.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/balken
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/balken02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/border001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/border001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/border003
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/border003.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/brettborder
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/brettborder.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/brettborder02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/brettborder02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/dach03_1024
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/dach03_1024.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/oldstam03
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/oldstam03.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/newspaperfloor2
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/newspaperfloor2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/oldfloorhalf
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/oldfloorhalf.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/oldfloorrand02
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/oldfloorrand02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/oldmetal
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/oldmetal.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/regalbrett
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/regalbrett.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/schlaeger
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/schlaeger.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/schrankboard
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/schrankboard.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/schrankrollo
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/schrankrollo.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



textures/pad_attic/woodborder04
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/woodborder04.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_attic/woodlatten02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/woodlatten02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_attic/woodlatten02b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_attic/woodlatten02b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



