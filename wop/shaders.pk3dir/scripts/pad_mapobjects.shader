
// =================
// BUMMYBEARS
// =================

//transparent effect for GummyBears by SLoB :)
//Green GummyBear :)
models/mapobjects/gummybears/greengum
{

	{
                map models/mapobjects/gummybears/greengum.tga
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }

}

//Red GummyBear :)
models/mapobjects/gummybears/redgum
{

	{
                map models/mapobjects/gummybears/redgum.tga
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }


}

//Yellow GummyBear :)
models/mapobjects/gummybears/yellowgum
{
	{
                map models/mapobjects/gummybears/yellowgum.tga
		tcGen environment
		blendfunc GL_ONE GL_ONE
        }


}


//Orange GummyBear :)
models/mapobjects/gummybears/orangegum
{
	{
                map models/mapobjects/gummybears/orangegum.tga
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
	qer_editorimage models/mapobjects/pad_cake/plate001.tga
	{
		map models/mapobjects/pad_cake/plate001.tga
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_cake/plate_pack005.tga
	{
		map models/mapobjects/pad_cake/plate_pack005.tga
		rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_cake/plate_under.tga
	{
		map models/mapobjects/pad_cake/plate_under.tga
        	rgbGen vertex
	}
	{
		map $lightmap
		rgbGen identity
		blendfunc filter
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
		map models/mapobjects/pad_cake/pad_cake.tga
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
		map models/mapobjects/pad_cake/pad_cakeglass.tga
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
		map textures/pad_gfx02/invispad.jpg
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
		map models/mapobjects/pad_condiments/saltandpepper.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_condiments/sugar.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/cup001.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/cup002.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/cup003.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/cup004.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/cup_under.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cups/handle.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate001.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate002.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate003.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate004.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate005.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_pack003.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_pack004.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_pack005.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_pack.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_under.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffee002.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffee003.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffee004.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffee005.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffeeblue.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_plates/plate_coffeered.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_maple/maple.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_cutlery/knifefork.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_tins/coke1.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_tins/cola_bottom.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_tins/cola_top.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_tins/pupsi.tga
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
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// CUTLERY
// =================


models/mapobjects/pad_cutlery/pad_cup01
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup001.tga
		rgbGen Vertex
	}

}

models/mapobjects/pad_cutlery/pad_cup02
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup001.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup03
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup002.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup04
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup002.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup05
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup003.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup06
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup003.tga
		rgbGen Vertex
	}

}

models/mapobjects/pad_cutlery/pad_cup07
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup004.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup08
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup004.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup01_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup001.tga
		rgbGen Vertex
	}

}

models/mapobjects/pad_cutlery/pad_cup02_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup001.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup03_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup002.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup04_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup002.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup05_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup003.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup06_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup003.tga
		rgbGen Vertex
	}

}

models/mapobjects/pad_cutlery/pad_cup07_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup004.tga
		rgbGen Vertex
	}

}


