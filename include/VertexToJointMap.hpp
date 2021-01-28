#ifndef VERTEXTOJOINTMAP_HPP_
#define VERTEXTOJOINTMAP_HPP_

class VertexToJointMap// : public graphics::ISkeleton
{
public:

    VertexToJointMap() = default;

    VertexToJointMap(std::string name, std::vector<glm::ivec4> jointIndices, std::vector<glm::vec4> weights)
            :
            name_(std::move(name)),
            jointIndices_(std::move(jointIndices)),
            weights_(std::move(weights))
    {
    }

    VertexToJointMap(const VertexToJointMap&) = default;
    VertexToJointMap(VertexToJointMap&&) = default;

    ~VertexToJointMap() override = default;

    VertexToJointMap& operator=(const VertexToJointMap&) = default;
    VertexToJointMap& operator=(VertexToJointMap&&) = default;

    const std::string& name() const// override
    {
        return name_;
    }

    const std::vector<glm::ivec4>& jointIndices() const// override
    {
        return jointIndices_;
    }

    const std::vector<glm::vec4>& weights() const// override
    {
        return weights_;
    }

private:
    std::string name_;
    std::vector<glm::ivec4> jointIndices_;
    std::vector<glm::vec4> weights_;
};

#endif //VERTEXTOJOINTMAP_HPP_
