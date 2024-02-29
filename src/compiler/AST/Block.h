#pragma once

class StmntAST;

class BlockAST : public ASTNode
{
public:
    std::vector<StmntAST*> Statements;
    BlockAST(const std::vector<StmntAST*>& stmnts) : Statements(stmnts)
    {
    }
    ~BlockAST()
    {
    }

    ASTChildren GetChildren();
    CGValue* Codegen(CModule* module);
};