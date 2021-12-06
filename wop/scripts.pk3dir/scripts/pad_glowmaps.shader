textures/pad_glowsky/padbox
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_glowsky/padbox_white.tga
	q3map_sun	1 1 1 100 -58 58
	q3map_surfacelight 90

        skyparms env/padboxglow - -
       {
		map textures/pad_glowsky/sky.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.02
		tcMod scale 1 2
	}
       {
		map textures/pad_glowsky/sky2.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.02 -0.01
		tcMod scale 1 2
	}
}




textures/pad_glowsky/utopiaatoll
{
        qer_editorimage textures/pad_glowsky/utopiaatoll.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/orange01.tga
	q3map_sun	0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 200

        skyparms env/utopiaatoll512 - -
//       {
//		map textures/pad_glowsky/utopiaatoll.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_petesky/wolfl
{
        qer_editorimage textures/pad_petesky/wolf.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/grey.tga
//	q3map_sun	0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 20

        skyparms env/wolf-pack512 - -
//       {
//		map textures/pad_petesky/wolfl.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_glowsky/urban-terror
{
        qer_editorimage textures/pad_glowsky/urban-terror.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/orange03.tga
	q3map_sun	0.266383 0.274632 0.358662 90 90 90
	q3map_surfacelight 20

        skyparms env/urban-terror512 - -
//       {
//		map textures/pad_glowsky/urban-terror.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_glowsky/sandtrap
{
        qer_editorimage textures/pad_glowsky/sandtrap.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/blue.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 2

        skyparms env/sandtrap512 - -
//       {
//		map textures/pad_glowsky/sandtrap.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}




textures/pad_glowsky/gglass03
{
	qer_editorimage textures/pad_glowsky/gglass03.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_glowsky/gglass03.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_glowsky/gglass01
{
	qer_editorimage textures/pad_glowsky/gglass01.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_glowsky/gglass01.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_glowsky/gglass02
{
	qer_editorimage textures/pad_glowsky/gglass02.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_glowsky/gglass02.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}

textures/pad_backyard/pf-tornado-alley
{
        qer_editorimage textures/pad_backyard/pf-tornado-alley.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_backyard/grey.tga
	q3map_sun	0.266383 0.274632 0.358662 100 230 70
	q3map_surfacelight 175

        skyparms env/pf-tornado-alley512 - -
//       {
//		map textures/pad_backyard/pf-tornado-alley.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_glowsky/glass
{
   qer_editorimage   textures/pad_glowsky/glass.tga
              surfaceparm trans
   q3map_nolightmap
   q3map_onlyvertexlighting
    {
        map textures/pad_glowsky/glass.tga
        blendFunc GL_ONE GL_ONE
    }
}


textures/glowstar/glas
{
	qer_editorimage textures/colors/greygreen.tga
	cull back
	{
		map textures/colors/greygreen.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
}

textures/glowstar/greenglas
{
	qer_editorimage textures/colors/greygreen.tga
	cull back
	{
		map textures/colors/greygreen.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
	{
		map textures/colors/greygreen.tga
		blendfunc filter
	}
}

textures/glowstar/milchglas
{
	qer_editorimage textures/colors/greygreen.tga
	cull back
	{
		map textures/colors/greygreen.tga
		blendfunc add
	}
	{
		map textures/pad_gfx/env2.tga
		blendfunc filter
		tcGen environment 
	}
	{
		map textures/colors/greygreen.tga
		blendfunc filter
	}
	{
		map textures/colors/greygreen.tga
		blendfunc blend
	}
}


textures/pad_glow/liege
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_glow/liege.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}  


textures/pad_glow/marble01
{
	qer_editorimage textures/pad_glow/marble01.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_glow/marble04
{
	qer_editorimage textures/pad_glow/marble04.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble04.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_glow/marble06
{
	qer_editorimage textures/pad_glow/marble06.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble06.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble07
{
	qer_editorimage textures/pad_glow/marble07.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble07.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble08
{
	qer_editorimage textures/pad_glow/marble08.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble08.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble21
{
	qer_editorimage textures/pad_glow/marble21.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble21.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble27
{
	qer_editorimage textures/pad_glow/marble27.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble27.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_cabin/Moni
{
	q3map_lightimage textures/pad_cabin/Moni.tga
	q3map_surfacelight 20

        {
		map textures/pad_cabin/Moni.tga
		rgbGen identity
	}
	
       
	{
		map $lightmap
		tcgen environment
		tcmod scale .5 .5
		rgbGen wave sin .25 0 0 0
		blendfunc add
	}
}

textures/pad_backyard/Holz00y
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/Holz00y.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/Holz00z
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/Holz00y.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/planke02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/planke02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/wood0a
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0a.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/wood0b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



textures/pad_backyard/wood0c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood0d
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0d.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_backyard/wood0e
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0e.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}  


textures/pad_backyard/wood0f
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0f.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood0g
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0g.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood0h
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0h.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood0i
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0i.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood0j
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood0j.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



textures/pad_backyard/wood01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/wood02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/wood02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



textures/pad_backyard/glow_rost01
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/glow_rost01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_backyard/metal02
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/metal02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/metal04
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/metal04.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/metal06
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/metal06.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_backyard/Metal1
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/Metal01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}  

textures/pad_backyard/Metal2
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/Metal2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_backyard/rau
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/rau.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 
 


textures/pad_cabin/darkrost
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/darkrost.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_cabin/metal
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/metal.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_cabin/metal1
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/metal1.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_cabin/metall
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/metall.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}  


textures/pad_cabin/rost001
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/rost001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_cabin/fensterbank
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/fensterbank.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_cabin/wood0a
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/wood0a.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}    

textures/pad_cabin/wood0b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/wood0b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}   

textures/pad_cabin/wood0c
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/wood0c.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}  

textures/pad_cabin/wood0d
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cabin/wood0d.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}    

textures/pad_glow/wallcol016
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_glow/wallcol016.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_glow/wallcol017
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_glow/wallcol017.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_glowsky/glowscreen
{
	q3map_lightimage textures/pad_glowsky/glowscreen.tga
	q3map_surfacelight 20

        {
		map textures/pad_glowsky/glowscreen.tga
		rgbGen identity
	}
	
       
	{
		map $lightmap
		tcgen environment
		tcmod scale .5 .5
		rgbGen wave sin .25 0 0 0
		blendfunc add
	}
}


textures/pad_glow/metal_old
{
	qer_editorimage textures/pad_glow/metal_old.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/metal_old.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}