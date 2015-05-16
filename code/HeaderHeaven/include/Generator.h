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
	void GenerateClass(const Class& aClass, const std::string& aNameSpace, std::string& aCPPsrc);
	std::string GenerateMethod(const Function& aMethod, const std::string& aNameSpace) const;
	bool DoesDeclarationMatch(const Function& aCPP, const Function& aHeader, const std::string& aNameSpace) const;

	std::vector<Function> m_declaredFunctions;
	std::vector<std::string> m_usedNamespaces;
};

#endif