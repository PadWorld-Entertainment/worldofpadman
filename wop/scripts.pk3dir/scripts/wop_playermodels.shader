// =================
// PADMAN
// =================

models/wop_players/padman/default_goggles
{
     cull disable
        {
                map models/wop_players/padman/default_head
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx
		//map textures/effects/envmapdimb
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }

}

models/wop_players/padman/default_cape
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/default_body
                rgbGen identity
        }

}

models/wop_players/padman/default_head
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/default_head
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/default_head
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/default_body
{

	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/default_body
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/default_body
		alphaFunc ge128
		rgbGen lightingdiffuse
	}

}

models/wop_players/padman/body_blue
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/body_blue
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/body_blue
		alphaFunc ge128
		rgbGen lightingdiffuse
	}

}

models/wop_players/padman/cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_blue
                rgbGen identity
        }

}

models/wop_players/padman/body_red
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/body_red
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/body_red
		alphaFunc ge128
		rgbGen lightingdiffuse
	}

}


models/wop_players/padman/cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_red
                rgbGen identity
        }

}


// =================
// BiOPAD
// =================








// =================
// PAD-RA
// =================

models/wop_players/padman/padra_cape
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padra_body
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/padra_head
{

    cull disable
        {
                map models/wop_players/padman/padra_head
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padra_cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padra_body_blue
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/padra_head_blue
{

    cull disable
        {
                map models/wop_players/padman/padra_head_blue
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padra_cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padra_body_red
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/padra_head_red
{

    cull disable
        {
                map models/wop_players/padman/padra_head_red
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}


// =================
// PADROCK
// =================

models/wop_players/padman/padrock_goggles
{
     cull disable
        {
                map models/wop_players/padman/padrock_head
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
		tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
	}
}

models/wop_players/padman/padrock_goggles_blue
{
     cull disable
        {
                map models/wop_players/padman/padrock_head_blue
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }

}

models/wop_players/padman/padrock_goggles_red
{
     cull disable
        {
                map models/wop_players/padman/padrock_head_red
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }

}

models/wop_players/padman/null
{
	{
	map textures/colors/hex_000000ff
	blendFunc GL_ONE GL_ONE
	}
}


// =================
// PADSOLDiER
// =================

models/wop_players/padman/padsoldier_head
{

    cull disable
        {
                map models/wop_players/padman/padsoldier_head
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padsoldier_head_blue
{

    cull disable
        {
                map models/wop_players/padman/padsoldier_head_blue
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}


models/wop_players/padman/padsoldier_head_red
{

    cull disable
        {
                map models/wop_players/padman/padsoldier_head_red
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}


// =================
// STONEPAD
// =================

models/wop_players/padman/stonepad_head
{

    cull disable
        {
                map models/wop_players/padman/stonepad_head
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}


// =================
// SUPERPAD
// =================

models/wop_players/padman/superpad_goggles
{
     cull disable
        {
                map models/wop_players/padman/superpad_head
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
		tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
	}
}

models/wop_players/padman/superpad_head
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_head
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_body
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_body
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_cape
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/superpad_body
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/superpad_goggles_blue
{
     cull disable
        {
                map models/wop_players/padman/superpad_head_blue
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
		tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
	}
}

models/wop_players/padman/superpad_cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/superpad_body_blue
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/superpad_head_blue
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_head_blue
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head_blue
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_body_blue
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_body_blue
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body_blue
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_goggles_red
{
     cull disable
        {
                map models/wop_players/padman/superpad_head_red
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
		tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
	}
}

models/wop_players/padman/superpad_cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/superpad_body_red
		alphaFunc ge128
                rgbGen identity
        }

}

models/wop_players/padman/superpad_head_red
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_head_red
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head_red
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_body_red
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/superpad_body_red
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body_red
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

// ============
// HELLPAD
// ============

models/wop_players/padman/hellpad_goggles
{
	cull disable
	{
	map models/wop_players/padman/hellpad_head
	alphaFunc ge128
	rgbGen lightingDiffuse
	}
	{
        map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
        tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
    }
}

models/wop_players/padman/hellpad_goggles_blue
{
	cull disable
	{
		map models/wop_players/padman/hellpad_head_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
	}
	{
        map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
        tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
    }
}

models/wop_players/padman/hellpad_cape
{
    cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/hellpad_body
        rgbGen identity
	}
}

models/wop_players/padman/hellpad_cape_blue
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/hellpad_body_blue
        rgbGen identity
	}
}

models/wop_players/padman/hellpad_cape_red
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/hellpad_body_red
        rgbGen identity
	}
}

models/wop_players/padman/hellpad_head
{
	nopicmip
	nomipmaps
	cull none
    {
		map models/wop_players/padman/hellpad_head
		rgbGen lightingDiffuse
    }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
    }
    {
        map models/wop_players/padman/hellpad_head
		alphaFunc ge128
		rgbGen lightingDiffuse
    }
}

models/wop_players/padman/hellpad_head_blue
{
	nopicmip
	nomipmaps
	cull none
    {
		map models/wop_players/padman/hellpad_head_blue
		rgbGen lightingDiffuse
    }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
    }
    {
        map models/wop_players/padman/hellpad_head_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
    }
}


// ===================
// PADCLASSiC
// ===================

models/wop_players/padman/padclassic_goggles
{
    cull disable
    {
        map models/wop_players/padman/padclassic_head
		alphaFunc ge128
	    rgbGen lightingDiffuse
    }
	{
        map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
        tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
    }
}

models/wop_players/padman/padclassic_goggles_blue
{
    cull disable
    {
        map models/wop_players/padman/padclassic_head_blue
		alphaFunc ge128
	    rgbGen lightingDiffuse
    }
	{
        map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
        tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
    }
}

models/wop_players/padman/padclassic_goggles_red
{
    cull disable
    {
        map models/wop_players/padman/padclassic_head_red
		alphaFunc ge128
	    rgbGen lightingDiffuse
    }
	{
        map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
        tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
    }
}

models/wop_players/padman/padclassic_cape
{
    cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
    {
		map models/wop_players/padman/padclassic_body
        rgbGen identity
    }
}

models/wop_players/padman/padclassic_head
{
	nopicmip
	nomipmaps
	cull none
    {
		map models/wop_players/padman/padclassic_head
		rgbGen lightingDiffuse
    }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
    }
    {
        map models/wop_players/padman/padclassic_head
		alphaFunc ge128
		rgbGen lightingDiffuse
    }
}

