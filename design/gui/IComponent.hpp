virtual void render(const float32 delta) = 0;

virtual glm::ivec2 dimensions() const = 0;
virtual void dimensions(const glm::ivec2& dimensions) = 0;
virtual void dimensions(const int32 width, const int32 height) = 0;
virtual glm::ivec2 position() const = 0;
virtual void position(const int32 x, const int32 y) = 0;
virtual void position(const glm::ivec2& position) = 0;



virtual IComponent* getParent() const = 0;
virtual void addComponent(IComponent* component) = 0;
virtual void removeComponent(IComponent* component) = 0;


/* MISC STUFF */
virtual void processEvent(const Event& event) = 0;

