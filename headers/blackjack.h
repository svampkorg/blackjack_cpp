#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "statehandling.h"

class Blackjack {
public:
  void run();

private:
  State state;
};

#endif
