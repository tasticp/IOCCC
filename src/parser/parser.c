#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * Parser Implementation with Expression Parsing
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Operator Precedence Table */
typedef enum {
    PREC_NONE = 0,
    PREC_ASSIGNMENT,    // = += -= *= /= %= &= |= ^= <<= >>=
    PREC_TERNARY,       // ?:
    PREC_LOGICAL_OR,    // ||
    PREC_LOGICAL_AND,   // &&
    PREC_BITWISE_OR,    // |
    PREC_BITWISE_XOR,   // ^
    PREC_BITWISE_AND,   // &
    PREC_EQUALITY,      // == !=
    PREC_RELATIONAL,    // < <= > >=
    PREC_SHIFT,         // << >>
    PREC_ADDITIVE,      // + -
    PREC_MULTIPLICATIVE,// * / %
    PREC_UNARY,         // ! ~ + - ++ -- & * sizeof
    PREC_POSTFIX,       // [] () . -> ++ --
    PREC_PRIMARY        // literals, identifiers, ()
} Precedence;

/* ═══════════════════════════════════════════════════════════════════════════
 * Parser State Management
 * ═══════════════════════════════════════════════════════════════════════════ */

ParserState* parser_create(Token* tokens) {
    ParserState* parser = malloc(sizeof(ParserState));
    if (!parser) return NULL;
    
    parser->tokens = tokens;
    parser->current_token = tokens;
    parser->symbol_table = symbol_table_create();
    parser->errors = NULL;
    parser->error_count = 0;
    
    return parser;
}

void parser_destroy(ParserState* parser) {
    if (!parser) return;
    
    symbol_table_destroy(parser->symbol_table);
    // TODO: Free error list
    free(parser);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * AST Node Management
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode* ast_node_create(NodeType type, SourceLocation location) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    node->location = location;
    node->next = NULL;
    
    return node;
}

