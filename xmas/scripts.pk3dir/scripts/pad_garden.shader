textures/pad_garden/jail_alpha_000 // Primary texture ONLY
{
qer_editorimage textures/common/alpha_000
q3map_alphaMod volume
q3map_alphaMod set 0.00
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}


textures/pad_garden/jail_alpha_025
{
qer_editorimage textures/common/alpha_025
q3map_alphaMod volume
q3map_alphaMod set 0.25
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_050 // Perfect mix of both Primary + Secondary
{
qer_editorimage textures/common/alpha_050
q3map_alphaMod volume
q3map_alphaMod set 0.50
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_075
{
qer_editorimage textures/common/alpha_075
q3map_alphaMod volume
q3map_alphaMod set 0.75
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_085
{
qer_editorimage textures/common/alpha_085
q3map_alphaMod volume
q3map_alphaMod set 0.85
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_100 // Secondary texture ONLY
{
qer_editorimage textures/common/alpha_100
q3map_alphaMod volume
q3map_alphaMod set 1.0
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/vines2
{
        qer_editorimage textures/pad_garden/vines2
    	surfaceparm trans
	surfaceparm alphashadow
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_garden/vines2
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

textures/pad_garden/schilf02
{
        qer_editorimage textures/pad_garden/schilf02
      //q3map_lightimage textures/pad_garden/schilf02
        qer_trans 0.5
        q3map_globaltexture
        surfaceparm trans
        surfaceparm nonsolid
        surfaceparm water
        cull disable


        {
               map textures/pad_garden/schilf02
               tcMod turb 0.2 0.1 1 0.05
               tcMod scale 0.5 0.5
               tcMod scroll 0.01 0.01
	       blendFunc add
	       rgbGen identity

        }

        {
               map textures/pad_garden/schilf03
               tcMod turb 0.2 0.1 1 0.05
               tcMod scale -0.5 -0.5
               tcMod scroll .025 -.001
	       blendFunc filter
	       rgbGen identity

        }

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ONE
	}
}



textures/pad_garden/tuereglass
{
        surfaceparm trans
	cull none
	qer_trans 	0.5

        {
		map textures/pad_garden/tuereglass
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


textures/pad_garden/tuerglasdark
{
        surfaceparm trans
	cull none
	qer_trans 	0.5

        {
		map textures/pad_garden/tuerglasdark
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


textures/pad_garden/fensterglass
{
        surfaceparm trans
	cull none
	qer_trans 	0.5

        {
		map textures/pad_garden/fensterglass
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


textures/pad_garden/fensterglasdark
{
        surfaceparm trans
	cull none
	qer_trans 	0.5

        {
		map textures/pad_garden/fensterglasdark
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


textures/pad_garden/firedrops01
{
qer_editorimage textures/pad_garden/firedrops
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.000043 0.000000 -1100 sawtooth 0 1 0.000000 3
{
clampmap textures/pad_garden/firedrops
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.000000 8.000000 0.000000 0.129033
rgbGen wave sawtooth 1.000000 0.000000 0.000000 0.129033
tcMod stretch sawtooth 1.000000 0.000000 0.000000 0.129033
blendfunc blend
}
}


textures/pad_garden/padflagred
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7

        {
                map textures/pad_garden/padflagred
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


textures/pad_garden/padflagblue
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7

        {
                map textures/pad_garden/padflagblue
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

textures/pad_garden/fort-pad
{
	qer_editorimage env/fort-pad512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_ffffe5ff
	q3map_sun 1.000000 0.913939 0.701350 390 220 40
	q3map_surfacelight 250

	skyparms env/fort-pad512 - -
}

textures/pad_garden/polter_geist
{
	qer_editorimage env/pf-garden512_ft
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/colors/hex_1c1ac7ff
	q3map_sun 0.404654 0.474220 1.000000 330 220 50
	q3map_surfacelight 160

	skyparms env/pf-garden512 - -
}


textures/pad_garden/nachtfenster
{
    q3map_surfacelight 1000
    surfaceparm nolightmap
    {
        map textures/pad_garden/nachtfenster
    }
}




textures/pad_garden/redpoints01
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints01
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints01_blend
		rgbGen wave sin 0.5 0.5 1 1
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/redpoints02
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints02
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints02_blend
		rgbGen wave sin 0.7 0.7 1 1
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/kreidemotiv01
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_garden/kreidemotiv01
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_garden/kreidemotiv02
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_garden/kreidemotiv02
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_garden/kreidemotiv03
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_garden/kreidemotiv03
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_garden/bubbles2_1
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.093287 -41.198784 141.500214 sawtooth 0 1 1.000725 0.195925
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 20.229958
AlphaGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
rgbGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
tcMod stretch sawtooth 0.000000 1.138240 1.000725 0.195925
blendfunc add
}
}

textures/pad_garden/bubbles2_2
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.769913 -21.519264 169.001572 sawtooth 0 1 0.622631 0.252309
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 2.200842
AlphaGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
rgbGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
tcMod stretch sawtooth 0.000000 0.869579 0.622631 0.252309
blendfunc add
}
}

textures/pad_garden/bubbles2_3
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.625161 -6.980746 87.586830 sawtooth 0 1 1.213530 0.445164
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 19.405041
AlphaGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
rgbGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
tcMod stretch sawtooth 0.000000 2.052110 1.213530 0.445164
blendfunc add
}
}

textures/pad_garden/bubbles2_4
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -8.668365 -12.663155 109.066895 sawtooth 0 1 1.085688 0.300365
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 11.414990
AlphaGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
rgbGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
tcMod stretch sawtooth 0.000000 1.173104 1.085688 0.300365
blendfunc add
}
}

textures/pad_garden/bubbles2_5
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -12.763139 -30.482132 151.611313 sawtooth 0 1 0.288759 0.189146
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 1.619465
AlphaGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
rgbGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
tcMod stretch sawtooth 0.000000 1.285534 0.288759 0.189146
blendfunc add
}
}

textures/pad_garden/bubbles2_6
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 7.834942 -10.679131 100.490509 sawtooth 0 1 1.237915 0.365962
{
clampmap textures/pad_garden/bubbles2
tcMod rotate -2.907956
AlphaGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
rgbGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
tcMod stretch sawtooth 0.000000 1.686840 1.237915 0.365962
blendfunc add
}
}

textures/pad_garden/bubbles2_7
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 5.454388 -41.883984 127.730408 sawtooth 0 1 0.402287 0.207241
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 3.890042
AlphaGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
rgbGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
tcMod stretch sawtooth 0.000000 0.946705 0.402287 0.207241
blendfunc add
}
}

textures/pad_garden/bubbles2_8
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -9.112141 -36.627399 116.117142 sawtooth 0 1 0.784623 0.218790
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 24.587999
AlphaGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
rgbGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
tcMod stretch sawtooth 0.000000 0.350276 0.784623 0.218790
blendfunc add
}
}

