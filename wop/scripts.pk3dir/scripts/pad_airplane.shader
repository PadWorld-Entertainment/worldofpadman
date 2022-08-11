//-----------------------------------------------------------------
//Skybox
//-----------------------------------------------------------------

textures/pad_o4texs/ueberdenwolken
{
	qer_editorimage textures/pad_o4texs/ueberdenwolken
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_lightimage textures/pad_o4texs/125
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
		map textures/pad_o4texs/danke_erklaert_de
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
		map textures/pad_o4texs/danke_noctuagraphics
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
		map textures/pad_o4texs/danke_maxmustermann
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
		map textures/pad_o4texs/gurtschlossa
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
		map textures/pad_o4texs/gurtschlossc
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
		map textures/pad_o4texs/sechseck
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
		map textures/pad_o4texs/serviette
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
	qer_editorimage textures/pad_o4texs/neonwhite
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 3000
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/neonwhite
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/neonwhite
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/warmwhite
{
	qer_editorimage textures/pad_o4texs/warmwhite
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 2000
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/warmwhite
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/warmwhite
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/redlight
{
	qer_editorimage textures/pad_o4texs/redlight
	surfaceparm nomarks
	surfaceparm nolightmap
	q3map_surfacelight 200
	light 1
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/redlight
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_o4texs/redlight
		blendfunc GL_ONE GL_ONE
	}
}

//-----------------------------------------------------------------
//Blink...Blink...Blink
//-----------------------------------------------------------------
textures/pad_o4texs/blinkgruen1
{
	qer_editorimage textures/pad_o4texs/blinkgruen
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 1.99
	}
}

textures/pad_o4texs/blinkgruen2
{
	qer_editorimage textures/pad_o4texs/blinkgruen
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 2.51
	}
}

textures/pad_o4texs/blinkgruen3
{
	qer_editorimage textures/pad_o4texs/blinkgruen
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.07
	}
}

textures/pad_o4texs/blinkgruen4
{
	qer_editorimage textures/pad_o4texs/blinkgruen
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.53
	}
}

textures/pad_o4texs/blinkgruen5
{
	qer_editorimage textures/pad_o4texs/blinkgruen
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkgruen
		blendFunc GL_ONE GL_ONE
		rgbGen wave Square 0 1 0 3.97
	}
}

textures/pad_o4texs/blinkblauring
{
	qer_editorimage textures/pad_o4texs/blinkblauring
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/blinkblauring
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
		Map textures/pad_o4texs/rubikrot
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
		Map textures/pad_o4texs/rubikgruen
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
		Map textures/pad_o4texs/rubikblau
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
		Map textures/pad_o4texs/rubikgelb
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
		Map textures/pad_o4texs/rubikorange
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
		Map textures/pad_o4texs/rubikweiss
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0 1 0 1.27
	}
}


//-----------------------------------------------------------------
//Kabel nonsolid
//-----------------------------------------------------------------

textures/pad_o4texs/p032nonsolid
{
	qer_editorimage textures/pad_o4texs/p032nonsolid
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
	qer_editorimage textures/pad_o4texs/cabin-dispa
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/cabin-dispa
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/cabin-dispb
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit001
{
	qer_editorimage textures/pad_o4texs/airplanecockpit001
	q3map_surfacelight 5
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit001
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit001b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit002
{
	qer_editorimage textures/pad_o4texs/airplanecockpit002
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit002
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit002b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit010
{
	qer_editorimage textures/pad_o4texs/airplanecockpit010
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit010
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit010b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit011
{
	qer_editorimage textures/pad_o4texs/airplanecockpit011
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit011
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit011b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit014
{
	qer_editorimage textures/pad_o4texs/airplanecockpit014
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit014
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit014b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit015
{
	qer_editorimage textures/pad_o4texs/airplanecockpit015
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit015
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit015b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit016
{
	qer_editorimage textures/pad_o4texs/airplanecockpit016
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit016
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit016b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit020
{
	qer_editorimage textures/pad_o4texs/airplanecockpit020
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit020
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit020b
		blendFunc add
	}
}

textures/pad_o4texs/airplanecockpit022
{
	qer_editorimage textures/pad_o4texs/airplanecockpit022
	q3map_surfacelight 10
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit022
		blendFunc filter
		rgbGen identity
	}
	{
		map textures/pad_o4texs/airplanecockpit022b
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
		map textures/pad_o4texs/001
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
	{
		map textures/pad_o4texs/badspiegel
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
		map textures/pad_o4texs/glass-nonglare-001
		blendFunc GL_ONE GL_ONE
	}
}

textures/pad_o4texs/glass-nonglare-002
{
	surfaceparm trans
	qer_trans 	0.5
	{
		map textures/pad_o4texs/glass-nonglare-002
		blendFunc GL_ONE GL_ONE
	}
}

//-----------------------------------------------------------------
//Texturen die auf Flaechen draufgepappt werden
//zB Grafitti
//-----------------------------------------------------------------

textures/pad_o4texs/AcmeTires
{
	qer_editorimage textures/pad_o4texs/acmetires
	surfaceparm trans
	qer_trans .6
	cull none
	surfaceparm nomipmaps
	{
		map textures/pad_o4texs/acmetires
		blendFunc GL_DST_COLOR GL_ONE
	}
}

textures/pad_o4texs/lueftergitter
{
	surfaceparm trans
	{
		map textures/pad_o4texs/lueftergitter
		blendfunc filter
	}
}

textures/pad_o4texs/lueftergitter2
{
	surfaceparm trans
	{
		map textures/pad_o4texs/lueftergitter2
		blendfunc filter
	}
}

textures/pad_o4texs/lueftergitter3
{
	surfaceparm trans
	{
		map textures/pad_o4texs/lueftergitter3
		blendfunc filter
	}
}

//-----------------------------------------------------------------
//Texturen die bestimmte Schrittgeraeusche haben
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
		map textures/pad_o4texs/teppich
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
		map textures/pad_o4texs/p008leder
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
		map textures/pad_o4texs/p002leder
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
		map textures/pad_o4texs/p888alugeb
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
		map textures/pad_o4texs/p001alugeb
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
	qer_editorimage textures/pad_o4texs/metalstep063
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_o4texs/063
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
		map textures/pad_o4texs/p002holz
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
		map textures/pad_o4texs/p008holz
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
		map textures/pad_o4texs/p009holz
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
		map textures/pad_o4texs/p039holz
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}