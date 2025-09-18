#ifndef OBFUSCATOR_OBFUSCATOR_H
#define OBFUSCATOR_OBFUSCATOR_H

#include "../common/types.h"
#include "../symbols/symbols.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Obfuscation Engine Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Obfuscation Context */
typedef struct {
    ObfuscationConfig* config;
    SymbolTable* symbol_table;
    NameGenerator* name_gen;
    Error* errors;
    int pass_count;
} ObfuscationContext;

/* Function Prototypes */

/* Main Obfuscation Interface */
ObfuscationContext* obfuscator_create(ObfuscationConfig* config);
void obfuscator_destroy(ObfuscationContext* ctx);

ASTNode* obfuscate_ast(ObfuscationContext* ctx, ASTNode* ast);
bool obfuscator_has_errors(const ObfuscationContext* ctx);
Error* obfuscator_get_errors(const ObfuscationContext* ctx);

/* Obfuscation Passes */
bool obfuscate_identifiers(ObfuscationContext* ctx, ASTNode* ast);
bool obfuscate_expressions(ObfuscationContext* ctx, ASTNode* ast);
bool obfuscate_strings(ObfuscationContext* ctx, ASTNode* ast);
bool obfuscate_control_flow(ObfuscationContext* ctx, ASTNode* ast);
bool insert_dead_code(ObfuscationContext* ctx, ASTNode* ast);
bool apply_macro_obfuscation(ObfuscationContext* ctx, ASTNode* ast);

/* Expression Transformation */
ASTNode* transform_arithmetic(ASTNode* expr);
ASTNode* insert_opaque_predicates(ASTNode* stmt);
ASTNode* flatten_control_flow(ASTNode* stmt);

/* String Obfuscation */
ASTNode* split_string_literal(const char* str);
ASTNode* encode_string_xor(const char* str, unsigned char key);
ASTNode* encode_string_rot13(const char* str);
ASTNode* create_char_array(const char* str);

/* Dead Code Generation */
ASTNode* generate_dummy_loop(void);
ASTNode* generate_unreachable_code(void);
ASTNode* generate_meaningless_computation(void);

/* Macro Obfuscation */
char* create_token_paste_macro(const char* identifier);
char* create_stringize_macro(const char* value);

/* Configuration Management */
ObfuscationConfig* config_create_default(void);
void config_destroy(ObfuscationConfig* config);
void config_set_level(ObfuscationConfig* config, ObfuscationLevel level);
void config_set_aesthetic(ObfuscationConfig* config, AestheticStyle style);

#endif /* OBFUSCATOR_OBFUSCATOR_H */