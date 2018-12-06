#ifndef GRAPHICS_EXCEPTION_H_
#define GRAPHICS_EXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace ice_engine
{
namespace graphics
{

struct GraphicsException : virtual Exception
{
	using Exception::Exception;
};

}
}

#endif /* GRAPHICS_EXCEPTION_H_ */
