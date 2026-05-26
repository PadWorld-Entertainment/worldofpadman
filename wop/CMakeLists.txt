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
    wop_buddyfriendguy.shader
    wop_hectic.shader
    wop_iltis.shader
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
    wop_7colors.bsp
    wop_alongwayCTL.bsp
    wop_cheesedome.bsp
    wop_coloor.bsp
    wop_copyandpaste.bsp
    wop_cubefight.bsp
    wop_darkplace.bsp
    wop_darkplaceCTL.bsp
    wop_flyordie.bsp
    wop_fragbox.bsp
    wop_hurryup.bsp
    wop_hurryupCTL.bsp
    wop_pacman.bsp
    wop_tinyCTL.bsp
    wop_workshop.bsp
)
foreach (BSP ${MAPPACK_BSPS})
    bsp_validation(wop ${BSP} 2)
endforeach()

separate_arguments(WOPBSPC_OPTIONS_LIST NATIVE_COMMAND "${WOPBSPC_OPTIONS}")

set(UPDATE_AAS_OUTPUTS)

function(add_update_aas_commands MAP_DIR)
    string(REPLACE "/" "_" MAP_DIR_TAG "${MAP_DIR}")

    foreach (BSP ${ARGN})
        set(BSP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${MAP_DIR}/${BSP}")
        get_filename_component(BSP_NAME_WE "${BSP}" NAME_WLE)
        set(AAS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${MAP_DIR}/${BSP_NAME_WE}.aas")
        set(AAS_WORK_DIR "${CMAKE_CURRENT_BINARY_DIR}/update-aas-work/${MAP_DIR_TAG}/${BSP_NAME_WE}")
        set(AAS_LOG_FILE "${CMAKE_CURRENT_BINARY_DIR}/update-aas-logs/${MAP_DIR_TAG}-${BSP_NAME_WE}.log")

        add_custom_command(
            OUTPUT "${AAS_PATH}"
            COMMAND ${CMAKE_COMMAND}
                -DBSPC_EXECUTABLE=$<TARGET_FILE:wopbspc>
                "-DBSPC_OPTIONS:STRING=${WOPBSPC_OPTIONS}"
                "-DBSP_INPUT:STRING=${BSP_PATH}"
                "-DWORK_DIR:STRING=${AAS_WORK_DIR}"
                "-DLOG_FILE:STRING=${AAS_LOG_FILE}"
                -P "${ROOT_DIR}/cmake/run_wopbspc_with_log.cmake"
            DEPENDS wopbspc "${BSP_PATH}"
            COMMENT "Update AAS ${MAP_DIR}/${BSP_NAME_WE}.aas (log: ${AAS_LOG_FILE})"
            VERBATIM
        )

        list(APPEND UPDATE_AAS_OUTPUTS "${AAS_PATH}")
    endforeach()

    set(UPDATE_AAS_OUTPUTS "${UPDATE_AAS_OUTPUTS}" PARENT_SCOPE)
endfunction()

add_update_aas_commands("maps.pk3dir/maps" ${BSPS})
add_update_aas_commands("padpack.pk3dir/maps" ${PADPACK_BSPS})
add_update_aas_commands("mappack.pk3dir/maps" ${MAPPACK_BSPS})

add_custom_target(update-aas
    DEPENDS ${UPDATE_AAS_OUTPUTS}
    COMMENT "Update AAS files for all BSP maps"
)
