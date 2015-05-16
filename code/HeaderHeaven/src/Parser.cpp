#include "Parser.h"
#include <algorithm>

Parser::Parser(const std::string& aSource)
{
	m_lexer.SetCode(aSource);
}

Parser::~Parser()
{

}

void Parser::ParseHeader(std::vector<Class>& aOutClasses)
{
	do {
		TryParseNamespace(aOutClasses);
		Class cClass;
		if (TryParseClass(cClass)) {
			aOutClasses.push_back(cClass);
		}
	} while (m_lexer.PeekToken().id != Lexer::TOK_EOF);
}

void Parser::ParseSource(std::vector<Function>& aOutFunctions, std::vector<std::string>& aUsedNamespaces)
{
	do {
		Lexer::Token peekToken = m_lexer.PeekToken();
		if (peekToken.id = Lexer::TOK_IDENTIFIER && peekToken.strId == "using") {
			m_lexer.NextToken();
			peekToken = m_lexer.PeekToken();
			if (peekToken.id == Lexer::TOK_IDENTIFIER && peekToken.strId == "namespace") {
				m_lexer.NextToken();
				m_lexer.NextToken();
				if (m_lexer.currTok.id == Lexer::TOK_IDENTIFIER) {
					aUsedNamespaces.push_back(m_lexer.currTok.strId);
					if (m_lexer.NextToken().id != ';') {
						return; // syntax error
					}
				}
				else {
					return; // syntax error
				}
			}
		}
		Function func;
		if (TryParseFunction(func, ""))
			aOutFunctions.push_back(func);
	} while (m_lexer.PeekToken().id != Lexer::TOK_EOF);
}

bool Parser::TryParseNamespace(std::vector<Class> &aOutClasses)
{
	Lexer::Token peekToken = m_lexer.PeekToken();
	if (!(peekToken.id == Lexer::TOK_IDENTIFIER && peekToken.strId == "namespace"))
		return false;
	m_lexer.NextToken();

	if (m_lexer.NextToken().id != Lexer::TOK_IDENTIFIER)
		return false; // syntax error

	std::string nameSpace = m_lexer.currTok.strId;
	if (m_lexer.NextToken().id != '{')
		return false; // syntax error

	do {
		std::vector<Class> newClasses;
		if (TryParseNamespace(newClasses)) {
			for (uint16_t i = 0; i < newClasses.size(); i++) {
				if (newClasses[i].nameSpace.empty())
					newClasses[i].nameSpace = nameSpace;
				else
					newClasses[i].nameSpace = nameSpace + "::" + newClasses[i].nameSpace;
				aOutClasses.push_back(newClasses[i]);
			}
		}

		Class cClass;
		if (TryParseClass(cClass)) {
			cClass.nameSpace = nameSpace;
			aOutClasses.push_back(cClass);
		}
	} while (m_lexer.PeekToken().id != '}');
	m_lexer.NextToken();
}

bool Parser::TryParseClass(Class &aOutClass)
{
	Lexer::Token peekToken = m_lexer.PeekToken();
	if (!(peekToken.id == Lexer::TOK_IDENTIFIER && (peekToken.strId == "class" || peekToken.strId == "struct")))
		return false;
	m_lexer.NextToken();

	// this may be a class/struct declaration
	// process all rubbish up to either ; or {. Take the last identifier as the name of the class.
	aOutClass.name = "";
	while (m_lexer.NextToken().id != Lexer::TOK_EOF) {
		if (m_lexer.currTok.id == ';') {
			// false alarm. Just a forward class/struct declaration. Disregard.
			return false;
		}
		else if (m_lexer.currTok.id == '{') {
			break;
		}
		else {
			Lexer::Token peekToken = m_lexer.PeekToken();
			if ((peekToken.id == ':' || peekToken.id == '{') && aOutClass.name.empty())
				aOutClass.name = m_lexer.currTok.strId;
		}
	}
	// process class' body
	bool parsedFunctionBody = false;
	while (m_lexer.currTok.id != Lexer::TOK_EOF) {
		parsedFunctionBody = false;
		bool foundAnything;
		do {
			foundAnything = false;
			Lexer::Token peekToken = m_lexer.PeekToken();
			if (peekToken.id == Lexer::TOK_IDENTIFIER)
			{
				if (peekToken.strId == "class" || peekToken.strId == "struct") {
					foundAnything = true;
					Class subClass;
					if (TryParseClass(subClass))
					{
						aOutClass.classes.push_back(subClass);
					}
				}
				else if (peekToken.strId == "public" || peekToken.strId == "protected" || peekToken.strId == "private")
				{
					foundAnything = true;
					m_lexer.NextToken();
					if (m_lexer.NextToken().id != ':')
						return false; // syntax error
				}
			}
		} while (foundAnything);
		Function func;
		if (TryParseFunction(func, aOutClass.name)) {
			aOutClass.methods.push_back(func);
			if (m_lexer.currTok.id == '}')
				parsedFunctionBody = true;
		}
		if (m_lexer.currTok.id == '}' && !parsedFunctionBody)
			break;
	}
	if (m_lexer.NextToken().id != ';')
		return false; // syntax error
	return true;
}

