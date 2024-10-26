%code requires{
	#include "ast.hpp"

	#include <cassert>
	#include <vector>

    extern Node *g_root;

    int yylex(void);
    void yyerror(const char *);
}

%union{
    NodePointer node;
	std::vector<NodePointer> *list;
    int val;
	std::string *string;
}

%token IDENTIFIER INTEGER
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR INT SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM SIZEOF

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR CONTINUE BREAK RETURN

%type<node>  external_declaration function_definition
%type<node> declaration_specifiers declarator direct_declarator
%type<node> compound_statement statement assignment_expression
%type<node> jump_statement expression conditional_expression enumerator
%type<node> logical_or_expression logical_and_expression enum_specifier
%type<node> inclusive_or_expression exclusive_or_expression constant_expression
%type<node> and_expression equality_expression relational_expression
%type<node> shift_expression additive_expression multiplicative_expression
%type<node> cast_expression unary_expression postfix_expression parameter_declaration
%type<node> primary_expression declaration init_declarator expression_statement labeled_statement
%type<node> initializer iteration_statement selection_statement parameter_type_list
%type<list> declaration_list statement_list translation_unit parameter_list argument_expression_list initializer_list enumerator_list
%type<val> INTEGER
%type<string> IDENTIFIER INT ENUM UNSIGNED CHAR type_specifier

%start ROOT

%%

ROOT : translation_unit { g_root = new FScope(*$1); }

translation_unit
	: external_declaration { $$ = new std::vector<NodePointer> {$1}; }
	| translation_unit external_declaration { $$ = $1; ($1)->push_back($2); }
	;

external_declaration
	: function_definition { $$ = $1; }
	| declaration { $$ = $1; }
	;

function_definition
	: declaration_specifiers declarator compound_statement { $$ = new FDefine($1, $2, $3); }
	;

parameter_type_list
	: parameter_list { $$ = new ParameterList(*$1); }
	;

parameter_list
	: parameter_declaration { $$ = new std::vector<NodePointer> {$1}; }
	| parameter_list ',' parameter_declaration { $$ = $1; ($1)->push_back($3); }
	;

parameter_declaration
	: declaration_specifiers declarator { $$ = new Parameter($1, $2); }
	;

declaration_specifiers
	: INT { $$ = new Type(*$1); }
	| enum_specifier { $$ = new Type($1); }
	| UNSIGNED { $$ = new Type(*$1); }
	| CHAR { $$ = new Type(*$1); }
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { $$ = new Enumerator(*$3); }
	| ENUM IDENTIFIER '{' enumerator_list '}' { $$ = new Enumerator(*$4); }
	| ENUM IDENTIFIER { $$ = new Enumerator();}
	;

enumerator_list
	: enumerator  { $$ = new std::vector<NodePointer> {$1}; }
	| enumerator_list ',' enumerator  { $$ = $1; ($1)->push_back($3); }
	;

enumerator
	: IDENTIFIER  { $$ = new VEnumerate(*$1); }
	| IDENTIFIER '=' constant_expression { $$ = new VEnumerate(*$1, $3); }
	;

type_specifier
	: CHAR{$$ = new std::string("char");}
	| INT { $$ = new std::string("int"); }
	| FLOAT {$$ = new std::string("float");}
	| DOUBLE{$$ = new std::string("double");}
	| UNSIGNED{$$ = new std::string("unsigned");}
	| enum_specifier{$$ = new std::string("enum");}
	;

declarator
	: pointer direct_declarator {$$= new Pointer($2); }
	| direct_declarator { $$ = $1; }
	;

pointer
	: '*'
	;

direct_declarator
	: IDENTIFIER { $$ = new Identifier(*$1); }
	| direct_declarator '[' constant_expression ']' { $$ = new ADeclare($1, $3); }
	| direct_declarator '[' ']' { $$ = new ADeclare($1); }
	| direct_declarator '(' parameter_type_list ')' { $$ = new FDeclare($1, $3); }
	| direct_declarator '(' ')' { $$ = new FDeclare($1); }
	;

