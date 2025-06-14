#ifndef ARTHANDLING_H
#define ARTHANDLING_H

#include "blackjacktypes.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

class ArtHandling {
public:
  static string value_to_sign(int value);
  static vector<string> card_art(Card card);
  static vector<string> facedown_card_art();
  static string cards_to_art(vector<optional<Card>>);
};

#endif
