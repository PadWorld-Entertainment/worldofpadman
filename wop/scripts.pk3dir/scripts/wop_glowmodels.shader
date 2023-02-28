// =================
// PADMAN
// =================

models/wop_players/padman/padman_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/padman_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/padman_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/padman_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/padman_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/padman_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/padman_cape_light_s
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/padman_body_light
                blendFunc GL_ONE GL_ZERO
                rgbGen identity
        }
        {
                map models/wop_players/padman/padman_body_light
                blendFunc GL_ONE GL_ZERO
                rgbGen entity
        }
}


// =================
// PADROCK
// =================

models/wop_players/padman/rock_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/rock_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/rock_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/rock_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/rock_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/rock_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// SUPERPAD
// =================

models/wop_players/padman/super_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/super_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/super_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/super_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/super_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/super_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

// =================
// PAD-RA
// =================

models/wop_players/padman/ra_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/ra_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/ra_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/ra_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/ra_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/ra_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/ra_cape_light_s
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/ra_body_light
                blendFunc GL_ONE GL_ZERO
                alphaFunc ge128
                rgbGen identity
        }
        {
                map models/wop_players/padman/ra_body_light
                blendFunc GL_ONE GL_ZERO
                rgbGen entity
        }
}


// =================
// BIOPAD
// =================

models/wop_players/padman/bio_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/bio_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/bio_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/bio_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/bio_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/bio_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// STONEPAD
// =================

models/wop_players/padman/stone_head_light_s
{

    cull disable
        {
                map models/wop_players/padman/stonepad_head
                alphaFunc GE128
		rgbGen identity
        }
}

models/wop_players/padman/stone_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/stone_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/stone_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADSOLDIER
// =================

models/wop_players/padman/soldier_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padman/soldier_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padman/soldier_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padman/soldier_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padman/soldier_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padman/soldier_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// ================
// HELLPAD
// ================

models/wop_players/padman/hellpad_head_light_s
{
	nopicmip
	{		
		map models/wop_players/padman/hellpad_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc GE128		
        rgbGen entity	
    }
	{
		map models/wop_players/padman/hellpad_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc LT128		
        rgbGen identity	
    }		
}

models/wop_players/padman/hellpad_body_light_s
{
	nopicmip
	{
		map models/wop_players/padman/hellpad_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		rgbGen entity
	}
	{
		map models/wop_players/padman/hellpad_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc LT128
		rgbGen identity
	}	
}

models/wop_players/padman/hellpad_cape_light_s
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/hellpad_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen identity
		}
		{
		map models/wop_players/padman/hellpad_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen entity
        }
}


// =======================
// PADCLASSiC
// =======================

models/wop_players/padman/padclassic_head_light_s
{
	nopicmip
	{		
		map models/wop_players/padman/padclassic_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc GE128		
        rgbGen entity	
    }
	{
		map models/wop_players/padman/padclassic_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc LT128		
        rgbGen identity	
    }		
}

models/wop_players/padman/padclassic_body_light_s
{
	nopicmip
	{
		map models/wop_players/padman/padclassic_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		rgbGen entity
	}
	{
		map models/wop_players/padman/padclassic_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc LT128
		rgbGen identity
	}	
}

models/wop_players/padman/padclassic_cape_light_s
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/padclassic_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen identity
		}
		{
		map models/wop_players/padman/padclassic_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen entity
        }
}


// =======================
// PELViS
// =======================

models/wop_players/padman/pelvis_head_light_s
{
	nopicmip
	{		
		map models/wop_players/padman/pelvis_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc GE128		
        rgbGen entity	
    }
	{
		map models/wop_players/padman/pelvis_head_light		
        blendFunc GL_ONE GL_ZERO		
        alphaFunc LT128		
        rgbGen identity	
    }		
}

models/wop_players/padman/pelvis_body_light_s
{
	nopicmip
	{
		map models/wop_players/padman/pelvis_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc GE128
		rgbGen entity
	}
	{
		map models/wop_players/padman/pelvis_body_light
		blendFunc GL_ONE GL_ZERO
		alphaFunc LT128
		rgbGen identity
	}	
}

