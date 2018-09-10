#ifndef TEXTINARCHIVE_H_
#define TEXTINARCHIVE_H_

#include <string>
#include <istream>

#include <boost/archive/text_iarchive.hpp>

namespace ice_engine
{
namespace serialization
{

/**
 * 
 */
class TextInArchive : public boost::archive::text_iarchive
{
public:
	TextInArchive(std::istream& istream) : boost::archive::text_iarchive(istream)
	{
	}

	virtual ~TextInArchive() = default;
};

}
}

#endif /* TEXTINARCHIVE_H_ */