models/mapobjects/pad_cutlery/pad_cup08_upd
{
	surfaceparm noimpact
	surfaceparm nonsolid
	{
		map models/mapobjects/pad_cutlery/pad_cup004.tga
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
		map models/mapobjects/pad_ducks/pad_duck.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage models/mapobjects/pad_fish/flame_angel.tga
	{
		map models/mapobjects/pad_fish/flame_angel.tga
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins
{
	nopicmip
	nomipmaps
	cull none
                qer_editorimage models/mapobjects/pad_fish/flame_angel.tga
	{
		map models/mapobjects/pad_fish/flame_angel.tga
		blendfunc GL_ONE GL_ONE
		rgbGen lightingdiffuse

	}

}


models/mapobjects/pad_fish/flame_angel02
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_fish/new_fish02.tga
	{
		map models/mapobjects/pad_fish/new_fish02.tga
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins02
{
	nopicmip
	nomipmaps
	cull none
	qer_editorimage models/mapobjects/pad_fish/new_fish02.tga
	{
		map models/mapobjects/pad_fish/new_fish02.tga
		blendfunc GL_ONE GL_ONE
		rgbGen lightingdiffuse

	}

}

models/mapobjects/pad_fish/flame_angel05
{
	nopicmip
	nomipmaps
	qer_editorimage models/mapobjects/pad_fish/new_fish05.tga
	{
		map models/mapobjects/pad_fish/new_fish05.tga
		rgbGen lightingdiffuse

	}

}

//fins make em transparentish?
models/mapobjects/pad_fish/flame_angel_fins05
{
        nopicmip
	nomipmaps
	cull none
	qer_editorimage models/mapobjects/pad_fish/new_fish05.tga
	{
		map models/mapobjects/pad_fish/new_fish05.tga
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
		map models/mapobjects/pad_lamps/gooseneck.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_lamps/gooseneck2.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
	map models/mapobjects/padmousetrap/padmousetrap.tga
	rgbGen vertex
	}
}

// =================
// OPENBOOK
// =================

//show both sides of page for pad_openbook model

models/mapobjects/pad_books/pad_openbook_rp
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook.tga
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook.tga
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_rpwop
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_wop.tga
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_wop.tga
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_pir1
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_pir1.tga
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_pir1.tga
		rgbGen vertex
        }
}

models/mapobjects/pad_books/pad_openbook_pir2
{
	qer_editorimage models/mapobjects/pad_books/pad_openbook_pir2.tga
	surfaceparm nolightmap
	surfaceparm nonsolid
	cull twosided
	{
		map models/mapobjects/pad_books/pad_openbook_pir2.tga
		rgbGen vertex
        }
}

models/mapobjects/pad_books/glow/pad_openbook_glow
{
   cull none
   nopicmip
   nomipmaps
   {
      map models/mapobjects/pad_books/glow/pad_openbook_glow.tga
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
		map models/mapobjects/pad_sunflower/sunhead.tga
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
		map models/mapobjects/pad_sunflower/sunleaf1.tga
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
		map models/mapobjects/pad_sunflower/sunleaf2.tga
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
	q3map_lightimage models/mapobjects/pad_pc/pad_tft.tga
	q3map_surfacelight 10
	nopicmip

       	{
               	animmap .3 models/mapobjects/pad_pc/screen01.tga models/mapobjects/pad_pc/screen02.tga models/mapobjects/pad_pc/screen03.tga models/mapobjects/pad_pc/screen04.tga models/mapobjects/pad_pc/screen05.tga
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline.tga
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_nascars/padcar_orange.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_nascars/padcar_black.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_nascars/padcar_muster.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/ente/padgold_head.tga
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
		map textures/pad_gfx02/tinpad.tga
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
		map models/mapobjects/ente/padgold_body.tga
		rgbGen Vertex
        }
	{
		map $lightmap
		tcgen environment
		blendfunc filter
	}
	{
		map textures/pad_gfx02/tinpad.tga
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
		map models/mapobjects/ente/padgold_body.tga
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
		map textures/pad_gfx02/tinpad.tga
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
		map models/mapobjects/ente/padgold_head.tga
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
		map textures/pad_gfx02/tinpad.tga
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
		map models/mapobjects/ente/padwinner_head.tga
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
		map models/mapobjects/ente/padwinner_body.tga
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
		map models/mapobjects/padtv/screen.tga
		rgbGen Vertex
	}
	{
		videomap pad_test.roq //pad_test.roq //blinkeye.roq //end.roq //pad_test.roq
		blendfunc add
		//tcMod scale .25 .25
		rgbGen identity
	}
	{
		map models/mapobjects/padtv/scanline.tga
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	q3map_lightimage models/mapobjects/padtv/tv004.tga
	q3map_surfacelight 100
	nopicmip

       	{
               	animmap .4 models/mapobjects/padtv/tv001.tga models/mapobjects/padtv/tv003.tga models/mapobjects/padtv/tv001.tga models/mapobjects/padtv/tv004.tga models/mapobjects/padtv/tv006.tga models/mapobjects/padtv/tv001.tga models/mapobjects/padtv/tv005.tga
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline.tga
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage models/mapobjects/pad_ducktape/ducktape.tga
	{
		map models/mapobjects/pad_ducktape/ducktape.tga
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
	qer_editorimage models/mapobjects/pad_sink/caps.jpg
	{
		map models/mapobjects/pad_sink/caps.jpg
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_sink/tap.jpg
	{
		map models/mapobjects/pad_sink/tap.jpg
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_sink/sinkback.jpg
	{
		map models/mapobjects/pad_sink/sinkback.jpg
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_sink/sinkback.jpg
	{
		map models/mapobjects/pad_sink/sinkback.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_sink/sinkback.jpg
	{
		map models/mapobjects/pad_sink/sinkback.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_clothes/prisonjacket.jpg
	{
		map models/mapobjects/pad_clothes/prisonjacket.jpg
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/prisonjacketfixedpoint
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/prisonjacket.jpg
	{
		map models/mapobjects/pad_clothes/prisonjacket.jpg
		rgbGen vertex
	}
}

// =================
// SOCKS
// =================

models/mapobjects/pad_clothes/sock_grey
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey.jpg
	{
		map models/mapobjects/pad_clothes/sock_grey.jpg
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/sock_grey2
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey2.jpg
	{
		map models/mapobjects/pad_clothes/sock_grey2.jpg
		rgbGen vertex
	}
}

models/mapobjects/pad_clothes/sock_grey3
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock_grey3.jpg
	{
		map models/mapobjects/pad_clothes/sock_grey3.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackred.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackred.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackblue.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackblue.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackorange.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackorange.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackpurple.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackpurple.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackcyan.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackcyan.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
	qer_editorimage models/mapobjects/pad_items/toothbrush_blackgreen.jpg
	{
		map models/mapobjects/pad_items/toothbrush_blackgreen.jpg
		rgbGen vertex

	}
	{
		map textures/pad_gfx02/invispad.jpg
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
		map models/mapobjects/pad_bath/electric_toothbrush.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_bath/deocan1.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_bath/toothpaste.tga
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
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_bath/ointment.tga
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
		map textures/pad_gfx02/tinpad3.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                rgbGen vertex //identity //lightingDiffuse //makes effect darker
	}
}


// =================
// BOOKEND
// =================

//shaders for doomdragons mapobjects for world of padman
models/mapobjects/pad_ddmix/padlibrary/bookend
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/padlibrary/bookend.tga
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
		map models/mapobjects/pad_ddmix/chess/chess_white.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/chess/chess_black.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/padlibrary/bookend_shader.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/padship/bottle.tga
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
		map textures/pad_gfx02/invispad.tga
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
		map models/mapobjects/pad_ddmix/plants/flower_pink.tga
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
		map models/mapobjects/pad_ddmix/plants/tulip1.tga
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
		map models/mapobjects/pad_ddmix/plants/water_lily.tga
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
		map models/mapobjects/pad_ddmix/plants/fern.tga
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
		map models/mapobjects/pad_ddmix/plants/flower_yellow.tga
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
		map models/mapobjects/pad_ddmix/plants/waterplant1.tga
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
		map models/mapobjects/pad_ddmix/plants/ikebana_red.tga
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
		map models/mapobjects/pad_ddmix/plants/ikebana_blue.tga
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
		map models/mapobjects/pad_ddmix/diner/can_top.tga
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
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
		map models/mapobjects/pad_ddmix/diner/coffee_pot.tga
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
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
		map models/mapobjects/pad_ddmix/padkitchen/towel.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_kitchen
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_kitchen.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_bath_blue
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_bath_blue.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_ddmix/padkitchen/towel_bath_pink
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel_bath_pink.tga
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
		map models/mapobjects/pad_ddmix/mix/matches.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_diner
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_diner.tga
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_doomdragon
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_doomdragon.tga
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/mix/matchbook_padman
{
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/mix/matchbook_padman.tga
		rgbGen Vertex
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
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
		map models/mapobjects/pad_ddmix/plants/bonsai.tga
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
		map models/mapobjects/pad_ddmix/anteroom/vase.tga
		blendfunc blend
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/anteroom/vase.tga
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
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
		map models/mapobjects/pad_ddmix/anteroom/water_vase.tga
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
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
		map models/mapobjects/pad_ddmix/jail/snowman.tga
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
		map models/mapobjects/pad_ddmix/diner/tunnellamp.tga
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
		map models/mapobjects/pad_ddmix/diner/light.tga
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
		map models/mapobjects/pad_ddmix/plants/cane.tga
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
		map models/mapobjects/pad_ddmix/anteroom/injection.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/attic/kaspad.tga
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
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_day.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_day
{
	surfaceparm nolightmap
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_day.tga
	}
	{
		map models/mapobjects/pad_ddmix/trashcan/reflection.tga
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_night
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_night.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night
{
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night.tga
	q3map_surfacelight 50000
	{
		map $lightmap
		rgbGen identity
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_glass_night.tga
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_house_lightrays
{
	surfaceparm nolightmap
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_house_lightrays.tga
		blendfunc add
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_dirty
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_dirty.tga
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_unlit
{
	surfaceparm nolightmap
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_unlit.tga
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit
{
	surfaceparm nolightmap
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit.tga
	q3map_surfacelight 50000
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit.tga
	}
}

models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit2
{
	surfaceparm nolightmap
	q3map_lightimage models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit.tga
	q3map_surfacelight 25000
	{
		map models/mapobjects/pad_ddmix/trashcan/lamp_sewer_lit.tga
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
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight.tga
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
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight_small.tga
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
		map models/mapobjects/pad_ddmix/diner/glass_rings.tga
		blendfunc blend
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight.tga
		blendfunc add
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/diner/glass_betty
{
	surfaceparm trans
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_betty.tga
		blendfunc blend
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_betty.tga
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight.tga
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
		map models/mapobjects/pad_ddmix/diner/glass_sunrise.tga
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight.tga
		blendfunc add
		tcGen environment
	}
}


models/mapobjects/pad_ddmix/diner/glass_midnight
{
	surfaceparm trans
	cull disable
	{
		map models/mapobjects/pad_ddmix/diner/glass_midnight.tga
		blendfunc blend
		rgbGen Vertex
		depthWrite
		alphaFunc GE128
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass01.tga
		blendfunc add
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/diner/glass_highlight.tga
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
		map models/mapobjects/pad_ddmix/cabin/telescope.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/cabin/occulars
{
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/cabin/occulars.tga
		tcMod scale 0.8 0.8
		tcGen environment
	}
	{
		map models/mapobjects/pad_ddmix/cabin/occulars2.tga
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
		animmap 10 models/mapobjects/pad_ddmix/effects/FireBigA.tga models/mapobjects/pad_ddmix/effects/FireBigB.tga models/mapobjects/pad_ddmix/effects/FireBigC.tga models/mapobjects/pad_ddmix/effects/FireBigD.tga models/mapobjects/pad_ddmix/effects/FireBigE.tga models/mapobjects/pad_ddmix/effects/FireBigF.tga models/mapobjects/pad_ddmix/effects/FireBigG.tga models/mapobjects/pad_ddmix/effects/FireBigH.tga
		blendfunc add
		rgbGen wave inversesawtooth 1 3 1 3
	}
}

// =================
// GHOSTPAD
// =================

models/mapobjects/pad_ddmix/ghostpad/ghostpad
{
	surfaceparm trans
	{
		map models/mapobjects/pad_ddmix/ghostpad/ghostpad.tga
		blendfunc blend
		rgbGen Vertex
		depthWrite
	}
	{
		map models/mapobjects/pad_ddmix/ghostpad/ghostpad.tga
		blendfunc add
		rgbGen Vertex
	}
	{
		map models/mapobjects/pad_ddmix/ghostpad/ghostpad.tga
		blendfunc add
		rgbGen Vertex
	}
}

// =================
// CASE
// =================

models/mapobjects/pad_ddmix/anteroom/medical_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/medical_case.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/black_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/black_case.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/metal_suitcase
{
	{
		map models/mapobjects/pad_ddmix/anteroom/metal_suitcase.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/anteroom/silver_case
{
	{
		map models/mapobjects/pad_ddmix/anteroom/silver_case.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/padship/shiplamp_light.tga
	}
}

// =================
// VALVE
// =================

models/mapobjects/pad_ddmix/padship/valve
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padship/valve.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad2c.tga
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
		map models/mapobjects/pad_ddmix/attic/pumper.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_ddmix/attic/tubes
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/attic/tubes.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_ddmix/trashcan/tree1_twigs.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}


models/mapobjects/pad_ddmix/trashcan/tree1_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree1_leaves.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree2_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree2_leaves.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree3_twigs
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree3_twigs.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/tree4_leaves
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/tree4_leaves.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/grass
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/grass.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_ddmix/trashcan/bush
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_ddmix/trashcan/bush.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

// =================
// PADKiNG
// =================

//shaders for doomdragons mapobjects for world of padman
models/mapobjects/pad_ddmix/castle/padking
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/padking.tga
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
		map models/mapobjects/pad_ddmix/castle/turul.tga
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
		map models/mapobjects/pad_ddmix/castle/knightshield.tga
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
		map models/mapobjects/pad_cloister/groundplant.tga
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
		map models/mapobjects/pad_cloister/cloister_lamp.tga
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
		map models/mapobjects/pad_cloister/fern_brown.tga
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
		map models/mapobjects/pad_cloister/cane_brown.tga
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
		map models/mapobjects/pad_train/ttoilet.tga
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/ceramic.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/toilet_plastic_shiny
{
	{
		map models/mapobjects/pad_train/ttoilet.tga
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/plastic_shiny.tga
		blendfunc add
		rgbGen vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/toilet_plastic_matte
{
	{
		map models/mapobjects/pad_train/ttoilet.tga
		rgbGen vertex
	}
}

models/mapobjects/pad_train/toilet_metal
{
	{
		map models/mapobjects/pad_train/ttoilet.tga
		rgbGen vertex
	}
	{
		map textures/pad_gfx02/metal_test.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/tdoorhandle
{
	{
		map models/mapobjects/pad_train/tdoorhandle.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_train/ehammer_metal
{
	{
		map models/mapobjects/pad_train/ehammer.tga
		rgbGen Vertex
	}
	{
		map textures/pad_gfx02/metal_test.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/ente/deadalien
{
	{
		map models/mapobjects/ente/deadalien.tga
		rgbGen identity
	}
        {
                map textures/pad_gfx02/tinpad3.tga
		blendfunc add
                rgbGen Vertex
		tcGen environment
        }
        nomipmaps
        {
		map models/mapobjects/ente/deadalien_light.tga
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
		map models/mapobjects/ente/blendomatic.tga
		rgbGen const ( 0.0 0.2 0.6 )
		blendFunc blend
                tcGen environment
	}
        {
		map textures/pad_gfx/glass_dark.tga
                tcgen environment
		blendFunc add
		rgbGen identity
	}
}

models/mapobjects/pad_train/soapdispenser
{
	{
		map models/mapobjects/pad_train/soapdispenser.tga
		rgbGen identity
	}
        {
                map textures/pad_gfx02/tinpad4.tga
		blendfunc add
                rgbGen Vertex
		tcGen environment
        }
}
models/mapobjects/pad_train/soapdispenser_window
{
  {
		map models/mapobjects/pad_train/soapdispenser.tga
		rgbGen identity
	}
  {
    map textures/pad_gfx/glass_bright.tga
		blendfunc add
    rgbGen Vertex
		tcGen environment
  }
}

models/mapobjects/pad_train/ptoweldispenser
{
  cull none
  {
    map models/mapobjects/pad_train/ptoweldispenser.tga
    rgbGen identity
  }
}
models/mapobjects/pad_train/ptoweldispenser_shiny
{
	{
		map models/mapobjects/pad_train/ptoweldispenser.tga
		rgbGen identity
	}
  {
    map textures/pad_gfx02/tinpad4.tga
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
		map textures/pad_gfx02/tinpad3.tga
		blendfunc gl_one gl_one_minus_src_color
		rgbGen identity
		tcGen environment
	}
}

models/mapobjects/pad_actionfigures/fig_pumper
{
	{
		map models/weapons2/pumper/pumper.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_pumpertubes
{
	{
		map models/weapons2/pumper/tubes.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_splasher
{
	{
		map models/weapons2/splasher/splasher.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_frontface
{
	{
		map models/weapons2/splasher/new_frontface.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/fig_splasherwater
{
	{
		map textures/pad_gfx02/padmapplas.jpg
		blendfunc GL_ONE GL_ZERO
		tcMod turb 0 .6 0 .2
		tcmod scale .4 .4
		tcMod scroll .09 -.1.1
		//rgbGen lightingdiffuse //identity
	}
	{
		map textures/pad_gfx02/padmapblue2.jpg
		blendfunc GL_ONE GL_ONE
		tcMod turb 0 .7 0 .3
		tcmod scale .2 .2
		tcMod scroll .1 .09
		//rgbGen lightingdiffuse
	}
	{
		map textures/pad_gfx02/padmapblue.jpg
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
		map models/wop_players/padgirl/padgrlhd.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/padgrlbd
{
	{
		map models/wop_players/padgirl/padgrlbd.tga
		rgbGen Vertex
	}
}

/// ****************
/// *** PadLilly ***
/// ****************


models/mapobjects/pad_actionfigures/lilhead
{
	{
		map models/wop_players/padlilly/lilhead.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_actionfigures/lilbody
{
	{
		map models/wop_players/padlilly/lilbody.tga
		rgbGen Vertex
	}
}


// =================
// WESTERNSET
// =================

models/mapobjects/pad_westernset/colt
{
	{
		map models/mapobjects/pad_westernset/colt.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
		blendfunc add
		rgbGen Vertex
		tcGen environment
	}
}

models/mapobjects/pad_westernset/camera_screens
{
	nomipmaps
	{
		map models/mapobjects/pad_westernset/camera_screens.tga
		tcMod scale 0.8 0.8
		tcGen environment
	}
}

models/mapobjects/pad_westernset/directors_chair_wood
{
	{
		map models/mapobjects/pad_westernset/directors_chair_wood.tga
		rgbGen Vertex
	}
	{
		map $lightmap
		blendfunc filter
		tcGen environment
	}
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_westernset/directors_chair_cloth.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_westernset/actors_chair_cloth
{
	surfaceparm alphashadow
	cull disable
	{
		map models/mapobjects/pad_westernset/actors_chair_cloth.tga
		rgbGen Vertex
		alphaFunc GE128
	}
}

models/mapobjects/pad_westernset/spotlight
{
	cull disable
	{
		map models/mapobjects/pad_westernset/spotlight.tga
		rgbGen Vertex
	}
}

models/mapobjects/pad_westernset/spotlight_front
{
	cull disable
	q3map_surfacelight 60
	{
		map models/mapobjects/pad_westernset/spotlight_front.tga
	}
}


models/mapobjects/pad_westernset/sheriff_badge
{
	cull none

	{
		map models/mapobjects/pad_westernset/sheriff_badge.tga
		rgbGen vertex
        }
	{
		map textures/pad_gfx02/tinpad3.tga
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
		map models/mapobjects/pad_fruits/citrus.tga
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
//    map models/wop_players/padman/default_head.tga
    map models/mapobjects/ente/padwinner_head.tga	//head still needs reference to old padwinner statue texture for correct alignment ~Kai-Li 
  }
}

models/mapobjects/pad_statues/statue_super_hands
{
  {
    map models/wop_players/padman/default_head.tga
  }
}

models/mapobjects/pad_statues/statue_super_shoes
{
  {
    map models/wop_players/padman/default_head.tga
  }
}

models/mapobjects/pad_statues/statue_super_shades
{
  cull none
  {
    map models/wop_players/padman/default_head.tga
  }
}

//// Body
models/mapobjects/pad_statues/statue_super_neck
{
  {
    map models/wop_players/padman/default_body.tga
  }
}

models/mapobjects/pad_statues/statue_super_cape
{
  cull none
  {
    map models/wop_players/padman/default_body.tga
  }
} 

models/mapobjects/pad_statues/statue_super_suit
{
  {
    map models/wop_players/padman/default_body.tga
  }
}

models/mapobjects/pad_statues/statue_super_buckle
{
  {
    map models/wop_players/padman/default_head.tga
  }
}

models/mapobjects/pad_statues/statue_super_button
{
  {
    map models/wop_players/padman/default_head.tga
  }
}


// =================
// SiNGLEPLAYER MAPOBJECTS
// =================

//FERN

models/mapobjects/pad_ddmix/plants/fern_singleplayer
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/fern_singleplayer.tga
		rgbGen exactVertex
		alphaFunc GE128
	}
}

//BONSAi

models/mapobjects/pad_ddmix/plants/bonsai_singleplayer
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/bonsai_singleplayer.tga
		rgbGen exactVertex
		alphaFunc GE128
	}
}


//TOWEL01

models/mapobjects/pad_ddmix/padkitchen/towel01_singleplayer
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel01_singleplayer.tga
		rgbGen exactVertex
	}
}

//TOWEL02

models/mapobjects/pad_ddmix/padkitchen/towel02_singleplayer
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/padkitchen/towel02_singleplayer.tga
		rgbGen exactVertex
	}
}


//KNiGHTSHiELD

models/mapobjects/pad_ddmix/castle/kshield_singleplayer
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/kshield_singleplayer.tga
		rgbGen exactVertex
		alphaFunc GE128
	}
}

//PADKiNG

models/mapobjects/pad_ddmix/castle/padking_singleplayer
{
	cull disable
	{
		map models/mapobjects/pad_ddmix/castle/padking_singleplayer.tga
		rgbGen exactVertex
		alphaFunc GE128
	}
}


//SOCK01

models/mapobjects/pad_clothes/sock01_singleplayer
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock01_singleplayer.jpg
	{
		map models/mapobjects/pad_clothes/sock01_singleplayer.jpg
		rgbGen exactVertex
	}	
}

//SOCK02

models/mapobjects/pad_clothes/sock02_singleplayer
{
    cull disable
	qer_editorimage models/mapobjects/pad_clothes/sock02_singleplayer.jpg
	{
		map models/mapobjects/pad_clothes/sock02_singleplayer.jpg
		rgbGen exactVertex
	}	
}

//TULPi1

models/mapobjects/pad_ddmix/plants/tulip1_singleplayer
{
	surfaceparm alphashadow
	cull disable
	nomipmaps
	{
		map models/mapobjects/pad_ddmix/plants/tulip1_singleplayer.tga
		rgbGen exactVertex
		alphaFunc GE128
	}
}


//SUNFLOWER

//models/mapobjects/pad_sunflower/sunflower_single.md3

models/mapobjects/pad_sunflower/sunhead_single
{
	cull none
	nopicmip
	nomipmaps 
	{
		map models/mapobjects/pad_sunflower/sunhead_single.tga
		alphaFunc GE128
		rgbGen exactvertex
	}
}

models/mapobjects/pad_sunflower/sunleaf1_single
{
	cull none
	nopicmip
	nomipmaps 
	{
		map models/mapobjects/pad_sunflower/sunleaf1_single.tga
		alphaFunc GE128
		rgbGen exactvertex
	}
}

models/mapobjects/pad_sunflower/sunleaf2_single
{
	cull none
	nopicmip
	nomipmaps 
	{
		map models/mapobjects/pad_sunflower/sunleaf2_single.tga
		alphaFunc GE128
		rgbGen exactvertex
	}
}


models/mapobjects/ente/padshop_tft_screen
{
   	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nonsolid
	q3map_lightimage models/mapobjects/pad_pc/pad_tft.tga
	q3map_surfacelight 10
	nopicmip

       	{
               	animmap .3 models/mapobjects/ente/screen01.tga models/mapobjects/ente/screen02.tga models/mapobjects/ente/screen03.tga models/mapobjects/ente/screen04.tga models/mapobjects/ente/screen05.tga
               	rgbgen identity
       	}
	{
		map models/mapobjects/padtv/scanline.tga
		blendfunc blend
		rgbGen identity
		tcMod scroll 0 -0.035
		tcMod scale 45 45
	}
	{
		map textures/pad_gfx02/tinpad3.tga
        	tcGen environment
                blendfunc GL_ONE GL_ONE
                rgbGen identity
	}
}