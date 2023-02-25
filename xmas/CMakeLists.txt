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

set(SKINS
    special/BlueBalloon_vertex.skin
    special/RedBalloon_vertex.skin
    wop_players/fatpad/upper_red.skin
    wop_players/fatpad/lower_red.skin
    wop_players/piratpad/upper_red.skin
    wop_players/piratpad/head_blue.skin
    wop_players/padman/upper_red.skin
    wop_players/padman/lower_red.skin
    wop_players/padgirl/upper_red.skin
    wop_players/padgirl/lower_red.skin
    wop_players/padlilly/upper_red.skin
    wop_players/padlilly/lower_red.skin
)
foreach (SKIN ${SKINS})
    skin_validation(xmas ${SKIN})
endforeach()
