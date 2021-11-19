textures/pad_pirate/doornthree
{
        qer_editorimage textures/pad_pirate/doornthree.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_pirate/orange02.tga
	q3map_sun	1.000000 0.635081 0.272618 280 45 45
	q3map_surfacelight 230
 {
    alphafunc LT128
    map *white
  }

}





textures/pad_pirate/deep-six
{
        qer_editorimage textures/pad_pirate/deep-six.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_pirate/blue.tga
	q3map_sun	0.266383 0.274632 0.358662 130 65 55
	q3map_surfacelight 170

        skyparms env/deep-six512 - -
//       {
//		map textures/pad_pirate/deep-six.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}

textures/pad_pirate/padflagg
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_pirate/padflagg.tga
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


textures/pad_pirate/busch_p
{
        qer_editorimage textures/pad_pirate/busch_p.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/busch_p.tga
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


textures/pad_pirate/falcon
{
        qer_editorimage textures/pad_pirate/falcon.tga
    	surfaceparm trans
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/falcon.tga
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

textures/pad_pirate/flame1
{
	deformVertexes autoSprite2 
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	q3map_surfacelight 50
	qer_editorimage textures/pad_pirate/flame1.tga
	

	{
		animMap 3 textures/pad_pirate/flame1.tga textures/pad_pirate/flame2.tga textures/pad_pirate/flame3.tga textures/pad_pirate/flame2.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 10
		
	}	
	{
		animMap 3 textures/pad_pirate/flame2.tga textures/pad_pirate/flame3.tga textures/pad_pirate/flame2.tga textures/pad_pirate/flame1.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sawtooth 0 1 0 10
	}	


	{
		map textures/pad_pirate/flameball.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6	
	}

}


textures/pad_pirate/feder01
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_pirate/feder01.tga
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


textures/pad_pirate/feder02
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_pirate/feder02.tga
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


textures/pad_pirate/feder03
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_pirate/feder03.tga
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

textures/pad_pirate/klinge00
{
	qer_editorimage textures/pad_pirate/klinge00.tga
	{
		map textures/pad_pirate/klinge00.tga
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


textures/pad_pirate/klinge01
{
	qer_editorimage textures/pad_pirate/klinge01.tga
	{
		map textures/pad_pirate/klinge01.tga
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


textures/pad_pirate/annebonny2
{
        qer_editorimage textures/pad_pirate/annebonny2.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/annebonny2.tga
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

textures/pad_pirate/letter01
{
        qer_editorimage textures/pad_pirate/letter01.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/letter01.tga
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


textures/pad_pirate/letter02
{
        qer_editorimage textures/pad_pirate/letter02.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/letter02.tga
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

textures/pad_pirate/gold
{
	qer_editorimage textures/pad_pirate/gold.tga
	{
		map textures/pad_pirate/gold.tga
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

textures/pad_pirate/bubbles2_1
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.093287 -41.198784 641.500214 sawtooth 0 1 1.000725 0.195925
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 20.229958
AlphaGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
rgbGen wave sawtooth 1.000000 0.000000 1.000725 0.195925
tcMod stretch sawtooth 0.000000 1.138240 1.000725 0.195925
blendfunc add
}
}

textures/pad_pirate/bubbles2_2
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.769913 -21.519264 589.001572 sawtooth 0 1 0.622631 0.252309
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 2.200842
AlphaGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
rgbGen wave sawtooth 1.000000 0.000000 0.622631 0.252309
tcMod stretch sawtooth 0.000000 0.869579 0.622631 0.252309
blendfunc add
}
}

textures/pad_pirate/bubbles2_3
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.625161 -6.980746 507.586830 sawtooth 0 1 1.213530 0.445164
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 19.405041
AlphaGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
rgbGen wave sawtooth 1.000000 0.000000 1.213530 0.445164
tcMod stretch sawtooth 0.000000 2.052110 1.213530 0.445164
blendfunc add
}
}

textures/pad_pirate/bubbles2_4
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -8.668365 -12.663155 559.066895 sawtooth 0 1 1.085688 0.300365
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 11.414990
AlphaGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
rgbGen wave sawtooth 1.000000 0.000000 1.085688 0.300365
tcMod stretch sawtooth 0.000000 1.173104 1.085688 0.300365
blendfunc add
}
}

textures/pad_pirate/bubbles2_5
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -12.763139 -30.482132 651.611313 sawtooth 0 1 0.288759 0.189146
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 1.619465
AlphaGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
rgbGen wave sawtooth 1.000000 0.000000 0.288759 0.189146
tcMod stretch sawtooth 0.000000 1.285534 0.288759 0.189146
blendfunc add
}
}

textures/pad_pirate/bubbles2_6
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 7.834942 -10.679131 530.490509 sawtooth 0 1 1.237915 0.365962
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate -2.907956
AlphaGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
rgbGen wave sawtooth 1.000000 0.000000 1.237915 0.365962
tcMod stretch sawtooth 0.000000 1.686840 1.237915 0.365962
blendfunc add
}
}

