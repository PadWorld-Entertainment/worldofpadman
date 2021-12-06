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
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	
}

models/wop_players/padman/cape_blue
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_blue.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	
}

models/wop_players/padman/cape_red
{
     	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
        {
                map models/wop_players/padman/body_red.tga
		alphaFunc ge128
                rgbGen lightingDiffuse
        }
	
}
