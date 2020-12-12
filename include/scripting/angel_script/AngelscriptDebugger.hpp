#ifndef ANGELSCRIPTDEBUGGER_HPP_
#define ANGELSCRIPTDEBUGGER_HPP_

#include <mutex>
#include <sstream>
#include <unordered_map>

#include "scripting/IScriptingEngineDebugger.hpp"

#include "scripting/angel_script/AngelscriptUtilities.hpp"

#include "scripting/angel_script/debugger/debugger.h"

#include "logger/ILogger.hpp"

#include "exceptions/InvalidArgumentException.hpp"

namespace ice_engine
{
namespace scripting
{
namespace angel_script
{

class AngelscriptDebugger : public CDebugger, public IScriptingEngineDebugger
{
public:
    AngelscriptDebugger(logger::ILogger* logger) : logger_(logger)
    {

    }

    void prepare(asIScriptContext* context)
    {
        // TODO: Make this more efficient
        if (enabled())
        {
            int32 r = context->SetLineCallback(asMETHOD(AngelscriptDebugger, lineCallback), this, asCALL_THISCALL);
            assertNoAngelscriptError(r);
        } else
        {
            context->ClearLineCallback();
        }


//        const auto testFilename = "C:\\Users\\Jarrett\\projects\\icebreakersentertainment\\dark_horizon\\build\\data\\scripts\\controllers\\AbstractController.as";
//        AddFileBreakPoint(testFilename, 71);
    }

    void addBreakPoint(const std::string& file, const uint32 line) override
    {
        AddFileBreakPoint(file, line);
    }

    void performAction(const scripting::DebugAction action) override
    {
        std::cout << "performAction 1 context_: " << context_ << " action: " << action << std::endl;

        switch (action)
        {
            case DebugAction::CONTINUE:
                m_action = CONTINUE;
                break;

            case DebugAction::STEP_OVER:
                m_action = STEP_OVER;
//                m_lastCommandAtStackLevel = context_ ? context_->GetCallstackSize() : 1;
                std::cout << "asdfasdf: " << m_lastCommandAtStackLevel << std::endl;
                break;

            case DebugAction::STEP_INTO:
                m_action = STEP_INTO;
                break;

            case DebugAction::STEP_OUT:
                m_action = STEP_OUT;
//                m_lastCommandAtStackLevel = context_ ? context_->GetCallstackSize() : 0;
                std::cout << "asdfasdf 2: " << m_lastCommandAtStackLevel << std::endl;
                break;

            default:
                std::stringstream ss;
                ss << "Action '" << action << "' is not valid.";
                throw InvalidArgumentException(ss.str());
                break;
        }

        std::cout << "performAction 2 context_: " << context_ << " action: " << action << std::endl;
        setRunning(false);

        keepContext_ = true;
    }

    std::string filename() const override
    {
        if (context_ == nullptr) return "";

        const char* scriptSection;
        context_->GetLineNumber(0, 0, &scriptSection);

        return scriptSection != nullptr ? scriptSection : "";
    }

    std::string functionName() const override
    {
        if (context_ == nullptr) return "";

        const asIScriptFunction* func = context_->GetFunction(0);

        return func->GetDeclaration();
    }

    int32 line() const override
    {
        if (context_ == nullptr) return -1;

        return context_->GetLineNumber();
    }

    std::string stacktrace() override
    {
        if (context_ == nullptr) return "";

        buffer_ = {};

        PrintCallstack(context_);

        return buffer_.str();
    }

    std::string variables() override
    {
        if (context_ == nullptr) return "";

        buffer_ = {};

        ListMemberProperties(context_);
        ListGlobalVariables(context_);
        ListLocalVariables(context_);

        return buffer_.str();
    }

    void Output(const std::string& str) override
    {
        buffer_ << str;
    }

