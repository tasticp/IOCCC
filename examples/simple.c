/* Simple example program for obfuscation testing */
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
    int num = 5;
    
    printf("Factorial of %d is %d\n", num, factorial(num));
    printf("Fibonacci of %d is %d\n", num, fibonacci(num));
    
    struct Point p = {10, 20};
    print_point(p);
    
    char message[] = "Hello, World!";
    printf("%s\n", message);
    
    return 0;
}