#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ═══════════════════════════════════════════════════════════════════════════
 * Symbol Table Implementation (Basic version for expression parsing)
 * ═══════════════════════════════════════════════════════════════════════════ */

SymbolTable* symbol_table_create(void) {
    SymbolTable* table = malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    table->global_scope = scope_create(NULL);
    table->current_scope = table->global_scope;
    table->symbol_count = 0;
    
    return table;
}

void symbol_table_destroy(SymbolTable* table) {
    if (!table) return;
    
    scope_destroy(table->global_scope);
    free(table);
}

Scope* scope_create(Scope* parent) {
    Scope* scope = malloc(sizeof(Scope));
    if (!scope) return NULL;
    
    scope->symbols = NULL;
    scope->parent = parent;
    scope->children = NULL;
    scope->next_sibling = NULL;
    scope->depth = parent ? parent->depth + 1 : 0;
    
    return scope;
}

void scope_destroy(Scope* scope) {
    if (!scope) return;
    
    // Free all symbols in this scope
    Symbol* symbol = scope->symbols;
    while (symbol) {
        Symbol* next = symbol->next;
        symbol_destroy(symbol);
        symbol = next;
    }
    
    // Free child scopes
    Scope* child = scope->children;
    while (child) {
        Scope* next = child->next_sibling;
        scope_destroy(child);
        child = next;
    }
    
    free(scope);
}

void scope_enter(SymbolTable* table, Scope* scope) {
    if (!table || !scope) return;
    
    scope->parent = table->current_scope;
    
    // Add to parent's children list
    if (table->current_scope) {
        scope->next_sibling = table->current_scope->children;
        table->current_scope->children = scope;
    }
    
    table->current_scope = scope;
}

void scope_exit(SymbolTable* table) {
    if (!table || !table->current_scope) return;
    
    table->current_scope = table->current_scope->parent;
}

Symbol* symbol_create(const char* name, SymbolType type, const char* data_type) {
    if (!name) return NULL;
    
    Symbol* symbol = malloc(sizeof(Symbol));
    if (!symbol) return NULL;
    
    symbol->original_name = strdup(name);
    symbol->obfuscated_name = NULL;
    symbol->type = type;
    symbol->data_type = data_type ? strdup(data_type) : NULL;
    symbol->scope = NULL;
    symbol->is_global = false;
    symbol->is_obfuscated = false;
    symbol->next = NULL;
    
    return symbol;
}

void symbol_destroy(Symbol* symbol) {
    if (!symbol) return;
    
    free(symbol->original_name);
    free(symbol->obfuscated_name);
    free(symbol->data_type);
    free(symbol);
}

bool symbol_table_add(SymbolTable* table, Symbol* symbol) {
    if (!table || !symbol || !table->current_scope) return false;
    
    // Check for duplicate in current scope
    Symbol* existing = symbol_table_lookup_current_scope(table, symbol->original_name);
    if (existing) return false;
    
    // Add to current scope
    symbol->scope = table->current_scope;
    symbol->is_global = (table->current_scope == table->global_scope);
    symbol->next = table->current_scope->symbols;
    table->current_scope->symbols = symbol;
    table->symbol_count++;
    
    return true;
}

Symbol* symbol_table_lookup(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    Scope* scope = table->current_scope;
    while (scope) {
        Symbol* symbol = scope->symbols;
        while (symbol) {
            if (strcmp(symbol->original_name, name) == 0) {
                return symbol;
            }
            symbol = symbol->next;
        }
        scope = scope->parent;
    }
    
    return NULL;
}

Symbol* symbol_table_lookup_current_scope(SymbolTable* table, const char* name) {
    if (!table || !name || !table->current_scope) return NULL;
    
    Symbol* symbol = table->current_scope->symbols;
    while (symbol) {
        if (strcmp(symbol->original_name, name) == 0) {
            return symbol;
        }
        symbol = symbol->next;
    }
    
    return NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Name Generation (Basic implementation)
 * ═══════════════════════════════════════════════════════════════════════════ */

char* generate_obfuscated_name(const NameGenerator* gen, SymbolType type) {
    // TODO: Implement full name generation
    static int counter = 0;
    char* name = malloc(32);
    if (!name) return NULL;
    
    snprintf(name, 32, "_obf_%d", counter++);
    return name;
}

char* generate_aesthetic_name(AestheticStyle style, int counter) {
    // TODO: Implement aesthetic name generation
    char* name = malloc(32);
    if (!name) return NULL;
    
    switch (style) {
        case AESTHETIC_MINIMAL:
            snprintf(name, 32, "_v%d", counter);
            break;
        case AESTHETIC_HEXADECIMAL:
            snprintf(name, 32, "_0x%08X", counter);
            break;
        default:
            snprintf(name, 32, "_var_%03d", counter);
            break;
    }
    
    return name;
}

char* generate_unicode_name(int counter) {
    // TODO: Implement Unicode name generation
    char* name = malloc(32);
    if (!name) return NULL;
    
    snprintf(name, 32, "_u%d", counter);
    return name;
}

char* generate_hex_name(int counter) {
    char* name = malloc(32);
    if (!name) return NULL;
    
    snprintf(name, 32, "_0x%08X", counter);
    return name;
}

NameGenerator* name_generator_create(AestheticStyle style) {
    NameGenerator* gen = malloc(sizeof(NameGenerator));
    if (!gen) return NULL;
    
    gen->pattern = NULL;
    gen->counter = 0;
    gen->use_unicode = (style == AESTHETIC_UNICODE);
    gen->use_numbers = true;
    gen->use_underscores = true;
    
    return gen;
}

void name_generator_destroy(NameGenerator* gen) {
    if (!gen) return;
    
    free(gen->pattern);
    free(gen);
}

void name_generator_reset(NameGenerator* gen) {
    if (!gen) return;
    
    gen->counter = 0;
}

bool is_valid_identifier(const char* name) {
    if (!name || !*name) return false;
    
    // Must start with letter or underscore
    if (!isalpha(*name) && *name != '_') return false;
    
    // Rest must be alphanumeric or underscore
    for (const char* p = name + 1; *p; p++) {
        if (!isalnum(*p) && *p != '_') return false;
    }
    
    return true;
}

bool is_reserved_keyword(const char* name) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "inline", "int", "long", "register", "restrict", "return", "short",
        "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary",
        NULL
    };
    
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(name, keywords[i]) == 0) {
            return true;
        }
    }
    
    return false;
}

char* make_unique_name(SymbolTable* table, const char* base_name) {
    if (!table || !base_name) return NULL;
    
    // If base name is unique, use it
    if (!symbol_table_lookup(table, base_name)) {
        return strdup(base_name);
    }
    
    // Otherwise, append numbers until unique
    for (int i = 1; i < 10000; i++) {
        char* candidate = malloc(strlen(base_name) + 16);
        if (!candidate) return NULL;
        
        snprintf(candidate, strlen(base_name) + 16, "%s_%d", base_name, i);
        
        if (!symbol_table_lookup(table, candidate)) {
            return candidate;
        }
        
        free(candidate);
    }
    
    return NULL;
}