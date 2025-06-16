#ifndef BLACKJACK_TYPES_H
#define BLACKJACK_TYPES_H

#include <iostream>
#include <unordered_map>
#include <optional>
#include <string>
#include <vector>
#include <utility>

using namespace std;

enum class Standing {
  Player,
  Dealer,
  PlayerBlackjack,
  DealerBlackjack,
  PlayerBust,
  DealerBust,
  Draw
};

struct Card {
  string color;
  int value;

  Card(string c, int v);
};

ostream& operator<<(ostream& os, const Card& card);

using Hand = vector<optional<Card>>;
using Deck = unordered_map<string, vector<int>>;
using DealData = tuple<vector<optional<Card>>, Deck>;

extern const Deck master_deck;

#endif
