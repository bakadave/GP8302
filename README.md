# GP8302 library for Aruino

This library was created for use with Guestgood GP8302 I2C 0-25mA DAC (used for example in DFRobot DFR0972 modules).

Please note, this expander has an open-drain, active-low ALERT output if the IC detects no load on output. In the future I might publish an example on how to use it with a HW interrupt.

DFRobot has their own library for this IC but I decided to create my own that uses Arduino TwoWire library instead of bit-banging.

## Example usage
```cpp
//TODO
```