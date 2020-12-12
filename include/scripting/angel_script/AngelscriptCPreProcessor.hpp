#ifndef ANGELSCRIPTCPREPROCESSOR_HPP_
#define ANGELSCRIPTCPREPROCESSOR_HPP_

#include <vector>
#include <sstream>
#include <regex>

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
namespace scripting
{
namespace angel_script
{

class AngelscriptCPreProcessor : public CPreProcessor
{
public:
    AngelscriptCPreProcessor(fs::IFileSystem* fileSystem, logger::ILogger* logger, const std::unordered_map<std::string, std::string>& includeOverrides = {});

    std::string process(std::string source, const std::unordered_map<std::string, std::string>& defineMap = {}, const bool autoIncludeGuard = false, const bool preserveLineNumbers = false);

    std::vector<std::pair<std::string, std::string>> getProcessedFileSources() const;

    /* Below is a bunch of Boost Wave call-back functions */
    template <typename ContextT>
    void opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& filename, bool is_system_include)
    {
        auto ss = std::make_shared<std::stringstream>();
        *currentIncludeFileOutputStringStream_ = ss;
        currentIncludeFileStack_->push({filename, ss});
//        std::cout << "cpp_include_begin: " << filename << std::endl;
        CPreProcessor::template opened_include_file<ContextT>(ctx, relname, filename, is_system_include);
    }

    template <typename ContextT>
    void returning_from_include_file(ContextT const& ctx)
    {
        const auto currentIncludeFileData = currentIncludeFileStack_->top();
        processedFileSources_->push_back({currentIncludeFileData.first, currentIncludeFileData.second->str()});
//        std::cout << "cpp_include_end: " << currentIncludeFileData.first << std::endl;
        CPreProcessor::template returning_from_include_file<ContextT>(ctx);

        currentIncludeFileStack_->pop();

//        std::cout << "cpp_include_end resuming: " << currentIncludeFileStack_->top().first << std::endl;
        *currentIncludeFileOutputStringStream_ = currentIncludeFileStack_->top().second;

        // inject a newline after we return from the include
        if (preserveLineNumbers_)
        {
            **currentIncludeFileOutputStringStream_ << boost::wave::get_token_value(boost::wave::T_NEWLINE);
        }
    }

    template <typename ContextT>
    bool found_include_directive(ContextT const& ctx, std::string const& filename, bool include_next)
    {
        const bool result = CPreProcessor::template found_include_directive<ContextT>(ctx, filename, include_next);

        if (result && preserveLineNumbers_)
        {
//            *outputStringStream_ << boost::wave::get_token_value(boost::wave::T_NEWLINE);
            **currentIncludeFileOutputStringStream_ << boost::wave::get_token_value(boost::wave::T_NEWLINE);
        }

        return result;
    }

    template <typename ContextT, typename TokenT>
    void skipped_token(ContextT const& ctx, TokenT const& token)
    {
//        if (token != boost::wave::T_SPACE)
//        {
//            std::cout << "skipped_token: " << token
//                      << " | " << boost::wave::get_token_name(token)
//                      << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                      << std::endl;
//        }

        switch (token)
        {
            case boost::wave::T_NEWLINE:
                if (conditionalAllowNewlineDepth_)
                {
//                    std::cout << "skipped_token conditionalAllowNewlineDepth_ NEWLINE: "
//                              << conditionalAllowNewlineDepth_
//                              << " | " << token
//                              << " | " << boost::wave::get_token_name(token)
//                              << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                              << std::endl;

                    *outputStringStream_ << boost::wave::get_token_value(token);
                    **currentIncludeFileOutputStringStream_ << boost::wave::get_token_value(token);
                    --conditionalAllowNewlineDepth_;
                }
                else if (conditionalDepth_)
                {
//                    std::cout << "skipped_token NEWLINE: "
//                              << conditionalDepth_
//                              << " | " << token
//                              << " | " << boost::wave::get_token_name(token)
//                              << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                              << std::endl;

                    *outputStringStream_ << boost::wave::get_token_value(token);
                    **currentIncludeFileOutputStringStream_ << boost::wave::get_token_value(token);
                }
                else if (inDefine_)
                {
//                    std::cout << "skipped_token inDefine_: " << token
//                              << " | " << boost::wave::get_token_name(token)
//                              << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                              << std::endl;

                    *outputStringStream_ << boost::wave::get_token_value(token);
                    **currentIncludeFileOutputStringStream_ << boost::wave::get_token_value(token);

                    inDefine_ = false;
                }

                break;

            case boost::wave::T_PP_ENDIF:
                if (conditionalDepth_)
                {
//                    std::cout << "skipped_token T_PP_ENDIF: "
//                              << conditionalDepth_
//                              << " | " << token
//                              << " | " << boost::wave::get_token_name(token)
//                              << " | " << (token == boost::wave::T_NEWLINE ? "\\n" : boost::wave::get_token_value(token))
//                              << std::endl;

                    ++conditionalAllowNewlineDepth_;
                    --conditionalDepth_;
                }

                break;
        }
    }

private:
    std::shared_ptr<std::stack<std::pair<std::string, std::shared_ptr<std::stringstream>>>> currentIncludeFileStack_;
    std::shared_ptr<std::shared_ptr<std::stringstream>> currentIncludeFileOutputStringStream_;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> processedFileSources_;
};

}
}
}

#endif //ANGELSCRIPTCPREPROCESSOR_HPP_
