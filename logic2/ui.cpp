#include "ui.h"

#include <iostream>
#include "card.h"

MUI::MUI() {
}
MCard* MUI::selectCard(const std::vector<MCard*>& cards) {
  int sel;
  for(int i=0; i<cards.size(); i++) {
    std::cout<<i<<" - "<<cards[i]->getName()<<std::endl;
  }
  std::cin >> sel;
  if(sel > cards.size()) sel = 0;
  return cards[sel];
}
bool MUI::askQuestion(const std::string& question) {
  int sel;
  std::cout<<question<<std::endl;
  std::cout<<"0 - no, 1 - yes"<<std::endl;
  std::cin >> sel;
  return (bool)sel;
}
std::string MUI::select(const std::vector<std::string>& data) {
  int sel;
  for(int i=0; i<data.size(); i++) {
    std::cout<<i<<" - "<<data[i]<<std::endl;
  }
  std::cin >> sel;
  if(sel > data.size()) sel = 0;
  return data[sel];
}
