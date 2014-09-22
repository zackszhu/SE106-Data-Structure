#include "TryTokenScanner.h"

#include "tokenscanner.h"

using namespace std;

// Part I
Vector<string> TryTokenScanner::allTokens(string text) {
  Vector<string> tokens;
  // [TODO] put your code here
  TokenScanner scanner(text);
  string str;
  do {
  	str = scanner.nextToken();
  	if (str != "") {
  		tokens.push_back(str);
  	}
  } while (str != "");
  return tokens;
}

// Part II
Vector<string> TryTokenScanner::noWhitespaces(string text) {
  Vector<string> tokens;
  // [TODO] put your code here
  TokenScanner scanner(text);
  scanner.ignoreWhitespace();
  string str;
  do {
  	str = scanner.nextToken();
  	if (str != "") {
  		tokens.push_back(str);
  	}
  } while (str != "");
  return tokens;
}

// Part III
Vector<string> TryTokenScanner::noComments(string text) {
  Vector<string> tokens;
  // [TODO] put your code here
  TokenScanner scanner(text);
  scanner.ignoreComments();
  string str;
  do {
  	str = scanner.nextToken();
  	if (str != "") {
  		tokens.push_back(str);
  	}
  } while (str != "");
  return tokens;
}
