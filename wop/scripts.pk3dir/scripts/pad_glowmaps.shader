textures/pad_glowsky/padbox
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_glowsky/padbox_white
	q3map_sun	1 1 1 100 -58 58
	q3map_surfacelight 90

        skyparms env/padboxglow - -
       {
		map textures/pad_glowsky/sky
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.02
		tcMod scale 1 2
	}
       {
		map textures/pad_glowsky/sky2
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.02 -0.01
		tcMod scale 1 2
	}
}




textures/pad_glowsky/utopiaatoll
{
        qer_editorimage textures/pad_glowsky/utopiaatoll


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/orange01
	q3map_sun	0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 200

        skyparms env/utopiaatoll512 - -
//       {
//		map textures/pad_glowsky/utopiaatoll
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_petesky/wolfl
{
        qer_editorimage textures/pad_petesky/wolf


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/grey
//	q3map_sun	0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 20

        skyparms env/wolf-pack512 - -
//       {
//		map textures/pad_petesky/wolfl
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_glowsky/urban-terror
{
        qer_editorimage textures/pad_glowsky/urban-terror


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/orange03
	q3map_sun	0.266383 0.274632 0.358662 90 90 90
	q3map_surfacelight 20

        skyparms env/urban-terror512 - -
//       {
//		map textures/pad_glowsky/urban-terror
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_glowsky/sandtrap
{
        qer_editorimage textures/pad_glowsky/sandtrap


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_glowsky/blue
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 2

        skyparms env/sandtrap512 - -
//       {
//		map textures/pad_glowsky/sandtrap
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}




textures/pad_glowsky/gglass03
{
	qer_editorimage textures/pad_glowsky/gglass03
        surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5

        {
		map textures/pad_glowsky/gglass03
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
	qer_editorimage textures/pad_glowsky/gglass01
        surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5

        {
		map textures/pad_glowsky/gglass01
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
	qer_editorimage textures/pad_glowsky/gglass02
        surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5

        {
		map textures/pad_glowsky/gglass02
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
        qer_editorimage textures/pad_backyard/pf-tornado-alley


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_backyard/grey
	q3map_sun	0.266383 0.274632 0.358662 100 230 70
	q3map_surfacelight 175

        skyparms env/pf-tornado-alley512 - -
//       {
//		map textures/pad_backyard/pf-tornado-alley
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_glowsky/glass
{
   qer_editorimage   textures/pad_glowsky/glass
              surfaceparm trans
   q3map_nolightmap
   q3map_onlyvertexlighting
    {
        map textures/pad_glowsky/glass
        blendFunc GL_ONE GL_ONE
    }
}


textures/glowstar/glas
{
	qer_editorimage textures/colors/greygreen
	cull back
	{
		map textures/colors/greygreen
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
}

textures/glowstar/greenglas
{
	qer_editorimage textures/colors/greygreen
	cull back
	{
		map textures/colors/greygreen
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
	{
		map textures/colors/greygreen
		blendfunc filter
	}
}

textures/glowstar/milchglas
{
	qer_editorimage textures/colors/greygreen
	cull back
	{
		map textures/colors/greygreen
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
	{
		map textures/colors/greygreen
		blendfunc filter
	}
	{
		map textures/colors/greygreen
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
map textures/pad_glow/liege
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_glow/marble01
{
	qer_editorimage textures/pad_glow/marble01

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble01
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_glow/marble04
{
	qer_editorimage textures/pad_glow/marble04

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble04
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_glow/marble06
{
	qer_editorimage textures/pad_glow/marble06

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble06
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble07
{
	qer_editorimage textures/pad_glow/marble07

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble07
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble08
{
	qer_editorimage textures/pad_glow/marble08

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble08
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble21
{
	qer_editorimage textures/pad_glow/marble21

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble21
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_glow/marble27
{
	qer_editorimage textures/pad_glow/marble27

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/marble27
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_cabin/Moni
{
	q3map_lightimage textures/pad_cabin/Moni
	q3map_surfacelight 20

        {
		map textures/pad_cabin/Moni
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
map textures/pad_backyard/Holz00y
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
map textures/pad_backyard/Holz00y
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
map textures/pad_backyard/planke02
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
map textures/pad_backyard/wood0a
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
map textures/pad_backyard/wood0b
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
map textures/pad_backyard/wood0c
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
map textures/pad_backyard/wood0d
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
map textures/pad_backyard/wood0e
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
map textures/pad_backyard/wood0f
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
map textures/pad_backyard/wood0g
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
map textures/pad_backyard/wood0h
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
map textures/pad_backyard/wood0i
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
map textures/pad_backyard/wood0j
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
map textures/pad_backyard/wood01
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
map textures/pad_backyard/wood02
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
map textures/pad_backyard/glow_rost01
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
map textures/pad_backyard/metal02
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
map textures/pad_backyard/metal04
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
map textures/pad_backyard/metal06
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
map textures/pad_backyard/Metal01
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
map textures/pad_backyard/Metal2
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
map textures/pad_backyard/rau
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
map textures/pad_cabin/darkrost
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
map textures/pad_cabin/metal
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
map textures/pad_cabin/metal1
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
map textures/pad_cabin/metall
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
map textures/pad_cabin/rost001
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
map textures/pad_cabin/fensterbank
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
map textures/pad_cabin/wood0a
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
map textures/pad_cabin/wood0b
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
map textures/pad_cabin/wood0c
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
map textures/pad_cabin/wood0d
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
map textures/pad_glow/wallcol016
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
map textures/pad_glow/wallcol017
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_glowsky/glowscreen
{
	q3map_lightimage textures/pad_glowsky/glowscreen
	q3map_surfacelight 20

        {
		map textures/pad_glowsky/glowscreen
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
	qer_editorimage textures/pad_glow/metal_old

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_glow/metal_old
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}