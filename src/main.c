#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Windows compatibility for getopt */
#ifdef _WIN32
#include "getopt_win.h"
#else
#include <getopt.h>
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * Application Information
 * ═══════════════════════════════════════════════════════════════════════════ */

#define VERSION "1.0.0"
#define PROGRAM_NAME "obfuscator"

/* ═══════════════════════════════════════════════════════════════════════════
 * Command Line Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS] INPUT_FILE\n\n", program_name);
    printf("A sophisticated C code obfuscator that transforms readable source code\n");
    printf("into highly unreadable yet functionally equivalent code.\n\n");
    
    printf("OPTIONS:\n");
    printf("  -o, --output FILE     Output file (default: input_obf.c)\n");
    printf("  -l, --level LEVEL     Obfuscation level: basic, intermediate, extreme\n");
    printf("                        (default: intermediate)\n");
    printf("  -a, --aesthetic STYLE Aesthetic style: minimal, unicode, hex, artistic, chaotic\n");
    printf("                        (default: artistic)\n");
    printf("  -d, --debug           Preserve debug information\n");
    printf("  -s, --strings         Obfuscate string literals (default: enabled)\n");
    printf("  -c, --control-flow    Obfuscate control flow (default: enabled)\n");
    printf("  -m, --macros          Use macro obfuscation (default: enabled)\n");
    printf("  -v, --verbose         Verbose output\n");
    printf("  -h, --help            Show this help message\n");
    printf("      --version         Show version information\n\n");
    
    printf("OBFUSCATION LEVELS:\n");
    printf("  basic        - Identifier renaming and simple formatting\n");
    printf("  intermediate - Expression transformation and control flow modifications\n");
    printf("  extreme      - Complete control flow flattening and dead code insertion\n\n");
    
    printf("AESTHETIC STYLES:\n");
    printf("  minimal      - Clean, simple obfuscated names\n");
    printf("  unicode      - Unicode characters: ℓ𝓁𝓵𝓁, 𝒶𝒷𝒸\n");
    printf("  hex          - Hexadecimal patterns: _0x41414141, __0xDEADBEEF\n");
    printf("  artistic     - Creative patterns: __aesthetic_var_001\n");
    printf("  chaotic      - Maximum visual chaos with mixed patterns\n\n");
    
    printf("EXAMPLES:\n");
    printf("  %s input.c                           # Basic obfuscation\n", program_name);
    printf("  %s -l extreme -a chaotic input.c     # Maximum chaos\n", program_name);
    printf("  %s -o output.c -d input.c            # With debug info\n", program_name);
    printf("  %s --level artistic input.c          # Artistic style\n\n", program_name);
}

void print_version(void) {
    printf("%s version %s\n", PROGRAM_NAME, VERSION);
    printf("A sophisticated C code obfuscator\n");
    printf("Copyright (c) 2024. Licensed under MIT License.\n");
}

void print_help(void) {
    print_usage(PROGRAM_NAME);
}

static ObfuscationLevel parse_obfuscation_level(const char* level_str) {
    if (!level_str) return OBF_INTERMEDIATE;
    
    if (strcmp(level_str, "basic") == 0) return OBF_BASIC;
    if (strcmp(level_str, "intermediate") == 0) return OBF_INTERMEDIATE;
    if (strcmp(level_str, "extreme") == 0) return OBF_EXTREME;
    
    fprintf(stderr, "Warning: Unknown obfuscation level '%s', using 'intermediate'\n", level_str);
    return OBF_INTERMEDIATE;
}

static AestheticStyle parse_aesthetic_style(const char* style_str) {
    if (!style_str) return AESTHETIC_ARTISTIC;
    
    if (strcmp(style_str, "minimal") == 0) return AESTHETIC_MINIMAL;
    if (strcmp(style_str, "unicode") == 0) return AESTHETIC_UNICODE;
    if (strcmp(style_str, "hex") == 0) return AESTHETIC_HEXADECIMAL;
    if (strcmp(style_str, "artistic") == 0) return AESTHETIC_ARTISTIC;
    if (strcmp(style_str, "chaotic") == 0) return AESTHETIC_CHAOTIC;
    
    fprintf(stderr, "Warning: Unknown aesthetic style '%s', using 'artistic'\n", style_str);
    return AESTHETIC_ARTISTIC;
}

AppConfig* parse_command_line(int argc, char* argv[]) {
    AppConfig* config = app_config_create_default();
    if (!config) return NULL;
    
    static struct option long_options[] = {
        {"output",       required_argument, 0, 'o'},
        {"level",        required_argument, 0, 'l'},
        {"aesthetic",    required_argument, 0, 'a'},
        {"debug",        no_argument,       0, 'd'},
        {"strings",      no_argument,       0, 's'},
        {"control-flow", no_argument,       0, 'c'},
        {"macros",       no_argument,       0, 'm'},
        {"verbose",      no_argument,       0, 'v'},
        {"help",         no_argument,       0, 'h'},
        {"version",      no_argument,       0, 1000},
        {0, 0, 0, 0}
    };\n    \n    int option_index = 0;\n    int c;\n    \n    while ((c = getopt_long(argc, argv, \"o:l:a:dscmvh\", long_options, &option_index)) != -1) {\n        switch (c) {\n            case 'o':\n                free(config->output_file);\n                config->output_file = strdup(optarg);\n                break;\n                \n            case 'l':\n                config->config->level = parse_obfuscation_level(optarg);\n                break;\n                \n            case 'a':\n                config->config->aesthetic = parse_aesthetic_style(optarg);\n                config_set_aesthetic(config->config, config->config->aesthetic);\n                break;\n                \n            case 'd':\n                config->config->preserve_debug_info = true;\n                break;\n                \n            case 's':\n                config->config->obfuscate_strings = true;\n                break;\n                \n            case 'c':\n                config->config->obfuscate_control_flow = true;\n                break;\n                \n            case 'm':\n                config->config->use_macros = true;\n                break;\n                \n            case 'v':\n                config->verbose = true;\n                break;\n                \n            case 'h':\n                config->show_help = true;\n                return config;\n                \n            case 1000: // --version\n                print_version();\n                exit(0);\n                break;\n                \n            case '?':\n                fprintf(stderr, \"Try '%s --help' for more information.\\n\", argv[0]);\n                app_config_destroy(config);\n                return NULL;\n                \n            default:\n                break;\n        }\n    }\n    \n    // Get input file\n    if (optind < argc) {\n        config->input_file = strdup(argv[optind]);\n    } else if (!config->show_help) {\n        fprintf(stderr, \"Error: No input file specified\\n\");\n        fprintf(stderr, \"Try '%s --help' for more information.\\n\", argv[0]);\n        app_config_destroy(config);\n        return NULL;\n    }\n    \n    // Generate output filename if not specified\n    if (!config->output_file && config->input_file) {\n        config->output_file = create_output_filename(config->input_file);\n    }\n    \n    return config;\n}\n\n/* ═══════════════════════════════════════════════════════════════════════════\n * File I/O Functions\n * ═══════════════════════════════════════════════════════════════════════════ */\n\nchar* read_file(const char* filename) {\n    if (!filename) return NULL;\n    \n    FILE* file = fopen(filename, \"r\");\n    if (!file) {\n        fprintf(stderr, \"Error: Cannot open file '%s'\\n\", filename);\n        return NULL;\n    }\n    \n    // Get file size\n    fseek(file, 0, SEEK_END);\n    long size = ftell(file);\n    fseek(file, 0, SEEK_SET);\n    \n    if (size < 0) {\n        fprintf(stderr, \"Error: Cannot determine file size for '%s'\\n\", filename);\n        fclose(file);\n        return NULL;\n    }\n    \n    // Allocate buffer\n    char* content = malloc(size + 1);\n    if (!content) {\n        fprintf(stderr, \"Error: Cannot allocate memory for file '%s'\\n\", filename);\n        fclose(file);\n        return NULL;\n    }\n    \n    // Read file\n    size_t bytes_read = fread(content, 1, size, file);\n    content[bytes_read] = '\\0';\n    \n    fclose(file);\n    return content;\n}\n\nbool write_file(const char* filename, const char* content) {\n    if (!filename || !content) return false;\n    \n    FILE* file = fopen(filename, \"w\");\n    if (!file) {\n        fprintf(stderr, \"Error: Cannot create file '%s'\\n\", filename);\n        return false;\n    }\n    \n    size_t len = strlen(content);\n    size_t written = fwrite(content, 1, len, file);\n    \n    fclose(file);\n    \n    if (written != len) {\n        fprintf(stderr, \"Error: Failed to write complete content to '%s'\\n\", filename);\n        return false;\n    }\n    \n    return true;\n}\n\n/* ═══════════════════════════════════════════════════════════════════════════\n * Main Obfuscation Function\n * ═══════════════════════════════════════════════════════════════════════════ */\n\nint obfuscate_file(const char* input_file, const char* output_file, ObfuscationConfig* config) {\n    if (!input_file || !output_file || !config) {\n        fprintf(stderr, \"Error: Invalid parameters\\n\");\n        return 1;\n    }\n    \n    printf(\"Obfuscating '%s' -> '%s'\\n\", input_file, output_file);\n    printf(\"Level: %s, Style: %s\\n\", \n           (config->level == OBF_BASIC) ? \"basic\" :\n           (config->level == OBF_INTERMEDIATE) ? \"intermediate\" : \"extreme\",\n           (config->aesthetic == AESTHETIC_MINIMAL) ? \"minimal\" :\n           (config->aesthetic == AESTHETIC_UNICODE) ? \"unicode\" :\n           (config->aesthetic == AESTHETIC_HEXADECIMAL) ? \"hex\" :\n           (config->aesthetic == AESTHETIC_ARTISTIC) ? \"artistic\" : \"chaotic\");\n    \n    // Step 1: Read input file\n    char* source_code = read_file(input_file);\n    if (!source_code) {\n        return 1;\n    }\n    \n    // Step 2: Tokenize\n    printf(\"Tokenizing...\\n\");\n    LexerState* lexer = lexer_create(source_code, input_file);\n    if (!lexer) {\n        fprintf(stderr, \"Error: Failed to create lexer\\n\");\n        free(source_code);\n        return 1;\n    }\n    \n    Token* tokens = lexer_tokenize(lexer);\n    if (!tokens || lexer_has_errors(lexer)) {\n        fprintf(stderr, \"Error: Tokenization failed\\n\");\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    // Step 3: Parse (for now, just create a simple expression AST)\n    printf(\"Parsing...\\n\");\n    ParserState* parser = parser_create(tokens);\n    if (!parser) {\n        fprintf(stderr, \"Error: Failed to create parser\\n\");\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    ASTNode* ast = parser_parse_expression(parser); // Simplified for now\n    if (!ast) {\n        fprintf(stderr, \"Error: Parsing failed\\n\");\n        parser_destroy(parser);\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    // Step 4: Obfuscate\n    printf(\"Obfuscating...\\n\");\n    ObfuscationContext* obf_ctx = obfuscator_create(config);\n    if (!obf_ctx) {\n        fprintf(stderr, \"Error: Failed to create obfuscator\\n\");\n        ast_node_destroy(ast);\n        parser_destroy(parser);\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    ASTNode* obfuscated_ast = obfuscate_ast(obf_ctx, ast);\n    if (!obfuscated_ast) {\n        fprintf(stderr, \"Error: Obfuscation failed\\n\");\n        obfuscator_destroy(obf_ctx);\n        ast_node_destroy(ast);\n        parser_destroy(parser);\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    // Step 5: Generate code\n    printf(\"Generating code...\\n\");\n    CodeGenConfig* codegen_config = codegen_config_create_default();\n    codegen_config_set_style(codegen_config, config->aesthetic);\n    \n    CodeGenState* codegen = codegen_create(codegen_config);\n    if (!codegen) {\n        fprintf(stderr, \"Error: Failed to create code generator\\n\");\n        codegen_config_destroy(codegen_config);\n        obfuscator_destroy(obf_ctx);\n        ast_node_destroy(ast);\n        parser_destroy(parser);\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    char* obfuscated_code = generate_code(codegen, obfuscated_ast);\n    if (!obfuscated_code) {\n        fprintf(stderr, \"Error: Code generation failed\\n\");\n        codegen_destroy(codegen);\n        codegen_config_destroy(codegen_config);\n        obfuscator_destroy(obf_ctx);\n        ast_node_destroy(ast);\n        parser_destroy(parser);\n        lexer_destroy(lexer);\n        free(source_code);\n        return 1;\n    }\n    \n    // Step 6: Write output\n    printf(\"Writing output...\\n\");\n    bool success = write_file(output_file, obfuscated_code);\n    \n    // Cleanup\n    free(obfuscated_code);\n    codegen_destroy(codegen);\n    codegen_config_destroy(codegen_config);\n    obfuscator_destroy(obf_ctx);\n    ast_node_destroy(ast);\n    parser_destroy(parser);\n    lexer_destroy(lexer);\n    free(source_code);\n    \n    if (success) {\n        printf(\"✓ Obfuscation completed successfully!\\n\");\n        printf(\"Output written to: %s\\n\", output_file);\n        return 0;\n    } else {\n        fprintf(stderr, \"Error: Failed to write output file\\n\");\n        return 1;\n    }\n}\n\n/* ═══════════════════════════════════════════════════════════════════════════\n * Configuration Management\n * ═══════════════════════════════════════════════════════════════════════════ */\n\nAppConfig* app_config_create_default(void) {\n    AppConfig* config = malloc(sizeof(AppConfig));\n    if (!config) return NULL;\n    \n    config->config = config_create_default();\n    config->codegen_config = codegen_config_create_default();\n    config->input_file = NULL;\n    config->output_file = NULL;\n    config->verbose = false;\n    config->show_help = false;\n    \n    return config;\n}\n\nvoid app_config_destroy(AppConfig* config) {\n    if (!config) return;\n    \n    config_destroy(config->config);\n    codegen_config_destroy(config->codegen_config);\n    free(config->input_file);\n    free(config->output_file);\n    free(config);\n}\n\n/* ═══════════════════════════════════════════════════════════════════════════\n * Utility Functions\n * ═══════════════════════════════════════════════════════════════════════════ */\n\nbool file_exists(const char* filename) {\n    if (!filename) return false;\n    \n    FILE* file = fopen(filename, \"r\");\n    if (file) {\n        fclose(file);\n        return true;\n    }\n    return false;\n}\n\nchar* get_file_extension(const char* filename) {\n    if (!filename) return NULL;\n    \n    const char* dot = strrchr(filename, '.');\n    if (!dot || dot == filename) return NULL;\n    \n    return strdup(dot + 1);\n}\n\nchar* create_output_filename(const char* input_file) {\n    if (!input_file) return NULL;\n    \n    size_t len = strlen(input_file);\n    const char* dot = strrchr(input_file, '.');\n    \n    char* output_file;\n    if (dot) {\n        size_t base_len = dot - input_file;\n        output_file = malloc(base_len + 8); // \"_obf.c\" + null terminator\n        if (output_file) {\n            strncpy(output_file, input_file, base_len);\n            strcpy(output_file + base_len, \"_obf.c\");\n        }\n    } else {\n        output_file = malloc(len + 8);\n        if (output_file) {\n            strcpy(output_file, input_file);\n            strcat(output_file, \"_obf.c\");\n        }\n    }\n    \n    return output_file;\n}\n\nvoid print_errors(Error* errors) {\n    // TODO: Implement error printing\n    (void)errors;\n}\n\nvoid cleanup_and_exit(int exit_code) {\n    // TODO: Implement cleanup\n    exit(exit_code);\n}\n\n/* ═══════════════════════════════════════════════════════════════════════════\n * Main Function\n * ═══════════════════════════════════════════════════════════════════════════ */\n\nint main(int argc, char* argv[]) {\n    printf(\"C Code Obfuscator v%s\\n\", VERSION);\n    printf(\"═══════════════════════════════════════\\n\");\n    \n    // Parse command line arguments\n    AppConfig* config = parse_command_line(argc, argv);\n    if (!config) {\n        return 1;\n    }\n    \n    // Show help if requested\n    if (config->show_help) {\n        print_help();\n        app_config_destroy(config);\n        return 0;\n    }\n    \n    // Validate input file\n    if (!config->input_file) {\n        fprintf(stderr, \"Error: No input file specified\\n\");\n        app_config_destroy(config);\n        return 1;\n    }\n    \n    if (!file_exists(config->input_file)) {\n        fprintf(stderr, \"Error: Input file '%s' does not exist\\n\", config->input_file);\n        app_config_destroy(config);\n        return 1;\n    }\n    \n    // Perform obfuscation\n    int result = obfuscate_file(config->input_file, config->output_file, config->config);\n    \n    app_config_destroy(config);\n    return result;\n}"