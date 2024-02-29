#pragma once

#include "../AST/AST.h"
#include "../Common/Logger.h"
#include <map>

struct TypeInfo
{
public:
	int typeSize;
	int typeAlignment;
	std::vector<std::string> fields;
};

struct QualifiedType
{
	bool isConstQual;
	bool isRefType;
	int indirections;
	std::string baseName;

	bool IsPtrType() const
	{
		return indirections > 0;
	}

	friend bool operator==(const QualifiedType& left, const QualifiedType& right)
	{
		return left.isConstQual == right.isConstQual &&
			left.isRefType == right.isRefType,
			left.baseName == right.baseName,
			left.indirections == right.indirections;
	}


	QualifiedType GetBaseType() const
	{
		return QualifiedType{ false, false, 0, baseName };
	}
};

struct SymbolInfo
{
	std::string name;
	QualifiedType returnType;
	std::vector<QualifiedType> argTypes;

};


template<LogSeverity logSeverity>
struct SAMessage : LogMessage
{
	std::string message;
	SAMessage(const std::string& str) : message(str), LogMessage(logSeverity) {}

	std::string GetStr()
	{
		return message;
	}
};

typedef SAMessage<LogSeverity::Info> SAInfo;
typedef SAMessage<LogSeverity::Warning> SAWarning;
typedef SAMessage<LogSeverity::Error> SAError;


class StaticAnalysis
{

	std::map<std::string, TypeInfo> TypeRegistry;
	std::map<std::string, SymbolInfo> SymbolRegistry;
	Logger* logger;
public:

	StaticAnalysis(Logger* logger);

	bool VerifyTree(AST* tree);
	bool EnumerateTypes(AST* tree);
	bool EnumerateSymbols(AST* tree);
	bool CheckTypes(ASTNode* children);

	bool Error(const std::string& msg)
	{
		logger->Log(new SAError{ msg });
		return false;
	}
	bool Warning(const std::string& msg)
	{
		logger->Log(new SAWarning{ msg });
		return true;
	}


	SymbolInfo CreateSymbolInfo(ASTNode* parent, NameAST* symbolName, FuncProtoAST* protoAST);
	QualifiedType CreateQualifiedType(QualifiedTypeAST* qualTypeAST);
};