models/special/ballon

{
	nomipmaps
	{
		map models/special/ballon
		rgbGen entity        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}

	{
		map models/special/balloon_color
		rgbGen lightingDiffuse
		alphaFunc Ge128

	}

	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen entity //lightingDiffuse //vertex //identity //makes effect darker
	}
}


models/special/box
{
	nopicmip
	nomipmaps
	qer_editorimage models/special/box
	{
		map models/special/box
		rgbGen lightingdiffuse

	}

}