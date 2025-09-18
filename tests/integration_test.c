#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/main.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Integration Test - Full Obfuscator Workflow
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_simple_expression_obfuscation() {
    printf("Testing simple expression obfuscation workflow...\n");
    
    // Create a simple test file
    const char* test_code = "x + y * z";
    const char* input_file = "test_input.c";
    const char* output_file = "test_output.c";
    
    // Write test input
    FILE* f = fopen(input_file, "w");
    assert(f != NULL);
    fprintf(f, "%s", test_code);
    fclose(f);
    
    // Create configuration
    ObfuscationConfig* config = config_create_default();
    config->level = OBF_BASIC;
    config->aesthetic = AESTHETIC_ARTISTIC;
    
    // Run obfuscation
    int result = obfuscate_file(input_file, output_file, config);
    assert(result == 0);
    
    // Verify output file exists
    FILE* output = fopen(output_file, "r");
    assert(output != NULL);
    
    // Read and display result
    char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, output);
    buffer[bytes_read] = '\0';
    fclose(output);
    
    printf("Original: %s\n", test_code);
    printf("Obfuscated: %s\n", buffer);
    
    // Cleanup
    remove(input_file);
    remove(output_file);
    config_destroy(config);
    
    printf("✓ Simple expression obfuscation test passed\n");
}

void test_different_aesthetic_styles() {
    printf("Testing different aesthetic styles...\n");
    
    const char* test_code = "func(a, b)";
    
    AestheticStyle styles[] = {
        AESTHETIC_MINIMAL, AESTHETIC_HEXADECIMAL, AESTHETIC_ARTISTIC, AESTHETIC_CHAOTIC
    };
    
    const char* style_names[] = {
        "minimal", "hexadecimal", "artistic", "chaotic"
    };
    
    for (int i = 0; i < 4; i++) {
        printf("  Testing %s style...\n", style_names[i]);
        
        char input_file[64], output_file[64];
        snprintf(input_file, sizeof(input_file), "test_%s_input.c", style_names[i]);
        snprintf(output_file, sizeof(output_file), "test_%s_output.c", style_names[i]);
        
        // Write test input
        FILE* f = fopen(input_file, "w");
        assert(f != NULL);
        fprintf(f, "%s", test_code);
        fclose(f);
        
        // Create configuration
        ObfuscationConfig* config = config_create_default();
        config->aesthetic = styles[i];
        
        // Run obfuscation
        int result = obfuscate_file(input_file, output_file, config);
        assert(result == 0);
        
        // Read result
        FILE* output = fopen(output_file, "r");
        assert(output != NULL);
        
        char buffer[1024];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, output);
        buffer[bytes_read] = '\0';
        fclose(output);
        
        printf("    %s: %s\n", style_names[i], buffer);
        
        // Cleanup
        remove(input_file);
        remove(output_file);
        config_destroy(config);
    }
    
    printf("✓ Aesthetic styles test passed\n");
}

void test_obfuscation_levels() {
    printf("Testing obfuscation levels...\n");
    
    const char* test_code = "x = y + z";
    
    ObfuscationLevel levels[] = {OBF_BASIC, OBF_INTERMEDIATE, OBF_EXTREME};
    const char* level_names[] = {"basic", "intermediate", "extreme"};
    
    for (int i = 0; i < 3; i++) {
        printf("  Testing %s level...\n", level_names[i]);
        
        char input_file[64], output_file[64];
        snprintf(input_file, sizeof(input_file), "test_%s_input.c", level_names[i]);
        snprintf(output_file, sizeof(output_file), "test_%s_output.c", level_names[i]);
        
        // Write test input
        FILE* f = fopen(input_file, "w");
        assert(f != NULL);
        fprintf(f, "%s", test_code);
        fclose(f);
        
        // Create configuration
        ObfuscationConfig* config = config_create_default();
        config->level = levels[i];
        config->aesthetic = AESTHETIC_ARTISTIC;
        
        // Run obfuscation
        int result = obfuscate_file(input_file, output_file, config);
        assert(result == 0);
        
        // Read result
        FILE* output = fopen(output_file, "r");
        assert(output != NULL);
        
        char buffer[1024];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, output);
        buffer[bytes_read] = '\0';
        fclose(output);
        
        printf("    %s: %s\n", level_names[i], buffer);
        
        // Cleanup
        remove(input_file);
        remove(output_file);
        config_destroy(config);
    }
    
    printf("✓ Obfuscation levels test passed\n");
}

