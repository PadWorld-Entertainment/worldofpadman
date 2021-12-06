textures/pad_garden/jail_alpha_000 // Primary texture ONLY
{

qer_editorimage textures/pad_garden/jail_alpha_000.jpg

q3map_alphaMod volume
q3map_alphaMod set 0.00
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}


textures/pad_garden/jail_alpha_025
{
qer_editorimage textures/pad_garden/jail_alpha_025.jpg
q3map_alphaMod volume
q3map_alphaMod set 0.25
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_050 // Perfect mix of both Primary + Secondary
{
qer_editorimage textures/pad_garden/jail_alpha_050.jpg
q3map_alphaMod volume
q3map_alphaMod set 0.50
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}
textures/pad_garden/jail_alpha_075
{
qer_editorimage textures/pad_garden/jail_alpha_075.jpg
q3map_alphaMod volume
q3map_alphaMod set 0.75
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_085
{
qer_editorimage textures/pad_garden/jail_alpha_085.jpg
q3map_alphaMod volume
q3map_alphaMod set 0.85
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}

textures/pad_garden/jail_alpha_100 // Secondary texture ONLY
{
qer_editorimage textures/pad_garden/jail_alpha_100.jpg
q3map_alphaMod volume
q3map_alphaMod set 1.0
surfaceparm nodraw
surfaceparm nonsolid
surfaceparm trans
qer_trans 0.75
}


textures/pad_garden/vines
{
        qer_editorimage textures/pad_garden/vines.tga
    	surfaceparm trans
	surfaceparm alphashadow
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_garden/vines.tga
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

textures/pad_garden/vines2
{
        qer_editorimage textures/pad_garden/vines2.tga
    	surfaceparm trans
	surfaceparm alphashadow
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_garden/vines2.tga
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


textures/pad_garden/schilf01 
{ 
        qer_editorimage textures/pad_garden/schilf01.tga 
      //q3map_lightimage textures/pad_garden/schilf01.tga 
        qer_trans 0.5 
        q3map_globaltexture 
        surfaceparm trans 
        surfaceparm nolightmap 
        surfaceparm nonsolid 
        surfaceparm water 
        cull disable 
        

        { 
               map textures/pad_garden/schilf01.tga 
               tcMod turb 0.2 0.1 1 0.05 
               tcMod scale 0.5 0.5 
               tcMod scroll 0.01 0.01 
               blendfunc add 
               rgbGen vertex 

        } 

} 


textures/pad_garden/schilf02 
{ 
        qer_editorimage textures/pad_garden/schilf02.tga 
      //q3map_lightimage textures/pad_garden/schilf02.tga 
        qer_trans 0.5 
        q3map_globaltexture 
        surfaceparm trans 
        surfaceparm nonsolid 
        surfaceparm water 
        cull disable 
        

        { 
               map textures/pad_garden/schilf02.tga 
               tcMod turb 0.2 0.1 1 0.05 
               tcMod scale 0.5 0.5 
               tcMod scroll 0.01 0.01 
	       blendFunc filter
	       rgbGen identity

        } 

        { 
               map textures/pad_garden/schilf03.tga 
               tcMod turb 0.2 0.1 1 0.05 
               tcMod scale -0.5 -0.5 
               tcMod scroll .025 -.001 
	       blendFunc add
	       rgbGen identity

        } 

	{
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ONE
	}
}



textures/pad_garden/schilf05
	{
		qer_editorimage textures/pad_garden/schilf05.tga 
		qer_trans 0.5
		q3map_globaltexture
		surfaceparm trans
		surfaceparm nonsolid
		surfaceparm water
	
		cull disable

                            fogparms ( .211 .231 .094 ) 650

		deformVertexes wave 128 sin .65 .65 0 .5	
		{ 
			map textures/pad_garden/schilf05.tga 
			blendFunc GL_one GL_one
			rgbgen identity
                                          tcMod turb 0.1 0.1 1 0.05 
                                          tcMod scale 0.4 0.4 
                                          tcMod scroll 0.01 0.01 
		}
	
		{ 
			map textures/pad_garden/schilf05.tga 
			blendFunc GL_one GL_one
                                          tcMod turb 0.05 0.05 0.5 0.05 
                                          tcMod scale 0.5 0.5 
                                          tcMod scroll 0.01 0.01 
		}

	
		{
			map $lightmap
			blendFunc GL_dst_color GL_zero
			rgbgen identity		
		}
	
	

}

textures/pad_garden/gardine
{
	q3map_nolightmap
	q3map_onlyvertexlighting
    {
        map textures/pad_garden/gardine.tga
        blendFunc GL_ONE GL_ONE
    }
}


textures/pad_garden/tuereglass
{
        surfaceparm trans	
	cull none
	qer_trans 	0.5
     
        {
		map textures/pad_garden/tuereglass.tga
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
		map textures/pad_garden/fensterglass.tga
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
qer_editorimage textures/pad_garden/firedrops.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.000043 0.000000 -1100 sawtooth 0 1 0.000000 3
{
clampmap textures/pad_garden/firedrops.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.000000 8.000000 0.000000 0.129033
rgbGen wave sawtooth 1.000000 0.000000 0.000000 0.129033
tcMod stretch sawtooth 1.000000 0.000000 0.000000 0.129033
blendfunc blend
}
}


textures/pad_garden/firedrops02
{
		qer_editorimage textures/pad_garden/firedrops.tga
		surfaceparm noimpact
		surfaceparm nolightmap
		cull none
		surfaceparm trans
		surfaceparm nonsolid
		surfaceparm nodlight
		deformvertexes autosprite
		deformvertexes move 0.000043 0.000000 -1100 sawtooth 0 1 0.800000 3
	{
		clampmap textures/pad_garden/firedrops.tga
		tcMod rotate 0.000000
		AlphaGen wave sawtooth 0.000000 8.000000 0.800000 0.129033
		rgbGen wave sawtooth 1.000000 0.000000 0.800000 0.129033
		tcMod stretch sawtooth 1.000000 0.000000 0.800000 0.129033
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
                map textures/pad_garden/padflagred.tga
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
                map textures/pad_garden/padflagblue.tga
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
        qer_editorimage textures/pad_garden/fort-pad.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_garden/white.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 230

        skyparms env/fort-pad512 - -
//       {
//		map textures/pad_garden/fort-pad.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
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
		map textures/pad_garden/redpoints01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints01_blend.tga
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
		map textures/pad_garden/redpoints02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/redpoints02_blend.tga
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
		map textures/pad_garden/kreidemotiv01.tga
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
		map textures/pad_garden/kreidemotiv02.tga
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
		map textures/pad_garden/kreidemotiv03.tga
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_garden/neon01
{
	qer_editorimage textures/pad_garden/neon01.tga
	surfaceparm nomarks
	q3map_surfacelight 300
        {
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/neon01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/neon01_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light01
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light01_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light02
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light02_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light03
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light03.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light03_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light04
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light04.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light04_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light05
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light05.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light05_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light06
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light06.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light06_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/light07
{
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/light07.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_garden/light07_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/lightlamp01
{
	qer_editorimage textures/pad_garden/lightlamp01.tga
	q3map_lightimage textures/pad_garden/lightlamp01_blend.tga
	surfaceparm nomarks
	q3map_surfacelight 400
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/lightlamp01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{	
		map textures/pad_garden/lightlamp01_blend.tga
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_garden/lightlamp02
{
	q3map_lightimage textures/pad_garden/lightlamp02_blend.tga
	surfaceparm nomarks
	q3map_surfacelight 500
        {
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/lightlamp02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{	
		map textures/pad_garden/lightlamp02_blend.tga
		blendfunc GL_ONE GL_ONE
                rgbGen wave sin .5 0.5 1 .1
	}
        {	
		map textures/pad_garden/lightlamp02b.tga
		blendfunc GL_ONE GL_ONE
                rgbgen wave triangle 1 5 1 3
	}
        {	
		map textures/pad_garden/lightlamp02b.tga
		blendfunc GL_ONE GL_ONE
                tcmod scale -1 -1
                 rgbgen wave triangle 1 2 0 7
	}
}


textures/pad_garden/lightred
{
	q3map_lightimage textures/pad_garden/lightred.tga
	surfaceparm nomarks
	q3map_surfacelight 300
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/lightred.tga
		blendFunc filter
		rgbGen identity
	}
        {
		map textures/pad_garden/lightred.tga
		blendFunc add
	}
	
}





textures/pad_garden/bubbles2_1
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.093287 -41.198784 141.500214 sawtooth 0 1 1.000725 0.195925
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 20.229958
AlphaGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
rgbGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
tcMod stretch sawtooth 0.000000 1.138240 1.000725 0.195925
blendfunc add
}
}

textures/pad_garden/bubbles2_2
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.769913 -21.519264 169.001572 sawtooth 0 1 0.622631 0.252309
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 2.200842
AlphaGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
rgbGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
tcMod stretch sawtooth 0.000000 0.869579 0.622631 0.252309
blendfunc add
}
}

textures/pad_garden/bubbles2_3
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.625161 -6.980746 87.586830 sawtooth 0 1 1.213530 0.445164
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 19.405041
AlphaGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
rgbGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
tcMod stretch sawtooth 0.000000 2.052110 1.213530 0.445164
blendfunc add
}
}

textures/pad_garden/bubbles2_4
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -8.668365 -12.663155 109.066895 sawtooth 0 1 1.085688 0.300365
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 11.414990
AlphaGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
rgbGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
tcMod stretch sawtooth 0.000000 1.173104 1.085688 0.300365
blendfunc add
}
}

textures/pad_garden/bubbles2_5
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -12.763139 -30.482132 151.611313 sawtooth 0 1 0.288759 0.189146
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 1.619465
AlphaGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
rgbGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
tcMod stretch sawtooth 0.000000 1.285534 0.288759 0.189146
blendfunc add
}
}

textures/pad_garden/bubbles2_6
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 7.834942 -10.679131 100.490509 sawtooth 0 1 1.237915 0.365962
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate -2.907956
AlphaGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
rgbGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
tcMod stretch sawtooth 0.000000 1.686840 1.237915 0.365962
blendfunc add
}
}

