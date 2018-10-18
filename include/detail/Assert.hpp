#ifndef ICEENGINEASSERT_H_
#define ICEENGINEASSERT_H_

#include <iostream>

namespace ice_engine
{
namespace detail
{

#define ASSERT(condition) if (!(condition)) \
{ \
	std::cerr << __FILE__ << " (" << __LINE__ << "): Assert " << #condition << " failed" << std::endl; \
	std::abort(); \
	\
}

}
}

#endif /* ICEENGINEASSERT_H_ */
