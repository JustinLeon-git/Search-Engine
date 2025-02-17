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

// 

#pragma endregion CleanTokenTests

#pragma region GatherTokenTests

// Text with leading spaces
TEST(GatherTokens, Colors) {
  string text = "          one two three";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// Text with trailing spaces
TEST(GatherTokens, Colors) {
  string text = "one two three           ";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

// Text with multiple spaces between words
TEST(GatherTokens, Colors) {
  string text = "one       two         three";
  set<string> expected = {"one", "two", "three"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

#pragma endregion GatherTokenTests