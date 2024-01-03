// World of PADMAN skyboxes shader

// =================
// Huette skybox
// =================

textures/pad_harm/padcity
{
    qer_editorimage env/padcity512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_ffffffff
    q3map_sun 0.266383 0.274632 0.358662 100 50 55
    q3map_surfacelight 100
    skyparms env/padcity512 - -
}

// =================
// Jail skybox
// =================

textures/pad_jail/pad_jail_envcolor
{
    qer_editorimage env/padcity-hilton512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_d1a76bff
    q3map_globaltexture
    q3map_sunExt 0.68 0.82 0.86 40 -300 27.4 2 1
    q3map_lightRGB 0.7 0.8 1.0
    q3map_lightmapFilterRadius 0 64
    q3map_skyLight 38 3
    skyparms env/padcity-hilton512 - -
}

textures/pad_jail/pad_jail_sky4
{
    qer_editorimage env/jail_sprayroom3_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_globaltexture
    q3map_sunExt 0.68 0.82 0.86 40 -300 27.4 2 1
    q3map_lightRGB 0.7 0.8 1.0
    q3map_lightmapFilterRadius 0 64
    q3map_skyLight 38 3
    skyparms env/jail_sprayroom3 - -
}

// =================
// PadCloister skybox
// =================

textures/pad_fountain/utopiaatoll
{
    qer_editorimage env/utopiaatoll512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_dbac7fff
    q3map_sun 1.000000 0.828885 0.672267 220 100 90
    q3map_surfacelight 300
    skyparms env/utopiaatoll512 - -
}

// =================
// PadGarden skybox
// =================

textures/pad_garden/fort-pad
{
    qer_editorimage env/fort-pad512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_ffffe5ff
    q3map_sun 1.000000 0.913939 0.701350 390 220 40
    q3map_surfacelight 250
    skyparms env/fort-pad512 - -
}

textures/pad_garden/polter_geist
{
    qer_editorimage env/pf-garden512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_1c1ac7ff
    q3map_sun 0.404654 0.474220 1.000000 330 220 50
    q3map_surfacelight 160
    skyparms env/pf-garden512 - -
}

// =================
// PadKitchen skybox
// =================

textures/pad_kitchen/urban-chaos
{
    qer_editorimage env/urban-chaos512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_ffffffff
    q3map_sun 0.266383 0.274632 0.358662 100 50 55
    q3map_surfacelight 240
    skyparms env/urban-chaos512 - -
}

textures/pad_petesky/urban-terror
{
    qer_editorimage env/urban-terror512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_ff9860ff
    q3map_sun 1.000000 0.718471 0.616587 200 60 225
    q3map_surfacelight 200
    skyparms env/urban-terror512 - -
}

// =================
// PadLibrary skybox
// =================

textures/pad_petesky/sep
{
    qer_editorimage env/sep512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_ff6600ff
    q3map_sun 0.266383 0.274632 0.358662 94 48 75
    q3map_surfacelight 280
    skyparms env/sep512 - -
}

textures/pad_bookroom/frost-bite
{
    qer_editorimage env/frost-bite512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_5b88c8ff
    q3map_sun 0.754360 0.959199 1.000000 300 50 55
    q3map_surfacelight 300
    skyparms env/frost-bite512 - -
}

// =================
// PadShop skybox
// =================

textures/pad_shop/padnight
{
    qer_editorimage env/mercury512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_cc0000ff
    q3map_sun 1 1 1 100 -58 58
    q3map_surfacelight 100
    skyparms env/mercury512 - -
    {
        map textures/pad_shop/skystuff
        blendfunc GL_ONE GL_ONE
        tcMod scroll 0.07 0.09
        tcMod scale 3 2
    }
}

textures/pad_shop/pc-ground-zero512
{
    qer_editorimage env/pc-ground-zero512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_lightimage textures/colors/hex_2f8fcfff
    q3map_sun 0.266383 0.274632 0.358662 20 50 55
    q3map_surfacelight 300
    skyparms env/pc-ground-zero512 - -
}

// =================
// TrashMap skybox
// =================

textures/pad_trash/trash_skybox_night
{
    qer_editorimage env/pc-friday-13th-512_ft
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_sun 0.47451 0.576471 1 55 210 60
    skyParms env/pc-friday-13th-512 128 -
}

textures/pad_trash/trash_skybox_day
{
    qer_editorimage env/padcity512_ft
    q3map_lightimage textures/colors/hex_ffffe5ff
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_sun 1 1 0.9 150 210 68
    q3map_surfacelight 300
    skyParms env/padcity512 128 -
}

textures/pad_trash/trash_skybox_evening
{
    qer_editorimage env/pc-morning-madness512_ft
    q3map_lightimage textures/colors/hex_ff9860ff
    surfaceparm noimpact
    surfaceparm nolightmap
    surfaceparm sky
    q3map_surfacelight 200
    skyparms env/pc-morning-madness512 128 -
}