textures/pad_garden/bubbles2_7
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 5.454388 -41.883984 127.730408 sawtooth 0 1 0.402287 0.207241
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 3.890042
AlphaGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
rgbGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
tcMod stretch sawtooth 0.000000 0.946705 0.402287 0.207241
blendfunc add
}
}

textures/pad_garden/bubbles2_8
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -9.112141 -36.627399 116.117142 sawtooth 0 1 0.784623 0.218790
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 24.587999
AlphaGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
rgbGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
tcMod stretch sawtooth 0.000000 0.350276 0.784623 0.218790
blendfunc add
}
}

textures/pad_garden/bubbles2_9
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 13.988351 -11.087501 103.268707 sawtooth 0 1 1.044519 0.235235
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 12.468795
AlphaGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
rgbGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
tcMod stretch sawtooth 0.000000 1.113410 1.044519 0.235235
blendfunc add
}
}

textures/pad_garden/bubbles2_10
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 10.265741 7.714635 131.291351 sawtooth 0 1 0.524331 0.144779
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 12.796563
AlphaGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
rgbGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
tcMod stretch sawtooth 0.000000 0.329548 0.524331 0.144779
blendfunc add
}
}

textures/pad_garden/bubbles2_11
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 25.100649 4.780672 156.159500 sawtooth 0 1 0.907308 0.255448
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate 16.214331
AlphaGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
rgbGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
tcMod stretch sawtooth 0.000000 2.025596 0.907308 0.255448
blendfunc add
}
}

