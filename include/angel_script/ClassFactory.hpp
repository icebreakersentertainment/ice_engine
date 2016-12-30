#ifndef CLASSFACTORY_H_
#define CLASSFACTORY_H_

#include "angel_script/TestAtom.hpp"

namespace angel_script
{

class ClassFactory
{
public:

	static TestAtom* atomRefFactory();

private:
	ClassFactory();
	ClassFactory(const ClassFactory& other);
	virtual ~ClassFactory();
};

}

#endif /* CLASSFACTORY_H_ */
