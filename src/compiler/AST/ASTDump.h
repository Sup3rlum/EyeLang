#include "AST.h"
#include "../Util/string_util.h"
#include "../Util/console_helper.h"

class ASTDump
{
private:
	std::string Tabs(int level)
	{
		std::string out = "";
		for (int i = 0; i < level; i++)
		{
			out += "  ";
		}
		return out;
	}
public:
	std::string ToString(ASTNode* ast, int level, std::string& current)
	{
		if (ast)
		{
			for (auto childNode : ast->GetChildren())
			{
				auto info = std::string(typeid(*childNode).name());
				auto strs = SplitString(info, ' ');
				auto values = childNode->GetValues();
				std::cout << Tabs(level + 1) << "[" << strs[1] << "]: " << CText<FG_LIGHT_YELLOW>(values) << std::endl;
				ToString(childNode, level + 1, current);
			}
		}
		return current;
	}
};