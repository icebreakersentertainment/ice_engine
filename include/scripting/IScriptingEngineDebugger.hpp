#ifndef ISCRIPTINGENGINEDEBUGGER_HPP_
#define ISCRIPTINGENGINEDEBUGGER_HPP_

#include <string>

#include "Types.hpp"

namespace ice_engine
{
namespace scripting
{

enum DebugAction
{
    CONTINUE,
    STEP_INTO,
    STEP_OVER,
    STEP_OUT
};

enum EventType
{
    UNKNOWN = 0,

    HIT_BREAKPOINT,
    DONE
};

struct DebugEvent
{
    uint32 type;
};

class IDebugEventListener
{
public:
    virtual ~IDebugEventListener() = default;

    virtual void processEvent(const DebugEvent& event) = 0;
};

class IScriptingEngineDebugger
{
public:
    virtual ~IScriptingEngineDebugger() = default;

    virtual void addBreakPoint(const std::string& file, const uint32 line) = 0;

    virtual void performAction(const DebugAction action) = 0;

    /**
     * Returns the name of the file that the debugger is currently in.
     *
     * If we aren't currently debugging anything, returns an empty string.
     *
     * @return
     */
    virtual std::string filename() const = 0;

    /**
     * Returns the name of the function that the debugger is currently in.
     *
     * If we aren't currently debugging anything, returns an empty string.
     *
     * @return
     */
    virtual std::string functionName() const = 0;

    /**
     * Returns the line in the file that the debugger is currently on.
     *
     * If we aren't currently debugging anything, returns -1.
     *
     * @return
     */
    virtual int32 line() const = 0;

    virtual std::string stacktrace() = 0;
    virtual std::string variables() = 0;

    virtual void registerToStringCallback(const std::string& obj, const std::function<std::string(void*)>& function) = 0;

    virtual void addDebugEventListener(IDebugEventListener* listener) = 0;
    virtual void removeDebugEventListener(const IDebugEventListener* listener) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(const bool enabled) = 0;

    virtual bool running() const = 0;
    virtual void setRunning(const bool running) = 0;
};

}
}

#endif //ISCRIPTINGENGINEDEBUGGER_HPP_
