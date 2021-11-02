#ifndef FISH_MODE_H
#define FISH_MODE_H

#include "Mode.h"

struct _FishImpl;

class FishTank : public Mode {
  public:
    FishTank();
    ~FishTank();
  protected:
    void run(uint8_t delta);
  private:
    _FishImpl* impl;
};

#endif