    std::string ToString(void* value, asUINT typeId, int expandMembers, asIScriptEngine* engine) override
    {
        // If no engine pointer was provided use the default
        if( engine == 0 )
            engine = m_engine;

        std::stringstream s;
        if( typeId & asTYPEID_APPOBJECT )
        {
            // Dereference handles, so we can see what it points to
            if( typeId & asTYPEID_OBJHANDLE )
                value = *(void**)value;

            // Print the address for reference types so it will be
            // possible to see when handles point to the same object
            if( engine )
            {
                asITypeInfo *type = engine->GetTypeInfoById(typeId);
                if( type->GetFlags() & asOBJ_REF )
                {
                    if (!value)
                    {
                        s << "{null}";
                    }
                    else
                    {
                        s << "{" << value << "}";
                    }
                }

                if( value )
                {
                    // Check if there is a registered to-string callback
                    auto it = registeredToStringCallbacks_.find(type->GetName());
                    if( it == registeredToStringCallbacks_.end() )
                    {
                        // If the type is a template instance, there might be a
                        // to-string callback for the generic template type
                        if( type->GetFlags() & asOBJ_TEMPLATE )
                        {
                            asITypeInfo *tmplType = engine->GetTypeInfoByName(type->GetName());
                            it = registeredToStringCallbacks_.find(tmplType->GetName());
                        }
                    }

                    if( it != registeredToStringCallbacks_.end() )
                    {
                        if( type->GetFlags() & asOBJ_REF )
                            s << " ";

                        // Invoke the callback to get the string representation of this type
                        std::string str = it->second(value);
                        s << str;
                    }
                    else
                    {
                        if (type->GetFlags() & asOBJ_VALUE)
                        {
                            // Attempt to print out the properties of the type
                            for (asUINT n = 0; n < type->GetPropertyCount(); n++)
                            {
                                if (n == 0)
                                    s << " ";
                                else
                                    s << ", ";

                                const auto decl = type->GetPropertyDeclaration(n);
                                //                            auto addr = obj->GetAddressOfProperty(n);
                                //                            const auto typeId = obj->GetPropertyTypeId(n);
                                //                            const auto str = ToString(addr, typeId, expandMembers - 1, type->GetEngine());
                                //                            s << decl << " = " << str;
                                s << decl << " = {?}";
                            }
                        }
                    }
                }
            }
            else
                s << "{no engine}";
        }
        else
        {
            return CDebugger::ToString(value, typeId, expandMembers, engine);
        }

        return s.str();
    }

    void registerToStringCallback(const std::string& obj, const std::function<std::string(void*)>& function) override
    {
//        const auto type = engine_->GetTypeInfoByName(obj.c_str());
//
//        if (!type) throw InvalidArgumentException("'" + obj + "' is not a registered type.");

        if (registeredToStringCallbacks_.find(obj) != registeredToStringCallbacks_.end())
        {
            LOG_WARN(logger_, "toString callback for object '%s' already exists - overwriting.", obj)
        }

        registeredToStringCallbacks_[obj] = function;
    }

    void addDebugEventListener(IDebugEventListener* listener) override
    {
        if (std::find(debugEventListeners_.begin(), debugEventListeners_.end(), listener) != debugEventListeners_.end())
        {
            throw InvalidArgumentException("TextInput event listener already exists.");
        }

        debugEventListeners_.push_back(listener);
    }

    void removeDebugEventListener(const IDebugEventListener* listener) override
    {
        auto it = std::find(debugEventListeners_.begin(), debugEventListeners_.end(), listener);
        if (it != debugEventListeners_.end())
        {
            debugEventListeners_.erase(it);
        }
    }

    bool enabled() const override
    {
        return enabled_;
    }

    void setEnabled(const bool enabled) override
    {
        enabled_ = enabled;
    }

    bool running() const override
    {
        return running_;
    }

    void setRunning(const bool running) override
    {
        running_ = running;
    }

