// =================
// XMAS LOADiNGSTATiON
// =================

models/mapobjects/pad_healthstation/HS_ring

{
	cull none
	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
		rgbGen identity
	}
	{
		map models/mapobjects/pad_healthstation/HS_ring
		rgbGen lightingDiffuse
		alphaFunc Ge128
	}
}

models/mapobjects/pad_healthstation/HS_base
{
	cull none
	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8
		rgbGen identity
	}
	{
		map models/mapobjects/pad_healthstation/HS_base
		rgbGen lightingDiffuse
		alphaFunc Ge128
	}
}

models/mapobjects/pad_healthstation/star
{
	{
		map models/mapobjects/pad_healthstation/star
		rgbGen identity
	}
}


// =================
// XMAS PADSHiELD and PADSHARD
// =================

models/powerups/armor/holly
{
	cull none
	{
		map models/powerups/armor/holly
		alphaFunc ge128
		rgbGen identity
	}
}

models/powerups/armor/ribbon
{
	cull none
	{
		map textures/pad_gfx02/padmapred
		blendfunc GL_ONE GL_ZERO
		tcGen environment
		rgbGen identity
	}
}


// =================
// XMAS AMMOBOTTLES
// =================

models/powerups/ammo/ammo_nipper
{
	cull disable
	{
		map models/powerups/ammo/ammo_nipper
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_balloony
{
	cull disable
	{
		map models/powerups/ammo/ammo_balloony
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_bubbleg
{
	cull disable
	{
		map models/powerups/ammo/ammo_bubbleg
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_boaster
{
	cull disable
	{
		map models/powerups/ammo/ammo_boaster
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_betty
{
	cull disable
	{
		map models/powerups/ammo/ammo_betty
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_imperius
{
	cull disable
	{
		map models/powerups/ammo/ammo_imperius
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_pumper
{
	cull disable
	{
		map models/powerups/ammo/ammo_pumper
		rgbGen lightingDiffuse
	}
}

models/powerups/ammo/ammo_splasher
{
	cull disable
	{
		map models/powerups/ammo/ammo_splasher
		rgbGen lightingDiffuse
	}
}


// =================
// XMAS HATS
// =================

models/hats/elvehat
{
	cull none
	{
		map models/hats/elvehat
		rgbGen lightingDiffuse
	}
}

models/hats/reindeer
{
	{
		map models/hats/reindeer
		rgbGen lightingDiffuse
	}
}

models/hats/halo
{
	cull none
	{
		map models/hats/halo
		blendfunc GL_ONE GL_ONE
		blendfunc add
	}
}

models/hats/irish
{
	{
		map models/hats/irish
		rgbGen lightingDiffuse
	}
}

models/hats/tophat
{
	{
		map models/hats/tophat
		rgbGen lightingDiffuse
	}
}

models/hats/nikolaus
{
	cull none
	{
		map models/hats/nikolaus
		rgbGen lightingDiffuse
	}
}

models/hats/hking1
{
	cull disable
	{
		map models/hats/hking1
		alphaFunc ge128
		rgbGen lightingDiffuse
	}
	{
		map textures/pad_gfx02/tinpad
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
		tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
	}
}

models/hats/lightstringhat
{
	cull none
	{
	map models/hats/lightstringhat
	rgbGen lightingDiffuse
	}
}

models/hats/light01
{
	{
	animMap 1 models/hats/light01 models/hats/light02
	rgbGen identity
	}
}


// =================
// XMAS BiG BALLOON
// =================

models/special/candle
{
	nomipmaps
	{
		map models/special/candle
		rgbGen entity
	}
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map models/special/candle_color
		rgbGen lightingDiffuse
		alphaFunc Ge128
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
		rgbGen entity
	}
}


// =================
// XMAS SYC TELEPORTER
// =================

models/mapobjects/pad_teleporter/xmas_base
{
	{
		map textures/pad_gfx02/padmapyel3
		tcGen environment
		blendfunc GL_ONE GL_ZERO
		tcmod scale .8 .8a
		rgbGen identity

	}
	{
		map models/mapobjects/pad_teleporter/xmas_base
		rgbGen lightingdiffuse
		alphaFunc Ge128
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
}

models/mapobjects/pad_teleporter/xmas_sphere
{
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc GL_ONE GL_ONE
		tcGen environment
	}
}

models/mapobjects/pad_teleporter/xmas_snow
{
	cull none
	{
		map models/mapobjects/pad_teleporter/xmas_snow
		blendfunc add
		tcMod scroll -.25 0
	}
}


// =================
// XMAS SYC CARTRiDGES
// =================

models/weapons2/spraypistol/xmascart_r
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
		map textures/pad_gfx02/padmapyel2
		tcGen environment
		tcMod turb 0 .5 0 .3
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/spraypistol/xmascart_r
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/weapons2/spraypistol/xmascart_b
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
		map textures/pad_gfx02/padmapyel2
		tcGen environment
		tcMod turb 0 .5 0 .3
		tcMod scroll .1 .09
		rgbGen lightingdiffuse
	}
	{
		map models/weapons2/spraypistol/xmascart_b
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/weapons2/spraypistol/xmascart_n
{
	cull disable
	surfaceparm nolightmap
	surfaceparm nonsolid
	nopicmip
	sort 9
	{
		map textures/pad_gfx02/padmapyel2
		tcGen environment
		tcMod turb 0 .5 0 .3
		tcMod scroll .1 .09
		rgbGen lightingdiffuse

	}
	{
		map models/weapons2/spraypistol/xmascart_n
		alphaFunc ge128
		rgbGen lightingdiffuse

  	}
}
