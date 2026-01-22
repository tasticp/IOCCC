#ifndef OBFUSCATOR_TYPES_H
#define OBFUSCATOR_TYPES_H

#include <stddef.h>
#include <stdbool.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * Core Data Types for C Code Obfuscator
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Token Types */
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_PREPROCESSOR,
    TOKEN_COMMENT,
    TOKEN_WHITESPACE,
    TOKEN_UNKNOWN
} TokenType;

/* Source Location */
typedef struct {
    int line;
    int column;
    const char* filename;
} SourceLocation;

/* Token Structure */
typedef struct Token {
    TokenType type;
    char* value;
    size_t length;
    SourceLocation location;
    struct Token* next;
} Token;

/* AST Node Types */
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_VARIABLE,
    NODE_PARAMETER,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_ASSIGNMENT,
    NODE_CALL,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BLOCK,
    NODE_RETURN,
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_STRUCT,
    NODE_UNION,
    NODE_TYPEDEF,
    NODE_ARRAY_ACCESS,
    NODE_MEMBER_ACCESS,
    NODE_CAST,
    NODE_SIZEOF
} NodeType;

/* Forward declaration */
struct ASTNode;

/* AST Node Structure */
typedef struct ASTNode {
    NodeType type;
    SourceLocation location;
    
    union {
        /* Program node */
        struct {
            struct ASTNode* declarations;
        } program;
        
        /* Function node */
        struct {
            char* name;
            char* return_type;
            struct ASTNode* parameters;
            struct ASTNode* body;
            bool is_static;
        } function;
        
        /* Variable node */
        struct {
            char* name;
            char* type;
            struct ASTNode* initializer;
            bool is_static;
            bool is_const;
        } variable;
        
        /* Binary operation */
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char* operator;
        } binary;
        
        /* Unary operation */
        struct {
            struct ASTNode* operand;
            char* operator;
            bool is_prefix;
        } unary;
        
        /* Function call */
        struct {
            struct ASTNode* function;
            struct ASTNode* arguments;
        } call;
        
        /* Control structures */
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_stmt;
            struct ASTNode* else_stmt;
        } if_stmt;
        
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;
        
        struct {
            struct ASTNode* init;
            struct ASTNode* condition;
            struct ASTNode* update;
            struct ASTNode* body;
        } for_stmt;
        
        /* Block statement */
        struct {
            struct ASTNode* statements;
        } block;
        
        /* Literals and identifiers */
        struct {
            char* value;
        } literal;
        
        struct {
            char* name;
        } identifier;
        
        /* Struct/Union */
        struct {
            char* name;
            struct ASTNode* members;
        } struct_def;
    } data;
    
    struct ASTNode* next;  /* For linked lists */
} ASTNode;

/* Symbol Types */
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER,
    SYMBOL_STRUCT,
    SYMBOL_UNION,
    SYMBOL_TYPEDEF,
    SYMBOL_MACRO
} SymbolType;

/* Symbol Structure */
typedef struct Symbol {
    char* original_name;
    char* obfuscated_name;
    SymbolType type;
    char* data_type;
    struct Scope* scope;
    bool is_global;
    bool is_obfuscated;
    struct Symbol* next;
} Symbol;

/* Scope Structure */
typedef struct Scope {
    Symbol* symbols;
    struct Scope* parent;
    struct Scope* children;
    struct Scope* next_sibling;
    int depth;
} Scope;

/* Symbol Table */
typedef struct {
    Scope* global_scope;
    Scope* current_scope;
    size_t symbol_count;
} SymbolTable;

/* Obfuscation Levels */
typedef enum {
    OBF_BASIC = 1,
    OBF_INTERMEDIATE = 2,
    OBF_EXTREME = 3
} ObfuscationLevel;

/* Aesthetic Styles */
typedef enum {
    AESTHETIC_MINIMAL,
    AESTHETIC_UNICODE,
    AESTHETIC_HEXADECIMAL,
    AESTHETIC_ARTISTIC,
    AESTHETIC_CHAOTIC,
    AESTHETIC_MATRIX,
    AESTHETIC_MYSTICAL,
    AESTHETIC_ASCII_ART,
    AESTHETIC_RUNIC
} AestheticStyle;

/* Name Generation Patterns */
typedef struct {
    char* pattern;
    int counter;
    bool use_unicode;
    bool use_numbers;
    bool use_underscores;
} NameGenerator;

/* Obfuscation Configuration */
typedef struct {
    ObfuscationLevel level;
    AestheticStyle aesthetic;
    bool preserve_debug_info;
    bool obfuscate_strings;
    bool obfuscate_control_flow;
    bool insert_dead_code;
    bool use_macros;
    char* output_file;
    NameGenerator name_gen;
} ObfuscationConfig;

/* Code Generation Configuration */
typedef struct {
    bool pretty_print;
    int indent_size;
    bool add_comments;
    bool add_ascii_art;
    AestheticStyle style;
} CodeGenConfig;

/* Error Types */
typedef enum {
    ERROR_NONE = 0,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_OBFUSCATION,
    ERROR_MEMORY,
    ERROR_IO
} ErrorType;

/* Error Structure */
typedef struct {
    ErrorType type;
    char* message;
    SourceLocation location;
    struct Error* next;
} Error;

/* Obfuscation Technique */
typedef struct {
    char* name;
    char* description;
    ObfuscationLevel min_level;
    bool (*apply)(ASTNode* node, void* context);
} ObfuscationTechnique;

#endif /* OBFUSCATOR_TYPES_H */