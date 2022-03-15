// World of PADMAN common shader
// Contains mostly shaders for map editor support and q3map2 features

// =================
// ALPHA FADE
// =================

textures/common/alpha_000		// primary texture ONLY
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/alpha_025
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 0.25
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/alpha_050		// perfect mix of both, primary AND secondary texture
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/alpha_075
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 0.75
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/alpha_085
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 0.85
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

textures/common/alpha_100		// secondary texture ONLY
{
	qer_trans 0.5
	q3map_alphaMod volume
	q3map_alphaMod scale 1.0
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
}

// =================
// CAULK
// =================

textures/common/caulk			// solid and invisible
{
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nomarks
}

textures/common/lavacaulk		// use on faces between or overlap lave brushes
{
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm lava
}

textures/common/slimecaulk		// use on faces between or overlap slime brushes
{
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm slime
}

textures/common/watercaulk		// use on faces between or overlap water brushes
{
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm water
}

// =================
// CLiP & CUSHiON
// =================

textures/common/botclip			// blocks ONLY bots
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm botclip
}

textures/common/carpetclip		// causes footstep sounds like walking on carpet
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm carpetsteps
}

textures/common/clip			// blocks bots AND players
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
}

textures/common/cushion			// disables falling damage and landing sounds
{
	qer_nocarve
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nodamage
}

textures/common/full_clip		// quite useless, better use weapclip instead
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm playerclip
}

textures/common/latticeclip		// causes footstep sounds like walking on lattice
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm latticesteps
}

textures/common/metalclip		// causes footstep sounds like walking on metal
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm metalsteps
}

textures/common/missileclip		// quite useless, use clip instead
{
	qer_trans 0.40
	surfaceparm nodamage
	surfaceparm nomarks
	surfaceparm nodraw
	surfaceparm playerclip
	surfaceparm trans
}

textures/common/sandclip		// causes footstep sounds like walking on sand or dirt
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm sandsteps
}

textures/common/snowclip		// causes footstep sounds like walking on snow
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm snowsteps
}

textures/common/softclip		// causes footstep sounds like walking on soft surfaces
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm softsteps
}

textures/common/splashclip		// causes footstep sounds like walking on water surfaces, like a puddle
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm splashsteps
}

textures/common/weapclip		// blocks bots, players, AND bullets, replaces full_clip
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
}

textures/common/woodclip		// causes footstep sounds like walking on wood
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm woodsteps
}

// =================
// DONOT & NO
// =================

textures/common/donotenter		// bots are not blocked, but they avoid and try to leave those areas
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm donotenter
}

textures/common/nodraw			// makes surfaces invisible and nonsolid, not drawn at all
{
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
}

textures/common/nodrawnonsolid	// quite useless, better use nodraw instead
{
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nodraw
}

textures/common/nodrop			// items do not fall on this surface, useful for pitfalls and space maps
{
	qer_nocarve
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nodrop
}

textures/common/noimpact		// seems not to have any use
{
	qer_editorimage textures/common/nolightmap
	surfaceparm noimpact
}

textures/common/nolightmap		// seems not to have any use
{
	surfaceparm nolightmap
}

// =================
// PORTAL & HiNT
// =================

textures/common/antiportal		// works like hint, but supresses portals
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
	surfaceparm antiportal
}

textures/common/areaportal		// used for vis calculation to hide areas behind closed doors
{
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm areaportal
}

textures/common/clusterportal	// works like hint, but still fragments the AAS navigation file
{
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm clusterportal
}

textures/common/hint			// used for vis calculation to hide areas not in the direct field of view
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
	surfaceparm hint
}

textures/common/hintlocal		// works like hint, but does not create portals outside local structure brushes
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
}

textures/common/skip			// works like quake 2 hint, it doesn't generate bsp splits
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
	surfaceparm skip
}

textures/common/hintskip		// same as skip, but name changed to allow Radiant to filter both hint and skip
{
	qer_editorimage textures/common/skip
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
	surfaceparm skip
}

// =================
// OTHER SURFACES
// =================

textures/common/invisible		// solid and transparent polygons, which casts shadows
{
	surfaceparm nolightmap
	{
		map textures/common/invisible
		alphaFunc GE128
		depthWrite
		rgbGen vertex
    }
}

textures/common/mirror1			// clear mirror surface, use with misc_portal_surface enity
{
	qer_editorimage textures/common/mirror
	surfaceparm nolightmap
	portal
	{
		map textures/common/mirror1
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}

textures/common/mirror2			// dirty mirror surface, use with misc_portal_surface enity
{
	qer_editorimage textures/common/mirror
	surfaceparm nolightmap
	portal
	{
		map textures/common/mirror1
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
    {
		map textures/sfx/mirror
		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
    }
}

textures/common/portal			// same as mirror1
{
	qer_editorimage textures/common/mirror
	surfaceparm nolightmap
	portal
	{
		map textures/common/mirror1
		tcMod turb 0 0.25 0 0.05
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite

	}
}

textures/common/slick			// slippery surface, makes players slide
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm slick
}

textures/common/terrain			// used for old style terrain (outdated)
{
	q3map_terrain
	surfaceparm nodraw
    surfaceparm nolightmap
	surfaceparm nomarks
}

textures/common/terrain2		// used for old style terrain (outdated)
{
	qer_editorimage textures/common/terrain
	q3map_terrain
	surfaceparm dust
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nolightmap
}

textures/common/origin			// assigned to origin brushes
{
	qer_nocarve
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm origin
}

textures/common/trigger			// assigned to trigger brushes
{
	qer_trans 0.50
	qer_nocarve
	surfaceparm nodraw
}

textures/common/lightgrid		// min/max bounds of brushes with this shader in a map will define the bounds of the map's lightgrid
{
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm detail
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm lightgrid
}
