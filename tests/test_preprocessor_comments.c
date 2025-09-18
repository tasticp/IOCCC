#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/lexer/lexer.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Test Cases for Enhanced Preprocessor and Comment Handling
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_basic_preprocessor_directives() {
    printf("Testing basic preprocessor directives...\n");
    
    const char* source = 
        "#include <stdio.h>\n"
        "#define MAX_SIZE 100\n"
        "#ifdef DEBUG\n"
        "#define LOG(x) printf(x)\n"
        "#else\n"
        "#define LOG(x)\n"
        "#endif\n"
        "#undef MAX_SIZE\n"
        "#pragma once\n"
        "#line 42 \"test.c\"\n";
    
    LexerState* lexer = lexer_create(source, "test_preprocessor.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Count preprocessor tokens
    int pp_count = 0;
    Token* current = tokens;
    while (current && current->type != TOKEN_EOF) {
        if (current->type == TOKEN_PREPROCESSOR) {
            pp_count++;
            printf("  Found: %s\n", current->value);
        }
        current = current->next;
    }
    
    assert(pp_count == 9); // Should find 9 preprocessor directives
    assert(!lexer_has_errors(lexer)); // Should have no errors
    
    lexer_destroy(lexer);
    printf("✓ Basic preprocessor directives test passed\n");
}

void test_unknown_preprocessor_directive() {
    printf("Testing unknown preprocessor directive...\n");
    
    const char* source = "#unknown_directive some_value\n";
    
    LexerState* lexer = lexer_create(source, "test_unknown.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Should have warnings for unknown directive
    assert(lexer_has_errors(lexer));
    
    Error* errors = lexer_get_errors(lexer);
    assert(errors != NULL);
    printf("  Warning: %s\n", errors->message);
    
    lexer_destroy(lexer);
    printf("✓ Unknown preprocessor directive test passed\n");
}

void test_line_continuation() {
    printf("Testing line continuation in preprocessor...\n");
    
    const char* source = 
        "#define LONG_MACRO(x, y) \\\n"
        "    do { \\\n"
        "        printf(\"x=%d, y=%d\\n\", x, y); \\\n"
        "    } while(0)\n";
    
    LexerState* lexer = lexer_create(source, "test_continuation.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Find the preprocessor token
    Token* current = tokens;
    while (current && current->type != TOKEN_PREPROCESSOR) {
        current = current->next;
    }
    
    assert(current != NULL);
    printf("  Macro with continuation: %s\n", current->value);
    
    // Should contain the entire macro definition including continuations
    assert(strstr(current->value, "LONG_MACRO") != NULL);
    assert(strstr(current->value, "while(0)") != NULL);
    
    lexer_destroy(lexer);
    printf("✓ Line continuation test passed\n");
}

void test_single_line_comments() {
    printf("Testing single-line comments...\n");
    
    const char* source = 
        "int x = 5; // This is a comment\n"
        "// Another comment\n"
        "int y = 10; // Comment with special chars: @#$%^&*()\n";
    
    LexerState* lexer = lexer_create(source, "test_single_comments.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Count comment tokens
    int comment_count = 0;
    Token* current = tokens;
    while (current && current->type != TOKEN_EOF) {
        if (current->type == TOKEN_COMMENT) {
            comment_count++;
            printf("  Comment: %s\n", current->value);
        }
        current = current->next;
    }
    
    assert(comment_count == 3); // Should find 3 comments
    assert(!lexer_has_errors(lexer)); // Should have no errors
    
    lexer_destroy(lexer);
    printf("✓ Single-line comments test passed\n");
}

void test_multi_line_comments() {
    printf("Testing multi-line comments...\n");
    
    const char* source = 
        "/* This is a\n"
        "   multi-line comment\n"
        "   with multiple lines */\n"
        "int x = 5;\n"
        "/* Another comment */ int y = 10;\n"
        "/* Comment with special chars: @#$%^&*() */\n";
    
    LexerState* lexer = lexer_create(source, "test_multi_comments.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Count comment tokens
    int comment_count = 0;
    Token* current = tokens;
    while (current && current->type != TOKEN_EOF) {
        if (current->type == TOKEN_COMMENT) {
            comment_count++;
            printf("  Comment: %s\n", current->value);
        }
        current = current->next;
    }
    
    assert(comment_count == 3); // Should find 3 comments
    assert(!lexer_has_errors(lexer)); // Should have no errors
    
    lexer_destroy(lexer);
    printf("✓ Multi-line comments test passed\n");
}

void test_unterminated_comment() {
    printf("Testing unterminated comment...\n");
    
    const char* source = 
        "int x = 5;\n"
        "/* This comment is never closed\n"
        "   and goes to end of file";
    
    LexerState* lexer = lexer_create(source, "test_unterminated.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Should have error for unterminated comment
    assert(lexer_has_errors(lexer));
    
    Error* errors = lexer_get_errors(lexer);
    assert(errors != NULL);
    printf("  Error: %s\n", errors->message);
    assert(strstr(errors->message, "Unterminated") != NULL);
    
    lexer_destroy(lexer);
    printf("✓ Unterminated comment test passed\n");
}

void test_mixed_comments_and_preprocessor() {
    printf("Testing mixed comments and preprocessor...\n");
    
    const char* source = 
        "// Header comment\n"
        "#include <stdio.h> // Include standard I/O\n"
        "/* Multi-line comment\n"
        "   before define */\n"
        "#define MAX 100 /* Inline comment */\n"
        "// End comment\n";
    
    LexerState* lexer = lexer_create(source, "test_mixed.c");
    assert(lexer != NULL);
    
    Token* tokens = lexer_tokenize(lexer);
    assert(tokens != NULL);
    
    // Count different token types
    int comment_count = 0;
    int pp_count = 0;
    Token* current = tokens;
    while (current && current->type != TOKEN_EOF) {
        if (current->type == TOKEN_COMMENT) {
            comment_count++;
        } else if (current->type == TOKEN_PREPROCESSOR) {
            pp_count++;
        }
        current = current->next;
    }
    
    printf("  Found %d comments and %d preprocessor directives\n", comment_count, pp_count);
    assert(comment_count == 4); // Should find 4 comments
    assert(pp_count == 2); // Should find 2 preprocessor directives
    assert(!lexer_has_errors(lexer)); // Should have no errors
    
    lexer_destroy(lexer);
    printf("✓ Mixed comments and preprocessor test passed\n");
}

int main() {
    printf("═══════════════════════════════════════════════════════════════════════════\n");
    printf("Running Enhanced Preprocessor and Comment Tests\n");
    printf("═══════════════════════════════════════════════════════════════════════════\n");
    
    test_basic_preprocessor_directives();
    test_unknown_preprocessor_directive();
    test_line_continuation();
    test_single_line_comments();
    test_multi_line_comments();
    test_unterminated_comment();
    test_mixed_comments_and_preprocessor();
    
    printf("\n✅ All preprocessor and comment tests passed!\n");
    printf("Task 2.2: Handle preprocessor directives and comments - COMPLETED\n");
    
    return 0;
}