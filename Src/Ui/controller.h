#ifndef Controller_h
#define Controller_h

#include <stdint.h>
#include "chapter.h"

#define SERIAL_ID(collumn, row) ((collumn * 8) + row)

class Controller {

public:

	enum Id {
		TUNE_1_OCT_UP			= SERIAL_ID(0, 0),
		TUNE_1_OCT_DOWN			= SERIAL_ID(0, 1),
		TUNE_2_OCT_UP			= SERIAL_ID(0, 2),
		TUNE_2_OCT_DOWN			= SERIAL_ID(0, 3),
		LFO_1_TYPE				= SERIAL_ID(0, 4),
		LFO_2_TYPE				= SERIAL_ID(0, 5),
		TUNE_1_VCO_MOD			= SERIAL_ID(1, 0),
		TUNE_1_MOD_TYPE			= SERIAL_ID(1, 1),
		VCO_2_SYNC				= SERIAL_ID(1, 2),
		TUNE_2_MOD_TYPE			= SERIAL_ID(1, 3),
		SHAPE_1_MOD_TYPE		= SERIAL_ID(2, 0),
		VCO_1_OSC_TYPE			= SERIAL_ID(2, 1),
		SHAPE_2_MOD_TYPE		= SERIAL_ID(2, 2),
		VCO_2_OSC_TYPE			= SERIAL_ID(2, 3),
		VCO_MOD_SOURCE			= SERIAL_ID(2, 4),
		SLIDE_VCO_SELECT		= SERIAL_ID(2, 5),
		CUTOFF_1_MOD_TYPE		= SERIAL_ID(3, 0),
		VCO_1_MUTE				= SERIAL_ID(3, 1),
		CUTOFF_2_MOD_TYPE		= SERIAL_ID(3, 2),
		VCO_2_MUTE				= SERIAL_ID(3, 3),
		VCO_MOD_DEPTH_MOD_TYPE	= SERIAL_ID(3, 4),
		VOLUME_MOD_TYPE			= SERIAL_ID(3, 5),
		VCF_1_TYPE				= SERIAL_ID(4, 0),
		CUTOFF_1_VCO_MOD		= SERIAL_ID(4, 1),
		VCF_2_ROUTING			= SERIAL_ID(4, 2),
		CUTOFF_2_VCO_MOD		= SERIAL_ID(4, 3),
		VOLUME_VCO_MOD			= SERIAL_ID(4, 5),
		DISPLAY_B				= SERIAL_ID(5, 0),
		DISPLAY_A				= SERIAL_ID(5, 1),
		PATCH_PAGE				= SERIAL_ID(5, 2),
		ENVELOPE_PAGE			= SERIAL_ID(5, 3),
		VCO_PAGE				= SERIAL_ID(5, 4),
		PAN_MOD_TYPE			= SERIAL_ID(5, 5),
		DISPLAY_D				= SERIAL_ID(6, 0),
		DISPLAY_C				= SERIAL_ID(6, 1),
		LFO_PAGE				= SERIAL_ID(6, 2),
		MATRIX_PAGE				= SERIAL_ID(6, 3),
		SYSTEM_PAGE				= SERIAL_ID(6, 4),
		SHIFT					= SERIAL_ID(6, 5),
		ENC_X					= SERIAL_ID(7, 0),
		AMP_PAGE				= SERIAL_ID(7, 2),
		VCF_PAGE				= SERIAL_ID(7, 3),
		ENC_Y					= SERIAL_ID(7, 5),
	};

	static void init() {
		for (int i = 0; i < 2; ++i) {
			pressed_[i] = 0;
		}
	}

	static void onButton(uint8_t id, bool state) {
		uint8_t index = id / 32;
		uint32_t mask = 1 << (id & 31);
		pressed_[index] &= ~(mask);
		if (state) {
			pressed_[index] |= mask;
		}
	}

	static bool isPressed(uint8_t id) {
		uint8_t index = id / 32;
		uint32_t mask = 1 << (id & 31);
		return pressed_[index] & mask;
	}

	static int8_t buttonToFunction(uint8_t id) {
		switch (id)
		{
		case DISPLAY_A:		return 0;
		case DISPLAY_B:		return 1;
		case DISPLAY_C:		return 2;
		case DISPLAY_D:		return 3;
		default:			return -1;
			break;
		}
	}

        /*
                static int8_t buttonToPage(uint8_t id) {
                        switch (id)
                        {
                        case LFO_PAGE:					return Pages::LFO_PAGE;
                        default:						return -1;
                                break;
                        }
                }
        */

private:
	static inline uint32_t pressed_[2];
};

#endif
