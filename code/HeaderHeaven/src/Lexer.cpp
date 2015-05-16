#include "Lexer.h"
#include <algorithm>
#include <sstream>

Lexer::Lexer() {
	m_pCode = nullptr;
	NullValues();
	m_reversed = false;
}

Lexer::~Lexer() {

}

void Lexer::NullValues() {
	currTok.strId = "";
	currTok.id = TOK_EOF;
	m_codeIt = 0;
}

void Lexer::SetCode(const std::string& code) {
	NullValues();
	m_pCode = &code;
	// TODO. Change all \r\n to \n
}

uint32_t Lexer::GetCursor() const
{
	return m_codeIt;
}

int Lexer::GetChar(int add) {
	if (add < 0 && m_codeIt == 0) return -1; // this'd be a big mistake if this happened
	if (m_codeIt < m_pCode->length()) {
		m_codeIt += add;
		char c = m_pCode->at(m_codeIt - 1);
		return c;
	}
	return -1;
}

int Lexer::PeekChar(int aOffset) {
	if ((m_codeIt+aOffset) < m_pCode->length())
		return m_pCode->at(m_codeIt+aOffset);
	return -1;
}

Lexer::Token Lexer::NextToken() {
	currTok = _NextToken();
	return currTok;
}

Lexer::Token Lexer::PeekToken(uint32_t aOffset) {
	Lexer tmpLexer;
	tmpLexer.SetCode(*m_pCode);
	tmpLexer.m_codeIt = m_codeIt;
	for (aOffset; aOffset > 0; aOffset--)
		tmpLexer.NextToken();
	return tmpLexer.currTok;
}

Lexer::Token Lexer::_NextToken() {
	m_lastChar = GetChar(1);
	if (m_codeIt + 1 > m_pCode->length()) return Token(TOK_EOF);

	// eat comments & new lines
	do {
		if (m_lastChar == '#' || (m_lastChar == '/' && PeekChar() == '/')) {
			// comment line or preprocessor directive. Just skip to the next line
			m_lastChar = GetChar(1);
			while ((m_lastChar = GetChar(1)) != '\n' && m_lastChar != -1) {}
			m_lastChar = GetChar(1);
		}
		else if (m_lastChar == '/' && PeekChar() == '*') {
			// multiline comment.
			m_lastChar = GetChar(2);
			while (m_lastChar != -1) {
				if (m_lastChar == '*' && PeekChar() == '/')
				{
					m_lastChar = GetChar(2);
					break;
				}
				else {
					m_lastChar = GetChar(1);
				}
			}
		}
		else if (isspace(m_lastChar)) {
			m_lastChar = GetChar(1);
		}
		else {
			break;
		}
	} while (true);
	if (m_lastChar == -1) return TOK_EOF;

	if (isalpha(m_lastChar) || m_lastChar == '_' || m_lastChar == '~') {
		// tokenize an identifier
		std::string tokIdentifier = "";
		do {
			tokIdentifier += m_lastChar;
			m_lastChar = GetChar(1);
		} while (isalnum(m_lastChar) || m_lastChar == '_');
		if (m_lastChar != '\n') m_codeIt += -1;
		// if the next two chars are "::", try to get another identifier.
		if (PeekChar() == ':' && PeekChar(1) == ':') {
			tokIdentifier += "::";
			GetChar(2);
			if (NextToken().id == TOK_IDENTIFIER) {
				tokIdentifier = tokIdentifier + currTok.strId;
			}
		}
		if (tokIdentifier == "extern")
			return Token(TOK_EXTERN, tokIdentifier);
		return Token(TOK_IDENTIFIER, tokIdentifier);
	}
	if (isalnum(m_lastChar)) {
		// tokenize a number
		bool hadDot = false; // a double/float
		bool hadX = false; // a hex number
		std::string tokIdentifier = "";
		do {
			if (m_lastChar == '.') {
				if (!hadDot && !hadX)
					hadDot = true;
				else {
					return TOK_DOUBLE;
				}
			}
			else if (m_lastChar == 'x') {
				if (!hadX && !hadDot)
					hadX = true;
				else {
					return Token(TOK_INTEGER);
				}
			}
			tokIdentifier += m_lastChar;
			m_lastChar = GetChar(1);
		} while (isalnum(m_lastChar) || (m_lastChar == '.' && !hadDot) || (m_lastChar == 'x' && !hadX));
		if (m_lastChar != '\n') m_codeIt += -1;
		if (!hadX) {
			if (hadDot)
				return Token(TOK_DOUBLE);
			else
				return Token(TOK_INTEGER);
		}
		else {
			return Token(TOK_INTEGER);
		}
	}
	if (m_lastChar == '\"') {
		// tokenize a string
		std::string tokIdentifier = "";
		bool escaped = false;
		while ((m_lastChar = GetChar(1)) != '\"' && !escaped) {
			escaped = false;
			tokIdentifier += m_lastChar;
			if (m_lastChar == '\\')
				escaped = true;
		}
		return Token(TOK_STRING, tokIdentifier);
	}
	if (m_lastChar == '=' && PeekChar() == '=') {
		m_lastChar = GetChar(1);
		return Token(TOK_EQUAL);
	}
	if (m_lastChar == '|' && PeekChar() == '|') {
		m_lastChar = GetChar(1);
		return Token(TOK_OR);
	}
	if (m_lastChar == '&' && PeekChar() == '&') {
		m_lastChar = GetChar(1);
		return Token(TOK_AND);
	}
	// no idea what it might be. Return by value.
	return Token(m_lastChar);
}