void ast_node_destroy(ASTNode* node) {
    if (!node) return;
    
    // Free node-specific data
    switch (node->type) {
        case NODE_FUNCTION:
            free(node->data.function.name);
            free(node->data.function.return_type);
            ast_node_destroy(node->data.function.parameters);
            ast_node_destroy(node->data.function.body);
            break;
            
        case NODE_VARIABLE:
            free(node->data.variable.name);
            free(node->data.variable.type);
            ast_node_destroy(node->data.variable.initializer);
            break;
            
        case NODE_BINARY_OP:
            free(node->data.binary.operator);
            ast_node_destroy(node->data.binary.left);
            ast_node_destroy(node->data.binary.right);
            break;
            
        case NODE_UNARY_OP:
            free(node->data.unary.operator);
            ast_node_destroy(node->data.unary.operand);
            break;
            
        case NODE_CALL:
            ast_node_destroy(node->data.call.function);
            ast_node_destroy(node->data.call.arguments);
            break;
            
        case NODE_IF:
            ast_node_destroy(node->data.if_stmt.condition);
            ast_node_destroy(node->data.if_stmt.then_stmt);
            ast_node_destroy(node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            ast_node_destroy(node->data.while_stmt.condition);
            ast_node_destroy(node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            ast_node_destroy(node->data.for_stmt.init);
            ast_node_destroy(node->data.for_stmt.condition);
            ast_node_destroy(node->data.for_stmt.update);
            ast_node_destroy(node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            ast_node_destroy(node->data.block.statements);
            break;
            
        case NODE_LITERAL:
            free(node->data.literal.value);
            break;
            
        case NODE_IDENTIFIER:
            free(node->data.identifier.name);
            break;
            
        case NODE_STRUCT:
            free(node->data.struct_def.name);
            ast_node_destroy(node->data.struct_def.members);
            break;
            
        default:
            break;
    }
    
    free(node);
}

void ast_tree_destroy(ASTNode* root) {
    while (root) {
        ASTNode* next = root->next;
        ast_node_destroy(root);
        root = next;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Parser Utility Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

Token* parser_advance(ParserState* parser) {
    if (parser->current_token && parser->current_token->type != TOKEN_EOF) {
        parser->current_token = parser->current_token->next;
    }
    return parser->current_token;
}

Token* parser_peek(ParserState* parser) {
    return parser->current_token;
}

bool parser_match(ParserState* parser, TokenType type) {
    if (!parser->current_token) return false;
    return parser->current_token->type == type;
}

bool parser_consume(ParserState* parser, TokenType type, const char* error_msg) {
    if (parser_match(parser, type)) {
        parser_advance(parser);
        return true;
    }
    
    // TODO: Add error reporting
    parser->error_count++;
    return false;
}

static bool parser_match_operator(ParserState* parser, const char* op) {
    if (!parser_match(parser, TOKEN_OPERATOR)) return false;
    return strcmp(parser->current_token->value, op) == 0;
}

static Precedence get_operator_precedence(const char* op) {
    if (!op) return PREC_NONE;
    
    // Assignment operators
    if (strcmp(op, "=") == 0 || strcmp(op, "+=") == 0 || strcmp(op, "-=") == 0 ||
        strcmp(op, "*=") == 0 || strcmp(op, "/=") == 0 || strcmp(op, "%=") == 0 ||
        strcmp(op, "&=") == 0 || strcmp(op, "|=") == 0 || strcmp(op, "^=") == 0 ||
        strcmp(op, "<<=") == 0 || strcmp(op, ">>=") == 0) {
        return PREC_ASSIGNMENT;
    }
    
    // Logical OR
    if (strcmp(op, "||") == 0) return PREC_LOGICAL_OR;
    
    // Logical AND
    if (strcmp(op, "&&") == 0) return PREC_LOGICAL_AND;
    
    // Bitwise OR
    if (strcmp(op, "|") == 0) return PREC_BITWISE_OR;
    
    // Bitwise XOR
    if (strcmp(op, "^") == 0) return PREC_BITWISE_XOR;
    
    // Bitwise AND
    if (strcmp(op, "&") == 0) return PREC_BITWISE_AND;
    
    // Equality
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) return PREC_EQUALITY;
    
    // Relational
    if (strcmp(op, "<") == 0 || strcmp(op, "<=") == 0 ||
        strcmp(op, ">") == 0 || strcmp(op, ">=") == 0) {
        return PREC_RELATIONAL;
    }
    
    // Shift
    if (strcmp(op, "<<") == 0 || strcmp(op, ">>") == 0) return PREC_SHIFT;
    
    // Additive
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return PREC_ADDITIVE;
    
    // Multiplicative
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
        return PREC_MULTIPLICATIVE;
    }
    
    return PREC_NONE;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Expression Parsing (Pratt Parser)
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode* parse_expression_precedence(ParserState* parser, Precedence min_prec);

ASTNode* parser_parse_primary(ParserState* parser) {
    Token* token = parser_peek(parser);
    if (!token) return NULL;
    
    switch (token->type) {
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_CHAR: {
            ASTNode* node = ast_node_create(NODE_LITERAL, token->location);
            if (node) {
                node->data.literal.value = strdup(token->value);
            }
            parser_advance(parser);
            return node;
        }
        
        case TOKEN_IDENTIFIER: {
            ASTNode* node = ast_node_create(NODE_IDENTIFIER, token->location);
            if (node) {
                node->data.identifier.name = strdup(token->value);
            }
            parser_advance(parser);
            
            // Check for function call
            if (parser_match(parser, TOKEN_PUNCTUATION) && 
                strcmp(parser_peek(parser)->value, "(") == 0) {
                
                ASTNode* call_node = ast_node_create(NODE_CALL, token->location);
                if (call_node) {
                    call_node->data.call.function = node;
                    
                    parser_advance(parser); // consume '('
                    
                    // Parse arguments
                    ASTNode* args = NULL;
                    ASTNode* last_arg = NULL;
                    
                    while (!parser_match(parser, TOKEN_PUNCTUATION) ||
                           strcmp(parser_peek(parser)->value, ")") != 0) {
                        
                        ASTNode* arg = parser_parse_expression(parser);
                        if (arg) {
                            if (!args) {
                                args = arg;
                                last_arg = arg;
                            } else {
                                last_arg->next = arg;
                                last_arg = arg;
                            }
                        }
                        
                        if (parser_match(parser, TOKEN_PUNCTUATION) &&
                            strcmp(parser_peek(parser)->value, ",") == 0) {
                            parser_advance(parser); // consume ','
                        } else {
                            break;
                        }
                    }
                    
                    call_node->data.call.arguments = args;
                    parser_consume(parser, TOKEN_PUNCTUATION, "Expected ')'");
                }
                return call_node;
            }
            
            return node;
        }
        
        case TOKEN_PUNCTUATION: {
            if (strcmp(token->value, "(") == 0) {
                parser_advance(parser); // consume '('
                ASTNode* expr = parser_parse_expression(parser);
                parser_consume(parser, TOKEN_PUNCTUATION, "Expected ')'");
                return expr;
            }
            break;
        }
        
        case TOKEN_OPERATOR: {
            // Unary operators
            if (strcmp(token->value, "+") == 0 || strcmp(token->value, "-") == 0 ||
                strcmp(token->value, "!") == 0 || strcmp(token->value, "~") == 0 ||
                strcmp(token->value, "*") == 0 || strcmp(token->value, "&") == 0 ||
                strcmp(token->value, "++") == 0 || strcmp(token->value, "--") == 0) {
                
                ASTNode* node = ast_node_create(NODE_UNARY_OP, token->location);
                if (node) {
                    node->data.unary.operator = strdup(token->value);
                    node->data.unary.is_prefix = true;
                    parser_advance(parser);
                    node->data.unary.operand = parse_expression_precedence(parser, PREC_UNARY);
                }
                return node;
            }
            break;
        }
        
        case TOKEN_KEYWORD: {
            if (strcmp(token->value, "sizeof") == 0) {
                ASTNode* node = ast_node_create(NODE_SIZEOF, token->location);
                parser_advance(parser);
                
                if (parser_match(parser, TOKEN_PUNCTUATION) &&
                    strcmp(parser_peek(parser)->value, "(") == 0) {
                    parser_advance(parser); // consume '('
                    // TODO: Parse type or expression
                    parser_consume(parser, TOKEN_PUNCTUATION, "Expected ')'");
                }
                
                return node;
            }
            break;
        }
        
        default:
            break;
    }
    
    return NULL;
}

static ASTNode* parse_expression_precedence(ParserState* parser, Precedence min_prec) {
    ASTNode* left = parser_parse_primary(parser);
    if (!left) return NULL;
    
    while (parser_match(parser, TOKEN_OPERATOR)) {
        const char* op = parser_peek(parser)->value;
        Precedence prec = get_operator_precedence(op);
        
        if (prec < min_prec) break;
        
        SourceLocation op_location = parser_peek(parser)->location;
        parser_advance(parser); // consume operator
        
        // Handle ternary operator
        if (strcmp(op, "?") == 0) {
            ASTNode* then_expr = parser_parse_expression(parser);
            parser_consume(parser, TOKEN_OPERATOR, "Expected ':'");
            ASTNode* else_expr = parse_expression_precedence(parser, prec);
            
            // Create ternary node (represented as special binary op)
            ASTNode* ternary = ast_node_create(NODE_BINARY_OP, op_location);
            if (ternary) {
                ternary->data.binary.operator = strdup("?:");
                ternary->data.binary.left = left;
                // Create a special node to hold both then and else expressions
                // This is a simplified representation
                ternary->data.binary.right = then_expr;
                then_expr->next = else_expr;
            }
            left = ternary;
            continue;
        }
        
        // Right associative operators
        Precedence next_prec = prec;
        if (strcmp(op, "=") == 0 || strstr(op, "=") == op + strlen(op) - 1) {
            // Assignment operators are right associative
            next_prec = (Precedence)(prec - 1);
        } else {
            // Most operators are left associative
            next_prec = (Precedence)(prec + 1);
        }
        
        ASTNode* right = parse_expression_precedence(parser, next_prec);
        if (!right) {
            ast_node_destroy(left);
            return NULL;
        }
        
        ASTNode* binary = ast_node_create(NODE_BINARY_OP, op_location);
        if (binary) {
            binary->data.binary.operator = strdup(op);
            binary->data.binary.left = left;
            binary->data.binary.right = right;
        }
        left = binary;
    }
    
    return left;
}

ASTNode* parser_parse_expression(ParserState* parser) {
    return parse_expression_precedence(parser, PREC_ASSIGNMENT);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Error Handling
 * ═══════════════════════════════════════════════════════════════════════════ */

bool parser_has_errors(const ParserState* parser) {
    return parser && parser->error_count > 0;
}

Error* parser_get_errors(const ParserState* parser) {
    return parser ? parser->errors : NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Placeholder Functions (to be implemented in later tasks)
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode* parser_parse(ParserState* parser) {
    // TODO: Implement full program parsing
    return parser_parse_expression(parser);
}

ASTNode* parser_parse_program(ParserState* parser) {
    // TODO: Implement program parsing
    return NULL;
}

ASTNode* parser_parse_declaration(ParserState* parser) {
    // TODO: Implement declaration parsing
    return NULL;
}

ASTNode* parser_parse_function(ParserState* parser) {
    // TODO: Implement function parsing
    return NULL;
}

ASTNode* parser_parse_variable(ParserState* parser) {
    // TODO: Implement variable parsing
    return NULL;
}

ASTNode* parser_parse_statement(ParserState* parser) {
    // TODO: Implement statement parsing
    return NULL;
}