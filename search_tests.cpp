#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

#pragma region CleanTokenTests

// Tokens with punctuation at both the beginning and end
TEST(CleanToken, PrefixAndSuffixCleaning) {
  ASSERT_THAT(cleanToken(".HELLO."), StrEq("hello"));
  ASSERT_THAT(cleanToken("...heLlo..."), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?hellO.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken(",HELLO,"), StrEq("hello"));
  ASSERT_THAT(cleanToken("...Hello,,,"), StrEq("hello"));
  ASSERT_THAT(cleanToken(".\"!?heLLo.\"!?"), StrEq("hello"));
}

// Tokens with punctuation in the middle, as well as possibly at the ends
TEST(CleanToken, MiddlePunctAndEndCleaning) {
  ASSERT_THAT(cleanToken("HE..LLO"), StrEq("he..llo"));
  ASSERT_THAT(cleanToken("he...Llo..."), StrEq("he...llo"));
  ASSERT_THAT(cleanToken("h.e.l.l.O.\"!?"), StrEq("h.e.l.l.o"));
  ASSERT_THAT(cleanToken("H.E.L.L.O"), StrEq("h.e.l.l.o"));
  ASSERT_THAT(cleanToken("Hell.o,,,"), StrEq("hell.o"));
  ASSERT_THAT(cleanToken("he.\"!?LL.\"!?o.\"!?"), StrEq("he.\"!?ll.\"!?o"));
}

// Tokens with punctuation in the middle, as well as possibly at the beginning
TEST(CleanToken, MiddlePunctAndBeginningCleaning) {
  ASSERT_THAT(cleanToken("HE..LLO"), StrEq("he..llo"));
  ASSERT_THAT(cleanToken("...he...Llo"), StrEq("he...llo"));
  ASSERT_THAT(cleanToken(".\"!?h.e.l.l.O"), StrEq("h.e.l.l.o"));
  ASSERT_THAT(cleanToken(".H.E.L.L.O"), StrEq("h.e.l.l.o"));
  ASSERT_THAT(cleanToken("Hell.o"), StrEq("hell.o"));
  ASSERT_THAT(cleanToken("\"!?.he.\"!?LL.\"!?o"), StrEq("he.\"!?ll.\"!?o"));
}

// Tokens with numbers at the end
TEST(CleanToken, NumAtEnd) {
  ASSERT_THAT(cleanToken("HELLO1"), StrEq("hello1"));
  ASSERT_THAT(cleanToken("heLlo123"), StrEq("hello123"));
  ASSERT_THAT(cleanToken("hellO0"), StrEq("hello0"));
  ASSERT_THAT(cleanToken("hellO011223344"), StrEq("hello011223344"));
}

// Tokens that are 1 letter long
TEST(CleanToken, OneLetter) {
  ASSERT_THAT(cleanToken("x"), StrEq("x"));
  ASSERT_THAT(cleanToken("y"), StrEq("y"));
  ASSERT_THAT(cleanToken("z"), StrEq("z"));
  ASSERT_THAT(cleanToken("X"), StrEq("x"));
  ASSERT_THAT(cleanToken("Y"), StrEq("y"));
  ASSERT_THAT(cleanToken("Z"), StrEq("z"));
}

#pragma endregion CleanTokenTests

#pragma region GatherTokenTests

// Text with leading spaces
TEST(GatherTokens, LeadingSpaces) {
  string text = "          one two three";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// Text with trailing spaces
TEST(GatherTokens, TrailingSpaces) {
  string text = "one two three           ";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// Text with multiple spaces between words
TEST(GatherTokens, SpacesBetween) {
  string text = "one       two         three";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

#pragma endregion GatherTokenTests

#pragma region BuildIndexTests

// Testing invalid filenames
TEST(BuildIndex, InvalidFilename) {
  string filename = "data/bogus.txt";
  map<string, set<string>> index;
  ASSERT_FALSE(filesystem::exists(filename))
    << "Filename of " + filename + " actually exists when it shouldn't";
  int websiteCount = buildIndex(filename, index);
  EXPECT_TRUE(index.empty())
    << "Index is not empty with invalid filename";
  EXPECT_THAT(websiteCount, Eq(0))
    << "Website count equal to " + to_string(websiteCount) + " not 0 with invalid filename";
}

// Checking with tiny.txt for valid build of index
TEST(BuildIndex, Tiny) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}},
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

#pragma endregion BuildIndexTests

#pragma region FindQueryMatches

map<string, set<string>> INDEX = {
  {"hello", {"example.com", "uic.edu"}},
  {"there", {"example.com"}},
  {"according", {"uic.edu"}},
  {"to", {"uic.edu"}},
  {"all", {"example.com", "uic.edu", "random.org"}},
  {"known", {"uic.edu"}},
  {"laws", {"random.org"}},
  {"of", {"random.org"}},
  {"aviation", {"random.org"}},
  {"a", {"uic.edu", "random.org"}},
};

// First query term isn't in the index
TEST(FindQueryMatches, FirstTermMissing) {
  set<string> expected;
  
  expected = {"example.com"};
  EXPECT_THAT(findQueryMatches(INDEX, "bingo there"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "bongo -hello"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "bango +Hello!"), ContainerEq(expected));
}

// Later query term not in index
TEST(FindQueryMatches, LaterTermMissing) {
  set<string> expected;
  
  expected = {"example.com", "uic.edu", "random.org"};
  EXPECT_THAT(findQueryMatches(INDEX, "all bingo"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "all -bango"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "all, +bongo"), ContainerEq(expected));
}



#pragma endregion FindQueryMatches