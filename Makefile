# Windows/PSPSDK v0.10.0 Makefile for CMD

TARGET = usb_input
OBJS = main.o

PSPDEV = C:/pspsdk
PSP_FW_VERSION = 660

CFLAGS = -O2 -G0 -Wall
LIBS = -lpspdebug -lpspctrl -lpspgu -lpsppower -lpspsystemctrl
LIBDIR = $(PSPDEV)/psp/lib
INCDIR = $(PSPDEV)/psp/include

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = USB Input Homebrew

# Object files
$(TARGET): $(OBJS)
	$(PSPDEV)/bin/psp-gcc $(OBJS) -o $(TARGET).elf $(LIBS)
	$(PSPDEV)/bin/psp-strip $(TARGET).elf
	$(PSPDEV)/bin/psp-make-fself $(TARGET).elf $(TARGET).prx
	$(PSPDEV)/bin/psp-pbp $(TARGET).prx EBOOT.PBP

# Compile the source code
main.o: main.c
	$(PSPDEV)/bin/psp-gcc -c main.c -o main.o $(CFLAGS) -I $(INCDIR)

# Clean the build
clean:
	del $(OBJS) $(TARGET).elf $(TARGET).prx EBOOT.PBP
