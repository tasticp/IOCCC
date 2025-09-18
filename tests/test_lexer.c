#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/lexer/lexer.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Lexer Unit Tests
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_basic_tokens() {
    printf("Testing basic token recognition...\n");
    
    const char* source = "int main() { return 0; }";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Check token sequence
    assert(tokens->type == TOKEN_KEYWORD);
    assert(strcmp(tokens->value, "int") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_IDENTIFIER);
    assert(strcmp(tokens->value, "main") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PUNCTUATION);
    assert(strcmp(tokens->value, "(") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PUNCTUATION);
    assert(strcmp(tokens->value, ")") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PUNCTUATION);
    assert(strcmp(tokens->value, "{") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_KEYWORD);
    assert(strcmp(tokens->value, "return") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "0") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PUNCTUATION);
    assert(strcmp(tokens->value, ";") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PUNCTUATION);
    assert(strcmp(tokens->value, "}") == 0);
    
    tokens = tokens->next;
    assert(tokens->type == TOKEN_EOF);
    
    lexer_destroy(lexer);
    printf("✓ Basic tokens test passed\n");
}

void test_numbers() {
    printf("Testing number recognition...\n");
    
    const char* source = "42 3.14 0x1A 1e10 2.5e-3";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    // Integer
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "42") == 0);
    
    // Float
    tokens = tokens->next;
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "3.14") == 0);
    
    // Hex
    tokens = tokens->next;
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "0x1A") == 0);
    
    // Scientific notation
    tokens = tokens->next;
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "1e10") == 0);
    
    // Scientific with negative exponent
    tokens = tokens->next;
    assert(tokens->type == TOKEN_NUMBER);
    assert(strcmp(tokens->value, "2.5e-3") == 0);
    
    lexer_destroy(lexer);
    printf("✓ Numbers test passed\n");
}

void test_strings() {
    printf("Testing string recognition...\n");
    
    const char* source = "\"hello world\" 'c' \"escaped\\\"string\"";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    // String literal
    assert(tokens->type == TOKEN_STRING);
    assert(strcmp(tokens->value, "\"hello world\"") == 0);
    
    // Character literal
    tokens = tokens->next;
    assert(tokens->type == TOKEN_CHAR);
    assert(strcmp(tokens->value, "'c'") == 0);
    
    // Escaped string
    tokens = tokens->next;
    assert(tokens->type == TOKEN_STRING);
    assert(strcmp(tokens->value, "\"escaped\\\"string\"") == 0);
    
    lexer_destroy(lexer);
    printf("✓ Strings test passed\n");
}

void test_operators() {
    printf("Testing operator recognition...\n");
    
    const char* source = "+ - * / % == != <= >= << >> && || ++ -- += -= -> <<= >>=";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    const char* expected[] = {
        "+", "-", "*", "/", "%", "==", "!=", "<=", ">=", 
        "<<", ">>", "&&", "||", "++", "--", "+=", "-=", "->", "<<=", ">>="
    };
    
    for (int i = 0; i < 20; i++) {
        assert(tokens->type == TOKEN_OPERATOR);
        assert(strcmp(tokens->value, expected[i]) == 0);
        tokens = tokens->next;
    }
    
    lexer_destroy(lexer);
    printf("✓ Operators test passed\n");
}

void test_comments() {
    printf("Testing comment recognition...\n");
    
    const char* source = "// single line comment\n/* multi\nline\ncomment */";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    // Single-line comment
    assert(tokens->type == TOKEN_COMMENT);
    assert(strncmp(tokens->value, "//", 2) == 0);
    
    // Multi-line comment
    tokens = tokens->next;
    assert(tokens->type == TOKEN_COMMENT);
    assert(strncmp(tokens->value, "/*", 2) == 0);
    
    lexer_destroy(lexer);
    printf("✓ Comments test passed\n");
}

void test_preprocessor() {
    printf("Testing preprocessor recognition...\n");
    
    const char* source = "#include <stdio.h>\n#define MAX 100\n#ifdef DEBUG";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    // #include
    assert(tokens->type == TOKEN_PREPROCESSOR);
    assert(strncmp(tokens->value, "#include", 8) == 0);
    
    // #define
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PREPROCESSOR);
    assert(strncmp(tokens->value, "#define", 7) == 0);
    
    // #ifdef
    tokens = tokens->next;
    assert(tokens->type == TOKEN_PREPROCESSOR);
    assert(strncmp(tokens->value, "#ifdef", 6) == 0);
    
    lexer_destroy(lexer);
    printf("✓ Preprocessor test passed\n");
}

void test_keywords() {
    printf("Testing keyword recognition...\n");
    
    const char* source = "int float double char void if else while for return";
    LexerState* lexer = lexer_create(source, "test.c");
    
    Token* tokens = lexer_tokenize(lexer);
    
    const char* expected[] = {
        "int", "float", "double", "char", "void", 
        "if", "else", "while", "for", "return"
    };
    
    for (int i = 0; i < 10; i++) {
        assert(tokens->type == TOKEN_KEYWORD);
        assert(strcmp(tokens->value, expected[i]) == 0);
        tokens = tokens->next;
    }
    
    lexer_destroy(lexer);
    printf("✓ Keywords test passed\n");
}

void test_complex_program() {
    printf("Testing complex program tokenization...\n");
    
    const char* source = 
        "#include <stdio.h>\n"
        "int factorial(int n) {\n"
        "    if (n <= 1) return 1;\n"
        "    return n * factorial(n - 1);\n"
        "}\n";
    
    LexerState* lexer = lexer_create(source, "factorial.c");
    Token* tokens = lexer_tokenize(lexer);
    
    // Just verify we can tokenize without errors
    assert(tokens != NULL);
    assert(!lexer_has_errors(lexer));
    
    // Count tokens (should be more than 20)
    int count = 0;
    Token* current = tokens;
    while (current && current->type != TOKEN_EOF) {
        count++;
        current = current->next;
    }
    assert(count > 20);
    
    lexer_destroy(lexer);
    printf("✓ Complex program test passed\n");
}

int main() {
    printf("Running Lexer Tests...\n");
    printf("═══════════════════════════════════════\n");
    
    test_basic_tokens();
    test_numbers();
    test_strings();
    test_operators();
    test_comments();
    test_preprocessor();
    test_keywords();
    test_complex_program();
    
    printf("═══════════════════════════════════════\n");
    printf("All lexer tests passed! ✓\n");
    
    return 0;
}