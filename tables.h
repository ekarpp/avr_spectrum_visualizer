#ifndef TABLES
#define TABLES

/* gauss window function */
const int16_t window_table[SAMPLES/2] = {
  0x05A0, 0x06D6, 0x0842, 0x09EA, 0x0BD3, 0x0E05, 0x1083, 0x1354, 
  0x167C, 0x19FD, 0x1DDA, 0x2212, 0x26A5, 0x2B8F, 0x30CA, 0x364D, 
  0x3C10, 0x4203, 0x4819, 0x4E40, 0x5465, 0x5A73, 0x6054, 0x65F2, 
  0x6B37, 0x700D, 0x745D, 0x7817, 0x7B27, 0x7D80, 0x7F18, 0x7FE5
};

const int16_t sin_table[3*SAMPLES/4] = {
  0x0000, 0x0C8C, 0x18F9, 0x2528, 0x30FB, 0x3C56, 0x471C, 0x5133, 
  0x5A82, 0x62F1, 0x6A6D, 0x70E2, 0x7641, 0x7A7C, 0x7D89, 0x7F61, 
  0x7FFF, 0x7F61, 0x7D89, 0x7A7C, 0x7641, 0x70E2, 0x6A6D, 0x62F1, 
  0x5A82, 0x5133, 0x471C, 0x3C56, 0x30FB, 0x2528, 0x18F9, 0x0C8C, 
  0x0000, 0xF374, 0xE707, 0xDAD8, 0xCF05, 0xC3AA, 0xB8E4, 0xAECD, 
  0xA57E, 0x9D0F, 0x9593, 0x8F1E, 0x89BF, 0x8584, 0x8277, 0x809F
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

const int8_t scale_weights[SAMPLES/2] = {
  -22,  -8,  -4,  -2,  -1,   0,   0,   0, 
    0,   0,   1,   1,   1,   1,   1,   1, 
    1,   1,   1,   1,   1,   1,   1,   1, 
    0,   0,   0,   0,   0,   0,   0
};

#endif
