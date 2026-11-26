#include "vkimpl.h"
#include "vk_instance.h"
#include "tr_config.h"
#include "tr_cvar.h"

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_icdGetInstanceProcAddr( VkInstance instance, const char *name );

static cvar_t *r_ps5Height;
static cvar_t *r_ps5Refresh;

void vk_getInstanceProcAddrImpl( void )
{
	ri.Printf( PRINT_ALL, " *** Vulkan Initialization (PS5 RADV) ***\n" );
	qvkGetInstanceProcAddr = vk_icdGetInstanceProcAddr;
	if ( qvkGetInstanceProcAddr == NULL ) {
		ri.Error( ERR_FATAL, "Failed to find entrypoint vk_icdGetInstanceProcAddr" );
	}
}

void vk_createWindow( void )
{
	int height;
	int width;
	R_glConfigInit();

	ri.Printf( PRINT_ALL, "\n...Creating PS5 display window...\n" );
	r_ps5Height = ri.Cvar_Get( "r_ps5Height", "2160", CVAR_ARCHIVE );
	r_ps5Refresh = ri.Cvar_Get( "r_ps5Refresh", "0", CVAR_ARCHIVE | CVAR_LATCH );
	ri.Cvar_Get( "r_availableModes", "3840x2160 2560x1440 1920x1080", CVAR_ROM );

	height = r_ps5Height->integer;
	if ( r_mode->integer == -1 ) {
		height = ri.Cvar_VariableIntegerValue( "r_customheight" );
	}
	if ( height != 1080 && height != 1440 && height != 2160 ) {
		height = 2160;
	}
	width = height * 16 / 9;

	R_SetWinMode( -2, width, height, 60 );
	ri.IN_Init( NULL );
}

void vk_createSurfaceImpl( void )
{
#define ICD( name ) PFN_##name q##name = (PFN_##name)vk_icdGetInstanceProcAddr( vk.instance, #name ); \
	if ( !q##name ) { ri.Error( ERR_FATAL, "vk_createSurfaceImpl: missing " #name ); }
	ICD( vkEnumeratePhysicalDevices )
	ICD( vkGetPhysicalDeviceDisplayPropertiesKHR )
	ICD( vkGetDisplayModePropertiesKHR )
	ICD( vkCreateDisplayPlaneSurfaceKHR )
#undef ICD

	VkPhysicalDevice device;
	VkDisplayPropertiesKHR display;
	VkDisplayModePropertiesKHR modes[32];
	VkDisplaySurfaceCreateInfoKHR desc;
	uint32_t count, i, pick;
	VkResult res;

	count = 1;
	res = qvkEnumeratePhysicalDevices( vk.instance, &count, &device );
	if ( ( res != VK_SUCCESS && res != VK_INCOMPLETE ) || count == 0 ) {
		ri.Error( ERR_FATAL, "vk_createSurfaceImpl: no physical device (%d)", (int)res );
	}
	count = 1;
	res = qvkGetPhysicalDeviceDisplayPropertiesKHR( device, &count, &display );
	if ( ( res != VK_SUCCESS && res != VK_INCOMPLETE ) || count == 0 ) {
		ri.Error( ERR_FATAL, "vk_createSurfaceImpl: no display (%d)", (int)res );
	}
	count = ARRAY_LEN( modes );
	res = qvkGetDisplayModePropertiesKHR( device, display.display, &count, modes );
	if ( ( res != VK_SUCCESS && res != VK_INCOMPLETE ) || count == 0 ) {
		ri.Error( ERR_FATAL, "vk_createSurfaceImpl: no display mode (%d)", (int)res );
	}

	pick = count;
	for ( i = 0; i < count; i++ ) {
		const VkDisplayModeParametersKHR *mp = &modes[i].parameters;
		ri.Printf( PRINT_ALL, "display mode %u: %ux%u at %.2f Hz\n", i, mp->visibleRegion.width, mp->visibleRegion.height, mp->refreshRate / 1000.0 );
		if ( pick != count ) continue;
		int curW = 0, curH = 0; R_GetWinResolution( &curW, &curH );
		if ( (int)mp->visibleRegion.height != curH ) continue;
		if ( r_ps5Refresh->integer > 0 && mp->refreshRate > (uint32_t)( r_ps5Refresh->integer * 1000 + 500 ) ) continue;
		pick = i;
	}
	if ( pick == count ) pick = 0;

	memset( &desc, 0, sizeof( desc ) );
	desc.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
	desc.displayMode = modes[pick].displayMode;
	desc.planeIndex = 0;
	desc.planeStackIndex = 0;
	desc.transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	desc.globalAlpha = 1.0f;
	desc.alphaMode = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR;
	desc.imageExtent = modes[pick].parameters.visibleRegion;

	res = qvkCreateDisplayPlaneSurfaceKHR( vk.instance, &desc, NULL, &vk.surface );
	if ( res != VK_SUCCESS ) {
		ri.Error( ERR_FATAL, "vk_createSurfaceImpl: vkCreateDisplayPlaneSurfaceKHR failed (%d)", (int)res );
	}

	unsigned int hz = ( modes[pick].parameters.refreshRate + 500 ) / 1000;
	R_SetWinMode( -2, desc.imageExtent.width, desc.imageExtent.height, hz );
	ri.Printf( PRINT_ALL, "PS5 display: mode %u, %ux%u at %u Hz\n", pick, desc.imageExtent.width, desc.imageExtent.height, hz );
}

void vk_destroyWindow( void )
{
	ri.IN_Shutdown();
}

void vk_minimizeWindow( void )
{
}
