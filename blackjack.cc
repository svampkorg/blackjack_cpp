#include "headers/blackjack.h"
#include "headers/blackjacktypes.h"
#include "headers/debug.h"
#include "headers/helpers.h"
#include <sstream>

void Blackjack::run() {
  bool is_play_again = true;

  DEBUG_PRINT("Your wallet: $" << state.player_wallet << endl);

  while (is_play_again) {
    state.shuffle();
    state.initial_deal();

    state.draw_logo();

    state.bet();

    bool is_hit = true;

    state.draw_board(true);

    while (state.get_standing() != Standing::PlayerBlackjack &&
           state.get_standing() != Standing::PlayerBust && is_hit) {
      cout << "Hit? (enter 'y' to draw another card): ";

      string input;
      getline(cin, input);
      stringstream ss(input);
      ss >> input;

      if (input != "y") {
        is_hit = false;
        continue;
      }

      state.hit_player();
      state.draw_board(true);
    }

    // NOTE: No need for dealer to play if Player is bust
    if (state.get_standing() != Standing::PlayerBust) {
      while (state.get_standing() != Standing::DealerBlackjack &&
             state.get_standing() != Standing::DealerBust &&
             state.get_dealer_hand_value() < 17) {
        state.hit_dealer();
      }
    }

    state.maybe_payout();
    state.draw_board(false);

    DEBUG_PRINT("Your wallet: $" << state.player_wallet << endl);

    Helpers::print_standing(state.get_standing());

    if (state.player_wallet <= 0) {
      is_play_again = false;
    } else {
      cout << "Do you want to keep going? (enter 'y' to continue): ";

      string input;
      getline(cin, input);
      stringstream ss(input);
      ss >> input;

      if (input != "y") {
        is_play_again = false;
        continue;
      }

      is_play_again = true;
    }
  }

  DEBUG_PRINT("Your wallet: $" << state.player_wallet << endl);

  if (state.player_wallet == 0) {
    cout << "Game's Over! You have nothing left to bet with." << endl;
  } else if (state.player_wallet == 25) {
    cout << "You are break even. Lucky you!" << endl << "Bye!" << endl;
  } else if (state.player_wallet > 25) {
    cout << "You've earned $" << state.player_wallet - 25
         << ". Congratulations! Bye!" << endl;
  } else if (state.player_wallet < 25) {
    cout << "You've lost $" << 25 - state.player_wallet
         << ". Very unfortunate. Bye!" << endl;
  }
}
