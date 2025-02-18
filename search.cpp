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

string cleanToken(const string &token) {
  if (token.empty()) {
    return "";
  }

  int start = 0;
  int end = token.size() - 1;

  // Remove punctuation from the beginning.
  while (start < token.size() && ispunct(token[start])) {
    start++;
  }
  // Remove punctuation from the end.
  while (end >= start && ispunct(token[end])) {
    end--;
  }
  if (start > end) {
    return "";
  }

  // Check that there is at least one alphabetic character.
  bool hasLetter = false;
  for (int i = start; i <= end; i++) {
    if (isalpha(token[i])) {
      hasLetter = true;
      break;
    }
  }
  if (!hasLetter) {
    return "";
  }

  string result;
  for (int i = start; i <= end; i++) {
    result.push_back(tolower(token[i]));
  }
  return result;
}

set<string> gatherTokens(const string& text) {
  set<string> tokenSet;
  if (text.empty()) {
    return tokenSet;
  }
  
  istringstream stream(text);
  string token;
  while (stream >> token) {
    string cleaned = cleanToken(token);
    if (!cleaned.empty()) {
      tokenSet.insert(cleaned);
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
  map<string, set<string>> index;
  
  // If the file doesn't exist, print the error.
  if (!filesystem::exists(filename)) {
    cout << "Invalid filename." << "\n";
  }
  
  cout << "Stand by while building index..." << "\n";
  int pages = buildIndex(filename, index);
  cout << "Indexed " << pages << " pages containing " << index.size() << " unique terms" << "\n";
  
  string input;
  while (true) {
    // Print the prompt WITHOUT a newline for the tests
    cout << "Enter query sentence (press enter to quit): ";
    if (!getline(cin, input))
      break;
      
    // Trim off the leading and trailing whitespace.
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == string::npos)
      input = "";
    else {
      size_t end = input.find_last_not_of(" \t\r\n");
      input = input.substr(start, end - start + 1);
    }
    
    // If the query is empty, print the exit message on the same line and break.
    if (input.empty()) {
      cout << "Thank you for searching!";
      break;
    }
    
    // Process the query.
    set<string> matches = findQueryMatches(index, input);
    cout << "Found " << matches.size() << " matching pages" << "\n";
    for (const string &page : matches) {
      cout << page << "\n";
    }
  }
}
