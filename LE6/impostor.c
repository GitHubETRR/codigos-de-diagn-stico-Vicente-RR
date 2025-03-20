#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MIN_JUGADORES 3
#define ARCHIVO "palabras.txt"
#define MAX_CHAR 100
#define NOESPIA false
#define ESPIA true

int PedirJugadores(void);
int PedirEspias(int jugadores);
void ElegirEspias(bool rolEspia[], int jugadores, int espias);
void VaciarEspias(bool rolEspia[], int jugadores);
char* elegir_palabra_aleatoria(void);
void MostrarPalabra(int jugadores, bool rolEspia[], char* palabra);
void LimpiarPantalla(void);


int main() {
    int jugadores = PedirJugadores();
    int espias = PedirEspias(jugadores);
    bool rolEspia[jugadores];
    
    ElegirEspias(rolEspia, jugadores, espias);
    char *palabra = elegir_palabra_aleatoria();
    if (!palabra) {
        printf("Error al elegir la palabra.\n");
        free(palabra);
        exit(1);
    }
    MostrarPalabra(jugadores, rolEspia, palabra);
    free(palabra);
    return 0;
}

int PedirJugadores()
{
    int jugadores;
    do
    {
        printf("¿Cuántos jugadores?\n");
        scanf("%i", &jugadores);
        if (jugadores<MIN_JUGADORES) printf("Mínimo %i\n", MIN_JUGADORES);
    } while (jugadores<MIN_JUGADORES);
    return jugadores;
}

int PedirEspias(int jugadores)
{
    int espias;
    do
    {
        printf("¿Cuántos espías?\n");
        scanf("%i", &espias);
        if (espias>jugadores) printf("Máximo %i\n", jugadores);
    } while (espias>jugadores);
    return espias;
}

void ElegirEspias(bool rolEspia[], int jugadores, int espias)
{
    VaciarEspias(rolEspia, jugadores);
    while (espias>0)
    {
        int espiaElegido;
        do
            espiaElegido = rand() % jugadores;
        while (rolEspia[espiaElegido] == ESPIA);
        rolEspia[espiaElegido] = ESPIA;
        espias--;
    }
}

void VaciarEspias(bool rolEspia[], int jugadores)
{
    for (int i=0; i<jugadores; i++)
        rolEspia[i] = NOESPIA;
}

char* elegir_palabra_aleatoria() {
    FILE *file = fopen(ARCHIVO, "r");
    if (!file) {
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    int cantidad = 0;
    char palabra[MAX_CHAR];

    // Contar la cantidad de palabras en el archivo
    while (fgets(palabra, sizeof(palabra), file)) {
        cantidad++;
    }

    if (cantidad == 0) {
        fclose(file);
        return NULL;
    }

    // Elegir un índice aleatorio
    srand(time(NULL));
    int indice = rand() % cantidad;

    // Volver a leer el archivo hasta llegar a la palabra seleccionada
    rewind(file);
    for (int i = 0; i <= indice; i++) {
        fgets(palabra, sizeof(palabra), file);
    }

    fclose(file);

    // Reservar memoria y devolver la palabra
    char* seleccionada = malloc(strlen(palabra) + 1);
    if (seleccionada) {
        strcpy(seleccionada, palabra);
        seleccionada[strcspn(seleccionada, "\n")] = 0;  // Quitar el salto de línea
    }
    return seleccionada;
}

void MostrarPalabra(int jugadores, bool rolEspia[], char* palabra)
{
    LimpiarPantalla();
    getchar();
    for (int i=0; i<jugadores; i++)
    {
        printf("Jugador %i\n", (i+1));
        printf("Presioná enter para revelar tu rol\n");
        getchar();
        if (rolEspia[i]==ESPIA)
            printf("Espía\n");
        else
            printf("Palabra: %s\n", palabra);
        printf("Presioná enter para pasar\n");
        getchar();
        LimpiarPantalla();
    }
}

void LimpiarPantalla()
{
    // Detectar el sistema operativo
    #ifdef _WIN32
        system("cls"); // Limpiar pantalla en Windows
    #else
        system("clear"); // Limpiar pantalla en Unix/Linux
    #endif
}
