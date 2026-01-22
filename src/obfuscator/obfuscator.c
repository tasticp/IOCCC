#include "obfuscator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * Aesthetic Name Generation Patterns
 * ═══════════════════════════════════════════════════════════════════════════ */

static const char* unicode_chars[] = {
    "ℓ", "𝓁", "𝓵", "𝓁", "𝒶", "𝒷", "𝒸", "𝒹", "𝑒", "𝒻", "𝑔", "𝒽", "𝒾", "𝒿",
    "𝓀", "𝓁", "𝓂", "𝓃", "𝑜", "𝓅", "𝓆", "𝓇", "𝓈", "𝓉", "𝓊", "𝓋", "𝓌", "𝓍", "𝓎", "𝓏",
    NULL
};

static const char* matrix_chars[] = {
    "𝚊", "𝚋", "𝚌", "𝚍", "𝚎", "𝚏", "𝚐", "𝚑", "𝚒", "𝚓", "𝚔", "𝚕", "𝚖", "𝚗", "𝚘", "𝚙",
    "𝚚", "𝚛", "𝚜", "𝚝", "𝚞", "𝚟", "𝚠", "𝚡", "𝚢", "𝚣", "𝟶", "𝟷", "𝟸", "𝟹", "𝟺", "𝟻",
    "𝟼", "𝟽", "𝟾", "𝟿", NULL
};

static const char* mystical_chars[] = {
    "⚡", "✦", "✧", "☆", "★", "♦", "♠", "♣", "♥", "☽", "☾", "✵", "✶", "✷", "✸", "✹",
    "◇", "◆", "○", "●", "□", "■", "△", "▽", "◁", "▷", "◐", "◑", "◒", "◓", NULL
};

static const char* ascii_patterns[] = {
    "@@@@", "####", "$$$$", "%%%%", "&&&&", "****", "++++", "====",
    "-><-", ">-<", "<->", "<|>", "|-|", "-|-", "/\\/", "\\/\\", NULL
};

static const char* runic_chars[] = {
    "ᚠ", "ᚢ", "ᚦ", "ᚨ", "ᚱ", "ᚲ", "ᚷ", "ᚹ", "ᚺ", "ᚾ", "ᛁ", "ᛃ", "ᛇ", "ᛈ", "ᛉ", "ᛊ",
    "ᛏ", "ᛒ", "ᛖ", "ᛗ", "ᛚ", "ᛜ", "ᛟ", "ᛞ", "ᚪ", "ᚫ", "ᚣ", "ᛡ", "ᛠ", NULL
};

static const char* aesthetic_prefixes[] = {
    "__aesthetic_", "__obf_", "__hidden_", "__secret_", "__encoded_",
    "_0x", "__var_", "__func_", "__data_", "__code_", NULL
};

static const char* chaotic_patterns[] = {
    "l1l1l1", "O0O0O0", "Il1Il1", "oO0oO0", "___", "____", "_____",
    "aaaa", "bbbb", "cccc", "xxxx", "yyyy", "zzzz", NULL
};

/* ═══════════════════════════════════════════════════════════════════════════
 * Obfuscation Context Management
 * ═══════════════════════════════════════════════════════════════════════════ */

ObfuscationContext* obfuscator_create(ObfuscationConfig* config) {
    if (!config) return NULL;
    
    ObfuscationContext* ctx = malloc(sizeof(ObfuscationContext));
    if (!ctx) return NULL;
    
    ctx->config = config;
    ctx->symbol_table = symbol_table_create();
    ctx->name_gen = name_generator_create(config->aesthetic);
    ctx->errors = NULL;
    ctx->pass_count = 0;
    
    // Seed random number generator for obfuscation
    srand((unsigned int)time(NULL));
    
    return ctx;
}

void obfuscator_destroy(ObfuscationContext* ctx) {
    if (!ctx) return;
    
    symbol_table_destroy(ctx->symbol_table);
    name_generator_destroy(ctx->name_gen);
    // TODO: Free error list
    free(ctx);
}

bool obfuscator_has_errors(const ObfuscationContext* ctx) {
    return ctx && ctx->errors != NULL;
}

