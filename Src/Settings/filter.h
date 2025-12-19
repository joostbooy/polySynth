#if !defined(Filter_h)
#define Filter_h

#include "fileReader.h"
#include "fileWriter.h"
#include "settingsUtils.h"

class Filter {
 public:
  enum Type { 
    HP, 
    BP, 
    LP2P, 
    NUM_FILTER_TYPES 
  };
  
  enum Routing { 
    SERIES, 
    PARALEL,
    NUM_FILTER_ROUTINGS
   };

  void init() {
    setType(HP);
    setRouting(SERIES);
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

  void setType(int value) {
    type_ = Type(SettingsUtils::clip(0, NUM_FILTER_TYPES - 1, value));
  }

  const char* typeText() {
    return typeText(type());
  }

  // Rourting
  Routing routing() {
    return routing_;
  }

  void setRouting(int value) {
    routing_ = Routing(SettingsUtils::clip(0, NUM_FILTER_ROUTINGS - 1, value));
  }

  const char* routingText() {
    return routingText(routing());
  }

  // Cutoff 1
  float cutoff1() {
    return cutoff1_;
  }

  void setCutoff1(float value) {
    cutoff1_ = value;
  }

  // Cutoff 2
  float cutoff2() {
    return cutoff2_;
  }

  void setCutoff2(float value) {
    cutoff2_ = value;
  }

  // Resonance 1
  float resonance1() {
    return resonace1_;
  }

  void setResonace1(float value) {
    resonace1_ = value;
  }

  // Resonance 2
  float resonance2() {
    return resonace2_;
  }

  void setResonace2(float value) {
    resonace2_ = value;
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(cutoff1_);
    fileWriter.write(cutoff2_);
    fileWriter.write(resonace1_);
    fileWriter.write(resonace2_);
    fileWriter.write(type_);
    fileWriter.write(routing_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(cutoff1_);
    fileReader.read(cutoff2_);
    fileReader.read(resonace1_);
    fileReader.read(resonace2_);
    fileReader.read(type_);
    fileReader.read(routing_);
  }

  void paste(Filter* filter) {
    cutoff2_ = filter->cutoff2();
    cutoff2_ = filter->cutoff2();
    resonace1_ = filter->resonance1();
    resonace2_ = filter->resonance2();
    type_ = filter->type();
    routing_ = filter->routing();
  }

 private:
  float cutoff1_;
  float cutoff2_;
  float resonace1_;
  float resonace2_;
  Type type_;
  Routing routing_;
};

#endif  // Filter_h
