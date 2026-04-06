#if !defined(Filter_h)
#define Filter_h

#include "fileReader.h"
#include "fileWriter.h"
#include "settingsText.h"
#include "settingsUtils.h"

class Filter {
 public:
  enum Type {
    HP,
    BP,
    LP2P,
    NUM_FILTER_TYPES,
  };

  enum Routing {
    SERIES,
    PARALEL,
    NUM_FILTER_ROUTINGS,
  };

  void init() {
    setType(HP);
    setRouting(SERIES);
    setCutoff1(0.9f);
    setCutoff2(0.9f);
    setCutoff1Spread(0.f);
    setCutoff2Spread(0.f);
    setResonace1(0.f);
    setResonace2(0.f);
    setFmEnable1(false);
    setFmEnable2(false);
    setLink(false);
  }

  const char* typeText(Type type) {
    switch (type) {
      case HP:    return "HP";
      case BP:    return "BP";
      case LP2P:  return "2PLP";
      default:
        break;
    }
    return nullptr;
  }

  const char* routingText(Routing type) {
    switch (type) {
      case SERIES:  return "SERIES";
      case PARALEL: return "PARALEL";
      default:
        break;
    }
    return nullptr;
  }

  // Type
  Type type() {
    return type_;
  }

  void setType(size_t value) {
    type_ = Type(value % NUM_FILTER_TYPES);
  }

  const char* typeText() {
    return typeText(type());
  }

  // Rourting
  Routing routing() {
    return routing_;
  }

  void setRouting(size_t value) {
    routing_ = Routing(value % NUM_FILTER_ROUTINGS);
  }

  const char* routingText() {
    return routingText(routing());
  }

  // Cutoff 1
  float cutoff1() {
    return cutoff1_;
  }

  void setCutoff1(float value) {
    cutoff1_ = SettingsUtils::clipFloat(value);
  }

  const char* cutoff1Text() {
    return SettingsText::floatToText(cutoff1());
  }

  // Cutoff 2
  float cutoff2() {
    return link() ? SettingsUtils::clipFloat(cutoff1_ + cutoff2_) : cutoff2_;
  }

  void setCutoff2(float value) {
    cutoff2_ = SettingsUtils::clipFloat(value);
  }

  const char* cutoff2Text() {
    return SettingsText::floatToText(cutoff2());
  }

  // Cutoff 1 spread
  float cutoff1Spread() {
    return cutoff1Spread_;
  }

  void setCutoff1Spread(float value) {
    cutoff1Spread_ = SettingsUtils::clipFloat(value);
  }

  const char* cutoff1SpreadText() {
    return SettingsText::floatToText(cutoff1Spread());
  }

  // Cutoff 2 spread
  float cutoff2Spread() {
    return cutoff2Spread_;
  }

  void setCutoff2Spread(float value) {
    cutoff2Spread_ = SettingsUtils::clipFloat(value);
  }

  const char* cutoff2SpreadText() {
    return SettingsText::floatToText(cutoff2Spread());
  }

  // Resonance 1
  float resonance1() {
    return resonace1_;
  }

  void setResonace1(float value) {
    resonace1_ = SettingsUtils::clipFloat(value);
  }

  const char* resonance1Text() {
    return SettingsText::floatToText(resonance1());
  }

  // Resonance 2
  float resonance2() {
    return link() ? SettingsUtils::clipFloat(resonace1_ + resonace2_) : resonace2_;
  }

  void setResonace2(float value) {
    resonace2_ = SettingsUtils::clipFloat(value);
  }

  const char* resonance2Text() {
    return SettingsText::floatToText(resonance2());
  }

  // Fm enable 1
  bool fmEnable1() {
    return fmEnable1_;
  }

  void setFmEnable1(bool value) {
    fmEnable1_ = value;
  }

  const char* fmEnable1Text() {
    return SettingsText::boolToOnOff(fmEnable1());
  }

  // Fm enable 2
  bool fmEnable2() {
    return fmEnable2_;
  }

  void setFmEnable2(bool value) {
    fmEnable2_ = value;
  }

  const char* fmEnable2Text() {
    return SettingsText::boolToOnOff(fmEnable2());
  }

  // Link
  bool link() {
    return link_;
  }

  void setLink(bool value) {
    link_ = value;
  }

  const char* linkText() {
    return SettingsText::boolToOnOff(link());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(cutoff1_);
    fileWriter.write(cutoff2_);
    fileWriter.write(cutoff1Spread_);
    fileWriter.write(cutoff2Spread_);
    fileWriter.write(resonace1_);
    fileWriter.write(resonace2_);
    fileWriter.write(type_);
    fileWriter.write(routing_);
    fileWriter.write(fmEnable1_);
    fileWriter.write(fmEnable2_);
    fileWriter.write(link_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(cutoff1_);
    fileReader.read(cutoff2_);
    fileReader.read(cutoff1Spread_);
    fileReader.read(cutoff2Spread_);
    fileReader.read(resonace1_);
    fileReader.read(resonace2_);
    fileReader.read(type_);
    fileReader.read(routing_);
    fileReader.read(fmEnable1_);
    fileReader.read(fmEnable2_);
    fileReader.read(link_);
  }

  void paste(Filter* filter) {
    cutoff1Spread_ = filter->cutoff1Spread();
    cutoff2Spread_ = filter->cutoff2Spread();
    resonace1_ = filter->resonance1();
    resonace2_ = filter->resonance2();
    type_ = filter->type();
    routing_ = filter->routing();
    fmEnable1_ = filter->fmEnable1();
    fmEnable2_ = filter->fmEnable2();

    // Temporarily unlink cutoff so we can paste cutoff 2
    link_ = filter->link();
    filter->setLink(false);
    cutoff1_ = filter->cutoff1();
    cutoff2_ = filter->cutoff2();
    filter->setLink(link_);
  }

  void writeHash(Hash& hash) {
    hash.write(cutoff1_);
    hash.write(cutoff2_);
    hash.write(cutoff1Spread_);
    hash.write(cutoff2Spread_);
    hash.write(resonace1_);
    hash.write(resonace2_);
    hash.write(type_);
    hash.write(routing_);
    hash.write(fmEnable1_);
    hash.write(fmEnable2_);
    hash.write(link_);
  }

 private:
  float cutoff1_;
  float cutoff2_;
  float cutoff1Spread_;
  float cutoff2Spread_;
  float resonace1_;
  float resonace2_;
  bool fmEnable1_;
  bool fmEnable2_;
  bool link_;
  Type type_;
  Routing routing_;
};

#endif  // Filter_h
