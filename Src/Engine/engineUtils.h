#if !defined(EngineUtils_h)
#define EngineUtils_h

#include "settingsUtils.h"

namespace EngineUtils {

  static float spread(float value, float spreadAmmount, int playOrder) {
    float spread_ = spreadAmmount * (0.5f / Settings::kNumVoices) * playOrder;
    if (playOrder % 2) {
      return SettingsUtils::clipFloat(value + spread_);
    } else {
      return SettingsUtils::clipFloat(value - spread_);
    }
  }

}  // namespace EngineUtils

#endif  // EngineUtils_h
