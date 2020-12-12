#ifndef ANGELSCRIPTUTILITIES_HPP_
#define ANGELSCRIPTUTILITIES_HPP_

#ifndef ANGELSCRIPT_H
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

#include "Types.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/InvalidArgumentException.hpp"

namespace ice_engine
{
namespace scripting
{
namespace angel_script
{

inline void assertNoAngelscriptError(const int32 returnCode)
{
    if (returnCode < 0)
    {
        switch (returnCode)
        {
            case asERROR:
                throw Exception("ScriptEngine: Generic error.");
                break;

            case asINVALID_ARG:
                throw InvalidArgumentException("ScriptEngine: Argument was invalid.");
                break;

            case asNOT_SUPPORTED:
                throw Exception("ScriptEngine: Operation not supported.");
                break;

            case asNO_MODULE:
                throw Exception("ScriptEngine: Module not found.");
                break;

            case asINVALID_TYPE:
                throw Exception("ScriptEngine: The type specified is invalid.");
                break;

            case asNO_GLOBAL_VAR:
                throw Exception("ScriptEngine: No matching property was found.");
                break;

            case asINVALID_DECLARATION:
                throw Exception("ScriptEngine: The specified declaration is invalid.");
                break;

            case asINVALID_NAME:
                throw Exception("ScriptEngine: The name specified is invalid.");
                break;

            case asALREADY_REGISTERED:
                throw Exception("ScriptEngine: The specified type or name is already registered.");
                break;

            case asNAME_TAKEN:
                throw Exception("ScriptEngine: The specified name is already taken.");
                break;

            case asWRONG_CALLING_CONV:
                throw Exception(
                        "ScriptEngine: The specified calling convention is not valid or does not match the registered calling convention.");
                break;

            case asWRONG_CONFIG_GROUP:
                throw Exception("ScriptEngine: Wrong configuration group.");
                break;

            case asCONFIG_GROUP_IS_IN_USE:
                throw Exception("ScriptEngine: Configuration group already in use.");
                break;

            case asILLEGAL_BEHAVIOUR_FOR_TYPE:
                throw Exception("ScriptEngine: Illegal behaviour for type.");
                break;

            case asINVALID_OBJECT:
                throw Exception("ScriptEngine: The object does not specify a valid object type.");
                break;

            case asLOWER_ARRAY_DIMENSION_NOT_REGISTERED:
                throw Exception("ScriptEngine:  Array element must be a primitive or a registered type.");
                break;

            default:
                const auto str = std::string("ScriptEngine: Unknown error: ") + std::to_string(returnCode);
                throw Exception(str.c_str());
                break;
        }
    }
}

}
}
}

#endif //ANGELSCRIPTUTILITIES_HPP_
