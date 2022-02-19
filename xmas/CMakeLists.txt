set(PK3DIRS
    scripts
    models
    gfx
    menu
    textures
    maps
    music
)
compress_pk3(xmas ${PK3DIRS})

set(SHADERS
    freeztag.shader
    healthstation.shader
    lightstringhat.shader
    monster.shader
    pad_armour.shader
    pad_balloonbox.shader
    pad_garden.shader
    padmodel.shader
    pad_teleporter.shader
    pad_waterbottle.shader
    sometestlogos.shader
    spraypistol.shader
)
foreach (SHADER ${SHADERS})
    shader_validation(xmas ${SHADER})
endforeach()

set(BSPS
    weinacht.bsp
)
foreach (BSP ${BSPS})
    bsp_validation(xmas ${BSP})
endforeach()
