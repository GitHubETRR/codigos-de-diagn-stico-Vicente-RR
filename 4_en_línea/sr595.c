#include "sr595.h"

void sr595_latch(void)
{
    SR595_LATCH = 0;
    SR595_LATCH = 1;
}

void sr595_shiftByte(uint8_t data)
{
    for(int i = 7; i >= 0; i--)
    {
        SR595_DATA = (data >> i) & 1;
        SR595_CLK = 1;
        SR595_CLK = 0;
    }
}

void sr595_update(uint8_t *buffer)
{
    for(int i = SR595_NUMBER - 1; i >= 0; i--)
        sr595_shiftByte(buffer[i]);
    sr595_latch();
}

void sr595_resetAll(void)
{
    for(int i = SR595_NUMBER - 1; i >= 0; i--)
        sr595_shiftByte(0b00000000);
    sr595_latch();
}

void sr595_setAll(void)
{
    for(int i = SR595_NUMBER - 1; i >= 0; i--)
        sr595_shiftByte(0b11111111);
    sr595_latch();
}