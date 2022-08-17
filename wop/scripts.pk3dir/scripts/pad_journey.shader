
textures/pad_journey/abhaube005
{
                surfaceparm metalsteps
	qer_editorimage textures/pad_journey/abhaube005
	{
		map textures/pad_journey/abhaube005
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3
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

textures/pad_journey/metal05
{
                surfaceparm metalsteps
	qer_editorimage textures/pad_journey/metal05
	{
		map textures/pad_journey/metal05
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3
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

textures/pad_journey/mirror_01
{
	qer_editorimage textures/pad_journey/mirror_01
	{
		map textures/pad_journey/mirror_01
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_02
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


textures/pad_journey/mirror_02
{
	qer_editorimage textures/pad_journey/mirror_02
	{
		map textures/pad_journey/mirror_02
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_03
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


textures/pad_journey/mirror_03
{
	qer_editorimage textures/pad_journey/mirror_01
	{
		map textures/pad_journey/mirror_01
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_04
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


textures/pad_journey/mirror_04
{
	qer_editorimage textures/pad_journey/mirror_02
	{
		map textures/pad_journey/mirror_02
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_05
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


textures/pad_journey/mirror_05
{
	qer_editorimage textures/pad_journey/mirror_01
	{
		map textures/pad_journey/mirror_01
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_06
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


textures/pad_journey/mirror_06
{
	qer_editorimage textures/pad_journey/mirror_02
	{
		map textures/pad_journey/mirror_02
		rgbGen identity
	}
	{
		map textures/pad_journey/mirrorgl_07
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


textures/pad_journey/milchglass02
{
	qer_editorimage textures/colors/hex_afb1b5ff
	cull back
	{
		map textures/colors/hex_afb1b5ff
		blendfunc add
	}
	{
		map textures/pad_gfx/env2
		blendfunc filter
		tcGen environment
	}
}


textures/pad_journey/puppet_window02
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks


        {
                map textures/pad_journey/puppet_window02
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


textures/pad_journey/trainticket
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_journey/trainticket
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


textures/pad_journey/trainticketr
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks
     tessSize 64


        {
                map textures/pad_journey/trainticketr
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


textures/pad_journey/oberl01
{
     cull disable
     surfaceparm alphashadow
     surfaceparm trans
     surfaceparm nomarks


        {
                map textures/pad_journey/oberl01
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


textures/pad_journey/blue_trainchair02
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_journey/blue_trainchair02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_journey/red_trainchair02
{
surfaceparm carpetsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_journey/red_trainchair02
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_journey/riffelblech01
{
surfaceparm metalsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_journey/riffelblech01
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_journey/wood019blu
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_journey/wood019blu
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_journey/wood019red
{
surfaceparm woodsteps
{
map $lightmap
rgbGen identity
}
{
map textures/pad_journey/wood019red
blendFunc GL_DST_COLOR GL_ZERO
rgbGen identity
}
}


textures/pad_journey/schienen01
{
      qer_editorimage textures/pad_journey/schienen01
      surfaceparm nolightmap
      surfaceparm nonsolid
      surfaceparm trans
      cull disable

      {
            map textures/pad_journey/schienen01
            blendfunc blend
            tcMod scroll 0 -2.6
      }
      {
            map textures/pad_journey/schienen01
            blendfunc blend
            tcMod scroll 0 -2.6
      }
}


textures/pad_journey/nature_train
{
      qer_editorimage textures/pad_journey/nature_train
      surfaceparm nolightmap
      surfaceparm nonsolid
      surfaceparm trans
      cull disable

      {
            map textures/pad_journey/nature_train
            blendfunc blend
            tcMod scroll 0 -2.5
      }
      {
            map textures/pad_journey/nature_train
            blendfunc blend
            tcMod scroll 0 -2.5
      }
}


textures/pad_journey/wall_stone_train
{
      qer_editorimage textures/pad_journey/wall_stone_train
      surfaceparm nolightmap
      surfaceparm nonsolid
      surfaceparm trans
      cull disable

      {
            map textures/pad_journey/wall_stone_train
            blendfunc blend
            tcMod scroll 0 -1.2
      }
      {
            map textures/pad_journey/wall_stone_train
            blendfunc blend
            tcMod scroll 0 -1.2
      }
}


textures/pad_journey/wall_stone036_train
{
      qer_editorimage textures/pad_journey/wall_stone036_train
      surfaceparm nolightmap
      surfaceparm nonsolid
      surfaceparm trans
      cull disable

      {
            map textures/pad_journey/wall_stone036_train
            blendfunc blend
            tcMod scroll 0 -3.6
      }
      {
            map textures/pad_journey/wall_stone036_train
            blendfunc blend
            tcMod scroll 0 -3.6
      }
}


textures/pad_journey/plakat
{
        qer_editorimage textures/pad_journey/plakat
	{
		map textures/pad_journey/plakat
		rgbGen identity
	}
	{
		map textures/pad_gfx02/tinpad3
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


