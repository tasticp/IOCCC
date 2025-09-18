#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/parser/parser.h"
#include "../src/lexer/lexer.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Parser Expression Tests
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_primary_expressions() {
    printf("Testing primary expressions...\n");
    
    // Test number literal
    const char* source = "42";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_primary(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_LITERAL);
    assert(strcmp(expr->data.literal.value, "42") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Primary expressions test passed\n");
}

void test_binary_expressions() {
    printf("Testing binary expressions...\n");
    
    const char* source = "2 + 3 * 4";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.operator, "+") == 0);
    
    // Left should be 2
    assert(expr->data.binary.left->type == NODE_LITERAL);
    assert(strcmp(expr->data.binary.left->data.literal.value, "2") == 0);
    
    // Right should be 3 * 4
    assert(expr->data.binary.right->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.right->data.binary.operator, "*") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Binary expressions test passed\n");
}

void test_unary_expressions() {
    printf("Testing unary expressions...\n");
    
    const char* source = "-x";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_UNARY_OP);
    assert(strcmp(expr->data.unary.operator, "-") == 0);
    assert(expr->data.unary.is_prefix == true);
    
    // Operand should be identifier 'x'
    assert(expr->data.unary.operand->type == NODE_IDENTIFIER);
    assert(strcmp(expr->data.unary.operand->data.identifier.name, "x") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Unary expressions test passed\n");
}

void test_function_calls() {
    printf("Testing function calls...\n");
    
    const char* source = "func(a, b + c)";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_CALL);
    
    // Function should be identifier 'func'
    assert(expr->data.call.function->type == NODE_IDENTIFIER);
    assert(strcmp(expr->data.call.function->data.identifier.name, "func") == 0);
    
    // Should have arguments
    assert(expr->data.call.arguments != NULL);
    
    // First argument should be 'a'
    ASTNode* arg1 = expr->data.call.arguments;
    assert(arg1->type == NODE_IDENTIFIER);
    assert(strcmp(arg1->data.identifier.name, "a") == 0);
    
    // Second argument should be 'b + c'
    ASTNode* arg2 = arg1->next;
    assert(arg2 != NULL);
    assert(arg2->type == NODE_BINARY_OP);
    assert(strcmp(arg2->data.binary.operator, "+") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Function calls test passed\n");
}

void test_parenthesized_expressions() {
    printf("Testing parenthesized expressions...\n");
    
    const char* source = "(2 + 3) * 4";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.operator, "*") == 0);
    
    // Left should be (2 + 3)
    assert(expr->data.binary.left->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.left->data.binary.operator, "+") == 0);
    
    // Right should be 4
    assert(expr->data.binary.right->type == NODE_LITERAL);
    assert(strcmp(expr->data.binary.right->data.literal.value, "4") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Parenthesized expressions test passed\n");
}

void test_operator_precedence() {
    printf("Testing operator precedence...\n");
    
    const char* source = "a + b * c - d / e";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_BINARY_OP);
    
    // Should parse as: ((a + (b * c)) - (d / e))
    // Top level should be subtraction
    assert(strcmp(expr->data.binary.operator, "-") == 0);
    
    // Left side should be addition
    assert(expr->data.binary.left->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.left->data.binary.operator, "+") == 0);
    
    // Right side should be division
    assert(expr->data.binary.right->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.right->data.binary.operator, "/") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Operator precedence test passed\n");
}

void test_assignment_expressions() {
    printf("Testing assignment expressions...\n");
    
    const char* source = "x = y + z";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.operator, "=") == 0);
    
    // Left should be identifier 'x'
    assert(expr->data.binary.left->type == NODE_IDENTIFIER);
    assert(strcmp(expr->data.binary.left->data.identifier.name, "x") == 0);
    
    // Right should be 'y + z'
    assert(expr->data.binary.right->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.right->data.binary.operator, "+") == 0);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Assignment expressions test passed\n");
}

void test_complex_expressions() {
    printf("Testing complex expressions...\n");
    
    const char* source = "func(a * b, c + d) + array[index] * 2";
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    ParserState* parser = parser_create(tokens);
    ASTNode* expr = parser_parse_expression(parser);
    
    assert(expr != NULL);
    assert(expr->type == NODE_BINARY_OP);
    assert(strcmp(expr->data.binary.operator, "+") == 0);
    
    // Left should be function call
    assert(expr->data.binary.left->type == NODE_CALL);
    
    ast_node_destroy(expr);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Complex expressions test passed\n");
}

int main() {
    printf("Running Parser Expression Tests...\n");
    printf("═══════════════════════════════════════\n");
    
    test_primary_expressions();
    test_binary_expressions();
    test_unary_expressions();
    test_function_calls();
    test_parenthesized_expressions();
    test_operator_precedence();
    test_assignment_expressions();
    test_complex_expressions();
    
    printf("═══════════════════════════════════════\n");
    printf("All parser expression tests passed! ✓\n");
    
    return 0;
}