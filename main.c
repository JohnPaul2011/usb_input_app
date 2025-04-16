#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <string.h>

PSP_MODULE_INFO("USBKeyboardHomebrew", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int map_key(const char *input) {
    if (strcmp(input, "UP") == 0) return PSP_CTRL_UP;
    if (strcmp(input, "DOWN") == 0) return PSP_CTRL_DOWN;
    if (strcmp(input, "LEFT") == 0) return PSP_CTRL_LEFT;
    if (strcmp(input, "RIGHT") == 0) return PSP_CTRL_RIGHT;
    if (strcmp(input, "CROSS") == 0) return PSP_CTRL_CROSS;
    if (strcmp(input, "CIRCLE") == 0) return PSP_CTRL_CIRCLE;
    if (strcmp(input, "SQUARE") == 0) return PSP_CTRL_SQUARE;
    if (strcmp(input, "TRIANGLE") == 0) return PSP_CTRL_TRIANGLE;
    if (strcmp(input, "START") == 0) return PSP_CTRL_START;
    if (strcmp(input, "SELECT") == 0) return PSP_CTRL_SELECT;
    return 0;
}

int main() {
    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    SceUID fd = sceIoOpen("usbhost0:", PSP_O_RDONLY, 0777);
    if (fd < 0) {
        pspDebugScreenPrintf("USB not connected.\n");
        sceKernelSleepThread();
    }

    char buffer[32];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read = sceIoRead(fd, buffer, sizeof(buffer));
        if (read > 0) {
            int btn = map_key(buffer);
            if (btn) {
                pspDebugScreenPrintf("Pressed: %s -> 0x%X\n", buffer, btn);
            }
        }
        sceKernelDelayThread(100000);
    }

    sceIoClose(fd);
    return 0;
}
