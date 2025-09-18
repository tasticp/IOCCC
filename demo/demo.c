/* Demo program to showcase the C Code Obfuscator */
#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

struct Point {
    int x;
    int y;
};

void print_point(struct Point p) {
    printf("Point: (%d, %d)\n", p.x, p.y);
}

int main() {
    int number = 5;
    
    printf("=== C Code Obfuscator Demo ===\n");
    printf("Original readable code:\n\n");
    
    printf("Factorial of %d is %d\n", number, factorial(number));
    printf("Fibonacci of %d is %d\n", number, fibonacci(number));
    
    struct Point origin = {0, 0};
    struct Point point = {10, 20};
    
    print_point(origin);
    print_point(point);
    
    char message[] = "Hello, Obfuscated World!";
    printf("%s\n", message);
    
    return 0;
}