Error* obfuscator_get_errors(const ObfuscationContext* ctx) {
    return ctx ? ctx->errors : NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Advanced Name Generation
 * ═══════════════════════════════════════════════════════════════════════════ */

static char* generate_minimal_name(int counter) {
    char* name = malloc(16);
    if (!name) return NULL;
    
    if (counter < 26) {
        snprintf(name, 16, "_%c", 'a' + counter);
    } else {
        snprintf(name, 16, "_%c%d", 'a' + (counter % 26), counter / 26);
    }
    
    return name;
}

static char* generate_unicode_aesthetic_name(int counter) {
    char* name = malloc(64);
    if (!name) return NULL;
    
    int unicode_count = 0;
    while (unicode_chars[unicode_count]) unicode_count++;
    
    if (counter < unicode_count) {
        snprintf(name, 64, "_%s%s%s", 
                unicode_chars[counter % unicode_count],
                unicode_chars[(counter + 1) % unicode_count],
                unicode_chars[(counter + 2) % unicode_count]);
    } else {
        snprintf(name, 64, "_𝓋𝒶𝓇_%d", counter);
    }
    
    return name;
}

static char* generate_hexadecimal_name(int counter) {
    char* name = malloc(32);
    if (!name) return NULL;
    
    // Create visually appealing hex patterns
    unsigned int patterns[] = {
        0x41414141, 0x42424242, 0xDEADBEEF, 0xCAFEBABE, 0xFEEDFACE,
        0xBADDCAFE, 0xDEADC0DE, 0xBEEFFACE, 0xC0FFEE00, 0xFACEB00C
    };
    
    int pattern_count = sizeof(patterns) / sizeof(patterns[0]);
    unsigned int base_pattern = patterns[counter % pattern_count];
    unsigned int final_pattern = base_pattern + (counter / pattern_count);
    
    snprintf(name, 32, "_0x%08X", final_pattern);
    return name;
}

static char* generate_artistic_name(int counter) {
    char* name = malloc(64);
    if (!name) return NULL;
    
    int prefix_count = 0;
    while (aesthetic_prefixes[prefix_count]) prefix_count++;
    
    const char* prefix = aesthetic_prefixes[counter % prefix_count];
    snprintf(name, 64, "%s%03d", prefix, counter);
    
    return name;
}

static char* generate_chaotic_name(int counter) {
    char* name = malloc(64);
    if (!name) return NULL;
    
    int pattern_count = 0;
    while (chaotic_patterns[pattern_count]) pattern_count++;
    
    const char* pattern = chaotic_patterns[counter % pattern_count];
    
    // Add random numbers and characters for maximum chaos
    int random_suffix = rand() % 1000;
    char random_char = 'A' + (rand() % 26);
    
    snprintf(name, 64, "_%s%c%d_%c", pattern, random_char, random_suffix, 
             'a' + (counter % 26));
    
    return name;
}

static char* generate_matrix_name(int counter) {
    char* name = malloc(128);
    if (!name) return NULL;
    
    int matrix_count = 0;
    while (matrix_chars[matrix_count]) matrix_count++;
    
    // Create Matrix-style names like 𝚊_𝟷𝟹𝟹𝟽_𝚣
    snprintf(name, 128, "%s_%04d_%s", 
            matrix_chars[counter % matrix_count],
            1337 + counter,
            matrix_chars[(counter + 7) % matrix_count]);
    
    return name;
}

static char* generate_mystical_name(int counter) {
    char* name = malloc(128);
    if (!name) return NULL;
    
    int mystical_count = 0;
    while (mystical_chars[mystical_count]) mystical_count++;
    
    // Create mystical names like ⚡_✦_star_001
    snprintf(name, 128, "%s_%s_magic_%03d", 
            mystical_chars[counter % mystical_count],
            mystical_chars[(counter + 3) % mystical_count],
            counter);
    
    return name;
}

static char* generate_ascii_art_name(int counter) {
    char* name = malloc(128);
    if (!name) return NULL;
    
    int pattern_count = 0;
    while (ascii_patterns[pattern_count]) pattern_count++;
    
    const char* pattern = ascii_patterns[counter % pattern_count];
    
    // Create ASCII art names like @@@@func_001
    snprintf(name, 128, "%sfunc_%03d", pattern, counter);
    
    return name;
}

static char* generate_runic_name(int counter) {
    char* name = malloc(128);
    if (!name) return NULL;
    
    int runic_count = 0;
    while (runic_chars[runic_count]) runic_count++;
    
    // Create runic names like ᚠᚢᚦ_secret_001
    snprintf(name, 128, "%s%s%s_rune_%03d", 
            runic_chars[counter % runic_count],
            runic_chars[(counter + 1) % runic_count],
            runic_chars[(counter + 2) % runic_count],
            counter);
    
    return name;
}

char* generate_aesthetic_name_advanced(AestheticStyle style, int counter) {
    switch (style) {
        case AESTHETIC_MINIMAL:
            return generate_minimal_name(counter);
            
        case AESTHETIC_UNICODE:
            return generate_unicode_aesthetic_name(counter);
            
        case AESTHETIC_HEXADECIMAL:
            return generate_hexadecimal_name(counter);
            
        case AESTHETIC_ARTISTIC:
            return generate_artistic_name(counter);
            
        case AESTHETIC_CHAOTIC:
            return generate_chaotic_name(counter);
            
        case AESTHETIC_MATRIX:
            return generate_matrix_name(counter);
            
        case AESTHETIC_MYSTICAL:
            return generate_mystical_name(counter);
            
        case AESTHETIC_ASCII_ART:
            return generate_ascii_art_name(counter);
            
        case AESTHETIC_RUNIC:
            return generate_runic_name(counter);
            
        default:
            return generate_artistic_name(counter);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * AST Traversal for Identifier Collection
 * ═══════════════════════════════════════════════════════════════════════════ */

static void collect_identifiers_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_IDENTIFIER: {
            const char* name = node->data.identifier.name;
            
            // Skip if already in symbol table
            if (symbol_table_lookup(ctx->symbol_table, name)) {
                break;
            }
            
            // Create symbol and add to table
            Symbol* symbol = symbol_create(name, SYMBOL_VARIABLE, "unknown");
            if (symbol) {
                symbol_table_add(ctx->symbol_table, symbol);
            }
            break;
        }
        
        case NODE_FUNCTION: {
            const char* name = node->data.function.name;
            if (name && !symbol_table_lookup(ctx->symbol_table, name)) {
                Symbol* symbol = symbol_create(name, SYMBOL_FUNCTION, 
                                             node->data.function.return_type);
                if (symbol) {
                    symbol_table_add(ctx->symbol_table, symbol);
                }
            }
            
            // Recursively process parameters and body
            collect_identifiers_recursive(ctx, node->data.function.parameters);
            collect_identifiers_recursive(ctx, node->data.function.body);
            break;
        }
        
        case NODE_VARIABLE: {
            const char* name = node->data.variable.name;
            if (name && !symbol_table_lookup(ctx->symbol_table, name)) {
                Symbol* symbol = symbol_create(name, SYMBOL_VARIABLE, 
                                             node->data.variable.type);
                if (symbol) {
                    symbol_table_add(ctx->symbol_table, symbol);
                }
            }
            
            collect_identifiers_recursive(ctx, node->data.variable.initializer);
            break;
        }
        
        case NODE_CALL:
            collect_identifiers_recursive(ctx, node->data.call.function);
            collect_identifiers_recursive(ctx, node->data.call.arguments);
            break;
            
        case NODE_BINARY_OP:
            collect_identifiers_recursive(ctx, node->data.binary.left);
            collect_identifiers_recursive(ctx, node->data.binary.right);
            break;
            
        case NODE_UNARY_OP:
            collect_identifiers_recursive(ctx, node->data.unary.operand);
            break;
            
        case NODE_IF:
            collect_identifiers_recursive(ctx, node->data.if_stmt.condition);
            collect_identifiers_recursive(ctx, node->data.if_stmt.then_stmt);
            collect_identifiers_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            collect_identifiers_recursive(ctx, node->data.while_stmt.condition);
            collect_identifiers_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            collect_identifiers_recursive(ctx, node->data.for_stmt.init);
            collect_identifiers_recursive(ctx, node->data.for_stmt.condition);
            collect_identifiers_recursive(ctx, node->data.for_stmt.update);
            collect_identifiers_recursive(ctx, node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            collect_identifiers_recursive(ctx, node->data.block.statements);
            break;
            
        case NODE_STRUCT:
            collect_identifiers_recursive(ctx, node->data.struct_def.members);
            break;
            
        default:
            break;
    }
    
    // Process next node in list
    collect_identifiers_recursive(ctx, node->next);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Identifier Obfuscation Pass
 * ═══════════════════════════════════════════════════════════════════════════ */

static void generate_obfuscated_names(ObfuscationContext* ctx) {
    if (!ctx || !ctx->symbol_table) return;
    
    int counter = 0;
    
    // Traverse all scopes and generate obfuscated names
    Scope* scope = ctx->symbol_table->global_scope;
    while (scope) {
        Symbol* symbol = scope->symbols;
        while (symbol) {
            if (!symbol->is_obfuscated && !is_reserved_keyword(symbol->original_name)) {
                // Generate aesthetic obfuscated name
                symbol->obfuscated_name = generate_aesthetic_name_advanced(
                    ctx->config->aesthetic, counter++);
                symbol->is_obfuscated = true;
                
                // Ensure uniqueness
                while (symbol_table_lookup(ctx->symbol_table, symbol->obfuscated_name)) {
                    free(symbol->obfuscated_name);
                    symbol->obfuscated_name = generate_aesthetic_name_advanced(
                        ctx->config->aesthetic, counter++);
                }
            }
            symbol = symbol->next;
        }
        
        // TODO: Traverse child scopes
        scope = NULL; // For now, only process global scope
    }
}

static void apply_identifier_obfuscation_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_IDENTIFIER: {
            Symbol* symbol = symbol_table_lookup(ctx->symbol_table, 
                                                node->data.identifier.name);
            if (symbol && symbol->obfuscated_name) {
                free(node->data.identifier.name);
                node->data.identifier.name = strdup(symbol->obfuscated_name);
            }
            break;
        }
        
        case NODE_FUNCTION: {
            Symbol* symbol = symbol_table_lookup(ctx->symbol_table, 
                                                node->data.function.name);
            if (symbol && symbol->obfuscated_name) {
                free(node->data.function.name);
                node->data.function.name = strdup(symbol->obfuscated_name);
            }
            
            apply_identifier_obfuscation_recursive(ctx, node->data.function.parameters);
            apply_identifier_obfuscation_recursive(ctx, node->data.function.body);
            break;
        }
        
        case NODE_VARIABLE: {
            Symbol* symbol = symbol_table_lookup(ctx->symbol_table, 
                                                node->data.variable.name);
            if (symbol && symbol->obfuscated_name) {
                free(node->data.variable.name);
                node->data.variable.name = strdup(symbol->obfuscated_name);
            }
            
            apply_identifier_obfuscation_recursive(ctx, node->data.variable.initializer);
            break;
        }
        
        case NODE_CALL:
            apply_identifier_obfuscation_recursive(ctx, node->data.call.function);
            apply_identifier_obfuscation_recursive(ctx, node->data.call.arguments);
            break;
            
        case NODE_BINARY_OP:
            apply_identifier_obfuscation_recursive(ctx, node->data.binary.left);
            apply_identifier_obfuscation_recursive(ctx, node->data.binary.right);
            break;
            
        case NODE_UNARY_OP:
            apply_identifier_obfuscation_recursive(ctx, node->data.unary.operand);
            break;
            
        case NODE_IF:
            apply_identifier_obfuscation_recursive(ctx, node->data.if_stmt.condition);
            apply_identifier_obfuscation_recursive(ctx, node->data.if_stmt.then_stmt);
            apply_identifier_obfuscation_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            apply_identifier_obfuscation_recursive(ctx, node->data.while_stmt.condition);
            apply_identifier_obfuscation_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            apply_identifier_obfuscation_recursive(ctx, node->data.for_stmt.init);
            apply_identifier_obfuscation_recursive(ctx, node->data.for_stmt.condition);
            apply_identifier_obfuscation_recursive(ctx, node->data.for_stmt.update);
            apply_identifier_obfuscation_recursive(ctx, node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            apply_identifier_obfuscation_recursive(ctx, node->data.block.statements);
            break;
            
        case NODE_STRUCT:
            apply_identifier_obfuscation_recursive(ctx, node->data.struct_def.members);
            break;
            
        default:
            break;
    }
    
    // Process next node in list
    apply_identifier_obfuscation_recursive(ctx, node->next);
}

bool obfuscate_identifiers(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Step 1: Collect all identifiers in the AST
    collect_identifiers_recursive(ctx, ast);
    
    // Step 2: Generate obfuscated names for all symbols
    generate_obfuscated_names(ctx);
    
    // Step 3: Apply obfuscation to the AST
    apply_identifier_obfuscation_recursive(ctx, ast);
    
    ctx->pass_count++;
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Main Obfuscation Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

ASTNode* obfuscate_ast(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return NULL;
    
    // Apply obfuscation passes based on configuration level
    if (ctx->config->level >= OBF_BASIC) {
        if (!obfuscate_identifiers(ctx, ast)) {
            return NULL;
        }
    }
    
    // Add more obfuscation passes for higher levels
    if (ctx->config->level >= OBF_INTERMEDIATE) {
        if (!obfuscate_expressions(ctx, ast)) {
            return NULL;
        }
        if (!obfuscate_strings(ctx, ast)) {
            return NULL;
        }
    }
    
    if (ctx->config->level >= OBF_EXTREME) {
        if (!obfuscate_control_flow(ctx, ast)) {
            return NULL;
        }
        if (!insert_dead_code(ctx, ast)) {
            return NULL;
        }
        if (!apply_macro_obfuscation(ctx, ast)) {
            return NULL;
        }
    }
    
    return ast;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Configuration Management
 * ═══════════════════════════════════════════════════════════════════════════ */

ObfuscationConfig* config_create_default(void) {
    ObfuscationConfig* config = malloc(sizeof(ObfuscationConfig));
    if (!config) return NULL;
    
    config->level = OBF_INTERMEDIATE;
    config->aesthetic = AESTHETIC_ARTISTIC;
    config->preserve_debug_info = false;
    config->obfuscate_strings = true;
    config->obfuscate_control_flow = true;
    config->insert_dead_code = false;
    config->use_macros = true;
    config->output_file = NULL;
    
    // Initialize name generator
    config->name_gen.pattern = NULL;
    config->name_gen.counter = 0;
    config->name_gen.use_unicode = false;
    config->name_gen.use_numbers = true;
    config->name_gen.use_underscores = true;
    
    return config;
}

void config_destroy(ObfuscationConfig* config) {
    if (!config) return;
    
    free(config->output_file);
    free(config->name_gen.pattern);
    free(config);
}

void config_set_level(ObfuscationConfig* config, ObfuscationLevel level) {
    if (!config) return;
    config->level = level;
}

void config_set_aesthetic(ObfuscationConfig* config, AestheticStyle style) {
    if (!config) return;
    config->aesthetic = style;
    config->name_gen.use_unicode = (style == AESTHETIC_UNICODE);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Forward Declarations
 * ═══════════════════════════════════════════════════════════════════════════ */

static void obfuscate_expressions_recursive(ObfuscationContext* ctx, ASTNode* node);
static void obfuscate_strings_recursive(ObfuscationContext* ctx, ASTNode* node);
static void obfuscate_control_flow_recursive(ObfuscationContext* ctx, ASTNode* node);
static void insert_dead_code_recursive(ObfuscationContext* ctx, ASTNode* node);
static void insert_anti_debug_code_recursive(ObfuscationContext* ctx, ASTNode* node);

/* ═══════════════════════════════════════════════════════════════════════════
 * AST Helper Functions
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode* ast_create_literal(const char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_LITERAL;
    node->data.literal.value = strdup(value);
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_literal_number(int value) {
    char buffer[32];
    sprintf(buffer, "%d", value);
    return ast_create_literal(buffer);
}

static ASTNode* ast_create_identifier(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_IDENTIFIER;
    node->data.identifier.name = strdup(name);
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_binary_op(const char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_BINARY_OP;
    node->data.binary.operator = strdup(op);
    node->data.binary.left = left;
    node->data.binary.right = right;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_unary_op(const char* op, ASTNode* operand, bool is_prefix) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_UNARY_OP;
    node->data.unary.operator = strdup(op);
    node->data.unary.operand = operand;
    node->data.unary.is_prefix = is_prefix;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_variable(const char* name, const char* type, ASTNode* initializer) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_VARIABLE;
    node->data.variable.name = strdup(name);
    node->data.variable.type = strdup(type);
    node->data.variable.initializer = initializer;
    node->data.variable.is_static = false;
    node->data.variable.is_const = false;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_assignment(ASTNode* target, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_ASSIGNMENT;
    // Simplified - would need proper assignment structure in ASTNode
    node->data.binary.left = target;
    node->data.binary.right = value;
    node->data.binary.operator = strdup("=");
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_if(ASTNode* condition, ASTNode* then_stmt, ASTNode* else_stmt) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_stmt = then_stmt;
    node->data.if_stmt.else_stmt = else_stmt;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_while(ASTNode* condition, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_WHILE;
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_for(ASTNode* init, ASTNode* condition, ASTNode* update, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_FOR;
    node->data.for_stmt.init = init;
    node->data.for_stmt.condition = condition;
    node->data.for_stmt.update = update;
    node->data.for_stmt.body = body;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_block(ASTNode* statements) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_BLOCK;
    node->data.block.statements = statements;
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_switch(ASTNode* expression, ASTNode* cases) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_BINARY_OP; // Reuse for switch - simplified
    node->data.binary.left = expression;
    node->data.binary.right = cases;
    node->data.binary.operator = strdup("switch");
    node->next = NULL;
    return node;
}

static ASTNode* ast_create_case(ASTNode* value, ASTNode* statement) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) return NULL;
    
    node->type = NODE_BINARY_OP; // Reuse for case - simplified
    node->data.binary.left = value;
    node->data.binary.right = statement;
    node->data.binary.operator = strdup("case");
    node->next = NULL;
    return node;
}

static ASTNode* ast_copy(ASTNode* original) {
    if (!original) return NULL;
    
    ASTNode* copy = malloc(sizeof(ASTNode));
    if (!copy) return NULL;
    
    *copy = *original; // Shallow copy
    copy->next = NULL;
    
    // Deep copy specific data based on node type
    switch (original->type) {
        case NODE_LITERAL:
            copy->data.literal.value = strdup(original->data.literal.value);
            break;
        case NODE_IDENTIFIER:
            copy->data.identifier.name = strdup(original->data.identifier.name);
            break;
        case NODE_BINARY_OP:
            copy->data.binary.operator = strdup(original->data.binary.operator);
            copy->data.binary.left = ast_copy(original->data.binary.left);
            copy->data.binary.right = ast_copy(original->data.binary.right);
            break;
        case NODE_UNARY_OP:
            copy->data.unary.operator = strdup(original->data.unary.operator);
            copy->data.unary.operand = ast_copy(original->data.unary.operand);
            break;
        default:
            break;
    }
    
    return copy;
}

static ASTNode* ast_link(ASTNode* first, ASTNode* second) {
    if (!first) return second;
    if (!second) return first;
    
    ASTNode* current = first;
    while (current->next) {
        current = current->next;
    }
    current->next = second;
    return first;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Advanced Expression Obfuscation
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode* create_complex_expression(ASTNode* original) {
    if (!original || original->type != NODE_BINARY_OP) return original;
    
    // Transform simple operations into complex bitwise equivalents
    char* op = original->data.binary.operator;
    
    if (strcmp(op, "+") == 0) {
        // a + b -> ((a ^ b) + 2 * (a & b))
        ASTNode* xor_node = ast_create_binary_op("^", 
            ast_copy(original->data.binary.left), 
            ast_copy(original->data.binary.right));
        
        ASTNode* and_node = ast_create_binary_op("&", 
            ast_copy(original->data.binary.left), 
            ast_copy(original->data.binary.right));
        
        ASTNode* two_node = ast_create_literal("2");
        ASTNode* mul_node = ast_create_binary_op("*", two_node, and_node);
        
        return ast_create_binary_op("+", xor_node, mul_node);
    }
    
    if (strcmp(op, "*") == 0) {
        // a * b -> ((a & b) + ((a ^ b) >> 1)) << 1 + (a & b & 1)
        ASTNode* and_node = ast_create_binary_op("&", 
            ast_copy(original->data.binary.left), 
            ast_copy(original->data.binary.right));
        
        ASTNode* xor_node = ast_create_binary_op("^", 
            ast_copy(original->data.binary.left), 
            ast_copy(original->data.binary.right));
        
        ASTNode* shift_node = ast_create_binary_op(">>", xor_node, ast_create_literal("1"));
        ASTNode* add_node = ast_create_binary_op("+", and_node, shift_node);
        ASTNode* left_shift = ast_create_binary_op("<<", add_node, ast_create_literal("1"));
        
        ASTNode* final_and = ast_create_binary_op("&", 
            ast_copy(original->data.binary.left), 
            ast_copy(original->data.binary.right));
        ASTNode* one_node = ast_create_literal("1");
        ASTNode* final_and2 = ast_create_binary_op("&", final_and, one_node);
        
        return ast_create_binary_op("+", left_shift, final_and2);
    }
    
    return original;
}

bool obfuscate_expressions(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Recursively obfuscate all binary expressions
    obfuscate_expressions_recursive(ctx, ast);
    
    ctx->pass_count++;
    return true;
}

static void obfuscate_expressions_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_BINARY_OP: {
            // Obfuscate children first
            obfuscate_expressions_recursive(ctx, node->data.binary.left);
            obfuscate_expressions_recursive(ctx, node->data.binary.right);
            
            // Apply complex expression transformation with some probability
            if (rand() % 100 < 70) { // 70% chance to obfuscate
                ASTNode* complex = create_complex_expression(node);
                if (complex != node) {
                    // Replace the node (simplified - in real implementation would need proper AST node replacement)
                    *node = *complex;
                    free(complex);
                }
            }
            break;
        }
        
        case NODE_UNARY_OP:
            obfuscate_expressions_recursive(ctx, node->data.unary.operand);
            break;
            
        case NODE_CALL:
            obfuscate_expressions_recursive(ctx, node->data.call.function);
            obfuscate_expressions_recursive(ctx, node->data.call.arguments);
            break;
            
        case NODE_IF:
            obfuscate_expressions_recursive(ctx, node->data.if_stmt.condition);
            obfuscate_expressions_recursive(ctx, node->data.if_stmt.then_stmt);
            obfuscate_expressions_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            obfuscate_expressions_recursive(ctx, node->data.while_stmt.condition);
            obfuscate_expressions_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            obfuscate_expressions_recursive(ctx, node->data.for_stmt.init);
            obfuscate_expressions_recursive(ctx, node->data.for_stmt.condition);
            obfuscate_expressions_recursive(ctx, node->data.for_stmt.update);
            obfuscate_expressions_recursive(ctx, node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            obfuscate_expressions_recursive(ctx, node->data.block.statements);
            break;
            
        default:
            break;
    }
    
    obfuscate_expressions_recursive(ctx, node->next);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * String Encryption Obfuscation
 * ═══════════════════════════════════════════════════════════════════════════ */

static char* encrypt_string(const char* original) {
    if (!original) return NULL;
    
    size_t len = strlen(original);
    char* encrypted = malloc(len * 8 + 256); // Extra space for decryption code
    if (!encrypted) return NULL;
    
    // Create XOR encryption with random key
    unsigned char key = 0x42 ^ (rand() % 256);
    
    // Generate decryption function code
    sprintf(encrypted, 
        "({ static const unsigned char k[%zu] = {", len);
    char* ptr = encrypted + strlen(encrypted);
    
    for (size_t i = 0; i < len; i++) {
        unsigned char encrypted_char = original[i] ^ key;
        ptr += sprintf(ptr, "%d", encrypted_char);
        if (i < len - 1) ptr += sprintf(ptr, ",");
    }
    
    ptr += sprintf(ptr, "}; "
        "char buf[%zu]; "
        "for(size_t i=0;i<%zu;i++) buf[i]=k[i]^%d; "
        "buf[%zu]='\\0'; buf; })", len, len, key, len);
    
    return encrypted;
}

bool obfuscate_strings(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Recursively find and encrypt all string literals
    obfuscate_strings_recursive(ctx, ast);
    
    ctx->pass_count++;
    return true;
}

static void obfuscate_strings_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LITERAL: {
            if (node->data.literal.value && node->data.literal.value[0] == '"') {
                // This is a string literal
                char* encrypted = encrypt_string(node->data.literal.value);
                if (encrypted) {
                    free(node->data.literal.value);
                    node->data.literal.value = encrypted;
                }
            }
            break;
        }
        
        case NODE_CALL:
            obfuscate_strings_recursive(ctx, node->data.call.function);
            obfuscate_strings_recursive(ctx, node->data.call.arguments);
            break;
            
        case NODE_BINARY_OP:
            obfuscate_strings_recursive(ctx, node->data.binary.left);
            obfuscate_strings_recursive(ctx, node->data.binary.right);
            break;
            
        case NODE_UNARY_OP:
            obfuscate_strings_recursive(ctx, node->data.unary.operand);
            break;
            
        case NODE_IF:
            obfuscate_strings_recursive(ctx, node->data.if_stmt.condition);
            obfuscate_strings_recursive(ctx, node->data.if_stmt.then_stmt);
            obfuscate_strings_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            obfuscate_strings_recursive(ctx, node->data.while_stmt.condition);
            obfuscate_strings_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            obfuscate_strings_recursive(ctx, node->data.for_stmt.init);
            obfuscate_strings_recursive(ctx, node->data.for_stmt.condition);
            obfuscate_strings_recursive(ctx, node->data.for_stmt.update);
            obfuscate_strings_recursive(ctx, node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            obfuscate_strings_recursive(ctx, node->data.block.statements);
            break;
            
        default:
            break;
    }
    
    obfuscate_strings_recursive(ctx, node->next);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Control Flow Obfuscation
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode* create_state_machine(ASTNode* statements) {
    if (!statements) return NULL;
    
    // Create state variable
    ASTNode* state_var = ast_create_variable("__state", "int", ast_create_literal("0"));
    
    // Create main state machine loop
    ASTNode* state_condition = ast_create_binary_op("!=", 
        ast_create_identifier("__state"), 
        ast_create_literal("-1"));
    
    // Create switch statement
    ASTNode* switch_stmt = ast_create_switch(ast_create_identifier("__state"), NULL);
    
    // Convert each statement to a case in the state machine
    int state_id = 0;
    ASTNode* current = statements;
    while (current) {
        // Create case statement
        ASTNode* case_label = ast_create_literal_number(state_id);
        ASTNode* case_stmt = ast_create_case(case_label, current);
        
        // Add case to switch (simplified - would need proper AST linking)
        current = current->next;
        state_id++;
    }
    
    // Create while loop containing the switch
    ASTNode* while_body = ast_create_block(switch_stmt);
    ASTNode* while_loop = ast_create_while(state_condition, while_body);
    
    // Create the complete function body
    ASTNode* body = ast_create_block(ast_link(state_var, while_loop));
    
    return body;
}

bool obfuscate_control_flow(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Find function bodies and transform them
    obfuscate_control_flow_recursive(ctx, ast);
    
    ctx->pass_count++;
    return true;
}

static void obfuscate_control_flow_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_FUNCTION: {
            // Transform function body into state machine
            if (node->data.function.body && 
                node->data.function.body->type == NODE_BLOCK) {
                
                ASTNode* statements = node->data.function.body->data.block.statements;
                if (statements && statements->next) { // Only if multiple statements
                    ASTNode* state_machine = create_state_machine(statements);
                    if (state_machine) {
                        node->data.function.body = state_machine;
                    }
                }
            }
            break;
        }
        
        case NODE_IF:
            obfuscate_control_flow_recursive(ctx, node->data.if_stmt.condition);
            obfuscate_control_flow_recursive(ctx, node->data.if_stmt.then_stmt);
            obfuscate_control_flow_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            obfuscate_control_flow_recursive(ctx, node->data.while_stmt.condition);
            obfuscate_control_flow_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            obfuscate_control_flow_recursive(ctx, node->data.for_stmt.init);
            obfuscate_control_flow_recursive(ctx, node->data.for_stmt.condition);
            obfuscate_control_flow_recursive(ctx, node->data.for_stmt.update);
            obfuscate_control_flow_recursive(ctx, node->data.for_stmt.body);
            break;
            
        case NODE_BLOCK:
            obfuscate_control_flow_recursive(ctx, node->data.block.statements);
            break;
            
        default:
            break;
    }
    
    obfuscate_control_flow_recursive(ctx, node->next);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Dead Code Insertion
 * ═══════════════════════════════════════════════════════════════════════════ */

static ASTNode* generate_dead_code(void) {
    int type = rand() % 4;
    
    switch (type) {
        case 0: {
            // Redundant calculation
            ASTNode* var = ast_create_variable("__dead_var", "int", 
                ast_create_binary_op("*", ast_create_literal("42"), ast_create_literal("0")));
            return var;
        }
        
        case 1: {
            // Unconditional false condition
            ASTNode* condition = ast_create_binary_op("==", 
                ast_create_literal("1"), ast_create_literal("0"));
            ASTNode* body = ast_create_block(ast_create_literal("0"));
            return ast_create_if(condition, body, NULL);
        }
        
        case 2: {
            // Meaningless loop
            ASTNode* condition = ast_create_literal("0");
            ASTNode* body = ast_create_block(ast_create_literal("0"));
            return ast_create_while(condition, body);
        }
        
        case 3: {
            // Dead assignment
            ASTNode* var = ast_create_identifier("__dead_counter");
            ASTNode* value = ast_create_binary_op("+", var, ast_create_literal("0"));
            return ast_create_assignment(var, value);
        }
        
        default:
            return NULL;
    }
}

bool insert_dead_code(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Insert dead code into blocks
    insert_dead_code_recursive(ctx, ast);
    
    ctx->pass_count++;
    return true;
}

static void insert_dead_code_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_BLOCK: {
            // Insert dead code with some probability
            if (rand() % 100 < 30) { // 30% chance
                ASTNode* dead = generate_dead_code();
                if (dead) {
                    // Insert dead code into the block
                    dead->next = node->data.block.statements;
                    node->data.block.statements = dead;
                }
            }
            
            insert_dead_code_recursive(ctx, node->data.block.statements);
            break;
        }
        
        case NODE_FUNCTION:
            insert_dead_code_recursive(ctx, node->data.function.body);
            break;
            
        case NODE_IF:
            insert_dead_code_recursive(ctx, node->data.if_stmt.then_stmt);
            insert_dead_code_recursive(ctx, node->data.if_stmt.else_stmt);
            break;
            
        case NODE_WHILE:
            insert_dead_code_recursive(ctx, node->data.while_stmt.body);
            break;
            
        case NODE_FOR:
            insert_dead_code_recursive(ctx, node->data.for_stmt.body);
            break;
            
        default:
            break;
    }
    
    insert_dead_code_recursive(ctx, node->next);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Macro Obfuscation
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ═══════════════════════════════════════════════════════════════════════════
 * Anti-Debugging and Anti-Reverse Engineering Techniques
 * ═══════════════════════════════════════════════════════════════════════════ */

static char* generate_anti_debug_code(void) {
    char* code = malloc(1024);
    if (!code) return NULL;
    
    // Generate anti-debugging checks
    sprintf(code,
        "// Anti-debugging protection\n"
        "if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) { exit(1); }\n"
        "if (getppid() != getppid()) { exit(1); }\n"
        "// Runtime integrity check\n"
        "volatile int __check = 0xDEADC0DE;\n"
        "if (__check != 0xDEADC0DE) { abort(); }\n");
    
    return code;
}

static char* generate_checksum_verification(void) {
    char* code = malloc(512);
    if (!code) return NULL;
    
    sprintf(code,
        "// Runtime checksum verification\n"
        "static const unsigned int __checksum = 0x%08X;\n"
        "unsigned int __calc = 0;\n"
        "for(int i=0;i<sizeof(check_data);i++) __calc += ((unsigned char*)check_data)[i];\n"
        "if(__calc != __checksum) { exit(1); }\n",
        rand());
    
    return code;
}

bool apply_macro_obfuscation(ObfuscationContext* ctx, ASTNode* ast) {
    if (!ctx || !ast) return false;
    
    // Insert anti-debugging code at the beginning of main function
    insert_anti_debug_code_recursive(ctx, ast);
    
    // Add macro definitions for obfuscation
    char* macros = malloc(2048);
    if (macros) {
        sprintf(macros,
            "// Obfuscation macros\n"
            "#define HIDE_VAR(x) (__##x##__hidden)\n"
            "#define OPAQUE_ZERO ((1^1)^1)\n"
            "#define COMPLEX_ADD(a,b) (((a)^(b))+2*((a)&(b)))\n"
            "#define DEBUG_CHECK if(ptrace(PTRACE_TRACEME,0,1,0)<0)exit(1)\n");
        
        // These would be added to the output file header
        free(macros);
    }
    
    ctx->pass_count++;
    return true;
}

static void insert_anti_debug_code_recursive(ObfuscationContext* ctx, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_FUNCTION: {
            // Check if this is main function
            if (strcmp(node->data.function.name, "main") == 0) {
                char* anti_debug = generate_anti_debug_code();
                if (anti_debug) {
                    // Insert at the beginning of main function
                    // This would be properly integrated into the AST
                    free(anti_debug);
                }
            }
            break;
        }
        
        default:
            break;
    }
    
    insert_anti_debug_code_recursive(ctx, node->next);
}