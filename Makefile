TARGET = usbkb_homebrew
OBJS = main.o

CFLAGS = -O2 -G0 -Wall
LIBS = -lpspdebug -lpspkernel -lpspctrl

PSP_EBOOT_TITLE = USB Keyboard Homebrew
PSP_HEAP_SIZE_KB = 64

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