textures/pad_pirate/bubbles2_7
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 5.454388 -41.883984 627.730408 sawtooth 0 1 0.402287 0.207241
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 3.890042
AlphaGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
rgbGen wave sawtooth 1.000000 0.000000 0.402287 0.207241
tcMod stretch sawtooth 0.000000 0.946705 0.402287 0.207241
blendfunc add
}
}

textures/pad_pirate/bubbles2_8
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -9.112141 -36.627399 616.117142 sawtooth 0 1 0.784623 0.218790
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 24.587999
AlphaGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
rgbGen wave sawtooth 1.000000 0.000000 0.784623 0.218790
tcMod stretch sawtooth 0.000000 0.350276 0.784623 0.218790
blendfunc add
}
}

textures/pad_pirate/bubbles2_9
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 13.988351 -11.087501 603.268707 sawtooth 0 1 1.044519 0.235235
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 12.468795
AlphaGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
rgbGen wave sawtooth 1.000000 0.000000 1.044519 0.235235
tcMod stretch sawtooth 0.000000 1.113410 1.044519 0.235235
blendfunc add
}
}

textures/pad_pirate/bubbles2_10
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 10.265741 7.714635 731.291351 sawtooth 0 1 0.524331 0.144779
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 12.796563
AlphaGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
rgbGen wave sawtooth 1.000000 0.000000 0.524331 0.144779
tcMod stretch sawtooth 0.000000 0.329548 0.524331 0.144779
blendfunc add
}
}

textures/pad_pirate/bubbles2_11
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 25.100649 4.780672 586.159500 sawtooth 0 1 0.907308 0.255448
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate 16.214331
AlphaGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
rgbGen wave sawtooth 1.000000 0.000000 0.907308 0.255448
tcMod stretch sawtooth 0.000000 2.025596 0.907308 0.255448
blendfunc add
}
}

textures/pad_pirate/bubbles2_12
{
qer_editorimage textures/pad_pirate/bubbles2.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 12.983425 -1.691125 495.949432 sawtooth 0 1 0.845538 0.495670
{
clampmap textures/pad_pirate/bubbles2.tga
tcMod rotate -2.724845
AlphaGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
rgbGen wave sawtooth 1.000000 0.000000 0.845538 0.495670
tcMod stretch sawtooth 0.000000 0.903418 0.845538 0.495670
blendfunc add
}
}


