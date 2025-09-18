#ifndef OBFUSCATOR_MAIN_H
#define OBFUSCATOR_MAIN_H

#include "common/types.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "symbols/symbols.h"
#include "obfuscator/obfuscator.h"
#include "codegen/codegen.h"

/* ═══════════════════════════════════════════════════════════════════════════
 * Main Application Interface
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Application State */
typedef struct {
    ObfuscationConfig* config;
    CodeGenConfig* codegen_config;
    char* input_file;
    char* output_file;
    bool verbose;
    bool show_help;
} AppConfig;

/* Function Prototypes */

/* Main Application */
int main(int argc, char* argv[]);
int obfuscate_file(const char* input_file, const char* output_file, ObfuscationConfig* config);

/* Command Line Interface */
AppConfig* parse_command_line(int argc, char* argv[]);
void print_usage(const char* program_name);
void print_version(void);
void print_help(void);

/* File I/O */
char* read_file(const char* filename);
bool write_file(const char* filename, const char* content);

/* Error Handling */
void print_errors(Error* errors);
void cleanup_and_exit(int exit_code);

/* Configuration */
AppConfig* app_config_create_default(void);
void app_config_destroy(AppConfig* config);

/* Utility Functions */
bool file_exists(const char* filename);
char* get_file_extension(const char* filename);
char* create_output_filename(const char* input_file);

#endif /* OBFUSCATOR_MAIN_H */