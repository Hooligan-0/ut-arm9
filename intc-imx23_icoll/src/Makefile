##
 # ARM9 Unit Test - Interrupt Collector for IMX23
 #
 # Copyright (c) 2014 Saint-Genest Gwenael <gwenael.saint-genest@agilack.fr>
 #
TARGET_APP=ut_imx23_icoll

BASE=0x00000000

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

OBJS=start.o main.o

all: app img
	
clean:
	rm -f *.o *~ $(TARGET_APP) $(TARGET_APP).map
	rm -f $(TARGET_APP).dump $(TARGET_APP).bin $(TARGET_APP).u

app: $(OBJS)
	@echo "  [LD] $(TARGET_APP)"
	@$(LD) $(LDFLAGS) $(OBJS) -Map $(TARGET_APP).map -o $(TARGET_APP)
	@echo "  [OC] $(TARGET_APP).bin"
	@arm-none-eabi-objcopy -O binary $(TARGET_APP) $(TARGET_APP).bin

img:
	@echo " [MKIMAGE] $(TARGET_APP).u"
	@mkimage -A arm -O linux -T kernel -C none -a 0x00000000 -e 0x00000000 -n 'utICOLL' -d $(TARGET_APP).bin $(TARGET_APP).u

%.o: %.c
	@echo "  [CC] $@"
	@$(CC) $(CFLAGS) -o $@ -c $<
%.o: %.S
	@echo "  [CC] $@"
	@$(CC) $(SFLAGS) -o $@ -c $<
