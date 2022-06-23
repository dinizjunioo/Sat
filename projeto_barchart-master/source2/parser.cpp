

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <sstream>  // std::istringstream
#include <cstddef>  // std::ptrdiff_t
#include <cctype>   // std::isspace()

#include "parser.h"

/// Sets result with a error code.
/*! This method sets up the current error code.
 *  If an error has previously been set, it has priority and, therefore,
 *  it is kept.
 *
 *  However, if the `force_update` is true, only the error code is replaced
 *  (i.e. the original error location is kept).
 *  This happens if we need to provide a more accurate/informative
 *  error message for the client.
 *
 *  @param err_code The error code.
 *  @param force_update Flag that requires the error code update.
 */

void Parser::set_err_code(ResultType::code_t err_code, bool force_update) {
    // Update error code only if none is set or a force update is required.
    if (m_result.type == ResultType::OK or force_update) {
        // Store the column where the original error happened and do not update it.
        if (m_result.type == ResultType::OK)
            m_result.at_col = std::distance(m_exp.begin(), m_curr_symb);
        // Update error code.
        m_result.type = err_code;
    }
}

/// Converts the input character c_ into its corresponding terminal symbol code.
Parser::term_symb_t  Parser::lexer(char c) const {
    switch (c) {
    case '+':  return term_symb_t::TS_PLUS;
    case '-':  return term_symb_t::TS_MINUS;
    case ' ':  return term_symb_t::TS_WS;
    case   9:  return term_symb_t::TS_TAB;
    case '0':  return term_symb_t::TS_ZERO;
    case '1':  case '2':  case '3':  case '4':
    case '5':  case '6':  case '7':  case '8':
    case '9':  return term_symb_t::TS_NON_ZERO_DIGIT;
    case '\0': return term_symb_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return term_symb_t::TS_INVALID;
}

/// Consumes a valid character from the input expression.
void Parser::next_symbol(void) {
    std::advance(m_curr_symb, 1); // Same as m_it_curr_symb++;
}

/// Checks whether we reached the end of the input expression string.
bool Parser::end_input(void) const {
    return m_curr_symb == m_exp.end();
}

/// Returns the result of trying to match and consume the current character with c.
/*!
 * If the match is not successful, the current character (from the input) **is not consumed**.
 * @param c The current character we want to find & consume from the input source expression.
 * @return true if we got a successful match, false otherwise.
 *
 * @see expect(), next_symbol().
 */
bool Parser::accept(term_symb_t c) {
    // If we have a match, we consume the character from the input source expression.
    // if (not end_input() and lexer(*m_it_curr_symb) == c){
    if (lexer(*m_curr_symb) == c) {
        next_symbol();
        return true;
    }
    return false;
}

/// Tries to accept() the next valid character.
/*!
 * This method tries to accept the symbol.
 * This may fail if (1) we have a premature end of input; or,
 * (2) we receive from the expression a symbol that does not
 * match the symbol send as argument.
 *
 * In the case (1) the accept() sets the erro code there.
 * In the case (2) we set the error here by calling
 * set_err_code(). The error code is updated only if
 * the error member varible is set to OK (no error).
 *
 * @param c The symbol we are expecting.
 * @return true if the symbol is accepted, false otherwise.
 *
 * @see accept().
 */
bool Parser::expect(term_symb_t c) {
    if (not accept(c))
        set_err_code(ResultType::UNEXPECTED_SYMBOL);
    return m_result.type == ResultType::OK;
}

/// Ignores any white space or tabs in the expression until reach a valid character or end of input.
void Parser::skip_ws(void) {
    // Skip white spaces, while at the same time, check for end of string.
    while (not end_input() and std::isspace(*m_curr_symb))
        next_symbol();
}


//== Non Terminal Symbols (NTS) methods.

/// Validates (i.e. returns true or false) and consumes an **expression** from the input expression string.
/*!
 * This method parses a valid expression from the input and, at the same time, it tokenizes its components.
 *
 * Production rule is:
 * ```
 *  <expr> := <term>,{ <white_sp>, ("+"|"-"), <white_sp>, <term> };
 * ```
 * An expression might be just a term or one or more terms with '+'/'-' between them.
 */
bool Parser::expression(void) {
    // We must receive at least one valid term here.
    if (not term()) {
        set_err_code(ResultType::MISSING_TERM);
        return false;
    }
    // See if we've got more terms to follow.
    skip_ws();
    while (accept(Parser::term_symb_t::TS_MINUS) or
        accept(Parser::term_symb_t::TS_PLUS)) {
        skip_ws();
        if (not term())
            set_err_code(ResultType::MISSING_TERM);
        else skip_ws(); // Keep looking for more terms.
    }
    // Return true if everything ran smoothly.
    return m_result.type == ResultType::OK;
}
/// Validates (i.e. returns true or false) and consumes a **term** from the input expression string.
/*! This method parses and tokenizes a valid term from the input.
 *
 * Production rule is:
 * ```
 *  <term> := <integer>;
 * ```
 * A term is made of a single integer.
 * @return true if a term has been successfuly parsed from the input; false otherwise.
 */
bool Parser::term(void) {
    // We must receive an integer.
    if (not integer())
        set_err_code(ResultType::MISSING_TERM);
    return m_result.type == ResultType::OK;
}

/// Validates (i.e. returns true or false) and consumes an **integer** from the input expression string.
/*! This method parses a valid integer from the input and, at the same time, add the integer to the token list.
 *
 * Production rule is:
 * ```
 * <integer> := 0 | ["-"],<natural_number>;
 * ```
 * A integer might be a zero or a natural number, which, in turn, might begin with an unary minus.
 * @return true if an integer has been successfuly parsed from the input; false otherwise.
 */
bool Parser::integer(void) {
    // If we got a zero, the integert is done. go back.
    if (accept(term_symb_t::TS_ZERO))
        return true;
    // If this is a negative integer, let us consume the minus symbol as part of the number.
    accept(term_symb_t::TS_MINUS);
    // Retonar o resultado da tentativa de validar um numero natural.
    return  natural_number();
}

/// Validates (i.e. returns true or false) and consumes a **natural number** from the input string.
/*! This method parses a valid natural number from the input.
 *
 * Production rule is:
 * ```
 * <natural_number> := <digit_excl_zero>,{<digit>};
 * ```
 * @return true if a natural number has been successfuly parsed from the input; false otherwise.
 */
bool Parser::natural_number(void) {
    // Tem que vir um número que não seja zero! (de acordo com a definição).
    if (not digit_excl_zero())
        return false; // FAILED HERE.
    // Consume the remaining digit, if available...
    while (digit())
        ; /* empty */
    return m_result.type == ResultType::OK;
}

/// Validates (i.e. returns true or false) and consumes a **non-zero digit** from the input expression string.
/*! This method parses a single valid non-zero digit from the input.
 *
 * Production rule is:
 * ```
 * <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 * ```
 * @return true if a non-zero digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit_excl_zero(void) {
    return accept(term_symb_t::TS_NON_ZERO_DIGIT);
}

/// Validates (i.e. returns true or false) and consumes a **digit** from the input expression string.
/*! This method parses a single valid digit from the input.
 *
 * Production rule is:
 * ```
 * <digit> := "0"| <digit_excl_zero>;
 * ```
 * @return true if a digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit(void) {
    return (accept(term_symb_t::TS_ZERO) or digit_excl_zero()) ? true : false;
}
/*!
 * This is the parser's  **entry point** method that should be called in the client code.
 * This method tries to (recursivelly) validate an expression.
 *
 * e The string with the expression to parse.
 * @return The parsing result.
 *
 * @see ResultType
 */
Parser::ResultType  Parser::parse(std::string e) 
{
    m_exp = e; // Update the current input string.
    m_curr_symb = m_exp.begin(); // Defines the first char to be processed (consumed).
    m_result = ResultType{ ResultType::OK }; // Ok, by default,

    // Let us ignore any leading white spaces.
    skip_ws();
    if (end_input()) {
        // Ops, input has finished before we even started to parse...
        m_result = ResultType{ ResultType::PREMATURE_END_OF_INPUT,
                                std::distance(m_exp.begin(), m_curr_symb) };
    }
    else {
        // Trying to validate an expression.
        if (expression()) {
            // We only accept one expression per input string.
            // Therefore, at this point there should not be any non-whitespace
            // character in the input string.
            skip_ws(); // Clear any trailing 'whitespaces'.
            if (not end_input()) {// Is there still any residual symbol left in the string?
                m_result = ResultType{ ResultType::EXTRANEOUS_SYMBOL,
                                        std::distance(m_exp.begin(), m_curr_symb) };
            }
        }
    }
    return m_result;
}

//==========================[ End of parse.cpp ]==========================//
