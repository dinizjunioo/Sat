#ifndef COMS_H
#define COMS_H

#include <iostream>
#include <iomanip> // setw()
#include <string>
#include <sstream>

#include "text_color.h"

#define SC SourceContext(__FILE__, __LINE__)

#define ERROR(msg) Error( msg, SC )
#define WARNING(msg) Warning( msg, SC )
#define MESSAGE(msg) Message( msg )

namespace  coms{
    // Holds context information for a warning or error while pre-processing scene file or executing the renderer.
    // Use macro SC to create one
    struct SourceContext 
    {
        const char* file;
        int line;
        SourceContext(const char* file="n/a", int line=0) : file(file), line(line) { }
        SourceContext& operator=( const SourceContext& ) = default;
        SourceContext( const SourceContext& clone) = default;
        ~SourceContext() = default;
    };

    void Error( const std::string&,  const SourceContext& );
    void Error( const std::string& );
    void Warning( const std::string&,  const SourceContext& );
    void Warning( const std::string& );
    void Message( const std::string& );
} // namespace coms

#endif
//-------------------------------------------------------------------------------
