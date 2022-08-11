
// =================
// BUMMYBEARS
// =================

//transparent effect for GummyBears by SLoB :)
//Green GummyBear :)
models/mapobjects/gummybears/greengum
{

	{
                map models/mapobjects/gummybears/greengum
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}

//Red GummyBear :)
models/mapobjects/gummybears/redgum
{

	{
                map models/mapobjects/gummybears/redgum
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }


}

//Yellow GummyBear :)
models/mapobjects/gummybears/yellowgum
{
	{
                map models/mapobjects/gummybears/yellowgum
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }


}


//Orange GummyBear :)
models/mapobjects/gummybears/orangegum
{
	{
                map models/mapobjects/gummybears/orangegum
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }


}


// =================
// CAKE
// =================

//padcake model for Harmonieman :) SLoB

models/mapobjects/pad_cake/pad_cakeplatetop
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_cake/plate001
	{
		map models/mapobjects/pad_cake/plate001
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
               	rgbGen vertex
	}

}

models/mapobjects/pad_cake/pad_cakeplaterim
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_cake/plate_pack005
	{
		map models/mapobjects/pad_cake/plate_pack005
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
               	rgbGen vertex
	}

}

models/mapobjects/pad_cake/pad_cakestandbase
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_cake/plate_under
	{
		map models/mapobjects/pad_cake/plate_under
        	rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale .5 .5
		tcGen environment
               	rgbGen vertex
	}
}

models/mapobjects/pad_cake/pad_cake
{
	nopicmip
	nomipmaps

	{
		map models/mapobjects/pad_cake/pad_cake
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}

}

models/mapobjects/pad_cake/pad_cakeglass
{
	nopicmip
	nomipmaps
	cull disable
	sort additive

	{
		map models/mapobjects/pad_cake/pad_cakeglass
		tcGen environment
		blendfunc GL_ONE GL_ONE
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad
		tcGen environment
		blendfunc GL_ONE GL_ONE
		rgbGen vertex
	}

}


// =================
// CONDIMENTS
// =================


models/mapobjects/pad_condiments/saltandpepper
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_condiments/saltandpepper
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_condiments/sugar
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_condiments/sugar
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// CUPS
// =================

models/mapobjects/pad_cups/cup001
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/cup001
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

models/mapobjects/pad_cups/cup002
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/cup002
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_cups/cup003
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/cup003
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_cups/cup004
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/cup004
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_cups/cup_under
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/cup_under
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_cups/handle
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cups/handle
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate001
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate001
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate002
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate002
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate003
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate003
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate004
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate004
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate005
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate005
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_pack003
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_pack003
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_pack004
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_pack004
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_pack005
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_pack005
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_pack
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_pack
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_under
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_under
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffee002
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffee002
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffee003
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffee003
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffee004
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffee004
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffee005
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffee005
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffeeblue
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffeeblue
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_plates/plate_coffeered
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_plates/plate_coffeered
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_maple/maple
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_maple/maple
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_cutlery/knifefork
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_cutlery/knifefork
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_tins/coke1
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_tins/coke1
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

models/mapobjects/pad_tins/cola_bottom
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_tins/cola_bottom
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

models/mapobjects/pad_tins/cola_top
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_tins/cola_top
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_tins/pupsi
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_tins/pupsi
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// CUPS
// =================


models/mapobjects/pad_cups/pad_cup01
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup001
		rgbGen Vertex
	}

}

models/mapobjects/pad_cups/pad_cup02
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup001
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup03
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup002
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup04
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup002
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup05
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup003
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup06
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup003
		rgbGen Vertex
	}

}

models/mapobjects/pad_cups/pad_cup07
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup004
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup08
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup004
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup01_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup001
		rgbGen Vertex
	}

}

models/mapobjects/pad_cups/pad_cup02_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup001
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup03_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup002
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup04_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup002
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup05_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup003
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup06_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup003
		rgbGen Vertex
	}

}

