textures/pad_tex02/padjump_high
{
	qer_editorimage textures/pad_tex02/padjump_high
	{
		map textures/pad_tex02/padjump_high
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/pad_tex02/padjump_light03
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 1 0 1
	}
}

textures/pad_tex02/padjump_highred
{
	qer_editorimage textures/pad_tex02/padjump_highred
	{
		map textures/pad_tex02/padjump_highred
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/pad_tex02/padjump_light01
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 1 0 1
	}
}

textures/pad_tex02/telepad
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_tex02/telepad
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_tex02/padjump_fly
{


	{
		map $lightmap
		rgbGen identity
	}

	{
		map textures/pad_tex02/padjump_fly
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}

	{
		map textures/pad_tex02/padjump_light02b
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 1 0 1
	}

	{
		animmap 4 textures/pad_tex02/padjump_start textures/pad_tex02/launchpad_arrow2 textures/pad_tex02/launchpad_arrow2 textures/pad_tex02/launchpad_arrow2
		blendfunc gl_one gl_one
		tcmod scroll 0 2
	}

}

textures/pad_tex02/padjump_flyred
{

	{
		map $lightmap
		rgbGen identity
	}

	{
		map textures/pad_tex02/padjump_flyred
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}

	{
		map textures/pad_tex02/padjump_light02
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 1 0 1
	}


	{
		animmap 4 textures/pad_tex02/padjump_start textures/pad_tex02/launchpad_arrow2 textures/pad_tex02/launchpad_arrow2 textures/pad_tex02/launchpad_arrow2
		blendfunc gl_one gl_one
		tcmod scroll 0 2
	}

}

textures/pad_tex02/padani
{



	surfaceparm nodamage
	q3map_surfacelight 200


	{
		map textures/pad_tex02/padani
		rgbGen identity
	}

	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}

	{
		animMap 2 textures/pad_tex02/padani_log textures/pad_tex02/padani_log02 textures/pad_tex02/padani_log03
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}

}

textures/pad_tex02/padani_blue
{


	surfaceparm nodamage
	q3map_surfacelight 200


	{
		map textures/pad_tex02/padani_blue
		rgbGen identity
	}

	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}

	{
		animMap 2 textures/pad_tex02/padani_log textures/pad_tex02/padani_log02 textures/pad_tex02/padani_log03
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}

}



textures/pad_tex02/padani_red
{



	surfaceparm nodamage
	q3map_surfacelight 200


	{
		map textures/pad_tex02/padani_red
		rgbGen identity
	}

	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}

	{
		animMap 2 textures/pad_tex02/padani_log textures/pad_tex02/padani_log02 textures/pad_tex02/padani_log03
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}

}

textures/pad_tex02/padlolly_red
{

	qer_editorimage textures/pad_tex02/padani_red
	surfaceparm nodamage
	q3map_surfacelight 200


	{
		map textures/pad_tex02/padani_red
		rgbGen identity
	}

	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}

	{
		animMap 2 textures/pad_tex02/padani_flaglog01 textures/pad_tex02/padani_flaglog03
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}

}

textures/pad_tex02/padlolly_blue
{
	qer_editorimage textures/pad_tex02/padani_blue
	surfaceparm nodamage
	q3map_surfacelight 200


	{
		map textures/pad_tex02/padani_blue
		rgbGen identity
	}

	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}

	{
		animMap 2 textures/pad_tex02/padani_flaglog04 textures/pad_tex02/padani_flaglog02
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}

}

textures/pad_tex02/padbox3
{
	q3map_editorimage env/padbox3_up
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/pad_tex02/padbox_yel
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

	skyparms env/padbox3 - -
	{
		map textures/pad_tex02/skystuff3
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.03
		tcMod scale 1 2
	}
	{
		map textures/pad_tex02/skystuff3b
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}

textures/pad_tex02/padbox4
{
	q3map_editorimage env/padbox4_up
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/pad_tex02/padbox_blu
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

	skyparms env/padbox4 - -
	{
		map textures/pad_tex02/skystuff4
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.02 0.04
		tcMod scale 1 2
	}
}

textures/pad_tex02/padbox5
{
	q3map_editorimage env/padbox5_up
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/pad_tex02/padbox_ora
	q3map_sun	1 1 1 190 -58 90
	q3map_surfacelight 200

	skyparms env/padbox5 - -
	{
		map textures/pad_tex02/skystuff2
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.02 0.04
		tcMod scale 1 2
	}
	{
		map textures/pad_tex02/skystuff2b
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}

textures/pad_tex02/padbox6
{
	q3map_editorimage env/padbox6_up
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_lightimage textures/pad_tex02/padbox_yel
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

	skyparms env/padbox6 - -
	{
		map textures/pad_tex02/skystuff6
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.03
		tcMod scale 1 2
	}
	{
		map textures/pad_tex02/skystuff6b
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}


textures/pad_maps/schrank01d
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/schrank01d
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/burgfloor001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/burgfloor001
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/woptrailer
{

    qer_editorimage textures/pad_maps/pad_movie
    surfaceparm trans
    surfaceparm nolightmap
    cull disable

        {
		map textures/pad_gfx02/tinpad3
        tcGen environment
        rgbGen identity
	}

	{
		videoMap wopintro512.roq
		blendFunc add
        rgbGen identity
	}

}


textures/pad_maps/padlamp01
{
	qer_editorimage textures/pad_maps/padlamp01
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp01
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/padlamp02
{
	qer_editorimage textures/pad_maps/padlamp02
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp02
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/padlamp03
{
	qer_editorimage textures/pad_maps/padlamp03
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp03
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/weaplight_blue
{
	qer_editorimage textures/pad_maps/weaplight_blue
	q3map_surfacelight 150
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplight_blue
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplamp_flash
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/weaplight_red
{
	qer_editorimage textures/pad_maps/weaplight_red
	q3map_surfacelight 150
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplight_red
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplamp_flash
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/blobpad02
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_maps/blobpad02
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}

}


textures/pad_tex02/jumploop1
{
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	surfaceparm nonsolid
              surfaceparm trans
	q3map_lightimage textures/pad_tex02/jumploop1
	q3map_surfacelight 100

	{
		map textures/pad_tex02/jumploop2
		blendFunc add
                	rgbgen wave triangle 1.8 2 0 7
                	tcMod scroll 0 0.5
	}
	{
		map textures/pad_tex02/jumploop2
		blendFunc add
                	rgbgen wave triangle 1 1.1 1 5
                	tcMod scale  -1 1.5
                	tcMod scroll 0 1.1
	}
	{
		map textures/pad_tex02/jumploop1
		blendFunc add
                	rgbgen wave triangle 2 1.4 3 7.7
                	tcMod scroll 0 1.5
	}

}


textures/pad_tex02/padbase
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/padbase
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num01
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num01
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num02
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num02
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num03
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num03
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num04
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num04
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num05
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num05
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num06
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num06
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/arrow01
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/arrow01
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/arrow02
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/arrow02
               		blendFunc add
		rgbGen vertex
	}
}



textures/pad_tex02/blueflag
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7

        {
                map textures/pad_tex02/blueflag
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


textures/pad_tex02/redflag
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7

        {
                map textures/pad_tex02/redflag
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

