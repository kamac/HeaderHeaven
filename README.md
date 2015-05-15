Header Heaven
==============

A tool for generating C++ source files from C++ headers (for now methods only). HeaderHeaven uses it's own **simple** C++ header parser, so expect it to break every now and then. (If you mess around with stuff like volatile, mutable etc. it probably won't handle it too well)

So, when HeaderHeaven parses such C++ header in MyProject/include/TestClass.h:
```c++
#ifndef HEADER_GUARD_H
#define HEADER_GUARD_H

#include "Common.h"

class TestClass
{
public:
	TestClass();
	~TestClass();

	void Method(const std::string& aArgument) const;
}
```
It would produce following source file in MyProject/src/TestClass.cpp:
```c++
TestClass::TestClass()
{
}

TestClass::~TestClass()
{
}

void TestClass::Method(const std::string& aArgument) const
{
}
```

HeaderHeaven tries it's best to be smart and not generate functions that are supposed to be an interface, or functions whose body is already defined in the header.

How to build
--------------

To build HeaderHeaven you need to grab **premake5** executable from [here](https://premake.github.io/download.html), and drop it in build/ folder.
Then simply double-click vs2013.bat, which will generate visual studio project files. If you're not on windows, you'll have to figure out how to invoke
premake yourself (sorry!) - use vs2013.bat as a reference.

How to use
--------------

Once you build HeaderHeaven, create a config.txt file next to the executable. Then edit it to match this format:

	headersPath sourcesPath
	headersPath sourcesPath
	...

The paths are relative, so for example:

	MyProject/include/ MyProject/src/

Would be a correct configuration, as long as MyProject folder is right next to HeaderHeaven executable.

License
--------------

HeaderHeaven uses the GNU General Public License v2, which describes the rights
to distribute or change the code. 

Please read LICENSE.md file for the full license.

© 2015, Maciej Kozik