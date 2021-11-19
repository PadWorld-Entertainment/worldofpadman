models/special/ballon

{
	nomipmaps
	{
		map models/special/ballon.tga
		rgbGen entity        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}

	{
		map models/special/balloon_color.tga
		rgbGen lightingDiffuse
		alphaFunc Ge128
		
	}

	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE 
		tcGen environment
                rgbGen entity //lightingDiffuse //vertex //identity //makes effect darker
	}
}


models/special/box
{	
	nopicmip
	nomipmaps
	qer_editorimage models/special/box.tga
	{
		map models/special/box.tga
		rgbGen lightingdiffuse
		
	}

}