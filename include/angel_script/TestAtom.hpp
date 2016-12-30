#ifndef TESTATOM_H_
#define TESTATOM_H_

#include "as_wrapper/ASRefObject.h"

namespace angel_script
{

class TestAtom : public as_wrapper::ASRefObject
{
public:
	TestAtom();
	virtual ~TestAtom();

	void test();
};

}

#endif /* TESTATOM_H_ */