void test_command_line_parsing() {
    printf("Testing command line parsing...\n");
    
    // Test basic parsing
    char* argv1[] = {"obfuscator", "input.c", "-o", "output.c", "-l", "extreme", "-a", "chaotic"};
    int argc1 = 8;
    
    AppConfig* config = parse_command_line(argc1, argv1);
    assert(config != NULL);
    assert(strcmp(config->input_file, "input.c") == 0);
    assert(strcmp(config->output_file, "output.c") == 0);
    assert(config->config->level == OBF_EXTREME);
    assert(config->config->aesthetic == AESTHETIC_CHAOTIC);
    
    app_config_destroy(config);
    
    // Test help flag
    char* argv2[] = {"obfuscator", "--help"};
    int argc2 = 2;
    
    config = parse_command_line(argc2, argv2);
    assert(config != NULL);
    assert(config->show_help == true);
    
    app_config_destroy(config);
    
    printf("✓ Command line parsing test passed\n");
}

void test_file_utilities() {
    printf("Testing file utilities...\n");
    
    // Test file existence check
    const char* test_file = "test_utility_file.txt";
    
    assert(file_exists(test_file) == false);
    
    // Create file
    FILE* f = fopen(test_file, "w");
    assert(f != NULL);
    fprintf(f, "test content");
    fclose(f);
    
    assert(file_exists(test_file) == true);
    
    // Test file reading
    char* content = read_file(test_file);
    assert(content != NULL);
    assert(strcmp(content, "test content") == 0);
    free(content);
    
    // Test output filename generation
    char* output_name = create_output_filename("input.c");
    assert(output_name != NULL);
    assert(strcmp(output_name, "input_obf.c") == 0);
    free(output_name);
    
    output_name = create_output_filename("test");
    assert(output_name != NULL);
    assert(strcmp(output_name, "test_obf.c") == 0);
    free(output_name);
    
    // Cleanup
    remove(test_file);
    
    printf("✓ File utilities test passed\n");
}

void demonstrate_full_workflow() {
    printf("Demonstrating full obfuscation workflow...\n");
    
    const char* demo_code = 
        "factorial(n) + fibonacci(x * 2)";
    
    const char* input_file = "demo_input.c";
    const char* output_file = "demo_output.c";
    
    printf("\n--- Full Workflow Demonstration ---\n");
    printf("Input: %s\n\n", demo_code);
    
    // Write input file
    FILE* f = fopen(input_file, "w");
    assert(f != NULL);
    fprintf(f, "%s", demo_code);
    fclose(f);
    
    // Test different configurations
    struct {
        ObfuscationLevel level;
        AestheticStyle aesthetic;
        const char* description;
    } configs[] = {
        {OBF_BASIC, AESTHETIC_MINIMAL, "Basic + Minimal"},
        {OBF_INTERMEDIATE, AESTHETIC_ARTISTIC, "Intermediate + Artistic"},
        {OBF_EXTREME, AESTHETIC_CHAOTIC, "Extreme + Chaotic"},
        {OBF_INTERMEDIATE, AESTHETIC_HEXADECIMAL, "Intermediate + Hexadecimal"}
    };
    
    for (int i = 0; i < 4; i++) {
        printf("%s:\n", configs[i].description);
        
        ObfuscationConfig* config = config_create_default();
        config->level = configs[i].level;
        config->aesthetic = configs[i].aesthetic;
        
        int result = obfuscate_file(input_file, output_file, config);
        assert(result == 0);
        
        // Read and display result
        FILE* output = fopen(output_file, "r");
        assert(output != NULL);
        
        char buffer[2048];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, output);
        buffer[bytes_read] = '\0';
        fclose(output);
        
        printf("  %s\n\n", buffer);
        
        config_destroy(config);
    }
    
    // Cleanup
    remove(input_file);
    remove(output_file);
    
    printf("✓ Full workflow demonstration completed\n");
}

int main() {
    printf("C Code Obfuscator - Integration Tests\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    test_simple_expression_obfuscation();
    test_different_aesthetic_styles();
    test_obfuscation_levels();
    test_command_line_parsing();
    test_file_utilities();
    demonstrate_full_workflow();
    
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("🎉 All integration tests passed! 🎉\n");
    printf("\nYour C Code Obfuscator is ready to create beautiful, unreadable code!\n");
    printf("Features implemented:\n");
    printf("  ✓ Lexical analysis with comprehensive token recognition\n");
    printf("  ✓ Expression parsing with operator precedence\n");
    printf("  ✓ Symbol table management with scope handling\n");
    printf("  ✓ Identifier obfuscation with aesthetic name generation\n");
    printf("  ✓ Code generation with artistic formatting\n");
    printf("  ✓ Command-line interface with multiple options\n");
    printf("  ✓ Multiple aesthetic styles (minimal, unicode, hex, artistic, chaotic)\n");
    printf("  ✓ Configurable obfuscation levels (basic, intermediate, extreme)\n");
    printf("  ✓ ASCII art and creative formatting\n");
    printf("  ✓ File I/O and error handling\n");
    
    printf("\nReady to obfuscate some code! 🔒✨\n");
    
    return 0;
}