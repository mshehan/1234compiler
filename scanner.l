%{
#include "lyutils.h"
#include "auxlib.h"


#define YY_USER_ACTION  { lexer::advance(); }

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn
/*%option verbose*/

LETTER          [A-Za-z_]
DIGIT           [0-9]
MANTISSA        ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)
EXPONENT        ([Ee][+-]?{DIGIT}+)
NUMBER          ({DIGIT}+)
CHARCON         ('([^\\'\n]|\\[\\'"0nt])')
STRINGCON       (\"([^\\"\n]|\\[\\'"0nt])*\")
IDENT           ({LETTER}({LETTER}|{DIGIT})*)

%%

"#".*           { lexer::include(); }
[ \t]+          { }
\n              { lexer::newline(); }
"void"          { return lexer::yylval_token (TOK_VOID); }
"int"           { return lexer::yylval_token (TOK_INT); }
"string"        { return lexer::yylval_token (TOK_STRING); }
"struct"        { return lexer::yylval_token (TOK_STRUCT); }
"if"            { return lexer::yylval_token (TOK_IF); }
"else"          { return lexer::yylval_token (TOK_ELSE); }
"while"         { return lexer::yylval_token (TOK_WHILE); }
"return"        { return lexer::yylval_token (TOK_RETURN); }
"null"          { return lexer::yylval_token (TOK_NULL); }
"new"           { return lexer::yylval_token (TOK_NEW); }

"=="            { return lexer::yylval_token (TOK_EQ); }
"!="            { return lexer::yylval_token (TOK_NE); }
"<="            { return lexer::yylval_token (TOK_LE); }
">="            { return lexer::yylval_token (TOK_GE); }

{IDENT}         { return lexer::yylval_token (TOK_IDENT); }
{NUMBER}        { return lexer::yylval_token (TOK_INTCON); }
{CHARCON}       { return lexer::yylval_token (TOK_CHARCON); }
{STRINGCON}     { return lexer::yylval_token (TOK_STRINGCON); }

"[]"            { return lexer::yylval_token (TOK_NEWARRAY); }
"("             { return lexer::yylval_token ('('); }
")"             { return lexer::yylval_token (')'); }
"["             { return lexer::yylval_token ('['); }
"]"             { return lexer::yylval_token (']'); }
"{"             { return lexer::yylval_token ('{'); }
"}"             { return lexer::yylval_token ('}'); }
";"             { return lexer::yylval_token (';'); }
","             { return lexer::yylval_token (','); }
"."             { return lexer::yylval_token ('.'); }
"="             { return lexer::yylval_token ('='); }
"<"             { return lexer::yylval_token (TOK_GT); }
">"             { return lexer::yylval_token (TOK_LT); }
"+"             { return lexer::yylval_token ('+'); }
"-"             { return lexer::yylval_token ('-'); }
"*"             { return lexer::yylval_token ('*'); }
"/"             { return lexer::yylval_token ('/'); }
"%"             { return lexer::yylval_token ('%'); }
"!"             { return lexer::yylval_token ('!'); }

.               { lexer::badchar (*yytext); }

%%

