#if !defined(Pots_h)
#define Pots_h

class Pots {
 public:
//  Pots(/* args */);
  //~Pots();

  enum Id {
    Resonace_1,
    Resonace_2,
    Cutoff_1,
    Cutoff_2,
    Shape_1,
    Shape_2,
    Coarse_1,
    Coarse_2,
    Fine_1,
    Fine_2,

    numPots
  };

  void write(int id, float value) {
    data_[id] = value;
  }

  float read(int id) {
    return data_[id];
  }

 private:
  float data_[numPots];
};

#endif  // Pots_h