textures/pad_garden/bubbles2_9
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 13.988351 -11.087501 103.268707 sawtooth 0 1 1.044519 0.235235
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 12.468795
AlphaGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
rgbGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
tcMod stretch sawtooth 0.000000 1.113410 1.044519 0.235235
blendfunc add
}
}

textures/pad_garden/bubbles2_10
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 10.265741 7.714635 131.291351 sawtooth 0 1 0.524331 0.144779
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 12.796563
AlphaGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
rgbGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
tcMod stretch sawtooth 0.000000 0.329548 0.524331 0.144779
blendfunc add
}
}

textures/pad_garden/bubbles2_11
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 25.100649 4.780672 156.159500 sawtooth 0 1 0.907308 0.255448
{
clampmap textures/pad_garden/bubbles2
tcMod rotate 16.214331
AlphaGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
rgbGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
tcMod stretch sawtooth 0.000000 2.025596 0.907308 0.255448
blendfunc add
}
}

textures/pad_garden/bubbles2_12
{
qer_editorimage textures/pad_garden/bubbles2
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 12.983425 -1.691125 95.949432 sawtooth 0 1 0.845538 0.495670
{
clampmap textures/pad_garden/bubbles2
tcMod rotate -2.724845
AlphaGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
rgbGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
tcMod stretch sawtooth 0.000000 0.903418 0.845538 0.495670
blendfunc add
}
}

