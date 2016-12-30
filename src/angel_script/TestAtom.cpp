#include <iostream>

#include "angel_script/TestAtom.hpp"

namespace angel_script
{

TestAtom::TestAtom() : ASRefObject()
{
}

TestAtom::~TestAtom()
{
}

void TestAtom::test()
{
	std::cout << "TESTING 123" << std::endl;
}

}

