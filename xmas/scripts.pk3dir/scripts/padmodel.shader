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
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/padman/cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_blue
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}

models/wop_players/padman/cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_red
		alphaFunc ge128
                rgbGen lightingDiffuse
        }

}