    void TakeCommands(asIScriptContext* ctx) override
    {
//        std::cout << "TakeCommands 1 context_: " << context_ << " ctx: " << ctx << " running_: " << running_ << " keepContext_: " << keepContext_ << std::endl;

        asUINT nestCount = 0;
        ctx->IsNested(&nestCount);

        if (context_ && (context_ != ctx || nestCount_ != nestCount))
        {
//            LOG_DEBUG(logger_, "Ignoring commands for context '%s' with nest count %s - debugging in another context.", ctx, nestCount)
//            std::cout << "Ignoring commands for context - debugging in another context." << std::endl;
            return;
        }

        buffer_ = {};

        PrintCallstack(ctx);

        LOG_DEBUG(logger_, "Begin debugging for context '%s' with nest count %s and %s and  callstack size %s. Callstack: \n%s", ctx, nestCount, m_lastCommandAtStackLevel, ctx->GetCallstackSize(), buffer_.str())

        std::cout << "TakeCommands 2 context_: " << context_ << " ctx: " << ctx << std::endl;

        buffer_ = {};

        running_ = true;

        context_ = ctx;
        nestCount_ = nestCount;
        m_lastCommandAtStackLevel = context_->GetCallstackSize();

        asIScriptFunction* func;
        const char* scriptSection;
        int line, column;
        func = context_->GetFunction(0);
        line = context_->GetLineNumber(0, &column, &scriptSection);
        printf("%s:%s:%d,%d\n", scriptSection,
               func->GetDeclaration(),
               line, column);

        ListBreakPoints();

        std::cout << "TakeCommands ListBreakPoints(): " << buffer_.str() << std::endl;

        buffer_ = {};

//        for(;;)
//        {
//            char buf[512];
//
//            Output("[dbg]> ");
//            std::cin.getline(buf, 512);
//
//            if( InterpretCommand(std::string(buf), ctx) )
//                break;
//        }
        {
            DebugEvent event;
            event.type = HIT_BREAKPOINT;

            std::cout << "TakeCommands 3" << std::endl;
            for (auto listener : debugEventListeners_)
            {
                listener->processEvent(event);

//                if (!running_) break;
            }
            std::cout << "TakeCommands 4 running_: " << running_ << ", keepContext_: " << keepContext_ << std::endl;
        }

        // If we have been told to stop, null out the context to allow a different context to be debugged
        if (!keepContext_)
        {
            context_ = nullptr;
            nestCount_ = 0;
        }

        keepContext_ = false;
        running_ = false;

        LOG_DEBUG(logger_, "End debugging for context '%s' with nest count %s.", ctx, nestCount)
    }

private:
    logger::ILogger* logger_;

    bool enabled_ = false;
    bool running_ = false;
    bool keepContext_ = false;
    asIScriptContext* context_ = nullptr;
    asUINT nestCount_ = 0;
    std::mutex mutex_;

    std::stringstream buffer_;

    std::unordered_map<std::string, std::function<std::string(void*)>> registeredToStringCallbacks_;

    std::vector<IDebugEventListener*> debugEventListeners_;

    void lineCallback(asIScriptContext* context)
    {
//        std::cout << "lineCallback 1" << std::endl;

//        context_ = context;

//        asIScriptFunction* func;
//        const char* scriptSection;
//        int line, column;
//        func = context->GetFunction(0);
//        line = context->GetLineNumber(0, &column, &scriptSection);
//        printf("%s:%s:%d,%d\n", scriptSection,
//               func->GetDeclaration(),
//               line, column);

//        buffer_ = {};

        LineCallback(context);
//
//        std::lock_guard<std::mutex> lockGuard(mutex_); 
//
//        std::cout << "lineCallback 2" << std::endl;
//        running_ = true;
//
//        {
//            DebugEvent event;
//            event.type = HIT_BREAKPOINT;
//
//            std::cout << "lineCallback 3" << std::endl;
//            for (auto listener : debugEventListeners_)
//            {
//                listener->processEvent(event);
//
////                if (!running_) break;
//            }
//        }
//
//        std::cout << "lineCallback 4" << std::endl;
//
////        LineCallback(context);
//
//        DebugEvent event;
//        event.type = DONE;
//
//        for (auto listener : debugEventListeners_)
//        {
//            listener->processEvent(event);
//        }
//
//        running_ = false;
    }
};

}
}
}

#endif //ANGELSCRIPTDEBUGGER_HPP_
