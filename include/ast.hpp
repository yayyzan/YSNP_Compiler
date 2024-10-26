#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_Node.hpp"
#include "ast/ast_Primitives.hpp"
#include "ast/ast_Operators.hpp"
#include "ast/ast_Unary.hpp"
#include "ast/ast_Return.hpp"
#include "ast/ast_Type.hpp"
#include "ast/ast_FDefine.hpp"
#include "ast/ast_FDeclare.hpp"
#include "ast/ast_VDeclaration.hpp"
#include "ast/ast_VInitializer.hpp"
#include "ast/ast_FScope.hpp"
#include "ast/ast_IfElseStatement.hpp"
#include "ast/ast_WhileStatement.hpp"
#include "ast/ast_ForStatement.hpp"
#include "ast/ast_context.hpp"
#include "ast/ast_FunctionCall.hpp"
#include "ast/ast_Parameter.hpp"
#include "ast/ast_ParameterList.hpp"
#include "ast/ast_ADeclare.hpp"
#include "ast/ast_VDeclarator.hpp"
#include "ast/ast_AInitializer.hpp"
#include "ast/ast_ManipulateArray.hpp"
#include "ast/ast_Enumerator.hpp"
#include "ast/ast_VEnumerate.hpp"
#include "ast/ast_Case.hpp"
#include "ast/ast_Switch.hpp"
#include "ast/ast_Default.hpp"
#include "ast/ast_Continue.hpp"
#include "ast/ast_Break.hpp"
#include "ast/ast_Pointer.hpp"
#include "ast/ast_Dereference.hpp"
#include "ast/ast_Address.hpp"
#include "ast/ast_SizeOf.hpp"



extern Node *parseAST();

#endif
