#include "Tokenizer.h"
//#include <stdexcept>
#include <iostream>
#include <string>
namespace simpleparser
{

	std::vector<Token> Tokenizer::tokenize( const std::string &inProgram)
	{
		std::vector<Token> tokens;
		Token currentToken;

		for (char currCh : inProgram)
		{
			if (currentToken.nType == STRING_ESCAPE_SEQUENCE)
			{
				switch(currCh)
				{
					case 'n':
						currentToken.nText.append(1, '\n');
						break;
					case 'r':
						currentToken.nText.append(1, '\r');
						break;
					case 't':
						currentToken.nText.append(1, '\t');
						break;
					case '\\':
						currentToken.nText.append(1, '\\');
						break;
					default:
						throw std::runtime_error(std::string("unknown espace sequence: ") + std::string(1,currCh) + 
						" in string on line " + std::to_string(currentToken.mLineNumber)+ ".");
					break;
				}

				currentToken.nType = STRING_LITERAL;
				continue;
			}
			
			switch(currCh)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if (currentToken.nType == WHITESPACE)
					{
						currentToken.nType = INTEGER_LITERAL;
						currentToken.nText.append(1, currCh);
					}else{
						currentToken.nText.append(1, currCh);
					}
					break;
				case '.':
					if (currentToken.nType == WHITESPACE)
					{
						currentToken.nType = POTENCIAL_DOUBLE;
						currentToken.nText.append(1, currCh);
					}
					else if(currentToken.nType == INTEGER_LITERAL)
					{
						currentToken.nType = DOUBLE_LITERAL;
						currentToken.nText.append(1, currCh);
					}
					else if(currentToken.nType == STRING_LITERAL)
					{
						currentToken.nText.append(1, currCh);
					}else{
						endToken(currentToken, tokens);
						currentToken.nType = OPERATOR;
						currentToken.nText.append(1, currCh);
						endToken(currentToken, tokens);
					}
					break;	
				//operadores
				case '(':
					currentToken.nType = OPENING_SCOPE;
					break;
				case ')':
					currentToken.nType = CLOSING_SCOPE;
					break;
				case '+':
				case '-':
				case '^':
				case '/':
				case ';':
				case ',':
				case '*':
					if(currentToken.nType != STRING_LITERAL)
					{
						endToken(currentToken, tokens);
						currentToken.nType = INTEGER_LITERAL;
						currentToken.nText.append(1, currCh);
					} else {
						endToken(currentToken, tokens);
						//currentToken.nText.append(1, currCh);
					}
					break;
				case ' ':
				case '\t':
					endToken(currentToken, tokens);
					break;
				case '\r':
				case '\n':
					endToken(currentToken, tokens);
					++currentToken.mLineNumber;
					break;
				case '"':
					if (currentToken.nType != STRING_LITERAL)
					{
						endToken(currentToken, tokens);
						currentToken.nType = STRING_LITERAL;
						currentToken.nText.append(1, currCh);
					} 
					else if(currentToken.nType == STRING_LITERAL)
					{
						endToken(currentToken, tokens);
					}
					break;
				case '\\':
					if (currentToken.nType == STRING_LITERAL)
					{
						currentToken.nType = STRING_ESCAPE_SEQUENCE;
					} else {
						endToken(currentToken, tokens);
						currentToken.nType = OPERATOR;
						currentToken.nText.append(1, currCh);
						endToken(currentToken, tokens);
					}
					break;
				default:
					if (currentToken.nType == WHITESPACE || currentToken.nType == INTEGER_LITERAL || 
						currentToken.nType == DOUBLE_LITERAL)
					{
						endToken(currentToken, tokens);
						currentToken.nText = IDENTIFIER;
						currentToken.nText.append(1, currCh);
					}else{
						currentToken.nText.append(1, currCh);
					}
					break;
			}
		}
	
	endToken(currentToken, tokens);
		
	return tokens;
	
	}

	void Tokenizer::endToken(Token &token, std::vector <Token> &tokens)
	{
		if(token.nType != WHITESPACE)
		{
			tokens.push_back(token);
		}	
		token.nType = WHITESPACE;
		token.nText.erase();
	}

	void Tokenizer::printToken() const
	{
		
	}


		friend std::ostream& operator<<(std::ostream & os, const Token& t)
		{
			std::string symbol_name[] = {"OPERAND", "OPERATOR", "OPENING_SCOPE", "CLOSING_SCOPE"};
			os << "<\"" << t.nText << "\"." << symbol_name[t.type] << ">"; 
			return os;
		}
}
