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
TEST(CleanToken, MiddleAndSuffixCleaning) {
  ASSERT_THAT(cleanToken("HE..LLO"), StrEq("hello"));
  ASSERT_THAT(cleanToken("he...Llo..."), StrEq("hello"));
  ASSERT_THAT(cleanToken("h.e.l.l.O.\"!?"), StrEq("hello"));
  ASSERT_THAT(cleanToken("H.E.L.L.O"), StrEq("hello"));
  ASSERT_THAT(cleanToken("Hell.o,,,"), StrEq("hello"));
  ASSERT_THAT(cleanToken("he.\"!?LL.\"!?o.\"!?"), StrEq("hello"));
}

// Tokens with numbers at the end
TEST(CleanToken, NumAtEnd) {
  ASSERT_THAT(cleanToken("HELLO1"), StrEq("hello"));
  ASSERT_THAT(cleanToken("heLlo123"), StrEq("hello"));
  ASSERT_THAT(cleanToken("hellO0"), StrEq("hello"));
  ASSERT_THAT(cleanToken("hellO011223344"), StrEq("hello"));
}

// Tokens that are 1 letter long
TEST(CleanToken, OneLetter) {
  ASSERT_THAT(cleanToken("x"), StrEq("x"));
  ASSERT_THAT(cleanToken("y"), StrEq("y"));
  ASSERT_THAT(cleanToken("z"), StrEq("z"));
  ASSERT_THAT(cleanToken("X"), StrEq("X"));
  ASSERT_THAT(cleanToken("Y"), StrEq("Y"));
  ASSERT_THAT(cleanToken("Z"), StrEq("Z"));
}

#pragma endregion CleanTokenTests

