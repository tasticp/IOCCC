#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * C Keywords for Recognition
 * ═══════════════════════════════════════════════════════════════════════════ */
static const char* c_keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary",
    NULL
};

/* ═══════════════════════════════════════════════════════════════════════════
 * Lexer State Management
 * ═══════════════════════════════════════════════════════════════════════════ */

LexerState* lexer_create(const char* source, const char* filename) {
    if (!source) return NULL;
    
    LexerState* lexer = malloc(sizeof(LexerState));
    if (!lexer) return NULL;
    
    lexer->source = source;
    lexer->position = 0;
    lexer->length = strlen(source);
    lexer->line = 1;
    lexer->column = 1;
    lexer->filename = filename ? strdup(filename) : strdup("<unknown>");
    lexer->tokens = NULL;
    lexer->current_token = NULL;
    lexer->errors = NULL;
    
    return lexer;
}

void lexer_destroy(LexerState* lexer) {
    if (!lexer) return;
    
    free((char*)lexer->filename);
    token_list_destroy(lexer->tokens);
    // TODO: Free error list
    free(lexer);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Character Classification
 * ═══════════════════════════════════════════════════════════════════════════ */

bool is_keyword(const char* str) {
    for (int i = 0; c_keywords[i]; i++) {
        if (strcmp(str, c_keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_operator(char c) {
    return strchr("+-*/%=<>!&|^~?:", c) != NULL;
}

bool is_punctuation(char c) {
    return strchr("(){}[];,.", c) != NULL;
}

bool is_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

bool is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Token Management
 * ═══════════════════════════════════════════════════════════════════════════ */

Token* token_create(TokenType type, const char* value, SourceLocation location) {
    Token* token = malloc(sizeof(Token));
    if (!token) return NULL;
    
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->length = value ? strlen(value) : 0;
    token->location = location;
    token->next = NULL;
    
    return token;
}

void token_destroy(Token* token) {
    if (!token) return;
    free(token->value);
    free(token);
}

void token_list_destroy(Token* tokens) {
    while (tokens) {
        Token* next = tokens->next;
        token_destroy(tokens);
        tokens = next;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Forward Declarations
 * ═══════════════════════════════════════════════════════════════════════════ */
static void lexer_add_error(LexerState* lexer, ErrorType type, const char* message, SourceLocation location);
static void lexer_add_warning(LexerState* lexer, const char* message, SourceLocation location);
static bool is_preprocessor_directive(const char* directive);

/* ═══════════════════════════════════════════════════════════════════════════
 * Lexer Helper Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

static char lexer_current_char(LexerState* lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->source[lexer->position];
}

static char lexer_peek_char(LexerState* lexer, int offset) {
    size_t pos = lexer->position + offset;
    if (pos >= lexer->length) return '\0';
    return lexer->source[pos];
}

static void lexer_advance_char(LexerState* lexer) {
    if (lexer->position < lexer->length) {
        if (lexer->source[lexer->position] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->position++;
    }
}

static SourceLocation lexer_current_location(LexerState* lexer) {
    SourceLocation loc = {lexer->line, lexer->column, lexer->filename};
    return loc;
}

static void lexer_skip_whitespace(LexerState* lexer) {
    while (isspace(lexer_current_char(lexer))) {
        lexer_advance_char(lexer);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Error Handling Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

static void lexer_add_error(LexerState* lexer, ErrorType type, const char* message, SourceLocation location) {
    if (!lexer || !message) return;
    
    Error* error = malloc(sizeof(Error));
    if (!error) return;
    
    error->type = type;
    error->message = strdup(message);
    error->location = location;
    error->next = lexer->errors;
    lexer->errors = error;
}

static void lexer_add_warning(LexerState* lexer, const char* message, SourceLocation location) {
    char warning_msg[512];
    snprintf(warning_msg, sizeof(warning_msg), "Warning: %s", message);
    lexer_add_error(lexer, ERROR_SYNTAX, warning_msg, location);
}

/* Enhanced preprocessor directive parsing */
static bool is_preprocessor_directive(const char* directive) {
    if (!directive) return false;
    
    const char* directives[] = {
        // Standard C preprocessor directives
        "include", "define", "undef", "ifdef", "ifndef", "if", "elif", 
        "else", "endif", "error", "warning", "pragma", "line",
        // GCC-specific directives
        "import", "include_next", "warning", "ident", "sccs",
        // Microsoft Visual C++ directives
        "using", "import",
        // Other common directives
        "region", "endregion", // Visual Studio regions
        NULL
    };
    
    for (int i = 0; directives[i]; i++) {
        if (strcmp(directive, directives[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Get preprocessor directive type for better handling */
typedef enum {
    PP_INCLUDE,
    PP_DEFINE,
    PP_UNDEF,
    PP_IFDEF,
    PP_IFNDEF,
    PP_IF,
    PP_ELIF,
    PP_ELSE,
    PP_ENDIF,
    PP_ERROR,
    PP_WARNING,
    PP_PRAGMA,
    PP_LINE,
    PP_UNKNOWN
} PreprocessorType;

static PreprocessorType get_preprocessor_type(const char* directive) {
    if (!directive) return PP_UNKNOWN;
    
    if (strcmp(directive, "include") == 0) return PP_INCLUDE;
    if (strcmp(directive, "define") == 0) return PP_DEFINE;
    if (strcmp(directive, "undef") == 0) return PP_UNDEF;
    if (strcmp(directive, "ifdef") == 0) return PP_IFDEF;
    if (strcmp(directive, "ifndef") == 0) return PP_IFNDEF;
    if (strcmp(directive, "if") == 0) return PP_IF;
    if (strcmp(directive, "elif") == 0) return PP_ELIF;
    if (strcmp(directive, "else") == 0) return PP_ELSE;
    if (strcmp(directive, "endif") == 0) return PP_ENDIF;
    if (strcmp(directive, "error") == 0) return PP_ERROR;
    if (strcmp(directive, "warning") == 0) return PP_WARNING;
    if (strcmp(directive, "pragma") == 0) return PP_PRAGMA;
    if (strcmp(directive, "line") == 0) return PP_LINE;
    
    return PP_UNKNOWN;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Token Recognition Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

static Token* lexer_read_identifier(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    size_t start_pos = lexer->position;
    
    while (is_identifier_char(lexer_current_char(lexer))) {
        lexer_advance_char(lexer);
    }
    
    size_t length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    TokenType type = is_keyword(value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    Token* token = token_create(type, value, start_loc);
    
    free(value);
    return token;
}

static Token* lexer_read_number(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    size_t start_pos = lexer->position;
    
    // Handle hex numbers (0x...)
    if (lexer_current_char(lexer) == '0' && 
        (lexer_peek_char(lexer, 1) == 'x' || lexer_peek_char(lexer, 1) == 'X')) {
        lexer_advance_char(lexer); // skip '0'
        lexer_advance_char(lexer); // skip 'x'
        while (isxdigit(lexer_current_char(lexer))) {
            lexer_advance_char(lexer);
        }
    } else {
        // Regular decimal number
        while (isdigit(lexer_current_char(lexer))) {
            lexer_advance_char(lexer);
        }
        
        // Handle decimal point
        if (lexer_current_char(lexer) == '.' && isdigit(lexer_peek_char(lexer, 1))) {
            lexer_advance_char(lexer); // skip '.'
            while (isdigit(lexer_current_char(lexer))) {
                lexer_advance_char(lexer);
            }
        }
        
        // Handle scientific notation (e/E)
        if (lexer_current_char(lexer) == 'e' || lexer_current_char(lexer) == 'E') {
            lexer_advance_char(lexer);
            if (lexer_current_char(lexer) == '+' || lexer_current_char(lexer) == '-') {
                lexer_advance_char(lexer);
            }
            while (isdigit(lexer_current_char(lexer))) {
                lexer_advance_char(lexer);
            }
        }
    }
    
    // Handle suffixes (L, U, F, etc.)
    while (isalpha(lexer_current_char(lexer))) {
        lexer_advance_char(lexer);
    }
    
    size_t length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    Token* token = token_create(TOKEN_NUMBER, value, start_loc);
    free(value);
    return token;
}

static Token* lexer_read_string(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    size_t start_pos = lexer->position;
    char quote = lexer_current_char(lexer);
    
    lexer_advance_char(lexer); // skip opening quote
    
    while (lexer_current_char(lexer) != quote && lexer_current_char(lexer) != '\0') {
        if (lexer_current_char(lexer) == '\\') {
            lexer_advance_char(lexer); // skip backslash
            if (lexer_current_char(lexer) != '\0') {
                lexer_advance_char(lexer); // skip escaped character
            }
        } else {
            lexer_advance_char(lexer);
        }
    }
    
    if (lexer_current_char(lexer) == quote) {
        lexer_advance_char(lexer); // skip closing quote
    }
    
    size_t length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    TokenType type = (quote == '"') ? TOKEN_STRING : TOKEN_CHAR;
    Token* token = token_create(type, value, start_loc);
    
    free(value);
    return token;
}

static Token* lexer_read_operator(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    char first = lexer_current_char(lexer);
    char second = lexer_peek_char(lexer, 1);
    char third = lexer_peek_char(lexer, 2);
    
    // Three-character operators
    if ((first == '<' && second == '<' && third == '=') ||
        (first == '>' && second == '>' && third == '=')) {
        char value[4] = {first, second, third, '\0'};
        lexer_advance_char(lexer);
        lexer_advance_char(lexer);
        lexer_advance_char(lexer);
        return token_create(TOKEN_OPERATOR, value, start_loc);
    }
    
    // Two-character operators
    if ((first == '=' && second == '=') ||
        (first == '!' && second == '=') ||
        (first == '<' && second == '=') ||
        (first == '>' && second == '=') ||
        (first == '<' && second == '<') ||
        (first == '>' && second == '>') ||
        (first == '&' && second == '&') ||
        (first == '|' && second == '|') ||
        (first == '+' && second == '+') ||
        (first == '-' && second == '-') ||
        (first == '+' && second == '=') ||
        (first == '-' && second == '=') ||
        (first == '*' && second == '=') ||
        (first == '/' && second == '=') ||
        (first == '%' && second == '=') ||
        (first == '&' && second == '=') ||
        (first == '|' && second == '=') ||
        (first == '^' && second == '=') ||
        (first == '-' && second == '>')) {
        char value[3] = {first, second, '\0'};
        lexer_advance_char(lexer);
        lexer_advance_char(lexer);
        return token_create(TOKEN_OPERATOR, value, start_loc);
    }
    
    // Single-character operators
    char value[2] = {first, '\0'};
    lexer_advance_char(lexer);
    return token_create(TOKEN_OPERATOR, value, start_loc);
}

static Token* lexer_read_comment(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    size_t start_pos = lexer->position;
    bool is_unterminated = false;
    
    if (lexer_current_char(lexer) == '/' && lexer_peek_char(lexer, 1) == '/') {
        // Single-line comment
        lexer_advance_char(lexer); // skip first '/'
        lexer_advance_char(lexer); // skip second '/'
        
        while (lexer_current_char(lexer) != '\n' && lexer_current_char(lexer) != '\0') {
            lexer_advance_char(lexer);
        }
        
        // Don't consume the newline - let the main tokenizer handle it
        
    } else if (lexer_current_char(lexer) == '/' && lexer_peek_char(lexer, 1) == '*') {
        // Multi-line comment
        lexer_advance_char(lexer); // skip '/'
        lexer_advance_char(lexer); // skip '*'
        
        int nesting_level = 1; // For potential nested comment support
        
        while (lexer_current_char(lexer) != '\0') {
            if (lexer_current_char(lexer) == '*' && lexer_peek_char(lexer, 1) == '/') {
                lexer_advance_char(lexer); // skip '*'
                lexer_advance_char(lexer); // skip '/'
                nesting_level--;
                if (nesting_level == 0) break;
            } else if (lexer_current_char(lexer) == '/' && lexer_peek_char(lexer, 1) == '*') {
                // Nested comment start (not standard C, but some compilers support it)
                lexer_advance_char(lexer); // skip '/'
                lexer_advance_char(lexer); // skip '*'
                // Note: Standard C doesn't support nested comments, so we don't increment nesting_level
            } else {
                lexer_advance_char(lexer);
            }
        }
        
        if (lexer_current_char(lexer) == '\0' && nesting_level > 0) {
            is_unterminated = true;
            lexer_add_error(lexer, ERROR_SYNTAX, "Unterminated multi-line comment", start_loc);
        }
    }
    
    size_t length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    Token* token = token_create(TOKEN_COMMENT, value, start_loc);
    
    // Error information already added above if comment is unterminated
    
    free(value);
    return token;
}

static Token* lexer_read_preprocessor(LexerState* lexer) {
    SourceLocation start_loc = lexer_current_location(lexer);
    size_t start_pos = lexer->position;
    
    lexer_advance_char(lexer); // skip '#'
    
    // Skip whitespace after #
    while (isspace(lexer_current_char(lexer)) && lexer_current_char(lexer) != '\n') {
        lexer_advance_char(lexer);
    }
    
    // Read the directive name
    size_t directive_start = lexer->position;
    while (is_identifier_char(lexer_current_char(lexer))) {
        lexer_advance_char(lexer);
    }
    
    // Extract directive name for validation
    size_t directive_length = lexer->position - directive_start;
    char* directive_name = NULL;
    if (directive_length > 0) {
        directive_name = malloc(directive_length + 1);
        strncpy(directive_name, lexer->source + directive_start, directive_length);
        directive_name[directive_length] = '\0';
    }
    
    // Read the rest of the line, handling line continuations
    while (lexer_current_char(lexer) != '\n' && lexer_current_char(lexer) != '\0') {
        // Handle line continuation with backslash
        if (lexer_current_char(lexer) == '\\' && lexer_peek_char(lexer, 1) == '\n') {
            lexer_advance_char(lexer); // skip '\'
            lexer_advance_char(lexer); // skip '\n'
        } else {
            lexer_advance_char(lexer);
        }
    }
    
    size_t length = lexer->position - start_pos;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start_pos, length);
    value[length] = '\0';
    
    // Validate preprocessor directive
    if (directive_name && !is_preprocessor_directive(directive_name)) {
        char warning_msg[256];
        snprintf(warning_msg, sizeof(warning_msg), "Unknown preprocessor directive: #%s", directive_name);
        lexer_add_warning(lexer, warning_msg, start_loc);
    }
    
    Token* token = token_create(TOKEN_PREPROCESSOR, value, start_loc);
    
    if (directive_name) free(directive_name);
    free(value);
    return token;
}

/* Enhanced preprocessor directive parsing */
static bool is_preprocessor_directive(const char* directive) {
    if (!directive) return false;
    
    const char* directives[] = {
        // Standard C preprocessor directives
        "include", "define", "undef", "ifdef", "ifndef", "if", "elif", 
        "else", "endif", "error", "warning", "pragma", "line",
        // GCC-specific directives
        "import", "include_next", "warning", "ident", "sccs",
        // Microsoft Visual C++ directives
        "using", "import",
        // Other common directives
        "region", "endregion", // Visual Studio regions
        NULL
    };
    
    for (int i = 0; directives[i]; i++) {
        if (strcmp(directive, directives[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Get preprocessor directive type for better handling */
typedef enum {
    PP_INCLUDE,
    PP_DEFINE,
    PP_UNDEF,
    PP_IFDEF,
    PP_IFNDEF,
    PP_IF,
    PP_ELIF,
    PP_ELSE,
    PP_ENDIF,
    PP_ERROR,
    PP_WARNING,
    PP_PRAGMA,
    PP_LINE,
    PP_UNKNOWN
} PreprocessorType;

static PreprocessorType get_preprocessor_type(const char* directive) {
    if (!directive) return PP_UNKNOWN;
    
    if (strcmp(directive, "include") == 0) return PP_INCLUDE;
    if (strcmp(directive, "define") == 0) return PP_DEFINE;
    if (strcmp(directive, "undef") == 0) return PP_UNDEF;
    if (strcmp(directive, "ifdef") == 0) return PP_IFDEF;
    if (strcmp(directive, "ifndef") == 0) return PP_IFNDEF;
    if (strcmp(directive, "if") == 0) return PP_IF;
    if (strcmp(directive, "elif") == 0) return PP_ELIF;
    if (strcmp(directive, "else") == 0) return PP_ELSE;
    if (strcmp(directive, "endif") == 0) return PP_ENDIF;
    if (strcmp(directive, "error") == 0) return PP_ERROR;
    if (strcmp(directive, "warning") == 0) return PP_WARNING;
    if (strcmp(directive, "pragma") == 0) return PP_PRAGMA;
    if (strcmp(directive, "line") == 0) return PP_LINE;
    
    return PP_UNKNOWN;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Main Tokenization Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

Token* lexer_next_token(LexerState* lexer) {
    lexer_skip_whitespace(lexer);
    
    char current = lexer_current_char(lexer);
    
    if (current == '\0') {
        return token_create(TOKEN_EOF, "", lexer_current_location(lexer));
    }
    
    // Preprocessor directives
    if (current == '#') {
        return lexer_read_preprocessor(lexer);
    }
    
    // Comments
    if (current == '/' && (lexer_peek_char(lexer, 1) == '/' || lexer_peek_char(lexer, 1) == '*')) {
        return lexer_read_comment(lexer);
    }
    
    // String and character literals
    if (current == '"' || current == '\'') {
        return lexer_read_string(lexer);
    }
    
    // Numbers
    if (isdigit(current) || (current == '.' && isdigit(lexer_peek_char(lexer, 1)))) {
        return lexer_read_number(lexer);
    }
    
    // Identifiers and keywords
    if (is_identifier_start(current)) {
        return lexer_read_identifier(lexer);
    }
    
    // Operators
    if (is_operator(current)) {
        return lexer_read_operator(lexer);
    }
    
    // Punctuation
    if (is_punctuation(current)) {
        SourceLocation loc = lexer_current_location(lexer);
        char value[2] = {current, '\0'};
        lexer_advance_char(lexer);
        return token_create(TOKEN_PUNCTUATION, value, loc);
    }
    
    // Unknown character
    SourceLocation loc = lexer_current_location(lexer);
    char value[2] = {current, '\0'};
    lexer_advance_char(lexer);
    return token_create(TOKEN_UNKNOWN, value, loc);
}

Token* lexer_tokenize(LexerState* lexer) {
    Token* first_token = NULL;
    Token* current_token = NULL;
    
    while (true) {
        Token* token = lexer_next_token(lexer);
        if (!token) break;
        
        if (!first_token) {
            first_token = token;
            current_token = token;
        } else {
            current_token->next = token;
            current_token = token;
        }
        
        if (token->type == TOKEN_EOF) break;
    }
    
    lexer->tokens = first_token;
    lexer->current_token = first_token;
    return first_token;
}

Token* lexer_peek_token(LexerState* lexer) {
    return lexer->current_token;
}

bool lexer_has_errors(const LexerState* lexer) {
    return lexer && lexer->errors != NULL;
}

Error* lexer_get_errors(const LexerState* lexer) {
    return lexer ? lexer->errors : NULL;
}

void lexer_clear_errors(LexerState* lexer) {
    if (!lexer) return;
    
    Error* current = lexer->errors;
    while (current) {
        Error* next = current->next;
        free(current->message);
        free(current);
        current = next;
    }
    lexer->errors = NULL;
}