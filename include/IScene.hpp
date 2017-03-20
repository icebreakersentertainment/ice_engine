#ifndef ISCENE_H_
#define ISCENE_H_

namespace hercules
{

class IScene
{
public:
	virtual ~IScene()
	{
	}
	;
	
	virtual void tick(const float32 elapsedTime) = 0;
};

}

#endif /* ISCENE_H_ */
