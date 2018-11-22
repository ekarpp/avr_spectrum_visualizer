MCU        = atmega328p
F_CPU      = 16000000L
OUT        = out
SRCS       = I2C.c SSD1306.c FFT.c main.c
DEV        = /dev/ttyUSB0
FLASH      = avrdude
PROGRAMMER = arduino
C_FLAGS    = -Wall -Os -mmcu=${MCU} -DF_CPU=${F_CPU}
AVRD_FLAGS = -v -P ${DEV} -p ${MCU} -c ${PROGRAMMER} \
-b 57600 -U flash:w:${OUT}.hex

all: bin hex flash clean

tables:
	./table_gen.py

bin:
	avr-gcc ${C_FLAGS} -o ${OUT}.bin ${SRCS}

hex: bin
	avr-objcopy -j .text -j .data -O ihex ${OUT}.bin ${OUT}.hex

asm: bin
	avr-objdump -d ${OUT}.bin > ${OUT}.asm

flash: hex
	${FLASH} ${AVRD_FLAGS}

clean:
	rm -f ${OUT}.bin ${OUT}.hex ${OUT}.asm
