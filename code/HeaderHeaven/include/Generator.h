#ifndef HHEAVEN_GENERATOR_H
#define HHEAVEN_GENERATOR_H

#include "Common.h"
#include "Lexer.h"
#include "LanguageComponents.h"

class Generator
{
public:
	Generator();
	~Generator();

	void Generate(const std::string& aInPath, const std::string& aOutPath);

private:
	std::string GenerateCPP(const std::string& aHeaderSource);
	std::string GenerateClass(const Class& aClass, const std::string& aNameSpace) const;
	std::string GenerateMethod(const Function& aMethod, const std::string& aNameSpace) const;
	bool TryParseClass(Class &aOutClass);
	bool TryParseType(std::string& aOutType);
	bool TryParseFunction(Function &aOutFunc, const std::string &aClassName);
	bool TryParseVarDeclaration(Variable& aOutVar);
	bool IsTypeName(const std::string& aIdentifier) const;

	Lexer m_lexer;
};

#endif