//=======================================================
// wehender vorhang Harmonieman
//=======================================================

textures/pad_gfx/harmvorhang
{
	surfaceparm nomarks
	//surfaceparm trans //durchsichtig !?
	cull disable
	deformVertexes wave 100 sin 0 3 0 0.7
	tessSize 80
	{
		map textures/pad_gfx/harmvorhang
		rgbGen Vertex
		depthWrite
		//blendfunc add //durchsichtig !?
	}

}

//=======================================================
// wehender vorhang 2 Harmonieman
//=======================================================

textures/pad_harm/vorhang
{
	surfaceparm nomarks
	//surfaceparm trans //durchsichtig !?
	cull disable
	deformVertexes wave 100 sin 0 3 0 0.7
	tessSize 80
	{
		map textures/pad_harm/vorhang
		rgbGen Vertex
		depthWrite
		//blendfunc add //durchsichtig !?
	}

}

//=======================================================
// transparent effect by SLoB :)
//=======================================================

//Green Gummy :)
textures/pad_harm/green
{
	qer_editorimage textures/pad_harm/green
	{
                map textures/pad_harm/green
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}

//Red Gummy :)
textures/pad_harm/red
{
	qer_editorimage textures/pad_harm/red
	{
                map textures/pad_harm/red
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}

//Yellow Gummy :)
textures/pad_harm/yellow
{
	qer_editorimage textures/pad_harm/yellow
	{
                map textures/pad_harm/yellow
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}


//Orange Gummy :)
textures/pad_harm/orange
{
	qer_editorimage textures/pad_harm/orange
	{
                map textures/pad_harm/orange
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}


//=======================================================
// leicht gl�nzendes Metall
//=======================================================
textures/pad_harm/metal06x
{
	qer_editorimage textures/pad_harm/metal06x
	surfaceparm metalsteps
	{
		map textures/pad_harm/metal06x
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


// ohne clip

textures/pad_harm/metal06x2
{
	qer_editorimage textures/pad_harm/metal06x2
	surfaceparm nonsolid
	surfaceparm metalsteps
	{
		map textures/pad_harm/metal06x2
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

textures/pad_harm/metal06bnew
{
    surfaceparm metalsteps
    surfaceparm nonsolid
	qer_editorimage textures/pad_harm/metal06bnew
	{
		map textures/pad_harm/metal06bnew
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

//=======================================================
// Neonschilder
//=======================================================

textures/pad_harm/closed
{
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    {
        map textures/pad_harm/closed
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        RgbGen identityLighting
    }
}

textures/pad_harm/nocigs
{
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    {
        map textures/pad_harm/nocigs
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        RgbGen identityLighting
    }
}


textures/pad_harm/open_neon
{
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    {
        map textures/pad_harm/open_neon
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        RgbGen identityLighting
    }
}


textures/pad_harm/route66
{
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    {
        map textures/pad_harm/route66
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        RgbGen identityLighting
    }
}

//=======================================================
//Monitor Screens
//=======================================================


textures/pad_harm/pad_harm01
{
	q3map_lightimage textures/pad_harm/pad_harm01
	q3map_surfacelight 100

	{
		map textures/pad_harm/pad_harm01
		rgbGen identity
	}
	{
		map textures/pad_gfx02/comp_line
		blendfunc add
		rgbGen identity
		tcmod scroll 0 1
	}

	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map $lightmap
		tcgen environment
		tcmod scale .5 .5
		rgbGen wave sin .25 0 0 0
		blendfunc add
	}
}


//=======================================================
// Milchglas
//=======================================================

textures/pad_harm/color24x
{
	qer_editorimage textures/pad_harm/color24x
    surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
    q3map_surfacelight 10

        {
		map textures/pad_harm/color24x
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


//=======================================================
// Milchglas mit Flare
//=======================================================

textures/pad_harm/milchflare
{
	qer_editorimage textures/pad_harm/milchflare
    surfaceparm trans
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
        q3map_surfacelight 10
        q3map_flareShader flareShader

        {
		map textures/pad_harm/milchflare
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


//=======================================================
// Rubber Chicken
//=======================================================

textures/pad_harm/rubber_chick
{
	qer_editorimage textures/pad_harm/rubber_chick
    surfaceparm trans
    q3map_surfacelight 10

	{
		map textures/pad_harm/rubber_chick
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
		depthWrite
	}
}


//=======================================================
// komisches zeug welches so aufpoppt und so ;)
//=======================================================

textures/pad_harm/glow01
{
	deformVertexes autosprite
        surfaceparm nonsolid
     	surfaceparm	trans
     	nomipmaps

	{
		clampmap textures/pad_harm/glow01
		blendfunc gl_one gl_one
		tcMod stretch sawtooth 1 -.9 .5 1.2
		tcMod rotate 10
		rgbGen wave sawtooth .5 .5 .25 1.2
	}
	{
		clampmap textures/pad_harm/glow01
		blendfunc gl_one gl_one
		tcMod stretch sawtooth 1 -.9 .5 1.3
		tcMod rotate -50
		rgbGen wave sawtooth .75 .5 .5 1.3
	}
}



//=======================================================
// Licht
//=======================================================

textures/pad_harm/lighty
{
    q3map_surfacelight 2000
    q3map_flareShader flareShader
    surfaceparm nolightmap
    q3map_styleMarker
    {
        map textures/pad_harm/lighty
    }
}


//=======================================================
// stepsounds
//=======================================================
textures/pad_floorin/col04extra
{
   surfaceparm carpetsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_floorin/col04extra
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/woodnew
{
   surfaceparm woodsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/woodnew
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}


textures/pad_harm/metal
{
   surfaceparm metalsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/metal
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/metal06x2
{
   surfaceparm metalsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/metal06x2
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/sitzbank01b
{
   surfaceparm softsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/sitzbank01b
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/sitzbank01b_black
{
   surfaceparm softsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/sitzbank01b_black
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/sitzbank01b_white
{
   surfaceparm softsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/sitzbank01b_white
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}


textures/pad_harm/sitzup
{
   surfaceparm softsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/sitzup
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/fensterbank
{
   surfaceparm woodsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/fensterbank
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/fussmatte
{
   surfaceparm carpetsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/fussmatte
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/table01
{
   surfaceparm woodsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/table01
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}

textures/pad_harm/wallcol002b
{
   surfaceparm woodsteps
     {
        map $lightmap
        rgbGen identity
     }
     {
        map textures/pad_harm/wallcol002b
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
     }
}


//=======================================================
// Warpkern
//=======================================================

textures/pad_harm/warpkern
{
	deformVertexes wave 300 sin 3 0 0 0
	{
		map textures/pad_harm/warpkern
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 40
                tcmod scroll 1 .1
	}
}


//=======================================================
//Invisible Water
//=======================================================

textures/pad_harm/water
{
   qer_editorimage textures/pad_harm/water
   surfaceparm nodraw
   surfaceparm noimpact
   surfaceparm nolightmap
   surfaceparm nomarks
   surfaceparm nonsolid
   surfaceparm water
}


//=======================================================
//Wei�er Tele
//=======================================================

textures/pad_harm/padtele_white
{
	cull none
	q3map_lightimage textures/pad_harm/padtele_white
	q3map_surfacelight 100

	{
		map textures/pad_harm/padtele_electric_white
		blendfunc add
		rgbGen wave square .25 .25 0 2.5
		tcmod scale 1 1
		tcMod scroll 1 1
	}



	{
		map textures/pad_harm/padtele_white
		blendfunc add
		rgbgen wave square 0 1 0 3
		tcmod scale 1 1
		tcMod scroll -2 1
	}

}

//=======================================================
//Blasen f�r Aquarium
//=======================================================

textures/pad_harm/bubbles2_1
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.093287 -41.198784 111.500214 sawtooth 0 1 1.000725 0.195925
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 20.229958
AlphaGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
rgbGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
tcMod stretch sawtooth 0.000000 1.138240 1.000725 0.195925
blendfunc add
}
}

textures/pad_harm/bubbles2_2
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.769913 -21.519264 139.001572 sawtooth 0 1 0.622631 0.252309
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 2.200842
AlphaGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
rgbGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
tcMod stretch sawtooth 0.000000 0.869579 0.622631 0.252309
blendfunc add
}
}

textures/pad_harm/bubbles2_3
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.625161 -6.980746 57.586830 sawtooth 0 1 1.213530 0.445164
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 19.405041
AlphaGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
rgbGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
tcMod stretch sawtooth 0.000000 2.052110 1.213530 0.445164
blendfunc add
}
}

textures/pad_harm/bubbles2_4
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -8.668365 -12.663155 79.066895 sawtooth 0 1 1.085688 0.300365
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 11.414990
AlphaGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
rgbGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
tcMod stretch sawtooth 0.000000 1.173104 1.085688 0.300365
blendfunc add
}
}

textures/pad_harm/bubbles2_5
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -12.763139 -30.482132 121.611313 sawtooth 0 1 0.288759 0.189146
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 1.619465
AlphaGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
rgbGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
tcMod stretch sawtooth 0.000000 1.285534 0.288759 0.189146
blendfunc add
}
}

textures/pad_harm/bubbles2_6
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 7.834942 -10.679131 70.490509 sawtooth 0 1 1.237915 0.365962
{
clampmap textures/pad_harm/bubbles2
tcMod rotate -2.907956
AlphaGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
rgbGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
tcMod stretch sawtooth 0.000000 1.686840 1.237915 0.365962
blendfunc add
}
}

textures/pad_harm/bubbles2_7
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 5.454388 -41.883984 97.730408 sawtooth 0 1 0.402287 0.207241
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 3.890042
AlphaGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
rgbGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
tcMod stretch sawtooth 0.000000 0.946705 0.402287 0.207241
blendfunc add
}
}

textures/pad_harm/bubbles2_8
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -9.112141 -36.627399 86.117142 sawtooth 0 1 0.784623 0.218790
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 24.587999
AlphaGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
rgbGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
tcMod stretch sawtooth 0.000000 0.350276 0.784623 0.218790
blendfunc add
}
}

textures/pad_harm/bubbles2_9
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 13.988351 -11.087501 73.268707 sawtooth 0 1 1.044519 0.235235
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 12.468795
AlphaGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
rgbGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
tcMod stretch sawtooth 0.000000 1.113410 1.044519 0.235235
blendfunc add
}
}

textures/pad_harm/bubbles2_10
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 10.265741 7.714635 101.291351 sawtooth 0 1 0.524331 0.144779
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 12.796563
AlphaGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
rgbGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
tcMod stretch sawtooth 0.000000 0.329548 0.524331 0.144779
blendfunc add
}
}

textures/pad_harm/bubbles2_11
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 25.100649 4.780672 126.159500 sawtooth 0 1 0.907308 0.255448
{
clampmap textures/pad_harm/bubbles2
tcMod rotate 16.214331
AlphaGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
rgbGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
tcMod stretch sawtooth 0.000000 2.025596 0.907308 0.255448
blendfunc add
}
}

textures/pad_harm/bubbles2_12
{
qer_editorimage textures/pad_harm/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 12.983425 -1.691125 65.949432 sawtooth 0 1 0.845538 0.495670
{
clampmap textures/pad_harm/bubbles2
tcMod rotate -2.724845
AlphaGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
rgbGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
tcMod stretch sawtooth 0.000000 0.903418 0.845538 0.495670
blendfunc add
}
}

//=======================================================
//Boden im Aquarium weicher
//=======================================================

textures/pad_harm/sand002
{
q3map_nonplanar
q3map_shadeangle 60
surfaceparm sandsteps
qer_editorimage textures/pad_harm/sand002
{
map $lightmap
rgbGen identity
}
{
map textures/pad_harm/sand002
blendFunc filter
}
}


//=======================================================
//Haribow�nde weicher
//=======================================================

textures/pad_objects02/haribo4
{
q3map_nonplanar
q3map_shadeangle 120
surfaceparm sandsteps
qer_editorimage textures/pad_objects02/haribo4
{
map $lightmap
rgbGen identity
}
{
map textures/pad_objects02/haribo4
blendFunc filter
}
}

//=======================================================
//Glanz auf Poster
//=======================================================

textures/pad_poster/gummibaer
{
	qer_editorimage textures/pad_poster/gummibaer
	{
		map textures/pad_poster/gummibaer
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

textures/pad_harm/girlass03
{
	qer_editorimage textures/pad_harm/girlass03
	{
		map textures/pad_harm/girlass03
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

textures/pad_poster/poster004
{
	qer_editorimage textures/pad_poster/poster004
	{
		map textures/pad_poster/poster004
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

textures/pad_poster/poster001
{
	qer_editorimage textures/pad_poster/poster001
	{
		map textures/pad_poster/poster001
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

textures/pad_poster/poster002
{
	qer_editorimage textures/pad_poster/poster002
	{
		map textures/pad_poster/poster002
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

textures/pad_harm/gold_haley
{
	qer_editorimage textures/pad_harm/gold_haley
	{
		map textures/pad_harm/gold_haley
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

textures/pad_harm/gold_buddy
{
	qer_editorimage textures/pad_harm/gold_buddy
	{
		map textures/pad_harm/gold_buddy
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

textures/pad_harm/gold_elvis
{
	qer_editorimage textures/pad_harm/gold_elvis
	{
		map textures/pad_harm/gold_elvis
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

textures/pad_harm/gold_monroe
{
	qer_editorimage textures/pad_harm/gold_monroe
	{
		map textures/pad_harm/gold_monroe
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


//=======================================================
//Skybox
//=======================================================

textures/pad_harmsky/padcity
{
        qer_editorimage textures/pad_harmsky/padcity


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/white
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 100

        skyparms env/padcity512 - -
//       {
//		map textures/pad_petesky/padcity
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}



textures/pad_harmsky/morning-madness
{
        qer_editorimage textures/pad_harmsky/morning-madness


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange02
	q3map_sun	0.368 -0.886 0.258 100 50 55
	q3map_surfacelight 600

        skyparms env/pc-morning-madness512 - -
//       {
//		map textures/pad_petesky/morning-madness
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_harmsky/harmnight-life
{
        qer_editorimage textures/pad_harmsky/harmnight-life


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/white02
	q3map_sun	0.266383 0.274632 0.358662 50 50 55
	q3map_surfacelight 50

        skyparms env/pc-night-life512 - -
//       {
//		map textures/pad_petesky/night-life
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_harmsky/harmpc-ground-zero512
{
        qer_editorimage textures/pad_harmsky/harmpc-ground-zero512


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/white02
	q3map_sun	0.266383 0.274632 0.358662 20 50 55
	q3map_surfacelight 10

        skyparms env/pc-ground-zero512 - -
//       {
//		map textures/pad_petesky/pc-ground-zero512
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


//=======================================================
//Spiegelnder Boden
//=======================================================


// special thx to sst13 for the shader!!

textures/pad_harm/blend
{
	qer_editorimage textures/pad_harm/Floor008mirror
	surfaceparm nolightmap
	portal
	{
		map textures/pad_harm/Floor008mirror
		blendfunc blend
		depthWrite
	}
}

textures/pad_harm/add
{
	qer_editorimage textures/pad_harm/Floor008mirror
	surfaceparm nolightmap
	portal
	{
		map textures/pad_harm/Floor008mirror
		blendfunc add
		depthWrite
	}
}

textures/pad_harm/filter
{
	qer_editorimage textures/pad_harm/Floor008mirror
	surfaceparm nolightmap
	portal
	{
		map textures/pad_harm/Floor008mirror
		blendfunc filter
		depthWrite
	}
}