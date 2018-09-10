#ifndef TEXTOUTARCHIVE_H_
#define TEXTOUTARCHIVE_H_

#include <string>
#include <ostream>

#include <boost/archive/text_oarchive.hpp>

namespace ice_engine
{
namespace serialization
{

/**
 * 
 */
class TextOutArchive : public boost::archive::text_oarchive
{
public:
	TextOutArchive(std::ostream& ostream) : boost::archive::text_oarchive(ostream)
	{

	}

	virtual ~TextOutArchive() = default;
};

}
}

#endif /* TEXTOUTARCHIVE_H_ */
