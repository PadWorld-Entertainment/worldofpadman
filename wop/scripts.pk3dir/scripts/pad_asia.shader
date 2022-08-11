//==============================
//lights
//==============================

textures/pad_teehouse/reispaper_512
{
    q3map_surfacelight 80
    surfaceparm nolightmap
    {
        map textures/pad_teehouse/reispaper_512
    }
}

textures/pad_teehouse/paper_yellow
{
    q3map_surfacelight 70
    surfaceparm nolightmap
    {
        map textures/pad_teehouse/paper_yellow
    }
}



//=================================
//steps
//=================================

textures/pad_teehouse/bambusfloor
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/bambusfloor
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_teehouse/shogi02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/shogi02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/shogi03
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/shogi03
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/wood_black
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/wood_black
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/wood01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/wood01
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tee_untersetzer
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tee_untersetzer
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tatami_border_red_dark
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tatami_border_red_dark
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tatami_border_red
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tatami_border_red
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tatami_border_blue_dark
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tatami_border_blue_dark
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tatami_border_blue
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tatami_border_blue
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/tatami_border_green
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/tatami_border_green
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/geflecht_blue
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/geflecht_blue
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/geflecht_red
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/geflecht_red
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/red_border
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/red_border
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_teehouse/flower_red2
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_red2
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/flower_blue2
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_blue2
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/flower_blue3
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_blue3
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/flower_red3
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_red3
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/flower_red4
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_red4
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_teehouse/flower_blue4
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/flower_blue4
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

//===================================
//shiny
//===================================

textures/pad_teehouse/art04
{
	qer_editorimage textures/pad_teehouse/art04
        surfaceparm woodsteps
	{
		map textures/pad_teehouse/art04
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_teehouse/art06
{
	qer_editorimage textures/pad_teehouse/art06
        surfaceparm woodsteps
	{
		map textures/pad_teehouse/art06
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3a
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_teehouse/cupboarder_red
{
	qer_editorimage textures/pad_teehouse/cupboarder_red
	{
		map textures/pad_teehouse/cupboarder_red
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_teehouse/cupboarder2_red
{
	qer_editorimage textures/pad_teehouse/cupboarder2_red
	{
		map textures/pad_teehouse/cupboarder2_red
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_teehouse/cupboarder_blue
{
	qer_editorimage textures/pad_teehouse/cupboarder_blue
	{
		map textures/pad_teehouse/cupboarder_blue
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_teehouse/cupboarder2_blue
{
	qer_editorimage textures/pad_teehouse/cupboarder2_blue
	{
		map textures/pad_teehouse/cupboarder2_blue
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}


textures/pad_teehouse/flower_red
{
	qer_editorimage textures/pad_teehouse/flower_red
	{
		map textures/pad_teehouse/flower_red
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

textures/pad_teehouse/flower_blue
{
	qer_editorimage textures/pad_teehouse/flower_blue
	{
		map textures/pad_teehouse/flower_blue
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen identity
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
	}
}

//===============================
//alphas
//===============================

textures/pad_teehouse/bambuspinsel
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_teehouse/bambuspinsel
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}

}


textures/pad_teehouse/kirschbluete
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_teehouse/kirschbluete
                alphaFunc GE128
		depthWrite
		rgbGen identity
        }
        {
		map $lightmap
		rgbGen identity
		blendFunc filter
		depthFunc equal
	}

}



//==============================
//mix
//==============================


textures/pad_teehouse/matcha02
{
q3map_nonplanar
q3map_shadeangle 60 l
qer_editorimage textures/pad_teehouse/matcha02
surfaceparm sandsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_teehouse/matcha02
blendFunc filter
}
}


textures/pad_teehouse/color_red
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_teehouse/color_red
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_teehouse/color_blue
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_teehouse/color_blue
               		blendFunc add
		rgbGen vertex
	}
}


textures/pad_teehouse/goldlogo
{
     surfaceparm	nomarks
     surfaceparm	trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_teehouse/goldlogo
               		blendFunc add
		rgbGen vertex
	}
}
