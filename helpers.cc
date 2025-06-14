#include "headers/helpers.h"
#include "headers/blackjacktypes.h"
#include <iostream>

int Helpers::get_hand_value(Hand hand) {
  int value = 0;

  for (int i = 0; i < hand.size(); ++i) {
    if (!hand[i].has_value())
      continue;

    Card card = hand[i].value();

    if (card.value - 10 > 0) {
      value += 10;
    } else if (card.value == 1) {
      if (value + 11 > 21) {
        value += 1;
      } else {
        value += 11;
      }
    } else {
      value += card.value;
    }
  }
  return value;
}

Standing Helpers::get_standing(int player, int dealer) {
  if (player > 21) {
    return Standing::PlayerBust;
  } else if (dealer > 21) {
    return Standing::DealerBust;
  }
  if (dealer == 21) {
    return Standing::DealerBlackjack;
  } else if (player == 21) {
    return Standing::PlayerBlackjack;
  } else if (player == dealer) {
    return Standing::Draw;
  } else if (player > dealer) {
    return Standing::Player;
  } else {
    return Standing::Dealer;
  }
}

int Helpers::standing_value(Standing standing) {
  switch (standing) {
  case Standing::Draw:
    return 1;
  case Standing::PlayerBlackjack:
    return 2;
  case Standing::DealerBlackjack:
    return 3;
  case Standing::Player:
    return 4;
  case Standing::Dealer:
    return 5;
  case Standing::DealerBust:
    return 6;
  case Standing::PlayerBust:
    return 7;
  }
}

void Helpers::print_standing(Standing standing) {
  switch (standing) {
  case Standing::Draw:
    std::cout << "It's a draw! Nobody wins! ¯\\_(ツ)_/¯\n";
    break;
  case Standing::PlayerBlackjack:
    std::cout << "You win on Blackjack! ⊂(◉‿◉)つ\n";
    break;
  case Standing::DealerBlackjack:
    std::cout << "Dealer wins on Blackjack! (´סּ︵סּ`)\n";
    break;
  case Standing::Player:
    std::cout << "You Win! ᕕ( ᐛ )ᕗ\n";
    break;
  case Standing::Dealer:
    std::cout << "Dealer Wins! (ⱺ ʖ̯ⱺ)\n";
    break;
  case Standing::DealerBust:
    std::cout << "Dealer is Bust! You Win! (͠≖ ͜ʖ͠≖) hehe\n";
    break;
  case Standing::PlayerBust:
    std::cout << "You are Bust! Dealer wins! ୧༼ಠ益ಠ༽୨\n";
    break;
  }
}