bool Parser::TryParseFunction(Function &aOutFunc, const std::string &aClassName)
{
	auto SplitFuncName = [](const std::string& aInput) {
		std::pair<std::string, std::string> ret;
		auto separator = aInput.rfind("::");
		if (separator != std::string::npos) {
			ret.first = aInput.substr(0, separator);
			ret.second = aInput.substr(separator + 2, aInput.size());
		}
		else {
			ret.second = aInput;
		}
		return ret;
	};
	bool isFunctionDeclaration = false;
	Lexer::Token peekToken = m_lexer.PeekToken();
	auto funcName = SplitFuncName(peekToken.strId);
	// constructor
	if (peekToken.id == Lexer::TOK_IDENTIFIER) {
		if ((funcName.second == aClassName || funcName.second == SplitFuncName(funcName.first).second) && m_lexer.PeekToken(2).id == '(') {
			m_lexer.NextToken(); m_lexer.NextToken(); // skip to '('
			isFunctionDeclaration = true;
			aOutFunc.declaration.name = funcName.second;
			aOutFunc.declaration.returnType = "";
			aOutFunc.nameSpace = funcName.first;
		}
	}
	// destructor
	if (funcName.second.size() > 0 && funcName.second[0] == '~') {
		m_lexer.NextToken();
		if ((funcName.second == ('~' + SplitFuncName(funcName.first).second) || funcName.second == '~' + aClassName) && m_lexer.PeekToken().id == '(') {
			m_lexer.NextToken(); // skip to '('
			isFunctionDeclaration = true;
			aOutFunc.declaration.name = funcName.second;
			aOutFunc.declaration.returnType = "";
			aOutFunc.nameSpace = funcName.first;
		}
	}
	std::string type = "";
	if (!isFunctionDeclaration && TryParseType(type)) {
		std::string name = "";
		// this could be a method initialization (that is not a constructor/destructor).
		isFunctionDeclaration = true;
		// process all rubbish up to either ; or (. Take the last identifier as the name.
		while (m_lexer.currTok.id != Lexer::TOK_EOF) {
			if (m_lexer.currTok.id == ';') {
				// false alarm. Just a variable declaration. Disregard.
				isFunctionDeclaration = false;
				break;
			}
			else if (m_lexer.currTok.id == '(') {
				break;
			}
			else {
				name = m_lexer.currTok.strId;
			}
			m_lexer.NextToken();
		}
		if (!isFunctionDeclaration) return false;
		auto funcName = SplitFuncName(name);
		aOutFunc.declaration.name = funcName.second;
		aOutFunc.declaration.returnType = type;
		aOutFunc.nameSpace = funcName.first;
	}
	if (isFunctionDeclaration) {
		// parse arguments if present
		do {
			Variable var;
			if (!TryParseVarDeclaration(var)) {
				if (m_lexer.currTok.id == ')')
					break;
				return false;
			}
			aOutFunc.arguments.push_back(var);
			Lexer::Token peekToken = m_lexer.PeekToken();
			if (peekToken.id == ',') {
				m_lexer.NextToken();
			}
			else if (peekToken.id == ')') {
				m_lexer.NextToken();
				break;
			}
		} while (true);

		m_lexer.NextToken();
		if (m_lexer.currTok.id == Lexer::TOK_IDENTIFIER && m_lexer.currTok.strId == "const") {
			aOutFunc.isConst = true;
			m_lexer.NextToken();
		}
		bool isInterfaceMethod = false;
		if (m_lexer.currTok.id == '=') {
			// interface method, skip the = 0
			m_lexer.NextToken();
			if (m_lexer.currTok.id != Lexer::TOK_INTEGER)
				return false;
			m_lexer.NextToken();
			isInterfaceMethod = true;
			aOutFunc.hasBody = true;
		}
		do {
			if (m_lexer.currTok.id == '{') {
				aOutFunc.hasBody = true;
				// skip the body
				uint16_t nested = 0;
				do {
					m_lexer.NextToken();
					if (m_lexer.currTok.id == '{')
						nested++;
					else if (m_lexer.currTok.id == '}') {
						if (nested > 0)
							nested--;
					}
				} while (m_lexer.currTok.id != '}' && nested == 0);
				aOutFunc.endIndex = m_lexer.GetCursor();
				break;
			}
			else if (m_lexer.currTok.id == ';') {
				if (!isInterfaceMethod)
					aOutFunc.hasBody = false;
				break;
			}
			m_lexer.NextToken();
		} while (m_lexer.currTok.id != Lexer::TOK_EOF);
		return true;
	}
	return false;
}