models/mapobjects/pad_cups/pad_cup07_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup004
		rgbGen Vertex
	}

}


models/mapobjects/pad_cups/pad_cup08_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cups/cup004
		rgbGen Vertex
	}

}

// =================
// DUCK
// =================

models/mapobjects/pad_ducks/pad_duck
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_ducks/pad_duck
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// FiSHES
// =================

//Pad_Fish :) for PadMod 2003
//by SLoB

models/mapobjects/pad_fish/flame_angel
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_fish/flame_angel
	{
		map models/mapobjects/pad_fish/flame_angel
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins
{
	nopicmip
	nomipmaps
	cull none
                qer_editorimage models/mapobjects/pad_fish/flame_angel
	{
		map models/mapobjects/pad_fish/flame_angel
		blendfunc GL_ONE GL_ONE
		rgbGen lightingdiffuse

	}

}


models/mapobjects/pad_fish/flame_angel02
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_fish/new_fish02
	{
		map models/mapobjects/pad_fish/new_fish02
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins02
{
	nopicmip
	nomipmaps
	cull none
	qer_editorimage models/mapobjects/pad_fish/new_fish02
	{
		map models/mapobjects/pad_fish/new_fish02
		blendfunc GL_ONE GL_ONE
		rgbGen lightingdiffuse

	}

}

models/mapobjects/pad_fish/flame_angel05
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_fish/new_fish05
	{
		map models/mapobjects/pad_fish/new_fish05
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins05
{
        nopicmip
	nomipmaps
	cull none
	qer_editorimage models/mapobjects/pad_fish/new_fish05
	{
		map models/mapobjects/pad_fish/new_fish05
		blendfunc GL_ONE GL_ONE
		rgbGen lightingdiffuse

	}

}


// =================
// LAMPS
// =================

models/mapobjects/pad_lamps/gooseneck
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_lamps/gooseneck
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


models/mapobjects/pad_lamps/gooseneck2
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_lamps/gooseneck2
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// MOUSETRAP
// =================

models/mapobjects/padmousetrap/padmousetrap
{
	nopicmip
	nomipmaps
        q3map_globaltexture
        surfaceparm nolightmap
	surfaceparm nomarks
	{
	map models/mapobjects/padmousetrap/padmousetrap
	rgbGen vertex
	}
}

// =================
// OPENBOOK
// =================

//show both sides of page for pad_openbook model

models/mapobjects/pad_books/pad_openbook_rp
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_rpwop
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_wop
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_wop
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_pir1
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_pir1
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_pir1
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_pir2
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_pir2
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_pir2
		rgbGen vertex
        }
}

models/mapobjects/pad_books/glow/pad_openbook_glow
{
   cull none
   nopicmip
   nomipmaps
   {
      map models/mapobjects/pad_books/glow/pad_openbook_glow
      alphaFunc GE128
      rgbGen vertex
   }
}

// =================
// SUNFLOWER
// =================


//SunFlower/shader for Padmod by SLoB
//models/mapobjects/pad_sunflower/pad_sunflower.md3

models/mapobjects/pad_sunflower/sunhead
{
	cull none
	nopicmip
	nomipmaps
	{
		map models/mapobjects/pad_sunflower/sunhead
		alphaFunc GE128
		rgbGen vertex
	}
}

models/mapobjects/pad_sunflower/sunleaf1
{
	cull none
	nopicmip
	nomipmaps
	{
		map models/mapobjects/pad_sunflower/sunleaf1
		alphaFunc GE128
		rgbGen vertex
	}
}

models/mapobjects/pad_sunflower/sunleaf2
{
	cull none
	nopicmip
	nomipmaps
	{
		map models/mapobjects/pad_sunflower/sunleaf2
		alphaFunc GE128
		rgbGen vertex
	}
}


// =================
// TFT MONiTOR
// =================

//pad tft monitor by SLoB


models/mapobjects/pad_pc/pad_tft_screen
{
   	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_pc/pad_tft
	q3map_surfacelight 10
	nopicmip

       	{
               	animmap .3 models/mapobjects/pad_pc/screen01 models/mapobjects/pad_pc/screen02 models/mapobjects/pad_pc/screen03 models/mapobjects/pad_pc/screen04 models/mapobjects/pad_pc/screen05
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3
        	tcGen environment
                blendfunc GL_ONE GL_ONE
                rgbGen identity
	}
}

// =================
// NASCARS
// =================

// PADCARS_NASCARS SHADER by SLoB
// ALL PAD NASCARS WILL BE IN THIS SHADER FILE
//models/shaders for PadNasCars by SLoB, Skins by Ente (for original Nascar4)

//ORANGE PAD NASCAR
models/mapobjects/pad_nascars/padcar_orange
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_nascars/padcar_orange
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

//BLACK PAD NASCAR
models/mapobjects/pad_nascars/padcar_black
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_nascars/padcar_black
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

//MUSTER PAD NASCAR
models/mapobjects/pad_nascars/padcar_muster
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_nascars/padcar_muster
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}



