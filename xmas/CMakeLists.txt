set(PK3DIRS
    gfx
    maps
    menu
    models
    music
    scripts
    textures
)
compress_pk3(xmas ${PK3DIRS})

set(SHADERS
    pad_garden.shader
    wop_xmas.shader
)
foreach (SHADER ${SHADERS})
    shader_validation(xmas ${SHADER} 0)
endforeach()

set(BSPS
    weinacht.bsp
)
foreach (BSP ${BSPS})
    bsp_validation(xmas ${BSP} 0)
endforeach()
