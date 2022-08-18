textures/pad_western/western_alpha_000   // Primary texture ONLY
{
   qer_editorimage textures/common/alpha_000

   q3map_alphaMod volume
   q3map_alphaMod set 0.00
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}

textures/pad_western/western_alpha_025
{
   qer_editorimage textures/common/alpha_025

   q3map_alphaMod volume
   q3map_alphaMod set 0.25
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}

textures/pad_western/western_alpha_050   // Perfect mix of both Primary + Secondary
{
   qer_editorimage textures/common/alpha_050

   q3map_alphaMod volume
   q3map_alphaMod set 0.50
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}

textures/pad_western/western_alpha_075
{
   qer_editorimage textures/common/alpha_075

   q3map_alphaMod volume
   q3map_alphaMod set 0.75
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}

textures/pad_western/western_alpha_085
{
   qer_editorimage textures/common/alpha_085

   q3map_alphaMod volume
   q3map_alphaMod set 0.85
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}

textures/pad_western/western_alpha_100   // Secondary texture ONLY
{
   qer_editorimage textures/common/alpha_100

   q3map_alphaMod volume
   q3map_alphaMod set 1.0
   surfaceparm nodraw
   surfaceparm nonsolid
   surfaceparm trans
   qer_trans 0.75
}



//====================================
//Leinwände geben etwas Licht ab
//====================================

textures/pad_harm/utopiaatoll512_ft
{
   qer_editorimage env/utopiaatoll512_ft
   q3map_surfacelight 150
   surfaceparm nolightmap
   {
      map env/utopiaatoll512_ft
   }
}

textures/pad_harm/utopiaatoll512_lf
{
   qer_editorimage env/utopiaatoll512_lf
   q3map_surfacelight 150
   surfaceparm nolightmap
   {
      map env/utopiaatoll512_lf
   }
}


//=======================================================
//Notausgang
//=======================================================

textures/pad_harm/notausgang
{
    q3map_surfacelight 300
    surfaceparm nolightmap
    {
        map textures/pad_harm/notausgang
    }
}


textures/pad_harm/notausganglight
{
    q3map_surfacelight 300
    surfaceparm nolightmap
    {
        map textures/pad_harm/notausganglight
    }
}


//=======================================================
// Alphachannel
//=======================================================
textures/pad_western/studioa
{
     surfaceparm nomarks
     surfaceparm trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_western/studioa
               	blendFunc blend
		rgbGen vertex
	}
}

textures/pad_western/studiob
{
     surfaceparm nomarks
     surfaceparm trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_western/studiob
               	blendFunc blend
		rgbGen vertex
	}
}

textures/pad_western/dressingroom
{
     surfaceparm nomarks
     surfaceparm trans
     surfaceparm pointlight
     polygonOffset
        {
		map textures/pad_western/dressingroom
               	blendFunc blend
		rgbGen vertex
	}
}
