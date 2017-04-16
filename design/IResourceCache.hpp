/* All functions here are thread safe */

virtual void addAudioSample(std::unique_ptr<AudioSample> audioSample, const std::string& name) = 0;
virtual void addImage(std::unique_ptr<Image> image, const std::string& name) = 0;
virtual void addModel(std::unique_ptr<Model> model, const std::string& name) = 0;

virtual void removeAudioSample(const std::string& name) = 0;
virtual void removeImage(const std::string& name) = 0;
virtual void removeModel(const std::string& name) = 0;

virtual AudioSample* getAudioSample(const std::string& name) const = 0;
virtual Image* getImage(const std::string& name) const = 0;
virtual Model* getModel(const std::string& name) const = 0;
