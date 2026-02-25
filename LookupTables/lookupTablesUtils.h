#if !defined(LookupTablesUtils_h)
#define LookupTablesUtils_h

class LookupTablesUtils {
 public:
  template <typename T>
  static size_t size(T& table) {
    return sizeof(table) / sizeof(table[0]);
  }

  template <typename T, size_t N>
  static T read(const T (&table)[N], float index) {
    return table[size_t(index * (N - 1))];
  }

  template <typename T, size_t N>
  static T readInterpolated(const T (&table)[N], float phase) {
    float index = phase * (N - 1);
    size_t intergral = index;
    float fractional = index - intergral;

    T a = table[intergral];
    T b = table[intergral + 1];
    return a + (b - a) * fractional;
  }

  template <typename T, size_t N>
  static T readInterpolated(const T (&table1)[N], const T (&table2)[N], float phase, float mix) {
    float index = phase * (N - 1);
    size_t intergral = index;
    float fractional = index - intergral;

    T a = table1[intergral] + (table1[intergral + 1] - table1[intergral]) * fractional;
    T b = table2[intergral] + (table2[intergral + 1] - table2[intergral]) * fractional;
    return a + (b - a) * mix;
  }
};

#endif  // LookupTablesUtils_h
