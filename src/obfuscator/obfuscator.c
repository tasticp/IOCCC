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
    
    // TODO: Add more obfuscation passes for higher levels
    if (ctx->config->level >= OBF_INTERMEDIATE) {
        // obfuscate_expressions(ctx, ast);
        // obfuscate_strings(ctx, ast);
    }
    
    if (ctx->config->level >= OBF_EXTREME) {
        // obfuscate_control_flow(ctx, ast);
        // insert_dead_code(ctx, ast);
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
 * Placeholder Functions (to be implemented in later tasks)
 * ═══════════════════════════════════════════════════════════════════════════ */

bool obfuscate_expressions(ObfuscationContext* ctx, ASTNode* ast) {
    // TODO: Implement expression obfuscation
    (void)ctx; (void)ast;
    return true;
}

bool obfuscate_strings(ObfuscationContext* ctx, ASTNode* ast) {
    // TODO: Implement string obfuscation
    (void)ctx; (void)ast;
    return true;
}

bool obfuscate_control_flow(ObfuscationContext* ctx, ASTNode* ast) {
    // TODO: Implement control flow obfuscation
    (void)ctx; (void)ast;
    return true;
}

bool insert_dead_code(ObfuscationContext* ctx, ASTNode* ast) {
    // TODO: Implement dead code insertion
    (void)ctx; (void)ast;
    return true;
}

bool apply_macro_obfuscation(ObfuscationContext* ctx, ASTNode* ast) {
    // TODO: Implement macro obfuscation
    (void)ctx; (void)ast;
    return true;
}