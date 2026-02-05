# 8 VOICE ANALOG SYNTHESIZER
Work in progress

## Features
- x

## Specs
- Cortex-M4 ARM 168mHz (STM32F4)
- 8 voices
- 2 polyphonic envelopes & lfo's
- 9/12V DC power

## Code structure
- `/Drivers`			Hardware peripheral drivers
- `/LookUpTables`	Tables & table generation
- `/Stm32`				MCU config
- `/Src/Settings`	All the synth data
- `/Src/Ui`				User interface to manipulate & visualize the data
- `/Src/Engine`		Responsible for running the data & talk to the hardware drivers
- `/Src/Disk`			Filesystem


## Credits
- Pichenettes		https://github.com/pichenettes
- Chan					https://elm-chan.org/fsw/ff/00index_e.html
- Ha Thach 			https://docs.tinyusb.org/en/latest/

## License
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
This work is licensed under a [MIT License](https://opensource.org/licenses/MIT).
