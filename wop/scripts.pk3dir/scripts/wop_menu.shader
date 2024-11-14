// =================
// MAiN MENU BG FX
// =================

menu/bg/mainbgfx
{
	nomipmaps
	{
		map menu/bg/main_bgfx
		tcMod rotate 7
		tcMod turb .1 .1 .1 .1
		alphaGen Vertex
	}
}

// =================
// OTHER MENU BG FX
// =================

menu/bg/menubgfx
{
	nomipmaps
	{
		map menu/bg/menu_bgfx
		tcMod rotate 7
		tcMod turb .1 .1 .1 .1
	}
}

// =================
// CONSOLE BG FX
// =================

console
{
	nopicmip
	nomipmaps
    {
        map menu/bg/consoleback
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        alphaGen vertex
        tcMod rotate 8	//scroll 0.01 0.02
		tcMod turb .1 .2 .1 .1
	//tcMod stretch sin .75 1 .75 .1
	//tcMod scroll 0.01 0.02
	//tcMod scale 2 1
    }
    {
        map menu/bg/consoletitle
        blendFunc GL_ONE GL_ONE
        tcMod rotate -5
    }
}
