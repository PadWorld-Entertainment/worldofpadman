models/wop_players/monsterpad/mhead
{
     cull disable
        {
                map models/wop_players/monsterpad/mnose
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monsterpad/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monsterpad/mhead_red
{
     cull disable
        {
                map models/wop_players/monsterpad/mnose
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monsterpad/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monsterpad/mhead_blue
{
     cull disable
        {
                map models/wop_players/monsterpad/mnose_blue
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monsterpad/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}