// =================
// PAD GOLDSTATUE
// =================


models/mapobjects/ente/padgold_goggles
{
	cull disable
	nopicmip
	nomipmaps
	{
		map models/mapobjects/ente/padgold_head
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
}
}

models/mapobjects/ente/padgold_cape
{
	cull disable
	nopicmip
	nomipmaps
	{
		map models/mapobjects/ente/padgold_body
		rgbGen Vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
}
}


models/mapobjects/ente/padgold_body
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/ente/padgold_body
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

models/mapobjects/ente/padgold_head
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/ente/padgold_head
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}





// =================
// PAD STATUE
// =================

models/mapobjects/ente/padwinner_goggles
{
	cull disable
	{
		map models/mapobjects/ente/padwinner_head
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
}

models/mapobjects/ente/padwinner_cape
{
	cull disable
	deformVertexes wave 100 sin 0 0.2 3 1
	{
		map models/mapobjects/ente/padwinner_body
		rgbGen Vertex
	}
}

// =================
// TELEViSION
// =================

//padtv for 1 tv screen atm

models/mapobjects/padtv/tv01
{
	nopicmip
	{
		map models/mapobjects/padtv/screen
		rgbGen Vertex
	}
	{
		videomap pad_test.roq //pad_test.roq //blinkeye.roq //end.roq //pad_test.roq
		blendfunc add
		//tcMod scale .25 .25
		rgbGen identity
	}
	{
		map models/mapobjects/padtv/scanline
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3
        	tcGen environment
                blendfunc GL_ONE GL_ONE
                rgbGen identity
	}

}

models/mapobjects/padtv/tv02
{
   	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/padtv/tv004
	q3map_surfacelight 100
	nopicmip

       	{
               	animmap .4 models/mapobjects/padtv/tv001 models/mapobjects/padtv/tv003 models/mapobjects/padtv/tv001 models/mapobjects/padtv/tv004 models/mapobjects/padtv/tv006 models/mapobjects/padtv/tv001 models/mapobjects/padtv/tv005
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3
        	tcGen environment
                blendfunc GL_ONE GL_ONE
                rgbGen identity
	}
}


// =================
// DUCKTAPE
// =================

//SLoB
//one shader for all sa map models, add to when needed


models/mapobjects/pad_ducktape/ducktape
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_ducktape/ducktape
	{
		map models/mapobjects/pad_ducktape/ducktape
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.2 0.2
		tcGen environment
        rgbGen vertex
	}

}

// =================
// SiNK
// =================

