// =================
// BALLOONY
// =================

waterExplosion
{
	{
		animmap 5 models/weaponsfx/waterboom1 models/weaponsfx/waterboom2 models/weaponsfx/waterboom3
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 5
	}
	{
		animmap 5 models/weaponsfx/waterboom2 models/weaponsfx/waterboom3 models/weaponsfx/waterboom1
		blendFunc add
		rgbGen wave sawtooth 0 1 0 5
	}
}

models/weaponsfx/drop
{
	cull none
	{
		map models/weaponsfx/drop
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}


waterSplash
{
	cull disable
	{
		animmap 5 models/weaponsfx/water_splash_1 models/weaponsfx/water_splash_2 models/weaponsfx/water_splash_3 gfx/colors/black
		rgbGen wave inversesawtooth 0 1 0 5
		blendfunc add
	}
	{
		animmap 5 models/weaponsfx/water_splash_2 models/weaponsfx/water_splash_3 gfx/colors/black gfx/colors/black
		rgbGen wave sawtooth 0 1 0 5
		blendfunc add
	}
}

models/weaponsfx/balloony_drop
{
	cull none
	{
		map models/weaponsfx/balloony_drop
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}

// =================
// BOOM BOOM BETTY
// =================

fireBall
{
	{
		animmap 10 models/weaponsfx/fireballs1 models/weaponsfx/fireballs2 models/weaponsfx/fireballs3
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 10
	}
	{
		animmap 3 models/weaponsfx/fireballs2 models/weaponsfx/fireballs3 models/weaponsfx/fireballs1
		blendFunc add
		rgbGen wave sawtooth 0 1 0 10
	}
}

fireTrail
{
	{
		map models/weaponsfx/firetrail
		blendFunc add
		rgbGen vertex
		tcMod scroll 1.6 0
	}
	{
		map models/weaponsfx/firetrail
		blendFunc add
		rgbGen vertex
		tcMod scale 2.1 1
		tcMod scroll -0.63 0
	}

}


fireExplosion
{
	cull disable
	{
		animmap 8 models/weaponsfx/bettyboom_1 models/weaponsfx/bettyboom_2 models/weaponsfx/bettyboom_3 models/weaponsfx/bettyboom_4 models/weaponsfx/bettyboom_5 models/weaponsfx/bettyboom_6 models/weaponsfx/bettyboom_7 models/weaponsfx/bettyboom_8
		rgbGen wave inversesawtooth 0 1 0 8
		blendfunc add
	}
	{
		animmap 8 models/weaponsfx/bettyboom_2 models/weaponsfx/bettyboom_3 models/weaponsfx/bettyboom_4 models/weaponsfx/bettyboom_5 models/weaponsfx/bettyboom_6 models/weaponsfx/bettyboom_7 models/weaponsfx/bettyboom_8 models/weaponsfx/black
		rgbGen wave sawtooth 0 1 0 8
		blendfunc add
	}
}


models/weaponsfx/firedrop
{
	cull none
	{
		map models/weaponsfx/firedrop
		blendFunc add
	}
}

// =================
// BOASTER
// =================

waterBeam
{
	cull none
	nopicmip
	nomipmaps
	{
		map models/weaponsfx/waterbeam
		blendFunc add
		rgbGen vertex
	}
}

boasterExplosion
{
	cull none
	{
		clampmap models/weaponsfx/boasterboom
		blendfunc add
        tcMod stretch sawtooth .2 1.2 0 2.0
        rgbGen wave inversesawtooth 0 1 0 2.0
	}
}

// =================
// PUMPER
// =================

pumperTrail
{
	cull none
	nopicmip
	nomipmaps
	{
		map models/weaponsfx/pumpertrail
		blendFunc add
		rgbGen vertex
	}
}

models/weaponsfx/flash
{
	cull none
	{
		animmap 9 models/weaponsfx/flash0 models/weaponsfx/flash1 models/weaponsfx/flash2
		blendFunc add
		tcMod turb 0 0.01 0 11
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}


// =================
// SPLASHER
// =================

waterTrail
{
	{
		map models/weaponsfx/watertrail
		blendFunc add
		rgbGen vertex
		tcMod scroll 1.6 0
	}
}

waterBall
{
	{
		map models/weaponsfx/waterball
		blendFunc add
	}
}

waterMark
{
	polygonOffset
	{
		map models/weaponsfx/waterball
		blendfunc add
		rgbGen vertex
	}
}


// =================
// NIPPER
// =================

nipperBall
{
	{
		map models/weaponsfx/nipperball
		blendFunc add
	}
}

nipperWave
{
	{
		map models/weaponsfx/nipperwave
		blendFunc add
        rgbGen wave inversesawtooth 0 1 0 1.5
	}
}

// =================
// IMPERIUS
// =================

models/weaponsfx/impsphere
{
	{
		map models/weaponsfx/impsphere
		blendFunc add
		rgbGen entity
		tcMod scroll 0.5 0.7
	}
}

models/weaponsfx/impring
{
	cull none
	{
		map models/weaponsfx/impring
		blendFunc add
		rgbGen entity
	}
}

models/weaponsfx/impbeam
{
	cull none
	{
		map models/weaponsfx/impbeam
		blendFunc add
		rgbGen entity
	}
}

imperiusCore
{
	cull none
	{
		map models/weaponsfx/impcore
		blendFunc add
		rgbGen entity
	}
}

imperiusRing
{
	cull none
	{
		map models/weaponsfx/impring
		blendFunc add
		rgbGen entity
	}
}


// =================
// BUBBLE G
// =================

models/weaponsfx/gum
{
	cull none
	{
		map models/weaponsfx/gum
		rgbGen entity
	}
}

// =================
// KMA97
// =================

kmaTrail
{
	sort nearest
	cull disable
	{
		map models/weaponsfx/kmatrail
		blendfunc add
		rgbGen Vertex
	}
}

kmaMark
{
	polygonOffset
	{
		map gfx/damage/kma_mrk
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
		rgbGen exactVertex
	}
}


models/weaponsfx/smallkmadrop
{
	cull none
	{
		map models/weaponsfx/smallkmadrop
		blendFunc add
		rgbGen wave inversesawtooth 0 1 0 1.2
	}
}


models/weapons2/kma97/kma97_flash
{
	sort additive
	cull disable
	{
		clampmap models/weapons2/kma97/f_kma97
		blendfunc GL_ONE GL_ONE
                tcmod rotate 360
		rgbGen entity
        }
        {
		clampmap models/weapons2/kma97/f_kma97
		blendfunc GL_ONE GL_ONE
                tcmod rotate -129
		//tcMod stretch sin .8 0.10 0 .7
		rgbGen entity
        }

}

// =================
// MISC
// =================

models/weaponsfx/star
{
	cull none
	{
		map models/weaponsfx/star
		blendFunc blend
	}
}

teleEffect
{
	cull none
	{
		map models/weaponsfx/teleflash
		blendFunc add
		rgbGen entity
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/teleflash
		blendFunc add
		rgbGen entity
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}

// =================
// TELEPORT
// =================

teleEffectRed
{
	cull none
	{
		map models/weaponsfx/redteleflash
		blendFunc add
		rgbGen entity
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/redteleflash
		blendFunc add
		rgbGen entity
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}


teleEffectGreen
{
	cull none
	{
		map models/weaponsfx/greenteleflash
		blendFunc add
		rgbGen entity
		tcMod scale 3 3
		tcMod scroll 1.2 1.9
	}
	{
		map models/weaponsfx/greenteleflash
		blendFunc add
		rgbGen entity
		tcMod scale 4 4
		tcMod scroll -0.9 1.3
	}
}


// =================
// KILLERDUCKS
// =================

newDuckExplosion
{
	cull disable
	{
		animmap 10 models/weaponsfx/duckexp_1 models/weaponsfx/duckexp_2 models/weaponsfx/duckexp_3 models/weaponsfx/duckexp_4 models/weaponsfx/duckexp_5 models/weaponsfx/duckexp_6 models/weaponsfx/duckexp_7 models/weaponsfx/duckexp_8
		rgbGen wave inversesawtooth 0 1 0 10
		blendfunc add
	}
	cull disable
	{
		animmap 10 models/weaponsfx/duckexp_2 models/weaponsfx/duckexp_3 models/weaponsfx/duckexp_4 models/weaponsfx/duckexp_5 models/weaponsfx/duckexp_6 models/weaponsfx/duckexp_7 models/weaponsfx/duckexp_8 gfx/colors/black
		rgbGen wave sawtooth 0 1 0 10
		blendfunc add
	}
}


// BAMBAM
bambamMissileRed {
	{
		map "models/weaponsfx/bambamball_red"
		blendFunc add
	}
}

bambamMissileBlue {
	{
		map "models/weaponsfx/bambamball_blue"
		blendFunc add
	}
}


