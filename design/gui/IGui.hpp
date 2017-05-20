
virtual void setViewport(const uint32 width, const uint32 height) = 0;
virtual void render(const float32 delta) = 0;

virtual void enabled(const bool enabled) = 0;
virtual bool enabled() const = 0;



virtual IComponent* createComponent(const std::string& name) = 0;
virtual void destroyComponent(IComponent* component) = 0;
virtual void destroyComponent(const std::string& name) = 0;
virtual IComponent* getComponent(const std::string& name) const = 0;


/* MISC STUFF */
virtual void processEvent(const Event& event) = 0;

