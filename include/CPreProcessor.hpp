#ifndef CPREPROCESSOR_H
#define CPREPROCESSOR_H

#if !defined(BOOST_WAVE_CUSTOM_DIRECTIVES_HOOKS_INCLUDED)
#define BOOST_WAVE_CUSTOM_DIRECTIVES_HOOKS_INCLUDED

#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <utility>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>

// Need to include this before other wave/phoenix includes
// @see https://groups.google.com/forum/#!msg/boost-list/3xZDWUyTJG0/IEF2wTy1EIsJ
#include <boost/phoenix/core/limits.hpp>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/macro_helpers.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpp_iteration_context.hpp>

#include <iterator>
#include <fstream>
#if defined(BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#include <sstream>
#endif

#include <boost/wave/wave_config.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <boost/wave/language_support.hpp>
#include <boost/wave/util/file_position.hpp>

#include "Types.hpp"

#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace wave = boost::wave;
namespace alg = boost::algorithm;

namespace ice_engine
{

/**
 * This class is a generic pre processor for C like code.
 */
class CPreProcessor : public wave::context_policies::default_preprocessing_hooks
{
public:
    CPreProcessor(fs::IFileSystem* fileSystem, logger::ILogger* logger, const std::unordered_map<std::string, std::string>& includeOverrides = {});

    /**
     * Processess the source code.
     *
     * @param defineMap this is an optional map that can contain any extra/special #defines that have been
     * defined.
     */
    std::string process(std::string source, const std::unordered_map<std::string, std::string>& defineMap = {}, const bool autoIncludeGuard = false, const bool preserveLineNumbers = false);

    /* Below is a bunch of Boost Wave call-back functions */
    template <typename ContextT, typename ContainerT>
    bool found_unknown_directive(ContextT const& ctx, ContainerT const& line, ContainerT& pending);

    template <typename ContextT, typename ContainerT>
    bool emit_line_directive(ContextT const & ctx, ContainerT & pending, typename ContextT::token_type const & act_token);

    template <typename ContextT>
    bool locate_include_file(ContextT& ctx, std::string&file_path, bool is_system, char const*current_name, std::string&dir_path, std::string&native_name);

    template <typename ContextT>
    void opened_include_file(ContextT const& ctx, std::string const& relname, std::string const& filename, bool is_system_include);

    template <typename ContextT>
    void returning_from_include_file(ContextT const& ctx);

    template <typename ContextT>
    bool found_include_directive(ContextT const& ctx, std::string const& filename, bool include_next);

    template <typename ContextT, typename TokenT>
    void
    skipped_token(ContextT const& ctx, TokenT const& token);

    template <typename ContextT, typename TokenT>
    TokenT const& generated_token(ContextT const &ctx, TokenT const& token);

    template <typename ContextT, typename TokenT, typename ContainerT>
    bool evaluated_conditional_expression(
            ContextT const &ctx, TokenT const& directive,
            ContainerT const& expression, bool expression_value);

    template <typename ContextT, typename TokenT>
    bool found_directive(ContextT const& ctx, TokenT const& directive);

    /**
     * Inner class to do the loading of a file.
     */
    template <typename IterContextT>
    class inner {
    public:
        template <typename PositionT>
        static void init_iterators(IterContextT& iter_ctx, PositionT const& act_pos, wave::language_support language)
        {
            typedef typename IterContextT::iterator_type iterator_type;

//            std::cout << "init_iterators: " << iter_ctx.filename << std::endl;

            const auto filename = std::string(iter_ctx.filename.c_str());

            {
                const auto it = CPreProcessor::staticIncludeOverrides_.find(filename);
                if (it != CPreProcessor::staticIncludeOverrides_.end())
                {
                    iter_ctx.instring = it->second;
                } else
                {
                    iter_ctx.instring = CPreProcessor::staticFileSystem_->readAll(filename);
                }
            }

            // read in the file
//            std::ifstream instream(iter_ctx.filename.c_str());
//            if ( !instream.is_open())
//            {
////                BOOST_WAVE_THROW_CTX(iter_ctx.ctx, wave::preprocess_exception, wave::bad_include_file, iter_ctx.filename.c_str(), act_pos);
//                return;
//            }
//            instream.unsetf(std::ios::skipws);
//
//            iter_ctx.instring.assign(
//                    std::istreambuf_iterator<char>(instream.rdbuf()),
//                    std::istreambuf_iterator<char>()
//            );

            // ensure our input string ends with a newline (if we don't then boost wave generates a newline token after the include directive finishes)
            auto it = iter_ctx.instring.end();
            if (*(--it) != *boost::wave::get_token_value(boost::wave::T_NEWLINE))
            {
                iter_ctx.instring += boost::wave::get_token_value(boost::wave::T_NEWLINE);
            }

            iter_ctx.first = iterator_type(
                    iter_ctx.instring.begin(),
                    iter_ctx.instring.end(),
                    PositionT(iter_ctx.filename),
                    language
            );
            iter_ctx.last = iterator_type();
        }

    private:
        std::string instring;
    };

private:


protected:
    bool preserveLineNumbers_ = false;
    bool autoIncludeGuard_ = false;
    uint32 conditionalDepth_ = 0;
    uint32 conditionalAllowNewlineDepth_ = 0;
    bool inDefine_ = false;

    uint32 numIncludes_ = 0;
    std::unordered_map<std::string, bool> includedFiles_;
    std::unordered_map<std::string, std::string> includeOverrides_;

    fs::IFileSystem* fileSystem_;
    logger::ILogger* logger_;

    static std::unordered_map<std::string, std::string> staticIncludeOverrides_;
    static fs::IFileSystem* staticFileSystem_;
    static logger::ILogger* staticLogger_;

    /*
     * We make this a shared pointer so that when the context object makes a copy of `this`,
     * it will be using the same outputStringStream_.
     */
    std::shared_ptr<std::stringstream> outputStringStream_;

    bool locateIncludeFile(const std::string& currentDirectory, /*const std::string& basePath,*/ std::string& filePath, bool isSystem, char const* currentName, std::string& dirPath, std::string& nativeName);

    std::string toCanonicalPath(const std::string& currentDirectory, const std::string& filename);
};

}

#include "CPreProcessor.inl"

#endif // !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)

#endif /* CPREPROCESSOR_H */