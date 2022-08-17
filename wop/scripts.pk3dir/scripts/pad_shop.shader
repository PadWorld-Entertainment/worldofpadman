textures/pad_shop/light_blue
{
    q3map_surfacelight 1000
    surfaceparm nolightmap
    {
        map textures/pad_shop/light_blue
    }
}

textures/pad_shop/marslava
{
	qer_editorimage textures/pad_shop/marslava
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
		map textures/pad_shop/marslava
		tcMod turb 0 .2 0 .1
	}
}


textures/pad_shop/planetstone
{
q3map_nonplanar
q3map_shadeangle 60 l
q3map_surfacelight 4
qer_editorimage textures/pad_shop/planetstone
surfaceparm nomarks
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/planetstone
blendFunc filter
}
{
map textures/pad_shop/planetstone02
blendfunc GL_ONE GL_ONE
}
}

textures/pad_shop/punk
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_shop/punk
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_shop/punk
blendFunc filter
}
}


textures/pad_shop/closed
{
        qer_editorimage textures/pad_shop/closed
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/closed
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
        qer_editorimage textures/pad_shop/d_midbars3
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/d_midbars3
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
        qer_editorimage textures/pad_shop/d_brnsmal1
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_shop/d_brnsmal1
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
	qer_editorimage textures/pad_shop/d_tekgren5
	q3map_surfacelight 10
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren5
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren5_light
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}



