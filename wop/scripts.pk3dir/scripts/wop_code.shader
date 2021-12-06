// =================
// SPRAYMODE
// =================

//file made by #@ ... don't modify it
//spraypuff
models/weaponsfx/spraypuff { {  map models/weaponsfx/spraypuff  blendFunc blend  rgbGen vertex  alphaGen vertex } }

powerupeffect/puff { {  map powerupeffect/puff.tga  blendFunc blend  rgbGen vertex  alphaGen vertex } }
powerupeffect/revival { {  map powerupeffect/heart_noalpha.tga  blendFunc add  rgbGen vertex  alphaGen vertex } }

//spraymark ... I will use the spraypuff =)
models/weaponsfx/spraymark { polygonoffset {  map models/weaponsfx/spraypuff  blendFunc blend  rgbGen vertex  alphaGen vertex } }

// =================
// WoPascii
// =================

gfx/2d/WoPascii
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/WoPascii.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}


// =================
// WHiTE
// =================

white
{
	{
		map *white
		blendfunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

// =================
// FULL SCREEN FX
// =================

gfx/screen/ice
{
	nomipmaps
	{
		map gfx/screen/ice
		blendFunc add
		rgbGen Vertex
	}
}

gfx/screen/puppet
{
	{
		map gfx/screen/puppet
		blendfunc blend
		alphaGen wave inversesawtooth 0.5 0.4 0 1
	}
}

gfx/screen/damage
{
	nomipmaps
	{
		map gfx/screen/damage
		blendFunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

gfx/screen/wet
{
	nomipmaps
	{
		map gfx/screen/wet
		blendfunc add
		rgbGen Vertex
	}
}

// =================
// TELEPORT
// 1st-Person-Effekt-Shader
// =================

teleEffectBlueFP
{
	cull none
	{
		map models/weaponsfx/teleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/teleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}

teleEffectRedFP
{
	cull none
	{
		map models/weaponsfx/redteleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/redteleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}

teleEffectGreenFP
{
	cull none
	{
		map models/weaponsfx/greenteleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/greenteleflash.tga
		blendFunc add
		rgbGen vertex
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}

// =================
// WEAPON MARKS
// =================

gfx/damage/bullet_mrk
{
	polygonOffset
	{
		map gfx/damage/bullet_mrk.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}

gfx/damage/burn_med_mrk
{
	polygonOffset
	{
		map gfx/damage/burn_med_mrk.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}

gfx/damage/hole_lg_mrk
{
	polygonOffset
	{
		map gfx/damage/hole_lg_mrk.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}

gfx/damage/plasma_mrk
{
	polygonOffset
	{
		map gfx/damage/plasma_mrk.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

gfx/damage/snow_mrk
{
	polygonOffset
	{
		map gfx/damage/burn_med_mrk.tga
		blendFunc add
		rgbGen vertex
		alphaGen vertex
	}
}

gfx/damage/foam_mrk
{
	polygonoffset
	{
		map gfx/damage/foam_mrk
		blendfunc add
	}
}

gfx/damage/gum_mrk
{
	cull none
	polygonOffset
	{
		map gfx/damage/gum_mrk
		blendfunc blend
		rgbGen vertex
		alphaGen vertex
	}
}

/*
#######################
	kma special zoom effects
#######################
*/

gfx/kmazoomAura
{
	sort nearest
	{
		map $whiteimage 
		blendfunc GL_ONE_MINUS_DST_COLOR GL_ONE_MINUS_SRC_COLOR
	}
}

gfx/kmaBlueScreen
{
	sort 14
	{
		map $whiteimage 
		blendfunc blend
		alphaGen const 0.5
		rgbGen const ( 0.0 0.0 0.50 )
	}
}


// =================
// MiX
// =================

// some shader which are used directly by the code
// ... some are just replacements for vq3-shaders

disconnected
{
	nopicmip
	{
		map gfx/2d/net.tga
	}
}

// markShadow is the very cheap blurry blob underneat the player
markShadow
{
	polygonOffset
	{
		map marks/shadow.tga
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}	
}

// projectionShadow is used for cheap squashed model shadows
projectionShadow
{
	polygonOffset
	deformVertexes projectionShadow
	{
		map			*white
		blendFunc GL_ONE GL_ZERO
		rgbGen wave square 0 0 0 0				// just solid black
	}	
}

// wake is the mark on water surfaces for paddling players
wake
{
	{
		clampmap marks/splash.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
                tcmod rotate 150
                tcMod stretch sin .9 0.1 0 0.7
		rgbGen wave sin .7 .3 .25 .5
	}	
        {
		clampmap marks/splash.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
                tcmod rotate -130
                tcMod stretch sin .9 0.05 0 0.9
		rgbGen wave sin .7 .3 .25 .4
	}	
}

waterBubble
{
	sort	underwater
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map sprites/bubble.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}

// normally we use "gfx/2d/WoPascii" ...
gfx/2d/bigchars
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/bigchars.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}


// brauch ich "console"(aus gfx.shader) noch? ... ich find es nirgends, aber es wird geladen O_o
// neuste gfx.shader -> pak5.pk3


bbox {
	nopicmip

	{
		map gfx/misc/bbox
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

bbox_nocull {
	nopicmip
	cull none

	{
		map gfx/misc/bbox
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

