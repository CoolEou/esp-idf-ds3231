# esp-idf-ds3231

Driver for DS3231 high precision RTC module.

## Credits

This project is based on the DS3231 driver from esp-idf-lib:
https://github.com/esp-idf-lib/ds3231

Original authors:
- Richard A Burton
- Bhuvanchandra DV
- Ruslan V. Uss

## Modifications

This version includes the following changes:

- Split set alarm function into alarm1 and alarm2 to make the functions slightly simpler to use
- Added get alarm functions for both alarm1 and alarm2 to check what time the alarm is set to
- Added get alarm ints function to check which alarms are active/will trigger the interrupt when time matches the alarm
- Enabled internal pullups when initializing the i2c device (done in the ds3231_init_desc function)

## Installation

```sh
idf.py add-dependency "cooleou/ds3231"
```

## Note about the DS3132 interrupts

Something that tripped me up when playing with the DS3231, is the fact that despite the interrupt for a given alarm being disabled, the alarm fired flag will still go high once the time matches the alarm time. So if you try to deactivate the alarm by disabling the interrupt, but you use the alarm fired flag to trigger your alarm, the alarm will still be active.