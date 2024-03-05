#ifndef uiH
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
  std::string select(const std::vector<std::string>& data);
};

#endif