textures/pad_pirate/coral
{
	qer_editorimage textures/pad_pirate/coral.tga
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_pirate/coral.tga
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


\\textures/pad_pirate/pglass01b 
{
	q3map_nolightmap
	q3map_onlyvertexlighting
    {
        map textures/pad_pirate/pglass01b.tga
        blendFunc GL_ONE GL_ONE
        rgbgen exactVertex
    }
}


\\textures/pad_pirate/pglass02b 
{
	q3map_nolightmap
	q3map_onlyvertexlighting
    {
        map textures/pad_pirate/pglass02b.tga
        blendFunc GL_ONE GL_ONE
        rgbgen exactVertex
    }
}


\\textures/pad_pirate/pglass03b 
{
	q3map_nolightmap
	q3map_onlyvertexlighting
    {
        map textures/pad_pirate/pglass03b.tga
        blendFunc GL_ONE GL_ONE
        rgbgen exactVertex
    }
}


textures/pad_pirate/pglass03
{
	qer_editorimage textures/pad_pirate/pglass03.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass03.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_pirate/pglass01
{
	qer_editorimage textures/pad_pirate/pglass01.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass01.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_pirate/pglass02
{
	qer_editorimage textures/pad_pirate/pglass02.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass02.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}



textures/pad_pirate/pglass04
{
	qer_editorimage textures/pad_pirate/pglass04.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass04.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_pirate/pglass05
{
	qer_editorimage textures/pad_pirate/pglass05.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass05.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_pirate/pglass06
{
	qer_editorimage textures/pad_pirate/pglass06.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass06.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}


textures/pad_pirate/pglass07
{
	qer_editorimage textures/pad_pirate/pglass07.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_pirate/pglass07.tga
                blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
	}
           
}

textures/pad_pirate/longsail02
{
        qer_editorimage textures/pad_pirate/longsail02.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/longsail02.tga
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


textures/pad_pirate/blanken01b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/blanken01b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_pirate/wood_line
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood_line.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 


textures/pad_pirate/kreideship
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_pirate/kreideship.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_pirate/kupfer001
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_pirate/kupfer001.tga
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
        {
		map textures/pad_gfx02/tinpad.tga
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
       
        {
		map textures/pad_pirate/kupfer001.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_pirate/kupfer002
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_pirate/kupfer002.tga
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
        {
		map textures/pad_gfx02/tinpad.tga
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
       
        {
		map textures/pad_pirate/kupfer002.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_pirate/kupfer003
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_pirate/kupfer003.tga
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
        {
		map textures/pad_gfx02/tinpad.tga
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
       
        {
		map textures/pad_pirate/kupfer003.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_pirate/pladde
{
        qer_editorimage textures/pad_pirate/pladde.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_pirate/pladde.tga
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


textures/pad_pirate/becher1
{
	qer_editorimage textures/pad_pirate/becher1.tga
	{
		map textures/pad_pirate/becher1.tga
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


textures/pad_pirate/becher2
{
	qer_editorimage textures/pad_pirate/becher2.tga
	{
		map textures/pad_pirate/becher2.tga
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


textures/pad_pirate/klinge00
{
	qer_editorimage textures/pad_pirate/klinge00.tga
	{
		map textures/pad_pirate/klinge00.tga
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

textures/pad_pirate/klinge01
{
	qer_editorimage textures/pad_pirate/klinge01.tga
	{
		map textures/pad_pirate/klinge01.tga
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


textures/pad_pirate/telescope01
{
	qer_editorimage textures/pad_pirate/telescope01.tga
	{
		map textures/pad_pirate/telescope01.tga
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


textures/pad_pirate/blanken01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/blanken01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/blanken01b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/blanken01b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/boarddoor
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/boarddoor.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}



textures/pad_pirate/doorship04
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/doorship04.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/doorshipkant
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/doorshipkant.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/mop
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/mop.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/p_sugar
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/p_sugar.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/planke01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/planke01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/planke02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/planke02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/planke03
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/planke03.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/planken02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/planken02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/segel
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/segel.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/seil
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/seil.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/shipbalken
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipbalken.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipbalken02b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipbalken02b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/shipbalkenb
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipbalkenb.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipluke2
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipluke2.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/shipmast
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipmast.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipplanke_blank
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_blank.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipplanke_blank3
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_blank3.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipplanke_wall02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_wall02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipplanke_wall04
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_wall04.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/shipplanke_wall05b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_wall05b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/shipplanke_wall08
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/shipplanke_wall08.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/stufen
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/stufen.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/sugar
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/sugar.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/teppich01
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/teppich01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/trepside
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/trepside.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhe001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhe001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhe001b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhe001b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/truhe002_big
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhe002_big.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhe002b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhe002b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhelatten001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhelatten001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhelatten001b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhelatten001b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/truhenplatte
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/truhenplatte.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood_canon
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood_canon.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/wood_line
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood_line.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood017black
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood017black.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood017Sblueb
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood017Sblueb.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/wood017Sgreenb
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood017Sgreenb.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood017Sredb
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood017Sredb.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood017yellow
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood017yellow.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood093cb
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood093cb.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/woodlatten01b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/woodlatten01b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/woodlattenblue02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/woodlattenblue02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/woodlattenred02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/woodlattenred02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/woodywood
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/woodywood.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/woodzier008cc
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/woodzier008cc.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wreling04b
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wreling04b.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/wood038fleck
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/wood038fleck.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_pirate/canon1024
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/canon1024.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_pirate/canon512
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_pirate/canon512.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}



textures/pad_pirate/waterdecke
{
	tessSize 64
	deformVertexes normal 1 1
	qer_editorimage textures/pad_pirate/holzdecke.tga
        {
		map textures/pad_pirate/holzdecke.tga
                rgbGen identity    
        }
        {
		map textures/pad_pirate/waterreflection.tga
                Blendfunc add
		tcgen environment
		rgbgen wave sin .2 0 0 0
		tcmod scale 1 1
		tcMod scroll .1 .2
	}
        {
		map $lightmap
                blendFunc filter
		rgbGen identity
	}
}


textures/pad_pirate/candleflare
{
    deformVertexes autoSprite
    surfaceparm trans
    surfaceparm nomarks
    surfaceparm nonsolid		
    surfaceparm nolightmap
    cull none
          {
            clampmap textures/pad_pirate/candleflare.tga
            blendFunc GL_ONE GL_ONE
          }
}


textures/pad_pirate/bechercurve
{
          qer_editorimage textures/pad_pirate/becher1.tga
           surfaceparm nonsolid		
	{
		map textures/pad_pirate/becher1.tga
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


textures/pad_pirate/bechercurve2
{
          qer_editorimage textures/pad_pirate/becher2.tga
           surfaceparm nonsolid		
	{
		map textures/pad_pirate/becher2.tga
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


textures/pad_pirate/bonnycurve
{
          qer_editorimage textures/pad_pirate/bonnyflag.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/bonnyflag.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/gramopcurve
{
          qer_editorimage textures/pad_pirate/gramop04.tga
           surfaceparm nonsolid		
	{
		map textures/pad_pirate/gramop04.tga
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

textures/pad_pirate/griffcurve
{
          qer_editorimage textures/pad_pirate/griff01.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/griff01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/kerzecurve1
{
          qer_editorimage textures/pad_pirate/kerze01.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/kerze01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/kerzecurve2
{
          qer_editorimage textures/pad_pirate/kerze02.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/kerze02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/klingecurve1
{
          qer_editorimage textures/pad_pirate/klinge00.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/klinge00.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/klingecurve2
{
          qer_editorimage textures/pad_pirate/klinge01.tga
           surfaceparm nonsolid		
	{
		map textures/pad_pirate/klinge01.tga
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


textures/pad_pirate/mopcurve
{
          qer_editorimage textures/pad_pirate/mop.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/mop.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/oldfloorbluecurve
{
          qer_editorimage textures/pad_pirate/oldfloorrandblue.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/oldfloorrandblue.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/oldfloorgreencurve
{
          qer_editorimage textures/pad_pirate/oldfloorrandgreen.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/oldfloorrandgreen.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_pirate/sugarcurve
{
          qer_editorimage textures/pad_pirate/p_sugar.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/p_sugar.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/plankecurve01
{
          qer_editorimage textures/pad_pirate/planke01.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/planke01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/plankecurve02
{
          qer_editorimage textures/pad_pirate/planke02.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/planke02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/plankecurve03
{
          qer_editorimage textures/pad_pirate/planke03.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/planke03.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_pirate/segecurve
{
          qer_editorimage textures/pad_pirate/segel.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/segel.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/seicurve
{
          qer_editorimage textures/pad_pirate/seil.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/seil.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/seilycurve
{
          qer_editorimage textures/pad_pirate/seilycurve.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/seilycurve.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/stonecurve01
{
          qer_editorimage textures/pad_pirate/stone_bl.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/stone_bl.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/stonecurve02
{
          qer_editorimage textures/pad_pirate/stone_or.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/stone_or.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_wallout/stonecurve03
{
          qer_editorimage textures/pad_wallout/wall_stone005d.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wallout/wall_stone005d.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_wallout/stonecurve04
{
          qer_editorimage textures/pad_wallout/wall_stone006c.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wallout/wall_stone006c.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_wallout/stonecurve05
{
          qer_editorimage textures/pad_wallout/wall_stone051.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wallout/wall_stone051.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/korkycurve
{
          qer_editorimage textures/pad_pirate/kork.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/kork.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_pirate/telescopecurve01
{
          qer_editorimage textures/pad_pirate/telescope01.tga
           surfaceparm nonsolid		
	{
		map textures/pad_pirate/lelescope01.tga
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


textures/pad_pirate/telescopecurve03
{
          qer_editorimage textures/pad_pirate/telescope3.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_pirate/telescope3.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_garden/poolstoncurve
{
          qer_editorimage textures/pad_garden/poolstone03.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_garden/poolstone03.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_gfx02/metalcurve08
{
          qer_editorimage textures/pad_gfx02/metal08.tga
          surfaceparm nonsolid		
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal08.tga
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
        {
		map textures/pad_gfx02/tinpad.tga
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
       
        {
		map textures/pad_gfx02/metal08.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}  


textures/pad_wood/woo48stoncurve
{
          qer_editorimage textures/pad_wood/wood048.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wood/wood048.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/pad_wood/woo39stoncurve
{
          qer_editorimage textures/pad_wood/wood039.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wood/wood039.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/pad_wood/woo37stoncurve
{
          qer_editorimage textures/pad_wood/wood037.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wood/wood037.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/pad_wood/woo34stoncurve
{
          qer_editorimage textures/pad_wood/wood034.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wood/wood034.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/pad_wood/woo26stoncurve
{
          qer_editorimage textures/pad_wood/wood026.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wood/wood026.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/pad_garden/rostcurve
{
          qer_editorimage textures/pad_garden/rostrough.tga
           surfaceparm nonsolid		
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


textures/pad_wallin/canoncurve
{
          qer_editorimage textures/pad_wallin/wallcol26.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_wallin/wallcol26.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_objects/shiplampcurve
{
          qer_editorimage textures/pad_objects/shiplampcurve.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_objects/ofen2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_metal/wallycurve
{
          qer_editorimage textures/pad_metal/wallycurve.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/wallrast_a.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}