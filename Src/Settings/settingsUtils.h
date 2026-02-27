#ifndef SettingsUtils_h
#define SettingsUtils_h

class SettingsUtils {

public:

	static inline int clipMin(int min, int value) {
		return value > min ? value : min;
	}

	static inline int clipMax(int max, int value) {
		return value < max ? value : max;
	}

	static inline int clip(int min, int max, int value) {
		return value > max ? max : (value < min ? min : value);
	}

	static inline float clipFloat(float value) {
		return value > 1.0f ? 1.0f : (value < 0.0f ? 0.0f : value);
	}

	template <typename type>
	static inline type difference(type a, type b) {
		return a > b ? a - b : b - a;
	}

	template <typename type>
	static inline void swap(type &a,type &b) {
		type temp = a;
		a = b;
		b = temp;
	}

	static inline float fInc(int inc, bool shift) {
		return inc * (1.f / 100.f) * (shift ? 10.f : 1.f);
	}
};

#endif
