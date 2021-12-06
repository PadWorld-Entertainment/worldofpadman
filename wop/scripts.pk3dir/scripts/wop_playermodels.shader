// =================
// PADMAN
// =================

models/wop_players/padman/default_goggles
{
     cull disable
        {
                map models/wop_players/padman/default_head.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx.tga
		//map textures/effects/envmapdimb.tga
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
                map models/wop_players/padman/default_body.tga
                rgbGen identity
        }
	
}

models/wop_players/padman/default_head
{
	nopicmip
	nomipmaps
	cull none
        {
                map models/wop_players/padman/default_head.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/default_head.tga
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
		map models/wop_players/padman/default_body.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/default_body.tga
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
		map models/wop_players/padman/body_blue.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/body_blue.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
	
}

models/wop_players/padman/cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_blue.tga
                rgbGen identity
        }
	
}

models/wop_players/padman/body_red
{
	nopicmip
	nomipmaps
	cull none
	{
		map models/wop_players/padman/body_red.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
 		map models/wop_players/padman/body_red.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
	
}


models/wop_players/padman/cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_red.tga
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
                map models/wop_players/padman/padra_body.tga
		alphaFunc ge128
                rgbGen identity
        }
	
}

models/wop_players/padman/padra_head
{
    
    cull disable
        {
                map models/wop_players/padman/padra_head.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padra_cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padra_body_blue.tga
		alphaFunc ge128
                rgbGen identity
        }
	
}

models/wop_players/padman/padra_head_blue
{
    
    cull disable
        {
                map models/wop_players/padman/padra_head_blue.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padra_cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padra_body_red.tga
		alphaFunc ge128
                rgbGen identity
        }
	
}

models/wop_players/padman/padra_head_red
{
    
    cull disable
        {
                map models/wop_players/padman/padra_head_red.tga
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
                map models/wop_players/padman/padrock_head.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padman/padrock_head_blue.tga
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padman/padrock_head_red.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx.tga
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }

}


// =================
// PADSOLDiER
// =================

