#ifndef HELPERS_H
#define HELPERS_H

#include "blackjacktypes.h"
#include <iostream>

class Helpers {
public:
  static int get_hand_value(Hand hand);
  static Standing get_standing(int player, int dealer);
  static void print_standing(Standing standing);
  static int standing_value(Standing standing);
};

#endif
