
textures/pad_cyb/mirroreng_01
{
	qer_editorimage textures/pad_cyb/mirroreng_01
	{
		map textures/pad_cyb/mirroreng_01
		rgbGen identity
	}
	{
		map textures/pad_cyb/mirrorglass_02
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


textures/pad_cyb/mirroreng_02
{
	qer_editorimage textures/pad_cyb/mirroreng_02
	{
		map textures/pad_cyb/mirroreng_02
		rgbGen identity
	}
	{
		map textures/pad_cyb/mirrorglass_03
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

textures/pad_cyb/plas_white
{
	qer_editorimage textures/colors/hex_ffffffff
	{
		map textures/colors/hex_ffffffff
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3
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



// ***********************************************************
// *
// *  cybbath: water-shader -> by MopAn, made with particle studio, thanks to the creators
// *
// ***********************************************************
textures/pad_cyb/waterstream
{
qer_editorimage textures/pad_cyb/waterstream
q3map_globaltexture
surfaceparm nonsolid
surfaceparm nolightmap
surfaceparm trans
surfaceparm noimpact
surfaceparm water
tessSize 32
deformVertexes move 0.2 0.5 0 sin 0 1 0.2 0.2
deformVertexes move 0.5 0.3 0 sin 0 1 0 0.4
//deformVertexes wave 32 sin 0 10 0 .2
cull disable
{
map textures/pad_cyb/waterstream
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
tcMod scale 0.4 0.55
tcMod turb .1 .08 .25 .08
tcMod scroll 0.005 -1
}
{
map textures/pad_cyb/waterstream
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
tcMod scale 0.5 0.45
tcMod turb .1 .05 .25 .08
tcMod scroll 0.008 -0.6
}
}


textures/pad_cyb/bubbles2_1
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.093287 -41.198784 541 sawtooth 0 1 1.000725 0.195925
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 20.229958
AlphaGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
rgbGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
tcMod stretch sawtooth 0.000000 1.138240 1.000725 0.195925
blendfunc add
}
}

textures/pad_cyb/bubbles2_2
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.769913 -21.519264 489 sawtooth 0 1 0.622631 0.252309
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 2.200842
AlphaGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
rgbGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
tcMod stretch sawtooth 0.000000 0.869579 0.622631 0.252309
blendfunc add
}
}

textures/pad_cyb/bubbles2_3
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.625161 -6.980746 407 sawtooth 0 1 1.213530 0.445164
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 19.405041
AlphaGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
rgbGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
tcMod stretch sawtooth 0.000000 2.052110 1.213530 0.445164
blendfunc add
}
}

textures/pad_cyb/bubbles2_4
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -8.668365 -12.663155 459 sawtooth 0 1 1.085688 0.300365
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 11.414990
AlphaGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
rgbGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
tcMod stretch sawtooth 0.000000 1.173104 1.085688 0.300365
blendfunc add
}
}

textures/pad_cyb/bubbles2_5
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -12.763139 -30.482132 551 sawtooth 0 1 0.288759 0.189146
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 1.619465
AlphaGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
rgbGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
tcMod stretch sawtooth 0.000000 1.285534 0.288759 0.189146
blendfunc add
}
}

textures/pad_cyb/bubbles2_6
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 7.834942 -10.679131 430 sawtooth 0 1 1.237915 0.365962
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate -2.907956
AlphaGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
rgbGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
tcMod stretch sawtooth 0.000000 1.686840 1.237915 0.365962
blendfunc add
}
}

textures/pad_cyb/bubbles2_7
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 5.454388 -41.883984 527 sawtooth 0 1 0.402287 0.207241
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 3.890042
AlphaGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
rgbGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
tcMod stretch sawtooth 0.000000 0.946705 0.402287 0.207241
blendfunc add
}
}

textures/pad_cyb/bubbles2_8
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -9.112141 -36.627399 516 sawtooth 0 1 0.784623 0.218790
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 24.587999
AlphaGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
rgbGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
tcMod stretch sawtooth 0.000000 0.350276 0.784623 0.218790
blendfunc add
}
}

textures/pad_cyb/bubbles2_9
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 13.988351 -11.087501 503 sawtooth 0 1 1.044519 0.235235
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 12.468795
AlphaGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
rgbGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
tcMod stretch sawtooth 0.000000 1.113410 1.044519 0.235235
blendfunc add
}
}

textures/pad_cyb/bubbles2_10
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 10.265741 7.714635 531 sawtooth 0 1 0.524331 0.144779
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 12.796563
AlphaGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
rgbGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
tcMod stretch sawtooth 0.000000 0.329548 0.524331 0.144779
blendfunc add
}
}

textures/pad_cyb/bubbles2_11
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 25.100649 4.780672 486 sawtooth 0 1 0.907308 0.255448
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate 16.214331
AlphaGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
rgbGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
tcMod stretch sawtooth 0.000000 2.025596 0.907308 0.255448
blendfunc add
}
}

textures/pad_cyb/bubbles2_12
{
qer_editorimage textures/pad_cyb/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 12.983425 -1.691125 395 sawtooth 0 1 0.845538 0.495670
{
clampmap textures/pad_cyb/bubbles2
tcMod rotate -2.724845
AlphaGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
rgbGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
tcMod stretch sawtooth 0.000000 0.903418 0.845538 0.495670
blendfunc add
}
}


textures/pad_cyb/pad_cyb_rust1
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight

        {
		map textures/pad_cyb/pad_cyb_rust1
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_cyb/screw1
{
        qer_editorimage textures/pad_cyb/screw1
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_cyb/screw1
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


textures/pad_objects02/pad_tuch
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_objects02/pad_tuch
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_cyb/schie04
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_cyb/schie04
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_cyb/toiletwater
{
	qer_editorimage textures/pad_cyb/wat_002
	q3map_lightimage textures/pad_cyb/wat_002
	surfaceparm nodrop
	surfaceparm noimpact
	// surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	deformVertexes wave 30 sin 0 4 0 0.2
	deformVertexes wave 100 sin 0 4 0 0.7
	tessSize 48
	qer_trans 0.5

	{
		map textures/pad_cyb/wat002
		blendfunc add
		rgbGen identity
		tcMod turb 0.4 0.3 1 0.05
		tcMod scale 0.5 0.5
		tcMod scroll 0.01 1.4
	}
}


textures/pad_cyb/marble_toilet002
{
	qer_editorimage textures/pad_cyb/marble_toilet002

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_cyb/marble_toilet002
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/pad_effects/pad_marblefx1
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}
