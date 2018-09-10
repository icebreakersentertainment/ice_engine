#ifndef ISCRIPTINGENGINEBINDING_H_
#define ISCRIPTINGENGINEBINDING_H_

namespace ice_engine
{

class IScriptingEngineBinding
{
public:
		virtual ~IScriptingEngineBinding()
		{
		}
		;

		virtual void bind() = 0;
};

}

#endif /* ISCRIPTINGENGINEBINDING_H_ */