textures/pad_garden/bubbles2_12
{
qer_editorimage textures/pad_garden/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 12.983425 -1.691125 95.949432 sawtooth 0 1 0.845538 0.495670
{
clampmap textures/pad_garden/bubbles2.tga
tcMod rotate -2.724845
AlphaGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
rgbGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
tcMod stretch sawtooth 0.000000 0.903418 0.845538 0.495670
blendfunc add
}
}

textures/pad_garden/wellen3_1 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.758238 0.386510 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 13.147161 
AlphaGen wave sawtooth 1.083261 -1.083261 0.758238 0.386510 
rgbGen wave sawtooth 0.651039 -0.651039 0.758238 0.386510 
tcMod stretch sawtooth 0.496692 0.396280 0.758238 0.386510 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_2 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap 
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.550810 0.215062 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 12.292642 
AlphaGen wave sawtooth 1.012714 -1.012714 0.550810 0.215062 
rgbGen wave sawtooth 0.747148 -0.747148 0.550810 0.215062 
tcMod stretch sawtooth 0.488470 0.418503 0.550810 0.215062 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_3 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.093259 0.268482 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 7.173833 
AlphaGen wave sawtooth 0.962444 -0.962444 1.093259 0.268482 
rgbGen wave sawtooth 0.716947 -0.716947 1.093259 0.268482 
tcMod stretch sawtooth 0.399271 0.423142 1.093259 0.268482 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_4 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.669686 0.210215 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 21.939421 
AlphaGen wave sawtooth 1.023920 -1.023920 0.669686 0.210215 
rgbGen wave sawtooth 0.638075 -0.638075 0.669686 0.210215 
tcMod stretch sawtooth 0.391726 0.652324 0.669686 0.210215 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_5 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.228907 0.215789 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 15.530046 
AlphaGen wave sawtooth 0.950432 -0.950432 1.228907 0.215789 
rgbGen wave sawtooth 0.785540 -0.785540 1.228907 0.215789 
tcMod stretch sawtooth 0.325874 0.732252 1.228907 0.215789 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_6 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.527409 0.303915 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 8.849666 
AlphaGen wave sawtooth 1.098447 -1.098447 0.527409 0.303915 
rgbGen wave sawtooth 0.905087 -0.905087 0.527409 0.303915 
tcMod stretch sawtooth 0.395859 0.421647 0.527409 0.303915 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_7 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap 
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.650606 0.368229 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 10.157323 
AlphaGen wave sawtooth 0.910831 -0.910831 0.650606 0.368229 
rgbGen wave sawtooth 0.628663 -0.628663 0.650606 0.368229 
tcMod stretch sawtooth 0.396371 0.581820 0.650606 0.368229 
blendfunc add 
} 
} 

