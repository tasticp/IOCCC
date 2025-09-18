#ifndef OBFUSCATOR_SYMBOLS_H
#define OBFUSCATOR_SYMBOLS_H

#include "../common/types.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Symbol Table Management Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Function Prototypes */

/* Symbol Table Management */
SymbolTable* symbol_table_create(void);
void symbol_table_destroy(SymbolTable* table);

Scope* scope_create(Scope* parent);
void scope_destroy(Scope* scope);
void scope_enter(SymbolTable* table, Scope* scope);
void scope_exit(SymbolTable* table);

/* Symbol Operations */
Symbol* symbol_create(const char* name, SymbolType type, const char* data_type);
void symbol_destroy(Symbol* symbol);

bool symbol_table_add(SymbolTable* table, Symbol* symbol);
Symbol* symbol_table_lookup(SymbolTable* table, const char* name);
Symbol* symbol_table_lookup_current_scope(SymbolTable* table, const char* name);

/* Name Generation */
char* generate_obfuscated_name(const NameGenerator* gen, SymbolType type);
char* generate_aesthetic_name(AestheticStyle style, int counter);
char* generate_unicode_name(int counter);
char* generate_hex_name(int counter);

/* Name Generator Management */
NameGenerator* name_generator_create(AestheticStyle style);
void name_generator_destroy(NameGenerator* gen);
void name_generator_reset(NameGenerator* gen);

/* Utility Functions */
bool is_valid_identifier(const char* name);
bool is_reserved_keyword(const char* name);
char* make_unique_name(SymbolTable* table, const char* base_name);

#endif /* OBFUSCATOR_SYMBOLS_H */