models/wop_players/padman/pelvis_cape_light_s
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/wop_players/padman/pelvis_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen identity
		}
		{
		map models/wop_players/padman/pelvis_body_light
		blendFunc GL_ONE GL_ZERO
		rgbGen entity
        }
}


// =================
// PADGiRL
// =================

models/wop_players/padgirl/padgirl_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padgirl/padgirl_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padgirl/padgirl_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padgirl/padgirl_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padgirl/padgirl_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padgirl/padgirl_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

// =================
// PADBABE
// =================

models/wop_players/padgirl/padbabe_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padgirl/padbabe_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padgirl/padbabe_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padgirl/padbabe_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padgirl/padbabe_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padgirl/padbabe_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADLILLY
// =================

models/wop_players/padlilly/lilly_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padlilly/lilly_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padlilly/lilly_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padlilly/lilly_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padlilly/lilly_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padlilly/lilly_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

// =================
// PADiLLA
// =================

models/wop_players/padlilly/dilla_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padlilly/dilla_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padlilly/dilla_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padlilly/dilla_body_light_s
              {
	nopicmip
              {
              map models/wop_players/padlilly/dilla_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padlilly/dilla_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// MONSTERPAD
// =================

models/wop_players/monster/monster_head_light_s
              {
	nopicmip
	{
              map models/wop_players/monster/monster_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/monster/monster_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/monster/monster_body_light_s
              {
	nopicmip
              {
              map models/wop_players/monster/monster_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/monster/monster_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// MONSTERHAWK
// =================

models/wop_players/monster/hawk_head_light_s
              {
	nopicmip
	{
              map models/wop_players/monster/hawk_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/monster/hawk_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/monster/hawk_body_light_s
              {
	nopicmip
              {
              map models/wop_players/monster/hawk_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/monster/hawk_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// FATPAD
// =================

models/wop_players/fatpad/fatpad_head_light_s
              {
	nopicmip
	{
              map models/wop_players/fatpad/fatpad_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/fatpad/fatpad_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/fatpad/fatpad_body_light_s
              {
	nopicmip
              {
              map models/wop_players/fatpad/fatpad_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/fatpad/fatpad_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/fatpad/fatpad_lower_light_s
              {
	nopicmip
              {
              map models/wop_players/fatpad/fatpad_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/fatpad/fatpad_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// FATTY
// =================

models/wop_players/fatpad/fatty_head_light_s
              {
	nopicmip
	{
              map models/wop_players/fatpad/fatty_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/fatpad/fatty_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/fatpad/fatty_body_light_s
              {
	nopicmip
              {
              map models/wop_players/fatpad/fatty_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/fatpad/fatty_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/fatpad/fatty_lower_light_s
              {
	nopicmip
              {
              map models/wop_players/fatpad/fatty_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/fatpad/fatty_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADPUNK
// =================

models/wop_players/padpunk/punk_head_light_s
{

    cull disable
        {
                map models/wop_players/padpunk/punk_head_light
                alphaFunc GE128
		rgbGen identity
        }
}

models/wop_players/padpunk/punk_torso_light_s
              {
	nopicmip
              {
              map models/wop_players/padpunk/punk_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padpunk/punk_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padpunk/punk_legs_light_s
              {
	nopicmip
              {
              map models/wop_players/padpunk/punk_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padpunk/punk_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADKING
// =================

models/wop_players/padking/king_head_light_s
              {
	nopicmip
	cull none
	{
              map models/wop_players/padking/king_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padking/king_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padking/king_lower_light_s
              {
	nopicmip
	cull none
              {
              map models/wop_players/padking/king_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padking/king_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padking/king_upper_light_s
              {
	nopicmip
	cull none
              {
              map models/wop_players/padking/king_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padking/king_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADKNIGHT
// =================

models/wop_players/padking/padknight_feather_light_s
{
	nopicmip
	cull none
	{
              map models/wop_players/padking/knight_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
        }
}

models/wop_players/padking/knight_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padking/knight_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padking/knight_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padking/knight_lower_light_s
              {
	nopicmip
	cull none
              {
              map models/wop_players/padking/knight_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padking/knight_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padking/knight_upper_light_s
              {
	nopicmip
	cull disable
              {
              map models/wop_players/padking/knight_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padking/knight_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// BEACHPAD
// =================

models/wop_players/beachpad/beach_head_light_s
              {
	nopicmip
	{
              map models/wop_players/beachpad/beach_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/beachpad/beach_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/beachpad/beach_body_light_s
              {
	nopicmip
              {
              map models/wop_players/beachpad/beach_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/beachpad/beach_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/beachpad/beach_barrel_light_s
              {
	nopicmip
              {
              map models/wop_players/beachpad/beach_barrel_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/beachpad/beach_barrel_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PADCHO
// =================

models/wop_players/padcho/padcho_head_light_s
              {
	nopicmip
	{
              map models/wop_players/padcho/padcho_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/padcho/padcho_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padcho/padcho_upper_light_s
              {
	nopicmip
              {
              map models/wop_players/padcho/padcho_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padcho/padcho_upper_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/padcho/padcho_lower_light_s
              {
	nopicmip
              {
              map models/wop_players/padcho/padcho_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/padcho/padcho_lower_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// PiRATEPAD
// =================

models/wop_players/piratepad/pirat_head_light_s
              {
	nopicmip
	{
              map models/wop_players/piratepad/pirat_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/piratepad/pirat_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/piratepad/pirat_legs_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/pirat_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/pirat_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/piratepad/pirat_torso_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/pirat_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/pirat_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

// =================
// GHOSTPiRATE
// =================

models/wop_players/piratepad/ghost_head_light_s
              {
	nopicmip
	{
              map models/wop_players/piratepad/ghost_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/piratepad/ghost_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/piratepad/ghost_legs_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/ghost_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/ghost_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/piratepad/ghost_torso_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/ghost_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/ghost_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}


// =================
// SPOOKY
// =================

models/wop_players/piratepad/spooky_head_light_s
              {
	nopicmip
	{
              map models/wop_players/piratepad/spooky_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/piratepad/spooky_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
              {
                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}


}

models/wop_players/piratepad/spooky_legs_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/spooky_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/spooky_legs_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
              {
                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}


}

models/wop_players/piratepad/spooky_torso_light_s
              {
	nopicmip
              {
              map models/wop_players/piratepad/spooky_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
              {
              map models/wop_players/piratepad/spooky_torso_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
              {
                map models/wop_players/piratepad/beam
                tcMod scroll -5 -.3
                tcgen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}


}


// =================
// PADDYBELL
// =================

models/wop_players/paddybell/default_head_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/default_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/default_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/default_body_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/default_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/default_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/default_leg_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/default_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/default_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/default_wings_glow_s
{
     cull none
        {
                map models/wop_players/paddybell/default_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}


// =================
// BADPADDY
// =================

models/wop_players/paddybell/badpaddy_head_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/badpaddy_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/badpaddy_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/badpaddy_body_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/badpaddy_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/badpaddy_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/badpaddy_leg_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/badpaddy_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/badpaddy_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/badpaddy_wings_glow_s
{
     cull none
        {
                map models/wop_players/paddybell/badpaddy_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}


// =================
// PADDYBEE
// =================

models/wop_players/paddybell/paddybee_head_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/paddybee_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/paddybee_head_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/paddybee_body_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/paddybee_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/paddybee_body_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/paddybee_leg_light_s
              {
	nopicmip
	{
              map models/wop_players/paddybell/paddybee_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc GE128
              rgbGen entity
              }
	{
              map models/wop_players/paddybell/paddybee_leg_light
              blendFunc GL_ONE GL_ZERO
              alphaFunc LT128
              rgbGen identity
              }
}

models/wop_players/paddybell/paddybee_wings_glow_s
{
     cull none
        {
                map models/wop_players/paddybell/paddybee_body
		blendfunc blend
		rgbGen lightingdiffuse
        }
}


