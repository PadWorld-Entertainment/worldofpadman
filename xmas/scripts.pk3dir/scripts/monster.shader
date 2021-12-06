models/wop_players/monster/mhead
{
     cull disable
        {
                map models/wop_players/monster/mnose.tga
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monster/mhead_red
{
     cull disable
        {
                map models/wop_players/monster/mnose.tga
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}

models/wop_players/monster/mhead_blue
{
     cull disable
        {
                map models/wop_players/monster/mnose_blue.tga
		rgbGen wave sin 1 .5 0 1
        }
        {
                map models/wop_players/monster/mhead.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
}