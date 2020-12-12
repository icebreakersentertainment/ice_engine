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

fs::IFileSystem* CPreProcessor::staticFileSystem_ = nullptr;
std::unordered_map<std::string, std::string> CPreProcessor::staticIncludeOverrides_;
logger::ILogger* CPreProcessor::staticLogger_ = nullptr;

CPreProcessor::CPreProcessor(fs::IFileSystem* fileSystem, logger::ILogger* logger, const std::unordered_map<std::string, std::string>& includeOverrides) : fileSystem_(fileSystem), logger_(logger), includeOverrides_(includeOverrides)
{
    CPreProcessor::staticIncludeOverrides_ = includeOverrides;
    CPreProcessor::staticFileSystem_ = fileSystem;
    CPreProcessor::staticLogger_ = logger;
}

std::string CPreProcessor::process(std::string source, const std::unordered_map<std::string, std::string>& defineMap, const bool autoIncludeGuard, const bool preserveLineNumbers)
{
    autoIncludeGuard_ = autoIncludeGuard;
    preserveLineNumbers_ = preserveLineNumbers;
    conditionalDepth_ = false;
    conditionalAllowNewlineDepth_ = false;
    inDefine_ = false;
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
            CPreProcessor,
            CPreProcessor
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
    ctx.set_language(boost::wave::language_support(boost::wave::support_option_no_newline_at_end_of_file | boost::wave::support_option_insert_whitespace));
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

//    std::stringstream ss;

    try
    {
        while ( first != last )
        {
            current_position = (*first).get_position();
            *outputStringStream_ << (*first).get_value();
            ++first;
        }
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

std::string CPreProcessor::toCanonicalPath(const std::string& currentDirectory, const std::string& filename)
{
//    std::cout << "toCanonicalPath:" << filename << std::endl;
//    std::cout << "toCanonicalFullFilePath2: " << currentDirectory << std::endl;

    std::string editedFilename = filename;
    // remove any quotations around the filename
    boost::algorithm::erase_all(editedFilename, "\"");

    // if the file exists we're done
    {
        const auto it = includeOverrides_.find(editedFilename);
        if (it != includeOverrides_.end())
        {
            return it->first;
        }
        else if (fileSystem_->exists(editedFilename))
        {
//        std::cout << "toCanonicalFullFilePath2 A: " << editedFilename << " | " << fileSystem_->getCanonicalPath(editedFilename) << std::endl;
            return fileSystem_->getCanonicalPath(editedFilename);
        }
    }

    namespace fs = boost::filesystem;
//
//    const auto currentDirectory = fs::path(currentDirectory);
//    auto currentDirectoryAsString = currentDirectory.string();
    auto editedCurrentDirectory = currentDirectory;
    const auto baseDirectoryAsString = fs::current_path().string();

    const size_t pos = currentDirectory.find(baseDirectoryAsString);

    if (pos != std::string::npos)
    {
        editedCurrentDirectory.erase(pos, baseDirectoryAsString.length());
    }

//    std::cout << "toCanonicalFullFilePath2 B: " << editedCurrentDirectory << " | " << baseDirectoryAsString << std::endl;

    fs::path file{editedFilename};
    fs::path relativeDirectory{editedCurrentDirectory};

    // it may be that we are doing a relative include, in which case we may need some of the directories that exist from the relative directory. This will pop directories off of the relative directory, combine them with the filename and see if the new file name created exists.
    while (!relativeDirectory.empty())
    {
        const auto pathPiece = fs::path(relativeDirectory.filename());
        file = pathPiece / file;

//        std::cout << "toCanonicalFullFilePath3 A: " << pathPiece << " | " << file << std::endl;

        if (fileSystem_->exists(file.string()))
        {
//            std::cout << "toCanonicalFullFilePath3 B: " << file.string() << " | " << fileSystem_->getCanonicalPath(file.string()) << std::endl;
            return fileSystem_->getCanonicalPath(file.string());
        }

        relativeDirectory = relativeDirectory.parent_path();
    }

//    std::cout << "toCanonicalFullFilePath2 END NOT FOUND" << std::endl;

    return "";
}

bool CPreProcessor::locateIncludeFile(const std::string& currentDirectory, /*const std::string& basePath,*/ std::string& filePath, bool isSystem, char const* currentName, std::string& dirPath, std::string& nativeName)
{
//    namespace fs = boost::filesystem;

    const auto canonicalPath = toCanonicalPath(currentDirectory, filePath);
//    std::cout << "canonicalPath: " << canonicalPath << std::endl;

    filePath = canonicalPath;
    nativeName = wave::util::native_file_string(canonicalPath);

//    if (!fs::exists(nativeName))
//    {
//        //BOOST_WAVE_THROW_CTX(ctx, preprocess_exception, bad_include_file,
//        //    filePath.c_str(), ctx.get_main_pos());
//        std::cout << "error: doesn't exist" << std::endl;
//        std::cout << "locate_include_file doesn't exist 3: filePath:" << filePath << " dirPath:" << dirPath
//                  << " nativeName:" << nativeName << std::endl;
//        return false;
//    }

//    fs::path baseDir = fs::path(basePath);
//    std::string filePathTmp = filePath;
//    if (fs::exists(baseDir) && fs::is_directory(baseDir))
//    {
//        std::cout << "basePath: " << basePath << std::endl;
//        filePathTmp = baseDir.string().empty() ? filePath :  baseDir.string() + std::string("/") + filePath;
//    }
//
//
//    std::cout << "locate_include_file doesn't exist 2: baseDir:" << baseDir.string() << " filePathTmp:" << filePathTmp << " dirPath:" << dirPath
//              << " nativeName:" << nativeName << std::endl;
//    if (!ctx.find_include_file(filePathTmp, dirPath, isSystem, currentName))
//    {
//        std::cout << "locate_include_file !find_include_file: filePathTmp:" << filePathTmp << " dirPath:"
//                  << dirPath << " nativeName:" << nativeName << std::endl;
//        return false;
//    }
//
//    filePath = filePathTmp;
//
//    fs::path nativePath(wave::util::create_path(filePath));
//    if (!fs::exists(nativePath))
//    {
//        //BOOST_WAVE_THROW_CTX(ctx, preprocess_exception, bad_include_file,
//        //    filePath.c_str(), ctx.get_main_pos());
//        std::cout << "error: doesn't exist" << std::endl;
//        std::cout << "locate_include_file doesn't exist 3: filePath:" << filePath << " dirPath:" << dirPath
//                  << " nativeName:" << nativeName << std::endl;
//        return false;
//    }
//
//    // return the unique full file system path of the located file
//    nativeName = wave::util::native_file_string(nativePath);
//
//    std::cout << "locate_include_file FOUND: filePath:" << filePath << " dirPath:" << dirPath
//              << " nativeName:" << nativeName << std::endl;

    return true;
}

}
