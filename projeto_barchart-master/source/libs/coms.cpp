#include "coms.h"

inline std::ostream& operator<<(std::ostream& os, const coms::SourceContext& sc)
{
    os << sc.file << ":" << sc.line;
    return os;
}

namespace coms{

    /// Prints out the warning + source context, but the program keeps running.
    void Warning( const std::string& msg,  const SourceContext& sc )
    {
        std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Warning: \"" << msg << "\"\n"
            << "     REPORTED AT: < " << sc << " > \n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;
    }
    /// Prints out the warning, but the program keeps running.
    void Warning( const std::string& msg )
    {
        std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Warning: \"" << msg << "\"\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;
    }

    /// Prints out the error message and exits the program.
    void Error( const std::string& msg, const SourceContext& sc )
    {
        std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Severe error: " << Color::tcolor( "\"" + msg + "\"", Color::RED ) << std::endl
            << "     REPORTED AT: < " << sc << " > " << std::endl
            << "     exiting...\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;

        std::exit( EXIT_FAILURE );
    }

    /// Prints out the error message + source context, and exits the program.
    void Error( const std::string& msg )
    {
        std::cerr << std::endl << std::setw(80) << std::setfill('=') << " " << std::endl
            << "Severe error: " << Color::tcolor( "\"" + msg + "\"", Color::RED ) << std::endl
            << "     exiting...\n"
            << std::setw(80) << std::setfill('=') << " " << std::endl;

        std::exit( EXIT_FAILURE );
    }

    /// Prints out a regular message.
    void Message( const std::string& str )
    {
        std::cout << Color::tcolor( ">>> "+ str, Color::GREEN ) << std::endl;
    }
}
