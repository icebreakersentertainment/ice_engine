#ifndef IGENERICCOMPONENTCONTAINER_H_
#define IGENERICCOMPONENTCONTAINER_H_

#include <string>
#include <memory>

#include "IComponent.hpp"
#include "ILabel.hpp"
#include "IButton.hpp"
#include "ITextField.hpp"
#include "ITextArea.hpp"
#include "IComboBox.hpp"
#include "IMenuBar.hpp"
#include "IRectangle.hpp"
#include "ITreeView.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IGenericComponentContainer
{
public:
	virtual ~IGenericComponentContainer() = default;

	virtual ILabel* createLabel(const std::string label = std::string()) = 0;
	virtual ILabel* createLabel(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string label = std::string()) = 0;
	virtual IButton* createButton(const std::string label = std::string()) = 0;
	virtual IButton* createButton(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string label = std::string()) = 0;
	virtual ITextField* createTextField(const std::string text = std::string()) = 0;
	virtual ITextField* createTextField(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string text = std::string()) = 0;
	virtual ITextArea* createTextArea(const std::string text = std::string()) = 0;
	virtual ITextArea* createTextArea(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string text = std::string()) = 0;
	virtual ITreeView* createTreeView() = 0;
    virtual IComboBox* createComboBox() = 0;

    virtual void add(std::unique_ptr<IComponent> component) = 0;

	virtual void destroy(const ILabel* label) = 0;
	virtual void destroy(const IButton* button) = 0;
	virtual void destroy(const ITextField* textField) = 0;
	virtual void destroy(const ITextArea* textArea) = 0;
	virtual void destroy(const ITreeView* treeView) = 0;
    virtual void destroy(const IComboBox* comboBox) = 0;

    virtual std::unique_ptr<IComponent> remove(const IComponent* component) = 0;
};

}
}
}

#endif /* IGENERICCOMPONENTCONTAINER_H_ */
