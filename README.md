# AUDIO SAMPLER
Work in progress
<img src="LoFiSampler_faceplate.png"/></a>

## Features
- 4 stereo output tracks
- 1 headphone mix output
- 4 x gate & CV inputs
- 8 voices
- modulation matrix per instrument
- 4 Lfo's
- 2 envelopes per voice

## Specs
- Cortex-M7 ARM 216mHz (STM32F7)
- 64 MB audio RAM
- +/-5V CV inputs
- 9/12V DC power

## Code structure
- `/Drivers`			Hardware peripheral drivers
- `/LookUpTables`	Tables & table generation
- `/Stm32`				MCU config
- `/Src/Settings`	All the sampler data
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
