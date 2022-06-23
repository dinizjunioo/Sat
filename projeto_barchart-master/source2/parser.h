#ifndef PARSER_H
#define PARSER_H

#include <string>

/// This class implements a recursive descent parser.
/*!
 * This class implements a recursive descendent parser that validates expressions
 * according to a EBNF grammar.
 *
 * The grammar is:
 * ```
 *   <expr>            := <term>,{ <white_sp>, ("+"|"-"), <white_sp>, <term> };
 *   <term>            := <integer>;
 *   <integer>         := "0" | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 *   <white_sp>        := ? white space chacaracters ?
 * ```
 */
class Parser
{
public:
    /// This struct represents the result of the parsing operation.
    struct ResultType {
        //== Alias
        typedef std::ptrdiff_t size_type; //!< Used for column location determination.

        /// List of possible syntax errors.
        enum code_t {
            OK = 0,                       //!< Expression successfuly parsed.
            PREMATURE_END_OF_INPUT,       //!< Unexpected end of expression.
            UNEXPECTED_SYMBOL,            //!< Unexpected symbol detected.
            MISSING_TERM,                 //!< Missing term in expression.
            EXTRANEOUS_SYMBOL,            //!< Unexpected symbol in expression.
            ILL_FORMED_INTEGER,           //!< Ill formed integer.
        };

        //== Members (public).
        code_t type;      //!< Error code.
        size_type at_col; //!< Stores the column number where the error happened.

        /// Default contructor.
        /*!
         * @type The error code (enumeration).
         * @col The original column in which the error was detected.
        */
        ResultType(code_t type = OK, size_type col = 0u)
            : type{ type }
            , at_col{ col }
        { /* empty */ }
    };

    //== Public interface
    /// Parses an input source expression.
    /*!
     * This is the method that starts the parsing process.
     * It returns the result of the parsing, with an error
     * message in case something error occurred while parsin
     * the expressio.
     * The result object also contains the column in which
     * the error was detected.
     * @exp The expresion we which to parse.
     */
    ResultType parse(std::string exp);

    //== Special methods
    /// Default constructor
    Parser() = default;
    /// Default destructor
    ~Parser() = default;
    /// Turn off copy constructor. We do not need it.
    Parser(const Parser&) = delete;  // Construtor c�pia.
    /// Turn off assignment operator.
    Parser& operator=(const Parser&) = delete; // Atribui��o.

    void get_expression(std::string, char);
    
private:
    /// Terminal symbols table
    enum class term_symb_t {  // The symbols:-
        TS_PLUS,            //!< code for "+"
        TS_MINUS,           //!< code for "-"
        TS_ZERO,            //!< code for "0"
        TS_NON_ZERO_DIGIT,  //!< code for digits, from "1" to "9"
        TS_WS,              //!< code for a white-space
        TS_TAB,             //!< code for tab
        TS_EOS,             //!< code for "End Of String"
        TS_INVALID          //!< invalid token
    };

    //== Private members.
    std::string m_exp;                      //!< The source expression to be parsed
    std::string::iterator m_curr_symb;   //!< Pointer to the current char inside the expression.
    ResultType m_result;                    //!< The result for the current expression (either error of OK).

    //== Support parser methods.
    term_symb_t lexer(char c) const;  // Get the corresponding code for a given input char.
    void next_symbol(void);           // Skips any WS and advances iterator to the next char in the expression.
    bool accept(term_symb_t c);       // Tries to accept the requested symbol. No error generated in case of failling to comply.
    bool expect(term_symb_t c);       // Expecting symbol `c`; if it did not come, then it's an error.
    void skip_ws(void);               // Skips any leading WS and stops at the next non-WS character.
    bool end_input(void) const;       // Checks whether we reached the end of the original expression string.
    void set_err_code(ResultType::code_t, bool = false); // Sets up the error code.

    //== Non terminal symbols (NTS) methods.
    bool expression();
    bool term();
    bool integer();
    bool natural_number();
    bool digit_excl_zero();
    bool digit();
};


#endif

