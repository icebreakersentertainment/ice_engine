#ifndef ITREENODE_HPP_
#define ITREENODE_HPP_

#include <string>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class ITreeNode : public virtual IComponent
{
public:
    virtual ~ITreeNode() = default;

    virtual ITreeNode* createNode(const std::string& label) = 0;
    virtual ITreeNode* getNode(const std::string& label) = 0;
    virtual void destroy(const ITreeNode* treeNode) = 0;

    virtual void setLabel(const std::string& label) = 0;
    virtual const std::string& label() const = 0;
};

}
}
}

#endif //ITREENODE_HPP_