textures/pad_garden/wellen3_8 
{ 
qer_editorimage textures/pad_garden/wellen.tga 
surfaceparm noimpact 
surfaceparm nolightmap
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.134458 0.217227 
{ 
clampmap textures/pad_garden/wellen.tga 
tcMod rotate 9.057192 
AlphaGen wave sawtooth 1.037141 -1.037141 1.134458 0.217227 
rgbGen wave sawtooth 0.722733 -0.722733 1.134458 0.217227 
tcMod stretch sawtooth 0.388113 0.698187 1.134458 0.217227 
blendfunc add 
} 
} 


textures/pad_garden/wave0 
{ 
qer_editorimage textures/pad_garden/wave0.tga 
surfaceparm noimpact 
surfaceparm nolightmap 
nomipmaps 
cull none 
surfaceparm trans 
surfaceparm nonsolid 
surfaceparm nodlight 
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.765160 0.500000 
{ 
clampmap textures/pad_garden/wave1.tga 
tcMod rotate 5.752586 
AlphaGen wave sawtooth 1.000000 -0.421894 0.765160 0.500000 
rgbGen wave sawtooth 1.000000 -0.720054 0.765160 0.500000 
tcMod stretch sawtooth 0 0 0 0 
blendfunc add 
} 
{ 
clampmap textures/pad_garden/wave2.tga 
tcMod rotate 5.752586 
AlphaGen wave sawtooth 1.000000 -0.421894 0.765160 0.500000 
rgbGen wave sawtooth 1.000000 -0.720054 0.765160 0.500000 
tcMod stretch sawtooth 0.177834 0.822166 0.765160 0.500000 
blendfunc add 
} 
} 

