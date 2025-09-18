#ifndef OBFUSCATOR_CODEGEN_H
#define OBFUSCATOR_CODEGEN_H

#include "../common/types.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Code Generation Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Code Generator State */
typedef struct {
    CodeGenConfig* config;
    char* output_buffer;
    size_t buffer_size;
    size_t buffer_pos;
    int indent_level;
    Error* errors;
} CodeGenState;

/* Function Prototypes */

/* Main Code Generation Interface */
CodeGenState* codegen_create(CodeGenConfig* config);
void codegen_destroy(CodeGenState* gen);

char* generate_code(CodeGenState* gen, ASTNode* ast);
bool codegen_has_errors(const CodeGenState* gen);
Error* codegen_get_errors(const CodeGenState* gen);

/* AST Traversal and Generation */
void generate_program(CodeGenState* gen, ASTNode* node);
void generate_function(CodeGenState* gen, ASTNode* node);
void generate_variable(CodeGenState* gen, ASTNode* node);
void generate_statement(CodeGenState* gen, ASTNode* node);
void generate_expression(CodeGenState* gen, ASTNode* node);
void generate_block(CodeGenState* gen, ASTNode* node);

/* Output Buffer Management */
void codegen_write(CodeGenState* gen, const char* str);
void codegen_write_char(CodeGenState* gen, char c);
void codegen_write_line(CodeGenState* gen, const char* str);
void codegen_newline(CodeGenState* gen);
void codegen_indent(CodeGenState* gen);

/* Aesthetic Formatting */
void generate_ascii_art_header(CodeGenState* gen, const char* title);
void generate_aesthetic_comment(CodeGenState* gen, const char* text);
void apply_creative_formatting(CodeGenState* gen, ASTNode* node);
void add_visual_separators(CodeGenState* gen);

/* Debug Information */
void generate_debug_mapping(CodeGenState* gen, const char* original, const char* obfuscated);
void preserve_line_numbers(CodeGenState* gen, SourceLocation location);

/* Configuration Management */
CodeGenConfig* codegen_config_create_default(void);
void codegen_config_destroy(CodeGenConfig* config);
void codegen_config_set_style(CodeGenConfig* config, AestheticStyle style);

#endif /* OBFUSCATOR_CODEGEN_H */