models/mapobjects/pad_sink/caps
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_sink/caps
	{
		map models/mapobjects/pad_sink/caps
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.2 0.2
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_sink/tap
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_sink/tap
	{
		map models/mapobjects/pad_sink/tap
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.5 0.5
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_sink/wastepipe
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_sink/sinkback
	{
		map models/mapobjects/pad_sink/sinkback
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.4 0.4
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_sink/sink
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_sink/sinkback
	{
		map models/mapobjects/pad_sink/sinkback
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex

	}
}

models/mapobjects/pad_sink/sinkdoublesided
{
	nopicmip
	nomipmaps
	cull disable
	qer_editorimage models/mapobjects/pad_sink/sinkback
	{
		map models/mapobjects/pad_sink/sinkback
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}


// =================
// PRiSiONJACKET
// =================

models/mapobjects/pad_clothes/prisonjacket
{
    cull disable
	//deformVertexes wave 100 sin 0 0.2 3 1
	deformVertexes wave 100 sin 3 0.2 3 0.5
	qer_editorimage models/mapobjects/pad_clothes/prisonjacket
	{
		map models/mapobjects/pad_clothes/prisonjacket
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/prisonjacketfixedpoint
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/prisonjacket
	{
		map models/mapobjects/pad_clothes/prisonjacket
		rgbGen vertex
	}
}

// =================
// SOCKS
// =================

models/mapobjects/pad_clothes/sock_grey
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey
	{
		map models/mapobjects/pad_clothes/sock_grey
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/sock_grey2
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey2
	{
		map models/mapobjects/pad_clothes/sock_grey2
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/sock_grey3
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey3
	{
		map models/mapobjects/pad_clothes/sock_grey3
		rgbGen vertex
	}
}


// =================
// TOOTHBRUSH
// =================

models/mapobjects/pad_items/toothbrush_blackred
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackred
	{
		map models/mapobjects/pad_items/toothbrush_blackred
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_items/toothbrush_blackblue
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackblue
	{
		map models/mapobjects/pad_items/toothbrush_blackblue
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_items/toothbrush_blackorange

{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackorange
	{
		map models/mapobjects/pad_items/toothbrush_blackorange
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_items/toothbrush_blackpurple
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackpurple
	{
		map models/mapobjects/pad_items/toothbrush_blackpurple
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}

models/mapobjects/pad_items/toothbrush_blackcyan
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackcyan
	{
		map models/mapobjects/pad_items/toothbrush_blackcyan
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}


models/mapobjects/pad_items/toothbrush_blackgreen
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackgreen
	{
		map models/mapobjects/pad_items/toothbrush_blackgreen
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad
		blendfunc GL_ONE GL_ONE
		tcmod scale 0.6 0.6
		tcGen environment
        rgbGen vertex
	}
}


// =================
// ELECTRiC TOOTHBRUSH
// =================


models/mapobjects/pad_bath/electric_toothbrush
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_bath/electric_toothbrush
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// DEOCAN
// =================


models/mapobjects/pad_bath/deocan1
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_bath/deocan1
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// TOOTHPASTE
// =================


models/mapobjects/pad_bath/toothpaste
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_bath/toothpaste
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}

// =================
// OINTMENT
// =================

models/mapobjects/pad_bath/ointment
{
	cull none
	nopicmip
	nomipmaps
	//sort 16

	{
		map models/mapobjects/pad_bath/ointment
		//blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		//alphaFunc GE128
		rgbGen vertex // identity //vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// BOOKEND
// =================

//shaders for doomdragons mapobjects for World of PADMAN
models/mapobjects/pad_ddmix/padlibrary/bookend
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/padlibrary/bookend
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// CHESS
// =================

models/mapobjects/pad_ddmix/chess/chess_white
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/chess/chess_white
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/chess/chess_black
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/chess/chess_black
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/padlibrary/bookend_shader
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/padlibrary/bookend_shader
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

// =================
// SHiP BOTTLE
// =================

models/mapobjects/pad_ddmix/padship/bottle
{
	sort additive
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/padship/bottle
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
	{
		map $lightmap
		blendfunc filter
		rgbGen identity
		tcGen lightmap
	}
	{
		map textures/pad_gfx02/invispad
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}


// =================
// FLOWER
// =================


models/mapobjects/pad_ddmix/plants/flower_pink
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/flower_pink
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/plants/tulip1
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/tulip1
		rgbGen Vertex
		alphaFunc GE128
	}
}


models/mapobjects/pad_ddmix/plants/water_lily
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/water_lily
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/plants/fern
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/fern
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/plants/flower_yellow
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/flower_yellow
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/plants/waterplant1
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	deformVertexes wave 194 sin 0 2 0 0.3
	{
		map models/mapobjects/pad_ddmix/plants/waterplant1
		rgbGen Vertex
		alphaFunc GE128
	}
}


models/mapobjects/pad_ddmix/plants/ikebana_red
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/ikebana_red
		rgbGen Vertex
		alphaFunc GE128
	}
}


models/mapobjects/pad_ddmix/plants/ikebana_blue
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/ikebana_blue
		rgbGen Vertex
		alphaFunc GE128
	}
}


// =================
// DiNER CAN
// =================

models/mapobjects/pad_ddmix/diner/can_top
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/diner/can_top
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}


// =================
// COFFEE POT
// =================


models/mapobjects/pad_ddmix/diner/coffee_pot
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/diner/coffee_pot
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}


// =================
// TOWEL
// =================

models/mapobjects/pad_ddmix/padkitchen/towel
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_kitchen
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_kitchen
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_bath_blue
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_bath_blue
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_bath_pink
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_bath_pink
		rgbGen Vertex
	}
}



