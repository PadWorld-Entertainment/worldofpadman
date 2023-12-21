textures/pad_garden/neon01
{
	qer_editorimage textures/pad_enteflare/neon01.tga
	surfaceparm nomarks
    q3map_flareShader flareShader
	q3map_surfacelight 300
        {
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_enteflare/neon01_blend.tga
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
	map textures/pad_metal/yb2.tga
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_effect/waterfall
{
	qer_editorimage textures/pad_effect/pooltest.tga
	q3map_globaltexture
	surfaceparm trans
	//surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm water
	surfaceparm nolightmap
	tessSize 64
	cull disable
	deformVertexes wave 64 triangle 1 3 .1 .8
	
	{
		map textures/pad_effect/pool3d_3e.tga
		blendfunc GL_DST_COLOR GL_SRC_COLOR		
		tcMod scale 1.0 2.0
		tcMod turb .1 .05 .25 .08
		tcMod scroll .8 -.125
	}
          
          { 
  map textures/pad_effect/pool3d_6b.tga 
  blendfunc GL_DST_COLOR GL_ONE 
  tcMod scale .5 .5 
  tcMod turb .1 .075 .5 .05 
  tcMod scroll 1. .1 
 } 
  

} 

textures/pad_effect/vines
{
        qer_editorimage textures/pad_effect/vines.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_effect/vines.tga
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

textures/pad_effect/vines2
{
        qer_editorimage textures/pad_effect/vines2.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_effect/vines2.tga
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

textures/pad_effect/walkwaylightf
{
    q3map_surfacelight 1225
    surfaceparm nolightmap
    {
        map textures/pad_effect/walkwaylightf.tga
    }
}


textures/pad_effect/galleryinfo
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/galleryinfo.tga
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
		map textures/pad_effect/galleryinfo.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}       


textures/pad_effect/schild
{
	//deformVertexes wave 100 sin 3 2 .1 3
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_effect/schild.tga
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
		map textures/pad_effect/schild.tga
		//blendFunc GL_ONE GL_ONE
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
		map textures/pad_effect/skinwin.tga
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
		map textures/pad_effect/skinwin.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_effect/busch
{
        qer_editorimage textures/pad_effect/busch.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/pad_effect/busch.tga
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


textures/pad_effect/elight1
{
    q3map_surfacelight 2000
    surfaceparm nolightmap
    {
        map textures/pad_effect/elight1.tga
    }
}


textures/pad_effect/light_basic
{
    q3map_surfacelight 175
    surfaceparm nolightmap
    {
        map textures/pad_effect/light_basic.tga
    }
}



textures/pad_effect/light_floor
{
    q3map_surfacelight 11375
    light 1
    surfaceparm nolightmap
    {
        map textures/pad_effect/light_floor.tga
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
		map textures/pad_effect/blacky.tga
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
		map textures/pad_effect/blacky.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   


textures/pad_effect/blatt
{
    cull disable
    surfaceparm alphashadow
//  deformVertexes autoSprite
    surfaceparm trans	
        {
        map textures/pad_effect/blatt.tga
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
		map textures/pad_effect/colabox.tga
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
		map textures/pad_effect/colabox.tga
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
		map textures/pad_effect/blacky01.tga
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
		map textures/pad_effect/blacky01.tga
		//blendFunc GL_ONE GL_ONE
                blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}   