#include "Generator.h"
#include "Parser.h"
#include <algorithm>

Generator::Generator()
{

}

Generator::~Generator()
{

}

void Generator::Generate(const std::string& aInPath, const std::string& aOutPath)
{
	std::ifstream infile(aInPath, std::ios::binary);
	std::string content;
	infile.seekg(0, std::ios::end);
	content.resize(infile.tellg());
	infile.seekg(0, std::ios::beg);
	infile.read(&content[0], content.size());
	infile.close();
	
	size_t pos = 0;
	std::string search, replace;
	search = "\r\n";
	replace = "\n";
	while ((pos = content.find(search, pos)) != std::string::npos) {
		content.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	
	std::string cppContent("");
	std::ifstream cppfile(aOutPath, std::ios::binary);
	if (cppfile.is_open() && cppfile.good()) {
		cppfile.seekg(0, std::ios::end);
		cppContent.resize(cppfile.tellg());
		cppfile.seekg(0, std::ios::beg);
		cppfile.read(&cppContent[0], cppContent.size());
	}
	cppfile.close();

	// for now just copy the content.
	GenerateCPP(content, cppContent);
	std::ofstream outfile(aOutPath, std::ios::binary);
	outfile.write((const char*)&cppContent[0], cppContent.size());
	outfile.close();
}

void Generator::GenerateCPP(const std::string& aHeaderSource, std::string& aSource)
{
	Parser parser(aHeaderSource);
	std::vector<Class> classes;
	parser.ParseHeader(classes);

	Parser srcParser(aSource);
	m_declaredFunctions.clear();
	m_usedNamespaces.clear();
	m_usedNamespaces.push_back("");
	srcParser.ParseSource(m_declaredFunctions, m_usedNamespaces);

	for (uint16_t i = 0; i < classes.size(); i++) {
		GenerateClass(classes[i], "", aSource);
	}
}

void Generator::GenerateClass(const Class& aClass, const std::string& aNameSpace, std::string& aCPPsrc)
{
	std::string ns("");
	if (aClass.nameSpace.empty())
		ns = aNameSpace + aClass.name + "::";
	else
		ns = aNameSpace + aClass.nameSpace + "::" + aClass.name + "::";
	for (uint16_t i = 0; i < aClass.methods.size(); i++) {
		const Function& generatedMethod = aClass.methods[i];
		if (!generatedMethod.hasBody) {
			// check if this method already exists in source file
			bool functionExists = false;
			for (uint16_t i = 0; i < m_declaredFunctions.size(); i++) {
				const Function& cppMethod = m_declaredFunctions[i];
				if (DoesDeclarationMatch(cppMethod, generatedMethod, ns)) {
					functionExists = true;
					break;
				}
			}
			if (!functionExists) {
				uint32_t insertIndex = aCPPsrc.length(), declaredMethodIndex = 0;
				std::string methodSrc("");
				// check if .cpp file uses our namespace. If it does, don't use namespace's prefix in the
				// generated function
				for (uint16_t i = 0; i < m_usedNamespaces.size(); i++) {
					std::string tempNamespace = aNameSpace + aClass.nameSpace;
					if (!m_usedNamespaces[i].empty() && tempNamespace.find(m_usedNamespaces[i]) == 0) {
						std::string newNS = aClass.nameSpace;
						newNS.erase(0, m_usedNamespaces[i].length());
						if (newNS.find("::") == 0)
							newNS.erase(0, 2);
						if (!newNS.empty() && newNS.rfind("::") != newNS.length() - 2)
							newNS += "::";
						methodSrc = "\n\n" + GenerateMethod(generatedMethod, newNS + aClass.name + "::");
						break;
					}
					else if (m_usedNamespaces[i] == aClass.nameSpace) {
						methodSrc = "\n\n" + GenerateMethod(generatedMethod, aClass.name + "::");
					}
				}
				if (methodSrc.empty())
					methodSrc = "\n\n" + GenerateMethod(generatedMethod, ns);
				if (i > 0) {
					// find the string index at which previous method ends in source file
					const Function& previousMethod = aClass.methods[i - 1];
					for (uint16_t j = 0; j < m_declaredFunctions.size(); j++) {
						const Function& cppMethod = m_declaredFunctions[j];
						if (DoesDeclarationMatch(cppMethod, previousMethod, ns)) {
							insertIndex = cppMethod.endIndex;
							declaredMethodIndex = j;
							break;
						}
					}
					// update endIndexes
					for (uint16_t j = declaredMethodIndex + 1; j < m_declaredFunctions.size(); j++) {
						m_declaredFunctions[j].endIndex += methodSrc.size();
					}
				}
				aCPPsrc.insert(insertIndex, methodSrc);
			}
		}
	}
	for (uint16_t i = 0; i < aClass.classes.size(); i++) {
		GenerateClass(aClass.classes[i], ns, aCPPsrc);
	}
}

std::string Generator::GenerateMethod(const Function& aMethod, const std::string& aNameSpace) const
{
	std::string out;
	if (!aMethod.declaration.returnType.empty())
		out = aMethod.declaration.returnType + " ";
	out += aNameSpace + aMethod.declaration.name + "(";
	size_t argSize = aMethod.arguments.size();
	for (uint8_t i = 0; i < argSize; i++) {
		out += aMethod.arguments[i].returnType + " " + aMethod.arguments[i].name;
		if (i < argSize - 1)
			out += ", ";
	}
	out += ")";
	if (aMethod.isConst)
		out += " const";
	out += "\n{\n}";
	return out;
}

bool Generator::DoesDeclarationMatch(const Function& aCPP, const Function& aHeader, const std::string& aNameSpace) const
{
	for (uint16_t i = 0; i < m_usedNamespaces.size(); i++) {
		std::string cppDeclarationName = aCPP.nameSpace + "::" + aCPP.declaration.name;
		if (!m_usedNamespaces[i].empty())
			cppDeclarationName = m_usedNamespaces[i] + "::" + cppDeclarationName;
		std::string headerDeclarationName = aNameSpace + aHeader.declaration.name;
		if (cppDeclarationName == headerDeclarationName && aCPP.declaration.returnType == aHeader.declaration.returnType)
		{
			return true;
		}
	}
	return false;
}