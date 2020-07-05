#ifndef ITREEVIEW_HPP_
#define ITREEVIEW_HPP_

#include <functional>
#include <string>

#include "IComponent.hpp"
#include "ITreeNode.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class ITreeView : public virtual IComponent
{
public:
    virtual ~ITreeView()
    {
    }
    ;

    virtual ITreeNode* createNode(const std::string& label) = 0;
    virtual ITreeNode* getNode(const std::string& label) = 0;
    virtual void destroy(const ITreeNode* treeNode) = 0;

    virtual void select(ITreeNode* treeNode) = 0;
    virtual void unselect(const ITreeNode* treeNode) = 0;
    virtual bool selected(const ITreeNode* treeNode) const = 0;

    virtual void setCallback(std::function<void(ITreeNode*)>& callback) = 0;
};

}
}
}


#endif //ITREEVIEW_HPP_
