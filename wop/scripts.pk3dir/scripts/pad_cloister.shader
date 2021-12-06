textures/pad_fountain/utopiaatoll
{
        qer_editorimage textures/pad_fountain/utopiaatoll.tga

	surfaceparm noimpact
	surfaceparm nolightmap             
                q3map_lightimage textures/pad_fountain/orange02.tga
	q3map_sun	1.000000 0.828885 0.672267 220 100 90
	q3map_surfacelight 300

        skyparms env/utopiaatoll512 - -

	}


textures/pad_fountain/ground01
{
q3map_nonplanar
q3map_shadeangle 60 l	
surfaceparm sandsteps
qer_editorimage textures/pad_fountain/ground01.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/ground01.tga
blendFunc filter
}
}



textures/pad_fountain/stone_01b
{
q3map_nonplanar
q3map_shadeangle 60 l	
surfaceparm sandsteps
qer_editorimage textures/pad_fountain/stone_01b.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/stone_01b.tga
blendFunc filter
}
}


textures/pad_fountain/roughwall008_512
{
q3map_nonplanar
q3map_shadeangle 60 l	
surfaceparm sandsteps
qer_editorimage textures/pad_fountain/roughwall008_512.tga
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/roughwall008_512.tga
blendFunc filter
}
}


textures/pad_fountain/tep003_512
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/tep003_512.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_fountain/dekostoff
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/dekostoff.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_fountain/balken01_512
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/balken01_512.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_fountain/balken01_end
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/balken01_end.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_fountain/blue_woood001old
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/blue_woood001old.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_fountain/red_woood001old
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/red_woood001old.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_fountain/woodleiste01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/woodleiste01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_fountain/woodleiste02
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/woodleiste02.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}

textures/pad_fountain/woodchair01
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_fountain/woodchair01.tga
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_fountain/seicurve
{
          qer_editorimage textures/pad_fountain/seilbrunnen.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_fountain/seilbrunnen.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/pad_fountain/oldmetalcurve
{
          qer_editorimage textures/pad_fountain/oldmetal.tga
           surfaceparm nonsolid		
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/pad_fountain/oldmetal.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}



