
# Those hardware-specific files should not be built for the host, even if
# most of them give no error no warning. The host has different implementations
obj-$(CONFIG_EMBEDDED_NODE) += \
	dev/endpoint.o \
	dev/ep_pfilter.o \
	dev/i2c.o \
	dev/temperature.o \
	dev/minic.o \
	dev/syscon.o \
	dev/sfp.o \
	dev/devicelist.o \
	dev/rxts_calibrator.o \
	dev/flash.o

obj-$(CONFIG_WR_NODE) += \
	dev/pps_gen.o

obj-$(CONFIG_WR_SWITCH) += dev/timer-wrs.o dev/ad9516.o

obj-$(CONFIG_LEGACY_EEPROM) += dev/eeprom.o
obj-$(CONFIG_SDB_STORAGE) += dev/sdb-storage.o

obj-$(CONFIG_W1) +=		dev/w1.o	dev/w1-hw.o	dev/w1-shell.o
obj-$(CONFIG_W1) +=		dev/w1-temp.o	dev/w1-eeprom.o
obj-$(CONFIG_W1) +=		dev/temp-w1.o
obj-$(CONFIG_UART) +=		dev/uart.o
obj-$(CONFIG_UART_SW) +=	dev/uart-sw.o

obj-$(CONFIG_FAKE_TEMPERATURES) += dev/fake-temp.o

# Filter rules are selected according to configuration, but we may
# have more than one. Note: the filename is reflected in symbol names,
# so they are hardwired in ../Makefile (and ../tools/pfilter-builder too)
obj-y += $(pfilter-y:.bin=.o)

rules-%.o: rules-%.bin
	$(OBJCOPY) -I binary -O elf32-lm32 -B lm32 $< $@

$(pfilter-y): tools/pfilter-builder
	$^

tools/pfilter-builder:
	$(MAKE) -C tools pfilter-builder

