#pragma once


class Evaluator
{
    public:
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

	private:
	
};