#pragma once

#include <fstream>
#include <iostream>
#include <span>
#include <vector>

class CModule;
class CGValue;

typedef size_t ASTUUID;
typedef std::vector<class ASTNode*> ASTChildren;

class ASTNode
{
public:
    ASTUUID GetUUID() { return (size_t)this; }
    virtual CGValue* Codegen(CModule* module) = 0;
    virtual ASTChildren GetChildren() = 0;
    virtual std::string GetValues() { return ""; }
    virtual ~ASTNode()
    {
    }


    template<typename TASTType>
    TASTType* AsNodeType()
    {
        return dynamic_cast<TASTType*>(this);
    }
};
/*
class AtomAST : public ASTNode {

    ASTChildren GetChildren() { return ASTChildren{}; }
};

*/