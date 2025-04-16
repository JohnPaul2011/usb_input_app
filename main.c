#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <string.h>
#include <stdio.h>

PSP_MODULE_INFO("USBInputApp", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define MAX_KEYS 64

typedef struct {
    char pc_key[16];
    int psp_button;
} KeyMap;

KeyMap key_map[MAX_KEYS];
int key_count = 0;

int get_psp_button(const char *btn) {
    if (strcmp(btn, "UP") == 0) return PSP_CTRL_UP;
    if (strcmp(btn, "DOWN") == 0) return PSP_CTRL_DOWN;
    if (strcmp(btn, "LEFT") == 0) return PSP_CTRL_LEFT;
    if (strcmp(btn, "RIGHT") == 0) return PSP_CTRL_RIGHT;
    if (strcmp(btn, "CROSS") == 0) return PSP_CTRL_CROSS;
    if (strcmp(btn, "CIRCLE") == 0) return PSP_CTRL_CIRCLE;
    if (strcmp(btn, "SQUARE") == 0) return PSP_CTRL_SQUARE;
    if (strcmp(btn, "TRIANGLE") == 0) return PSP_CTRL_TRIANGLE;
    if (strcmp(btn, "START") == 0) return PSP_CTRL_START;
    if (strcmp(btn, "SELECT") == 0) return PSP_CTRL_SELECT;
    return 0;
}

void load_key_config() {
    FILE *f = fopen("keys.cfg", "r");
    if (!f) return;

    char line[64];
    while (fgets(line, sizeof(line), f)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char *key = line;
        char *val = eq + 1;
        val[strcspn(val, "\r\n")] = 0;

        strncpy(key_map[key_count].pc_key, key, 15);
        key_map[key_count].psp_button = get_psp_button(val);
        key_count++;
    }

    fclose(f);
}

int find_button(const char *key) {
    for (int i = 0; i < key_count; i++) {
        if (strcmp(key_map[i].pc_key, key) == 0)
            return key_map[i].psp_button;
    }
    return 0;
}

int main() {
    pspDebugScreenInit();
    pspDebugScreenPrintf("USB Input Homebrew\n");

    load_key_config();

    char input[16];
    while (1) {
        pspDebugScreenSetXY(0, 2);
        pspDebugScreenPrintf("Waiting for input: ");

        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\r\n")] = 0;

            int btn = find_button(input);
            if (btn) {
                pspDebugScreenPrintf("Mapped '%s' -> button 0x%X\n", input, btn);
            } else {
                pspDebugScreenPrintf("Unknown key: %s\n", input);
            }
        }

        sceDisplayWaitVblankStart();
    }

    return 0;
}
