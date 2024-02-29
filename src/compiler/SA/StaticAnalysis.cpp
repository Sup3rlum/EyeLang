#include "StaticAnalysis.h"


StaticAnalysis::StaticAnalysis(Logger* logger) : logger(logger)
{
	TypeRegistry["int"] = TypeInfo{};
	TypeRegistry["float"] = TypeInfo{};
	TypeRegistry["char"] = TypeInfo{};
}

bool StaticAnalysis::VerifyTree(AST* tree)
{
	return true;// EnumerateTypes(tree);
}


bool StaticAnalysis::EnumerateTypes(AST* tree)
{
	/* Check if all types reference encountered type definitions */
	for (auto moduleStmnt : tree->Statements)
	{
		if (auto structStmnt = moduleStmnt->AsNodeType<StructDefAST>())
		{
			/* Check field types */
			for (auto structField : structStmnt->Fields)
			{
				auto baseName = structField->TypeName->GetBaseTypeName()->GetBaseName();
				if (TypeRegistry.find(baseName) == TypeRegistry.end())
				{
					return Error("Unrecognized label '" + baseName + "'");
				}
			}

			/* Check internal expressions and statements */
			for (auto memberFunction : structStmnt->Members)
			{
				if (!CheckTypes(memberFunction))
					return false;
			}

			auto typeName = structStmnt->StructName->GetBaseName();
			TypeRegistry[typeName] = TypeInfo{};
		}
	}
}

bool StaticAnalysis::EnumerateSymbols(AST* tree)
{
	for (auto moduleStmnt : tree->Statements)
	{
		if (auto moduleFuncStmnt = moduleStmnt->AsNodeType<MemberFuncDefAST>())
		{

		}
		if (auto structStmnt = moduleStmnt->AsNodeType<StructDefAST>())
		{

			/* Check internal expressions and statements */
			for (auto memberFunction : structStmnt->Members)
			{
				if (!CheckTypes(memberFunction))
					return false;
			}

			auto typeName = structStmnt->StructName->GetBaseName();
			TypeRegistry[typeName] = TypeInfo{};
		}
	}
}

bool StaticAnalysis::CheckTypes(ASTNode* node)
{
	if (auto typeName = node->AsNodeType<TypeAST>())
	{
		auto baseName = typeName->GetBaseTypeName()->GetBaseName();
		if (TypeRegistry.find(baseName) == TypeRegistry.end())
		{
			return Error("Unrecognized label '" + baseName + "'");
		}
	}
	else
	{
		for (auto child : node->GetChildren())
		{
			if (!CheckTypes(child))
				return false;
		}
	}
}

SymbolInfo StaticAnalysis::CreateSymbolInfo(ASTNode* parent, NameAST* symbolName, FuncProtoAST* protoAST)
{
	if (!parent)
	{
		
	}
	else if (auto structStmnt = parent->AsNodeType<StructDefAST>())
	{

	}

	return SymbolInfo{};
}

QualifiedType StaticAnalysis::CreateQualifiedType(QualifiedTypeAST* qualTypeAST)
{
	return QualifiedType
	{ 
		true, 
		qualTypeAST->IsReference, 
		qualTypeAST->Indirections, 
		qualTypeAST->GetBaseTypeName()->GetBaseName()
	};
}