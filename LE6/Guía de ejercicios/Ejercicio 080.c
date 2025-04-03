/*
int *a, b;
a es un puntero a un entero y b es un entero
int *a,*b;
ambos son punteros a enteros

Para comprobar esto, uso la palabra clave _Generic, que selecciona una expresión de una lista según el tipo del argumento (en tiempo de compilación)
*/

#include <stdio.h>

#define TYPEOF(x) _Generic((x), \
    int: "int", \
    int*: "int*", \
    float: "float", \
    double: "double", \
    char: "char", \
    char*: "char*", \
    default: "otro tipo" \
)


int main()
{
    int *a1, b1;
    int *a2, *b2;
    printf("a1: %s\n", TYPEOF(a1));
    printf("b1: %s\n", TYPEOF(b1));
    printf("a2: %s\n", TYPEOF(a2));
    printf("b2: %s\n", TYPEOF(b2));

    return 0;
}