textures/pad_garden/wellen3_1
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.758238 0.386510
{
clampmap textures/pad_garden/wellen
tcMod rotate 13.147161
AlphaGen wave sawtooth 1.083261 -1.083261 0.758238 0.386510
rgbGen wave sawtooth 0.651039 -0.651039 0.758238 0.386510
tcMod stretch sawtooth 0.496692 0.396280 0.758238 0.386510
blendfunc add
}
}

textures/pad_garden/wellen3_2
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.550810 0.215062
{
clampmap textures/pad_garden/wellen
tcMod rotate 12.292642
AlphaGen wave sawtooth 1.012714 -1.012714 0.550810 0.215062
rgbGen wave sawtooth 0.747148 -0.747148 0.550810 0.215062
tcMod stretch sawtooth 0.488470 0.418503 0.550810 0.215062
blendfunc add
}
}

textures/pad_garden/wellen3_3
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.093259 0.268482
{
clampmap textures/pad_garden/wellen
tcMod rotate 7.173833
AlphaGen wave sawtooth 0.962444 -0.962444 1.093259 0.268482
rgbGen wave sawtooth 0.716947 -0.716947 1.093259 0.268482
tcMod stretch sawtooth 0.399271 0.423142 1.093259 0.268482
blendfunc add
}
}

textures/pad_garden/wellen3_4
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.669686 0.210215
{
clampmap textures/pad_garden/wellen
tcMod rotate 21.939421
AlphaGen wave sawtooth 1.023920 -1.023920 0.669686 0.210215
rgbGen wave sawtooth 0.638075 -0.638075 0.669686 0.210215
tcMod stretch sawtooth 0.391726 0.652324 0.669686 0.210215
blendfunc add
}
}

textures/pad_garden/wellen3_5
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.228907 0.215789
{
clampmap textures/pad_garden/wellen
tcMod rotate 15.530046
AlphaGen wave sawtooth 0.950432 -0.950432 1.228907 0.215789
rgbGen wave sawtooth 0.785540 -0.785540 1.228907 0.215789
tcMod stretch sawtooth 0.325874 0.732252 1.228907 0.215789
blendfunc add
}
}

textures/pad_garden/wellen3_6
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.527409 0.303915
{
clampmap textures/pad_garden/wellen
tcMod rotate 8.849666
AlphaGen wave sawtooth 1.098447 -1.098447 0.527409 0.303915
rgbGen wave sawtooth 0.905087 -0.905087 0.527409 0.303915
tcMod stretch sawtooth 0.395859 0.421647 0.527409 0.303915
blendfunc add
}
}

textures/pad_garden/wellen3_7
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.650606 0.368229
{
clampmap textures/pad_garden/wellen
tcMod rotate 10.157323
AlphaGen wave sawtooth 0.910831 -0.910831 0.650606 0.368229
rgbGen wave sawtooth 0.628663 -0.628663 0.650606 0.368229
tcMod stretch sawtooth 0.396371 0.581820 0.650606 0.368229
blendfunc add
}
}

textures/pad_garden/wellen3_8
{
qer_editorimage textures/pad_garden/wellen
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.134458 0.217227
{
clampmap textures/pad_garden/wellen
tcMod rotate 9.057192
AlphaGen wave sawtooth 1.037141 -1.037141 1.134458 0.217227
rgbGen wave sawtooth 0.722733 -0.722733 1.134458 0.217227
tcMod stretch sawtooth 0.388113 0.698187 1.134458 0.217227
blendfunc add
}
}


