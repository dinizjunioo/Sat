#include <iostream>
#include <iomanip>
#include <vector>

#include "parser.h"
//#include "tokenizer.h"
#include "Evaluator.h"

void print_parser_err_msg(const Parser::ResultType& result, const std::string& str) {
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

void print_evaluate_err_msg(const Evaluator::ResultType& result) 
{
    //switch (er.type)
    //{
    //case Evaluator::ResultType::INTEGER_OUT_OF_RANGE:
    //    std::cout << "Integer constant out of range beginning at column ( " << er.at_col + 1 << ")!\n";
    //    break;
    //case Evaluator::ResultType::NUMERIC_OVERFLOW_ERROR:
    //    std::cout << "Numeric overflow error!\n";
    //    break;
    //case Evaluator::ResultType::DIVISION_BY_ZERO:
    //    std::cout << "Division by zero!\n";
    //    break;
    //case Evaluator::ResultType::VALUE_UNDEFINED:
    //    std::cout << "Undefined value!\n";
    //   break;
    //}
}

int main()
{
    Parser parser;
    Evaluator evaluator;

    std::string expression;

    while (getline(std::cin, expression)) 
    {
        auto parser_result = parser.parse(expression);

        if (parser_result.type != Parser::ResultType::OK)
        {
            //print_parser_err_msg(parser_result); // falta um argumento que é onde está o error! (opcional)
            continue;
        }
        //std::cout << ">>> Expression SUCESSFULLY parsed! \n";

        char unary_minus;

        parser.get_expression(expression, unary_minus);

        auto token_list = tokenize(expression, unary_minus);

        /*
        std::cout << ">>> tokens: { ";
        std::copy(token_list.begin(), token_list.end(), std::ostream_iterator<Token>(std::cout, " "));
        std::cout << "}\n";
        std::cout << std::endl;
        */

        auto evaluate_result = evaluator.evaluate(token_list);

        if (evaluate_result.type != Evaluator::ResultType::OK)
        {
            print_evaluate_err_msg(evaluate_result);
            continue;
        }

        std::cout << evaluate_result.value << std::endl;

        expression.clear();
    }
    return EXIT_SUCCESS;
}

