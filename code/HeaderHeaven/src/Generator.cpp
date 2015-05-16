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
	std::vector<Function> declaredFunctions;
	srcParser.ParseSource(declaredFunctions);

	for (uint16_t i = 0; i < classes.size(); i++) {
		GenerateClass(classes[i], "", aSource, declaredFunctions);
	}
}

void Generator::GenerateClass(const Class& aClass, const std::string& aNameSpace, std::string& aCPPsrc, std::vector<Function>& aDeclaredFuncs) const
{
	std::string ns("");
	auto DoesDeclarationMatch = [&](const Function& aLHS, const Function& aRHS) {
		return (aLHS.nameSpace + "::" + aLHS.declaration.name) == ns + aRHS.declaration.name
			&& aLHS.declaration.returnType == aRHS.declaration.returnType;
	};
	if (aClass.nameSpace.empty())
		ns = aNameSpace + aClass.name + "::";
	else
		ns = aNameSpace + aClass.nameSpace + "::" + aClass.name + "::";
	for (uint16_t i = 0; i < aClass.methods.size(); i++) {
		const Function& generatedMethod = aClass.methods[i];
		if (!generatedMethod.hasBody) {
			// check if this method already exists in source file
			bool functionExists = false;
			for (uint16_t i = 0; i < aDeclaredFuncs.size(); i++) {
				const Function& cppMethod = aDeclaredFuncs[i];
				if (DoesDeclarationMatch(cppMethod, generatedMethod)) {
					functionExists = true;
					break;
				}
			}
			if (!functionExists) {
				uint32_t insertIndex = aCPPsrc.length(), declaredMethodIndex = 0;
				std::string methodSrc = "\n\n" + GenerateMethod(generatedMethod, ns);
				if (i > 0) {
					const Function& previousMethod = aClass.methods[i - 1];
					for (uint16_t j = 0; j < aDeclaredFuncs.size(); j++) {
						const Function& cppMethod = aDeclaredFuncs[j];
						if (DoesDeclarationMatch(cppMethod, previousMethod)) {
							insertIndex = cppMethod.endIndex;
							declaredMethodIndex = j;
							break;
						}
					}
					// update endIndexes
					for (uint16_t j = declaredMethodIndex+1; j < aDeclaredFuncs.size(); j++) {
						aDeclaredFuncs[j].endIndex += methodSrc.size();
					}
				}
				aCPPsrc.insert(insertIndex, methodSrc);
			}
		}
	}
	for (uint16_t i = 0; i < aClass.classes.size(); i++) {
		GenerateClass(aClass.classes[i], ns, aCPPsrc, aDeclaredFuncs);
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