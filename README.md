# I2C-Altera-FPGA

An I2C driver on Altera EP4CE6E22C8, developed for TOF sensor VL53L1X.

## How to use

Just have to change the include path in the comment, don't forget to make sure the definition of `SDA_BASE` and `SCL_BASE`, you can find them in your `system.h`.

## Prompt

*I've tested it on Quartus Prime 18.1, if you want to use other sensors, you'd better modify some functions, because **some details might be different***