textures/pad_garden/wave0
{
qer_editorimage textures/pad_garden/wave0
surfaceparm noimpact
surfaceparm nolightmap
nomipmaps
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.765160 0.500000
{
clampmap textures/pad_garden/wave1
tcMod rotate 5.752586
AlphaGen wave sawtooth 1.000000 -0.421894 0.765160 0.500000
rgbGen wave sawtooth 1.000000 -0.720054 0.765160 0.500000
tcMod stretch sawtooth 0 0 0 0
blendfunc add
}
{
clampmap textures/pad_garden/wave2
tcMod rotate 5.752586
AlphaGen wave sawtooth 1.000000 -0.421894 0.765160 0.500000
rgbGen wave sawtooth 1.000000 -0.720054 0.765160 0.500000
tcMod stretch sawtooth 0.177834 0.822166 0.765160 0.500000
blendfunc add
}
}

textures/pad_garden/waterfall2
{
qer_editorimage textures/pad_garden/waterstream
q3map_globaltexture
surfaceparm nonsolid
surfaceparm nolightmap
surfaceparm trans
surfaceparm noimpact
surfaceparm water
tessSize 32
//deformVertexes move 2 1 0 sin 0 1 0.2 0.2
//deformVertexes move .6 3.3 0 sin 0 1 0 0.4
//deformVertexes wave 32 sin 0 10 0 .2
cull disable
{
map textures/pad_garden/waterstream
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
tcMod scale 0.4 0.55
tcMod turb .1 .08 .25 .08
tcMod scroll 0.005 -1
}
{
map textures/pad_garden/waterstream
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
tcMod scale 0.5 0.45
tcMod turb .1 .05 .25 .08
tcMod scroll 0.008 -0.6
}
}



textures/pad_garden/gardensand02
{
q3map_nonplanar
q3map_shadeangle 60 l
surfaceparm sandsteps
qer_editorimage textures/pad_garden/gardensand02
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/gardensand02
blendFunc filter
}
}

textures/pad_garden/sand003a
{
q3map_nonplanar
q3map_shadeangle 60 l
surfaceparm sandsteps
qer_editorimage textures/pad_garden/sand003a
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/sand003a
blendFunc filter
}
}

textures/pad_garden/algen
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_garden/algen
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/algen
blendFunc filter
}
}


textures/pad_garden/ashes_256
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_garden/ashes_256
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/ashes_256
blendFunc filter
}
}


textures/pad_garden/nail
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_garden/nail
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


textures/pad_garden/kaktus002
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_garden/kaktus002
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



