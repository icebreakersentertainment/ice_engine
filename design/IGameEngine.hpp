class ModelHandle
{
};

virtual void setIGameInstance(const scripting::ScriptObjectHandle& scriptObjectHandle) = 0;
virtual void setIGameInstance(asIScriptObject* obj) = 0; // eww
virtual void setIGameInstance(const IGame* game) = 0;
virtual void setBootstrapScript(const std::string& filename) = 0;

virtual graphics::IGraphicsEngine* getGraphicsEngine() const = 0;
virtual physics::IPhysicsEngine* getPhysicsEngine() const = 0;

virtual void run() = 0;



/* CAMERA STUFF */
virtual CameraHandle createCamera(const glm::vec3& position = glm::vec3(), const glm::vec3& lookAt = glm::vec3(vec3)) = 0;
virtual CameraHandle createCamera(const glm::vec3& position, const glm::quat& orientation) = 0;
virtual CameraHandle getCamera() const = 0;
virtual void destroyCamera(const CameraHandle& cameraHandle) = 0;

virtual void rotate(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const CameraHandle& cameraHandle, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
virtual void rotation(const CameraHandle& cameraHandle, const float32 degrees, const glm::vec3& axis) = 0;
virtual void rotation(const CameraHandle& cameraHandle, const glm::quat& orientation) = 0;
virtual glm::quat rotation(const CameraHandle& cameraHandle) const = 0;
virtual void translate(const CameraHandle& cameraHandle, const glm::vec3& translate) = 0;
virtual void lookAt(const CameraHandle& cameraHandle, const glm::vec3& lookAt) = 0;

virtual void position(const CameraHandle& cameraHandle, const glm::vec3& position) = 0;
virtual void position(const CameraHandle& cameraHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual glm::vec3 position(const CameraHandle& cameraHandle) const = 0;



/* RESOURCE STUFF */
virtual AudioSample* loadAudioSample(const std::string& name, const std::string& filename) = 0;
virtual utilities::Image* loadImage(const std::string& name, const std::string& filename) = 0;
virtual graphics::model::Model* loadModel(const std::string& name, const std::string& filename) = 0;
virtual graphics::model::Model* importModel(const std::string& name, const std::string& filename) = 0;

virtual void unloadAudioSample(const std::string& name) = 0;
virtual void unloadImage(const std::string& name) = 0;
virtual void unloadModel(const std::string& name) = 0;

virtual AudioSample* getAudioSample(const std::string& name) const = 0;
virtual utilities::Image* getImage(const std::string& name) const = 0;
virtual graphics::model::Model* getModel(const std::string& name) const = 0;



/* MODEL STUFF */
virtual ModelHandle loadStaticModel(const graphics::model::Model& model) = 0;



/* SHADER STUFF */
virtual graphics::ShaderHandle createVertexShader(const std::string& name, const std::string& filename) = 0;
virtual graphics::ShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data) = 0;
virtual graphics::ShaderHandle createFragmentShader(const std::string& name, const std::string& filename) = 0;
virtual graphics::ShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data) = 0;
virtual graphics::ShaderHandle getShader(const std::string& name) const = 0;
virtual void destroyShader(const std::string& name) = 0;
virtual void destroyShader(const graphics::ShaderHandle& shaderHandle) = 0;

virtual graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle) = 0;
virtual graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const = 0;
virtual void destroyShaderProgram(const std::string& name) = 0;
virtual void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle) = 0;



/* SCENE STUFF */
virtual IScene* createScene(const std::string& name) = 0;
virtual void destroyScene(const std::string& name) = 0;
virtual void destroyScene(const IScene* scene) = 0;
virtual IScene* getScene(const std::string& name) const = 0;



/* EVENT LISTENERS */
virtual bool addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) = 0;
virtual bool addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) = 0;
virtual bool addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) = 0;
virtual bool addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) = 0;



/* Implements the IEventListener interface */
virtual bool processEvent(const graphics::Event& event) = 0;
