#ifndef OBFUSCATOR_PARSER_H
#define OBFUSCATOR_PARSER_H

#include "../common/types.h"
#include "../lexer/lexer.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Parser Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Parser State */
typedef struct {
    Token* tokens;
    Token* current_token;
    SymbolTable* symbol_table;
    Error* errors;
    int error_count;
} ParserState;

/* Function Prototypes */
ParserState* parser_create(Token* tokens);
void parser_destroy(ParserState* parser);

ASTNode* parser_parse(ParserState* parser);
ASTNode* parser_parse_program(ParserState* parser);
ASTNode* parser_parse_declaration(ParserState* parser);
ASTNode* parser_parse_function(ParserState* parser);
ASTNode* parser_parse_variable(ParserState* parser);
ASTNode* parser_parse_statement(ParserState* parser);
ASTNode* parser_parse_expression(ParserState* parser);
ASTNode* parser_parse_primary(ParserState* parser);
ASTNode* parser_parse_block(ParserState* parser);
ASTNode* parser_parse_if(ParserState* parser);
ASTNode* parser_parse_while(ParserState* parser);
ASTNode* parser_parse_for(ParserState* parser);
ASTNode* parser_parse_function_from_name(ParserState* parser, char* name, SourceLocation loc);
ASTNode* parser_parse_variable_from_name(ParserState* parser, char* name, SourceLocation loc);

bool parser_has_errors(const ParserState* parser);
Error* parser_get_errors(const ParserState* parser);

/* AST Node Management */
ASTNode* ast_node_create(NodeType type, SourceLocation location);
void ast_node_destroy(ASTNode* node);
void ast_tree_destroy(ASTNode* root);

/* Utility Functions */
Token* parser_advance(ParserState* parser);
Token* parser_peek(ParserState* parser);
bool parser_match(ParserState* parser, TokenType type);
bool parser_consume(ParserState* parser, TokenType type, const char* error_msg);

#endif /* OBFUSCATOR_PARSER_H */