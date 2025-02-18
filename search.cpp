#include "include/search.h"

#include <filesystem>
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
  // Empty text
  set<string> tokenSet;
  if (!text.size()) {
    return tokenSet;
  }

  size_t subStart, curr = 0;
  while (curr < text.size()) {
    // Traversing garbage
    if (!isalpha(text.at(curr))) {
      curr++;
    // We encounter a token
    } else {
      subStart = curr;
      // Traversing token
      while (curr < text.size() && !isspace(text.at(curr))) {
        curr++;
      }
      // Put token in set
      tokenSet.insert(cleanToken(text.substr(subStart, curr - subStart)));
    }
  }
  return tokenSet;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  if (!filesystem::exists(filename)) {
    return 0;
  }

  ifstream file(filename);
  int websites = 0;
  
  // Now we traverse the file
  bool websiteFlag = true;
  string line, currWebsite;
  while (getline(file, line)) {
    // Processing website
    if (websiteFlag) {
      currWebsite = line;
      websites++;
      websiteFlag = false;
    // Processing website text
    } else {
      set<string> tokens = gatherTokens(line);
      // Looping through every word
      for (string token : tokens) {
        index[token].insert(currWebsite);
      }
      websiteFlag = true;
    }
  }

  return websites;
}

set<string> findQueryMatches(const map<string, set<string>>& index, const string& sentence) {
  set<string> runSet, opSet, resSet;
  string key;
  size_t start = 0;
  size_t curr = 0;
  char op = '\0';
  // Traversing through sentence
  while (curr <= sentence.size()) {
    // Found a full string
    if (curr == sentence.size() || isspace(sentence.at(curr))) {
      key = sentence.substr(start, curr - start);
      // Checking if we have an op
      if (key.at(0) == '+' || key.at(0) == '-') {
        op = key.at(0);
        key.erase(0, 1);
      }
      key = cleanToken(key);

      // Determining the set we're operating on
      if (index.contains(key)) {
        opSet = index.at(key);
      } else {
        opSet.clear();
      }

      // Now we perform the given operation
      if (op == '+') {
        set_intersection(runSet.begin(), runSet.end(), opSet.begin(), opSet.end(), inserter(resSet, resSet.end()));
      } else if (op == '-') {
        set_difference(runSet.begin(), runSet.end(), opSet.begin(), opSet.end(), inserter(resSet, resSet.end()));
      } else {
        set_union(runSet.begin(), runSet.end(), opSet.begin(), opSet.end(), inserter(resSet, resSet.end()));
      }
      // Prepping for next word
      op = '\0';
      runSet = resSet;
      resSet.clear();
      start = curr + 1;
    }
    // Traversing through a word
    curr++;
  }

  return runSet;
}

void searchEngine(const string& filename) {
  // TODO student
}
