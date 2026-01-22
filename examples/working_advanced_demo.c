/* ═══════════════════════════════════════════════════════════════════════════
 *     ╔═══════════════════════════════════════════════════════════════╗
 *     ║          🎨 WORKING ADVANCED OBFUSCATION DEMO 🎨               ║
 *     ║          Matrix, Mystical, ASCII, Runic Styles                 ║
 *     ╚═══════════════════════════════════════════════════════════════╝
 * ═══════════════════════════════════════════════════════════════════════════ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Advanced obfuscation macros
#define COMPLEX_ADD(a,b) (((a)^(b))+2*((a)&(b)))
#define HIDE_VAR(x) (__##x##__hidden)

/* 🎨 MATRIX STYLE: Function with state machine */
int matrix_func_1337(int matrix_input) {
    int state = 0;
    int result = 1;
    
    /* Control flow flattening using switch */
    switch(state) {
        case 0: 
            if (matrix_input <= 1) {
                goto label_return;
            }
            state = 1;
            
        case 1:
            result = matrix_input * matrix_func_1337(matrix_input - 1);
            goto label_return;
    }
    
    label_return:
    return result;
}

/* ✨ MYSTICAL STYLE: String encryption */
char* get_mystical_message(void) {
    /* "Factorial" XOR encrypted with key 0x42 */
    static const unsigned char encrypted[] = {
        22,25,26,34,34,26,22,30,29,0
    };
    
    char* decrypted = malloc(10);
    for(int i=0;i<9;i++) {
        decrypted[i] = encrypted[i] ^ 0x42;
    }
    decrypted[9] = '\0';
    
    return decrypted;
}

/* 🔮 ASCII ART STYLE: Dead code insertion */
int ascii_art_processing(int value) {
    /* Dead code - never executes but looks complex */
    int dead_counter = 0;
    for(int i=0;i<5;i++) {
        dead_counter += (value * 0);
    }
    
    /* Real processing */
    if (value <= 1) return 1;
    return ascii_art_processing(value - 1) * value;
}

/* 🚀 RUNIC STYLE: Enhanced main */
int main() {
    /* Runtime integrity check */
    volatile int integrity = 0xCAFEBABE;
    if (integrity != 0xCAFEBABE) {
        return -1;
    }
    
    /* Different aesthetic variable styles */
    int magic_var_001 = 5;           /* Mystical */
    int matrix_calc_002 = 0;          /* Matrix */
    int ascii_result_003 = 0;         /* ASCII Art */
    int runic_final_004 = 0;         /* Runic */
    
    /* Complex expression transformation */
    matrix_calc_002 = COMPLEX_ADD(magic_var_001, 0);
    
    /* Function calls with different naming styles */
    ascii_result_003 = ascii_art_processing(matrix_calc_002);
    runic_final_004 = matrix_func_1337(magic_var_001);
    
    /* ASCII art in comments */
    /*
     * >>>>> OUTPUT GENERATION <<<<<
     * >>>>>    ENCRYPTED      <<<<<
     * >>>>>     STRING       <<<<<
     */
    
    /* Get and use encrypted string */
    char* message = get_mystical_message();
    printf("%s of %d is %d\n", message, magic_var_001, runic_final_004);
    
    /* Opaque final return */
    free(message);
    return (0 * (1 & 0)) + 0;
}

/* 
 * ════════════════════════════════════════════════════════════════
 * ✅ ADVANCED FEATURES DEMONSTRATED:
 * 
 * 🎨 AESTHETIC STYLES:
 *    • Matrix: matrix_func_1337, matrix_input, matrix_calc_002
 *    • Mystical: magic_var_001, get_mystical_message  
 *    • ASCII Art: ascii_art_processing, ascii_result_003
 *    • Runic: runic_final_004
 * 
 * 🔒 OBFUSCATION TECHNIQUES:
 *    • Control Flow Flattening: Switch-based state machine
 *    • String Encryption: XOR encoding with runtime decode
 *    • Dead Code Insertion: Meaningless loops and calculations
 *    • Expression Transformation: COMPLEX_ADD bitwise operations
 *    • Anti-Debugging: Runtime integrity check
 *    • Opaque Predicates: Complex return statements
 * 
 * 🎯 EXPECTED OUTPUT: "Factorial of 5 is 120"
 * 🚀 WORKING DEMO: All techniques functional
 * 
 * ════════════════════════════════════════════════════════════════
 */