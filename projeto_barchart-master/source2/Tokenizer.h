#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace simpleparser
{

	enum TokenType
	{
		WHITESPACE,
		OPERATOR,
		OPERAND,
		OPENING_SCOPE,
		CLOSING_SCOPE,
		
		//INTEGER_LITERAL,
		//IDENTIFIER,
		//DOUBLE_LITERAL,
		//STRING_LITERAL,
		//STRING_ESCAPE_SEQUENCE,
		//POTENCIAL_DOUBLE
	};

	class Token
	{
		public:
			enum TokenType nType;
			std::string nText;
			size_t mStartOffset{0};
			size_t mEndOffset{0};
			size_t mLineNumber{0};
	};

	class Tokenizer
	{
		public:
			std::vector<Token> tokenize (const std::string &);
			void endToken(Token &, std::vector <Token> &);
	};

	struct token2
	{
		enum symbol_t {OPERAND = 0, OPERATOR, OPENING_SPOCE, CLOSING_SCOPE};
		
		std::string value;
		symbol_t type;
		size_t at_col;

		explicit token2(std::string v = "", symbol_t tt = OPERAND, size_t atc = 0)
		: value{v} 
		, type {tt} 
		, at_col{atc}
		{/* --- */}
		
		friend std::ostream& operator<<(std::ostream & os, const token2& t)
		{
			std::string symbol_name[] = {"OPERAND", "OPERATOR", "OPENING_SCOPE", "CLOSING_SCOPE"};
			os << "<\"" << t.value << "\"." << symbol_name[t.type] << ">"; 
			return os;
		}
	};
}
