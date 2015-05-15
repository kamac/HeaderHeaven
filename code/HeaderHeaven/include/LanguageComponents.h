#ifndef HHEAVEN_LANGUAGE_COMPONENTS_H
#define HHEAVEN_LANGUAGE_COMPONENTS_H

#include "Common.h"

struct Variable {
	std::string returnType, name;
};
struct Function {
	Variable declaration;
	std::vector<Variable> arguments;
	bool hasBody, isConst = false;
};
struct Class {
	std::string name;
	std::vector<Function> methods;
	std::vector<Class> classes;
};

#endif