models/wop_players/padman/padclassic_head_blue
{
	nopicmip
	nomipmaps
	cull none
    {
		map models/wop_players/padman/padclassic_head_blue
		rgbGen lightingDiffuse
    }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
    }
    {
        map models/wop_players/padman/padclassic_head_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
    }
}

models/wop_players/padman/padclassic_head_red
{
	nopicmip
	nomipmaps
	cull none
    {
		map models/wop_players/padman/padclassic_head_red
		rgbGen lightingDiffuse
    }
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
    }
    {
        map models/wop_players/padman/padclassic_head_red
		alphaFunc ge128
		rgbGen lightingDiffuse
    }
}

models/wop_players/padman/padclassic_body
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/padclassic_body
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/padclassic_body
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padman/padclassic_body_blue
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/padclassic_body_blue
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/padclassic_body_blue
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padman/padclassic_cape_blue
{
    cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
    {
        map models/wop_players/padman/padclassic_body_blue
        rgbGen identity
    }
}

models/wop_players/padman/padclassic_body_red
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/padclassic_body_red
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad4
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/padclassic_body_red
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padman/padclassic_cape_red
{
    cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
    {
        map models/wop_players/padman/padclassic_body_red
        rgbGen identity
    }
}


// =================
// MONSTERHAWK
// =================

models/wop_players/monster/monsterhawk_body
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_body
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_body_blue
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_body_blue
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_body_red
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_body_red
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_head
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_head
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_head_blue
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_head_blue
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_head_red
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_head_red
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}


// =================
// MONSTERHAWK NEW
// =================
//
//models/wop_players/monsterpad/monsterhawk_upper
//{
//     cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_upper
//		alphaFunc ge128
//		rgbGen lightingDiffuse
//        }
//
//}
//
//
//models/wop_players/monsterpad/monsterhawk_upper_blue
//{
//     	cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_upper_blue
//		alphaFunc ge128
//                rgbGen lightingDiffuse
//        }
//
//}
//
//
//models/wop_players/monsterpad/monsterhawk_upper_red
//{
//     cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_upper_red
//		alphaFunc ge128
//     	        rgbGen lightingDiffuse
//        }
//
//}
//models/wop_players/monsterpad/monsterhawk_head
//{
//     	cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_head
//		alphaFunc ge128
//                rgbGen lightingDiffuse
//        }
//
//}
//
//
//
//models/wop_players/monsterpad/monsterhawk_head_blue
//{
//     cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_head_blue
//		alphaFunc ge128
//                rgbGen lightingDiffuse
//        }
//
//}
//
//models/wop_players/monsterpad/monsterhawk_head_red
//{
//     	cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_head_red
//		alphaFunc ge128
//                rgbGen lightingDiffuse
//        }
//
//}


// =================
// PADKING
// =================

