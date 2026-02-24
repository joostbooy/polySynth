#if !defined(LookupTablesUtils_h)
#define LookupTablesUtils_h

class LookupTablesUtils {
 public:
  template <typename T>
  static T read(T& table, float index) {
    return table[size_t(index * (sizeof(table) - 1))];
  }

  template <typename T>
  static T readInterpolated(T& table, float phase) {
    float index = phase * (sizeof(table) - 1);
    size_t intergral = index;
    float fractional = index - intergral;

    T a = table[intergral];
    T b = table[intergral + 1];
    return a + (b - a) * fractional;
  }
};

#endif  // LookupTablesUtils_h
