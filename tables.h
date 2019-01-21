#ifndef TABLES
#define TABLES

const int16_t sin_table[SAMPLES/2] = {
  0x0000, 0x0C8C, 0x18F9, 0x2528, 0x30FB, 0x3C56, 0x471C, 0x5133, 
  0x5A82, 0x62F1, 0x6A6D, 0x70E2, 0x7641, 0x7A7C, 0x7D89, 0x7F61, 
  0x7FFF, 0x7F61, 0x7D89, 0x7A7C, 0x7641, 0x70E2, 0x6A6D, 0x62F1, 
  0x5A82, 0x5133, 0x471C, 0x3C56, 0x30FB, 0x2528, 0x18F9, 0x0C8C
};

#if SAMPLES <= 256
const uint8_t inverse_table[SAMPLES-2] = {
#else
const uint16_t inverse_table[SAMPLES-2] = {
#endif
   32,  16,  48,   8,  40,  24,  56,   4, 
   36,  20,  52,  12,  44,  28,  60,   2, 
   34,  18,  50,  10,  42,  26,  58,   6, 
   38,  22,  54,  14,  46,  30,  62,   1, 
   33,  17,  49,   9,  41,  25,  57,   5, 
   37,  21,  53,  13,  45,  29,  61,   3, 
   35,  19,  51,  11,  43,  27,  59,   7, 
   39,  23,  55,  15,  47,  31
};

const int8_t scale_weights[SAMPLES/2-1] = {
  -32, -18, -14, -12, -11, -10, -10, -10, 
  -10, -10,  -9,  -9,  -9,  -9,  -9,  -9, 
   -9,  -9,  -9,  -9,  -9,  -9,  -9,  -9, 
  -10, -10, -10, -10, -10, -10, -10
};

#endif
