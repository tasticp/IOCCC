#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/obfuscator/obfuscator.h"
#include "../src/lexer/lexer.h"
#include "../src/parser/parser.h"
#include "../src/codegen/codegen.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Obfuscator Integration Tests
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_identifier_obfuscation() {
    printf("Testing identifier obfuscation...\n");
    
    const char* source = "x + y * z";
    
    // Tokenize
    LexerState* lexer = lexer_create(source, "test.c");
    Token* tokens = lexer_tokenize(lexer);
    
    // Parse
    ParserState* parser = parser_create(tokens);
    ASTNode* ast = parser_parse_expression(parser);
    
    // Create obfuscation config
    ObfuscationConfig* config = config_create_default();
    config->level = OBF_BASIC;
    config->aesthetic = AESTHETIC_HEXADECIMAL;
    
    // Obfuscate
    ObfuscationContext* ctx = obfuscator_create(config);
    ASTNode* obfuscated_ast = obfuscate_ast(ctx, ast);
    
    assert(obfuscated_ast != NULL);
    assert(!obfuscator_has_errors(ctx));
    
    // Generate code
    CodeGenConfig* codegen_config = codegen_config_create_default();
    CodeGenState* codegen = codegen_create(codegen_config);
    char* obfuscated_code = generate_code(codegen, obfuscated_ast);
    
    assert(obfuscated_code != NULL);
    printf("Original: %s\n", source);
    printf("Obfuscated: %s\n", obfuscated_code);
    
    // Cleanup
    free(obfuscated_code);
    codegen_destroy(codegen);
    codegen_config_destroy(codegen_config);
    obfuscator_destroy(ctx);
    config_destroy(config);
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    printf("✓ Identifier obfuscation test passed\n");
}

void test_aesthetic_styles() {
    printf("Testing aesthetic styles...\n");
    
    const char* source = "func(a, b)";
    
    AestheticStyle styles[] = {
        AESTHETIC_MINIMAL, AESTHETIC_UNICODE, AESTHETIC_HEXADECIMAL,
        AESTHETIC_ARTISTIC, AESTHETIC_CHAOTIC
    };
    
    const char* style_names[] = {
        "Minimal", "Unicode", "Hexadecimal", "Artistic", "Chaotic"
    };
    
    for (int i = 0; i < 5; i++) {
        printf("  Testing %s style...\n", style_names[i]);
        
        // Tokenize
        LexerState* lexer = lexer_create(source, "test.c");
        Token* tokens = lexer_tokenize(lexer);
        
        // Parse
        ParserState* parser = parser_create(tokens);
        ASTNode* ast = parser_parse_expression(parser);
        
        // Create obfuscation config
        ObfuscationConfig* config = config_create_default();
        config->aesthetic = styles[i];
        
        // Obfuscate
        ObfuscationContext* ctx = obfuscator_create(config);
        ASTNode* obfuscated_ast = obfuscate_ast(ctx, ast);
        
        assert(obfuscated_ast != NULL);
        
        // Generate code
        CodeGenConfig* codegen_config = codegen_config_create_default();
        codegen_config_set_style(codegen_config, styles[i]);
        CodeGenState* codegen = codegen_create(codegen_config);
        char* obfuscated_code = generate_code(codegen, obfuscated_ast);
        
        printf("    %s: %s\n", style_names[i], obfuscated_code);
        
        // Cleanup
        free(obfuscated_code);
        codegen_destroy(codegen);
        codegen_config_destroy(codegen_config);
        obfuscator_destroy(ctx);
        config_destroy(config);
        parser_destroy(parser);
        lexer_destroy(lexer);
    }
    
    printf("✓ Aesthetic styles test passed\n");
}

void test_obfuscation_levels() {
    printf("Testing obfuscation levels...\n");
    
    const char* source = "x = y + z * 2";
    
    ObfuscationLevel levels[] = {OBF_BASIC, OBF_INTERMEDIATE, OBF_EXTREME};
    const char* level_names[] = {"Basic", "Intermediate", "Extreme"};
    
    for (int i = 0; i < 3; i++) {
        printf("  Testing %s level...\n", level_names[i]);
        
        // Tokenize
        LexerState* lexer = lexer_create(source, "test.c");
        Token* tokens = lexer_tokenize(lexer);
        
        // Parse
        ParserState* parser = parser_create(tokens);
        ASTNode* ast = parser_parse_expression(parser);
        
        // Create obfuscation config
        ObfuscationConfig* config = config_create_default();
        config->level = levels[i];
        config->aesthetic = AESTHETIC_ARTISTIC;
        
        // Obfuscate
        ObfuscationContext* ctx = obfuscator_create(config);
        ASTNode* obfuscated_ast = obfuscate_ast(ctx, ast);
        
        assert(obfuscated_ast != NULL);
        
        // Generate code
        CodeGenConfig* codegen_config = codegen_config_create_default();
        CodeGenState* codegen = codegen_create(codegen_config);
        char* obfuscated_code = generate_code(codegen, obfuscated_ast);
        
        printf("    %s: %s\n", level_names[i], obfuscated_code);
        
        // Cleanup
        free(obfuscated_code);
        codegen_destroy(codegen);
        codegen_config_destroy(codegen_config);
        obfuscator_destroy(ctx);
        config_destroy(config);
        parser_destroy(parser);
        lexer_destroy(lexer);
    }
    
    printf("✓ Obfuscation levels test passed\n");
}

