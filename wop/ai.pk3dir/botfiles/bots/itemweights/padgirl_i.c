// personalized itemweights, not used atm: see fw_items.c
#include "inv.h"

//initial health/armor states
#define FS_HEALTH				1
#define FS_ARMOR				2

//initial weapon weights
#define W_SHOTGUN				500
#define W_MACHINEGUN			70
#define W_GRENADELAUNCHER		140
#define W_ROCKETLAUNCHER		320
#define W_RAILGUN				85
#define W_BFG10K				30
#define W_LIGHTNING				50
#define W_PLASMAGUN				100

//initial powerup weights
#define W_PADPOWER				400
#define W_CLIMBER				100
#define W_SPEEDY				280
#define W_JUMPER				80
#define W_VISIONLESS			230
#define W_REVIVAL				180
#define W_FLOATER				 80
#define W_KILLERDUCKS			250

// now run the fuzz relations
#include "fw_items.c"
