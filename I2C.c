#include <avr/io.h>
#include "main.h"

#define i2c_busy (!(TWCR & (1 << TWINT)))

void i2c_start(void)
{
  /* send start condition */
  TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
  while (i2c_busy);

  /* send slave write address */
  TWDR = i2c_addr;
  TWCR = (1 << TWEN) | (1 << TWINT);
  while (i2c_busy);
}

void i2c_stop(void)
{
  /* send stop condition */
  TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
}

void i2c_send(uint8_t byte)
{
  /* load byte to data register */
  TWDR = byte;

  /* send it */
  TWCR = (1 << TWEN) | (1 << TWINT);
  while (i2c_busy);
}