models/wop_players/padking/default_head
{
     cull disable
        {
                map models/wop_players/padking/default_head
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/default_lower
{
     cull disable
        {
                map models/wop_players/padking/default_lower
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/default_upper
{
     cull disable
        {
                map models/wop_players/padking/default_upper
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/head_red
{
     cull disable
        {
                map models/wop_players/padking/head_red
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/lower_red
{
     cull disable
        {
                map models/wop_players/padking/lower_red
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/upper_red
{
     cull disable
        {
                map models/wop_players/padking/upper_red
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/head_blue
{
     cull disable
        {
                map models/wop_players/padking/head_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/lower_blue
{
     cull disable
        {
                map models/wop_players/padking/lower_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/upper_blue
{
     cull disable
        {
                map models/wop_players/padking/upper_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}


// =================
// PADKNIGHT
// =================

models/wop_players/padking/head_padknight
{
     cull disable
        {
                map models/wop_players/padking/head_padknight
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/padknight_feather
{
     cull none
        {
                map models/wop_players/padking/head_padknight
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/head_padknight
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }

}

models/wop_players/padking/lower_padknight
{
     cull disable
        {
                map models/wop_players/padking/lower_padknight
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/upper_padknight
{
     cull disable
        {
                map models/wop_players/padking/upper_padknight
		rgbGen lightingDiffuse
        }

}


models/wop_players/padking/padknight_head_red
{
     cull disable
        {
                map 	models/wop_players/padking/padknight_head_red
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/padknight_feather_red
{
     cull none
        {
                map models/wop_players/padking/padknight_head_red
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/padknight_head_red
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }

}

models/wop_players/padking/padknight_lower_red
{
     cull disable
        {
                map models/wop_players/padking/padknight_lower_red
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/padknight_upper_red
{
     cull disable
        {
                map models/wop_players/padking/padknight_upper_red
		rgbGen lightingDiffuse
        }

}


models/wop_players/padking/padknight_head_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_head_blue
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/padknight_feather_blue
{
     cull none
        {
                map models/wop_players/padking/padknight_head_blue
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/padknight_head_blue
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }

}

models/wop_players/padking/padknight_lower_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_lower_blue
		rgbGen lightingDiffuse
        }

}

models/wop_players/padking/padknight_upper_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_upper_blue
		rgbGen lightingDiffuse
        }

}


// =================
// PADPUNK
// =================

models/wop_players/padpunk/default_goggles
{
     cull disable
        {
                map models/wop_players/padpunk/default_legs
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }

}


// =================
// PADCHO
// =================

models/wop_players/padcho/lower_default
{
     cull disable
        {
                map models/wop_players/padcho/lower_default
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padcho/lower_red
{
     cull disable
        {
                map models/wop_players/padcho/lower_red
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}

models/wop_players/padcho/lower_blue
{
     cull disable
        {
                map models/wop_players/padcho/lower_blue
		alphaFunc ge128
		rgbGen lightingDiffuse
        }

}


// =================
// PIRATEPAD
// =================

models/wop_players/piratepad/spooky_head_default
{
     cull disable
        {
                map models/wop_players/piratepad/spooky_head_default
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}


}

models/wop_players/piratepad/spooky_head_red
{
     cull disable
        {
                map models/wop_players/piratepad/spooky_head_red
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_head_blue
{
     cull disable
        {
                map models/wop_players/piratepad/spooky_head_blue
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_torso_default
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_torso_default
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_torso_red
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_torso_red
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_torso_blue
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_torso_blue
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_legs_default
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_legs_default
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_legs_red
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_legs_red
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/wop_players/piratepad/spooky_legs_blue
{

    cull disable
        {
                map models/wop_players/piratepad/spooky_legs_blue
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {

                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}


// =================
// PADBABE
// =================

models/wop_players/padgirl/padbabebody
{

	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padgirl/padbabebody
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabebody
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padbabe_blue_body
{
              cull disable

	{
		map models/wop_players/padgirl/padbabe_blue_body
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabe_blue_body
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padbabe_red_body
{
	nopicmip
	nomipmaps

	{
		map models/wop_players/padgirl/padbabe_red_body
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabe_red_body
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}


// =================
// PADGiRL
// =================

models/wop_players/padgirl/padgrlbd
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padgirl/padgrlbd
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padgrlbd_blue
{
        cull disable

	{
		map models/wop_players/padgirl/padgrlbd_blue
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd_blue
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padgrlbd_red
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/padgirl/padgrlbd_red
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd_red
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}


// =================
// PADDYBELL
// =================

models/wop_players/paddybell/default_wings
{
     cull none
        {
                map models/wop_players/paddybell/default_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/default_wings_red
{
     cull none
        {
                map models/wop_players/paddybell/default_body_red
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/default_wings_blue
{
     cull none
        {
                map models/wop_players/paddybell/default_body_blue
		blendfunc blend
		rgbGen lightingdiffuse
        }
}


// =================
// BADPADDY
// =================

models/wop_players/paddybell/badpaddy_wings
{
     cull none
        {
                map models/wop_players/paddybell/badpaddy_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/badpaddy_wings_blue
{
     cull none
        {
                map models/wop_players/paddybell/badpaddy_body_blue
		blendfunc blend
		rgbGen lightingdiffuse
        }
}


// =================
// PADDYBEE
// =================

models/wop_players/paddybell/paddybee_head
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/paddybell/paddybee_head
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}


models/wop_players/paddybell/paddybee_head_red
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/paddybell/paddybee_head_red
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head_red
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/paddybell/paddybee_head_blue
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/paddybell/paddybee_head_blue
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head_blue
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/paddybell/paddybee_wings
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/paddybee_wings_red
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body_red
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/paddybee_wings_blue
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body_blue
		blendfunc blend
		rgbGen lightingdiffuse
        }
}