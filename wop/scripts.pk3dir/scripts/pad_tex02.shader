textures/pad_tex02/padjump_high
{
	qer_editorimage textures/pad_tex02/padjump_high.tga
	{
		map textures/pad_tex02/padjump_high.tga
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/pad_tex02/padjump_light03.tga
		blendfunc gl_one gl_one	
		rgbgen wave inversesawtooth 0 1 0 1
	}
}

textures/pad_tex02/padjump_highred
{
	qer_editorimage textures/pad_tex02/padjump_highred.tga
	{
		map textures/pad_tex02/padjump_highred.tga
		rgbGen identity
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}
	{
		map textures/pad_tex02/padjump_light01.tga
		blendfunc gl_one gl_one	
		rgbgen wave inversesawtooth 0 1 0 1
	}
}

textures/pad_tex02/telepad
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_tex02/telepad.tga
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}

textures/pad_tex02/padjump_fly
{

	
	{
		map $lightmap
		rgbGen identity
	}

	{ 
		map textures/pad_tex02/padjump_fly.tga
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}

	{	
		map textures/pad_tex02/padjump_light02b.tga
		blendfunc gl_one gl_one	
		rgbgen wave inversesawtooth 0 1 0 1	
	}

	{ 
		animmap 4 textures/pad_tex02/padjump_start.tga textures/pad_tex02/launchpad_arrow2.tga textures/pad_tex02/launchpad_arrow2.tga textures/pad_tex02/launchpad_arrow2.tga
		blendfunc gl_one gl_one
		tcmod scroll 0 2
	}

}

textures/pad_tex02/padjump_flyred
{
	
	{
		map $lightmap
		rgbGen identity
	}

	{ 
		map textures/pad_tex02/padjump_flyred.tga
		rgbGen identity
		blendfunc gl_dst_color gl_zero
	}

	{	
		map textures/pad_tex02/padjump_light02.tga
		blendfunc gl_one gl_one	
		rgbgen wave inversesawtooth 0 1 0 1	
	}


	{ 
		animmap 4 textures/pad_tex02/padjump_start.tga textures/pad_tex02/launchpad_arrow2.tga textures/pad_tex02/launchpad_arrow2.tga textures/pad_tex02/launchpad_arrow2.tga
		blendfunc gl_one gl_one
		tcmod scroll 0 2
	}

}

textures/pad_tex02/padani
{


	
	surfaceparm nodamage
	q3map_surfacelight 200

	
	{
		map textures/pad_tex02/padani.tga
		rgbGen identity
	}
	
	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}
	
	{
		animMap 2 textures/pad_tex02/padani_log.tga textures/pad_tex02/padani_log02.tga textures/pad_tex02/padani_log03.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}
	
}

textures/pad_tex02/padani_blue
{

	
	surfaceparm nodamage
	q3map_surfacelight 200

	
	{
		map textures/pad_tex02/padani_blue.tga
		rgbGen identity
	}
	
	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}
	
	{
		animMap 2 textures/pad_tex02/padani_log.tga textures/pad_tex02/padani_log02.tga textures/pad_tex02/padani_log03.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}
	
}



textures/pad_tex02/padani_red
{


	
	surfaceparm nodamage
	q3map_surfacelight 200

	
	{
		map textures/pad_tex02/padani_red.tga
		rgbGen identity
	}
	
	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}
	
	{
		animMap 2 textures/pad_tex02/padani_log.tga textures/pad_tex02/padani_log02.tga textures/pad_tex02/padani_log03.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}
	
}

textures/pad_tex02/padlolly_red
{

	qer_editorimage textures/pad_tex02/padani_red.tga	
	surfaceparm nodamage
	q3map_surfacelight 200

	
	{
		map textures/pad_tex02/padani_red.tga
		rgbGen identity
	}
	
	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}
	
	{
		animMap 2 textures/pad_tex02/padani_flaglog01.tga textures/pad_tex02/padani_flaglog03.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}
	
}

textures/pad_tex02/padlolly_blue
{
	qer_editorimage textures/pad_tex02/padani_blue.tga	
	surfaceparm nodamage
	q3map_surfacelight 200

	
	{
		map textures/pad_tex02/padani_blue.tga
		rgbGen identity
	}
	
	{
		map $lightmap
		blendfunc gl_dst_color gl_zero
		rgbGen identity
	}
	
	{
		animMap 2 textures/pad_tex02/padani_flaglog04.tga textures/pad_tex02/padani_flaglog02.tga
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth 0 1 0 2
	}
	
}


textures/pad_tex02/padbox3
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_tex02/padbox_yel.tga
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

        skyparms env/padbox3 - -
       {
		map textures/pad_tex02/skystuff3.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.03
		tcMod scale 1 2
	}
 {
		map textures/pad_tex02/skystuff3b.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}


