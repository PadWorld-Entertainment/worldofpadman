#include "../sys/sys_local.h"
#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../client/client.h"
#include "../qcommon/vm_local.h"
#include "../wiiu/wiiu_osk.h"

/* WoP Discord webhook stubs */
int DISCORD_Init(void) { return 0; }
void DISCORD_Close(void) {}
int DISCORD_EnqueueMessage(const char *user, const char *message) {
    (void)user;
    (void)message;
    return 0;
}

/* WoP HTTP stats/update stubs */
int HTTP_Init(void) { return 0; }
void HTTP_Close(void) {}
void HTTP_SetTimeouts(int connectTimeout, int timeout) {
    (void)connectTimeout;
    (void)timeout;
}

/* Home-state file write fallback on Wii U SD card */
fileHandle_t FS_FOpenFileWrite_HomeState(const char *filename) {
    return FS_FOpenFileWrite(filename);
}

/* Sound info / master gain stubs */
void SNDDMA_SoundInfo(void) {}
void SNDDMA_MasterGain(float val) { (void)val; }

/* Console / UI small & big string helpers expected by con_wiiu.c */
void SCR_DrawSmallChar(int x, int y, int ch) {
    fontSize_t sz = { SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT };
    SCR_DrawChar(x, y, ch, sz);
}

void SCR_DrawBigString(int x, int y, const char *s, float alpha, qboolean noColorEscape) {
    float color[4] = { 1.0f, 1.0f, 1.0f, alpha };
    re.SetColor(color);
    SCR_DrawString(x, y, s, noColorEscape, FONT_BIG);
    re.SetColor(NULL);
}

void SCR_DrawSmallStringExt(int x, int y, const char *string, float *setColor,
                            qboolean forceColor, qboolean noColorEscape) {
    (void)forceColor;
    re.SetColor(setColor);
    SCR_DrawString(x, y, string, noColorEscape, FONT_SMALL);
    re.SetColor(NULL);
}

/* VM_Compile / VM_CallCompiled stubs (vm_none.c calls exit(99)) */
void VM_Compile(vm_t *vm, vmHeader_t *header) {
    (void)header;
    if (vm) {
        vm->compiled = qfalse;
    }
}

int VM_CallCompiled(vm_t *vm, int *args) {
    (void)vm;
    (void)args;
    return 0;
}

/* cl_voip* cvars (declared in cl_main.c but only defined under USE_VOIP) */
/* Note: Cvar_Get() cannot be used in a static initializer, so we use a workaround */
__attribute__((weak)) cvar_t *cl_voipShowMeter = NULL;
__attribute__((weak)) cvar_t *cl_voipSend = NULL;
__attribute__((weak)) cvar_t *cl_voip = NULL;

/* Wii U OSK stubs */
__attribute__((weak)) qboolean WiiU_OSK_IsActive(void) { return qfalse; }
__attribute__((weak)) void WiiU_OSK_Toggle(void) {}
__attribute__((weak)) void WiiU_OSK_Cancel(void) {}
__attribute__((weak)) void WiiU_OSK_Frame(const oskInput_t *in) { (void)in; }
__attribute__((weak)) void WiiU_OSK_Draw(void) {}

/* WiiU_ExtractBundledPak stub (called from sys_main_wiiu.c) */
__attribute__((weak)) void WiiU_ExtractBundledPak(void) {}

void Sys_PrintResolveError(const char *s, int error) { (void)error; Com_Printf("Couldn't resolve address: %s\n", s); }
qboolean Sys_StringToAdr(const char *s, netadr_t *a, netadrtype_t family);
qboolean Sys_ResolveAddress(const char *s, netadr_t *a, netadrtype_t family, int netFlags, int *error) { char host[256]; char *colon; (void)netFlags; Q_strncpyz(host, s, sizeof(host)); colon = strchr(host, 58); if (colon) *colon = 0; if (Sys_StringToAdr(host, a, family)) { if (error) *error = 0; return qtrue; } if (error) *error = 1; return qfalse; }
int Cin_OGM_Init(cinematics_t *cin, const char *filename) { (void)cin; (void)filename; return 1; }
int Cin_OGM_Run(cinematics_t *cin, int time) { (void)cin; (void)time; return 0; }
void Cin_OGM_Shutdown(cinematics_t *cin) { (void)cin; }
unsigned char *Cin_OGM_GetOutput(cinematics_t *cin, int *outWidth, int *outHeight) { (void)cin; (void)outWidth; (void)outHeight; return NULL; }
