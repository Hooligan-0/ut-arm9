TARGET_APP=ut_imx23_fcse
TARGET_LDR=ut_loader
TARGET_AP1=ut_app1

BASE=0x40008000

SFLAGS=-g -Os -I. -fno-strict-aliasing -fno-common -ffixed-r8 -msoft-float -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv5
CFLAGS=-g -Os -I. -fno-strict-aliasing -fno-common -ffixed-r8 -msoft-float -marm  -mabi=aapcs-linux -mno-thumb-interwork -march=armv5 -nostdinc
LDFLAGS=-Bstatic -T linker.ld  -Ttext $(BASE)

ifndef $(CROSS)
CROSS=arm-none-eabi-
endif
ifndef $(CC)
CC=$(CROSS)gcc
endif
ifndef $(LD)
LD=$(CROSS)ld
endif

OBJS=ldr_start.o ldr_main.o

all: ut_load app1 img
	
clean:
	rm -f *.o *~ $(TARGET_APP) $(TARGET_APP).map
	rm -f $(TARGET_APP).dump $(TARGET_APP).bin $(TARGET_APP).u
	rm -f $(TARGET_LDR) $(TARGET_LDR).bin $(TARGET_LDR).map
	rm -f $(TARGET_AP1) $(TARGET_AP1).bin

ut_load: $(OBJS)
	@echo "  [LD] $(TARGET_LDR)"
	@$(LD) $(LDFLAGS) $(OBJS) -Map $(TARGET_LDR).map -o $(TARGET_LDR)
	@echo "  [OC] $(TARGET_LDR).bin"
	@arm-none-eabi-objcopy -O binary $(TARGET_LDR) $(TARGET_LDR).bin

app1: app1_start.o app1_main.o
	@echo "  [LD] $(TARGET_AP1)"
	@$(LD) -Bstatic -T app1_linker.ld -Ttext 0x00000000 app1_start.o app1_main.o -o $(TARGET_AP1)
	@echo "  [OC] $(TARGET_AP1).bin"
	@arm-none-eabi-objcopy -O binary $(TARGET_AP1) $(TARGET_AP1).bin

img:
	@echo "  [DD] $(TARGET_LDR).bin > $(TARGET_APP).bin"
	@dd if=$(TARGET_LDR).bin of=$(TARGET_APP).bin bs=4096
	@echo "  [DD] $(TARGET_AP1).bin > $(TARGET_APP).bin"
	@dd if=$(TARGET_AP1).bin of=$(TARGET_APP).bin bs=4096 seek=1
	@echo " [MKIMAGE] $(TARGET_APP).u"
	@mkimage -A arm -O linux -T kernel -C none -a 0x40008000 -e 0x40008000 -n 'utFCSE' -d $(TARGET_APP).bin $(TARGET_APP).u

%.o: %.c
	@echo "  [CC] $@"
	@$(CC) $(CFLAGS) -o $@ -c $<
%.o: %.S
	@echo "  [CC] $@"
	@$(CC) $(SFLAGS) -o $@ -c $<
