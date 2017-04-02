#ifndef SCRIPT_PARAMETER_LIST_H_
#define SCRIPT_PARAMETER_LIST_H_

#include <vector>

#include "Types.hpp"
#include "scripting/Parameter.hpp"

namespace hercules
{
namespace scripting
{

class ParameterList
{

public:
	ParameterList() {};
	virtual ~ParameterList() {};
	
	Parameter& operator[](size_t index)
	{
		return parameters_[index];
	};
	
	const Parameter& operator[](size_t index) const
	{
		return parameters_[index];
	};
	
	size_t size() const
	{
		return parameters_.size();
	};
	
	std::vector<Parameter>::iterator begin() { return parameters_.begin(); }
    std::vector<Parameter>::const_iterator begin() const { return parameters_.begin(); }
    std::vector<Parameter>::const_iterator cbegin() const { return parameters_.cbegin(); }
    std::vector<Parameter>::iterator end() { return parameters_.end(); }
    std::vector<Parameter>::const_iterator end() const { return parameters_.end(); }
    std::vector<Parameter>::const_iterator cend() const { return parameters_.cend(); }
	
	template <typename T>
	void addRef(T& value)
	{
		Parameter p = Parameter();
		p.valueRef(value);
		
		parameters_.push_back(p);
	};
	
	template <typename T>
	void add(T value)
	{
		Parameter p = Parameter();
		p.value(value);
		
		parameters_.push_back(p);
	};
	
	void add(Parameter p)
	{
		parameters_.push_back(p);
	};
	
	std::vector<Parameter>& list()
	{
		return parameters_;
	};
	
private:
	std::vector<Parameter> parameters_;
	
};

}
}

#endif /* SCRIPT_PARAMETER_LIST_H_ */
