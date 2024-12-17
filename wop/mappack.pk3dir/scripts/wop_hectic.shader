// Hectic's maps shader

// ==============
// 7 Colors
// ==============

textures/hectic/randlicht
{
    qer_editorimage textures/pad_gfx02/light_white
    q3map_surfacelight 750
    surfaceparm nolightmap
    tessSize 16
    {
        map textures/pad_gfx02/light_white
    }
}

// ==============
// Hurry up!
// ==============

textures/hectic/darksky
{
    qer_editorimage textures/colors/hex_000000ff
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    {
        map textures/colors/hex_000000ff
    }
}

textures/hectic/portal
{
    qer_editorimage textures/pad_gfx02/padmapyel
    q3map_lightimage textures/pad_gfx02/padmapyel
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm nonsolid
    portal
    {
        map textures/pad_gfx02/padmapyel
        blendfunc blend
        rgbGen identityLighting
        depthWrite
        alphaGen portal 700
    }
    {
        map textures/pad_gfx02/padmapyel
        blendfunc add
        tcgen environment
    }
}

textures/hectic/randlicht2000
{
    qer_editorimage textures/colors/hex_ffffffff
    q3map_surfacelight 2000
    surfaceparm nolightmap
    tessSize 16
    {
        map textures/colors/hex_ffffffff
    }
}

// ==============
// Workshop
// ==============

textures/hectic/plane
{
    qer_editorimage textures/pad_jail/jail2_bed_decke1_blau
    cull none
    nopicmip
    {
        map textures/pad_jail/jail2_bed_decke1_blau
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

textures/hectic/nebel
{
    qer_editorimage textures/hectic/abgas
    qer_trans 0.4
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm fog
    surfaceparm nolightmap
    q3map_globaltexture
    fogparms ( .15 .15 .15 ) 1200
    {
        map textures/hectic/abgas
        blendfunc gl_dst_color gl_zero
        tcmod scale -.05 -.05
        tcmod scroll .01 -.01
        rgbgen identity
    }
    {
        map textures/hectic/abgas
        blendfunc gl_dst_color gl_zero
        tcmod scale .05 .05
        tcmod scroll .01 -.01
        rgbgen identity
    }
}

textures/hectic/pad_jail_metal10_rust_nonsolid
{
    qer_editorimage textures/pad_jail/pad_jail_metal10_rust
    surfaceparm nonsolid
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/pad_jail/pad_jail_metal10_rust
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
}

textures/hectic/carp001f
{
    qer_editorimage textures/pad_floorin/carp001f
    surfaceparm carpetsteps
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/pad_floorin/carp001f
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
}

textures/hectic/carp001f_nonsolid
{
	qer_editorimage textures/pad_floorin/carp001f
	surfaceparm nonsolid

	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_floorin/carp001f
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/hectic/chrome_darkred
{
    qer_editorimage textures/pad_gfx02b/chrome_darkred
    {
        map textures/pad_gfx02b/chrome_darkred
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

textures/hectic/plas03
{
	qer_editorimage textures/pad_gfx02/plas03
	{
		map textures/pad_gfx02/plas03
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad
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

textures/hectic/plas06
{
	qer_editorimage textures/pad_gfx02/plas06
	{
		map textures/pad_gfx02/plas06
		rgbgen identity
	}
	{
		map textures/pad_gfx02/tinpad
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

// This file was automatically created by Particle Studio.
// For more information about Particle Studio, please
// visit http://www.quake3stuff.com/freebrief

// If this file was created with work-around code switched on,
// you will have to remove all occurences of the string
// "//-UNCMT4RELEASE-//" after you compile the map.
//
// If you forget to do this, your particles will look flat and
// will have misaligned textures.

textures/hectic/dampfz270x70_1
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 37.046421 3.208756 14.739963 sawtooth 0 1 0.100681 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.100681 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.100681 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.100681 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_2
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 37.509281 -4.182764 13.249842 sawtooth 0 1 0.929777 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.929777 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.929777 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.929777 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_3
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 37.864700 2.949964 12.552374 sawtooth 0 1 0.669607 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.669607 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.669607 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.669607 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_4
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 34.841732 1.189255 19.612221 sawtooth 0 1 0.833735 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.833735 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.833735 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.833735 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_5
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 38.912052 -1.951804 9.057748 sawtooth 0 1 0.559648 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.559648 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.559648 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.559648 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_6
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 36.036140 -7.836371 15.491544 sawtooth 0 1 0.768181 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.768181 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.768181 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.768181 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_7
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 38.313419 2.535122 11.209592 sawtooth 0 1 0.391095 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.391095 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.391095 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.391095 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_8
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 38.910984 -4.691234 7.995488 sawtooth 0 1 0.117161 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.117161 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.117161 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.117161 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_9
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 36.399521 2.157340 16.444475 sawtooth 0 1 0.691946 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.691946 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.691946 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.691946 1.000000
        blendfunc blend
    }
}

textures/hectic/dampfz270x70_10
{
    qer_editorimage textures/hectic/dampf
    surfaceparm noimpact
    surfaceparm nolightmap
    cull none
    surfaceparm trans
    surfaceparm nonsolid
    surfaceparm nodlight
    deformvertexes autosprite
    deformvertexes move 38.208855 5.305614 10.579887 sawtooth 0 1 0.781518 1.000000
    {
        clampmap textures/hectic/dampf
        tcMod rotate 0.000000
        AlphaGen wave sawtooth 0.800000 -0.800000 0.781518 1.000000
        rgbGen wave sawtooth 1.000000 0.000000 0.781518 1.000000
        tcMod stretch sawtooth 1.000000 1.000000 0.781518 1.000000
        blendfunc blend
    }
}
