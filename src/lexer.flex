%option noyywrap

%{
extern "C" int fileno(FILE *stream);

#include "parser.tab.hpp"
%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS		(f|F|l|L)
IS		(u|U|l|L)*

%%

"int"  { yylval.string = new std::string(yytext); return(INT); }
"unsigned" { yylval.string = new std::string(yytext); return(UNSIGNED); }
"float" {return (FLOAT);}
"double" {return (DOUBLE);}
"return"  {return(RETURN);}
"if"    {return(IF);}
"else"    {return(ELSE);}
"do"      {return(DO);}
"while"   {return (WHILE);}
"for"      {return (FOR);}
"enum"    {return(ENUM);}
"default" {return(DEFAULT);}
"switch" {return(SWITCH);}
"case"   {return(CASE);}
"break"  {return(BREAK);}
"continue" {return(CONTINUE);}
"char" { yylval.string = new std::string("int"); return(CHAR); }
"sizeof"  {return(SIZEOF);}

{L}({L}|{D})*		    { yylval.string = new std::string(yytext); return(IDENTIFIER); }

L?'(\\.|[^\\'])+'	  { if(strlen(yytext) == 3){yylval.val = (int)yytext[1];} else if (strlen(yytext) == 4){switch(yytext[2]){
                                                                                                            case 'n': yylval.val = 10; break;
                                                                                                            case '"': yylval.val = 34; break;
                                                                                                            case '?': yylval.val = 63; break;
                                                                                                            case 'a': yylval.val = 7; break;
                                                                                                            case 'b': yylval.val = 8; break;
                                                                                                            case 'f': yylval.val = 12; break;
                                                                                                            case 'r': yylval.val = 13; break;
                                                                                                            case 't': yylval.val = 9; break;
                                                                                                            case 'v': yylval.val = 11; break;
                                                                                                          }}
                                                                                                          return(INTEGER); }
{D}+{IS}?		        { yylval.val = atoi(yytext); return(INTEGER); }

">>="			{ return(RIGHT_ASSIGN); }
"<<="			{ return(LEFT_ASSIGN); }
"+="			{ return(ADD_ASSIGN); }
"-="			{ return(SUB_ASSIGN); }
"*="			{ return(MUL_ASSIGN); }
"/="			{ return(DIV_ASSIGN); }
"%="			{ return(MOD_ASSIGN); }
"&="			{ return(AND_ASSIGN); }
"^="			{ return(XOR_ASSIGN); }
"|="			{ return(OR_ASSIGN); }
">>"			{ return(RIGHT_OP); }
"<<"			{ return(LEFT_OP); }
"++"			{ return(INC_OP); }
"--"			{ return(DEC_OP); }
"&&"			{ return(AND_OP); }
"||"			{ return(OR_OP); }
"<="			{ return(LE_OP); }
">="			{ return(GE_OP); }
"=="			{ return(EQ_OP); }
"!="			{ return(NE_OP); }
";"			{ return(';'); }
("{"|"<%")		{ return('{'); }
("}"|"%>")		{ return('}'); }
","			{ return(','); }
":"			{ return(':'); }
"="			{ return('='); }
"("			{ return('('); }
")"			{ return(')'); }
("["|"<:")		{ return('['); }
("]"|":>")		{ return(']'); }
"."			{ return('.'); }
"&"			{ return('&'); }
"!"			{ return('!'); }
"~"			{ return('~'); }
"-"			{ return('-'); }
"+"			{ return('+'); }
"*"			{ return('*'); }
"/"			{ return('/'); }
"%"			{ return('%'); }
"<"			{ return('<'); }
">"			{ return('>'); }
"^"			{ return('^'); }
"|"			{ return('|'); }
"?"			{ return('?'); }


[ \t\v\n\f]		{ ; }

.			    { }

%%

void yyerror (char const *s)
{
  fprintf (stderr, "ERROR LEXER : %s\n", s);
  exit(1);
}
