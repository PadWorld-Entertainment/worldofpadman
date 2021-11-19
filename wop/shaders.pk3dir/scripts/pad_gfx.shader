//=======================================================
//sinnfreie gfx
//=======================================================
textures/pad_gfx/blobfx
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided

	deformVertexes wave 30 sin 0 5 0 0.2 
	deformVertexes wave 100 sin 0 4 0 0.1 
	tessSize 48
	qer_trans 0.5
	{
		map textures/pad_gfx/blobfx.tga
		tcGen environment
                tcMod turb 0 0.35 0 0.25
                tcmod scroll -0.6 -0.8
	}
	{
		map textures/pad_gfx/blobfx.tga
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}

//=======================================================
//glass
//=======================================================
textures/pad_gfx/glass_bright
{
	qer_editorimage textures/pad_gfx/glass_bright.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_bright.tga
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

textures/pad_gfx/glass_normal
{
	qer_editorimage textures/pad_gfx/glass_normal.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_normal.tga
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

textures/pad_gfx/glass_dark
{
	qer_editorimage textures/pad_gfx/glass_dark.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_dark.tga
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


textures/pad_gfx/glass_blue
{
	qer_editorimage textures/pad_gfx/glass_blue.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_blue.tga
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


textures/pad_gfx/glass_red
{
	qer_editorimage textures/pad_gfx/glass_red.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_red.tga
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


textures/pad_gfx/glass_green
{
	qer_editorimage textures/pad_gfx/glass_green.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass_green.tga
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


textures/pad_gfx/glass2_bright
{
	qer_editorimage textures/pad_gfx/glass2_bright.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_bright.tga
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

textures/pad_gfx/glass2_normal
{
	qer_editorimage textures/pad_gfx/glass2_normal.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_normal.tga
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

textures/pad_gfx/glass2_dark
{
	qer_editorimage textures/pad_gfx/glass2_dark.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_dark.tga
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


textures/pad_gfx/glass2_blue
{
	qer_editorimage textures/pad_gfx/glass2_blue.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_blue.tga
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


textures/pad_gfx/glass2_red
{
	qer_editorimage textures/pad_gfx/glass2_red.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_red.tga
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


textures/pad_gfx/glass2_green
{
	qer_editorimage textures/pad_gfx/glass2_green.tga
        surfaceparm trans	
	cull none
	surfaceparm nolightmap
	qer_trans 	0.5
     
        {
		map textures/pad_gfx/glass2_green.tga
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
//flare
//=======================================================
textures/pad_gfx/flare
{
    deformVertexes autoSprite
    surfaceparm trans
    surfaceparm nomarks
    surfaceparm nolightmap
    cull none
          {
            clampmap textures/pad_gfx/flare.tga
            blendFunc GL_ONE GL_ONE
          }
}


//=======================================================
//auto lack (orange)
//=======================================================

textures/pad_gfx/lack
{
	qer_editorimage textures/pad_gfx/lack.tga
	{
		map textures/pad_gfx/lack.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}

//=======================================================
//abgedunkeltes fenster
//=======================================================

textures/pad_gfx/window
{
	qer_editorimage textures/pad_gfx/window.tga
	{
		map textures/pad_gfx/window.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}


textures/pad_gfx/lack_blue
{
	qer_editorimage textures/pad_gfx/lack_blue.tga
	{
		map textures/pad_gfx/lack_blue.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}


textures/pad_gfx/lack_green
{
	qer_editorimage textures/pad_gfx/lack_green.tga
	{
		map textures/pad_gfx/lack_green.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}


textures/pad_gfx/lack_red
{
	qer_editorimage textures/pad_gfx/lack_red.tga
	{
		map textures/pad_gfx/lack_red.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}

//=======================================================
//schnee
//=======================================================

textures/pad_gfx/snow
{
	qer_editorimage textures/pad_gfx/snow.tga
	deformVertexes autosprite2
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	cull disable
	qer_trans 0.5
	{
		map textures/pad_gfx/snow.tga
		blendFunc add
		rgbGen identity
		tcMod scroll 0.1 -0.6
		tcMod turb 0.1 0.25 0 -0.1
	} 
}

//=======================================================
//spülwasser + zubehör :)
//=======================================================

textures/pad_gfx/water_static
{
	qer_editorimage textures/pad_gfx/water_01.tga
	q3map_lightimage textures/pad_gfx/water_02.tga
	surfaceparm nodrop
	surfaceparm noimpact
	//surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm water
	surfaceparm trans
	surfaceparm fog
	fogparms ( 0 0.27 0.42 ) 500
	cull disable
	qer_trans 0.5
	q3map_globaltexture
	

	{
		map textures/pad_gfx/water_01.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.2 0.1 1 0.05
		tcMod scale 0.5 0.5
		tcMod scroll 0.01 0.01
	}

}

textures/pad_gfx/water
{
	qer_editorimage textures/pad_gfx/water_02.tga
	q3map_lightimage textures/pad_gfx/water_02.tga
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
		map textures/liquids/pool2.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.4 0.3 1 0.05
		tcMod scale 0.5 0.5
		tcMod scroll 0.01 1.4
	}
}

textures/pad_gfx/schaum_static
{
	cull disable	 
        surfaceparm nomarks 
        surfaceparm trans 
        sort additive	
	qer_trans 0.5
	{
		map textures/pad_gfx/schaum_static.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.1 0.05 0 0.5
	}
}

textures/pad_gfx/schaum1
{
	qer_editorimage textures/pad_gfx/schaum.tga
	deformVertexes autosprite
	cull disable	 
        surfaceparm nomarks 
        surfaceparm trans 
        sort additive	
	qer_trans 0.5
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.1 0.05 0 0.95
		tcMod rotate 6

	}
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.8 0.08 1 -0.64
		tcMod rotate -10
		tcmod scale 0.75 0.75
	}
}

textures/pad_gfx/schaum2
{
	qer_editorimage textures/pad_gfx/schaum.tga
	deformVertexes autosprite
	cull disable	 
        surfaceparm nomarks 
        surfaceparm trans 
        sort additive	
	qer_trans 0.5
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.1 0.07 0 -0.9
		tcMod rotate -8
		tcmod scale 0.75 0.75
	}
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.8 0.03 1 -1
		tcMod rotate 10

	}

}

textures/pad_gfx/schaum3
{
	qer_editorimage textures/pad_gfx/schaum.tga
	deformVertexes autosprite
	cull disable	 
        surfaceparm nomarks 
        surfaceparm trans 
        sort additive	
	qer_trans 0.5
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.1 0.06 0 -0.4
		tcMod rotate 15
		tcmod scale 0.75 0.75
	}
	{
		map textures/pad_gfx/schaum.tga
		blendfunc add
		rgbGen identity
		tcMod turb 0.8 0.04 1 -0.8
		tcMod rotate 23
		tcmod scale 0.75 0.75
	}
}


//=======================================================
//Flowers
//=======================================================

textures/pad_gfx02/flow01
{
        qer_editorimage textures/pad_gfx02/flow01.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/flow01.tga
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


textures/pad_gfx02/flow02
{
        qer_editorimage textures/pad_gfx02/flow02.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/flow02.tga
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


textures/pad_gfx02/flow04
{
        qer_editorimage textures/pad_gfx02/flow04.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/flow04.tga
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


textures/pad_gfx02/cact03
{
        qer_editorimage textures/pad_gfx02/cact03.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/cact03.tga
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


textures/pad_gfx02/busch
{
        qer_editorimage textures/pad_gfx02/busch.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/busch.tga
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



textures/pad_gfx02/sun
{
        qer_editorimage textures/pad_gfx02/sun.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/sun.tga
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


textures/pad_gfx02/blatt01
{
        qer_editorimage textures/pad_gfx02/blatt01.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/blatt01.tga
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


//=======================================================
//Light
//=======================================================


textures/pad_gfx02/lighty02
{
    q3map_surfacelight 3000
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/lighty02.tga
    }
}


textures/pad_gfx02/lighty03
{
    q3map_surfacelight 2500
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/lighty03.tga
    }
}


textures/pad_gfx02/lighty
{
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/lighty.tga
    }
}


textures/pad_gfx02/light_blue
{
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/light_blue.tga
    }
}


textures/pad_gfx02/light_green
{
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/light_green.tga
    }
}


textures/pad_gfx02/light_green2
{
    q3map_surfacelight 1400
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/light_green2.tga
    }
}



textures/pad_gfx02/light_white
{
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/pad_gfx02/light_white.tga
    }
}


//=======================================================
//Mixed stuff
//=======================================================


textures/pad_gfx02/padp
{
        qer_editorimage textures/pad_gfx02/padp.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/padp.tga
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


textures/pad_gfx02/teller
{
        qer_editorimage textures/pad_gfx02/teller.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/teller.tga
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



textures/pad_gfx02/rollo
{
        qer_editorimage textures/pad_gfx02/rollo.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/rollo.tga
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


textures/pad_gfx02/rolloB
{
        qer_editorimage textures/pad_gfx02/rolloB.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/rolloB.tga
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


textures/pad_gfx02/rollo2B
{
        qer_editorimage textures/pad_gfx02/rollo2B.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/rollo2B.tga
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




textures/pad_gfx02/faden05
{
        qer_editorimage textures/pad_gfx02/faden05.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/faden05.tga
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




//=======================================================
//Gitter und Zeaune
//=======================================================


textures/pad_gfx02/gitter01
{
        qer_editorimage textures/pad_gfx02/git01.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/git01.tga
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

textures/pad_gfx02/gitter03
{
        qer_editorimage textures/pad_gfx02/git03.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/git03.tga
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


textures/pad_gfx02/gitter04
{
        qer_editorimage textures/pad_gfx02/git04.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/git04.tga
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
   


textures/pad_gfx02/zaun02
{
        qer_editorimage textures/pad_gfx02/zaun02.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_gfx02/zaun02.tga
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


//=======================================================
//Plastic lack
//=======================================================

textures/pad_gfx02/plas01
{
	qer_editorimage textures/pad_gfx02/plas01.tga
	{
		map textures/pad_gfx02/plas01.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}


textures/pad_gfx02/plas02
{
	qer_editorimage textures/pad_gfx02/plas02.tga
	{
		map textures/pad_gfx02/plas02.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}

textures/pad_gfx02/plas07
{
	qer_editorimage textures/pad_gfx02/plas07.tga
	{
		map textures/pad_gfx02/plas07.tga
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad.tga
        	tcGen environment
                blendfunc add
                rgbGen identity
	}
	{
		map $lightmap
		blendfunc filter
		rgbgen identity
	}
}


textures/pad_gfx02/col01
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/col01.tga
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
		map textures/pad_gfx02/col01.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_gfx02/col02
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/col02.tga
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
		map textures/pad_gfx02/col02.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_gfx02/col03
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/col03.tga
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
		map textures/pad_gfx02/col03.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_gfx02/col04
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/col04.tga
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
		map textures/pad_gfx02/col04.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   



//=======================================================
//Metal
//=======================================================

textures/pad_gfx02/metal01
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal01.tga
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
		map textures/pad_gfx02/metal01.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}       


textures/pad_gfx02/metal02
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal02.tga
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
		map textures/pad_gfx02/metal02.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}    


textures/pad_gfx02/metal03
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal03.tga
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
		map textures/pad_gfx02/metal03.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}      



textures/pad_gfx02/metal04
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal04.tga
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
		map textures/pad_gfx02/metal04.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   




textures/pad_gfx02/metal05
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal05.tga
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
		map textures/pad_gfx02/metal05.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}    


          

textures/pad_gfx02/metal06
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal06.tga
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
		map textures/pad_gfx02/metal06.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}     


textures/pad_gfx02/metal06b
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal06b.tga
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
		map textures/pad_gfx02/metal06b.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}     




textures/pad_gfx02/metal08
{
	//deformVertexes wave 100 sin 3 2 .1 3
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


textures/pad_gfx02/metal09
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal09.tga
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
		map textures/pad_gfx02/metal09.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}         



textures/pad_gfx02/metal10
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal10.tga
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
		map textures/pad_gfx02/metal10.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}     


textures/pad_gfx02/metal11
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_gfx02/metal11.tga
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
		map textures/pad_gfx02/metal11.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


//=======================================================
//Water
//=======================================================
        
textures/pad_gfx02/wat01
	{
		qer_editorimage textures/pad_gfx02/wat01.tga
		qer_trans .5
		q3map_globaltexture
		surfaceparm trans
		surfaceparm nonsolid
		surfaceparm water
	
		cull disable
		deformVertexes wave 64 sin .5 .5 0 .5	
		
		{ 
			map textures/pad_gfx02/wat02.tga
			blendFunc GL_dst_color GL_one
			rgbgen identity
			tcmod scale .5 .5
			tcmod transform 1.5 0 1.5 1 1 2
			tcmod scroll -.05 .001
		}
	
		{ 
			map textures/pad_gfx02/wat03.tga
			blendFunc GL_dst_color GL_one
			rgbgen identity
			tcmod scale .5 .5
			tcmod transform 0 1.5 1 1.5 2 1
			tcmod scroll .025 -.001
		}

		{ 
			map textures/pad_gfx02/wat01.tga
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



textures/pad_gfx02/wat04

{
		qer_editorimage textures/pad_gfx02/wat04.tga
		qer_trans .5
		q3map_globaltexture
		surfaceparm trans
		surfaceparm nonsolid
		surfaceparm water
	
		cull disable
		deformVertexes wave 64 sin .5 .5 0 .5	
		
		{ 
			map textures/pad_gfx02/wat05.tga
			blendFunc GL_dst_color GL_one
			rgbgen identity
			tcmod scale .5 .5
			tcmod transform 1.5 0 1.5 1 1 2
			tcmod scroll -.05 .001
		}
	
		{ 
			map textures/pad_gfx02/wat06.tga
			blendFunc GL_dst_color GL_one
			rgbgen identity
			tcmod scale .5 .5
			tcmod transform 0 1.5 1 1.5 2 1
			tcmod scroll .025 -.001
		}

		{ 
			map textures/pad_gfx02/wat04.tga
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

//=======================================================
//webs
//=======================================================

textures/pad_gfx02b/cweb_m01drk
{
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    deformVertexes wave 10 sin 0 2 0 0.2
    {
        map textures/pad_gfx02b/cweb_m01drk.tga
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}


textures/pad_gfx02b/cweb_m01a
{
    //removelightmap info
    //surfaceparm nolightmap
    qer_trans 0.8
    surfaceparm alphashadow
    surfaceparm nomarks
    surfaceparm nonsolid
    cull disable
    deformVertexes wave 10 sin 0 2 0 0.2
    {
        map textures/pad_gfx02b/cweb_m01a.tga
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen vertex
    }
}



//=======================================================
//mixed attic
//=======================================================


textures/pad_gfx02b/latern
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_gfx02b/latern.tga
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


//=======================================================
//mixed misc
//=======================================================


textures/pad_gfx02b/desk04
{
	q3map_lightimage textures/pad_gfx02b/desk04.tga
	q3map_surfacelight 70

        {
		map textures/pad_gfx02b/desk04.tga
		rgbGen identity
	}
	
        {
		map textures/pad_gfx02b/comp_line02.tga
		blendfunc add
		rgbGen identity
		tcmod scroll 0 1
	}

        {
		map textures/pad_gfx02b/compsnowb.tga
	        blendfunc add
                rgbGen identity
		tcmod scroll 3 3
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


textures/pad_gfx02b/git001b
{
     qer_editorimage textures/pad_gfx02b/git001b.tga   
     cull none
     surfaceparm alphashadow
     surfaceparm playerclip	
     surfaceparm nomarks
     nopicmip
     {
                map textures/pad_gfx02b/git001b.tga
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

textures/pad_gfx02b/git001d
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_gfx02b/git001d.tga
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



textures/pad_gfx02b/git005
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
    
     
        {
                map textures/pad_gfx02b/git005.tga
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




textures/pad_gfx02b/watblack
{	
	qer_editorimage textures/pad_gfx02b/watblack.tga
	q3map_lightimage textures/pad_gfx02b/watblack.tga
	q3map_globaltexture
	qer_trans .5

	surfaceparm noimpact
	surfaceparm slime
	surfaceparm nolightmap
	surfaceparm trans		

	q3map_surfacelight 20
	tessSize 32
	cull disable

	deformVertexes wave 100 sin 0 1 .2 .3

	{
		map textures/pad_gfx02b/watblackb.tga
		tcMod turb .3 .2 1 .05
		tcMod scroll .025 .025
	}
	
	{
		map textures/pad_gfx02b/watblack.tga
		blendfunc GL_ONE GL_ONE
		tcMod turb .2 .1 1 .05
		tcMod scale .5 .5
		tcMod scroll .02 .02
	}
	

}



textures/pad_gfx02b/watyell
{	
	qer_editorimage textures/pad_gfx02b/watyell.tga
	q3map_lightimage textures/pad_gfx02b/watyell.tga
	q3map_globaltexture
	qer_trans .5

	surfaceparm noimpact
	surfaceparm slime
	surfaceparm nolightmap
	surfaceparm trans		

	q3map_surfacelight 20
	tessSize 32
	cull disable

	deformVertexes wave 100 sin 0 1 .2 .3

	{
		map textures/pad_gfx02b/watyellow.tga
		tcMod turb .3 .2 1 .05
		tcMod scroll .025 .025
	}
	
	{
		map textures/pad_gfx02b/watyell.tga
		blendfunc GL_ONE GL_ONE
		tcMod turb .2 .1 1 .05
		tcMod scale .5 .5
		tcMod scroll .02 .02
	}
	

}



textures/pad_gfx02b/beam_yellow
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_yellow.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_red
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_red.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_blue
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_blue.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_grey
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_grey.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_purple
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_purple.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_brown
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_brown.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}


textures/pad_gfx02b/beam_green
{
        surfaceparm trans	
        surfaceparm nomarks	
        surfaceparm nonsolid
	surfaceparm nolightmap
	cull none
	surfaceparm nomipmaps
        //nopicmip
	{
		map textures/pad_gfx02b/beam_green.tga
                tcMod Scroll .4 0
                blendFunc add
        }
     
}



textures/pad_gfx02b/hotlava1
{
	qer_editorimage textures/pad_gfx02b/hotlava2d.tga
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	q3map_surfacelight 200
	cull disable
	
	tesssize 128
	cull disable
	deformVertexes wave 100 sin 3 2 .1 0.1
	
	{
		map textures/pad_gfx02b/hotlava2d.tga
		tcMod turb 0 .2 0 .1
	}
}



textures/pad_gfx02b/greenlava
{
	qer_editorimage textures/pad_gfx02b/greenlava.tga
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	q3map_surfacelight 200
	cull disable
	
	tesssize 128
	cull disable
	deformVertexes wave 100 sin 3 2 .1 0.1
	
	{
		map textures/pad_gfx02b/greenlava.tga
		tcMod turb 0 .2 0 .1
	}
}




textures/pad_gfx02b/hotlava2
{
	
	qer_editorimage textures/pad_gfx02b/hotlava2.tga
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	q3map_surfacelight 300
	cull disable
	
	tesssize 128
	cull disable
	deformVertexes wave 100 sin 3 2 .1 0.1
	
        {
		map textures/pad_gfx02b/hotmatsch.tga
                tcmod scale .2 .2
                tcmod scroll .04 .03
                tcMod turb 0 .1 0 .01
                blendFunc GL_ONE GL_ZERO
                rgbGen identity
	}
	{
		map textures/pad_gfx02b/hotlava2.tga
                blendfunc blend
		tcMod turb 0 .2 0 .1
	}

}


textures/pad_gfx02b/hotlava3
{	
	qer_editorimage textures/pad_gfx02b/hotlava2d.tga
	q3map_lightimage textures/pad_gfx02b/hotmatsch.tga
	q3map_globaltexture
	qer_trans .5

	surfaceparm noimpact
	surfaceparm lava
	surfaceparm nolightmap
	surfaceparm trans		

	q3map_surfacelight 20
	tessSize 128
	cull disable

	deformVertexes wave 100 sin 0 1 .2 .3

	{
		map textures/pad_gfx02b/hotlava2d.tga
		tcMod turb .3 .2 1 .05
		tcMod scroll .025 .025
	}
	
	{
		map textures/pad_gfx02b/hotmatsch.tga
		blendfunc GL_ONE GL_ONE
		tcMod turb .2 .1 1 .05
		tcMod scale .5 .5
		tcMod scroll .02 .02
	}
	

}



textures/pad_gfx02b/padfog_yel
{
		qer_editorimage textures/pad_gfx02b/padfog_yel.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .615 .309 0 ) 616
}


textures/pad_gfx02b/padfog_yel02
{
		qer_editorimage textures/pad_gfx02b/padfog_yel.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .75 .38 0 ) 800
}


textures/pad_gfx02b/padfog_red
{
		qer_editorimage textures/pad_gfx02b/padfog_red.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .5 0 0 ) 96
}


textures/pad_gfx02b/padfog_red02
{
	q3map_lightimage textures/pad_gfx02b/hotmatsch.tga	
	qer_editorimage textures/pad_gfx02b/padfog_red02.tga
	q3map_surfacelight 30
	q3map_lightsubdivide 32

	surfaceparm	trans
	surfaceparm	nonsolid
	surfaceparm	fog
	surfaceparm	nolightmap
	qer_nocarve
	fogparms ( 0.9 0.3 0.3 ) 128

}


textures/pad_gfx02b/padfog_red03
{
		qer_editorimage textures/pad_gfx02b/padfog_red.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( 0.3 0.2 0.2 ) 320

}


textures/pad_gfx02b/padfog_green
{
		qer_editorimage textures/pad_gfx02b/padfog_green.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .211 .231 .094 ) 250
}


textures/pad_gfx02b/padfog_green02
{
		qer_editorimage textures/pad_gfx02b/padfog_green.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .250 .280 .094 ) 800
}


textures/pad_gfx02b/padfog_grey
{
		qer_editorimage textures/pad_gfx02b/padfog_grey.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .733 .737 .620 ) 300

}


textures/pad_gfx02b/padfog_blue
{
		qer_editorimage textures/pad_gfx02b/padfog_blue.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( 0.1 0.1 0.3 ) 320

}


textures/pad_gfx02b/padfog_grey02
{
		qer_editorimage textures/pad_gfx02b/padfog_grey.tga
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		fogparms ( .729 .729 .780 ) 464

}



textures/pad_gfx02b/padmove05
{
	tessSize 128
	deformVertexes wave 100 sin 4 3 0 0.3
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_gfx02b/padmove05.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}



textures/pad_gfx02b/padflagred02
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_gfx02b/padflagred02.tga
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


textures/pad_gfx02b/padflagblue02
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_gfx02b/padflagblue02.tga
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



textures/pad_gfx02b/old_padflagred
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_gfx02b/old_padflagred.tga
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



textures/pad_gfx02b/old_padflaggreen
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_gfx02b/old_padflaggreen.tga
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


textures/pad_gfx02b/old_padflagblack
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_gfx02b/old_padflagblack.tga
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


textures/pad_metal/targetlight
{
	qer_editorimage textures/pad_metal/targetlight.tga
	q3map_surfacelight 200
              q3map_flareShader flareShader
//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/targetlight.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_metal/target_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}



textures/pad_flowerfx/grass4_move
{
	qer_editorimage textures/pad_flowerfx/pad_leaf4.tga
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_flowerfx/pad_leaf4.tga
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


textures/pad_flowerfx/grass2_move
{
	qer_editorimage textures/pad_flowerfx/pad_leaf2.tga
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_flowerfx/pad_leaf2.tga
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


textures/pad_flowerfx/leaf04_move
{
	qer_editorimage textures/pad_flowerfx/blatt.tga
	deformVertexes wave 194 sin 0 2 0 .2
	deformVertexes wave 30 sin 0 1 0 .3
	deformVertexes wave 194 sin 0 1 0 .1
	surfaceparm nomarks
	surfaceparm alphashadow
	cull none

	{
		map textures/pad_flowerfx/blatt.tga
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


textures/pad_flowerfx/sprite_leaf2
{
	qer_editorimage textures/pad_flowerfx/pad_leaf2.tga
	cull disable
	deformvertexes autosprite

{
	map textures/pad_flowerfx/pad_leaf2.tga
	blendFunc blend
      alphaFunc GE128
	depthWrite
	rgbGen identity
	}
}


//=======================================================
//SKY BOXES by MightyPete
//=======================================================

textures/pad_petesky/utopiaatoll
{
        qer_editorimage textures/pad_petesky/utopiaatoll.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange01.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 200

        skyparms env/utopiaatoll512 - -
//       {
//		map textures/pad_petesky/utopiaatoll.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/urban-terror
{
        qer_editorimage textures/pad_petesky/urban-terror.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange2.tga
	q3map_sun	1.000000 0.718471 0.616587 200 60 225
	q3map_surfacelight 200

        skyparms env/urban-terror512 - -
//       {
//		map textures/pad_petesky/urban-terror.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/poltergeist
{
        qer_editorimage textures/pad_petesky/poltergeist.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/blue01.tga
	q3map_sun	0.434981 0.749839 1.000000 260 340 50
	q3map_surfacelight 400

        skyparms env/pf-poltergeist512 - -
//       {
//		map textures/pad_petesky/poltergeist.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/pf-tornado-alley
{
        qer_editorimage textures/pad_petesky/pf-tornado-alley.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/grey.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 200

        skyparms env/pf-tornado-alley512 - -
//       {
//		map textures/pad_petesky/pf-tornado-alley.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/morning_alley
{
             qer_editorimage textures/pad_petesky/morning_alley.tga


	surfaceparm noimpact
	surfaceparm nolightmap
              q3map_lightimage textures/pad_petesky/orange2.tga
	q3map_sun	1.000000 0.862213 0.247457 330 340 50
	q3map_surfacelight 400

             skyparms env/morning_alley512 - -
}



textures/pad_petesky/sep
{
        qer_editorimage textures/pad_petesky/sep.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange.tga
	q3map_sun	0.266383 0.274632 0.358662 94 48 75
	q3map_surfacelight 280

        skyparms env/sep512 - -
//       {
//		map textures/pad_petesky/sep.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/morning-madness
{
        qer_editorimage textures/pad_petesky/morning-madness.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange2.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 200

        skyparms env/pc-morning-madness512 - -
//       {
//		map textures/pad_petesky/morning-madness.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}


textures/pad_petesky/sist
{
        qer_editorimage textures/pad_petesky/sist.tga


	surfaceparm noimpact
	surfaceparm nolightmap
        q3map_lightimage textures/pad_petesky/orange01.tga
	q3map_sun	0.266383 0.274632 0.358662 100 50 55
	q3map_surfacelight 200

        skyparms env/sist512 - -
//       {
//		map textures/pad_petesky/sist.tga
//		blendfunc GL_ONE GL_ONE
//		tcMod scroll 0.05 0.06
//		tcMod scale 3 2
//	}
}
