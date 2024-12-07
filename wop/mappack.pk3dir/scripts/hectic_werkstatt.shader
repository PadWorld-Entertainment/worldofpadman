textures/hectic_werkstatt/skip
{
	qer_nocarve
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
}

textures/hectic_werkstatt/plane
{
    qer_editorimage textures/pad_jail/jail2_bed_decke1_blau.jpg
	//surfaceparm trans
	cull none
    nopicmip
	{
		map textures/pad_jail/jail2_bed_decke1_blau.jpg
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

textures/hectic_werkstatt/nebel
{
	qer_editorimage textures/hectic_werkstatt/abgas.jpg
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
		map textures/hectic_werkstatt/abgas.jpg
		blendfunc gl_dst_color gl_zero
		tcmod scale -.05 -.05
		tcmod scroll .01 -.01
		rgbgen identity
	}

	{
		map textures/hectic_werkstatt/abgas.jpg
		blendfunc gl_dst_color gl_zero
		tcmod scale .05 .05
		tcmod scroll .01 -.01
		rgbgen identity
	}
}

textures/hectic_werkstatt/pad_jail_metal10_rust_nonsolid
{
	qer_editorimage textures/pad_jail/pad_jail_metal10_rust.jpg
	surfaceparm nonsolid

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_metal10_rust.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/hectic_werkstatt/carp001f_nonsolid
{
	qer_editorimage textures/wop12_floorin/carp001f.jpg
	surfaceparm nonsolid

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_floorin/carp001f.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/hectic_werkstatt/carp006_nonsolid
{
	qer_editorimage textures/wop12_floorin/carp006.jpg
	surfaceparm nonsolid

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_floorin/carp006.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/wop12_maps/lackgrey2
{
	qer_editorimage textures/wop12_maps/lackgrey2.tga
	
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_maps/lackgrey2.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbgen identity
	}
	{
		map textures/wop12_gfx02/tinpad5.tga
		tcgen environment
		blendfunc add
		rgbgen identity
	}
}

textures/wop12_maps/buche003
{
	qer_editorimage textures/wop12_maps/buche003.tga
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_maps/buche003.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 


textures/wop12_maps/buche004
{
	qer_editorimage textures/wop12_maps/buche004.tga
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_maps/buche004.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/wop12_jail/pad_jail_metalfloor1_zink
{
	qer_editorimage textures/wop12_jail/pad_jail_metalfloor1_zink.tga
	surfaceparm metalsteps		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_jail/pad_jail_metalfloor1_zink.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/wop12_floorin/carp001f
{
	qer_editorimage textures/wop12_floorin/carp001f.tga
	surfaceparm carpetsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_floorin/carp001f.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/wop12_backyard/Holz00z
{
	qer_editorimage textures/wop12_backyard/Holz00z.tga
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_backyard/Holz00z.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
} 

textures/wop12_wood/oldwood001
{
	qer_editorimage textures/wop12_wood/oldwood001.tga
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_wood/oldwood001.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/hectic_werkstatt/metalred
{
	qer_editorimage textures/wop12_backyard/Metall2.jpg
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_backyard/Metall2.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/hectic_werkstatt/metalblue
{
	qer_editorimage textures/wop12_backyard/Metall1.jpg
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/wop12_backyard/Metall1.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/wop12_gfx02b/chrome_darkred
{
	qer_editorimage textures/wop12_gfx02b/chrome_darkred.tga	
        {
		map textures/wop12_gfx02b/chrome_darkred.tga
                tcgen environment
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
        {
		map $lightmap
		rgbGen identity
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
	}
           
}

textures/wop12_gfx02/busch
{
        qer_editorimage textures/wop12_gfx02/busch.tga
    	surfaceparm trans
	surfaceparm alphashadow
	surfaceparm playerclip
   	surfaceparm nonsolid
	cull none
        nopicmip
	{
		map textures/wop12_gfx02/busch.tga
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


// This file was automatically created by Particle Studio.
// For more information about Particle Studio, please
// visit http://www.quake3stuff.com/freebrief

// If this file was created with work-around code switched on,
// you will have to remove all occurences of the string
// "//-UNCMT4RELEASE-//" after you compile the map.
// 
// If you forget to do this,  your particles will look flat and 
// will have misaligned textures.

textures/hectic_werkstatt/dampfz270x70_1
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 37.046421 3.208756 14.739963 sawtooth 0 1 0.100681 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.100681 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.100681 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.100681 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_2
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 37.509281 -4.182764 13.249842 sawtooth 0 1 0.929777 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.929777 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.929777 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.929777 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_3
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 37.864700 2.949964 12.552374 sawtooth 0 1 0.669607 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.669607 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.669607 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.669607 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_4
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 34.841732 1.189255 19.612221 sawtooth 0 1 0.833735 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.833735 1.000000

rgbGen wave sawtooth 1.000000 0.000000 0.833735 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.833735 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_5
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 38.912052 -1.951804 9.057748 sawtooth 0 1 0.559648 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.559648 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.559648 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.559648 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_6
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 36.036140 -7.836371 15.491544 sawtooth 0 1 0.768181 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.768181 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.768181 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.768181 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_7
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 38.313419 2.535122 11.209592 sawtooth 0 1 0.391095 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.391095 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.391095 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.391095 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_8
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 38.910984 -4.691234 7.995488 sawtooth 0 1 0.117161 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.117161 1.000000

rgbGen wave sawtooth 1.000000 0.000000 0.117161 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.117161 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_9
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 36.399521 2.157340 16.444475 sawtooth 0 1 0.691946 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.691946 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.691946 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.691946 1.000000
blendfunc blend
}
}

textures/hectic_werkstatt/dampfz270x70_10
{
qer_editorimage textures/hectic_werkstatt/dampf.tga
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 38.208855 5.305614 10.579887 sawtooth 0 1 0.781518 1.000000
{
clampmap textures/hectic_werkstatt/dampf.tga
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.800000 -0.800000 0.781518 1.000000
rgbGen wave sawtooth 1.000000 0.000000 0.781518 1.000000
tcMod stretch sawtooth 1.000000 1.000000 0.781518 1.000000
blendfunc blend
}
}