void test_name_generation() {
    printf("Testing name generation...\n");
    
    // Test different aesthetic name generation
    for (int i = 0; i < 10; i++) {\n        char* minimal = generate_aesthetic_name_advanced(AESTHETIC_MINIMAL, i);\n        char* hex = generate_aesthetic_name_advanced(AESTHETIC_HEXADECIMAL, i);\n        char* artistic = generate_aesthetic_name_advanced(AESTHETIC_ARTISTIC, i);\n        \n        printf(\"  %d: Minimal='%s', Hex='%s', Artistic='%s'\\n\", \n               i, minimal, hex, artistic);\n        \n        assert(minimal != NULL);\n        assert(hex != NULL);\n        assert(artistic != NULL);\n        \n        free(minimal);\n        free(hex);\n        free(artistic);\n    }\n    \n    printf(\"✓ Name generation test passed\\n\");\n}\n\nvoid test_complex_expression() {\n    printf(\"Testing complex expression obfuscation...\\n\");\n    \n    const char* source = \"func(a * b + c, array[index], x->member)\";\n    \n    // Tokenize\n    LexerState* lexer = lexer_create(source, \"test.c\");\n    Token* tokens = lexer_tokenize(lexer);\n    \n    // Parse\n    ParserState* parser = parser_create(tokens);\n    ASTNode* ast = parser_parse_expression(parser);\n    \n    // Create obfuscation config\n    ObfuscationConfig* config = config_create_default();\n    config->level = OBF_EXTREME;\n    config->aesthetic = AESTHETIC_CHAOTIC;\n    \n    // Obfuscate\n    ObfuscationContext* ctx = obfuscator_create(config);\n    ASTNode* obfuscated_ast = obfuscate_ast(ctx, ast);\n    \n    assert(obfuscated_ast != NULL);\n    \n    // Generate code with artistic formatting\n    CodeGenConfig* codegen_config = codegen_config_create_default();\n    codegen_config->add_ascii_art = true;\n    codegen_config->add_comments = true;\n    codegen_config_set_style(codegen_config, AESTHETIC_CHAOTIC);\n    \n    CodeGenState* codegen = codegen_create(codegen_config);\n    char* obfuscated_code = generate_code(codegen, obfuscated_ast);\n    \n    printf(\"Original: %s\\n\", source);\n    printf(\"Obfuscated:\\n%s\\n\", obfuscated_code);\n    \n    // Cleanup\n    free(obfuscated_code);\n    codegen_destroy(codegen);\n    codegen_config_destroy(codegen_config);\n    obfuscator_destroy(ctx);\n    config_destroy(config);\n    parser_destroy(parser);\n    lexer_destroy(lexer);\n    \n    printf(\"✓ Complex expression test passed\\n\");\n}\n\nvoid test_full_pipeline() {\n    printf(\"Testing full obfuscation pipeline...\\n\");\n    \n    const char* source = \"factorial(n - 1) * n\";\n    \n    printf(\"Input: %s\\n\", source);\n    printf(\"\\nProcessing through full pipeline...\\n\");\n    \n    // Step 1: Tokenize\n    printf(\"1. Tokenizing...\\n\");\n    LexerState* lexer = lexer_create(source, \"test.c\");\n    Token* tokens = lexer_tokenize(lexer);\n    assert(tokens != NULL);\n    \n    // Step 2: Parse\n    printf(\"2. Parsing...\\n\");\n    ParserState* parser = parser_create(tokens);\n    ASTNode* ast = parser_parse_expression(parser);\n    assert(ast != NULL);\n    \n    // Step 3: Obfuscate\n    printf(\"3. Obfuscating...\\n\");\n    ObfuscationConfig* config = config_create_default();\n    config->level = OBF_EXTREME;\n    config->aesthetic = AESTHETIC_ARTISTIC;\n    \n    ObfuscationContext* ctx = obfuscator_create(config);\n    ASTNode* obfuscated_ast = obfuscate_ast(ctx, ast);\n    assert(obfuscated_ast != NULL);\n    \n    // Step 4: Generate\n    printf(\"4. Generating code...\\n\");\n    CodeGenConfig* codegen_config = codegen_config_create_default();\n    codegen_config->add_ascii_art = true;\n    codegen_config->pretty_print = true;\n    \n    CodeGenState* codegen = codegen_create(codegen_config);\n    char* obfuscated_code = generate_code(codegen, obfuscated_ast);\n    assert(obfuscated_code != NULL);\n    \n    printf(\"\\n5. Final Result:\\n\");\n    printf(\"%s\\n\", obfuscated_code);\n    \n    // Cleanup\n    free(obfuscated_code);\n    codegen_destroy(codegen);\n    codegen_config_destroy(codegen_config);\n    obfuscator_destroy(ctx);\n    config_destroy(config);\n    parser_destroy(parser);\n    lexer_destroy(lexer);\n    \n    printf(\"✓ Full pipeline test passed\\n\");\n}\n\nint main() {\n    printf(\"Running Obfuscator Integration Tests...\\n\");\n    printf(\"═══════════════════════════════════════════════════════════════\\n\");\n    \n    test_identifier_obfuscation();\n    test_aesthetic_styles();\n    test_obfuscation_levels();\n    test_name_generation();\n    test_complex_expression();\n    test_full_pipeline();\n    \n    printf(\"═══════════════════════════════════════════════════════════════\\n\");\n    printf(\"All obfuscator tests passed! ✓\\n\");\n    printf(\"\\nYour C code obfuscator is ready to create beautiful chaos!\\n\");\n    \n    return 0;\n}"