models/wop_players/padman/padsoldier_head
{
    
    cull disable
        {
                map models/wop_players/padman/padsoldier_head.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/padsoldier_head_blue
{
    
    cull disable
        {
                map models/wop_players/padman/padsoldier_head_blue.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}


models/wop_players/padman/padsoldier_head_red
{
    
    cull disable
        {
                map models/wop_players/padman/padsoldier_head_red.tga
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
                map models/wop_players/padman/stonepad_head.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/stonepad_head_blue
{
    
    cull disable
        {
                map models/wop_players/padman/stonepad_head_blue.tga
                alphaFunc GE128
		rgbGen lightingdiffuse
        }
}

models/wop_players/padman/stonepad_head_red
{
    
    cull disable
        {
                map models/wop_players/padman/stonepad_head_red.tga
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
                map models/wop_players/padman/superpad_head.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padman/superpad_head.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head.tga
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
                map models/wop_players/padman/superpad_body.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body.tga
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_cape
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/superpad_body.tga
		alphaFunc ge128
                rgbGen identity
        }
	
}

models/wop_players/padman/superpad_goggles_blue
{
     cull disable
        {
                map models/wop_players/padman/superpad_head_blue.tga
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padman/superpad_body_blue.tga
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
                map models/wop_players/padman/superpad_head_blue.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head_blue.tga
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
                map models/wop_players/padman/superpad_body_blue.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body_blue.tga
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}

models/wop_players/padman/superpad_goggles_red
{
     cull disable
        {
                map models/wop_players/padman/superpad_head_red.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
        {
		map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padman/superpad_body_red.tga
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
                map models/wop_players/padman/superpad_head_red.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_head_red.tga
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
                map models/wop_players/padman/superpad_body_red.tga
	  //alphaFunc ge128
	  rgbGen lightingDiffuse
        }
	{
		map textures/pad_gfx02/tinpad4.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
        }
        {
                map models/wop_players/padman/superpad_body_red.tga
	  alphaFunc ge128
	  rgbGen lightingDiffuse
        }
}


// =================
// MONSTERHAWK
// =================

models/wop_players/monster/monsterhawk_body
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_body.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/monster/monsterhawk_body_blue
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_body_blue.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	
}

models/wop_players/monster/monsterhawk_body_red
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_body_red.tga
		alphaFunc ge128
     	        rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_head
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_head.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	
}

models/wop_players/monster/monsterhawk_head_blue
{
     cull disable
        {
                map models/wop_players/monster/monsterhawk_head_blue.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/monster/monsterhawk_head_red
{
     	cull disable
        {
                map models/wop_players/monster/monsterhawk_head_red.tga
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
//                map models/wop_players/monsterpad/monsterhawk_upper.tga
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
//                map models/wop_players/monsterpad/monsterhawk_upper_blue.tga
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
//                map models/wop_players/monsterpad/monsterhawk_upper_red.tga
//		alphaFunc ge128
//     	        rgbGen lightingDiffuse
//        }
//
//}
//models/wop_players/monsterpad/monsterhawk_head
//{
//     	cull disable
//        {
//                map models/wop_players/monsterpad/monsterhawk_head.tga
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
//                map models/wop_players/monsterpad/monsterhawk_head_blue.tga
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
//                map models/wop_players/monsterpad/monsterhawk_head_red.tga
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
                map models/wop_players/padking/default_head.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/default_lower
{
     cull disable
        {
                map models/wop_players/padking/default_lower.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/default_upper
{
     cull disable
        {
                map models/wop_players/padking/default_upper.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/head_red
{
     cull disable
        {
                map models/wop_players/padking/head_red.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/lower_red
{
     cull disable
        {
                map models/wop_players/padking/lower_red.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/upper_red
{
     cull disable
        {
                map models/wop_players/padking/upper_red.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/head_blue
{
     cull disable
        {
                map models/wop_players/padking/head_blue.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/lower_blue
{
     cull disable
        {
                map models/wop_players/padking/lower_blue.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/upper_blue
{
     cull disable
        {
                map models/wop_players/padking/upper_blue.tga
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
                map models/wop_players/padking/head_padknight.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/padknight_feather
{
     cull none
        {
                map models/wop_players/padking/head_padknight.tga
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/head_padknight.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }
 
}

models/wop_players/padking/lower_padknight
{
     cull disable
        {
                map models/wop_players/padking/lower_padknight.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/upper_padknight
{
     cull disable
        {
                map models/wop_players/padking/upper_padknight.tga
		rgbGen lightingDiffuse
        }
 
}


models/wop_players/padking/padknight_head_red
{
     cull disable
        {
                map 	models/wop_players/padking/padknight_head_red.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/padknight_feather_red
{
     cull none
        {
                map models/wop_players/padking/padknight_head_red.tga
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/padknight_head_red.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }
 
}

models/wop_players/padking/padknight_lower_red
{
     cull disable
        {
                map models/wop_players/padking/padknight_lower_red.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/padknight_upper_red
{
     cull disable
        {
                map models/wop_players/padking/padknight_upper_red.tga
		rgbGen lightingDiffuse
        }
 
}


models/wop_players/padking/padknight_head_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_head_blue.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/padknight_feather_blue
{
     cull none
        {
                map models/wop_players/padking/padknight_head_blue.tga
		alphaFunc GE128
		rgbGen identityLighting
        }
        {
                map models/wop_players/padking/padknight_head_blue.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//rgbGen lightingDiffuse
		rgbGen identityLighting
        }
 
}

models/wop_players/padking/padknight_lower_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_lower_blue.tga
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padking/padknight_upper_blue
{
     cull disable
        {
                map models/wop_players/padking/padknight_upper_blue.tga
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
                map models/wop_players/padpunk/default_legs.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
	{
                map models/wop_players/padman/pad_fx.tga
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
                map models/wop_players/padcho/lower_default.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padcho/lower_red
{
     cull disable
        {
                map models/wop_players/padcho/lower_red.tga
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
 
}

models/wop_players/padcho/lower_blue
{
     cull disable
        {
                map models/wop_players/padcho/lower_blue.tga
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
                map models/wop_players/piratepad/spooky_head_default.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_head_red.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_head_blue.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_torso_default.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_torso_red.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_torso_blue.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_legs_default.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_legs_red.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
                map models/wop_players/piratepad/spooky_legs_blue.tga
	blendfunc GL_ONE GL_ZERO
		alphaFunc ge128
		rgbGen lightingDiffuse
        }
        {
		
                map models/wop_players/piratepad/beam.tga
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
		map models/wop_players/padgirl/padbabebody.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabebody.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padbabe_blue_body
{
              cull disable

	{
		map models/wop_players/padgirl/padbabe_blue_body.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabe_blue_body.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padbabe_red_body
{
	nopicmip
	nomipmaps

	{
		map models/wop_players/padgirl/padbabe_red_body.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padbabe_red_body.tga
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
		map models/wop_players/padgirl/padgrlbd.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}

	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
		rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padgrlbd_blue
{
        cull disable

	{
		map models/wop_players/padgirl/padgrlbd_blue.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd_blue.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/padgirl/padgrlbd_red
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/padgirl/padgrlbd_red.tga
		//alphaFunc ge128
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                            rgbGen lightingdiffuse
	}
	{
		map models/wop_players/padgirl/padgrlbd_red.tga
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
                map models/wop_players/paddybell/default_body.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/default_wings_red
{
     cull none
        {
                map models/wop_players/paddybell/default_body_red.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/default_wings_blue
{
     cull none
        {
                map models/wop_players/paddybell/default_body_blue.tga
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
                map models/wop_players/paddybell/badpaddy_body.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/badpaddy_wings_blue
{
     cull none
        {
                map models/wop_players/paddybell/badpaddy_body_blue.tga
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
		map models/wop_players/paddybell/paddybee_head.tga
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}


models/wop_players/paddybell/paddybee_head_red
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/paddybell/paddybee_head_red.tga
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head_red.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/paddybell/paddybee_head_blue
{
	nopicmip
	nomipmaps
	{
		map models/wop_players/paddybell/paddybee_head_blue.tga
		rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/tinpad3a.tga
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
                rgbGen lightingdiffuse
	}
	{
		map models/wop_players/paddybell/paddybee_head_blue.tga
		alphaFunc ge128
		rgbGen lightingdiffuse
	}
}

models/wop_players/paddybell/paddybee_wings
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/paddybee_wings_red
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body_red.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}

models/wop_players/paddybell/paddybee_wings_blue
{
     cull disable
        {
                map models/wop_players/paddybell/paddybee_body_blue.tga
		blendfunc blend
		rgbGen lightingdiffuse
        }
}