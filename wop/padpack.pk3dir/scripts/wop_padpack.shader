// ENTE's PadPack shader

// =================
// PadCastle levelshots
// =================

levelshots/wop_padcastleBB
{
    nomipmaps
    {
        map levelshots/wop_padcastle
    }
}

levelshots/wop_padcastleBBA
{
    nomipmaps
    {
        map levelshots/wop_padcastleA
    }
}

levelshots/wop_padcastleBBB
{
    nomipmaps
    {
        map levelshots/wop_padcastleB
    }
}

// =================
// PadGallery levelshots
// =================

levelshots/wop_padgalleryBB
{
    nomipmaps
    {
        map levelshots/wop_padgallery
    }
}

levelshots/wop_padgalleryBBA
{
    nomipmaps
    {
        map levelshots/wop_padgalleryA
    }
}

levelshots/wop_padgalleryBBB
{
    nomipmaps
    {
        map levelshots/wop_padgalleryB
    }
}

// =================
// PadCastle skybox
// =================

textures/pad_shop/nottingham
{
	qer_editorimage env/nottingham1024_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/pad_shop/brown
	q3map_sun 0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 250

		skyparms env/nottingham1024 - -
}

// =================
// PadGallery skybox
// =================

textures/pad_petesky/night-life
{
	qer_editorimage env/pc-night-life1024_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/pad_petesky/white02
	q3map_sun 0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 200

		skyparms env/pc-night-life1024 - -
}


