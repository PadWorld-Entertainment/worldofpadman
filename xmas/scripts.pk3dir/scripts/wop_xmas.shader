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

models/hats/elf
{
	cull none
	{
		map models/hats/elf
		rgbGen lightingDiffuse
	}
}

models/hats/tophat
{
	cull none
	{
		map models/hats/tophat
		rgbGen lightingDiffuse
	}
}

models/hats/santa
{
	cull none
	{
		map models/hats/santa
		rgbGen lightingDiffuse
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


// =================
// XMAS SPRAY LOGOS
// =================

xmaslogos/01_stars
{
	nopicmip
	{
		map xmaslogos/01_stars
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/02_meteor
{
	nopicmip
	{
		map xmaslogos/02_meteor
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/03_star
{
	nopicmip
	{
		map xmaslogos/03_star
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/04_candle
{
	nopicmip
	{
		map xmaslogos/04_candle
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/05_bell
{
	nopicmip
	{
		map xmaslogos/05_bell
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/06_tree
{
	nopicmip
	{
		map xmaslogos/06_tree
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/07_holly
{
	nopicmip
	{
		map xmaslogos/07_holly
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/08_snowflake
{
	nopicmip
	{
		map xmaslogos/08_snowflake
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/09_icecrystal
{
	nopicmip
	{
		map xmaslogos/09_icecrystal
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/10_gift
{
	nopicmip
	{
		map xmaslogos/10_gift
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/11_snowman
{
	nopicmip
	{
		map xmaslogos/11_snowman
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/12_candycane
{
	nopicmip
	{
		map xmaslogos/12_candycane
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/13_advent
{
	nopicmip
	{
		map xmaslogos/13_advent
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/14_hat
{
	nopicmip
	{
		map xmaslogos/14_hat
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/15_ornaments
{
	nopicmip
	{
		map xmaslogos/15_ornaments
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/16_merryxmas
{
	nopicmip
	{
		map xmaslogos/16_merryxmas
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/17_beard
{
	nopicmip
	{
		map xmaslogos/17_beard
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/18_wings
{
	nopicmip
	{
		map xmaslogos/18_wings
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/19_gingy
{
	nopicmip
	{
		map xmaslogos/19_gingy
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/20_reindeer
{
	nopicmip
	{
		map xmaslogos/20_reindeer
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/21_boot
{
	nopicmip
	{
		map xmaslogos/21_boot
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/22_santa
{
	nopicmip
	{
		map xmaslogos/22_santa
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/23_sleighteam
{
	nopicmip
	{
		map xmaslogos/23_sleighteam
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/24_angel
{
	nopicmip
	{
		map xmaslogos/24_angel
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/25_drum
{
	nopicmip
	{
		map xmaslogos/25_drum
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/26_skate
{
	nopicmip
	{
		map xmaslogos/26_skate
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/27_nutcracker
{
	nopicmip
	{
		map xmaslogos/27_nutcracker
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/28_branch
{
	nopicmip
	{
		map xmaslogos/28_branch
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/29_wishlist
{
	nopicmip
	{
		map xmaslogos/29_wishlist
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/30_xmassucks
{
	nopicmip
	{
		map xmaslogos/30_xmassucks
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/31_stocking
{
	nopicmip
	{
		map xmaslogos/31_stocking
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/32_mine
{
	nopicmip
	{
		map xmaslogos/32_mine
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/33_ribbonbow
{
	nopicmip
	{
		map xmaslogos/33_ribbonbow
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/34_light
{
	nopicmip
	{
		map xmaslogos/34_light
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/35_newyear
{
	nopicmip
	{
		map xmaslogos/35_newyear
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/36_cheers
{
	nopicmip
	{
		map xmaslogos/36_cheers
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/37_firework
{
	nopicmip
	{
		map xmaslogos/37_firework
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

xmaslogos/38_rocket
{
	nopicmip
	{
		map xmaslogos/38_rocket
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}
