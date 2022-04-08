set(SRCS
	matrix_multiplication.c
	tr_globals.c
	tr_cvar.c
	tr_animation.c
	tr_bsp.c
	tr_cmds.c
	tr_curve.c
	tr_image.c
	tr_findshader.c
	tr_listshader.c
	tr_imageprocess.c
	tr_init.c
	tr_light.c
	tr_main.c
	tr_marks.c
	tr_mesh.c
	tr_model.c
	tr_model_iqm.c
	tr_registermodel.c
	tr_modelbounds.c
	tr_loadmd3.c
	tr_loadmdr.c
	tr_lerptag.c
	tr_scene.c
	tr_shade.c
	tr_shade_calc.c
	tr_shader.c
	tr_shadows.c
	tr_sky.c
	tr_surface.c
	tr_flares.c
	tr_fog.c
	tr_world.c
	vk_instance.c
	vk_init.c
	vk_cmd.c
	vk_image.c
	vk_image_sampler2.c
	vk_pipelines.c
	vk_frame.c
	vk_swapchain.c
	vk_screenshot.c
	vk_shade_geometry.c
	vk_depth_attachment.c

	vk_shaders.c

	tr_stretchraw.c
	tr_debuggraphics.c
	tr_showimages.c
	tr_drawnormals.c
	tr_drawtris.c
	tr_surfaceanim.c
	tr_cull.c
	tr_config.c
	tr_parser.c
	tr_portalplane.c
	tr_printmat.c

	tr_loadimage.c

	ref_import.c
	render_export.c

	vk_create_window_sdl.c

	../renderercommon/tr_font.c
	../renderercommon/tr_image_bmp.c
	../renderercommon/tr_image_jpg.c
	../renderercommon/tr_image_pcx.c
	../renderercommon/tr_image_png.c
	../renderercommon/tr_image_tga.c
	tr_noise.c
)

set(SHADER_SRCS
	shaders/multi_texture_add.frag
	shaders/multi_texture_clipping_plane.vert
	shaders/multi_texture_mul.frag
	shaders/multi_texture.vert
	shaders/single_texture_clipping_plane.vert
	shaders/single_texture.vert
	shaders/single_texture.frag
)

foreach (shader ${SHADER_SRCS})
	get_filename_component(shaderfile ${shader} NAME_WE)
	get_filename_component(shadertype ${shader} EXT)
	string(SUBSTRING ${shadertype} 1 -1 shadertype)

	set(spirvext vspv)
	if (${shadertype} STREQUAL "frag")
		set(spirvext fspv)
	endif()
	set(spirvfile ${shaderfile}_${shadertype}.${spirvext})
	set(cfile ${shaderfile}_${shadertype}.c)

	list(APPEND SRCS ${cfile})
	list(APPEND SRCS ${spirvfile})
	set_source_files_properties(${cfile} PROPERTIES GENERATED TRUE)
	set_source_files_properties(${spirvfile} PROPERTIES GENERATED TRUE)

	add_custom_command(OUTPUT ${spirvfile}
		COMMAND ${GLSL_LANG_VALIDATOR} --quiet -V ${CMAKE_CURRENT_SOURCE_DIR}/${shader} -o ${CMAKE_CURRENT_BINARY_DIR}/${spirvfile}
		COMMENT "Generate spirv for ${shader} (${shadertype}): ${spirvfile}"
	)
	add_custom_command(OUTPUT ${cfile}
		DEPENDS bintoc ${spirvfile}
		COMMAND bintoc ${CMAKE_CURRENT_BINARY_DIR}/${spirvfile} ${CMAKE_CURRENT_BINARY_DIR}/${cfile}
		COMMENT "Generate c source for ${shader} (${shadertype}): ${cfile}"
	)
endforeach()

if (USE_RENDERER_DLOPEN)
	list(APPEND SRCS
		../qcommon/q_shared.c
		../qcommon/puff.c
		../qcommon/q_math.c
	)
endif()

project(renderer_vulkan)
wop_add_library(TARGET ${PROJECT_NAME} SRCS ${SRCS})
set(LIBS jpeg SDL2::SDL2)
if (MSVC)
	list(APPEND LIBS ws2_32 winmm psapi gdi32 ole32)
endif()
target_link_libraries(${PROJECT_NAME} ${LIBS})

set(RENDERER_DEFINES)
if (USE_RENDERER_DLOPEN)
	list(APPEND RENDERER_DEFINES USE_RENDERER_DLOPEN)
endif()
if (RENDERER_DEFINES)
	target_compile_definitions(${PROJECT_NAME} PRIVATE ${RENDERER_DEFINES})
endif()
