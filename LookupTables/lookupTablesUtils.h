#if !defined(LookupTablesUtils_h)
#define LookupTablesUtils_h

class LookupTablesUtils {
 public:
  template <typename T>
  static T read(T& table, float value) {
    return table[floatToIndex(table, value)];
  }

  template <typename T>
  static T readInterpolated(T& table, float value) {
    value *= floatToIndex(table, value);
    size_t intergral = value;
    float fractional = value - intergral;

    float a = table[intergral];
    float b = table[intergral + 1];
    return a + (b - a) * fractional;
  }

  template <typename T>
  static size_t floatToIndex(T& table, float value) {
    return value * (sizeof(table) - 1);
  }
};

#endif  // LookupTablesUtils_h
