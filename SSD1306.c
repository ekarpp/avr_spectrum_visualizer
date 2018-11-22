#include <avr/io.h>
#include "SSD1306.h"
#include "main.h"

extern uint16_t ADC_data[SAMPLES*2];

void ssd1306_send_cmd(uint8_t byte)
{
  i2c_start();
  i2c_send(0x00);
  i2c_send(byte);
  i2c_stop();
}

void ssd1306_update_frame(complex* data)
{
  ssd1306_send_cmd(SSD1306_COL_ADDR);
  ssd1306_send_cmd(0x00);
  ssd1306_send_cmd(0x7F);

  ssd1306_send_cmd(SSD1306_PAGE_ADDR);
  ssd1306_send_cmd(0x00);
  ssd1306_send_cmd(0x07);

  /* 16 byte packets */
  /* 128*64 pixels = 8192 bits = 1024 bytes */
  /* 1024 / 16 = 64 */
#if SAMPLES <= 256
  uint8_t id = 0;
#else
  uint16_t id = 0;
#endif

  int16_t v = 0;
  uint8_t i,j;
  for (i = 0; i < 64; i++)
  {
    i2c_start();
    i2c_send(0x40);
    /* something better has to be done here */
    for (j = 0; j < 16; j++)
    {
      if (j%8 == 0)
	v = data[id].Re;

      if (i%2 == 1 && j > 7)
	i2c_send(0x00);
      else
      {
	if (v > 8)
	{
	  i2c_send(0xFF);
	  v -= 8;
	}
	else if (v)
	{
	  i2c_send((1 << v) - 1);
	  v = 0;
	}
	else
	  i2c_send(0x00);
      }
    }
    if (i%2 == 1)
      id += 1;
    i2c_stop();
  }
}
