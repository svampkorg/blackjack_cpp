#include "../headers/arthandling.h"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string ArtHandling::value_to_sign(int value) {
  switch (value) {
  case 13:
    return "K";
  case 12:
    return "Q";
  case 11:
    return "J";
  case 1:
    return "A";
  default:
    return to_string(value);
  }
}

vector<string> ArtHandling::card_art(Card card) {

  vector<string> card_parts;
  stringstream sstream;
  string padding = card.value == 10 ? " " : "  ";
  string test;

  card_parts.push_back("╭───╮");
  card_parts.push_back("│" + ArtHandling::value_to_sign(card.value) + padding + "│");
  card_parts.push_back("│ " + card.color + " │");
  card_parts.push_back("│" + padding + ArtHandling::value_to_sign(card.value) + "│");
  card_parts.push_back("╰───╯");

  return card_parts;
}

vector<string> ArtHandling::facedown_card_art() {
  vector<string> card_parts;

  card_parts.push_back("╭───╮");
  card_parts.push_back("│?  │");
  card_parts.push_back("│ ? │");
  card_parts.push_back("│  ?│");
  card_parts.push_back("╰───╯");

  return card_parts;
}

string ArtHandling::cards_to_art(vector<optional<Card>> cards) {

  if (cards.empty()) {
    return "";
  }

  vector<vector<string>> cards_arts;

  for (int i = 0; i < cards.size(); ++i) {
    if (cards[i].has_value()) {
      cards_arts.push_back(ArtHandling::card_art(cards[i].value()));
    } else {
      cards_arts.push_back(ArtHandling::facedown_card_art());
    }
  }

  const int full_art_length = cards_arts[0].size();

  const int nr_of_cards = cards_arts.size();
  string art_string;

  for (int art_index = 0; art_index < full_art_length; ++art_index) {
    for (int card_index = 0; card_index < nr_of_cards; ++card_index) {
      art_string.append(cards_arts[card_index][art_index]);
    }

    art_string.append("\n");
  }

  return art_string;
}
  // sstream << "│" << ArtHandling::value_to_sign(card.value) << padding << "│";
  // test.append("│" + ArtHandling::value_to_sign(card.value) + padding + "│");
  // sstream.clear();
  // sstream << "│ " << card.color << " │";
  // sstream.clear();
  // sstream << "│" << padding << ArtHandling::value_to_sign(card.value) << "│";
