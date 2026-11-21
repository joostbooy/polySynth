#if !defined(EngineUtils_h)
#define EngineUtils_h

#include "settingsUtils.h"

namespace EngineUtils {

  template <typename T>
  static T crossFade(T a, T b, float mix) {
    return a + (b - a) * mix;
  }

  static float foldWave(float wave, float amount) {
    float amount_ = 1.f + (amount * 4.f);
    float out = 0.25f * ((wave * 2.0f - 1.0f) * amount_) + 0.25f;
    return 2.0f * std::fabs(out - std::roundf(out));
  }

  static float spread(float value, float spreadAmmount, int playOrder) {
    float spread_ = spreadAmmount * (1.f / Settings::kNumVoices) * playOrder;
    if (playOrder % 2) {
      return SettingsUtils::clipFloat(value + spread_);
    } else {
      return SettingsUtils::clipFloat(value - spread_);
    }
  }

}  // namespace EngineUtils

#endif  // EngineUtils_h
