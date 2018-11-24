#include <avr/io.h>
#include "main.h"
#include "tables.h"

/* index for window table */
#define WINDOW_i(x) ((x < SAMPLES/2) ? x : SAMPLES/2 - x%(SAMPLES/2))

#define abs(x) ((x < 0) ? -x : x)

void bit_reversal(complex* data)
{
#if SAMPLES <= 128
  uint8_t i, new;
#else
  uint16_t i, new;
#endif

  complex tmp;
  for(i = 1; i < SAMPLES - 1; i++)
  {
    new = inverse_table[i-1];
    if (new > i)
    {
      tmp = data[i];
      data[i] = data[new];
      data[new] = tmp;
    }
  }
}

void window(complex* data)
{
#if SAMPLES <= 128
  uint8_t i;
#else
  uint16_t i;
#endif

  for (i = 0; i < SAMPLES; i++)
  {
    data->Re = mul_16_bit(data->Re, window_table[WINDOW_i(i)]);
    data++;
  }
}

/* ~10ms with bit inversion */
/* in place DIT FFT */
void fft(complex* data)
{
#if SAMPLES <= 128
  uint8_t i, n;
#else
  uint16_t i, n;
#endif

#if SAMPLES <= 256
  uint8_t j, sets;
#else
  uint16_t j, sets;
#endif

  complex* a,* b;
  complex coef, twiddle;
  sets = SAMPLES / 2;
  n = 1;
  while (n < SAMPLES)
  {
    a = &data[0];
    b = &data[n];
    for (i = 0; i < n; i++)
    {
      /* exp(-2*pi*i*k/t) where k = i and t = SAMPLES/sets */
      /* table is sin(2*pi*x*SAMPLES) so we have to fix indexes accordingly */
      twiddle.Re = sin_table[i*sets + SAMPLES/4];
      twiddle.Im = sin_table[i*sets];
      for (j = 0; j < sets; j++)
      {
	coef.Re = mul_16_bit(b->Re, twiddle.Re) \
	  + mul_16_bit(b->Im, twiddle.Im);

	coef.Im = mul_16_bit(b->Im, twiddle.Re) \
	  - mul_16_bit(b->Re, twiddle.Im);

	b->Re = a->Re - coef.Re;
	b->Im = a->Im - coef.Im;
	a->Re += coef.Re;
	a->Im += coef.Im;

	a += 2*n;
	b += 2*n;
      }
      a -= SAMPLES - 1;
      b -= SAMPLES - 1;
    }
    sets >>= 1;
    n <<= 1;
  }
}

void scale(complex* data)
{
#if SAMPLES <= 128
  uint8_t i;
#else
  uint16_t i;
#endif

  uint32_t tmp;
  uint16_t max = 0;
  /* just ignore the dc bin for now */
  data[0].Re = 0;
  for (i = 0; i < SAMPLES/2; i++)
  {
    tmp = (uint32_t) abs(data[i].Re) * abs(data[i].Re);
    tmp += (uint32_t) abs(data[i].Im) * abs(data[i].Im);
    data[i].Re = sqrt_32_bit(tmp);
    if (data[i].Re > max)
      max = data[i].Re;
  }

  for (i = 0; i < SAMPLES/2; i++)
    data[i].Re = (int16_t)((data[i].Re/(double)max)*64);

}

int16_t mul_16_bit(int16_t a, int16_t b)
{
  uint8_t count = 0;
  if (a < 0)
  {
    a = -a;
    count++;
  }
  if (b < 0)
  {
    b = -b;
    count++;
  }
  uint32_t tmp = a;
  tmp *= b;
  tmp >>= 15;
  return count == 1 ? -(int16_t) tmp : (int16_t) tmp;
}

int16_t sqrt_32_bit(uint32_t x)
{
  uint32_t g = x;
  int8_t i;
  for (i = 20; i < 50; i++)
    g = (g+x/g)/2;
  return (int16_t) g;
}
