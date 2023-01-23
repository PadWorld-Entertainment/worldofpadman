textures/pad_glowsky/padbox
{
	q3map_editorimage env/padboxglow_up
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/pad_glowsky/padbox_white
	q3map_sun 1 1 1 100 -58 58
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
	qer_editorimage env/utopiaatoll512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_ffcfafff
	q3map_sun 0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 200

	skyparms env/utopiaatoll512 - -
}

textures/pad_glowsky/wolfl
{
	qer_editorimage env/wolf-pack512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_5d5d5dff
//	q3map_sun 0.266383 0.274632 0.358662 150 60 85
	q3map_surfacelight 20

	skyparms env/wolf-pack512 - -
}

textures/pad_glowsky/urban-terror
{
	qer_editorimage env/urban-terror512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_da7d27ff
	q3map_sun 0.266383 0.274632 0.358662 90 90 90
	q3map_surfacelight 20

	skyparms env/urban-terror512 - -
}

textures/pad_backyard/pf-tornado-alley
{
	qer_editorimage env/pf-tornado-alley512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_373737ff
	q3map_sun 0.266383 0.274632 0.358662 100 230 70
	q3map_surfacelight 175

	skyparms env/pf-tornado-alley512 - -
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

textures/pad_glowsky/glass
{
	qer_editorimage textures/pad_glowsky/glass
	surfaceparm trans
	q3map_nolightmap
	q3map_onlyvertexlighting
	{
		map textures/pad_glowsky/glass
		blendFunc GL_ONE GL_ONE
	}
}

textures/pad_glow/glass
{
	qer_editorimage textures/colors/hex_87aba380
	cull back
	{
		map textures/colors/hex_87aba380
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
}

textures/pad_glow/greenglas
{
	qer_editorimage textures/colors/hex_87aba380
	cull back
	{
		map textures/colors/hex_87aba380
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
	{
		map textures/colors/hex_87aba380
		blendfunc filter
	}
}

textures/pad_glow/milchglas
{
	qer_editorimage textures/colors/hex_87aba380
	cull back
	{
		map textures/colors/hex_87aba380
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
	{
		map textures/colors/hex_87aba380
		blendfunc filter
	}
	{
		map textures/colors/hex_87aba380
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

textures/pad_backyard/metall1
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/metall1
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_backyard/metall2
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_backyard/metall2
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
