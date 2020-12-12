#ifndef CPREPROCESSOR_INL_
#define CPREPROCESSOR_INL_

#include <vector>
#include <sstream>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

///////////////////////////////////////////////////////////////////////////////
//  Include Wave itself
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

#include "CPreProcessor.hpp"

namespace ice_engine
{

template <typename ContextT, typename ContainerT>
bool CPreProcessor::found_unknown_directive(ContextT const& ctx, ContainerT const& line, ContainerT& pending)
{
    return false;
}


template <typename ContextT, typename ContainerT>
bool CPreProcessor::emit_line_directive(ContextT const& ctx, ContainerT&pending, typename ContextT::token_type const& act_token)
{
    return false;
}

template <typename ContextT>
void CPreProcessor::opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& filename, bool is_system_include)
{
//    std::cout << "opened_include_file: " << "relname: " << relname << " filename: " << filename << " is_system_include: " << is_system_include << std::endl;
    ++numIncludes_;
}


template <typename ContextT>
void CPreProcessor::returning_from_include_file(ContextT const& ctx)
{

}

template <typename ContextT>
bool CPreProcessor::found_include_directive(ContextT const& ctx, std::string const& filename, bool include_next)
{
//    std::cout << "found_include_directive:" << filename << std::endl;

    const auto canonicalPath = toCanonicalPath(ctx.get_current_directory().string(), filename);

    if (autoIncludeGuard_ && includedFiles_.find(canonicalPath) != includedFiles_.end())
    {
//        std::cout << "found_include_directive: ignored!" << filename << std::endl;

        if (preserveLineNumbers_)
        {
//            *outputStringStream_ << "<cpp_include_ignored>" << filename << "</cpp_include_ignored>" ;
            *outputStringStream_ << boost::wave::get_token_value(boost::wave::T_NEWLINE);
        }

        return true;
    }

    includedFiles_[canonicalPath] = true;

    return false;

//    boost::regex systemIncludeRegex("<.*>");
//
//    if ( !boost::regex_match(filename, systemIncludeRegex))
//    {
//        std::string editedFilename = filename;
//        // remove any quotations around the filename
//        boost::algorithm::erase_all(editedFilename, "\"");
//
//        //std::stringstream msg;
//        //msg << "adding: " << editedFilename << " size: " << shaderData_.size();
//        //LOG_DEBUG( msg.str() );
//
////        shaderData_.push_back(ShaderData(editedFilename));
//    }
//
//    // ok to include this file
//    return false;
}

template <typename ContextT>
bool CPreProcessor::locate_include_file(ContextT& ctx, std::string& file_path, bool is_system, char const* current_name, std::string& dir_path, std::string& native_name)
{
//    namespace fs = boost::filesystem;
//
//    for (const auto& includeDir : includeDirectories_)
//    {
//        ctx.add_include_path(includeDir.c_str());
//        if (locateIncludeFile<ContextT>(ctx, includeDir, file_path, is_system, current_name, dir_path, native_name))
//        {
//            return true;
//        }
//    }

//    if (numIncludes_ != 0
//        && locateIncludeFile<ContextT>(ctx, "", file_path, is_system, current_name, dir_path, native_name))
//    {
//        return true;
//    }

    return locateIncludeFile(ctx.get_current_directory().string(), file_path, is_system, current_name, dir_path, native_name);
}

template <typename ContextT, typename TokenT>
void
CPreProcessor::skipped_token(ContextT const& ctx, TokenT const& token)
{
//    if (token != boost::wave::T_SPACE)
//    {
//        std::cout << "skipped_token: " << token
//                  << " | " << boost::wave::get_token_name(token)
//                  << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                  << std::endl;
//    }

    switch (token)
    {
        case boost::wave::T_NEWLINE:
            if (conditionalAllowNewlineDepth_)
            {
//                std::cout << "skipped_token conditionalAllowNewlineDepth_ NEWLINE: "
//                          << conditionalAllowNewlineDepth_
//                          << " | " << token
//                          << " | " << boost::wave::get_token_name(token)
//                          << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                          << std::endl;

                *outputStringStream_ << boost::wave::get_token_value(token);
                --conditionalAllowNewlineDepth_;
            }
            else if (conditionalDepth_)
            {
//                std::cout << "skipped_token NEWLINE: "
//                          << conditionalDepth_
//                          << " | " << token
//                          << " | " << boost::wave::get_token_name(token)
//                          << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                          << std::endl;

                *outputStringStream_ << boost::wave::get_token_value(token);
            }
            else if (inDefine_)
            {
//                std::cout << "skipped_token inDefine_: " << token
//                          << " | " << boost::wave::get_token_name(token)
//                          << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                          << std::endl;

                *outputStringStream_ << boost::wave::get_token_value(token);

                inDefine_ = false;
            }

            break;

        case boost::wave::T_PP_ENDIF:
            if (conditionalDepth_)
            {
//                std::cout << "skipped_token T_PP_ENDIF: "
//                          << conditionalDepth_
//                          << " | " << token
//                          << " | " << boost::wave::get_token_name(token)
//                          << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                          << std::endl;

                ++conditionalAllowNewlineDepth_;
                --conditionalDepth_;
            }

            break;
    }
}

template <typename ContextT, typename TokenT>
TokenT const& CPreProcessor::generated_token(ContextT const &ctx, TokenT const& token)
{
//    if (token != boost::wave::T_SPACE)
//    {
//        std::cout << "generated_token: " << token
//                  << " | " << boost::wave::get_token_name(token)
//                  << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                  << std::endl;
//    }

    return token;
}

template <typename ContextT, typename TokenT, typename ContainerT>
bool CPreProcessor::evaluated_conditional_expression(
        ContextT const &ctx, TokenT const& directive,
        ContainerT const& expression, bool expression_value)
{
//    if (directive != boost::wave::T_SPACE)
//    {
//        std::cout << "evaluated_conditional_expression: "
//                  << directive
//                  << " | " << boost::wave::get_token_name(directive)
//                  << " | " << (directive == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(directive))
//                  << " | " << expression_value
//                  << std::endl;
//    }

    if (preserveLineNumbers_ && (directive == boost::wave::T_PP_IF || directive == boost::wave::T_PP_IFDEF ||
                                 directive == boost::wave::T_PP_IFNDEF))
    {
        if (conditionalDepth_)
        {
//            std::cout << "OOPS asdfasdf: " << std::endl;
//            std::terminate();
        }
        ++conditionalDepth_;
    }

    return false;
}

template <typename ContextT, typename TokenT>
bool CPreProcessor::found_directive(ContextT const& ctx, TokenT const& directive)
{
    if (preserveLineNumbers_ && directive == boost::wave::T_PP_DEFINE)
    {
        inDefine_ = true;
    }

//    if (directive != boost::wave::T_SPACE)
//    {
//        std::cout << "found_directive: "
//                  << directive
//                  << " | " << boost::wave::get_token_name(directive)
//                  << " | " << (directive == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(directive))
//                  << std::endl;
//    }

    return false;
}

}


#endif //CPREPROCESSOR_INL_