textures/pad_garden/grasground
{
qer_editorimage textures/pad_garden/grasground
surfaceparm snowsteps

q3map_nonplanar
q3map_shadeangle 60
q3map_lightmapAxis z
q3map_lightmapmergable
//q3map_globaltexture
q3map_tcGen ivector ( 256 0 0 ) ( 0 256 0 )
q3map_alphaMod dotproduct2 ( 0.0 0.0 0.75 )
q3map_lightmapSampleSize 16

{
map textures/pad_garden/sandground // Primary
rgbGen identity
}
{
map textures/pad_garden/grasground // Secondary
blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//alphaFunc GE128
rgbGen identity
alphaGen vertex
detail
}
{
map $lightmap
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/seerosenblatt_move
{
	qer_editorimage textures/pad_garden/seerosenblatt
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
              surfaceparm snowsteps
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_garden/seerosenblatt
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

textures/pad_garden/fussmatte01
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/fussmatte01
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/liegestoff02
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/liegestoff02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/lochgitter02
{
surfaceparm latticesteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/lochgitter02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/lochgitter03
{
surfaceparm latticesteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/lochgitter03
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/lochgitter04
{
surfaceparm latticesteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/lochgitter04
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/lochgitter05
{
surfaceparm latticesteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/lochgitter05
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostblack
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostblack
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostborderbig
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostborderbig
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostbordersmall
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostbordersmall
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/rostdoorsmall
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostdoorsmall
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/faltblech
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/faltblech
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/purpleflow
{
        qer_editorimage textures/pad_garden/purpleflow
    	surfaceparm trans
	surfaceparm alphashadow
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_garden/purpleflow
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


textures/pad_garden/rocket
{
	qer_editorimage textures/pad_garden/rocket
	{
		map textures/pad_garden/rocket
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

textures/pad_garden/metalwhite001big
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/metalwhite001big
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/birdfloor_big
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/birdfloor_big
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/teichbrett2_512
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/teichbrett2_512
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/rostsoft
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostsoft
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostrough
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostrough
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostred
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostred
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostblack
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostblack
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/sunchairwood
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/sunchairwood
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/liegestoff02
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/liegestoff02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}



textures/pad_garden/toyblock01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/toyblock01
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/toyblock02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/toyblock02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/toyblock03
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/toyblock03
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/toyblock04
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/toyblock04
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/big_flameblue256
{

		surfaceparm trans
		surfaceparm nomarks
		surfaceparm nonsolid
		qer_editorimage textures/pad_garden/bflame1
		q3map_surfacelight 200
		surfaceparm nolightmap
		cull none

	{
		animMap 10 textures/pad_garden/bflame1 textures/pad_garden/bflame2 textures/pad_garden/bflame3 textures/pad_garden/bflame4 textures/pad_garden/bflame5 textures/pad_garden/bflame6 textures/pad_garden/bflame7 textures/pad_garden/bflame8
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 10

	}
	{
		animMap 10 textures/pad_garden/bflame2 textures/pad_garden/bflame3 textures/pad_garden/bflame4 textures/pad_garden/bflame5 textures/pad_garden/bflame6 textures/pad_garden/bflame7 textures/pad_garden/bflame8 textures/pad_garden/bflame1
		blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
	}


	{
		map textures/pad_garden/bflameball
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6
	}

}


textures/pad_garden/red_flame256
{

		surfaceparm trans
		surfaceparm nomarks
		surfaceparm nonsolid
		qer_editorimage textures/pad_garden/sflame1
		q3map_surfacelight 200
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


textures/pad_garden/ship00
{
	qer_editorimage textures/pad_garden/ship00
	{
		map textures/pad_garden/ship00
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
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


textures/pad_garden/ship02
{
	qer_editorimage textures/pad_garden/ship02
	{
		map textures/pad_garden/ship02
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
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


textures/pad_garden/ship04
{
                surfaceparm metalsteps
	qer_editorimage textures/pad_garden/ship04
	{
		map textures/pad_garden/ship04
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
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


textures/pad_garden/ship05
{
	qer_editorimage textures/pad_garden/ship05
	{
		map textures/pad_garden/ship05
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


textures/pad_garden/ship06
{

	qer_editorimage textures/pad_garden/ship06
	{
		map textures/pad_garden/ship06
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


textures/pad_garden/ship07
{
	qer_editorimage textures/pad_garden/ship07
	{
		map textures/pad_garden/ship07
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
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


textures/pad_garden/ship08
{
	qer_editorimage textures/pad_garden/ship08
	{
		map textures/pad_garden/ship08
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


textures/pad_garden/ship10
{
	qer_editorimage textures/pad_garden/ship10
	{
		map textures/pad_garden/ship10
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
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


textures/pad_garden/radio002
{
	qer_editorimage textures/pad_garden/radio002
	{
		map textures/pad_garden/radio002
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


textures/pad_garden/radio003
{
	qer_editorimage textures/pad_garden/radio003
	{
		map textures/pad_garden/radio003
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


textures/pad_garden/radio004
{
	qer_editorimage textures/pad_garden/radio004
	{
		map textures/pad_garden/radio004
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


textures/pad_garden/radio005
{
	qer_editorimage textures/pad_garden/radio005
	{
		map textures/pad_garden/radio005
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

textures/pad_garden/rostdoorcurve
{
          qer_editorimage textures/pad_garden/rostdoorsmall
           surfaceparm nonsolid
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/rostdoorsmall
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_metal/testlvlcurve
{
          qer_editorimage textures/pad_metal/testlvlbase
           surfaceparm nonsolid
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/testlvlbase
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_wallout/wall_stonecurve
{
          qer_editorimage textures/pad_wallout/wall_stone043
           surfaceparm nonsolid
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wallout/wall_stone043
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}
