#include <iostream>
#include <iomanip>
#include <vector>

#include "parser.h"

/// List of expressions to evaluate and tokenize.
std::vector<std::string> expressions = {
    "10",
    "32767 - 42768 + 8",
    "5 + -32766",
    "5 + -32769",
    "12 + 3",
    "-3+-5+-6",
    "12 + 3     -3 + -34 ",
    "0",

    "    12    +    4   8",
    "1.3 * 4",
    "12 + + 5",
    "+12",
    "4 + ",
    "4 + 03",
    "a + 4",
    "       ",
    "  123 *  548",
    "32a23",
    "43 + 54 -   "
};

/// Send to the standard output the proper error messages.
void print_error_msg(const Parser::ResultType& result, const std::string& str) {
    std::string error_indicator(str.size() + 1, ' ');
    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch (result.type) {
    case Parser::ResultType::PREMATURE_END_OF_INPUT:
        std::cout << ">>> Premature end of input at column (" << result.at_col << ")!\n";
        break;
    case Parser::ResultType::MISSING_TERM:
        std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
        break;
    case Parser::ResultType::EXTRANEOUS_SYMBOL:
        std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
        break;
    case Parser::ResultType::ILL_FORMED_INTEGER:
        std::cout << ">>> Ill formed interger located at column (" << result.at_col << ")!\n";
        break;
    default:
        std::cout << ">>> Unhandled error found!\n";
        break;
    }
    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
}


int main(void) {
    Parser my_parser; // This is the parser.
    // Parse each expression we have.
    for (const auto& e : expressions) {
        // [I] Do parsing here.
        auto result = my_parser.parse(e);
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << e << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if (result.type != Parser::ResultType::OK)
            print_error_msg(result, e);
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
        std::cout << std::endl;
    }
    std::cout << "\n>>> Normal exiting...\n";
    return EXIT_SUCCESS;
}
