// =================
// CHRISTMAS HATS
// =================

models/hats/elvehat
{	
	cull none
	{
                map models/hats/elvehat.tga
		rgbGen lightingDiffuse
        }
	
}

models/hats/reindeer
{	
		
	{
                map models/hats/reindeer.tga
		rgbGen lightingDiffuse
        }
	
}

models/hats/halo
{	
	cull none	
	{
                map models/hats/halo.tga
		blendfunc GL_ONE GL_ONE
		blendfunc add
        }
	
}

models/hats/irish
{	
		
	{
                map models/hats/irish.tga
		rgbGen lightingDiffuse
        }
	
}

models/hats/tophat
{	
		
	{
                map models/hats/tophat.tga
		rgbGen lightingDiffuse
        }
	
}

models/hats/nikolaus
{	
	cull none	
	{
                map models/hats/nikolaus.tga
		rgbGen lightingDiffuse
        }
	
}

// with alpha

models/hats/hking1
{	
	cull disable
        {
                map models/hats/hking1.tga
		alphaFunc ge128
	        rgbGen lightingDiffuse
        }
	{
                //map models/wop_players/padman/pad_fx.tga
		map textures/pad_gfx02/tinpad.tga
		blendfunc GL_DST_ALPHA GL_DST_ALPHA
                tcgen environment
		rgbGen lightingDiffuse
		depthfunc equal
        }
	
}