// Iltis' maps shader

// ==============
// Dark Place
// ==============

textures/iltis/nebel_schwarz
{
    qer_editorimage textures/hectic/abgas
    qer_nocarve
    surfaceparm nonsolid
    surfaceparm trans
    surfaceparm nolightmap
    surfaceparm fog
    fogparms ( .01 .01 .01 ) 5000
}

// ==============
// Marble Maze
// ==============

textures/iltis/holz1
{
    surfaceparm woodsteps
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/iltis/holz1
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
}

textures/iltis/holz2
{
    surfaceparm woodsteps
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/iltis/holz2
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
}

textures/iltis/holz3
{
    surfaceparm woodsteps
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/iltis/holz3
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
}

textures/iltis/pfeil
{
    qer_editorimage textures/iltis/pfeil
    surfaceparm woodsteps
    {
        map textures/iltis/holz1
        tcMod scale 5 5
    }
    {
        map textures/iltis/pfeil
        blendfunc blend
    }
    {
        map $lightmap
        blendfunc filter
    }
}

textures/iltis/pfeil_blau
{
    qer_editorimage textures/iltis/pfeil_blau
    surfaceparm woodsteps
    {
        map textures/iltis/holz1
        tcMod scale 5 5
    }
    {
        map textures/iltis/pfeil_blau
        blendfunc blend
    }
    {
        map $lightmap
        blendfunc filter
    }
}

textures/iltis/pfeil_rot
{
    qer_editorimage textures/iltis/pfeil_rot
    surfaceparm woodsteps
    {
        map textures/iltis/holz1
        tcMod scale 5 5
    }
    {
        map textures/iltis/pfeil_rot
        blendfunc blend
    }
    {
        map $lightmap
        blendfunc filter
    }
}
