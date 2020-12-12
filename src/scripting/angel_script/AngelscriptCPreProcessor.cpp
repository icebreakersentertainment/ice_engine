#include "scripting/angel_script/AngelscriptCPreProcessor.hpp"


namespace ice_engine
{
namespace scripting
{
namespace angel_script
{

AngelscriptCPreProcessor::AngelscriptCPreProcessor(fs::IFileSystem* fileSystem, logger::ILogger* logger, const std::unordered_map<std::string, std::string>& includeOverrides) : CPreProcessor(fileSystem, logger, includeOverrides)
{

}

std::string AngelscriptCPreProcessor::process(std::string source, const std::unordered_map<std::string, std::string>& defineMap, const bool autoIncludeGuard, const bool preserveLineNumbers)
{
    autoIncludeGuard_ = autoIncludeGuard;
    preserveLineNumbers_ = preserveLineNumbers;
    conditionalDepth_ = 0;
    conditionalAllowNewlineDepth_ = 0;
    inDefine_ = false;
    currentIncludeFileStack_ = std::make_shared<std::stack<std::pair<std::string, std::shared_ptr<std::stringstream>>>>();
    currentIncludeFileOutputStringStream_ = std::make_shared<std::shared_ptr<std::stringstream>>();
    processedFileSources_ = std::make_shared<std::vector<std::pair<std::string, std::string>>>();
    outputStringStream_ = std::make_shared<std::stringstream>();
    includedFiles_ = {};
    numIncludes_ = 0;

    // current file position is saved for exception handling
    boost::wave::util::file_position_type current_position;

    //  The template boost::wave::cpplexer::lex_token<> is the token type to be
    //  used by the Wave library.
    typedef boost::wave::cpplexer::lex_token<> token_type;

    //  The template boost::wave::cpplexer::lex_iterator<> is the lexer type to
    //  be used by the Wave library.
    typedef boost::wave::cpplexer::lex_iterator<token_type> lex_iterator_type;

    //  This is the resulting context type to use. The first template parameter
    //  should match the iterator type to be used during construction of the
    //  corresponding context object (see below).
    typedef boost::wave::context<
            std::string::iterator,
            lex_iterator_type,
            AngelscriptCPreProcessor,
            AngelscriptCPreProcessor
    > context_type;

    //CPreProcessor hooks(files);

    // The preprocessor iterator shouldn't be constructed directly. It is
    // to be generated through a wave::context<> object. This wave:context<>
    // object additionally may be used to initialize and define different
    // parameters of the actual preprocessing (not done here).
    //
    // The preprocessing of the input stream is done on the fly behind the
    // scenes during iteration over the context_type::iterator_type stream.
    //context_type ctx (source.begin(), source.end(), "lex_infile", hooks);
    //std::cout << "processing: " << std::endl << source << std::endl;
    context_type ctx(source.begin(), source.end(), "lex_infile", *this);

//    ctx.set_language(boost::wave::support_cpp11);
    ctx.set_language(boost::wave::support_option_no_newline_at_end_of_file);
    ctx.set_language(boost::wave::enable_long_long(ctx.get_language()));
    ctx.set_language(boost::wave::enable_preserve_comments(ctx.get_language()));
    ctx.set_language(boost::wave::enable_prefer_pp_numbers(ctx.get_language()));

    for (const auto& define : defineMap)
    {
        ctx.add_macro_definition(define.first + "=" + define.second);
    }

    // analyze the input file, print out the preprocessed tokens
    context_type::iterator_type first = ctx.begin();
    context_type::iterator_type last = ctx.end();

    try
    {
        auto ss = std::make_shared<std::stringstream>();
        *currentIncludeFileOutputStringStream_ = ss;
        currentIncludeFileStack_->push({"", ss});

        while ( first != last )
        {
            current_position = (*first).get_position();
            *outputStringStream_ << (*first).get_value();
            **currentIncludeFileOutputStringStream_ << (*first).get_value();
            ++first;
        }

        const auto currentIncludeFileData = currentIncludeFileStack_->top();
        processedFileSources_->push_back({currentIncludeFileData.first, currentIncludeFileData.second->str()});
    }
    catch ( const boost::wave::cpp_exception& e )
    {
        // some preprocessing error
        std::cerr
                << e.file_name() << "(" << e.line_no() << "): "
                << e.description() << std::endl;
        return "";
    }
    catch ( const std::exception& e )
    {
        // use last recognized token to retrieve the error position
        std::cerr
                << current_position.get_file()
                << "(" << current_position.get_line() << "): "
                << "exception caught: " << e.what()
                << std::endl;
        return "";
    }
    catch ( ... )
    {
        // use last recognized token to retrieve the error position
        std::cerr
                << current_position.get_file()
                << "(" << current_position.get_line() << "): "
                << "unexpected exception caught." << std::endl;
        return "";
    }

    return outputStringStream_->str();
}

std::vector<std::pair<std::string, std::string>> AngelscriptCPreProcessor::getProcessedFileSources() const
{
    return *processedFileSources_;
}

}
}
}