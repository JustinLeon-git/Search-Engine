#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  // Checking for empty tokens
  if (token.empty()) {
    return "";
  }

  // Checking for letter in token
  bool hasLetter = false;
  for (char c : token) {
    if (isalpha(c)) {
        hasLetter = true;
        break;
    }
  }
  if (!hasLetter) {
    return "";
  }

  // Finding first letter
  size_t firstLetter;
  for (size_t i = 0; i < token.size(); i++) {
    if (isalpha(token.at(i))) {
      firstLetter = i;
      break;
    }
  }
  // Finding last letter
  size_t lastLetter;
  for (size_t i = token.size() - 1; i >= 0; i--) {
    if (isalpha(token.at(i))) {
      lastLetter = i;
      break;
    }
  }

  string newToken;
  // Cleansing Prefix
  for (size_t i = 0; i < firstLetter; i++) {
    if (!ispunct(token.at(i))) {
      newToken.push_back(token.at(i));
    }
  }
  // Cleansing middle
  for (size_t i = firstLetter; i <= lastLetter; i++) {
    if (isalpha(token.at(i))) {
      newToken.push_back(tolower(token.at(i)));
    } else {
      newToken.push_back(token.at(i));
    }
  }
  // Cleansing suffix
  for (size_t i = lastLetter + 1; i < token.size(); i++) {
    if (!ispunct(token.at(i))) {
      newToken.push_back(token.at(i));
    }
  }
  
  return newToken;
}

set<string> gatherTokens(const string& text) {
  // TODO student
  return {};
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student
  return 0;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  return {};
}

void searchEngine(const string& filename) {
  // TODO student
}
