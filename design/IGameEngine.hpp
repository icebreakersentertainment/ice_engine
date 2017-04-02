class ModelHandle
{
};
class ModelInstanceHandle
{
};
class AnimationList
{
};
class SkeletonInstanceHandle
{
};


virtual Model importModel(const std::string& filename) const = 0;

virtual ModelHandle loadModel(const std::string& filename, const std::string& name) = 0;
virtual ModelHandle loadModel(const Model& model, const std::string& name) = 0;
virtual void unloadModel(const std::string& name) = 0;
virtual void unloadModel(const ModelHandle& modelHandle) = 0;
virtual void unloadAllModels() = 0;
virtual ModelHandle getModel(const std::string& name) const = 0;

virtual ModelInstanceHandle createModelInstance(const std::string& modelName, const std::string& name = std::string()) = 0;
virtual ModelInstanceHandle createModelInstance(const ModelHandle& modelHandle, const std::string& name = std::string()) = 0;
virtual void destroyModelInstance(const std::string& name) = 0;
virtual void destroyModelInstance(const ModelHandle& modelHandle) = 0;
virtual void destroyAllModelInstances() = 0;
virtual ModelInstanceHandle getModelInstance(const std::string& name) const = 0;

virtual AnimationList getAnimations(const std::string& modelName) const = 0;
virtual AnimationList getAnimations(const ModelHandle& modelHandle) const = 0;
virtual AnimationHandle getAnimation(const std::string& modelName, const std::string& animationName) const = 0;
virtual AnimationHandle getAnimation(const ModelHandle& modelHandle, const std::string& animationName) const = 0;

virtual SkeletonInstanceHandle createSkeleton(const std::string& modelName) const = 0;
virtual SkeletonInstanceHandle createSkeleton(const ModelHandle& modelHandle) const = 0;
virtual Skeleton createSkeleton(const std::string& modelName) const = 0;
virtual Skeleton createSkeleton(const ModelHandle& modelHandle) const = 0;

virtual void assign(const ModelInstanceHandle& modelInstancehandle, const AnimationHandle& animationHandle) = 0;
virtual void assign(const ModelInstanceHandle& modelInstancehandle, const SkeletonInstanceHandle& skeletonInstanceHandle) = 0;