// =================
// MATCHES
// =================

models/mapobjects/pad_ddmix/mix/matches
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matches
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_diner
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_diner
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_doomdragon
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_doomdragon
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_padman
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_padman
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/plants/bonsai
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/bonsai
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// VASE
// =================

models/mapobjects/pad_ddmix/anteroom/vase
{
	surfaceparm trans
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/anteroom/vase
		blendfunc blend
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/anteroom/vase
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/water_vase
{
	sort additive
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/anteroom/water_vase
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
}

// =================
// SNOWMAN
// =================

models/mapobjects/pad_ddmix/jail/snowman
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/jail/snowman
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// TUNNELLAMP
// =================

models/mapobjects/pad_ddmix/diner/tunnellamp
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/diner/tunnellamp
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// LiGHT
// =================

models/mapobjects/pad_ddmix/diner/light
{
	cull disable
	nomipmaps
	q3map_surfacelight 1000
	{
		map models/mapobjects/pad_ddmix/diner/light
		rgbGen identity
		tcGen lightmap
	}
}

models/mapobjects/pad_ddmix/plants/cane
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/cane
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// iNJECTiON
// =================

models/mapobjects/pad_ddmix/anteroom/injection
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/anteroom/injection
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

// =================
// KASPAD
// =================

models/mapobjects/pad_ddmix/attic/kaspad
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/attic/kaspad
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// HOUSE LAMP
// =================

models/mapobjects/pad_ddmix/trashcan/lamp_house_day
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_day
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_day
{
	surfaceparm nolightmap
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_day
	}
	{
		map models/mapobjects/pad_ddmix/trashcan/reflection
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_night
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_night
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night
{
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night
	q3map_surfacelight 50000
	{
		map $lightmap
		rgbGen identity
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_lightrays
{
	surfaceparm nolightmap
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_lightrays
		blendfunc add
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_dirty
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_dirty
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_unlit
{
	surfaceparm nolightmap
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_unlit
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
{
	surfaceparm nolightmap
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
	q3map_surfacelight 50000
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit2
{
	surfaceparm nolightmap
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
	q3map_surfacelight 25000
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
	}
}

// =================
// GLASS
// =================

models/mapobjects/pad_ddmix/diner/glass01
{
	surfaceparm trans
	sort additive
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/diner/glass_highlight_small
{
	surfaceparm trans
	sort additive
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight_small
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/diner/glass_rings
{
	surfaceparm trans
	sort additive
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_rings
		blendfunc blend
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/diner/glass_betty
{
	surfaceparm trans
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_betty
		blendfunc blend
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_betty
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/diner/glass_sunrise
{
	surfaceparm trans
	sort additive
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_sunrise
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight
		blendfunc add
		tcGen environment
	}
}


models/mapobjects/pad_ddmix/diner/glass_midnight
{
	surfaceparm trans
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_midnight
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight
		blendfunc add
		tcGen environment
	}
}

// =================
// TELESCOPE
// =================

models/mapobjects/pad_ddmix/cabin/telescope
{
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/cabin/telescope
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/cabin/occulars
{
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/cabin/occulars
		tcMod scale 0.8 0.8
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/cabin/occulars2
		blendfunc blend
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// FIRE
// =================

models/mapobjects/pad_ddmix/effects/FireBigA
{
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	cull disable
	{
		animmap 10 models/mapobjects/pad_ddmix/effects/FireBigA models/mapobjects/pad_ddmix/effects/FireBigB models/mapobjects/pad_ddmix/effects/FireBigC models/mapobjects/pad_ddmix/effects/FireBigD models/mapobjects/pad_ddmix/effects/FireBigE models/mapobjects/pad_ddmix/effects/FireBigF models/mapobjects/pad_ddmix/effects/FireBigG models/mapobjects/pad_ddmix/effects/FireBigH
		blendfunc add
		rgbGen wave inversesawtooth 1 3 1 3
	}
}

// =================
// CASE
// =================

models/mapobjects/pad_ddmix/anteroom/medical_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/medical_case
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/black_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/black_case
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/metal_suitcase
{
	{
		map models/mapobjects/pad_ddmix/anteroom/metal_suitcase
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/silver_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/silver_case
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

// =================
// SHiPLAMP
// =================

models/mapobjects/pad_ddmix/padship/shiplamp_light
{
	cull disable
	q3map_surfacelight 60
	{
		map models/mapobjects/pad_ddmix/padship/shiplamp_light
	}
}

// =================
// VALVE
// =================

models/mapobjects/pad_ddmix/padship/valve
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padship/valve
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad2c
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

// =================
// BiG PUMPER
// =================

models/mapobjects/pad_ddmix/attic/pumper
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/attic/pumper
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/attic/tubes
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/attic/tubes
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

// =================
// TREES
// =================

models/mapobjects/pad_ddmix/trashcan/tree1_twigs
{
	surfaceparm alphashadow
	cull disable
	//deformVertexes wave 194 sin 0 2 0 0.3
	{
		map models/mapobjects/pad_ddmix/trashcan/tree1_twigs
		rgbGen Vertex
		alphaFunc GE128
	}
}


models/mapobjects/pad_ddmix/trashcan/tree1_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree1_leaves
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree2_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree2_leaves
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree3_twigs
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree3_twigs
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree4_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree4_leaves
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/grass
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/grass
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/bush
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/bush
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// PADKiNG
// =================

//shaders for doomdragons mapobjects for World of PADMAN
models/mapobjects/pad_ddmix/castle/padking
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/padking
		rgbGen Vertex
		alphaFunc GE128
	}
}


// =================
// TURUL
// =================

models/mapobjects/pad_ddmix/castle/turul
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/turul
		rgbGen Vertex
		alphaFunc GE128
	}
}


// =================
// KNiGHTSHiELD
// =================

models/mapobjects/pad_ddmix/castle/knightshield
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/knightshield
		rgbGen Vertex
		alphaFunc GE128
	}
}


// =================
// GROUNDPLANT
// =================

models/mapobjects/pad_cloister/groundplant
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_cloister/groundplant
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// CLOiSTER LAMP
// =================

models/mapobjects/pad_cloister/cloister_lamp
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_cloister/cloister_lamp
		rgbGen Vertex
		alphaFunc GE128
	}
}


// =================
// BROWN FERN
// =================

models/mapobjects/pad_cloister/fern_brown
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_cloister/fern_brown
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// BROWN CANE
// =================

models/mapobjects/pad_cloister/cane_brown
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_cloister/cane_brown
		rgbGen Vertex
		alphaFunc GE128
	}
}

// ================
// PAD TRAIN
// ================

models/mapobjects/pad_train/toilet_ceramic
{
	{
		map models/mapobjects/pad_train/ttoilet
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/ceramic
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/toilet_plastic_shiny
{
	{
		map models/mapobjects/pad_train/ttoilet
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/plastic_shiny
		blendfunc add
		rgbGen vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/toilet_plastic_matte
{
	{
		map models/mapobjects/pad_train/ttoilet
		rgbGen vertex
	}
}

models/mapobjects/pad_train/toilet_metal
{
	{
		map models/mapobjects/pad_train/ttoilet
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/metal_test
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/tdoorhandle
{
	{
		map models/mapobjects/pad_train/tdoorhandle
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/ehammer_metal
{
	{
		map models/mapobjects/pad_train/ehammer
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/metal_test
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/ente/deadalien
{
	{
		map models/mapobjects/ente/deadalien
		rgbGen identity
	}
        {
                map textures/pad_gfx02/tinpad3
		blendfunc add
                rgbGen Vertex
		tcGen environment
        }
        nomipmaps
        {
		map models/mapobjects/ente/deadalien_light
		rgbGen identity
		blendFunc filter
	}
}
models/mapobjects/ente/deadalien_glass
{
	surfaceparm trans
	cull none
	surfaceparm nolightmap
        {
		map models/mapobjects/ente/blendomatic
		rgbGen const ( 0.0 0.2 0.6 )
		blendFunc blend
                tcGen environment
	}
        {
		map textures/pad_gfx/glass_dark
                tcgen environment
		blendFunc add
		rgbGen identity
	}
}

models/mapobjects/pad_train/soapdispenser
{
	{
		map models/mapobjects/pad_train/soapdispenser
		rgbGen identity
	}
        {
                map textures/pad_gfx02/tinpad4
		blendfunc add
                rgbGen Vertex
		tcGen environment
        }
}
models/mapobjects/pad_train/soapdispenser_window
{
  {
		map models/mapobjects/pad_train/soapdispenser
		rgbGen identity
	}
  {
    map textures/pad_gfx/glass_bright
		blendfunc add
    rgbGen Vertex
		tcGen environment
  }
}

models/mapobjects/pad_train/ptoweldispenser
{
  cull none
  {
    map models/mapobjects/pad_train/ptoweldispenser
    rgbGen identity
  }
}
models/mapobjects/pad_train/ptoweldispenser_shiny
{
	{
		map models/mapobjects/pad_train/ptoweldispenser
		rgbGen identity
	}
  {
    map textures/pad_gfx02/tinpad4
		blendfunc add
    rgbGen Vertex
		tcGen environment
  }
}

// =================
// ACTIONFIGURES
// =================


models/mapobjects/pad_actionfigures/plastic
{
	surfaceparm trans
	cull disable
	{
		map textures/pad_gfx02/tinpad3
		blendfunc gl_one gl_one_minus_src_color
		rgbGen identity
		tcGen environment
	}
}

models/mapobjects/pad_actionfigures/fig_pumper
{
	{
		map models/weapons2/pumper/pumper
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_pumpertubes
{
	{
		map models/weapons2/pumper/tubes
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_splasher
{
	{
		map models/weapons2/splasher/splasher
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_frontface
{
	{
		map models/weapons2/splasher/new_frontface
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_splasherwater
{
	{
		map textures/pad_gfx02/padmapplas
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .6 0 .2
		tcmod scale .4 .4
		tcMod scroll .09 -.1.1
		//rgbGen lightingdiffuse //identity
	}
	{
		map textures/pad_gfx02/padmapblue2
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .7 0 .3
		tcmod scale .2 .2
		tcMod scroll .1 .09
		//rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/padmapblue
		blendFunc GL_ONE GL_ONE
		tcGen environment
		tcMod scroll -.3 0
		//rgbGen lightingDiffuse
	}
}



/// ***************
/// *** PadGirl ***
/// ***************


models/mapobjects/pad_actionfigures/padgrlhd
{
	{
		map models/wop_players/padgirl/padgrlhd
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/padgrlbd
{
	{
		map models/wop_players/padgirl/padgrlbd
		rgbGen Vertex
	}
}

/// ****************
/// *** PadLilly ***
/// ****************


models/mapobjects/pad_actionfigures/lilhead
{
	{
		map models/wop_players/padlilly/lilhead
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/lilbody
{
	{
		map models/wop_players/padlilly/lilbody
		rgbGen Vertex
	}
}


// =================
// WESTERNSET
// =================

models/mapobjects/pad_westernset/colt
{
	{
		map models/mapobjects/pad_westernset/colt
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_westernset/camera_screens
{
	nomipmaps
	{
		map models/mapobjects/pad_westernset/camera_screens
		tcMod scale 0.8 0.8
		tcGen environment
	}
}

models/mapobjects/pad_westernset/directors_chair_wood
{
	{
		map models/mapobjects/pad_westernset/directors_chair_wood
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_westernset/directors_chair_cloth
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_westernset/directors_chair_cloth
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_westernset/actors_chair_cloth
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_westernset/actors_chair_cloth
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_westernset/spotlight
{
	cull disable
	{
		map models/mapobjects/pad_westernset/spotlight
		rgbGen Vertex
	}
}

models/mapobjects/pad_westernset/spotlight_front
{
	cull disable
	q3map_surfacelight 60
	{
		map models/mapobjects/pad_westernset/spotlight_front
	}
}


models/mapobjects/pad_westernset/sheriff_badge
{
	cull none

	{
		map models/mapobjects/pad_westernset/sheriff_badge
		rgbGen vertex
        }
	{
		map textures/pad_gfx02/tinpad3
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex
	}
}

// =================
// PAD_FRUITS
// =================

models/mapobjects/pad_fruits/citrus_leaf
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_fruits/citrus
		rgbGen vertex
		alphaFunc GE128
	}
}

// =================
// PadStatue Super
// =================

//// Head
models/mapobjects/pad_statues/statue_super_head
{
  cull none
  {
//    map models/wop_players/padman/default_head
    map models/mapobjects/ente/padwinner_head	//head still needs reference to old padwinner statue texture for correct alignment ~Kai-Li
  }
}

models/mapobjects/pad_statues/statue_super_hands
{
  {
    map models/wop_players/padman/default_head
  }
}

models/mapobjects/pad_statues/statue_super_shoes
{
  {
    map models/wop_players/padman/default_head
  }
}

models/mapobjects/pad_statues/statue_super_shades
{
  cull none
  {
    map models/wop_players/padman/default_head
  }
}

//// Body
models/mapobjects/pad_statues/statue_super_neck
{
  {
    map models/wop_players/padman/default_body
  }
}

models/mapobjects/pad_statues/statue_super_cape
{
  cull none
  {
    map models/wop_players/padman/default_body
  }
}

models/mapobjects/pad_statues/statue_super_suit
{
  {
    map models/wop_players/padman/default_body
  }
}

models/mapobjects/pad_statues/statue_super_buckle
{
  {
    map models/wop_players/padman/default_head
  }
}

models/mapobjects/pad_statues/statue_super_button
{
  {
    map models/wop_players/padman/default_head
  }
}

models/mapobjects/ente/padshop_tft_screen
{
   	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_pc/pad_tft
	q3map_surfacelight 10
	nopicmip

       	{
               	animmap .3 models/mapobjects/ente/screen01 models/mapobjects/ente/screen02 models/mapobjects/ente/screen03 models/mapobjects/ente/screen04 models/mapobjects/ente/screen05
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3
        	tcGen environment
                blendfunc GL_ONE GL_ONE
                rgbGen identity
	}
}