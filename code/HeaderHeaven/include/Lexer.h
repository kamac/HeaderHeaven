#ifndef HHEAVEN_LEXER_H
#define HHEAVEN_LEXER_H

#include "Common.h"

class Lexer {
public:
	enum TOKENS {
		TOK_EOF = -10, TOK_IDENTIFIER, TOK_INTEGER, TOK_DOUBLE, TOK_STRING, TOK_PREPROC, TOK_EXTERN,
		TOK_EQUAL, TOK_OR, TOK_AND
	};

	struct Token {
		Token(int aId = TOK_EOF, std::string aStrId = "") : id(aId), strId(aStrId) { }
		int id;
		std::string strId;
	};

	Lexer();
	~Lexer();

	void SetCode(const std::string& apCode);
	Token NextToken();
	Token PeekToken(uint32_t aOffset = 1);

	Token currTok;

private:
	int GetChar(int aAdd);
	int PeekChar(int aOffset = 0);
	void NullValues();
	Token _NextToken();

	const std::string *m_pCode;
	int m_lastChar;
	bool m_reversed;
	unsigned int m_codeIt;
};

#endif