bool Parser::TryParseType(std::string& aOutType)
{
	if (m_lexer.NextToken().id != Lexer::TOK_IDENTIFIER)
		return false;
	aOutType = m_lexer.currTok.strId;
	if (!IsTypeName(aOutType))
		return false;
	if (aOutType == "const" || aOutType == "unsigned" || aOutType == "signed") {
		std::string properType = "";
		if (!TryParseType(properType))
			return false;
		aOutType += " " + properType;
		return true;
	}
	if (m_lexer.PeekToken().id == '<')
	{
		m_lexer.NextToken();
		// we have a complicated type to deal with. (like std::map<std::string, std::vector<T>>)
		aOutType += '<';
		do {
			std::string innerType = "";
			bool gotInnerType = TryParseType(innerType);
			// a syntax error occured
			if (!gotInnerType)
				return false;
			aOutType += innerType;
			m_lexer.NextToken();
			if (m_lexer.currTok.id == ',' || m_lexer.currTok.id == '>')
				aOutType += m_lexer.currTok.id;
			else
				return false; // a syntax error ocurred
		} while (m_lexer.currTok.id == ',' && m_lexer.currTok.id != '>' && m_lexer.currTok.id != Lexer::TOK_EOF);
		if (m_lexer.currTok.id == Lexer::TOK_EOF)
			return false; // syntax error
		if (m_lexer.PeekToken().id == ':')
		{
			m_lexer.NextToken();
			if (m_lexer.PeekToken().id == ':') {
				m_lexer.NextToken();
				std::string additionalType = "";
				if (TryParseType(additionalType))
					aOutType += "::" + additionalType;
				else
					return false;
			}
			else {
				return false; // syntax error
			}
		}
	}
	Lexer::Token peekToken = m_lexer.PeekToken();
	while (peekToken.id == '&' || peekToken.id == '*') {
		m_lexer.NextToken();
		aOutType += m_lexer.currTok.id;
		peekToken = m_lexer.PeekToken();
	}
	return true;
}

bool Parser::TryParseVarDeclaration(Variable& aOutVar)
{
	if (!TryParseType(aOutVar.returnType))
		return false;
	if (m_lexer.NextToken().id != Lexer::TOK_IDENTIFIER)
		return false;
	aOutVar.name = m_lexer.currTok.strId;
	if (m_lexer.PeekToken().id == '=') {
		m_lexer.NextToken();
		m_lexer.NextToken();
	}
	return true;
}

bool Parser::IsTypeName(const std::string& aIdentifier) const
{
	// we try to guess if it's a type name by matching it against reserved keywords that
	// are not types.
	if (
		aIdentifier == "auto" ||
		aIdentifier == "break" ||
		aIdentifier == "case" ||
		aIdentifier == "catch" ||
		aIdentifier == "class" ||
		aIdentifier == "constexpr" ||
		aIdentifier == "const_cast" ||
		aIdentifier == "continue" ||
		aIdentifier == "decltype" ||
		aIdentifier == "default" ||
		aIdentifier == "delete" ||
		aIdentifier == "do" ||
		aIdentifier == "dynamic_cast" ||
		aIdentifier == "else" ||
		aIdentifier == "enum" ||
		aIdentifier == "explicit" ||
		aIdentifier == "export" ||
		aIdentifier == "extern" ||
		aIdentifier == "false" ||
		aIdentifier == "for" ||
		aIdentifier == "friend" ||
		aIdentifier == "goto" ||
		aIdentifier == "if" ||
		aIdentifier == "inline" ||
		aIdentifier == "mutable" ||
		aIdentifier == "namespace" ||
		aIdentifier == "new" ||
		aIdentifier == "nullptr" ||
		aIdentifier == "operator" ||
		aIdentifier == "private" ||
		aIdentifier == "protected" ||
		aIdentifier == "public" ||
		aIdentifier == "register" ||
		aIdentifier == "reinterpret_cast" ||
		aIdentifier == "return" ||
		aIdentifier == "sizeof" ||
		aIdentifier == "static" ||
		aIdentifier == "static_assert" ||
		aIdentifier == "static_cast" ||
		aIdentifier == "struct" ||
		aIdentifier == "switch" ||
		aIdentifier == "template" ||
		aIdentifier == "this" ||
		aIdentifier == "throw" ||
		aIdentifier == "true" ||
		aIdentifier == "try" ||
		aIdentifier == "typedef" ||
		aIdentifier == "typeid" ||
		aIdentifier == "typename" ||
		aIdentifier == "union" ||
		aIdentifier == "using" ||
		aIdentifier == "virtual" ||
		aIdentifier == "volatile" ||
		aIdentifier == "while") {
		return false;
	}
	else {
		return true;
	}
}