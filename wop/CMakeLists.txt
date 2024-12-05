set(PK3DIRS
    botfiles
    gfx
    mappack
    maps
    menu
    models
    music
    padpack
    scripts
    sound
    textures
    video
)
compress_pk3(wop ${PK3DIRS})

set(SHADERS
    common.shader
    pad_airplane.shader
    pad_asia.shader
    pad_attic.shader
    pad_bookroom.shader
    pad_cloister.shader
    pad_cyb.shader
    pad_effects.shader
    pad_fridge.shader
    pad_garden.shader
    pad_gfx.shader
    pad_glowmaps.shader
    pad_harm.shader
    pad_jail.shader
    pad_journey.shader
    pad_kitchen.shader
    pad_mapobjects.shader
    pad_pirate.shader
    pad_shop.shader
    pad_skyboxes.shader
    pad_steps.shader
    pad_tex02.shader
    pad_trash.shader
    q3map2_wop_diner.shader
    q3map2_wop_dinerBB.shader
    q3map2_wop_trashmap.shader
    wop_code.shader
    wop_gamemodels.shader
    wop_glowmodels.shader
    wop_icons.shader
    wop_levelshots.shader
    wop_menu.shader
    wop_playermodels.shader
    wop_powerups.shader
    wop_spaylogos.shader
    wop_weapons.shader
    wop_weaponsfx.shader
)
foreach (SHADER ${SHADERS})
    shader_validation(wop ${SHADER} 0)
endforeach()

set(PADPACK_SHADERS
    wop_padpack.shader
    wop_padpack-levelshots.shader
    wop_padpack-skyboxes.shader
)
foreach (SHADER ${PADPACK_SHADERS})
    shader_validation(wop ${SHADER} 1)
endforeach()

set(MAPPACK_SHADERS
    wop_mappack-levelshots.shader
)
foreach (SHADER ${MAPPACK_SHADERS})
    shader_validation(wop ${SHADER} 2)
endforeach()

set(BSPS
    wop_airplane.bsp
    wop_anteroom.bsp
    wop_anteroomBB.bsp
    wop_backyard.bsp
    wop_backyardBB.bsp
    wop_bath.bsp
    wop_bathBB.bsp
    wop_cabin.bsp
    wop_cabinBB.bsp
    wop_colorstageCTL.bsp
    wop_diner.bsp
    wop_dinerBB.bsp
    wop_fridgeCTL.bsp
    wop_huette.bsp
    wop_huetteBB.bsp
    wop_jail.bsp
    wop_jailBB.bsp
    wop_journeyCTL.bsp
    wop_padasiaCTL.bsp
    wop_padattic.bsp
    wop_padatticBB.bsp
    wop_padatticLPS.bsp
    wop_padbaseCTL.bsp
    wop_padboxCTL.bsp
    wop_padcloisterCTL.bsp
    wop_padcrash.bsp
    wop_padcrash_dm17.bsp
    wop_padcrashBB.bsp
    wop_padcrashCTL.bsp
    wop_padgarden.bsp
    wop_padgarden_night.bsp
    wop_padgardenBB.bsp
    wop_padkitchen.bsp
    wop_padkitchenBB.bsp
    wop_padkitchenLPS.bsp
    wop_padlibrary.bsp
    wop_padlibraryBB.bsp
    wop_padlibraryLPS.bsp
    wop_padship.bsp
    wop_padshipBB.bsp
    wop_padshop.bsp
    wop_padshopBB.bsp
    wop_trashmap.bsp
    wop_trashmapBB.bsp
    wop_trashmapCTL.bsp
    wop_westernsetCTL.bsp
)
foreach (BSP ${BSPS})
    bsp_validation(wop ${BSP} 0)
endforeach()

set(PADPACK_BSPS
    wop_padcastle.bsp
    wop_padcastleBB.bsp
    wop_padgallery.bsp
    wop_padgalleryBB.bsp
)
foreach (BSP ${PADPACK_BSPS})
    bsp_validation(wop ${BSP} 1)
endforeach()

set(MAPPACK_BSPS
    wop_coloor.bsp
    wop_fragbox.bsp
)
foreach (BSP ${MAPPACK_BSPS})
    bsp_validation(wop ${BSP} 2)
endforeach()
