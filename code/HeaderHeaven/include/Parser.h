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

#ifndef HHEAVEN_PARSER_H
#define HHEAVEN_PARSER_H

#include "Common.h"
#include "Lexer.h"
#include "LanguageComponents.h"

class Parser
{
public:
	Parser(const std::string& aSource);
	~Parser();

	void ParseHeader(std::vector<Class>& aOutClasses);
	void ParseSource(std::vector<Function>& aOutFunctions);
	bool TryParseNamespace(std::vector<Class> &aOutClasses);
	bool TryParseClass(Class &aOutClass);
	bool TryParseType(std::string& aOutType);
	bool TryParseFunction(Function &aOutFunc, const std::string &aClassName);
	bool TryParseVarDeclaration(Variable& aOutVar);
	bool IsTypeName(const std::string& aIdentifier) const;

private:
	Lexer m_lexer;
};

#endif