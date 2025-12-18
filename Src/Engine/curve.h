#ifndef Curve_h
#define Curve_h

#include "lookupTables.h"

class Curve {

public:

	static inline float read(float phase, float shape) {
		phase *= EXP_TABLE_SIZE;
		uint16_t intergral = static_cast<uint16_t>(phase);
		float fractional = phase - intergral;

		const float *lut_a = &lut_exp[intergral];
		const float *lut_b = &lut_inv_exp[intergral];
		float a = Dsp::cross_fade(*lut_a, *(lut_a + 1), fractional);
		float b = Dsp::cross_fade(*lut_b, *(lut_b + 1), fractional);

		return Dsp::cross_fade(a, b, shape);
	}

private:

};

#endif