textures/pad_shop/d_flat2
{
	qer_editorimage textures/pad_shop/d_flat2
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_flat2
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_flat2_light
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_sw1gray
{
	qer_editorimage textures/pad_shop/d_sw1gray
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sw1gray
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sw1gray_light
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_tekgren3
{
	qer_editorimage textures/pad_shop/d_tekgren3
	q3map_surfacelight 5
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren3
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_tekgren3_light
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/d_trooper
{
	qer_editorimage textures/pad_shop/d_trooper
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_trooper
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_imp
{
	qer_editorimage textures/pad_shop/d_imp
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_imp
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_chainsaw_small
{
	qer_editorimage textures/pad_shop/d_chainsaw_small
              surfaceparm nonsolid
	cull disable
	deformvertexes autosprite

{
	map textures/pad_shop/d_chainsaw_small
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen vertex
	}
}


textures/pad_shop/d_sky1
{
	qer_editorimage textures/pad_shop/d_sky1
	q3map_surfacelight 10
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sky1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/d_sky1_light
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_shop/pc-ground-zero512
{
	qer_editorimage env/pc-ground-zero512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_2f8fcfff
	q3map_sun 0.266383 0.274632 0.358662 20 50 55
	q3map_surfacelight 300

	skyparms env/pc-ground-zero512 - -
}


textures/pad_shop/terrain_0
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight
	{
		map textures/pad_shop/planetstone
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
		map textures/pad_shop/planetstoneb
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
		map textures/pad_shop/planetstone
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.352 0.352
	}
	{
		map textures/pad_shop/planetstoneb
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
		map textures/pad_shop/planetstone
		rgbGen vertex
		tcmod scale 0.352 0.352
	}
}


textures/pad_shop/sparks
{
	qer_editorimage textures/pad_shop/sparks
        deformVertexes move 2 3 1.5  sin 0 5 0 0.3
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	qer_trans 0.5
	{
		map textures/pad_shop/sparks
		blendFunc add
		rgbGen identity
		tcMod scroll -0.1 0.6
		tcMod turb 0.1 0.25 0 -0.1
	}
}

textures/pad_shop/padnight
{
	qer_editorimage env/mercury512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/colors/hex_cc0000ff
	q3map_sun 1 1 1 100 -58 58
	q3map_surfacelight 100

	skyparms env/mercury512 - -
	{
		map textures/pad_shop/skystuff
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.07 0.09
		tcMod scale 3 2
	}
}

textures/pad_shop/lavafall
{
	qer_editorimage textures/pad_shop/marslavafa
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
	cull disable
	qer_trans 0.6
                q3map_surfacelight 140
	q3map_globaltexture
	{
		map textures/pad_shop/marslava
		blendfunc add
		tcMod scale 0.5 0.5
		tcMod turb 0.20 0.02 0 0.3
		tcMod scroll 0 -0.1
	}
	{
		map textures/pad_shop/marslavafa
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
		map textures/pad_shop/tuerchen
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
map textures/pad_shop/blueborder
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
map textures/pad_shop/blueborder2
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
map textures/pad_shop/blueborder3
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
map textures/pad_shop/pappe
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
map textures/pad_shop/venti04b
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
map textures/pad_shop/wood002
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
map textures/pad_shop/muelltonne
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}



textures/pad_shop/alienmoni
{
	qer_editorimage textures/pad_shop/alienmoni01
	surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
	{
		animMap 2 textures/pad_shop/alienmoni01 textures/pad_shop/alienmoni02 textures/pad_shop/alienmoni03 textures/pad_shop/alienmoni04 textures/pad_shop/alienmoni05 textures/pad_shop/alienmoni06 textures/pad_shop/alienmoni07 textures/pad_shop/alienmoni08 textures/pad_shop/alienmoni09 textures/pad_shop/alienmoni09
		blendFunc GL_ONE GL_ONE
	}
	{
		animMap 2 textures/pad_shop/alienmoni01 textures/pad_shop/alienmoni02 textures/pad_shop/alienmoni03 textures/pad_shop/alienmoni04 textures/pad_shop/alienmoni05 textures/pad_shop/alienmoni06 textures/pad_shop/alienmoni07 textures/pad_shop/alienmoni08 textures/pad_shop/alienmoni09 textures/pad_shop/alienmoni09
		blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_shop/alienmoniball
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6
              }
	{
		map textures/pad_shop/line01
		blendfunc add
		rgbGen identity
		tcmod scroll 0 -1
	}
	{
		map textures/pad_shop/line02
		blendfunc add
		rgbGen identity
		tcmod scroll 0.2 0
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/impscreen
{
	qer_editorimage	textures/pad_shop/impscreen
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/impscreen
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/greenscreen02
{
	qer_editorimage	textures/pad_shop/greenscreen02
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen02
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen03
{
	qer_editorimage	textures/pad_shop/greenscreen03
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen03
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad3
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen04
{
	qer_editorimage	textures/pad_shop/greenscreen04
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen04
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_shop/greenscreen05
{
	qer_editorimage	textures/pad_shop/greenscreen05
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen05
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/greenscreen06
{
	qer_editorimage	textures/pad_shop/greenscreen06
	deformVertexes autoSprite2
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenscreen06
        blendFunc GL_ONE GL_ONE
        rgbGen wave inverseSawtooth 0 1 0 10
	}

}


textures/pad_shop/greenline
{
	qer_editorimage	textures/pad_shop/greenline
              surfaceparm trans
	q3map_nolightmap
	q3map_surfacelight 40
    {
        map textures/pad_shop/greenline
        tcmod scroll 0 -0.3
        blendFunc GL_ONE GL_ONE
	}
	{
		map textures/pad_gfx02/tinpad4
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_shop/yellight1
{
	qer_editorimage textures/pad_shop/yellight1
	q3map_lightimage textures/pad_shop/yellight2
	surfaceparm nomarks
              q3map_flareShader flareShader
	q3map_lightRGB 102 204 255
              q3map_surfacelight 400
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/yellight1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_shop/yellight2
		blendfunc GL_ONE GL_ONE
	}
}



textures/pad_shop/game_ball
{
	qer_editorimage textures/pad_shop/game_ball

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ball
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


textures/pad_shop/game_ballb
{
	qer_editorimage textures/pad_shop/game_ballb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ballb
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


textures/pad_shop/game_bird
{
	qer_editorimage textures/pad_shop/game_bird

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_bird
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


textures/pad_shop/game_birdb
{
	qer_editorimage textures/pad_shop/game_birdb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_birdb
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


textures/pad_shop/game_boom2
{
	qer_editorimage textures/pad_shop/game_boom2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_boom2
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


textures/pad_shop/game_darkboy
{
	qer_editorimage textures/pad_shop/game_darkboy

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_darkboy
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


textures/pad_shop/game_darkboyb
{
	qer_editorimage textures/pad_shop/game_darkboyb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_darkboyb
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


textures/pad_shop/game_duckz
{
	qer_editorimage textures/pad_shop/game_duckz

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_duckz
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

textures/pad_shop/game_duckzb
{
	qer_editorimage textures/pad_shop/game_duckzb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_duckzb
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


textures/pad_shop/game_killd
{
	qer_editorimage textures/pad_shop/game_killd

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_killd
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


textures/pad_shop/game_killdb
{
	qer_editorimage textures/pad_shop/game_killdb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_killdb
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


textures/pad_shop/game_lilly
{
	qer_editorimage textures/pad_shop/game_lilly

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_lilly
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


textures/pad_shop/game_lillyb
{
	qer_editorimage textures/pad_shop/game_lillyb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_lillyb
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


textures/pad_shop/game_missing
{
	qer_editorimage textures/pad_shop/game_missing

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_missing
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


textures/pad_shop/game_missingb
{
	qer_editorimage textures/pad_shop/game_missingb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_missingb
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


textures/pad_shop/game_nopad
{
	qer_editorimage textures/pad_shop/game_nopad

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopad
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


textures/pad_shop/game_nopadb
{
	qer_editorimage textures/pad_shop/game_nopadb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopadb
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


textures/pad_shop/game_nopadc
{
	qer_editorimage textures/pad_shop/game_nopadc

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_nopadc
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


textures/pad_shop/game_padcry2
{
	qer_editorimage textures/pad_shop/game_padcry2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padcry2
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


textures/pad_shop/game_padcry2b
{
	qer_editorimage textures/pad_shop/game_padcry2b

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padcry2b
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


textures/pad_shop/game_pims1
{
	qer_editorimage textures/pad_shop/game_pims1

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_pims1
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


textures/pad_shop/game_pims2
{
	qer_editorimage textures/pad_shop/game_pims2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_pims2
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


textures/pad_shop/game_planet
{
	qer_editorimage textures/pad_shop/game_planet

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_planet
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


textures/pad_shop/game_planetb
{
	qer_editorimage textures/pad_shop/game_planetb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_planetb
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


textures/pad_shop/game_ptrek
{
	qer_editorimage textures/pad_shop/game_ptrek

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ptrek
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


textures/pad_shop/game_ptrekb
{
	qer_editorimage textures/pad_shop/game_ptrekb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_ptrekb
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


textures/pad_shop/game_punk
{
	qer_editorimage textures/pad_shop/game_punk

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_punk
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


textures/pad_shop/game_punkb
{
	qer_editorimage textures/pad_shop/game_punkb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_punkb
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


textures/pad_shop/game_redpadz
{
	qer_editorimage textures/pad_shop/game_redpadz

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_redpadz
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


textures/pad_shop/game_redpadzb
{
	qer_editorimage textures/pad_shop/game_redpadzb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_redpadzb
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


textures/pad_shop/game_spiderwalk
{
	qer_editorimage textures/pad_shop/game_spiderwalk

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_spiderwalk
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


textures/pad_shop/game_spiderwalkb
{
	qer_editorimage textures/pad_shop/game_spiderwalkb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_spiderwalkb
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


textures/pad_shop/game_wop02
{
	qer_editorimage textures/pad_shop/game_wop02

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_wop02
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


textures/pad_shop/game_wop2
{
	qer_editorimage textures/pad_shop/game_wop2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_wop2
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


textures/pad_shop/game_xman
{
	qer_editorimage textures/pad_shop/game_xman

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_xman
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


textures/pad_shop/game_l4p
{
	qer_editorimage textures/pad_shop/game_l4p

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_l4p
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


textures/pad_shop/game_l4pb
{
	qer_editorimage textures/pad_shop/game_l4pb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_l4pb
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


textures/pad_shop/game_padwar
{
	qer_editorimage textures/pad_shop/game_padwar

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padwar
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

textures/pad_shop/game_padwarb
{
	qer_editorimage textures/pad_shop/game_padwarb

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_shop/game_padwarb
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