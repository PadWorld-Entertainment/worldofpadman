
textures/darkplace/nebel_schwarz
{
		qer_editorimage textures/darkplace/abgas.jpg
		qer_nocarve
	
	surfaceparm	nonsolid
	surfaceparm	trans
	surfaceparm	nolightmap
	surfaceparm	fog
	fogparms ( .01 .01 .01 ) 5000
}

//-----------------aus hectic_wekstatt.shader-----------------
textures/darkplace/nebel
{
	qer_editorimage textures/darkplace/abgas.jpg
	qer_trans 0.4
	surfaceparm	trans
	surfaceparm	nonsolid
	surfaceparm	fog
	//surfaceparm nodrop
	surfaceparm	nolightmap
	q3map_globaltexture
	//q3map_surfacelight 50
	//fogparms ( .3 .3 .3 ) 1200
	fogparms ( .15 .15 .15 ) 1200

	
	
	{
		map textures/darkplace/abgas.jpg
		blendfunc gl_dst_color gl_zero
		tcmod scale -.05 -.05
		tcmod scroll .01 -.01
		rgbgen identity
	}

	{
		map textures/darkplace/abgas.jpg
		blendfunc gl_dst_color gl_zero
		tcmod scale .05 .05
		tcmod scroll .01 -.01
		rgbgen identity
	}
}