#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_

#include "IDebugRenderer.hpp"

#include "graphics/IGraphicsEngine.hpp"

namespace ice_engine
{

class DebugRenderer : public IDebugRenderer
{
public:
	DebugRenderer(graphics::IGraphicsEngine* graphicsEngine) : graphicsEngine_(graphicsEngine)
	{
    }
	
	~DebugRenderer() override = default;
	
	void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) override
	{
		lineData_.push_back(std::tuple<glm::vec3, glm::vec3, glm::vec3>(from, to, color));
	};

	void pushLines(const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& lineData) override {};

	void render() override
	{
		graphicsEngine_->renderLines(lineData_);
		
		lineData_.clear();
	};
	
private:
	graphics::IGraphicsEngine* graphicsEngine_ = nullptr;
	std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> lineData_;

};

}

#endif /* DEBUGRENDERER_H_ */
