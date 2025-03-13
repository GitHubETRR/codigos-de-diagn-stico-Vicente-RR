/*
  Código hecho en la clase del 6 de marzo
  Arma un equipo de fútbol de 22 jugadores (11 titulares y 11 suplentes)
  Recibe nombre, número de camiseta y si es titular o no para cada jugador
  En caso de que ya haya 11 suplentes u 11 titulares, el resto de jugadores van adónde haya espacio
  Imprime el equipo titular y luego el suplente, ordenados por número de camiseta
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CANTIDAD_JUGADORES 22
#define MAX_CHAR 50
#define MIN_CAMISETA 1
#define MAX_CAMISETA 99
#define MAX_TITULARES 11
#define MAX_SUPLENTES 11
#define TECLA_TITULAR 'T'
#define DISPONIBLE 0
#define DISPONIBLE_TITULAR 1
#define DISPONIBLE_SUPLENTE 2

typedef struct
{
    char nombre[MAX_CHAR];
    int camiseta;
    bool titular;
} jugador_t;

void ArmarEquipo(jugador_t equipo[CANTIDAD_JUGADORES]);
void PedirDatos(jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
void PedirNombre(jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
void PedirCamiseta(jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
bool ComprobarCamiseta(int numero, jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
bool CamisetaUsada(int numero, jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
void PedirTitular(jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
int ComprobarLugar(jugador_t equipo[CANTIDAD_JUGADORES], int jugador);
void ImprimirEquipo(jugador_t equipo[CANTIDAD_JUGADORES]);
void BuscarCamiseta(jugador_t equipo[CANTIDAD_JUGADORES], int nCamiseta, bool titular);
void LimpiarTeclado(void);

int main()
{
    jugador_t equipo[CANTIDAD_JUGADORES];
    
    ArmarEquipo(equipo);
    ImprimirEquipo(equipo);
    
    return 0;
}

void ArmarEquipo(jugador_t equipo[CANTIDAD_JUGADORES])
{
    for (int jugador=0; jugador<CANTIDAD_JUGADORES; jugador++)
        PedirDatos(equipo, jugador);
}

void PedirDatos(jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    PedirNombre(equipo, jugador);
    PedirCamiseta(equipo, jugador);
    PedirTitular(equipo, jugador);
}

void PedirNombre(jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    printf("Introducí el nombre del %i° jugador del equipo\n", (jugador+1)); //Para que vaya del 1 al 22 y no del 0 al 21
    scanf("%s", equipo[jugador].nombre);
    LimpiarTeclado();
}

void PedirCamiseta(jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    bool valido;
    int numero;
    
    do
    {
        printf("Introducí el número de camiseta\n");
        scanf("%i", &numero);
        LimpiarTeclado();
        valido = ComprobarCamiseta(numero, equipo, jugador);
        if (!valido)
            printf("Número no válido\n");
    } while (!valido);
    equipo[jugador].camiseta = numero;
}

bool ComprobarCamiseta(int numero, jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    bool valido=true;
    
    if ((numero<MIN_CAMISETA)||(numero>MAX_CAMISETA)) //Calcula si está fuera de los límites
        valido=false;
    else
        valido = CamisetaUsada(numero, equipo, jugador); //Calcula si el número ya está ocupado
    
    return valido;
}

bool CamisetaUsada(int numero, jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    bool valido=true;
    
    for (int i=0; i<jugador; i++)
        if (equipo[i].camiseta==numero)
            valido=false;
    
    return valido;
}

void PedirTitular(jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    char tecla;
    int lugarDisponible = ComprobarLugar(equipo, jugador);
    
    switch (lugarDisponible)
    {
        case DISPONIBLE:
            printf("Enviá %c si el jugador es titular (si es suplente, otro caracter)\n", TECLA_TITULAR);
            scanf("%c", &tecla);
            LimpiarTeclado();
            if (tecla==TECLA_TITULAR)
                equipo[jugador].titular = true;
            else
                equipo[jugador].titular = false;
            break;
        
        case DISPONIBLE_TITULAR:
            equipo[jugador].titular = true;
            break;
        
        case DISPONIBLE_SUPLENTE:
            equipo[jugador].titular = false;
            break;
    }
}

int ComprobarLugar(jugador_t equipo[CANTIDAD_JUGADORES], int jugador)
{
    int lugarDisponible = DISPONIBLE;
    int titulares = 0;
    int suplentes = 0;
    
    for (int i=0; i<jugador; i++)
        if (equipo[i].titular)
            titulares++;
        else
            suplentes++;
    if (titulares>=MAX_TITULARES)
        lugarDisponible=DISPONIBLE_SUPLENTE;
    if (suplentes>=MAX_SUPLENTES)
        lugarDisponible=DISPONIBLE_TITULAR;
    
    return lugarDisponible;
}

void ImprimirEquipo(jugador_t equipo[CANTIDAD_JUGADORES])
{
    printf("Titulares:\n");
    for (int nCamiseta=MIN_CAMISETA; nCamiseta<=MAX_CAMISETA); nCamiseta++)
        BuscarCamiseta(equipo, nCamiseta, true);
    printf("Suplentes:\n");
    for (int nCamiseta=MIN_CAMISETA; nCamiseta<=MAX_CAMISETA; nCamiseta++)
        BuscarCamiseta(equipo, nCamiseta, false);
}

void BuscarCamiseta(jugador_t equipo[CANTIDAD_JUGADORES], int nCamiseta, bool titular)
{
    for (int jugador=0; jugador<CANTIDAD_JUGADORES; jugador++)
        if ((equipo[jugador].camiseta==nCamiseta)&&(equipo[jugador].titular==titular))
            printf("%i: %s\n", nCamiseta, equipo[jugador].nombre);
}

void LimpiarTeclado(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != EOF && c != '\n');
}
