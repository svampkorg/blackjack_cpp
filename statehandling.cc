#include "headers/statehandling.h"
#include "headers/art.h"
#include "headers/arthandling.h"
#include "headers/blackjacktypes.h"
#include "headers/helpers.h"

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
    stringstream ss(input);
    ss >> input;

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
  cout << "Get as close to 21 as possible by either stand, or hit (Hit means "
          "draw a card).";
  cout << "Ace counts as 11, unless your total pass 21, in which case it "
          "counts as 1.";
  cout << "2 through 9 counts as is.";
  cout << "10, Jackal, Queen and King all count as 10.\n";
  cout << "A hand over 21 is considered a bust, a loss.\n";
  cout << "Total of 21 equals Jackpot, a win. Unless dealer also has Jackpot, "
          "in which case dealer wins.";
  cout << "Dealer must draw at a hand < 16, and stand at 17 or above.\n";
  cout << "Jackpot payoff is 3/2 and a regular win is 1/1.\n";
  cout << "Press any key to continue.";
}

void State::draw_logo() {
  lock_guard<recursive_mutex> lock(mtx);
  clear_console();
  cout << Art::logo;
  cout << "Player wallet: " << player_wallet << endl;
}

void State::clear_console() {
  // cout << "\u{001B}[2J\u{001B}[H";
  system("clear");
}

void State::draw_board(bool conceal_dealer = true) {
  lock_guard<recursive_mutex> lock(mtx);
  draw_logo();
  if (conceal_dealer) {
    cout << "Your hand:" << endl
         << ArtHandling::cards_to_art(player_hand)
         << "Value: " << get_player_hand_value() << endl
         << "Dealer's hand:" << endl
         << ArtHandling::cards_to_art({dealer_hand[0], nullopt});
  } else {
    cout << "Your hand:" << endl
         << ArtHandling::cards_to_art(player_hand)
         << "Value: " << get_player_hand_value() << endl
         << "Dealer's hand:" << endl
         << ArtHandling::cards_to_art(dealer_hand)
         << "Value: " << get_dealer_hand_value() << endl;
  }
}

int State::get_dealer_hand_value() {
  lock_guard<recursive_mutex> lock(mtx);
  return Helpers::get_hand_value(dealer_hand);
}

int State::get_player_hand_value() {
  lock_guard<recursive_mutex> lock(mtx);
  return Helpers::get_hand_value(player_hand);
}

Standing State::get_standing() {
  lock_guard<recursive_mutex> lock(mtx);
  return Helpers::get_standing(get_player_hand_value(),
                               get_dealer_hand_value());
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

optional<Card> State::random_card_from(const Deck &available_cards) {
  lock_guard<recursive_mutex> lock(mtx);

  if (available_cards.empty()) {
    return nullopt;
  }

  mt19937 mt(rd());

  uniform_int_distribution<int> random_color(0, available_cards.size() - 1);
  auto colorIterator = available_cards.begin();
  advance(colorIterator, random_color(mt));
  string color = colorIterator->first;

  uniform_int_distribution<int> random_value(0, available_cards.size() - 1);
  auto values = available_cards.find(color)->second;
  auto valuesIterator = values.begin();
  advance(valuesIterator, random_value(mt));
  int value = *valuesIterator;

  return Card(color, value);
}

Deck State::available_deck(const Deck &currently_dealt) {
  lock_guard<recursive_mutex> lock(mtx);
  Deck full_deck_copy = master_deck;

  // for every key, pair in currently_dealt cards
  for (const auto &pair : currently_dealt) {
    const string &color = pair.first;
    const vector<int> &dealt_cards = pair.second;

    // find the color/suit in the full deck. Gives us an iterator.
    auto it = full_deck_copy.find(color);

    if (it == full_deck_copy.end()) {
      // the iterator points to the end, we did not find what we were looking
      // for We should really always find the color, unless the master_deck
      // changed.
      continue;
    }

    // The values for this particular color/suit, a vector<int>
    vector<int> &values_in_full_deck_copy = it->second;

    // New vector<int> for which we ..
    vector<int> available_cards;
    // .. go through all the values ..
    for (int card_value : values_in_full_deck_copy) {
      // .. where we reach the end of dealt_cards, we did not find the card
      // to have been dealt, so we put that card in available_cards.
      if (find(dealt_cards.begin(), dealt_cards.end(), card_value) ==
          dealt_cards.end()) {
        available_cards.push_back(card_value);
      }
    }
    // replace the vector<int> in the reference of the full_deck_copy
    // with the cards deemed still available.
    values_in_full_deck_copy = available_cards;
  }
  return full_deck_copy;
}

tuple<vector<optional<Card>>, Deck>
State::deal_cards(const Deck &available_cards, const Deck &dealt_cards,
                  int nr_to_deal) {
  lock_guard<recursive_mutex> lock(mtx);
  vector<optional<Card>> new_cards;
  Deck currently_dealt = dealt_cards;

  for (int i = 0; i < nr_to_deal; ++i) {
    auto new_card = random_card_from(available_cards);
    if (!new_card.has_value()) {
      continue;
    }
    currently_dealt = add_card_to_deck(currently_dealt, new_card.value());
    new_cards.push_back(new_card);
  }

  return {new_cards, currently_dealt};
}

Deck State::add_card_to_deck(const Deck &current_deck, const Card &card) {
  Deck new_current_deck = current_deck;
  new_current_deck[card.color].push_back(card.value);
  return new_current_deck;
}
