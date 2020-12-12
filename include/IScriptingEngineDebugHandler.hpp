#ifndef ISCRIPTINGENGINEDEBUGHANDLER_HPP_
#define ISCRIPTINGENGINEDEBUGHANDLER_HPP_

#include "Types.hpp"

namespace ice_engine
{

class IScriptingEngineDebugHandler
{
public:
    virtual ~IScriptingEngineDebugHandler() = default;

    virtual void tick(const float32 delta) = 0;
};

}
#endif //ISCRIPTINGENGINEDEBUGHANDLER_HPP_
