#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "blackjacktypes.h"
#include <mutex>
#include <optional>
#include <random>
#include <tuple>

using namespace std;

class State {
public:
  Deck currently_dealt;
  Hand player_hand;
  Hand dealer_hand;
  int player_wallet;
  int player_bet;

  void shuffle();
  void initial_deal();
  void hit_player();
  void hit_dealer();
  void bet();
  void maybe_payout();
  void draw_rules();
  void draw_logo();
  void clear_console();
  void draw_board(bool conceal_dealer);
  int get_dealer_hand_value();
  int get_player_hand_value();
  Standing get_standing();
  bool player_has_won();
  optional<Card> random_card_from(const Deck& available_cards);
  Deck available_deck(const Deck& currently_dealt);
  tuple<vector<optional<Card>>, Deck>
  deal_cards(const Deck& available_cards, const Deck& dealt_cards, int nr_to_deal);
private:
  mutable recursive_mutex mtx;
  random_device rd;
};
#endif
