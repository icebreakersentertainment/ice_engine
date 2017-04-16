class ModelHandle
{
};

virtual void setIGameInstance(const scripting::ScriptObjectHandle& scriptObjectHandle) = 0;
virtual void setIGameInstance(asIScriptObject* obj) = 0; // eww
virtual void setIGameInstance(const IGame* game) = 0;
virtual void setBootstrapScript(const std::string& filename) = 0;

virtual void run();



/* RESOURCE STUFF */
virtual AudioSample* loadAudioSample(const std::string& filename, const std::string& name) = 0;
virtual Image* loadImage(const std::string& filename, const std::string& name) = 0;
virtual Model* loadModel(const std::string& filename, const std::string& name) = 0;
virtual Model* importModel(const std::string& filename, const std::string& name) = 0;

virtual void unloadAudioSample(const std::string& name) = 0;
virtual void unloadImage(const std::string& name) = 0;
virtual void unloadModel(const std::string& name) = 0;

virtual AudioSample* getAudioSample(const std::string& name) const = 0;
virtual Image* getImage(const std::string& name) const = 0;
virtual Model* getModel(const std::string& name) const = 0;



/* MODEL STUFF */
virtual ModelHandle loadStaticModel(const graphics::model::Model& model) = 0;



/* SCENE STUFF */
virtual IScene* createScene(const std::string& name) = 0;
virtual void destroyScene(const std::string& name) = 0;
virtual void destroyScene(const IScene* scene) = 0;
virtual IScene* getScene(const std::string& name) const = 0;



/* Implements the IEventListener interface */
virtual bool processEvent(const graphics::Event& event) = 0;
