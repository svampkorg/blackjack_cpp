#include "headers/statehandling.h"
#include "headers/art.h"
#include "headers/blackjacktypes.h"
#include <cstdlib>
#include <exception>
#include <iterator>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <tuple>

void State::shuffle() {
  currently_dealt = {};
  player_hand = {};
  dealer_hand = {};
}
void State::initial_deal() {
  lock_guard<recursive_mutex> lock(mtx);

  // NOTE: Player
  Deck available_cards = available_deck(currently_dealt);
  const DealData p_deal_data = deal_cards(available_cards, currently_dealt, 2);
  currently_dealt = get<1>(p_deal_data);
  player_hand.insert(player_hand.end(), get<0>(p_deal_data).begin(),
                     get<0>(p_deal_data).end());

  // NOTE: Dealer
  available_cards = available_deck(currently_dealt);
  const DealData d_deal_data = deal_cards(available_cards, currently_dealt, 2);
  currently_dealt = get<1>(d_deal_data);
  dealer_hand.insert(dealer_hand.end(), get<0>(d_deal_data).begin(),
                     get<0>(d_deal_data).end());
}
void State::hit_player() {
  lock_guard<recursive_mutex> lock(mtx);
  const Deck available_cards = available_deck(currently_dealt);
  const DealData deal_data = deal_cards(available_cards, currently_dealt, 1);

  currently_dealt = get<1>(deal_data);
  player_hand.insert(player_hand.end(), get<0>(deal_data).begin(),
                     get<0>(deal_data).end());
}
void State::hit_dealer() {
  lock_guard<recursive_mutex> lock(mtx);
  const Deck available_cards = available_deck(currently_dealt);
  const DealData deal_data = deal_cards(available_cards, currently_dealt, 1);

  currently_dealt = get<1>(deal_data);
  dealer_hand.insert(dealer_hand.end(), get<0>(deal_data).begin(),
                     get<0>(deal_data).end());
}
void State::bet() {
  lock_guard<recursive_mutex> lock(mtx);
  bool keep_asking = true;

  while (keep_asking) {
    cout << "First, place a bet $";

    string input;

    getline(cin, input);
    int amount = 0;

    try {
      amount = stoi(input);
      if (amount > player_wallet) {
        cout << "You cannot bet more than you have" << endl;
        continue;
      } else if (amount == 0) {
        cout << "Common you have to bet a little!" << endl;
        continue;
      }
    } catch (exception &err) {
      cout << "There was an error with that value: " << err.what() << endl;
      continue;
    }

    keep_asking = false;
    player_bet = amount;
    player_wallet -= amount;
  }
}
void State::maybe_payout() {
  lock_guard<recursive_mutex> lock(mtx);
  double multiple = 0;

  if (State::get_standing() == Standing::Draw) {
    multiple = 0;
  } else if (State::get_standing() == Standing::PlayerBlackjack) {
    multiple = 1.5;
  } else if (State::player_has_won()) {
    multiple = 1;
  } else {
    return;
  }

  const double payout = (double)player_bet * multiple;
  player_wallet += player_bet + (int)(round(payout));
}
void State::draw_rules() {
  draw_logo();

  cout << "\nThe rules are as follow:\n";
  cout << "Get as close to 21 as possible by either stand, or hit (Hit means draw a card).";
  cout << "Ace counts as 11, unless your total pass 21, in which case it counts as 1.";
  cout << "2 through 9 counts as is.";
  cout << "10, Jackal, Queen and King all count as 10.\n";
  cout << "A hand over 21 is considered a bust, a loss.\n";
  cout << "Total of 21 equals Jackpot, a win. Unless dealer also has Jackpot, in which case dealer wins.";
  cout << "Dealer must draw at a hand < 16, and stand at 17 or above.\n";
  cout << "Jackpot payoff is 3/2 and a regular win is 1/1.\n";
  cout << "Press any key to continue.";
}
void State::draw_logo() {
  lock_guard<recursive_mutex> lock(mtx);
  clear_console();
  cout << Art::logo;
  cout << "Player wallet: " << player_wallet;
}
void State::clear_console() {
  // cout << "\u{001B}[2J\u{001B}[H";
  system("clear");
}
void State::draw_board(bool conceal_dealer = true) {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
int State::get_dealer_hand_value() {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
int State::get_player_hand_value() {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
Standing State::get_standing() {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
bool State::player_has_won() {
  switch (State::get_standing()) {
  case Standing::Player:
    return true;
  case Standing::DealerBust:
    return true;
  case Standing::PlayerBlackjack:
    return true;
  default:
    return false;
  }
}
optional<Card> State::random_card_from(const Deck& available_cards) {
  lock_guard<recursive_mutex> lock(mtx);

  if (available_cards.empty()) { return nullopt; }

  mt19937 mt(rd());

  uniform_int_distribution<int> random_color(0, available_cards.size());
  auto mapIt = available_cards.begin();
  advance(mapIt, random_color(mt));
  // advance(mapIt, rand() % available_cards.size());
  string color = mapIt->first;

  uniform_int_distribution<int> random_value(0, available_cards.size());
  auto values = available_cards.find(color)->second;
  auto valuesIt = values.begin();
  advance(valuesIt, random_value(mt));
  // advance(valuesIt, rand() % available_cards.size());
  int value = *valuesIt;

  return Card(color, value);
}

Deck State::available_deck(const Deck& currently_dealt) {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
tuple<vector<optional<Card>>, Deck>
State::deal_cards(const Deck& available_cards, const Deck& dealt_cards,
                          int nr_to_deal) {
  lock_guard<recursive_mutex> lock(mtx);
  // TODO:
}
