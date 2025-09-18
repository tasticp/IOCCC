#ifndef OBFUSCATOR_LEXER_H
#define OBFUSCATOR_LEXER_H

#include "../common/types.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Lexical Analyzer Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Lexer State */
typedef struct {
    const char* source;
    size_t position;
    size_t length;
    int line;
    int column;
    const char* filename;
    Token* tokens;
    Token* current_token;
    Error* errors;
} LexerState;

/* Function Prototypes */
LexerState* lexer_create(const char* source, const char* filename);
void lexer_destroy(LexerState* lexer);

Token* lexer_tokenize(LexerState* lexer);
Token* lexer_next_token(LexerState* lexer);
Token* lexer_peek_token(LexerState* lexer);

bool lexer_has_errors(const LexerState* lexer);
Error* lexer_get_errors(const LexerState* lexer);
void lexer_clear_errors(LexerState* lexer);

/* Token Management */
Token* token_create(TokenType type, const char* value, SourceLocation location);
void token_destroy(Token* token);
void token_list_destroy(Token* tokens);

/* Utility Functions */
bool is_keyword(const char* str);
bool is_operator(char c);
bool is_punctuation(char c);
bool is_identifier_start(char c);
bool is_identifier_char(char c);

#endif /* OBFUSCATOR_LEXER_H */