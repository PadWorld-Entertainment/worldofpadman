textures/pad_shop/light_blue
{
    q3map_surfacelight 1000
    surfaceparm nolightmap
    {
        map textures/pad_shop/light_blue.tga
    }
}

textures/pad_shop/lampi001
{
	qer_editorimage textures/pad_shop/lampi001.tga
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/lampi001.tga
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

textures/pad_shop/marslava
{
	qer_editorimage textures/pad_shop/marslava.tga
	q3map_globaltexture
	surfaceparm trans
	//surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	q3map_surfacelight 140
	cull disable

	tesssize 128
	cull disable
	deformVertexes wave 100 sin 3 2 .1 0.1
	
	{
		map textures/pad_shop/marslava.tga
		tcMod turb 0 .2 0 .1
	}
}


textures/pad_shop/planetstone
{
q3map_nonplanar
q3map_shadeangle 60 l
q3map_surfacelight 4
qer_editorimage textures/pad_shop/planetstone.tga
surfaceparm nomarks
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/planetstone.tga
blendFunc filter
}
{
map textures/pad_shop/planetstone02.tga
blendfunc GL_ONE GL_ONE
}
}

textures/pad_shop/punk
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_shop/punk.tga
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/punk.tga
blendFunc filter
}
}


textures/pad_shop/closed
{
        qer_editorimage textures/pad_shop/closed.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/closed.tga
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


textures/pad_shop/d_midbars3
{
        qer_editorimage textures/pad_shop/d_midbars3.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/d_midbars3.tga
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


textures/pad_shop/d_brnsmal1
{
        qer_editorimage textures/pad_shop/d_brnsmal1.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/d_brnsmal1.tga
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


textures/pad_shop/d_tekgren5
{
	qer_editorimage textures/pad_shop/d_tekgren5.tga
	q3map_surfacelight 10
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren5.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren5_light.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}



textures/pad_shop/d_flat2
{
	qer_editorimage textures/pad_shop/d_flat2.tga
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_flat2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_flat2_light.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_sw1gray
{
	qer_editorimage textures/pad_shop/d_sw1gray.tga
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sw1gray.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sw1gray_light.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_tekgren3
{
	qer_editorimage textures/pad_shop/d_tekgren3.tga
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren3.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren3_light.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_trooper
{
	qer_editorimage textures/pad_shop/d_trooper.tga
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_trooper.tga
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_imp
{
	qer_editorimage textures/pad_shop/d_imp.tga
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_imp.tga
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_chainsaw_small
{
	qer_editorimage textures/pad_shop/d_chainsaw_small.tga
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_chainsaw_small.tga
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_sky1
{
	qer_editorimage textures/pad_shop/d_sky1.tga
	q3map_surfacelight 10
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sky1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sky1_light.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/pc-ground-zero512
{
        qer_editorimage textures/pad_shop/pc-ground-zero512.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_shop/blue.tga
	q3map_sun	0.266383 0.274632 0.358662 20 50 55
	q3map_surfacelight 300

        skyparms env/pc-ground-zero512 - -
//       {
//		map textures/pad_petesky/pc-ground-zero512.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_shop/terrain_0
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight
	{
		map textures/pad_shop/planetstone.tga
		rgbGen vertex
		tcmod scale 0.352 0.352
	}
}

textures/pad_shop/terrain_1
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight
	{
		map textures/pad_shop/planetstoneb.tga
		rgbGen vertex
		tcmod scale 0.352 0.352
	}
}

textures/pad_shop/terrain_0to1
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight
	{
		map textures/pad_shop/planetstone.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.352 0.352
	}
	{
		map textures/pad_shop/planetstoneb.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.352 0.352
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/pad_shop/terrain_vertex
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight
	{
		map textures/pad_shop/planetstone.tga
		rgbGen vertex
		tcmod scale 0.352 0.352
	}
}


textures/pad_shop/sparks
{
	qer_editorimage textures/pad_shop/sparks.tga
        deformVertexes move 2 3 1.5  sin 0 5 0 0.3
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	qer_trans 0.5
	{
		map textures/pad_shop/sparks.tga
		blendFunc add
		rgbGen identity
		tcMod scroll -0.1 0.6
		tcMod turb 0.1 0.25 0 -0.1
	} 
}


textures/pad_shop/padnight
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_shop/red.tga
	q3map_sun	1 1 1 100 -58 58
	q3map_surfacelight 100

        skyparms env/mercury512 - -
       {
		map textures/pad_shop/skystuff.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.07 0.09
		tcMod scale 3 2
	}
}


textures/pad_shop/lavafall
{
	qer_editorimage textures/pad_shop/marslavafa.tga
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull disable
	qer_trans 0.6
                q3map_surfacelight 140
	q3map_globaltexture
	{
		map textures/pad_shop/marslava.tga
		blendfunc add
		tcMod scale 0.5 0.5
		tcMod turb 0.20 0.02 0 0.3
		tcMod scroll 0 -0.1
	}
	{
		map textures/pad_shop/marslavafa.tga
		blendfunc filter
		tcMod scale 0.5 0.5
		tcMod turb 0.25 0.03 0 0.5
		tcMod scroll 0 -0.3
	}
}


textures/pad_shop/tuerchen
{
        surfaceparm trans	
	cull none
	qer_trans 	0.5
     
        {
		map textures/pad_shop/tuerchen.tga
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_shop/blueborder
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/blueborder.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_shop/blueborder2
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/blueborder2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_shop/blueborder3
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/blueborder3.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_shop/pappe
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/pappe.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_shop/venti04b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/venti04b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_shop/wood002
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/wood002.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_shop/muelltonne
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/muelltonne.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 



textures/pad_shop/alienmoni
{
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
	qer_editorimage textures/pad_shop/alienmoni01.tga
	

	{
		animMap 3 textures/pad_shop/alienmoni01.tga textures/pad_shop/alienmoni02.tga textures/pad_shop/alienmoni03.tga textures/pad_shop/alienmoni04.tga textures/pad_shop/alienmoni05.tga textures/pad_shop/alienmoni06.tga textures/pad_shop/alienmoni09.tga textures/pad_shop/alienmoni09.tga
		blendFunc GL_ONE GL_ONE

		
	}	
	{
		animMap 3 textures/pad_shop/alienmoni01.tga textures/pad_shop/alienmoni02.tga textures/pad_shop/alienmoni03.tga textures/pad_shop/alienmoni04.tga textures/pad_shop/alienmoni05.tga textures/pad_shop/alienmoni06.tga textures/pad_shop/alienmoni09.tga textures/pad_shop/alienmoni09.tga
		blendFunc GL_ONE GL_ONE

	}	


	{
		map textures/pad_shop/alienmoniball.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6	
              }
	{
		map textures/pad_shop/line01.tga
		blendfunc add
		rgbGen identity
		tcmod scroll 0 -1
              }
	{
		map textures/pad_shop/line02.tga
		blendfunc add
		rgbGen identity
		tcmod scroll 0.2 0
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/impscreen
{
	qer_editorimage	textures/pad_shop/impscreen.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/impscreen.tga
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/greenscreen02
{
	qer_editorimage	textures/pad_shop/greenscreen02.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen02.tga
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen03
{
	qer_editorimage	textures/pad_shop/greenscreen03.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen03.tga
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen04
{
	qer_editorimage	textures/pad_shop/greenscreen04.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen04.tga
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/greenscreen05
{
	qer_editorimage	textures/pad_shop/greenscreen05.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen05.tga
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen06
{
	qer_editorimage	textures/pad_shop/greenscreen06.tga
	deformVertexes autoSprite2 
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen06.tga
        blendFunc GL_ONE GL_ONE
        rgbGen wave inverseSawtooth 0 1 0 10
	}

}


textures/pad_shop/greenline
{
	qer_editorimage	textures/pad_shop/greenline.tga
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenline.tga
        tcmod scroll 0 -0.3
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/yellight1
{
	qer_editorimage textures/pad_shop/yellight1.tga
	q3map_lightimage textures/pad_shop/yellight2.tga
	surfaceparm nomarks
              q3map_flareShader flareShader
	q3map_lightRGB 102 204 255
              q3map_surfacelight 400
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/yellight1.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{	
		map textures/pad_shop/yellight2.tga
		blendfunc GL_ONE GL_ONE
	}
}



textures/pad_shop/game_ball
{
	qer_editorimage textures/pad_shop/game_ball.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ball.tga
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


textures/pad_shop/game_ballb
{
	qer_editorimage textures/pad_shop/game_ballb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ballb.tga
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


textures/pad_shop/game_bird
{
	qer_editorimage textures/pad_shop/game_bird.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_bird.tga
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


textures/pad_shop/game_birdb
{
	qer_editorimage textures/pad_shop/game_birdb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_birdb.tga
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


textures/pad_shop/game_boom2
{
	qer_editorimage textures/pad_shop/game_boom2.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_boom2.tga
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


textures/pad_shop/game_darkboy
{
	qer_editorimage textures/pad_shop/game_darkboy.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_darkboy.tga
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


textures/pad_shop/game_darkboyb
{
	qer_editorimage textures/pad_shop/game_darkboyb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_darkboyb.tga
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


textures/pad_shop/game_duckz
{
	qer_editorimage textures/pad_shop/game_duckz.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_duckz.tga
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

textures/pad_shop/game_duckzb
{
	qer_editorimage textures/pad_shop/game_duckzb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_duckzb.tga
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


textures/pad_shop/game_killd
{
	qer_editorimage textures/pad_shop/game_killd.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_killd.tga
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


textures/pad_shop/game_killdb
{
	qer_editorimage textures/pad_shop/game_killdb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_killdb.tga
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


textures/pad_shop/game_lilly
{
	qer_editorimage textures/pad_shop/game_lilly.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_lilly.tga
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


textures/pad_shop/game_lillyb
{
	qer_editorimage textures/pad_shop/game_lillyb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_lillyb.tga
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


textures/pad_shop/game_missing
{
	qer_editorimage textures/pad_shop/game_missing.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_missing.tga
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


textures/pad_shop/game_missingb
{
	qer_editorimage textures/pad_shop/game_missingb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_missingb.tga
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


textures/pad_shop/game_nolf1c
{
	qer_editorimage textures/pad_shop/game_nolf1c.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nolf1c.tga
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


textures/pad_shop/game_nopad
{
	qer_editorimage textures/pad_shop/game_nopad.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopad.tga
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


textures/pad_shop/game_nopadb
{
	qer_editorimage textures/pad_shop/game_nopadb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopadb.tga
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


textures/pad_shop/game_nopadc
{
	qer_editorimage textures/pad_shop/game_nopadc.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopadc.tga
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


textures/pad_shop/game_padcry2
{
	qer_editorimage textures/pad_shop/game_padcry2.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padcry2.tga
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


textures/pad_shop/game_padcry2b
{
	qer_editorimage textures/pad_shop/game_padcry2b.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padcry2b.tga
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


textures/pad_shop/game_pims1
{
	qer_editorimage textures/pad_shop/game_pims1.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_pims1.tga
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


textures/pad_shop/game_pims2
{
	qer_editorimage textures/pad_shop/game_pims2.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_pims2.tga
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


textures/pad_shop/game_planet
{
	qer_editorimage textures/pad_shop/game_planet.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_planet.tga
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


textures/pad_shop/game_planetb
{
	qer_editorimage textures/pad_shop/game_planetb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_planetb.tga
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


textures/pad_shop/game_ptrek
{
	qer_editorimage textures/pad_shop/game_ptrek.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ptrek.tga
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


textures/pad_shop/game_ptrekb
{
	qer_editorimage textures/pad_shop/game_ptrekb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ptrekb.tga
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


textures/pad_shop/game_punk
{
	qer_editorimage textures/pad_shop/game_punk.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_punk.tga
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


textures/pad_shop/game_punkb
{
	qer_editorimage textures/pad_shop/game_punkb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_punkb.tga
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


textures/pad_shop/game_redpadz
{
	qer_editorimage textures/pad_shop/game_redpadz.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_redpadz.tga
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


textures/pad_shop/game_redpadzb
{
	qer_editorimage textures/pad_shop/game_redpadzb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_redpadzb.tga
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


textures/pad_shop/game_spiderwalk
{
	qer_editorimage textures/pad_shop/game_spiderwalk.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_spiderwalk.tga
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


textures/pad_shop/game_spiderwalkb
{
	qer_editorimage textures/pad_shop/game_spiderwalkb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_spiderwalkb.tga
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


textures/pad_shop/game_wop02
{
	qer_editorimage textures/pad_shop/game_wop02.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_wop02.tga
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


textures/pad_shop/game_wop2
{
	qer_editorimage textures/pad_shop/game_wop2.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_wop2.tga
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


textures/pad_shop/game_xman
{
	qer_editorimage textures/pad_shop/game_xman.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_xman.tga
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


textures/pad_shop/game_l4p
{
	qer_editorimage textures/pad_shop/game_l4p.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_l4p.tga
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


textures/pad_shop/game_l4pb
{
	qer_editorimage textures/pad_shop/game_l4pb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_l4pb.tga
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


textures/pad_shop/game_padwar
{
	qer_editorimage textures/pad_shop/game_padwar.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padwar.tga
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

textures/pad_shop/game_padwarb
{
	qer_editorimage textures/pad_shop/game_padwarb.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padwarb.tga
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