textures/pad_tex02/padbox4
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_tex02/padbox_blu.tga
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

        skyparms env/padbox4 - -
       {
		map textures/pad_tex02/skystuff4.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.02 0.04
		tcMod scale 1 2
	}
}


textures/pad_tex02/padbox5
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_tex02/padbox_ora.tga
	q3map_sun	1 1 1 190 -58 90
	q3map_surfacelight 200

        skyparms env/padbox5 - -
       {
		map textures/pad_tex02/skystuff2.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.02 0.04
		tcMod scale 1 2
	}
 {
		map textures/pad_tex02/skystuff2b.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}


textures/pad_tex02/padbox6
{
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
        q3map_lightimage textures/pad_tex02/padbox_yel.tga
	q3map_sun	1 1 1 180 -58 90
	q3map_surfacelight 200

        skyparms env/padbox6 - -
       {
		map textures/pad_tex02/skystuff6.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll 0.01 0.03
		tcMod scale 1 2
	}
 {
		map textures/pad_tex02/skystuff6b.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.01 -0.03
		tcMod scale 1 2
	}
}


textures/pad_maps/schrank01d
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/schrank01d.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_maps/burgfloor001
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_maps/burgfloor001.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
} 

textures/pad_maps/woptrailer
{

    qer_editorimage textures/pad_maps/pad_movie.tga
    surfaceparm trans
    surfaceparm nolightmap
    cull disable

        {
		map textures/pad_gfx02/tinpad3.tga
        tcGen environment
        rgbGen identity
	}

	{
		videoMap wopintro512.roq
		blendFunc add
        rgbGen identity
	}
       
}


textures/pad_maps/padlamp01
{
	qer_editorimage textures/pad_maps/padlamp01.tga
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp01.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/padlamp02
{
	qer_editorimage textures/pad_maps/padlamp02.tga
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp02.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/padlamp03
{
	qer_editorimage textures/pad_maps/padlamp03.tga
	q3map_surfacelight 1000
              q3map_flareShader flareShader
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp03.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/padlamp_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/weaplight_blue
{
	qer_editorimage textures/pad_maps/weaplight_blue.tga
	q3map_surfacelight 150
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplight_blue.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplamp_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/weaplight_red
{
	qer_editorimage textures/pad_maps/weaplight_red.tga
	q3map_surfacelight 150
	//	light 1
	surfaceparm nomarks
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplight_red.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
	{
		map textures/pad_maps/weaplamp_flash.tga
		//tcMod scale 0.5 0.5
		blendfunc GL_ONE GL_ONE
	}
}


textures/pad_maps/blobpad02
{
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map textures/pad_maps/blobpad02.tga
		tcGen environment
                tcMod turb 0 0.25 0 0.5
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}

}


textures/pad_tex02/jumploop1
{
	surfaceparm nomarks
	surfaceparm nolightmap
	cull none
	surfaceparm nonsolid
              surfaceparm trans
	q3map_lightimage textures/pad_tex02/jumploop1.tga	
	q3map_surfacelight 100

	{
		map textures/pad_tex02/jumploop2.tga
		blendFunc add
                	rgbgen wave triangle 1.8 2 0 7
                	tcMod scroll 0 0.5
	}
	{
		map textures/pad_tex02/jumploop2.tga
		blendFunc add
                	rgbgen wave triangle 1 1.1 1 5
                	tcMod scale  -1 1.5
                	tcMod scroll 0 1.1
	}
	{
		map textures/pad_tex02/jumploop1.tga
		blendFunc add
                	rgbgen wave triangle 2 1.4 3 7.7
                	tcMod scroll 0 1.5
	}
	
}


textures/pad_tex02/padbase
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/padbase.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num01
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num01.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num02
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num02.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num03
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num03.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num04
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num04.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num05
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num05.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/num06
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/num06.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/arrow01
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/arrow01.tga
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_tex02/arrow02
{    
     surfaceparm	nomarks   
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_tex02/arrow02.tga
               		blendFunc add
		rgbGen vertex
	}
}



textures/pad_tex02/blueflag
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_tex02/blueflag.tga
                alphaFunc GE128
		depthWrite
		rgbGen vertex
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}


textures/pad_tex02/redflag
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans	
     surfaceparm nomarks
     tessSize 64
     deformVertexes wave 30 sin 0 3 0 .2
     deformVertexes wave 100 sin 0 3 0 .7
     
        {
                map textures/pad_tex02/redflag.tga
                alphaFunc GE128
		depthWrite
		rgbGen vertex
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}


}

