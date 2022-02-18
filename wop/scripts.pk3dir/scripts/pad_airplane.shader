//-----------------------------------------------------------------
//Skybox
//-----------------------------------------------------------------

textures/pad_o4texs/ueberdenwolken
{
qer_editorimage textures/pad_o4texs/ueberdenwolken.jpg
surfaceparm noimpact
surfaceparm nolightmap
q3map_lightimage textures/pad_o4texs/124.tga
q3map_sun	0.925 0.949 1 120 60 20
q3map_surfacelight 200
skyparms env/ueberdenwolken - -
}


//-----------------------------------------------------------------
// einfache Texturen mit (ALPHA)Transparenz
//-----------------------------------------------------------------

textures/pad_o4texs/danke_erklaert_de
{
surfaceparm trans
surfaceparm alphashadow
cull none
nopicmip
	{
	map textures/pad_o4texs/danke_erklaert_de.tga
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

textures/pad_o4texs/danke_noctuagraphics
{
surfaceparm trans
surfaceparm alphashadow
cull none
nopicmip
	{
	map textures/pad_o4texs/danke_noctuagraphics.tga
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

textures/pad_o4texs/danke_maxmustermann
{
surfaceparm trans
surfaceparm alphashadow	
cull none
nopicmip
	{
	map textures/pad_o4texs/danke_maxmustermann.tga
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

textures/pad_o4texs/gurtschlossA
{
surfaceparm trans
surfaceparm alphashadow	
nopicmip
	{
	map textures/pad_o4texs/gurtschlossA.tga
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

textures/pad_o4texs/gurtschlossC
{
surfaceparm trans
surfaceparm alphashadow	
cull none
nopicmip
	{
	map textures/pad_o4texs/gurtschlossC.tga
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

textures/pad_o4texs/sechseck
{
surfaceparm trans
surfaceparm alphashadow	
cull none
nopicmip
	{
	map textures/pad_o4texs/sechseck.tga
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


textures/pad_o4texs/serviette
{
surfaceparm trans
surfaceparm alphashadow	
cull none
nopicmip
	{
	map textures/pad_o4texs/serviette.tga
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

//-----------------------------------------------------------------
//Texturen die komplett Leuchten wie zB ein Bildschirm
//-----------------------------------------------------------------

textures/pad_o4texs/neonwhite
{
	qer_editorimage textures/pad_o4texs/neonwhite.jpg
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 3000
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/neonwhite.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/neonwhite.jpg
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/warmwhite
{
	qer_editorimage textures/pad_o4texs/warmwhite.jpg
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 2000
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/warmwhite.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/warmwhite.jpg
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/redlight
{
	qer_editorimage textures/pad_o4texs/redlight.tga
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 200
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/redlight.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/redlight.tga
		blendfunc GL_ONE GL_ONE
	}
}

//-----------------------------------------------------------------
//Blink...Blink...Blink
//-----------------------------------------------------------------
textures/pad_o4texs/blinkgruen1
{
	qer_editorimage textures/pad_o4texs/blinkgruen.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 1.99
	}
}

textures/pad_o4texs/blinkgruen2
{
	qer_editorimage textures/pad_o4texs/blinkgruen.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 2.51
	}
}

textures/pad_o4texs/blinkgruen3
{
	qer_editorimage textures/pad_o4texs/blinkgruen.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.07
	}
}

textures/pad_o4texs/blinkgruen4
{
	qer_editorimage textures/pad_o4texs/blinkgruen.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.53
	}
}

textures/pad_o4texs/blinkgruen5
{
	qer_editorimage textures/pad_o4texs/blinkgruen.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.97
	}
}

textures/pad_o4texs/blinkblauring
{
	qer_editorimage textures/pad_o4texs/blinkblauring.jpg
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkblauring.jpg
		blendFunc GL_ONE GL_ONE
		rgbGen wave Sin 0 1 0 0.25
	}
}

textures/pad_o4texs/rubikrot
{
	q3map_surfacelight	30
	surfaceparm	trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikrot.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.01
	}	
}


textures/pad_o4texs/rubikgruen
{
	q3map_surfacelight	30
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikgruen.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.03
	}	
}


textures/pad_o4texs/rubikblau
{
	q3map_surfacelight	30
	surfaceparm	trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikblau.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.07
	}	
}


textures/pad_o4texs/rubikgelb
{
	q3map_surfacelight	30
	surfaceparm	trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikgelb.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.09
	}	
}


textures/pad_o4texs/rubikorange
{
	q3map_surfacelight	30
	surfaceparm	trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikorange.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.13
	}	
}


textures/pad_o4texs/rubikweiss
{
	q3map_surfacelight	30
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	{
		Map textures/pad_o4texs/rubikweiss.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.27
	}	
}


//-----------------------------------------------------------------
//Kabel nonsolid
//-----------------------------------------------------------------

textures/pad_o4texs/p032nonsolid
{
qer_editorimage textures/pad_o4texs/p032nonsolid.tga
surfaceparm nomarks
surfaceparm nonsolid
	{
		map $lightmap
		rgbGen identity
	}
}


//-----------------------------------------------------------------
//Die shader sind dafuer da um Partiel beleuchtet zu sein zB die
//Bildschirmchen im Cockpit
//-----------------------------------------------------------------

textures/pad_o4texs/Cabin-dispA
{
qer_editorimage textures/pad_o4texs/Cabin-dispA.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/Cabin-dispA.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/Cabin-dispB.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit001
{
qer_editorimage textures/pad_o4texs/airplanecockpit001.tga
q3map_surfacelight 5
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit001.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit001b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit002
{
qer_editorimage textures/pad_o4texs/airplanecockpit002.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit002.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit002b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit010
{
qer_editorimage textures/pad_o4texs/airplanecockpit010.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit010.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit010b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit011
{
qer_editorimage textures/pad_o4texs/airplanecockpit011.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit011.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit011b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit014
{
qer_editorimage textures/pad_o4texs/airplanecockpit014.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit014.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit014b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit015
{
qer_editorimage textures/pad_o4texs/airplanecockpit015.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit015.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit015b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit016
{
qer_editorimage textures/pad_o4texs/airplanecockpit016.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit016.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit016b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit020
{
qer_editorimage textures/pad_o4texs/airplanecockpit020.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit020.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit020b.tga
	blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit022
{
qer_editorimage textures/pad_o4texs/airplanecockpit022.tga
q3map_surfacelight 10
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit022.tga
	blendFunc filter
	rgbGen identity
	}
	{
	map textures/pad_o4texs/airplanecockpit022b.tga
	blendFunc add
	}
}

//-----------------------------------------------------------------
//Fake Spiegel
//-----------------------------------------------------------------

textures/pad_o4texs/patschefinger
{
	{
	rgbGen identity
	map $lightmap
	}
	{
	map textures/pad_o4texs/001.tga
	blendFunc GL_DST_COLOR GL_SRC_ALPHA
	rgbGen identity
	alphaGen lightingSpecular
	}
	{
	map textures/pad_o4texs/badspiegel.tga
	tcgen environment
	blendFunc GL_ONE GL_ONE
	rgbGen identity
	}
}

//-----------------------------------------------------------------
//Transparente Texturen, effekt wie Sandgestrahltes Glas
//-----------------------------------------------------------------

textures/pad_o4texs/glass-nonglare-001
{
surfaceparm trans
qer_trans 	0.5
	{
	map textures/pad_o4texs/glass-nonglare-001.tga
	blendFunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/glass-nonglare-002
{
surfaceparm trans
qer_trans 	0.5
	{
	map textures/pad_o4texs/glass-nonglare-002.tga
	blendFunc GL_ONE GL_ONE
	}
}

//-----------------------------------------------------------------
//Texturen die auf Flächen draufgepappt werden
//zB Grafitti
//-----------------------------------------------------------------

textures/pad_o4texs/AcmeTires
{
qer_editorimage textures/pad_o4texs/AcmeTires.tga
surfaceparm trans
qer_trans .6
cull none
surfaceparm nomipmaps
	{
	map textures/pad_o4texs/AcmeTires.tga
	blendFunc GL_DST_COLOR GL_ONE
	}
}

textures/pad_o4texs/lueftergitter
{
surfaceparm trans
	{
	map textures/pad_o4texs/lueftergitter.jpg
	blendfunc filter
	}
}

textures/pad_o4texs/lueftergitter2
{
surfaceparm trans
	{
	map textures/pad_o4texs/lueftergitter2.jpg
	blendfunc filter
	}
}

textures/pad_o4texs/lueftergitter3
{
surfaceparm trans
	{
	map textures/pad_o4texs/lueftergitter3.jpg
	blendfunc filter
	}
}

//-----------------------------------------------------------------
//Texturen die bestimmte Schrittgeräusche haben
//zB Teppich
//-----------------------------------------------------------------


//carpetclip Teppich

textures/pad_o4texs/teppich
{
surfaceparm carpetsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/teppich.tga
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p008Leder
{
surfaceparm carpetsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p008Leder.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p002Leder
{
surfaceparm carpetsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p002Leder.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}


//metalclip Metall

textures/pad_o4texs/p888AluGeb
{
surfaceparm metalsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p888AluGeb.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p001AluGeb
{
surfaceparm metalsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p001AluGeb.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/zinc01
{
surfaceparm metalsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/zinc01
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/metalstep063
{
qer_editorimage textures/pad_o4texs/metalstep063.tga
surfaceparm metalsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/063.tga
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}


//woodclip Holz

textures/pad_o4texs/p002Holz
{
	surfaceparm woodsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p002Holz.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p008Holz
{
surfaceparm woodsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p008Holz.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p009Holz
{
surfaceparm woodsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p009Holz.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}

textures/pad_o4texs/p039Holz
{
surfaceparm woodsteps
	{
	map $lightmap
	rgbGen identity
	}
	{
	map textures/pad_o4texs/p039Holz.jpg
	blendFunc GL_DST_COLOR GL_ZERO
	rgbGen identity
	}
}