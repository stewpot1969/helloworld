include makefile.conf
NAME=minimum
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

LDSCRIPTS=-L. -T nokeep.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

$(NAME)-$(CORE).axf: $(NAME).c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@
	arm-none-eabi-objcopy -O ihex $(NAME)-$(CORE).axf $(NAME)-$(CORE).hex
	arm-none-eabi-objcopy -O binary $(NAME)-$(CORE).axf $(NAME)-$(CORE).bin

clean: 
	rm -f $(NAME)*.axf *.map *.o *.hex *.bin

prog:
	arm-none-eabi-gdb -cd=/home/sbb/bin/cm3/blinky2 \
	--eval-command="target remote localhost:3333" \
	--eval-command="monitor reset halt" \
	--eval-command="monitor stm32f1x mass_erase 0" \
	--eval-command="monitor flash write_bank 0 /home/sbb/bin/cm3/blinky2/minimum-CM3.bin 0" \
	--eval-command="monitor reset" \
	--batch

