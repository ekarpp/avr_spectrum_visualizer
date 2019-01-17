#!/usr/bin/env python3
from math import sin, pi, cos, exp, log, sqrt

def inv(n, bits):
    new = 0
    for i in range(bits):
        new |= ((n >> i)&1) << (bits - i - 1)
    return str(new)

def convert_16bit_signed(x):
    if x >= 0:
        return "0x%04X" % int(x*0x7FFF + 0.5)
    else:
        #convert to 16-bit two's complement
        v = abs(x)
        v = int(v*0x7FFF + 0.5)
        #v |= 1 << 15
        v ^= 0xFFFF
        v += 1
        return "0x%04X" % (v&0xFFFF)

def mk_string(lst):
    str = ""
    for i in range(len(lst) - 1):
        str += "%3s, " % lst[i]
        if i%8 == 7:
            str += "\n  "
    return "%s%3s" % (str, lst[i+1])

functions = {
    "hann" : (lambda x,N: sin(pi*x/(N-1))**2),
    "hamming" : (lambda x,N: 0.54 - 0.46*cos(2*pi*x/(N-1))),
    "gauss" : (lambda x,N: exp(-0.5*((x-(N-1)/2)/(0.4*(N-1)/2))**2)),
    "none" : (lambda x,N: 1)
}

N = int(input("samples?: "))
N_log2 = int(log(N,2))
func = input("window function?(%s): " % ", ".join(list(functions.keys())))


#window functions are symmetrical so we only need half of the values
window = [functions[func](x,N) for x in range(N)][:N//2]
window = [convert_16bit_signed(x) for x in window]
#sin table
sin_wave = [sin(2*pi*x/N) for x in range(3*N//4)]
sin_wave = [convert_16bit_signed(x) for x in sin_wave]
#cos table
#can be taken from sin table

inverse = [x for x in range(1,N-1)]
for i in range(N-2):
    inverse[i] = inv(inverse[i], N_log2)

#https://en.wikipedia.org/wiki/A-weighting
def r_a(f):
    return 12194**2*f**4/((f**2+20.6**2)*sqrt((f**2+107.7**2)*(f**2+737.9**2))*(f**2+12194**2))

def a(r):
    return 20*log(r)/log(10)+2

#highest frequency
f=5000
#N/2 - 1 bins
step = f/(N/2-1)
weights = [step/2]
for i in range(30):
    weights.append(weights[i] + step)
weights = [r_a(x) for x in weights]
weights = [int(a(x)) for x in weights]

template = """\
#ifndef TABLES
#define TABLES

/* %s window function */
const int16_t window_table[SAMPLES/2] = {
  %s
};

const int16_t sin_table[3*SAMPLES/4] = {
  %s
};

#if SAMPLES <= 256
const uint8_t inverse_table[SAMPLES-2] = {
#else
const uint16_t inverse_table[SAMPLES-2] = {
#endif
  %s
};

const int8_t scale_weights[SAMPLES/2] = {
  %s
};

#endif
"""

with open("tables.h", "w") as f:
    f.write(template % (
        func,
        mk_string(window),
        mk_string(sin_wave),
        mk_string(inverse),
        mk_string(weights)
    ))
