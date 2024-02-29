#include "Parser.h"

PResult<TypeAST> Parser::ParseType()
{
	if (tokens.Match(TokenType::KwVoid))
		return new VoidTypeAST{};

	NameAST* name = ParseName(true);
	if (!name)
		return NonMatch();

	int indirections = 0;
	bool isReference = false;

	while (tokens.Match(TokenType::OpAsterisk))
	{
		indirections++;
	}
	if (tokens.Match(TokenType::OpAmpersand))
	{
		isReference = true;
	}
	return new QualifiedTypeAST{ indirections, isReference, name };
}