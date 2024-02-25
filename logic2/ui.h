#ifndef ui*207H
#define uiH

#include <string>
#include <vector>

class MCard;

class MUI {
private:
public:
  MUI();
  MCard* selectCard(const std::vector<MCard*>& cards);
  bool askQuestion(const std::string& question);
  std::string selectAction(const std::vector<std::string>& actions);
};

#endif