textures/pad_garden/waterfall2 
{ 
qer_editorimage textures/pad_garden/waterstream.tga 
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
map textures/pad_garden/waterstream.tga 
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR 
tcMod scale 0.4 0.55 
tcMod turb .1 .08 .25 .08 
tcMod scroll 0.005 -1 
} 
{ 
map textures/pad_garden/waterstream.tga 
blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR 
tcMod scale 0.5 0.45 
tcMod turb .1 .05 .25 .08 
tcMod scroll 0.008 -0.6 
} 
} 


textures/pad_garden/gardengrass
{
q3map_nonplanar
q3map_shadeangle 60 l	
surfaceparm sandsteps
qer_editorimage textures/pad_garden/gardengrass.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/gardengrass.tga
blendFunc filter
}
}

textures/pad_garden/gardensand01
{
q3map_nonplanar
q3map_shadeangle 60 l
surfaceparm sandsteps
qer_editorimage textures/pad_garden/gardensand01.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/gardensand01.tga
blendFunc filter
}
}

textures/pad_garden/gardensand02
{
q3map_nonplanar
q3map_shadeangle 60 l
surfaceparm sandsteps
qer_editorimage textures/pad_garden/gardensand02.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/gardensand02.tga
blendFunc filter
}
}

textures/pad_garden/sand003a
{
q3map_nonplanar
q3map_shadeangle 60 l
surfaceparm sandsteps
qer_editorimage textures/pad_garden/sand003a.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/sand003a.tga
blendFunc filter
}
}

textures/pad_garden/algen
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_garden/algen.tga
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/algen.tga
blendFunc filter
}
}


textures/pad_garden/ashes_256
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_garden/ashes_256.tga
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/ashes_256.tga
blendFunc filter
}
}