textures/pad_maps/pad_vine
{
	cull disable
	surfaceparm alphashadow
	surfaceparm trans
	surfaceparm nomarks
	tessSize 64

	{
		map textures/pad_maps/pad_vine
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

textures/pad_maps/crosslight
{
	qer_editorimage textures/pad_maps/crosslight
	q3map_lightimage textures/pad_maps/crosslight02
	q3map_surfacelight 4000
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/crosslight
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/crosslight02
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/upperlight
{
	qer_editorimage textures/pad_maps/upperlight
	q3map_lightimage textures/pad_maps/upperlight02
	q3map_surfacelight 4000
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/upperlight
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/upperlight02
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_maps/tilewall001
{
	qer_editorimage textures/pad_maps/tilewall001

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/tilewall001
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_maps/tilewall002
{
	qer_editorimage textures/pad_maps/tilewall002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/tilewall002
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/pad_maps/tilewall004
{
	qer_editorimage textures/pad_maps/tilewall004

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/tilewall004
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_maps/lamp03
{
	q3map_surfacelight 1225
	surfaceparm nolightmap
	{
		map textures/pad_maps/lamp03
	}
}

textures/pad_maps/lamp04
{
	q3map_surfacelight 1225
	surfaceparm nolightmap
	{
		map textures/pad_maps/lamp04
	}
}

textures/pad_maps/lamp05
{
	q3map_surfacelight 1225
	surfaceparm nolightmap
	{
		map textures/pad_maps/lamp05
	}
}

textures/pad_maps/lamp06
{
	q3map_surfacelight 175
	surfaceparm nolightmap
	{
		map textures/pad_maps/lamp06
	}
}

textures/pad_maps/lamp08
{
	q3map_surfacelight 240
	surfaceparm nolightmap
	{
		map textures/pad_maps/lamp08
	}
}


textures/pad_maps/notausgang02
{
	q3map_surfacelight 300
	surfaceparm nolightmap
	{
		map textures/pad_maps/notausgang02
	}
}

textures/pad_maps/notausgang
{
	q3map_surfacelight 300
	surfaceparm nolightmap
	{
		map textures/pad_maps/notausgang
	}
}


textures/pad_maps/notausganglight
{
	q3map_surfacelight 300
	surfaceparm nolightmap
	{
		map textures/pad_maps/notausganglight
	}
}


textures/pad_maps/exit
{
	q3map_surfacelight 300
	surfaceparm nolightmap
	{
		map textures/pad_maps/exit
	}
}


textures/pad_maps/win002
{
	q3map_surfacelight 100
	surfaceparm nolightmap
	{
		map textures/pad_maps/win002
	}
}


textures/pad_maps/padgallery01
{
	q3map_surfacelight 100
	surfaceparm nolightmap
	{
		map textures/pad_maps/padgallery01
	}
}

textures/pad_maps/jumpvator
{
	q3map_surfacelight 300
	surfaceparm nolightmap
	{
		map textures/pad_maps/jumpvator
	}
}


textures/pad_maps/glassblue
{
	qer_editorimage textures/pad_maps/glassblue
	surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5

	{
		map textures/pad_maps/glassblue
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


textures/pad_maps/glassgreen
{
	qer_editorimage textures/pad_maps/glassgreen
	surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5

	{
		map textures/pad_maps/glassgreen
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


textures/pad_gallery/tomoyo003
{
	qer_editorimage textures/pad_gallery/tomoyo003

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/tomoyo003
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


textures/pad_gallery/lara_baumann012
{
	qer_editorimage textures/pad_gallery/lara_baumann012

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann012
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

textures/pad_gallery/padman002
{
	qer_editorimage textures/pad_gallery/padman002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman002
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

textures/pad_gallery/padentertainment2
{
	qer_editorimage textures/pad_gallery/padentertainment2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padentertainment2
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


textures/pad_maps/waytothemuseum
{
	qer_editorimage textures/pad_maps/waytothemuseum

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/waytothemuseum
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


textures/pad_maps/fish01
{
	qer_editorimage textures/pad_maps/fish01
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_maps/fish01
		blendFunc GL_ONE GL_ZERO
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

textures/pad_maps/fish02
{
	qer_editorimage textures/pad_maps/fish02
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_maps/fish02
		blendFunc GL_ONE GL_ZERO
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


textures/pad_maps/fish03
{
	qer_editorimage textures/pad_maps/fish03
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_maps/fish03
		blendFunc GL_ONE GL_ZERO
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


textures/pad_maps/fish04
{
	qer_editorimage textures/pad_maps/fish04
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_maps/fish04
		blendFunc GL_ONE GL_ZERO
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


textures/pad_maps/galllight
{
	q3map_surfacelight 1200
	q3map_flareShader flareShader
	surfaceparm nolightmap
	{
		map textures/pad_maps/galllight
	}
}



textures/pad_maps/fackel
{

		surfaceparm trans
		surfaceparm nomarks
		surfaceparm nonsolid
		qer_editorimage textures/pad_garden/sflame1
		q3map_surfacelight 900
		surfaceparm nolightmap
		cull none

	{
		animMap 10 textures/pad_garden/sflame1 textures/pad_garden/sflame2 textures/pad_garden/sflame3 textures/pad_garden/sflame4 textures/pad_garden/sflame5 textures/pad_garden/sflame6 textures/pad_garden/sflame7 textures/pad_garden/sflame8
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 10

	}
	{
		animMap 10 textures/pad_garden/sflame2 textures/pad_garden/sflame3 textures/pad_garden/sflame4 textures/pad_garden/sflame5 textures/pad_garden/sflame6 textures/pad_garden/sflame7 textures/pad_garden/sflame8 textures/pad_garden/sflame1
		blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
	}


	{
		map textures/pad_garden/sflameball
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6
	}

}


textures/pad_maps/notdoor002
{
	qer_editorimage textures/pad_maps/notdoor002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/notdoor002
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_maps/tafel01
{
	qer_editorimage textures/pad_maps/tafel01

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/tafel01
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


textures/pad_maps/tafel03
{
	qer_editorimage textures/pad_maps/tafel03

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/tafel03
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




textures/pad_maps/lackgrey
{
	qer_editorimage textures/pad_maps/lackgrey

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/lackgrey
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_maps/lackgrey2
{
	qer_editorimage textures/pad_maps/lackgrey2

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/lackgrey2
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad5
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}


textures/pad_maps/feuerl
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/feuerl
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_gallery/padman023
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman023
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/not04
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/not04
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/not05
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/not05
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/nosmoking
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/nosmoking
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/impmaster
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/impmaster
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/notenter
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/notenter
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_maps/jail2maschendrahtzaun
{
	qer_editorimage textures/pad_maps/jail2maschendrahtzaun
	cull disable
		surfaceparm alphashadow
			surfaceparm latticesteps
	{
		map textures/pad_maps/jail2maschendrahtzaun
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


textures/pad_maps/water_aqua
{
	qer_editorimage textures/pad_maps/aqua01
	qer_trans .4
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm nomarks

	{
		map textures/pad_maps/aqua01
		blendFunc GL_dst_color GL_one
		rgbgen identity
		tcmod scale .5 .5
		tcmod scroll .025 .01
	}
	{
		map textures/pad_maps/aqua01
		blendFunc GL_dst_color gl_one
		rgbgen identity
		tcmod scale -1 -1
		tcmod scroll .025 .025
	}
	{
		map $lightmap
		blendFunc GL_dst_color GL_zero
		rgbgen identity
	}
}


textures/pad_maps/moni001
{
	q3map_lightimage textures/pad_maps/moni001
	q3map_surfacelight 2

		{
		map textures/pad_maps/moni001
		rgbGen identity
	}

		{
		map textures/pad_maps/moni002
		blendfunc add
		rgbGen identity
		tcmod scroll 0 1
	}


	{
		map $lightmap
		tcgen environment
		tcmod scale .5 .5
		rgbGen wave sin .25 0 0 0
		blendfunc add
	}
}


textures/pad_maps/shinyblack
{
	qer_editorimage textures/pad_maps/shinyblack

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/shinyblack
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


textures/pad_maps/shinywhite
{
	qer_editorimage textures/pad_maps/shinywhite

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/shinywhite
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


textures/pad_gallery/lara_baumann_001
{
	qer_editorimage textures/pad_gallery/lara_baumann_001

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_001
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

textures/pad_gallery/lara_baumann_002
{
	qer_editorimage textures/pad_gallery/lara_baumann_002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_002
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


textures/pad_gallery/lara_baumann_003
{
	qer_editorimage textures/pad_gallery/lara_baumann_003

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_003
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

textures/pad_gallery/lara_baumann_004
{
	qer_editorimage textures/pad_gallery/lara_baumann_004

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_004
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


textures/pad_gallery/lara_baumann_005
{
	qer_editorimage textures/pad_gallery/lara_baumann_005

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_005
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


textures/pad_gallery/lara_baumann_006
{
	qer_editorimage textures/pad_gallery/lara_baumann_006

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_006
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

textures/pad_gallery/lara_baumann_007
{
	qer_editorimage textures/pad_gallery/lara_baumann_007

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_007
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


textures/pad_gallery/lara_baumann_008
{
	qer_editorimage textures/pad_gallery/lara_baumann_008

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_008
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


textures/pad_gallery/lara_baumann_009_256
{
	qer_editorimage textures/pad_gallery/lara_baumann_009_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_009_256
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


textures/pad_gallery/lara_baumann_010_256
{
	qer_editorimage textures/pad_gallery/lara_baumann_010_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_010_256
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


textures/pad_gallery/lara_baumann_011
{
	qer_editorimage textures/pad_gallery/lara_baumann_011

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_011
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


textures/pad_gallery/lara_baumann_012
{
	qer_editorimage textures/pad_gallery/lara_baumann_012

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_012
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


textures/pad_gallery/lara_baumann_017_256
{
	qer_editorimage textures/pad_gallery/lara_baumann_017_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_017_256
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


textures/pad_gallery/lara_baumann_018_256
{
	qer_editorimage textures/pad_gallery/lara_baumann_018_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/lara_baumann_018_256
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


textures/pad_gallery/ente004
{
	qer_editorimage textures/pad_gallery/ente004

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/ente004
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


textures/pad_gallery/tomoyo001
{
	qer_editorimage textures/pad_gallery/tomoyo001

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/tomoyo001
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


textures/pad_gallery/tomoyo002
{
	qer_editorimage textures/pad_gallery/tomoyo002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/tomoyo002
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


textures/pad_maps/kredit
{
	 cull disable
	 surfaceparm alphashadow
	 surfaceparm trans
	 surfaceparm nomarks
	 tessSize 64


		{
				map textures/pad_maps/kredit
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

textures/pad_maps/burgfloor002
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/burgfloor002
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/burgfloor003
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/burgfloor003
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_maps/burgfloor004
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/burgfloor004
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
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

textures/pad_maps/spint01
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/spint01
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/spint02
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/spint02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_maps/stroh
{
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/stroh
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/woodsteg
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/woodsteg
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_maps/woolwall001_512
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/woolwall001_512
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/buche003
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/buche003
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_maps/buche004
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/buche004
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_maps/redcarp
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/redcarp
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_bookroom/redfloor_256
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_bookroom/redfloor_256
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/padman/welcome
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/padman/welcome
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/grasground3
{
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/grasground3
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_gallery/doomdragon01
{
	qer_editorimage textures/pad_gallery/doomdragon01

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/doomdragon01
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

textures/pad_gallery/doomdragon02
{
	qer_editorimage textures/pad_gallery/doomdragon02

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/doomdragon02
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


textures/pad_gallery/entecartoon006
{
	qer_editorimage textures/pad_gallery/entecartoon006

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/entecartoon006
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


textures/pad_gallery/entecartoon009
{
	qer_editorimage textures/pad_gallery/entecartoon009

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/entecartoon009
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


textures/pad_gallery/gallery2000
{
	qer_editorimage textures/pad_gallery/gallery2000

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/gallery2000
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


textures/pad_gallery/padman005
{
	qer_editorimage textures/pad_gallery/padman005

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman005
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

textures/pad_gallery/padman007b
{
	qer_editorimage textures/pad_gallery/padman007b

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman007b
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


textures/pad_gallery/padman010_256
{
	qer_editorimage textures/pad_gallery/padman010_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman010_256
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


textures/pad_gallery/padman011_256
{
	qer_editorimage textures/pad_gallery/padman011_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman011_256
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


textures/pad_gallery/padman014_512
{
	qer_editorimage textures/pad_gallery/padman014_512

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman014_512
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


textures/pad_gallery/padman016_256
{
	qer_editorimage textures/pad_gallery/padman016_256

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman016_256
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

textures/pad_gallery/padman018
{
	qer_editorimage textures/pad_gallery/padman018

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman018
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

textures/pad_gallery/padman025
{
	qer_editorimage textures/pad_gallery/padman025

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman025
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

textures/pad_gallery/padman026
{
	qer_editorimage textures/pad_gallery/padman026

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gallery/padman026
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

textures/pad_maps/wop02
{
	qer_editorimage textures/pad_maps/wop02

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/wop02
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


textures/pad_maps/wop03
{
	qer_editorimage textures/pad_maps/wop03

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/wop03
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

textures/pad_maps/wop04
{
	qer_editorimage textures/pad_maps/wop04

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/wop04
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

textures/pad_maps/wop32
{
	qer_editorimage textures/pad_maps/wop32

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/wop32
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


textures/pad_maps/xwop34
{
	qer_editorimage textures/pad_maps/xwop34

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/xwop34
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

textures/pad_maps/blackdoor
{
	qer_editorimage textures/pad_maps/blackdoor

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/blackdoor
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

textures/pad_gallery/ente005_256
{
	qer_editorimage textures/pad_gallery/ente005_256
	{
		map textures/pad_gallery/ente005_256
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/pad_gallery/ente005_light
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth 0 1 0 1
	}
}

textures/pad_garden/neon01
{
	qer_editorimage textures/pad_enteflare/neon01
	surfaceparm nomarks
	q3map_flareShader flareShader
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01_blend
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_crytek/yb2
{
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/yb2
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_effect/skinwin
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/skinwin
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/skinwin
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_effect/blacky
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/blacky
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/blacky
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_effect/blatt
{
	cull disable
	surfaceparm alphashadow
//	deformVertexes autoSprite
	surfaceparm trans
	{
		map textures/pad_effect/blatt
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


textures/pad_effect/colabox
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/colabox
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/colabox
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}



textures/pad_effect/blacky01
{
	//deformVertexes wave 100 sin 3 2 .1 3
	{
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/blacky01
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_gfx02/tinpad
		tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

	{
		map textures/pad_effect/blacky01
		//blendFunc GL_ONE GL_ONE
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_liquidfx/water_calm1
{
	qer_editorimage textures/pad_liquidfx/wat01.tga
	qer_trans .5
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm water
	cull disable
	{
		map textures/pad_liquidfx/wat01.tga
		blendFunc GL_dst_color GL_one
		rgbgen identity
		tcmod scale .5 .5
		tcmod transform 1.5 0 1.5 1 1 2
		tcmod scroll -.05 .001
	}
	{
		map textures/pad_liquidfx/wat03.tga
		blendFunc GL_dst_color GL_one
		rgbgen identity
		tcmod scale .5 .5
		tcmod transform 0 1.5 1 1.5 2 1
		tcmod scroll .025 -.001
	}
	{
		map textures/pad_liquidfx/wat01.tga
		blendFunc GL_dst_color GL_one
		rgbgen identity
		tcmod scale .25 .5
		tcmod scroll .001 .025
	}
	{
		map $lightmap
		blendFunc GL_dst_color GL_zero
		rgbgen identity		
	}
}
