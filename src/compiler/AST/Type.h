#include "Common.h"


class TypeAST : public ASTNode 
{
public:
	virtual NameAST* GetBaseTypeName() = 0;

};

class VoidTypeAST : public TypeAST
{
public:
	NameAST* GetBaseTypeName() override { return new NameAST{ {"void"} }; }
	ASTChildren GetChildren() { return ASTChildren{}; }
	CGValue* Codegen(CModule* module);
};

class QualifiedTypeAST : public TypeAST
{
public:
	int Indirections = 0;
	bool IsReference = false;
	NameAST* TypeName;

	QualifiedTypeAST(int indirs, bool isRef, NameAST* typeName) : Indirections(indirs), IsReference(isRef), TypeName(typeName)
	{

	}

	NameAST* GetBaseTypeName() override { return TypeName; }
	ASTChildren GetChildren() { return { TypeName }; }
	CGValue* Codegen(CModule* module);

};
