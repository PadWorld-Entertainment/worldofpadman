models/wop_players/monster/mhead
{
     cull disable
        {
                map models/wop_players/monster/mnose
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monster/mhead_red
{
     cull disable
        {
                map models/wop_players/monster/mnose
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monster/mhead_blue
{
     cull disable
        {
                map models/wop_players/monster/mnose_blue
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}