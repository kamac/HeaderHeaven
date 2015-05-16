/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
	void GenerateCPP(const std::string& aHeaderSource, std::string& aSource);
	void GenerateClass(const Class& aClass, const std::string& aNameSpace, std::string& aCPPsrc, std::vector<Function>& aDeclaredFuncs) const;
	std::string GenerateMethod(const Function& aMethod, const std::string& aNameSpace) const;
	bool TryParseNamespace(std::vector<Class> &aOutClasses);
	bool TryParseClass(Class &aOutClass);
	bool TryParseType(std::string& aOutType);
	bool TryParseFunction(Function &aOutFunc, const std::string &aClassName);
	bool TryParseVarDeclaration(Variable& aOutVar);
	bool IsTypeName(const std::string& aIdentifier) const;

	Lexer m_lexer;
};

#endif