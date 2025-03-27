#include <stdio.h>

#define BITS_SIGNO 1
#define BITS_EXPONENTE 8
#define BITS_MANTISA 23

typedef union
{
    float f;
    struct
    {
        unsigned int mantisa : BITS_MANTISA;
        unsigned int exponente : BITS_EXPONENTE;
        unsigned int signo : BITS_SIGNO;

    };
}flotante;

int main()
{
    flotante x;
    printf("Introducí un flotante: ");
    scanf("%f", &x.f);
    printf("Signo: %b\n", x.signo);
    printf("Exponente: %b\n", x.exponente);
    printf("Mantisa: %b\n", x.mantisa);

    return 0;
}
