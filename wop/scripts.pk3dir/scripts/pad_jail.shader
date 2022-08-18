// ********************************************************
// *
// *   MopAn's Jail:  Pocket Lamp
// *
// ********************************************************
textures/pad_jail/pad_jail2_tlampe
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail2_tlampe
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  Playcards and fotos
// *
// ********************************************************
textures/pad_jail/pad_jail2_herz_koenig
{
        qer_editorimage textures/pad_jail/pad_jail2_herz_koenig
        surfaceparm trans
        surfaceparm alphashadow
        surfaceparm playerclip
        surfaceparm nonsolid
        cull none
        nopicmip
	{
		map textures/pad_jail/pad_jail2_herz_koenig
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

textures/pad_jail/pad_jail2_pik_bube
{
        qer_editorimage textures/pad_jail/pad_jail2_pik_bube
        surfaceparm trans
        surfaceparm alphashadow
        surfaceparm playerclip
        surfaceparm nonsolid
        cull none
        nopicmip
	{
		map textures/pad_jail/pad_jail2_pik_bube
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

textures/pad_jail/pad_jail2_pik_dame
{
        qer_editorimage textures/pad_jail/pad_jail2_pik_dame
        surfaceparm trans
        surfaceparm alphashadow
        surfaceparm playerclip
        surfaceparm nonsolid
        cull none
        nopicmip
	{
		map textures/pad_jail/pad_jail2_pik_dame
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

// ********************************************************
// *
// *   MopAn's Jail:  Notrespassing
// *
// ********************************************************

textures/pad_jail/pad_jail2_notrespas
{
        qer_editorimage textures/pad_jail/pad_jail2_notrespas
        surfaceparm trans
        surfaceparm alphashadow
        surfaceparm playerclip
        //surfaceparm nonsolid
        cull none
        nopicmip
	{
		map textures/pad_jail/pad_jail2_notrespas
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

// ********************************************************
// *
// *   MopAn's Jail:  flame small (little above original big flames)
// *
// ********************************************************
textures/pad_jail/jail2_particle_flame_small_1
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.104041 -0.987807 21.640051 sawtooth 0 1 0.907599 0.249029
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 2.119053
AlphaGen wave sawtooth 0.543016 -0.384780 0.907599 0.249029
rgbGen wave sawtooth 0.000000 0.154805 0.907599 0.249029
tcMod stretch sawtooth 0.512143 -0.412143 0.907599 0.249029
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_small_2
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.176399 -0.192010 3.166808 sawtooth 0 1 0.793765 0.333036
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 1.164556
AlphaGen wave sawtooth 0.573009 -0.311682 0.793765 0.333036
rgbGen wave sawtooth 0.000000 0.107294 0.793765 0.333036
tcMod stretch sawtooth 0.346298 -0.246298 0.793765 0.333036
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_small_3
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.059288 -0.439767 15.001798 sawtooth 0 1 1.000803 0.241049
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 2.132603
AlphaGen wave sawtooth 0.420917 -0.257897 1.000803 0.241049
rgbGen wave sawtooth 0.000000 0.159481 1.000803 0.241049
tcMod stretch sawtooth 0.429169 -0.329169 1.000803 0.241049
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_small_4
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.507019 0.002786 38.652100 sawtooth 0 1 0.967501 0.234538
{
clampmap textures/pad_jail/jail2_particle_flame_red2
tcMod rotate 2.487594
AlphaGen wave sawtooth 0.586914 -0.411240 0.967501 0.234538
rgbGen wave sawtooth 0.000000 0.230454 0.967501 0.234538
tcMod stretch sawtooth 0.293198 -0.193198 0.967501 0.234538
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_small_5
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.700055 -0.292394 10.281649 sawtooth 0 1 0.838847 0.282531
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 1.964568
AlphaGen wave sawtooth 0.425947 -0.200681 0.838847 0.282531
rgbGen wave sawtooth 0.000000 0.134816 0.838847 0.282531
tcMod stretch sawtooth 0.312117 -0.212117 0.838847 0.282531
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_small_6
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.039768 -0.691628 22.183887 sawtooth 0 1 1.011411 0.374374
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 1.950774
AlphaGen wave sawtooth 0.547581 -0.313172 1.011411 0.374374
rgbGen wave sawtooth 0.000000 0.138069 1.011411 0.374374
tcMod stretch sawtooth 0.488891 -0.388891 1.011411 0.374374
blendfunc add
}
}



// ********************************************************
// *
// *   MopAn's Jail:  big flames
// *
// ********************************************************
textures/pad_jail/jail2_particle_flame_big_1
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.855649 -0.594220 16.804434 sawtooth 0 1 0.614063 0.222318
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.719169
AlphaGen wave sawtooth 0.829713 -0.515009 0.614063 0.222318
rgbGen wave sawtooth 0.000000 0.519384 0.614063 0.222318
tcMod stretch sawtooth 0.771105 -0.496415 0.614063 0.222318
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_2
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.979301 -1.922151 49.622700 sawtooth 0 1 0.990722 0.224923
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.676534
AlphaGen wave sawtooth 0.852253 -0.505115 0.990722 0.224923
rgbGen wave sawtooth 0.000000 0.429305 0.990722 0.224923
tcMod stretch sawtooth 0.671038 -0.401764 0.990722 0.224923
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_3
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.466568 -0.073290 26.564932 sawtooth 0 1 0.700912 0.218787
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.616169
AlphaGen wave sawtooth 0.900192 -0.632621 0.700912 0.218787
rgbGen wave sawtooth 0.000000 0.410036 0.700912 0.218787
tcMod stretch sawtooth 0.764765 -0.476936 0.700912 0.218787
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_4
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.204901 -1.038951 42.484089 sawtooth 0 1 0.861190 0.222210
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.661550
AlphaGen wave sawtooth 1.066814 -0.828016 0.861190 0.222210
rgbGen wave sawtooth 0.000000 0.424880 0.861190 0.222210
tcMod stretch sawtooth 0.723492 -0.359928 0.861190 0.222210
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_5
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.081736 -0.500316 16.270197 sawtooth 0 1 0.713218 0.219080
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.440290
AlphaGen wave sawtooth 1.031272 -0.846748 0.713218 0.219080
rgbGen wave sawtooth 0.000000 0.445755 0.713218 0.219080
tcMod stretch sawtooth 0.777696 -0.439084 0.713218 0.219080
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_6
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -3.139118 -1.656225 53.107990 sawtooth 0 1 1.047908 0.209343
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.344890
AlphaGen wave sawtooth 0.966634 -0.710977 1.047908 0.209343
rgbGen wave sawtooth 0.000000 0.359961 1.047908 0.209343
tcMod stretch sawtooth 0.744564 -0.448903 1.047908 0.209343
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_7
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -2.186446 -0.896679 39.035198 sawtooth 0 1 1.025275 0.206471
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.674123
AlphaGen wave sawtooth 0.876690 -0.575194 1.025275 0.206471
rgbGen wave sawtooth 0.000000 0.383851 1.025275 0.206471
tcMod stretch sawtooth 0.766911 -0.398679 1.025275 0.206471
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_8
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.856490 -0.412961 49.444454 sawtooth 0 1 0.727921 0.225967
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.731407
AlphaGen wave sawtooth 0.931550 -0.762505 0.727921 0.225967
rgbGen wave sawtooth 0.000000 0.508916 0.727921 0.225967
tcMod stretch sawtooth 0.668647 -0.403572 0.727921 0.225967
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_9
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.378977 -1.107010 17.809118 sawtooth 0 1 0.673611 0.226183
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.674184
AlphaGen wave sawtooth 0.790664 -0.509860 0.673611 0.226183
rgbGen wave sawtooth 0.000000 0.355335 0.673611 0.226183
tcMod stretch sawtooth 0.747384 -0.463715 0.673611 0.226183
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_10
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.001536 0.003535 -0.106998 sawtooth 0 1 1.044832 0.219511
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.155736
AlphaGen wave sawtooth 0.789922 -0.559303 1.044832 0.219511
rgbGen wave sawtooth 0.000000 0.477323 1.044832 0.219511
tcMod stretch sawtooth 0.671162 -0.433003 1.044832 0.219511
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_11
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.491899 -0.285495 9.005352 sawtooth 0 1 0.936851 0.214816
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.287087
AlphaGen wave sawtooth 0.923740 -0.775533 0.936851 0.214816
rgbGen wave sawtooth 0.000000 0.364350 0.936851 0.214816
tcMod stretch sawtooth 0.718522 -0.408720 0.936851 0.214816
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_12
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.570885 -0.648670 13.179045 sawtooth 0 1 0.877999 0.223266
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.173528
AlphaGen wave sawtooth 0.783706 -0.457661 0.877999 0.223266
rgbGen wave sawtooth 0.000000 0.519976 0.877999 0.223266
tcMod stretch sawtooth 0.662337 -0.393609 0.877999 0.223266
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_13
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.830791 -1.903828 35.594635 sawtooth 0 1 1.006433 0.220798
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.392498
AlphaGen wave sawtooth 0.843034 -0.598571 1.006433 0.220798
rgbGen wave sawtooth 0.000000 0.365387 1.006433 0.220798
tcMod stretch sawtooth 0.739390 -0.427595 1.006433 0.220798
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_14
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.077821 0.002449 1.788899 sawtooth 0 1 0.761577 0.214307
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.703635
AlphaGen wave sawtooth 0.860970 -0.579250 0.761577 0.214307
rgbGen wave sawtooth 0.000000 0.410421 0.761577 0.214307
tcMod stretch sawtooth 0.681772 -0.321455 0.761577 0.214307
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_15
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.793900 -1.750969 29.973316 sawtooth 0 1 0.849580 0.219915
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 4.232582
AlphaGen wave sawtooth 0.907983 -0.740013 0.849580 0.219915
rgbGen wave sawtooth 0.000000 0.513353 0.849580 0.219915
tcMod stretch sawtooth 0.711593 -0.340065 0.849580 0.219915
blendfunc add
}
}

textures/pad_jail/jail2_particle_flame_big_16
{
qer_editorimage textures/pad_jail/jail2_particle_flame_red
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.976570 -0.946683 35.304443 sawtooth 0 1 0.895102 0.219551
{
clampmap textures/pad_jail/jail2_particle_flame_red
tcMod rotate 3.026551
AlphaGen wave sawtooth 0.883153 -0.608697 0.895102 0.219551
rgbGen wave sawtooth 0.000000 0.435879 0.895102 0.219551
tcMod stretch sawtooth 0.711267 -0.470686 0.895102 0.219551
blendfunc add
}
}


// ********************************************************
// *
// *   MopAn's Jail:  for flames, base fireball
// *
// ********************************************************
textures/pad_jail/jail2_flameball1
{
	qer_editorimage textures/pad_jail/jail2_flameball
	q3map_globaltexture
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans
	cull none
	deformvertexes autosprite

	{
		map textures/pad_jail/jail2_flameball
		blendFunc GL_ONE GL_ONE
		rgbGen wave sin .6 .2 0 .6
	}

}


// ********************************************************
// *
// *   MopAn's Jail:  this is light emitting surface for the key underneath the bed
// *
// ********************************************************
textures/pad_jail/pad_jail_keys_light
{
	qer_editorimage textures/pad_jail/pad_jail_keys
	q3map_lightimage textures/pad_jail/pad_jail_keys
	q3map_surfacelight 400
	light 1
	//surfaceparm nolightmap
	surfaceparm trans

	{
		map $lightmap
		rgbGen identity
	}

	{
		map textures/pad_jail/pad_jail_keys
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  green fog in gas-tank
// *
// ********************************************************
textures/pad_jail/pad_jail_imp_fog
{
		qer_editorimage textures/pad_gfx02b/padfog_green
		surfaceparm	trans
		surfaceparm	nonsolid
		surfaceparm	fog
		surfaceparm	nolightmap

		//fogparms ( .733 .737 .620 ) 450
		fogparms ( .01 .61 .28 ) 320
}


// ********************************************************
// *
// *   MopAn's Jail:  green liquid in gas-tank
// *
// ********************************************************
textures/pad_jail/pad_jail_liquid_gas
{
	qer_trans 0.6
	qer_editorimage textures/pad_jail/pad_jail_liquid_gas
	q3map_lightimage textures/pad_jail/pad_jail_liquid_gas
        	q3map_surfacelight 180
	q3map_globaltexture
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm water
	tessSize 64
	cull disable
	deformVertexes wave 100 sin 4 2 .1 0.1

	{
		map textures/pad_jail/pad_jail_liquid_gas
		tcMod turb .3 .2 1 .05
		tcMod scroll .01 .01
	}
	{
		map textures/pad_jail/pad_jail_liquid_gas
		blendfunc GL_ONE GL_ONE
		tcMod turb .2 .1 1 .05
		tcMod scale .5 .5
		tcMod scroll .01 .01
	}

}


// ********************************************************
// *
// *   MopAn's Jail:  electric zap
// *
// ********************************************************
textures/pad_jail/pad_jail_zap2
{
        	q3map_surfacelight 300
        	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none

	{
		map textures/pad_jail/pad_jail_zap2_blend
		blendFunc GL_ONE GL_ONE
                	rgbgen wave triangle .8 2 0 7
                	tcMod scroll 0 1
	}
        	{
		map textures/pad_jail/pad_jail_zap2_blend
		blendFunc GL_ONE GL_ONE
                	rgbgen wave triangle 1 1.4 0 5
                	tcMod scale  -1 1
                	tcMod scroll 0 1
	}
        	{
		map textures/pad_jail/pad_jail_zap2
		blendFunc GL_ONE GL_ONE
                	rgbgen wave triangle 1 1.4 0 6.3
                	tcMod scale  -1 1
                	tcMod scroll 2 1
	}
         	{
		map textures/pad_jail/pad_jail_zap2
		blendFunc GL_ONE GL_ONE
                	rgbgen wave triangle 1 1.4 0 7.7
                	tcMod scroll -1.3 1
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  forgotton what this is, have to check that ;-)
// *
// ********************************************************
textures/pad_jail/pad_jail_metaltools
	{
	q3map_surfacelight 100
	q3map_lightimage textures/pad_jail/pad_jail_metaltools_blend
	qer_editorimage textures/pad_jail/pad_jail_metaltools

	{
		map textures/pad_jail/pad_jail_metaltools
		rgbGen identity
	}

	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}


	{
		map textures/pad_jail/pad_jail_metaltools_blend
		blendfunc gl_one gl_one
		rgbgen wave sin 0 1 0 .5
		tcmod scale 1 .05
		tcmod scroll 0 1
	}

}


// ********************************************************
// *
// *   MopAn's Jail:  Beam
// *
// ********************************************************
textures/pad_jail/pad_jail_beam
{
        qer_editorimage textures/pad_jail/pad_jail_beam
        surfaceparm trans
        surfaceparm nomarks
        surfaceparm nonsolid
        surfaceparm nolightmap
       cull none
       {
	map textures/pad_jail/pad_jail_beam
                blendFunc add
        }
}

// ********************************************************
// *
// *   MopAn's Jail:  Dunkelheit
// *
// ********************************************************
textures/pad_jail/pad_jail_dunkelheit
{
	surfaceparm nolightmap
	surfaceparm noimpact
	surfaceparm nomarks
	{
		map textures/pad_jail/pad_jail_dunkelheit
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  Pad-Flagge im Sprayroom und Spraytafeln -> by MopAn
// *
// ********************************************************
textures/pad_jail/jail_padflagred
{
        tessSize 64
        deformVertexes wave 194 sin 0 3 0 .4
        deformVertexes normal .5 .1
        surfaceparm nomarks
        cull none
        {
		map textures/pad_jail/jail_padflagred
		rgbGen identity
	}
        	{
		map textures/pad_jail/jail_padflagred
                	blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
        	{
		map $lightmap
               		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
        	{
        		map textures/pad_jail/jail2_shadow
                	tcGen environment
                                blendFunc GL_DST_COLOR GL_ZERO
               		rgbGen identity
	}
}


textures/pad_jail/pad_jail2_flagge_blue
{
	cull disable
	//polygonOffset
	nopicmip
	nomipmaps

	{
		map textures/pad_jail/pad_jail2_flagge_blue
		rgbgen identity
		alphaFunc GE128
		blendFunc GL_ONE GL_ZERO
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}


}


textures/pad_jail/pad_jail2_flagge_red
{
	cull disable
	//polygonOffset
	nopicmip
	nomipmaps

	{
		map textures/pad_jail/pad_jail2_flagge_red
		rgbgen identity
		alphaFunc GE128
		blendFunc GL_ONE GL_ZERO
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}

}


// ********************************************************
// *
// *   MopAn's Jail:  Eis -> by MopAn
// *
// ********************************************************

textures/pad_jail/ice_blue
{
	qer_editorimage textures/pad_jail/pad_jail_sprayroom2_iceblue
	q3map_globaltexture
	qer_trans 0.6
	surfaceparm slick
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	cull twosided
	{
		map textures/pad_jail/pad_jail_sprayroom2_iceblue
		blendfunc add
		tcMod scale .75 .75
		rgbgen identity
	}
}

textures/pad_jail/ice_red
{
	qer_editorimage textures/pad_jail/pad_jail_sprayroom2_icered
	q3map_globaltexture
	qer_trans 0.6
	surfaceparm slick
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	cull twosided
	{
		map textures/pad_jail/pad_jail_sprayroom2_icered
		blendfunc add
		tcMod scale .75 .75
		rgbgen identity
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  icicles -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_icicles
{
	qer_editorimage textures/pad_jail/pad_jail_sprayroom2_icicles
	qer_nocarve
	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable

	{
		map textures/pad_jail/pad_jail_sprayroom2_icicles
		alphaFunc GE128
		blendFunc add
		rgbgen identity
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  snow-texture, get polygonoffset -> by MopAn
// *
// ********************************************************
textures/pad_jail/jail_snowrock10
{
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_snowrock10
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  sprayroom2 snowflakes -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_sprayroom2_snowflake1_1
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -375.257996 154.770050 -565.861633 sawtooth 0 1 1.352910 0.099800
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 3.051912
AlphaGen wave sawtooth 0.400000 0.300882 1.352910 0.099800
rgbGen wave sawtooth 0.077523 0.799829 1.352910 0.099800
tcMod stretch sawtooth 0.668944 0.331056 1.352910 0.099800
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_2
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.499016 -224.011414 -626.246338 sawtooth 0 1 0.475368 0.103483
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.352428
AlphaGen wave sawtooth 0.400000 0.436726 0.475368 0.103483
rgbGen wave sawtooth 0.184936 0.510697 0.475368 0.103483
tcMod stretch sawtooth 0.570357 0.429643 0.475368 0.103483
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_3
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -154.043091 14.982594 -726.850281 sawtooth 0 1 0.640901 0.097267
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 5.265236
AlphaGen wave sawtooth 0.400000 0.634291 0.640901 0.097267
rgbGen wave sawtooth 0.186071 0.744786 0.640901 0.097267
tcMod stretch sawtooth 0.355739 0.644261 0.640901 0.097267
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_4
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -263.780121 186.777710 -597.590820 sawtooth 0 1 1.344780 0.106198
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 5.488754
AlphaGen wave sawtooth 0.400000 0.437947 1.344780 0.106198
rgbGen wave sawtooth -0.043672 0.933915 1.344780 0.106198
tcMod stretch sawtooth 0.574606 0.425394 1.344780 0.106198
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_5
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -296.760834 161.784958 -741.389587 sawtooth 0 1 1.216932 0.082746
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 3.747124
AlphaGen wave sawtooth 0.400000 0.672256 1.216932 0.082746
rgbGen wave sawtooth 0.021784 0.607135 1.216932 0.082746
tcMod stretch sawtooth 0.596164 0.403836 1.216932 0.082746
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_6
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 209.174606 6.703498 -726.740662 sawtooth 0 1 0.927323 0.093157
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 3.294961
AlphaGen wave sawtooth 0.400000 0.217823 0.927323 0.093157
rgbGen wave sawtooth -0.184704 1.055867 0.927323 0.093157
tcMod stretch sawtooth 0.325941 0.674059 0.927323 0.093157
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_7
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 57.602821 -2.882029 -631.782776 sawtooth 0 1 1.243593 0.116368
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 4.607624
AlphaGen wave sawtooth 0.400000 0.251588 1.243593 0.116368
rgbGen wave sawtooth 0.019391 0.603143 1.243593 0.116368
tcMod stretch sawtooth 0.397024 0.602976 1.243593 0.116368
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_8
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 388.237518 -115.402596 -718.924927 sawtooth 0 1 1.484311 0.088551
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.229743
AlphaGen wave sawtooth 0.400000 0.972485 1.484311 0.088551
rgbGen wave sawtooth -0.083920 0.839186 1.484311 0.088551
tcMod stretch sawtooth 0.459941 0.540059 1.484311 0.088551
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_9
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 317.205292 277.092224 -813.609863 sawtooth 0 1 0.741429 0.072724
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 4.589313
AlphaGen wave sawtooth 0.400000 0.638954 0.741429 0.072724
rgbGen wave sawtooth 0.159752 0.692001 0.741429 0.072724
tcMod stretch sawtooth 0.515241 0.484759 0.741429 0.072724
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_10
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -55.973358 234.626526 -589.671814 sawtooth 0 1 1.535984 0.104206
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.901028
AlphaGen wave sawtooth 0.400000 0.910300 1.535984 0.104206
rgbGen wave sawtooth 0.181518 0.711435 1.535984 0.104206
tcMod stretch sawtooth 0.493780 0.506220 1.535984 0.104206
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_11
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 41.063725 239.369080 -756.462402 sawtooth 0 1 1.393268 0.087009
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.399060
AlphaGen wave sawtooth 0.400000 0.485409 1.393268 0.087009
rgbGen wave sawtooth -0.141856 0.761766 1.393268 0.087009
tcMod stretch sawtooth 0.417985 0.582015 1.393268 0.087009
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_12
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 412.659912 -61.512161 -561.618713 sawtooth 0 1 1.273586 0.103239
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 5.855831
AlphaGen wave sawtooth 0.400000 0.287332 1.273586 0.103239
rgbGen wave sawtooth -0.136229 0.779846 1.273586 0.103239
tcMod stretch sawtooth 0.526447 0.473553 1.273586 0.103239
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_13
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -307.889954 -55.496883 -906.551331 sawtooth 0 1 0.438966 0.073420
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 5.616687
AlphaGen wave sawtooth 0.400000 0.800165 0.438966 0.073420
rgbGen wave sawtooth -0.163353 1.141575 0.438966 0.073420
tcMod stretch sawtooth 0.324476 0.675524 0.438966 0.073420
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_14
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 31.069443 272.092072 -866.389038 sawtooth 0 1 0.946660 0.082528
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 5.325175
AlphaGen wave sawtooth 0.400000 0.896725 0.946660 0.082528
rgbGen wave sawtooth -0.112803 0.730345 0.946660 0.082528
tcMod stretch sawtooth 0.302100 0.697900 0.946660 0.082528
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_15
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 291.100616 244.395432 -452.378082 sawtooth 0 1 0.562896 0.120144
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.586077
AlphaGen wave sawtooth 0.400000 0.937840 0.562896 0.120144
rgbGen wave sawtooth -0.111582 1.108432 0.562896 0.120144
tcMod stretch sawtooth 0.500250 0.499750 0.562896 0.120144
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake1_16
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 263.455383 -164.752884 -520.489136 sawtooth 0 1 0.991302 0.109964
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 2.249519
AlphaGen wave sawtooth 0.400000 0.973827 0.991302 0.109964
rgbGen wave sawtooth -0.170873 1.087130 0.991302 0.109964
tcMod stretch sawtooth 0.540657 0.459343 0.991302 0.109964
blendfunc add
}
}



// ********************************************************
// *
// *   MopAn's Jail:  snowflakes2 sprayroom2 ;) -> by MopAn
// *
// ********************************************************

textures/pad_jail/pad_jail_sprayroom2_snowflake2_1
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -91.875648 49.249176 -669.370667 sawtooth 0 1 1.171700 0.107643
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.197262 1.028489 1.171700 0.107643
rgbGen wave sawtooth 0.192969 0.690829 1.171700 0.107643
tcMod stretch sawtooth -0.091537 1.091537 1.171700 0.107643
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_2
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -30.536669 32.385159 -693.094421 sawtooth 0 1 0.302045 0.098351
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.130696 0.497015 0.302045 0.098351
rgbGen wave sawtooth 0.133372 0.754369 0.302045 0.098351
tcMod stretch sawtooth 0.155309 0.844691 0.302045 0.098351
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_3
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -179.239807 24.907839 -726.113037 sawtooth 0 1 0.645775 0.093454
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.253813 0.545900 0.645775 0.093454
rgbGen wave sawtooth 0.072237 0.595648 0.645775 0.093454
tcMod stretch sawtooth 0.062703 0.937297 0.645775 0.093454
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_4
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 124.700798 -77.800995 -640.541443 sawtooth 0 1 0.887909 0.097926
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.162264 0.998056 0.887909 0.097926
rgbGen wave sawtooth 0.028803 0.905680 0.887909 0.097926
tcMod stretch sawtooth 0.167858 0.832142 0.887909 0.097926
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_5
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -19.279312 -5.407333 -738.776245 sawtooth 0 1 1.119239 0.093866
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.185043 0.718848 1.119239 0.093866
rgbGen wave sawtooth 0.157286 0.452199 1.119239 0.093866
tcMod stretch sawtooth 0.085592 0.914408 1.119239 0.093866
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_6
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -119.079674 -82.322777 -845.236877 sawtooth 0 1 0.714777 0.084093
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.222019 1.068261 0.714777 0.084093
rgbGen wave sawtooth -0.047224 0.684738 0.714777 0.084093
tcMod stretch sawtooth -0.061019 1.061019 0.714777 0.084093
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_7
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 41.701729 276.545380 -700.711975 sawtooth 0 1 1.152413 0.089498
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.132112 0.942439 1.152413 0.089498
rgbGen wave sawtooth -0.076473 0.944340 1.152413 0.089498
tcMod stretch sawtooth 0.167602 0.832398 1.152413 0.089498
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_8
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -265.627167 165.084656 -621.369385 sawtooth 0 1 0.362502 0.095352
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.111902 0.638151 0.362502 0.095352
rgbGen wave sawtooth -0.002008 0.671419 0.362502 0.095352
tcMod stretch sawtooth 0.123814 0.876186 0.362502 0.095352
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_9
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 215.580246 342.617493 -695.396912 sawtooth 0 1 1.075964 0.081699
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.227738 0.898303 1.075964 0.081699
rgbGen wave sawtooth -0.036189 0.682565 1.075964 0.081699
tcMod stretch sawtooth -0.021955 1.021955 1.075964 0.081699
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_10
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -250.471252 7.080132 -915.334106 sawtooth 0 1 0.409897 0.077977
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.256859 1.102295 0.409897 0.077977
rgbGen wave sawtooth 0.182385 0.777758 0.409897 0.077977
tcMod stretch sawtooth -0.093649 1.093649 0.409897 0.077977
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_11
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 228.175308 207.763031 -888.754883 sawtooth 0 1 0.467730 0.077472
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.144826 0.575832 0.467730 0.077472
rgbGen wave sawtooth -0.196045 1.030830 0.467730 0.077472
tcMod stretch sawtooth 0.071090 0.928910 0.467730 0.077472
blendfunc add
}
}

textures/pad_jail/pad_jail_sprayroom2_snowflake2_12
{
qer_editorimage textures/pad_jail/pad_jail_snowflake1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -36.482327 -209.509018 -705.667603 sawtooth 0 1 0.745326 0.095180
{
clampmap textures/pad_jail/pad_jail_snowflake1
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.135951 1.004642 0.745326 0.095180
rgbGen wave sawtooth -0.061885 0.699545 0.745326 0.095180
tcMod stretch sawtooth -0.045991 1.045991 0.745326 0.095180
blendfunc add
}
}



// ********************************************************
// *
// *   MopAn's Jail:  sprayroom2 teleporter rings -> by MopAn
// *
// ********************************************************

textures/pad_jail/wave3_1
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.585986 0.278106
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 9.146581
AlphaGen wave sawtooth 0.283844 -0.283844 1.585986 0.278106
rgbGen wave sawtooth 0.198733 -0.198733 1.585986 0.278106
tcMod stretch sawtooth 1.404874 -1.421766 1.585986 0.278106
//blendfunc add
blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_2
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.239296 0.292045
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 7.904111
AlphaGen wave sawtooth 0.213291 -0.213291 1.239296 0.292045
rgbGen wave sawtooth 0.134114 -0.134114 1.239296 0.292045
tcMod stretch sawtooth 1.400241 -1.178539 1.239296 0.292045
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_3
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.070852 0.235548
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 8.617512
AlphaGen wave sawtooth 0.212510 -0.212510 0.070852 0.235548
rgbGen wave sawtooth 0.287658 -0.287658 0.070852 0.235548
tcMod stretch sawtooth 1.402273 -1.281514 0.070852 0.235548
//blendfunc add
blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_4
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.669552 0.283332
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 8.694662
AlphaGen wave sawtooth 0.251653 -0.251653 0.669552 0.283332
rgbGen wave sawtooth 0.154585 -0.154585 0.669552 0.283332
tcMod stretch sawtooth 1.404735 -1.154224 0.669552 0.283332
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_5
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.544011 0.263802
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 11.808039
AlphaGen wave sawtooth 0.124970 -0.124970 0.544011 0.263802
rgbGen wave sawtooth 0.180380 -0.180380 0.544011 0.263802
tcMod stretch sawtooth 1.395282 -1.489410 0.544011 0.263802
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_6
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.026447 0.281844
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 9.286477
AlphaGen wave sawtooth 0.248485 -0.248485 1.026447 0.281844
rgbGen wave sawtooth 0.109912 -0.109912 1.026447 0.281844
tcMod stretch sawtooth 1.404124 -1.403889 1.026447 0.281844
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_7
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.440620 0.242127
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 12.746544
AlphaGen wave sawtooth 0.211991 -0.211991 1.440620 0.242127
rgbGen wave sawtooth 0.208707 -0.208707 1.440620 0.242127
tcMod stretch sawtooth 1.402449 -1.156210 1.440620 0.242127
//blendfunc add
blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_8
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.745872 0.256581
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 13.971923
AlphaGen wave sawtooth 0.159755 -0.159755 0.745872 0.256581
rgbGen wave sawtooth 0.277349 -0.277349 0.745872 0.256581
tcMod stretch sawtooth 1.398725 -1.161715 0.745872 0.256581
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_9
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.334922 0.273740
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 10.825830
AlphaGen wave sawtooth 0.222446 -0.222446 0.334922 0.273740
rgbGen wave sawtooth 0.218296 -0.218296 0.334922 0.273740
tcMod stretch sawtooth 1.404419 -1.158729 0.334922 0.273740
//blendfunc add
blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_10
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 1.470992 0.293101
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 11.824884
AlphaGen wave sawtooth 0.133900 -0.133900 1.470992 0.293101
rgbGen wave sawtooth 0.153127 -0.153127 1.470992 0.293101
tcMod stretch sawtooth 1.396518 -1.235963 1.470992 0.293101
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}

textures/pad_jail/wave3_11
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.989337 0.273592
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 11.077547
AlphaGen wave sawtooth 0.246123 -0.246123 0.989337 0.273592
rgbGen wave sawtooth 0.234263 -0.234263 0.989337 0.273592
tcMod stretch sawtooth 1.402610 -1.187940 0.989337 0.273592
//blendfunc add
blendfunc GL_DST_COLOR GL_ONE

}
}

textures/pad_jail/wave3_12
{
qer_editorimage textures/pad_jail/wave3
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes move 0.000000 0.000000 0.000000 sawtooth 0 1 0.189459 0.224522
{
clampmap textures/pad_jail/pad_jail_sprayroom2_wave3
tcMod rotate 12.702108
AlphaGen wave sawtooth 0.282159 -0.282159 0.189459 0.224522
rgbGen wave sawtooth 0.299695 -0.299695 0.189459 0.224522
tcMod stretch sawtooth 1.395708 -1.178425 0.189459 0.224522
blendfunc add
//blendfunc GL_DST_COLOR GL_ONE
}
}


textures/pad_jail/pad_jail_sprayroom2_telefield
{
	qer_trans 0.6
	qer_editorimage textures/pad_jail/pad_jail_sprayroom2_telefield
	q3map_globaltexture
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	cull twosided
	{
		map textures/pad_jail/pad_jail_sprayroom2_telefield
		blendfunc add
		tcmod scroll .04 .03
		tcMod transform 0.25 0 0 0.25 0.1075 0.1075
		rgbGen identity
	}

}

// ********************************************************
// *
// *   sky -> 1. by ydnar, 2. by MopAn
// *
// ********************************************************

textures/pad_jail/pad_jail_sky4
{
	qer_editorimage env/jail_sprayroom3_ft
	q3map_globaltexture
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_sunExt 0.68 0.82 0.86 40 -300 27.4 2 1
	q3map_lightRGB 0.7 0.8 1.0
	q3map_lightmapFilterRadius 0 64
	q3map_skyLight 38 3

	skyparms env/jail_sprayroom3 - -
}

// ***********************************************************
// *
// *  MopAn's Jail: environment, skybox -> by Mighty Pete
// *
// ***********************************************************
textures/pad_jail/pad_jail_envcolor
{
	qer_editorimage env/padcity-hilton512_ft
	q3map_lightimage textures/colors/hex_d1a76bff
	q3map_globaltexture
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_sunExt 0.68 0.82 0.86 40 -300 27.4 2 1
	q3map_lightRGB 0.7 0.8 1.0
	q3map_lightmapFilterRadius 0 64
	q3map_skyLight 38 3

	skyparms env/padcity-hilton512 - -
}


// ======================================================================
// collected from terrain-tutorial by sock at simland
// DotProduct2 Terrain blending
// ======================================================================
textures/pad_jail/jail_grassmud
{
        qer_editorimage textures/pad_jail/jail_grassmud

	q3map_nonplanar
	q3map_shadeangle 70
	q3map_lightmapAxis z
	q3map_lightmapmergable
	//q3map_globaltexture
	q3map_tcGen ivector ( 256 0 0 ) ( 0 256 0 )
	q3map_alphaMod dotproduct2 ( 0.0 0.0 0.75 )
	q3map_lightmapSampleSize 16
	surfaceparm sandsteps

	{
		map textures/pad_jail/jail_dirt1	// Primary
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_moss1	// Secondary
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen identity
		alphaGen vertex
		detail

	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/jail_snowrock
{
        qer_editorimage textures/pad_jail/jail_snowrock

	q3map_nonplanar
	q3map_shadeangle 70
	q3map_lightmapAxis z
	q3map_lightmapmergable
	//q3map_globaltexture
	q3map_tcGen ivector ( 256 0 0 ) ( 0 256 0 )
	q3map_alphaMod dotproduct2 ( 0.0 0.0 0.75 )
	q3map_lightmapSampleSize 16
	surfaceparm snowsteps

	{
		map textures/pad_jail/jail_snowrock5	// Primary
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_snowrock10	// Secondary
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen identity
		alphaGen vertex
		detail

	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

//textures/pad_jail/jail_snowandrock
//{
//        qer_editorimage textures/pad_jail/jail_place_snowrock
//
//	q3map_nonplanar
//	q3map_shadeangle 70
//	q3map_lightmapAxis z
//	q3map_lightmapmergable
//	q3map_tcGen ivector ( 256 0 0 ) ( 0 256 0 )
//	q3map_alphaMod dotproduct2 ( 0.0 0.0 0.75 )
//	q3map_lightmapSampleSize 16
//	{
//		map textures/pad_jail/jail_snowrock1	// Primary
//		rgbGen identity
//	}
//	{
//		map textures/pad_jail/jail_snowrock10	// Secondary
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		//alphaFunc GE128
//		rgbGen identity
//		alphaGen vertex
//		detail
//	}
//	{
//		map $lightmap
//		blendFunc GL_DST_COLOR GL_ZERO
//		rgbGen identity
//	}
//}


// ======================================================================
// Hong Phonged textures
// ======================================================================
//textures/pad_jail/snowrock5
//{
//	q3map_nonplanar
//	q3map_shadeangle 120
//	qer_editorimage textures/pad_jail/snowrock5
//	{
//		map $lightmap
//		rgbGen identity
//	}
//	{
//		map textures/pad_jail/snowrock5
//		blendFunc filter
//	}
//
//}


// ********************************************************
// *
// *   dcl blending on rock3 -> by ydnar
// *
// ********************************************************

//textures/pad_jail/jail_dcl_rock3_moss		// Normal texture blending
//{
//        qer_editorimage textures/pad_jail/bld_rock2moss1
//
//	q3map_nonplanar
//	q3map_shadeAngle 120
//
//	{
//		map textures/pad_jail/jail_rock3	// Primary
//		rgbGen identity
//	}
//	{
//		map textures/pad_jail/jail_moss1	// Secondary
//		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
//		alphaFunc GE128
//		rgbGen identity
//		alphaGen vertex
//	}
//	{
//		map $lightmap
//		blendFunc GL_DST_COLOR GL_ZERO
//		rgbGen identity
//	}
//}

// ======================================================================
// alpha fade shaders
// (c) 2004 randy reddig
// http://www.shaderlab.com
// distribution, in part or in whole, in any medium, permitted
// ======================================================================
//
// These shaders are not fixed to this directory location, they can
// be moved around. They exist here for convenience only.
//
textures/pad_jail/jail_alpha_000	// Primary texture ONLY
{
	qer_editorimage textures/common/alpha_000
	q3map_alphaMod volume
	q3map_alphaMod set 0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	qer_trans 0.75
}


textures/pad_jail/jail_alpha_025
{
	qer_editorimage textures/common/alpha_025
	q3map_alphaMod volume
	q3map_alphaMod set 0.25
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm nomarks
	surfaceparm trans
	qer_trans 0.75
}


textures/pad_jail/jail_alpha_050	// Perfect mix of both Primary + Secondary
{
	qer_editorimage textures/common/alpha_050
	q3map_alphaMod volume
	q3map_alphaMod set 0.50
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	qer_trans 0.75
}

textures/pad_jail/jail_alpha_075
{
	qer_editorimage textures/common/alpha_075
	q3map_alphaMod volume
	q3map_alphaMod set 0.75
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	qer_trans 0.75
}

textures/pad_jail/jail_alpha_085
{
	qer_editorimage textures/common/alpha_085
	q3map_alphaMod volume
	q3map_alphaMod set 0.85
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	qer_trans 0.75
}

textures/pad_jail/jail_alpha_100	// Secondary texture ONLY
{
	qer_editorimage textures/common/alpha_100
	q3map_alphaMod volume
	q3map_alphaMod set 1.0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	qer_trans 0.75
}



// ********************************************************
// *
// *   MopAn's Jail:  basketball-net -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_basket
{
	surfaceparm nonsolid
	//surfaceparm trans
	//surfaceparm nomarks
	//surfaceparm noimpact
	cull disable

	{
		map textures/pad_jail/pad_jail_basket
		rgbGen vertex
		alphaFunc GE128
		blendFunc GL_ONE GL_ZERO
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  moving animals, bat + fly-swarm -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_fly1
{
       qer_editorimage textures/pad_jail/pad_jail_fly1
       surfaceparm trans
       surfaceparm pointlight
       surfaceparm nomarks
       surfaceparm nodamage
       surfaceparm nonsolid
        surfaceparm nolightmap
        deformVertexes move 2 3 1.5  sin 0 5 0 0.3
        //deformVertexes move .6 3.3 0  sin 0 5 0 0.4
        deformVertexes wave 20 sin 0 8 0 .2
        tessSize 32
        cull none
{
	map $lightmap
	rgbGen identity
	blendfunc gl_zero gl_one
}
{

	map textures/pad_jail/pad_jail_fly1
                tcMod Scroll -5 0.1
                tcMod turb .3 .25 0 .1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
{
 	map textures/pad_jail/pad_jail_fly2
                tcMod Scroll 4 -0.5
                tcMod turb .1 .25 0 -.1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
}

textures/pad_jail/pad_jail_fly2
{
       qer_editorimage textures/pad_jail/pad_jail_fly1
       surfaceparm trans
       surfaceparm pointlight
       surfaceparm nomarks
       surfaceparm nodamage
       surfaceparm nonsolid
        surfaceparm nolightmap
        deformVertexes move 1 2.5 2  sin 0 6 0 0.4
        //deformVertexes move .6 3.3 0  sin 0 5 0 0.4
        deformVertexes wave 10 sin 0 4 0 .4
        tessSize 16
        cull none
{
	map $lightmap
	rgbGen identity
	blendfunc gl_zero gl_one
}
{

	map textures/pad_jail/pad_jail_fly2
                tcMod Scroll -4 0.3
                tcMod turb .2 .3 0 -.1
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
{
 	map textures/pad_jail/pad_jail_fly1
                tcMod Scroll 6 -0.4
                tcMod turb .2 .25 0 -.2
                blendfunc gl_src_alpha gl_one_minus_src_alpha
}
 }

textures/pad_jail/pad_jail_bat
{
	qer_editorimage textures/pad_jail/pad_jail_bat
	surfaceparm nodamage
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	deformVertexes wave 12 sin 0 18 0 3.2
	deformVertexes move 4 4 4 sin 0 1 0.75 0.5
	tessSize 16
        	cull disable
	{
		map textures/pad_jail/pad_jail_bat
		blendfunc blend
		rgbGen identity
	}
}


// ********************************************************
// *
// *   MopAn's Jail:  particle-glowstar for teleporter-tunnel -> by MopAn, generated by Particle Studio
// *
// ********************************************************
textures/pad_jail/pad_jail_glowstar1_1
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 0.923795 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.923795 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.923795 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.923795 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_2
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 0.207709 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.207709 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.207709 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.207709 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_3
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 1.573229 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.573229 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.573229 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.573229 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_4
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 0.432081 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.432081 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.432081 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.432081 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_5
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 1.816828 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.816828 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.816828 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.816828 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_6
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 0.600848 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.600848 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.600848 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.600848 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_7
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 1.889340 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.889340 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.889340 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.889340 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar1_8
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1850.000000 0.000000 0 sawtooth 0 1 1.053560 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.053560 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.053560 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.053560 0.125000
blendfunc add
	}
}

textures/pad_jail/pad_jail_glowstar2_1
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 0.923795 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.923795 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.923795 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.923795 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_2
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 0.207709 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.207709 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.207709 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.207709 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_3
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 1.573229 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.573229 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.573229 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.573229 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_4
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 0.432081 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.432081 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.432081 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.432081 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_5
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 1.816828 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.816828 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.816828 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.816828 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_6
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 0.600848 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 0.600848 0.125000
rgbGen wave sawtooth 1.000000 0.000000 0.600848 0.125000
tcMod stretch sawtooth 0.400000 0.600000 0.600848 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_7
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 1.889340 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.889340 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.889340 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.889340 0.125000
blendfunc add
}
}

textures/pad_jail/pad_jail_glowstar2_8
{
qer_editorimage textures/pad_jail/pad_jail_glowstar1
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1850.000000 0.000000 0 sawtooth 0 1 1.053560 0.125000
{
clampmap textures/pad_jail/pad_jail_glowstar1
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.000000 -0.800000 1.053560 0.125000
rgbGen wave sawtooth 1.000000 0.000000 1.053560 0.125000
tcMod stretch sawtooth 0.400000 0.600000 1.053560 0.125000
blendfunc add
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  particle-waterdrop -> by MopAn, generated by Particle Studio
// *
// ********************************************************
textures/pad_jail/waterdrop_1
{
qer_editorimage textures/pad_jail/pad_jail_waterdrop
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 3.722174 0.000012 -360 sawtooth 0 1 0.415662 0.700000
{
clampmap textures/pad_jail/pad_jail_waterdrop
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.700000 0.300000 0.415662 0.500000
rgbGen wave sawtooth 1.000000 0.000000 0.415662 0.500000
tcMod stretch sawtooth 0.300000 -0.100000 0.415662 0.500000
blendfunc add
}
}

textures/pad_jail/waterdrop_2
{
qer_editorimage textures/pad_jail/pad_jail_waterdrop
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.184236 0.000013 -360 sawtooth 0 1 0.013184 0.700000
{
clampmap textures/pad_jail/pad_jail_waterdrop
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.700000 0.300000 0.013184 0.500000
rgbGen wave sawtooth 1.000000 0.000000 0.013184 0.500000
tcMod stretch sawtooth 0.300000 -0.100000 0.013184 0.500000
blendfunc add
}
}

textures/pad_jail/waterdrop_3
{
qer_editorimage textures/pad_jail/pad_jail_waterdrop
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -1.155827 0.000013 -360 sawtooth 0 1 0.688711 0.700000
{
clampmap textures/pad_jail/pad_jail_waterdrop
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.700000 0.300000 0.688711 0.500000
rgbGen wave sawtooth 1.000000 0.000000 0.688711 0.500000
tcMod stretch sawtooth 0.300000 -0.100000 0.688711 0.500000
blendfunc add
}
}

textures/pad_jail/waterdrop_4
{
qer_editorimage textures/pad_jail/pad_jail_waterdrop
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -4.344173 0.000012 -360 sawtooth 0 1 0.067751 0.700000
{
clampmap textures/pad_jail/pad_jail_waterdrop
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.700000 0.300000 0.067751 0.500000
rgbGen wave sawtooth 1.000000 0.000000 0.067751 0.500000
tcMod stretch sawtooth 0.300000 -0.100000 0.067751 0.500000
blendfunc add
}
}

textures/pad_jail/waterdrop_5
{
qer_editorimage textures/pad_jail/pad_jail_waterdrop
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -3.709201 0.000012 -360 sawtooth 0 1 0.015198 0.700000
{
clampmap textures/pad_jail/pad_jail_waterdrop
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.700000 0.300000 0.015198 0.500000
rgbGen wave sawtooth 1.000000 0.000000 0.015198 0.500000
tcMod stretch sawtooth 0.300000 -0.100000 0.015198 0.500000
blendfunc add
}
}

// ********************************************************
// *
// *   MopAn's Jail:  Floor2_slimy -> by MopAn
// *
// ********************************************************

textures/pad_jail/pad_jail2_redfloor_water
{
     	qer_editorimage textures/pad_jail/pad_jail2_redfloor_slimy
	//surfaceparm metalsteps
                {
		map textures/pad_jail/pad_jail_slimy
                	blendFunc GL_ONE GL_ZERO
               		tcMod scroll .01 .03
                	tcMod turb 0 0.05 0 .05
	}
                {
		map textures/pad_gfx02/tinpad
                	blendFunc GL_ONE GL_ONE
                	tcGen environment
       	}
               {
		map textures/pad_jail/pad_jail2_redfloor_slimy
                	blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
        	{
		map $lightmap
                	blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  Teleporter-stripes -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_tele_stripes
{
	qer_editorimage textures/pad_jail/pad_jail_tele_stripes
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm nolightmap
	cull disable
	{
		map textures/pad_jail/pad_jail_tele_stripes
		blendfunc add
		rgbGen wave sin 0.4 0.2 0 0.2
		tcMod Scroll 2 0.5
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  pad_metal -> wallrast_a - blend-effect by MopAn
// *
// ********************************************************
textures/pad_jail/wallrast_a_blend
{
	qer_editorimage textures/pad_metal/wallrast_a
	surfaceparm	metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_metal/wallrast_a
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_jail/wallrast_a_blend
		blendFunc add
		rgbGen wave sin 0.4 0.2 0 0.2
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  Plants -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_busch
{
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	cull disable

	{
		map textures/pad_jail/pad_jail_busch
		rgbGen vertex
		alphaFunc GE128
		blendFunc GL_ONE GL_ZERO
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  Kleiderbuegel -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_buegel
{
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	cull disable

	{
		map textures/pad_jail/pad_jail_buegel
		rgbGen vertex
		alphaFunc GE128
		blendFunc GL_ONE GL_ZERO
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  Telepad -> by ?
// *
// ********************************************************
textures/pad_jail/telepad1
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
	map textures/pad_tex02/telepad
	tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
	blendfunc GL_ONE GL_ONE
	}
}

// ********************************************************
// *
// *   MopAn's Jail:  spidernet -> by MopAn
// *
// ********************************************************
textures/pad_jail/pad_jail_spidernet1
{
	qer_editorimage textures/pad_jail/pad_jail_spidernet1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable

	{
		map textures/pad_jail/pad_jail_spidernet1
		blendfunc add
		rgbgen identity
	}
}

textures/pad_jail/pad_jail_spidernet2
{
	qer_editorimage textures/pad_jail/pad_jail_spidernet2
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable
	tessSize 64
        	deformVertexes wave 194 sin 0 3 0 .4
        	deformVertexes normal .5 .1
	{
		map textures/pad_jail/pad_jail_spidernet2
		blendfunc add
		rgbgen identity
	}
}

textures/pad_jail/pad_jail_spidernet3
{
	qer_editorimage textures/pad_jail/pad_jail_spidernet3
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable
	tessSize 64
        	deformVertexes wave 194 sin 0 3 0 .4
        	deformVertexes normal .5 .1
	{
		map textures/pad_jail/pad_jail_spidernet3
		blendfunc add
		rgbgen identity
	}
}

// ***********************************************************
// *
// *  MopAn's Jail:  rain -> original by q3, modified by MopAn
// *
// ***********************************************************
textures/pad_jail/pad_jail_rain
{
        surfaceparm trans
        surfaceparm nomarks
        surfaceparm nonsolid
        surfaceparm nolightmap

        deformVertexes move 3 1 0  sin 0 5 0 0.2
        deformVertexes move .6 3.3 0  sin 0 5 0 0.4
        deformVertexes wave 30 sin 0 10 0 .2
        cull none
{
	map textures/pad_jail/pad_jail_rain
                tcMod Scroll .5 -8
                tcMod turb .1 .25 0 -.1
                blendFunc GL_ONE GL_ONE
        }
        {
	map textures/pad_jail/pad_jail_rain
                tcMod Scroll .01 -6.3
                blendFunc GL_ONE GL_ONE
        }
 }

// ***********************************************************
// *
// *  MopAn's Jail: barbed-wire -> by MopAn
// *
// ***********************************************************
textures/pad_jail/pad_jail_barbed_wire1
{
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm alphashadow
	cull disable

	{
		map textures/pad_jail/pad_jail_barbed_wire1
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



// ***********************************************************
// *
// *  MopAn's Jail: plastic, grates, floors and metals -> by MopAn
// *
// ***********************************************************
textures/pad_jail/pad_jail_tin
{
        {
		rgbGen identity
		map $lightmap
	}
	{
		map textures/pad_jail/pad_jail_tin
		blendFunc GL_DST_COLOR GL_SRC_ALPHA
		rgbGen identity
		alphaGen lightingSpecular
	}
        	{
		map textures/pad_gfx02/tinpad
                	tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}

        	{
		map textures/pad_jail/pad_jail_tin
               		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}



textures/pad_jail/pad_jail_metal14
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		tcmod scale .25 .25
	}
	{
		map textures/pad_jail/pad_jail_metal14
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		detail
        		tcMod scale 0.0693 0.0712
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_plasticred
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_plasticred
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_trash
{

	surfaceparm woodsteps

	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_trash
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_trashb
{

	surfaceparm metalsteps

	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_trashb
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_metal_shine
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_metal_shine
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_grate1
{
	qer_editorimage textures/pad_jail/pad_jail_grate1
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_grate1
		tcMod scale 1.0 1.0
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_farbsieb
{
	qer_editorimage textures/pad_jail/pad_jail_farbsieb
	cull disable
    	surfaceparm alphashadow
        	//surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_farbsieb
		tcMod scale 1.0 1.0
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_maschendrahtzaun
{
	qer_editorimage textures/pad_jail/jail2_maschendrahtzaun
	cull disable
    	surfaceparm alphashadow
        surfaceparm latticesteps
        surfaceparm nomarks
	surfaceparm trans
	{
		map textures/pad_jail/jail2_maschendrahtzaun
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


textures/pad_jail/pad_jail_grate2
{
	qer_editorimage textures/pad_jail/pad_jail_grate2
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_grate2
		tcMod scale 1.0 1.0
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}
}


textures/pad_jail/jail2_krippgitter
{
	qer_editorimage textures/pad_jail/jail2_krippgitter
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/jail2_krippgitter
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_zink_geruest_tools
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink_geruest_tools
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_zink_geruest_klein
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink_geruest_klein
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_zink_ger_klein_dark
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink_ger_klein_dark
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_zink1
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink1
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_grate6
{
	qer_editorimage textures/pad_jail/pad_jail_grate6
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_grate6
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity

	}
}


textures/pad_jail/pad_jail_grate6_deckel
{
	qer_editorimage textures/pad_jail/pad_jail_grate6_deckel
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_grate6_deckel
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity

	}
}


textures/pad_jail/pad_jail_metallstufe
{
	qer_editorimage textures/pad_jail/pad_jail_metallstufe
	cull disable
    	surfaceparm alphashadow
        	surfaceparm latticesteps
	{
		map textures/pad_jail/pad_jail_metallstufe
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		depthWrite
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ZERO
		depthFunc equal
		rgbGen identity

	}
}


textures/pad_jail/pad_jail_zink1a
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink1a
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_zink2
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink2
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_zink2_border
{

	surfaceparm metalsteps
	{
		map textures/pad_gfx02/tinpad
		tcGen environment
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_zink2_border
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map $lightmap
		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
		rgbGen identity
	}
}

// ***********************************************************
// *
// *  MopAn's Jail: stepsounds -> by MopAn
// *
// ***********************************************************



textures/pad_jail/jail2_holzkiste_deckel1
{
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/jail2_holzkiste_deckel1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_wood1
{
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_wood1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_wood2
{
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_wood2
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/jail_holzkiste_sound
{
	qer_editorimage textures/pad_woodobjects/chest01_6
	surfaceparm woodsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_woodobjects/chest01_6
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_metalfloor1
{
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_metalfloor1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_metal11
{
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_metal11
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_metal1
{
	surfaceparm metalsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_metal1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


// ***********************************************************
// *
// *  MopAn's Jail: rust -> by MopAn
// *
// ***********************************************************
textures/pad_jail/pad_jail_rust1
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_rust1
               		blendFunc add
		rgbGen vertex
	}
}

// ***********************************************************
// *
// *  MopAn's Jail: lights, lamps and flares-> by MopAn
// *
// ***********************************************************
textures/pad_jail/jail_padlamp
{
	qer_editorimage textures/pad_jail/jail_padlamp
	light 1
	surfaceparm nomarks
	q3map_surfacelight 4000
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_padlamp
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_padlamp_blend
		rgbGen wave sin .6 .1 .1 .1
		blendFunc GL_ONE GL_ONE
	}
}

textures/pad_jail/pad_jail_light_white
{
	qer_editorimage textures/pad_jail/pad_jail_light_white
	q3map_surfacelight 200
	light 1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans

	{
		map textures/pad_jail/pad_jail_light_white
		blendFunc GL_dst_color GL_one
	}
	{
		map textures/pad_jail/pad_jail_light_white
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_jail/pad_jail_light_white2
{
	qer_editorimage textures/pad_jail/pad_jail_light_white
	q3map_surfacelight 200
	light 1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans

	{
		map textures/pad_jail/pad_jail_light_white
		blendFunc GL_dst_color GL_one
	}
	{
		map textures/pad_jail/pad_jail_light_white
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_jail/pad_jail_light_white3
{
	qer_editorimage textures/pad_jail/pad_jail_light_white
	q3map_surfacelight 200
	light 1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans

	{
		map textures/pad_jail/pad_jail_light_white
		blendFunc GL_dst_color GL_one
	}
	{
		map textures/pad_jail/pad_jail_light_white
		blendfunc GL_ONE GL_ONE
		rgbGen wave square .5 .5 0 1
	}
}

textures/pad_jail/pad_jail_light_white1000
{
	qer_editorimage textures/pad_jail/pad_jail_light_white
	q3map_surfacelight 1000
	light 1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans

	{
		map textures/pad_jail/pad_jail_light_white
		blendFunc GL_dst_color GL_one
	}
	{
		map textures/pad_jail/pad_jail_light_white
		blendfunc GL_ONE GL_ONE
		//rgbGen wave square .5 .5 0 1
	}
}

textures/pad_jail/pad_jail_light_white2400
{
	qer_editorimage textures/pad_jail/pad_jail_light_white
	q3map_surfacelight 2400
	light 1
	surfaceparm noimpact
	surfaceparm nomarks
	surfaceparm nolightmap
	surfaceparm trans

	{
		map textures/pad_jail/pad_jail_light_white
		blendFunc GL_dst_color GL_one
	}
	{
		map textures/pad_jail/pad_jail_light_white
		blendfunc GL_ONE GL_ONE
		//rgbGen wave square .5 .5 0 1
	}
}


// ***********************************************************
// *
// *  MopAn's Jail: glass  -> by MopAn
// *
// ***********************************************************
textures/pad_jail/pad_jail_glass
{
	qer_editorimage textures/pad_gfx02/plas07
	surfaceparm nolightmap
	cull twosided
	{
		map textures/pad_gfx02/plas07
		tcGen environment
		blendfunc GL_ONE GL_ONE
                }
}


textures/pad_jail/pad_jail2_glass_matt
{
	qer_editorimage textures/pad_jail/pad_jail2_glass_matt
	surfaceparm nolightmap
	cull twosided
	{
		map textures/pad_jail/pad_jail2_glass_matt
		tcGen environment
		blendfunc GL_ONE GL_ONE
                }
}


// ***********************************************************
// *
// *  MopAn's Jail: decals -> by MopAn
// *
// ***********************************************************
textures/pad_objects/obj047d
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_objects/obj047d
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_poster/poster007
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_poster/poster007
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_poster/poster036
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_poster/poster036
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


textures/pad_jail/pad_jail_pinup1
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_pinup1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/jail_raquel
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/jail_raquel
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_striche
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_striche
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_jack
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_jack
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_trash_sign
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_trash_sign
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/pad_jail_flash
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/pad_jail_flash
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_objects/obj059
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_objects/obj059
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/wanted_pad1
{
	surfaceparm noimpact
	surfaceparm nonsolid
	surfaceparm trans
	polygonOffset
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_jail/wanted_pad1
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_jail/jail2_farbkleks
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/jail2_farbkleks
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_dirt1
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_jail/pad_jail_dirt1a
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1a
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_dirt1c
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1c
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_dirt1d
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1d
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_dirt1e
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1e
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_dirt1f
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_dirt1f
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_nr1
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_nr1
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_nr2
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_nr2
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_nr3
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_nr3
               		blendFunc add
		rgbGen vertex
	}
}

textures/pad_jail/pad_jail_nr4
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_jail/pad_jail_nr4
               		blendFunc add
		rgbGen vertex
	}
}

// **************************************************************************************
// *
// *  MopAn's Jail: liquids -> by MopAn *original created for 'Polaris' modified for pad_jail, changed textures, added light
// *
// **************************************************************************************
textures/pad_jail/pad_jail_water2
{
	qer_editorimage textures/pad_jail/pad_jail_water2
	q3map_lightsubdivide 32
	qer_trans .8
	qer_nocarve
	q3map_globaltexture
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm water
	q3map_surfacelight 50
	cull disable
	tesssize 64
	{
		map textures/pad_jail/pad_jail_water2
		//blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
		//blendfunc add
		//blendfunc GL_DST_COLOR GL_ONE
		blendfunc GL_DST_COLOR GL_SRC_COLOR
		//rgbgen identity
		tcmod scale .5 .5
		tcMod turb .3 .2 1 .05
		tcmod scroll .015 .001
      	}
	{
		map textures/pad_jail/pad_jail_water2
		//blendfunc GL_ONE GL_ONE_MINUS_SRC_COLOR
		//blendfunc add
		//blendfunc GL_DST_COLOR GL_ONE
		blendfunc GL_DST_COLOR GL_SRC_COLOR
		//rgbgen identity
		tcmod scale .7 .7
		tcMod turb .2 .1 1 .05
		tcmod scroll -.01 -.01
       	}

}


// ***********************************************************
// *
// *  MopAn's Jail: particles splash -> by MopAn, made with particle studio, thanx to the creators
// *
// ***********************************************************
textures/pad_jail/pad_jail_splash_1
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.001783 -0.017516 0.171927 sawtooth 0 1 0.050000 5.415586
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.168883 -0.927244 0.050000 5.415586
rgbGen wave sawtooth 0.735966 -0.142424 0.050000 5.415586
tcMod stretch sawtooth 0.300000 1.068737 0.050000 5.415586
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_2
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.003432 -0.013143 0.196668 sawtooth 0 1 0.050000 4.289267
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.015302 -0.293161 0.050000 4.289267
rgbGen wave sawtooth 0.813166 -0.318186 0.050000 4.289267
tcMod stretch sawtooth 0.300000 0.678637 0.050000 4.289267
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_3
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.082027 -0.074155 0.419187 sawtooth 0 1 0.050000 2.758142
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.179943 -0.923197 0.050000 2.758142
rgbGen wave sawtooth 0.957604 -0.446760 0.050000 2.758142
tcMod stretch sawtooth 0.300000 0.438395 0.050000 2.758142
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_4
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.096731 0.030182 0.397254 sawtooth 0 1 0.050000 2.821846
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.076534 -0.301932 0.050000 2.821846
rgbGen wave sawtooth 0.826069 -0.287912 0.050000 2.821846
tcMod stretch sawtooth 0.300000 0.523212 0.050000 2.821846
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_5
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.006604 0.014914 0.259085 sawtooth 0 1 0.050000 3.494662
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.113401 -0.445424 0.050000 3.494662
rgbGen wave sawtooth 0.733903 -0.219044 0.050000 3.494662
tcMod stretch sawtooth 0.300000 1.439134 0.050000 3.494662
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_6
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.035492 -0.033019 0.404591 sawtooth 0 1 0.050000 2.068297
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.017341 -0.727104 0.050000 2.068297
rgbGen wave sawtooth 0.698184 -0.226295 0.050000 2.068297
tcMod stretch sawtooth 0.300000 0.398111 0.050000 2.068297
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_7
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.012884 -0.000916 0.178425 sawtooth 0 1 0.050000 6.675291
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.173962 -0.647713 0.050000 6.675291
rgbGen wave sawtooth 0.643605 -0.064510 0.050000 6.675291
tcMod stretch sawtooth 0.300000 0.459526 0.050000 6.675291
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_8
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.060222 0.027980 0.505405 sawtooth 0 1 0.050000 1.748795
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.869045 -0.291763 0.050000 1.748795
rgbGen wave sawtooth 0.639760 -0.086288 0.050000 1.748795
tcMod stretch sawtooth 0.300000 1.264776 0.050000 1.748795
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_9
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.068247 0.030351 0.452946 sawtooth 0 1 0.050000 2.311882
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.806836 -0.164367 0.050000 2.311882
rgbGen wave sawtooth 0.861324 -0.351811 0.050000 2.311882
tcMod stretch sawtooth 0.300000 1.478210 0.050000 2.311882
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_10
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.011632 -0.040931 0.533328 sawtooth 0 1 0.050000 1.565514
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.838856 -0.092288 0.050000 1.565514
rgbGen wave sawtooth 0.667507 -0.154943 0.050000 1.565514
tcMod stretch sawtooth 0.300000 0.663549 0.050000 1.565514
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_11
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.051393 0.090922 0.726240 sawtooth 0 1 0.050000 1.557789
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.931498 -0.606214 0.050000 1.557789
rgbGen wave sawtooth 0.902292 -0.405335 0.050000 1.557789
tcMod stretch sawtooth 0.300000 0.603598 0.050000 1.557789
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_12
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.009477 -0.058013 0.509409 sawtooth 0 1 0.050000 2.104320
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.917887 -0.699539 0.050000 2.104320
rgbGen wave sawtooth 0.905429 -0.376501 0.050000 2.104320
tcMod stretch sawtooth 0.300000 0.630845 0.050000 2.104320
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_13
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.030592 0.009375 0.474610 sawtooth 0 1 0.050000 1.744549
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.842579 -0.482101 0.050000 1.744549
rgbGen wave sawtooth 0.796429 -0.339640 0.050000 1.744549
tcMod stretch sawtooth 0.300000 0.898920 0.050000 1.744549
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_14
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.029733 -0.034695 0.207681 sawtooth 0 1 0.050000 5.299960
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.811036 -0.261513 0.050000 5.299960
rgbGen wave sawtooth 0.906784 -0.322111 0.050000 5.299960
tcMod stretch sawtooth 0.300000 0.518671 0.050000 5.299960
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_15
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.080672 0.009013 0.327256 sawtooth 0 1 0.050000 3.306758
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.091452 -0.619770 0.050000 3.306758
rgbGen wave sawtooth 0.912265 -0.337150 0.050000 3.306758
tcMod stretch sawtooth 0.300000 0.337831 0.050000 3.306758
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_16
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.039629 0.000462 0.687502 sawtooth 0 1 0.050000 1.604236
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.890738 -0.318326 0.050000 1.604236
rgbGen wave sawtooth 0.915525 -0.452083 0.050000 1.604236
tcMod stretch sawtooth 0.300000 0.822672 0.050000 1.604236
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_17
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.014417 0.000118 0.438142 sawtooth 0 1 0.050000 2.578800
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.140391 -0.693246 0.050000 2.578800
rgbGen wave sawtooth 0.961083 -0.445314 0.050000 2.578800
tcMod stretch sawtooth 0.300000 0.815897 0.050000 2.578800
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_18
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.007536 -0.004976 0.341288 sawtooth 0 1 0.050000 2.494348
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.027277 -0.669967 0.050000 2.494348
rgbGen wave sawtooth 0.942833 -0.509574 0.050000 2.494348
tcMod stretch sawtooth 0.300000 1.402915 0.050000 2.494348
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_19
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.084113 0.021117 0.370545 sawtooth 0 1 0.050000 2.133421
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.967180 -0.583026 0.050000 2.133421
rgbGen wave sawtooth 0.978478 -0.435908 0.050000 2.133421
tcMod stretch sawtooth 0.300000 0.962313 0.050000 2.133421
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_20
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.038920 0.013735 0.531535 sawtooth 0 1 0.050000 1.943187
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.080734 -0.363903 0.050000 1.943187
rgbGen wave sawtooth 0.722513 -0.255074 0.050000 1.943187
tcMod stretch sawtooth 0.300000 1.233390 0.050000 1.943187
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_21
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.090587 -0.050741 0.417229 sawtooth 0 1 0.050000 2.485719
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.938786 -0.372509 0.050000 2.485719
rgbGen wave sawtooth 0.952757 -0.550298 0.050000 2.485719
tcMod stretch sawtooth 0.300000 1.012668 0.050000 2.485719
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_22
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.109010 -0.050115 0.629636 sawtooth 0 1 0.050000 1.849226
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.002582 -0.567174 0.050000 1.849226
rgbGen wave sawtooth 0.678420 -0.224928 0.050000 1.849226
tcMod stretch sawtooth 0.300000 0.581368 0.050000 1.849226
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_23
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.000845 -0.011895 0.261985 sawtooth 0 1 0.050000 4.265481
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.820557 -0.596570 0.050000 4.265481
rgbGen wave sawtooth 0.698868 -0.277212 0.050000 4.265481
tcMod stretch sawtooth 0.300000 0.360793 0.050000 4.265481
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_24
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.066402 0.092666 0.547784 sawtooth 0 1 0.050000 2.056769
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.139647 -0.432521 0.050000 2.056769
rgbGen wave sawtooth 0.638173 -0.099149 0.050000 2.056769
tcMod stretch sawtooth 0.300000 1.057604 0.050000 2.056769
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_25
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.060990 0.023611 0.487691 sawtooth 0 1 0.050000 2.229852
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.991522 -0.279470 0.050000 2.229852
rgbGen wave sawtooth 0.611499 -0.097208 0.050000 2.229852
tcMod stretch sawtooth 0.300000 0.961251 0.050000 2.229852
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_26
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.043759 -0.023044 0.589881 sawtooth 0 1 0.050000 1.844978
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.908817 -0.335545 0.050000 1.844978
rgbGen wave sawtooth 0.809125 -0.318723 0.050000 1.844978
tcMod stretch sawtooth 0.300000 1.030503 0.050000 1.844978
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_27
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.051986 -0.012217 0.262871 sawtooth 0 1 0.050000 4.269483
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.046236 -0.305216 0.050000 4.269483
rgbGen wave sawtooth 0.922422 -0.406452 0.050000 4.269483
tcMod stretch sawtooth 0.300000 1.048595 0.050000 4.269483
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_28
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.049827 -0.027156 0.361145 sawtooth 0 1 0.050000 2.293147
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.968206 -0.322440 0.050000 2.293147
rgbGen wave sawtooth 0.946092 -0.526005 0.050000 2.293147
tcMod stretch sawtooth 0.300000 1.424961 0.050000 2.293147
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_29
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.013764 -0.026963 0.410878 sawtooth 0 1 0.050000 2.776794
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 0.830189 -0.128739 0.050000 2.776794
rgbGen wave sawtooth 0.684744 -0.226746 0.050000 2.776794
tcMod stretch sawtooth 0.300000 1.031858 0.050000 2.776794
blendfunc add
}
}

textures/pad_jail/pad_jail_splash_30
{
qer_editorimage textures/pad_jail/pad_jail_splash
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.085460 -0.031875 0.601655 sawtooth 0 1 0.050000 1.826589
{
clampmap textures/pad_jail/pad_jail_splash
tcMod rotate 0.000000
AlphaGen wave sawtooth 1.198181 -0.544633 0.050000 1.826589
rgbGen wave sawtooth 0.734672 -0.137870 0.050000 1.826589
tcMod stretch sawtooth 0.300000 1.066430 0.050000 1.826589
blendfunc add
}
}

// ***********************************************************
// *
// *  MopAn's Jail: particles smoke_cigarette -> by MopAn, made with particle studio, thanx to the creators
// *
// ***********************************************************
textures/pad_jail/pad_jail_smoke2_1
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.758931 1.195109 9.779077 sawtooth 0 1 0.351521 1.429064
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 9.804224
AlphaGen wave sawtooth 0.751726 -0.363005 0.351521 1.429064
rgbGen wave sawtooth 1.000000 -0.271340 0.351521 1.429064
tcMod stretch sawtooth 0.200000 0.669204 0.351521 1.429064
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_2
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 4.525688 -2.174430 29.160147 sawtooth 0 1 0.242314 0.846443
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 21.364635
AlphaGen wave sawtooth 0.836775 -0.798151 0.242314 0.846443
rgbGen wave sawtooth 1.000000 -0.304605 0.242314 0.846443
tcMod stretch sawtooth 0.200000 0.828462 0.242314 0.846443
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_3
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.144382 -0.464500 8.018268 sawtooth 0 1 0.273992 1.619099
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 5.690939
AlphaGen wave sawtooth 0.629847 -0.504404 0.273992 1.619099
rgbGen wave sawtooth 1.000000 -0.330033 0.273992 1.619099
tcMod stretch sawtooth 0.200000 0.346171 0.273992 1.619099
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_4
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.344450 -0.415958 21.152004 sawtooth 0 1 0.493579 0.755372
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 19.886318
AlphaGen wave sawtooth 0.777191 -0.463326 0.493579 0.755372
rgbGen wave sawtooth 1.000000 -0.301358 0.493579 0.755372
tcMod stretch sawtooth 0.200000 0.884664 0.493579 0.755372
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_5
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 1.366418 0.591261 27.499413 sawtooth 0 1 0.414158 1.023246
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 22.946714
AlphaGen wave sawtooth 0.692349 -0.551500 0.414158 1.023246
rgbGen wave sawtooth 1.000000 -0.448872 0.414158 1.023246
tcMod stretch sawtooth 0.200000 0.538365 0.414158 1.023246
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_6
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.777955 -1.489562 9.252100 sawtooth 0 1 0.345405 1.377401
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 22.987610
AlphaGen wave sawtooth 0.761931 -0.399628 0.345405 1.377401
rgbGen wave sawtooth 1.000000 -0.557591 0.345405 1.377401
tcMod stretch sawtooth 0.200000 0.467586 0.345405 1.377401
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_7
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 3.191568 3.936936 24.095812 sawtooth 0 1 0.363692 0.739084
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 11.371655
AlphaGen wave sawtooth 0.778802 -0.744633 0.363692 0.739084
rgbGen wave sawtooth 1.000000 -0.335746 0.363692 0.739084
tcMod stretch sawtooth 0.200000 0.883493 0.363692 0.739084
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_8
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -0.501696 -1.628014 12.020740 sawtooth 0 1 0.474755 1.002239
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 7.193060
AlphaGen wave sawtooth 0.986010 -0.877523 0.474755 1.002239
rgbGen wave sawtooth 1.000000 -0.302762 0.474755 1.002239
tcMod stretch sawtooth 0.200000 0.536778 0.474755 1.002239
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_9
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move 0.220373 1.662529 9.744644 sawtooth 0 1 0.331782 1.603317
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 10.222938
AlphaGen wave sawtooth 0.890231 -0.751653 0.331782 1.603317
rgbGen wave sawtooth 1.000000 -0.566344 0.331782 1.603317
tcMod stretch sawtooth 0.200000 0.483993 0.331782 1.603317
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke2_10
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -3.757527 1.067393 25.584791 sawtooth 0 1 0.496313 0.775437
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 23.487503
AlphaGen wave sawtooth 0.975402 -0.673757 0.496313 0.775437
rgbGen wave sawtooth 1.000000 -0.431635 0.496313 0.775437
tcMod stretch sawtooth 0.200000 0.509604 0.496313 0.775437
blendfunc add
}
}

// ***********************************************************
// *
// *  MopAn's Jail: particles smoke_pipe -> by MopAn, made with particle studio, thanx to the creators
// *
// ***********************************************************

textures/pad_jail/pad_jail_smoke_1
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -45.229370 -5.494783 -27.901167 sawtooth 0 1 0.737034 0.922323
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 15.116886
AlphaGen wave sawtooth 0.786114 -0.776775 0.737034 0.922323
rgbGen wave sawtooth 1.000000 -0.449348 0.737034 0.922323
tcMod stretch sawtooth 0.200000 1.107047 0.737034 0.922323
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_2
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -54.298782 -8.733896 -32.662361 sawtooth 0 1 0.442396 0.760873
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 15.932341
AlphaGen wave sawtooth 0.800201 -0.417988 0.442396 0.760873
rgbGen wave sawtooth 1.000000 -0.451045 0.442396 0.760873
tcMod stretch sawtooth 0.200000 0.733256 0.442396 0.760873
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_3
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -58.349571 -6.087270 -30.759367 sawtooth 0 1 0.531608 0.958856
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 14.005402
AlphaGen wave sawtooth 0.789984 -0.408161 0.531608 0.958856
rgbGen wave sawtooth 1.000000 -0.340251 0.531608 0.958856
tcMod stretch sawtooth 0.200000 0.933555 0.531608 0.958856
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_4
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -31.388313 1.587495 -18.587152 sawtooth 0 1 0.043873 1.296932
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 23.623005
AlphaGen wave sawtooth 0.747588 -0.427119 0.043873 1.296932
rgbGen wave sawtooth 1.000000 -0.407550 0.043873 1.296932
tcMod stretch sawtooth 0.200000 0.729936 0.043873 1.296932
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_5
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -40.811230 2.639762 -31.198219 sawtooth 0 1 0.092166 0.931574
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 18.643604
AlphaGen wave sawtooth 0.850215 -0.569628 0.092166 0.931574
rgbGen wave sawtooth 1.000000 -0.218445 0.092166 0.931574
tcMod stretch sawtooth 0.200000 0.984484 0.092166 0.931574
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_6
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -61.938358 3.989294 -39.098846 sawtooth 0 1 0.362291 0.733946
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 24.509262
AlphaGen wave sawtooth 0.813886 -0.544908 0.362291 0.733946
rgbGen wave sawtooth 1.000000 -0.494809 0.362291 0.733946
tcMod stretch sawtooth 0.200000 0.882455 0.362291 0.733946
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_7
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -63.940540 12.319346 -46.902851 sawtooth 0 1 0.039283 0.766914
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 17.220222
AlphaGen wave sawtooth 0.873031 -0.693851 0.039283 0.766914
rgbGen wave sawtooth 1.000000 -0.587158 0.039283 0.766914
tcMod stretch sawtooth 0.200000 1.028993 0.039283 0.766914
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_8
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -48.332302 -5.596929 -35.665512 sawtooth 0 1 0.089602 0.885820
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 7.847987
AlphaGen wave sawtooth 0.949059 -0.921702 0.089602 0.885820
rgbGen wave sawtooth 1.000000 -0.549754 0.089602 0.885820
tcMod stretch sawtooth 0.200000 1.325828 0.089602 0.885820
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_9
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -53.902626 -1.053432 -30.814821 sawtooth 0 1 0.419349 0.800503
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 21.010620
AlphaGen wave sawtooth 0.662697 -0.368780 0.419349 0.800503
rgbGen wave sawtooth 1.000000 -0.200342 0.419349 0.800503
tcMod stretch sawtooth 0.200000 0.681619 0.419349 0.800503
blendfunc add
}
}

textures/pad_jail/pad_jail_smoke_10
{
qer_editorimage textures/pad_jail/pad_jail_smoke
surfaceparm noimpact
surfaceparm nolightmap
cull none
surfaceparm trans
surfaceparm nonsolid
surfaceparm nodlight
deformvertexes autosprite
deformvertexes move -24.899492 2.065606 -19.649031 sawtooth 0 1 0.075369 1.531097
{
clampmap textures/pad_jail/pad_jail_smoke
tcMod rotate 23.264717
AlphaGen wave sawtooth 0.832759 -0.765484 0.075369 1.531097
rgbGen wave sawtooth 1.000000 -0.260146 0.075369 1.531097
tcMod stretch sawtooth 0.200000 1.086709 0.075369 1.531097
blendfunc add
}
}

