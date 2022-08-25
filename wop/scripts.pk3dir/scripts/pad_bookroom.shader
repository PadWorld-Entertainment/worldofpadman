textures/pad_bookroom/fenstergitter
{
	cull disable
	surfaceparm alphashadow
	surfaceparm trans
	surfaceparm nomarks
	tessSize 64

	{
		map textures/pad_bookroom/fenstergitter
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
		map textures/pad_bookroom/fenstergitter003
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
		map textures/pad_bookroom/vinesss
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
	qer_editorimage textures/pad_bookroom/goldstab001
	{
		map textures/pad_bookroom/goldstab001
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/goldstab002
	{
		map textures/pad_bookroom/goldstab002
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/goldstab003
	{
		map textures/pad_bookroom/goldstab003
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/goldstab004
	{
		map textures/pad_bookroom/goldstab004
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/goldstab005
	{
		map textures/pad_bookroom/goldstab005
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/goldstab006
	{
		map textures/pad_bookroom/goldstab006
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bleifenster001
	surfaceparm trans
	cull none
	qer_trans 0.6
	{
		map textures/pad_bookroom/bleifenster001
		blendfunc gl_dst_color gl_zero
	}
}

textures/pad_bookroom/bleifenster002
{
	qer_editorimage textures/pad_bookroom/bleifenster002
	surfaceparm alphashadow
	surfaceparm trans
	surfaceparm nomarks
	tessSize 64
	cull none
	qer_trans 0.6
	{
		map textures/pad_bookroom/bleifenster002
		alphaFunc GE128
		blendfunc gl_dst_color gl_zero
	}
}

textures/pad_bookroom/globusmass
{
	qer_editorimage textures/pad_bookroom/globusmass
	{
		map textures/pad_bookroom/globusmass
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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

textures/pad_bookroom/telepaddy
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_bookroom/telepaddy
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
		map textures/pad_bookroom/blobbypad
		tcGen environment
		tcMod turb 0 0.35 0 0.25
		tcmod scroll -0.6 -0.8
	}
	{
		map textures/pad_bookroom/blobbypad
		tcGen environment
		tcMod turb 0 0.25 0 0.5
		tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_bookroom/bomarble01
{
	qer_editorimage textures/pad_bookroom/bomarble01
	{
		map textures/pad_bookroom/bomarble01
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble01_dark
	{
		map textures/pad_bookroom/bomarble01_dark
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble01b
	{
		map textures/pad_bookroom/bomarble01b
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble01blue
	{
		map textures/pad_bookroom/bomarble01blue
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble01blue_dark
	{
		map textures/pad_bookroom/bomarble01blue_dark
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble01b_blue
	{
		map textures/pad_bookroom/bomarble01b_blue
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/bomarble02
	{
		map textures/pad_bookroom/bomarble02
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad
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
	qer_editorimage textures/pad_bookroom/flystars
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
		map textures/pad_bookroom/flystars
		tcMod Scroll -5 0.1
		tcMod turb .3 .25 0 .1
		blendfunc gl_src_alpha gl_one_minus_src_alpha
	}
	{
		map textures/pad_bookroom/flystars2
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
		map textures/pad_bookroom/pad_flag_blue
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
		map textures/pad_bookroom/pad_flag_red
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
		map textures/pad_bookroom/beam_pink
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
		map textures/pad_bookroom/beam_yellow
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
		map textures/pad_bookroom/portal_sfx3
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//	This blend function is not working on RagePro drivers Mac only
		//	when it is used on top of portals
		depthWrite
	}
	{
		map textures/pad_bookroom/portal_sfx
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 .5 .2 .5
	}
	{
		map textures/pad_bookroom/portalfog
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen portal 1024
		rgbGen identityLighting
//		tcmod turb sin 0 .5 0 1
		tcmod rotate .1 //.1
		tcmod scroll .01 .03
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
		map textures/pad_bookroom/doc_poe_kid
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
		map textures/pad_bookroom/freud_hari_curie
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
		map textures/pad_bookroom/zierkante
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
		map textures/pad_bookroom/floor_pad03
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
		map textures/pad_bookroom/tep002
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
		map textures/pad_bookroom/tep004
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
		map textures/pad_bookroom/dekostoff001
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
		map textures/pad_bookroom/dekostoff002
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
		map textures/pad_bookroom/dekostoff002c
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
		map textures/pad_bookroom/decke005b
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
		map textures/pad_bookroom/gelan001c
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
		map textures/pad_bookroom/gelan003b
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
		map textures/pad_bookroom/gelan005b
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
		map textures/pad_bookroom/gelan006
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
		map textures/pad_bookroom/gelan007
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
		map textures/pad_bookroom/platte02big
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
		map textures/pad_bookroom/reg001
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
		map textures/pad_bookroom/reg002b
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
		map textures/pad_bookroom/reg002c
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
		map textures/pad_bookroom/reg003b
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
		map textures/pad_bookroom/schach001
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
		map textures/pad_bookroom/windowboard2
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
		map textures/pad_bookroom/windownutsmall
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
		map textures/pad_bookroom/woodkante001
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
		map textures/pad_bookroom/woodkante002
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
		map textures/pad_bookroom/woodkante003
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
		map textures/pad_bookroom/woodkante004bigc
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
		map textures/pad_bookroom/woodkante005
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
		map textures/pad_bookroom/woodsockel
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
		map textures/pad_bookroom/woodsockelc
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
		map textures/pad_bookroom/woodzier001b
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
		map textures/pad_bookroom/woodzier002
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
		map textures/pad_bookroom/woodzier003
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
		map textures/pad_bookroom/woodzier005
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
		map textures/pad_bookroom/woodzier005b
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
		map textures/pad_bookroom/woodzier005c
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
		map textures/pad_bookroom/woodzier006
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
		map textures/pad_bookroom/woodzier006b
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
		map textures/pad_bookroom/woodzier007
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
		map textures/pad_bookroom/woodzier008c
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
		map textures/pad_bookroom/woodzier011c
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
		map textures/pad_bookroom/woodzier012
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
		map textures/pad_bookroom/woodzier001b
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
		map textures/pad_bookroom/woodzier015
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
		map textures/pad_bookroom/woodzier018
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
		map textures/pad_bookroom/woodzier019b
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
		map textures/pad_bookroom/woodzier019
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
		map textures/pad_bookroom/wookkante008
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
		map textures/pad_bookroom/woood001
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
		map textures/pad_bookroom/woood002
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
		map textures/pad_bookroom/woood001old2
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
		map textures/pad_wallin/wallcol26
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
		map textures/pad_wood/wood034
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
		map textures/pad_wood/wood039
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
		map textures/pad_wood/wood047
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
		map textures/pad_bookroom/gelaenderstre
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
		map textures/pad_bookroom/tep001
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
		map textures/pad_bookroom/wookkante007
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
		map textures/pad_bookroom/wookkante007b
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
		map textures/pad_bookroom/woodzier003b
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
		map textures/pad_bookroom/gelan001b
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_bookroom/frost-bite
{
	qer_editorimage textures/pad_bookroom/frost_bite


	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/pad_bookroom/frost_color
	q3map_sun	0.754360 0.959199 1.000000 300 50 55
	q3map_surfacelight 300

	skyparms env/frost-bite512 - -
//	{
//		map textures/pad_bookroom/frost-bite
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}