argument_expression_list
	:assignment_expression { $$ = new std::vector<NodePointer> {$1}; }
	| argument_expression_list ',' assignment_expression { $$ = $1; ($1)->push_back($3); }
	;

compound_statement
	: '{' '}' { $$ = new FScope(); }
	| '{' statement_list '}' { $$ = new FScope(*$2); }
	| '{' declaration_list '}'{ $$ = new FScope(*$2); }
	| '{' declaration_list statement_list '}' { $$ = new FScope(*$2, *$3); }
	;

statement_list
	: statement { $$ = new std::vector<NodePointer> {$1}; }
	| statement_list statement { $$ = $1; ($1)->push_back($2); }
	;

statement
	: labeled_statement { $$ = $1; }
	| compound_statement { $$ = $1; }
	| expression_statement { $$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1; }
	;

jump_statement
	: CONTINUE ';' { $$ = new Continue(); }
	| BREAK ';' { $$ = new Break(); }
	| RETURN expression ';' { $$ = new Return($2); }
	;

selection_statement
	: IF '(' expression ')' statement { $$ = new IfElseStatement($3, $5); }
	| IF '(' expression ')' statement ELSE statement { $$ = new IfElseStatement($3, $5, $7); }
	| SWITCH '(' expression ')' statement { $$ = new Switch($3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = new WhileStatement($3, $5); }
	| DO statement WHILE '(' expression ')' ';' { $$ = new WhileStatement($5, $2); }
	| FOR '(' expression_statement expression_statement ')' statement {$$ = new ForStatement($3,$4,$6);}
	| FOR '(' expression_statement expression_statement expression ')' statement {$$ = new ForStatement($3,$4,$5,$7);}
	;

labeled_statement
	: CASE constant_expression ':' statement { $$ = new Case($2, $4); }
	| DEFAULT ':' statement { $$ = new Default($3); }
	;


expression_statement
	: ';' { }
	| expression ';' { $$ = $1; }
	;

declaration_list
	: declaration { $$ = new std::vector<NodePointer> {$1}; }
	| declaration_list declaration { $$ = $1; ($1)->push_back($2); }
	;

declaration
	: declaration_specifiers ';' { $$ = $1; }
	| declaration_specifiers init_declarator ';' { $$ = new VDeclaration($1, $2); }
	;

init_declarator
	: declarator { $$ = new VDeclarator($1); }
	| declarator '=' initializer { $$ = new VInitializer($1, $3); }
	;

initializer
	: expression { $$ = $1; }
	| '{' initializer_list '}' { $$ = new AInitializer(*$2); }
	| '{' initializer_list ',' '}' { $$ = new AInitializer(*$2); }
	;

initializer_list
	: initializer { $$ = new std::vector<NodePointer> {$1}; }
	| initializer_list ',' initializer { $$ = $1; ($1)->push_back($3); }
	;

expression
	: assignment_expression { $$ = $1; }
	;

assignment_expression
	: conditional_expression { $$ = $1; }
	| unary_expression '=' assignment_expression { $$ = new Assign($1, $3); }
	| unary_expression DIV_ASSIGN assignment_expression { $$ = new Assign($1, new DivideOperator($1, $3)); }
	| unary_expression MUL_ASSIGN assignment_expression { $$ = new Assign($1, new MultiplyOperator($1, $3)); }
	| unary_expression MOD_ASSIGN assignment_expression { $$ = new Assign($1, new ModuloOperator($1, $3)); }
	| unary_expression ADD_ASSIGN assignment_expression { $$ = new Assign($1, new AddOperator($1, $3)); }
	| unary_expression SUB_ASSIGN assignment_expression { $$ = new Assign($1, new SubtractOperator($1, $3)); }
	| unary_expression LEFT_ASSIGN assignment_expression { $$ = new Assign($1, new LeftShiftOperator($1, $3)); }
	| unary_expression RIGHT_ASSIGN assignment_expression { $$ = new Assign($1, new RightShiftOperator($1, $3)); }
	| unary_expression AND_ASSIGN assignment_expression { $$ = new Assign($1, new BAndOperator($1, $3)); }
	| unary_expression XOR_ASSIGN assignment_expression { $$ = new Assign($1, new BXorOperator($1, $3)); }
	| unary_expression OR_ASSIGN assignment_expression { $$ = new Assign($1, new BOrOperator($1, $3)); }
	;

constant_expression
	: conditional_expression { $$ = $1; }
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression { $$ = new IfElseStatement($1, $3, $5); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression { $$ = new LOrOperator($1, $3); }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression { $$ = new LAndOperator($1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression { $$ = new BOrOperator($1, $3); }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression { $$ = new BXorOperator($1, $3); }
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression { $$ = new BAndOperator($1, $3); }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQ_OP relational_expression { $$ = new EQOperator($1, $3); }
	| equality_expression NE_OP relational_expression { $$ = new NEQOperator($1, $3); }
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression { $$ = new LTOperator($1, $3); }
	| relational_expression '>' shift_expression { $$ = new GTOperator($1, $3); }
	| relational_expression LE_OP shift_expression { $$ = new LTEQOperator($1, $3); }
	| relational_expression GE_OP shift_expression { $$ = new GTEQOperator($1, $3); }
	;

shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression { $$ = new LeftShiftOperator($1, $3); }
	| shift_expression RIGHT_OP additive_expression { $$ = new RightShiftOperator($1, $3); }
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = new AddOperator($1, $3); }
	| additive_expression '-' multiplicative_expression { $$ = new SubtractOperator($1, $3); }
	;

multiplicative_expression
	: cast_expression { $$ = $1; }
	| multiplicative_expression '*' cast_expression { $$ = new MultiplyOperator($1, $3); } // 2*1
	| multiplicative_expression '/' cast_expression { $$ = new DivideOperator($1, $3); } // 2/1
	| multiplicative_expression '%' cast_expression { $$ = new ModuloOperator($1, $3); } // 2%1
	;

cast_expression
	: unary_expression { $$ = $1; }
	;

unary_expression
	: postfix_expression { $$ = $1; }
	| INC_OP unary_expression { $$ = new PreIncrement($2); } // ++foo
	| DEC_OP unary_expression { $$ = new PreDecrement($2); } // --foo
	| '-' cast_expression { $$ = new NegOperator($2); } // -foo
	| '*' cast_expression { $$ = new Dereference($2); }
	| '&' cast_expression { $$ = new Address($2); }
	| SIZEOF unary_expression {$$ = new SizeOf($2);}
	| SIZEOF '(' type_specifier ')'{$$ = new SizeOf(*$3);}
	;

postfix_expression
	: primary_expression { $$ = $1; }
	| postfix_expression '[' expression ']' { $$ = new ManipulateArray($1, $3); }
	| postfix_expression '(' ')' { $$ = new FunctionCall($1); }
	| postfix_expression '(' argument_expression_list ')' { $$ = new FunctionCall($1, *$3); }
	| postfix_expression INC_OP { $$ = new PostIncrement($1); } // foo++
	| postfix_expression DEC_OP { $$ = new PostDecrement($1); } // foo--
	;

primary_expression
	: IDENTIFIER { $$ = new Identifier(*$1); delete $1; } // foo
	| INTEGER { $$ = new Integer($1); } // 3
	| '(' expression ')' { $$ = $2; } // (2+1)
	;

/*

NOT IMPLEMENTED

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;
abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	: '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

*/

%%

 Node *g_root;

 Node *parseAST()
{
  g_root = 0;
  yyparse();
  return g_root;
}
