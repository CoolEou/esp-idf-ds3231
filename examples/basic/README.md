# Example for `ds3231` driver

## What it does

It initializes the date and the time to 2016-10-09 13:50:10.

It initializes alarm2 to 13:51

It initializes alarm1 to the 9th day of the month at 13:50:20

It enables the interrupt on alarm1

It checks which interrupts are active

It checks the oscillator stop flag

It shows date, time, temperature and alarms fired in a loop.

## Wiring

Connect `SCL` and `SDA` pins to the corresponding pins on your MCU, make sure to also update this in the ds3231_init_desc function. I've enabled the internal pullups in the ds3231_init_desc function, so you shouldn't need to add pullup resistors to your circuit, but feel free to do so anyway. I was able to run this program wihtout adding pullups to the circuit.

## Notes

I've only personally tested this on the ESP32C3. But the original authors noted the following:

`CONFIG_NEWLIB_LIBRARY_LEVEL_NORMAL` must be `y` on `esp8266`.