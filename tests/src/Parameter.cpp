#define BOOST_TEST_MODULE Parameter
#include <boost/test/unit_test.hpp>

#include "scripting/Parameter.hpp"

struct Fixture
{
	Fixture()
	{
		parameter = ice_engine::scripting::Parameter();
	}
	
	ice_engine::scripting::Parameter parameter;
};

class Object
{
public:
	Object()
	{
		a = 0;
		b = 1;
		c = 2;
		d = 3.0f;
	};
	
	int a;
	int b;
	int c;
	float d;
};

BOOST_FIXTURE_TEST_SUITE(Parameter, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(getValue)
{
	parameter.value(1);
	
	auto v = parameter.value<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(getValueRef)
{
	int i = 1;
	parameter.valueRef(i);
	
	auto v = parameter.valueRef<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(setValue)
{
	parameter.value(1);
	
	auto v = parameter.value<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(setValueRef)
{
	int i = 1;
	parameter.valueRef(i);
	
	auto v = parameter.valueRef<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(getValueObject)
{
	parameter.value(Object());
	
	auto ref = parameter.value<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_CASE(getValueRefObject)
{
	parameter.value(Object());
	
	auto ref = parameter.valueRef<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_CASE(setValueRefObject)
{
	auto o = Object();
	parameter.valueRef(o);
	
	auto ref = parameter.valueRef<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_CASE(setValueObject)
{
	auto o = Object();
	parameter.value(o);
	
	auto ref = parameter.valueRef<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_CASE(copyConstructorWithValue)
{
	parameter.value(1);
	
	{
		auto p2 = ice_engine::scripting::Parameter(parameter);
	}
	
	auto v = parameter.value<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(copyConstructorWithRef)
{
	int i = 1;
	parameter.valueRef(i);
	
	{
		auto p2 = ice_engine::scripting::Parameter(parameter);
	}
	
	auto v = parameter.valueRef<int>();
	BOOST_CHECK_EQUAL(v, 1);
}

BOOST_AUTO_TEST_CASE(copyConstructorWithValueObject)
{
	parameter.value(Object());
	
	{
		auto p2 = ice_engine::scripting::Parameter(parameter);
	}
	
	auto ref = parameter.valueRef<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_CASE(copyConstructorWithRefObject)
{
	auto o = Object();
	parameter.valueRef(o);
	
	{
		auto p2 = ice_engine::scripting::Parameter(parameter);
	}
	
	auto ref = parameter.valueRef<Object>();
	BOOST_CHECK_EQUAL(ref.a, 0);
	BOOST_CHECK_EQUAL(ref.b, 1);
	BOOST_CHECK_EQUAL(ref.c, 2);
	BOOST_CHECK_EQUAL(ref.d, 3.0f);
}

BOOST_AUTO_TEST_SUITE_END()
