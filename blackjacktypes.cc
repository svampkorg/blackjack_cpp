#include "headers/blackjacktypes.h"

Card::Card(std::string c, int v) : color(std::move(c)), value(v) { }

std::ostream& operator<<(std::ostream& os, const Card& card) {
  return os << card.color << ":" << card.value;
}

const Deck deck = {
    {"󰣎", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }},
    {"󰣑", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }},
    {"󰣐", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }},
    {"󰣏", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 }},
};