textures/pad_garden/ashes_512
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_garden/ashes_512.tga
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/ashes_512.tga
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
                map textures/pad_garden/nail.tga
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
                map textures/pad_garden/kaktus002.tga
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
qer_editorimage textures/pad_garden/grasground.tga
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
map textures/pad_garden/sandground.tga // Primary
rgbGen identity
}
{
map textures/pad_garden/grasground.tga // Secondary
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
	qer_editorimage textures/pad_garden/seerosenblatt.tga
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
              surfaceparm snowsteps
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_garden/seerosenblatt.tga
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
map textures/pad_garden/fussmatte01.tga
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
map textures/pad_garden/liegestoff02.tga
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
map textures/pad_garden/lochgitter02.tga
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
map textures/pad_garden/lochgitter03.tga
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
map textures/pad_garden/lochgitter04.tga
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
map textures/pad_garden/lochgitter05.tga
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
map textures/pad_garden/rostblack.tga
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
map textures/pad_garden/rostborderbig.tga
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
map textures/pad_garden/rostbordersmall.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rostdoorbig
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rostbdoorbig.tga
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
map textures/pad_garden/rostdoorsmall.tga
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
map textures/pad_garden/faltblech.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/purpleflow
{
        qer_editorimage textures/pad_garden/purpleflow.tga
    	surfaceparm trans
	surfaceparm alphashadow
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_garden/purpleflow.tga
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
	qer_editorimage textures/pad_garden/rocket.tga
	{
		map textures/pad_garden/rocket.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
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
map textures/pad_garden/metalwhite001big.tga
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
map textures/pad_garden/birdfloor_big.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/birdfloor_small
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/birdfloor_small.tga
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
map textures/pad_garden/teichbrett2_512.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rost003csmall
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rost003csmall.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/rost003bsmall
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/rost003bsmall.tga
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
map textures/pad_garden/rostsoft.tga
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
map textures/pad_garden/rostrough.tga
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
map textures/pad_garden/rostred.tga
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
map textures/pad_garden/rostblack.tga
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
map textures/pad_garden/sunchairwood.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_garden/liegestoff
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/liegestoff.tga
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
map textures/pad_garden/liegestoff02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/liegestoff03
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/liegestoff03.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/liegestoff04
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_garden/liegestoff04.tga
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
map textures/pad_garden/toyblock01.tga
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
map textures/pad_garden/toyblock02.tga
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
map textures/pad_garden/toyblock03.tga
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
map textures/pad_garden/toyblock04.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_garden/big_flameblue256
{

		surfaceparm trans
		surfaceparm nomarks
		surfaceparm nonsolid
		qer_editorimage textures/pad_garden/bflame1.tga
		q3map_surfacelight 200
		surfaceparm nolightmap
		cull none

	{
		animMap 10 textures/pad_garden/bflame1.tga textures/pad_garden/bflame2.tga textures/pad_garden/bflame3.tga textures/pad_garden/bflame4.tga textures/pad_garden/bflame5.tga textures/pad_garden/bflame6.tga textures/pad_garden/bflame7.tga textures/pad_garden/bflame8.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 10
		
	}	
	{
		animMap 10 textures/pad_garden/bflame2.tga textures/pad_garden/bflame3.tga textures/pad_garden/bflame4.tga textures/pad_garden/bflame5.tga textures/pad_garden/bflame6.tga textures/pad_garden/bflame7.tga textures/pad_garden/bflame8.tga textures/pad_garden/bflame1.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
	}	


	{
		map textures/pad_garden/bflameball.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6	
	}

}


textures/pad_garden/red_flame256
{

		surfaceparm trans
		surfaceparm nomarks
		surfaceparm nonsolid
		qer_editorimage textures/pad_garden/sflame1.tga
		q3map_surfacelight 200
		surfaceparm nolightmap
		cull none

	{
		animMap 10 textures/pad_garden/sflame1.tga textures/pad_garden/sflame2.tga textures/pad_garden/sflame3.tga textures/pad_garden/sflame4.tga textures/pad_garden/sflame5.tga textures/pad_garden/sflame6.tga textures/pad_garden/sflame7.tga textures/pad_garden/sflame8.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 10
		
	}	
	{
		animMap 10 textures/pad_garden/sflame2.tga textures/pad_garden/sflame3.tga textures/pad_garden/sflame4.tga textures/pad_garden/sflame5.tga textures/pad_garden/sflame6.tga textures/pad_garden/sflame7.tga textures/pad_garden/sflame8.tga textures/pad_garden/sflame1.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
	}	


	{
		map textures/pad_garden/sflameball.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6	
	}

}


textures/pad_garden/poolfog_green
{
		qer_editorimage textures/pad_gfx02b/padfog_green.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .211 .231 .094 ) 650
}


textures/pad_garden/ship00
{
	qer_editorimage textures/pad_garden/ship00.tga
	{
		map textures/pad_garden/ship00.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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



textures/pad_garden/ship01
{
	qer_editorimage textures/pad_garden/ship01.tga
	{
		map textures/pad_garden/ship01.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
	qer_editorimage textures/pad_garden/ship02.tga
	{
		map textures/pad_garden/ship02.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
	qer_editorimage textures/pad_garden/ship04.tga
	{
		map textures/pad_garden/ship04.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
	qer_editorimage textures/pad_garden/ship05.tga
	{
		map textures/pad_garden/ship05.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
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

	qer_editorimage textures/pad_garden/ship06.tga
	{
		map textures/pad_garden/ship06.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
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
	qer_editorimage textures/pad_garden/ship07.tga
	{
		map textures/pad_garden/ship07.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
	qer_editorimage textures/pad_garden/ship08.tga
	{
		map textures/pad_garden/ship08.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
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
	qer_editorimage textures/pad_garden/ship10.tga
	{
		map textures/pad_garden/ship10.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
	qer_editorimage textures/pad_garden/radio002.tga
	{
		map textures/pad_garden/radio002.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage textures/pad_garden/radio003.tga
	{
		map textures/pad_garden/radio003.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage textures/pad_garden/radio004.tga
	{
		map textures/pad_garden/radio004.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage textures/pad_garden/radio005.tga
	{
		map textures/pad_garden/radio005.tga
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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