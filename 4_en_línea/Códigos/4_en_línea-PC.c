//Tablero
#define FILAS 6
#define COLUMNAS 7
#define LINEA 4

#define VACIO 0
#define ROJO 1
#define AZUL 2

//Animaciones
#include <unistd.h>
#define __delay_ms(ms) usleep(ms*1000)
#define DELAY_FICHA 200
#define PARPADEOS 5
#define DELAY_PARPADEO 200
#define DELAY_RELLENO 40
#define DELAY_ANTIRREBOTE 20

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char columna[LINEA];
    char fila[LINEA];
    char color;
} LineaGanadora;

void jugar_partida(char tablero[COLUMNAS][FILAS]);
void limpiar_tablero(char tablero[COLUMNAS][FILAS]);
void imprimir_tablero(char tablero[COLUMNAS][FILAS]);

int detectar_entrada();
bool columna_disponible(int columna, char tablero[COLUMNAS][FILAS]);
void colocar_ficha(int columna, char turno, char tablero[COLUMNAS][FILAS]);

bool detectar_final(char tablero[COLUMNAS][FILAS]);
bool detectar_victoria(char tablero[COLUMNAS][FILAS]);
bool buscar_horizontal(char tablero[COLUMNAS][FILAS], LineaGanadora *l);
bool buscar_vertical(char tablero[COLUMNAS][FILAS], LineaGanadora *l);
bool buscar_diag_arriba(char tablero[COLUMNAS][FILAS], LineaGanadora *l);
bool buscar_diag_abajo(char tablero[COLUMNAS][FILAS], LineaGanadora *l);
void animar_linea(LineaGanadora *l, char tablero[COLUMNAS][FILAS]);
void animar_relleno_victoria(char tablero[COLUMNAS][FILAS], char color);
bool detectar_empate(char tablero[COLUMNAS][FILAS]);
void animar_relleno_empate(char tablero[COLUMNAS][FILAS]);

void main(void)
{
    char tablero[COLUMNAS][FILAS];
    while (1)
    {
        limpiar_tablero(tablero);
        jugar_partida(tablero);
        detectar_entrada();
    }
    
    return;
}

void jugar_partida(char tablero[COLUMNAS][FILAS])
{
    char turno = ROJO;
    do
    {
        int columna;
        do
            columna = detectar_entrada();
        while (!columna_disponible(columna, tablero));
        colocar_ficha(columna, turno, tablero);
        turno = (turno == ROJO)?AZUL:ROJO;
    } while (!detectar_final(tablero));
}

void limpiar_tablero(char tablero[COLUMNAS][FILAS])
{
    for (int columna = 0; columna < COLUMNAS; columna++)
    {
        for (int fila = 0; fila < FILAS; fila++)
        {
            tablero[columna][fila] = VACIO;
        }
    }
    imprimir_tablero(tablero);
}

void imprimir_tablero(char tablero[COLUMNAS][FILAS])
{
    #ifdef _WIN32
        system("cls"); // Limpiar pantalla en Windows
    #else
        system("clear"); // Limpiar pantalla en Unix/Linux
    #endif

    for (int fila = FILAS-1; fila >= 0; fila--)
    {
        for (int columna = 0; columna < COLUMNAS; columna++)
        {
            switch (tablero[columna][fila])
            {
                case VACIO:
                printf(" ");
                break;

                case ROJO:
                printf("X");
                break;

                case AZUL:
                printf("O");
                break;
            }
            printf(" | ");
        }
        printf("\n");
    }
    printf("1   2   3   4   5   6   7\n");
}

int detectar_entrada()
{
    int n;
    do
    {
        scanf("%i", &n);
    } while ((n < 1) || (n > 7));
    n--;
    
    return n;
}

bool columna_disponible(int columna, char tablero[COLUMNAS][FILAS])
{
    return (tablero[columna][FILAS-1] == VACIO);
}

void colocar_ficha(int columna, char turno, char tablero[COLUMNAS][FILAS])
{
    int fila_destino = -1;

    // buscar destino
    for (int f = 0; f < FILAS; f++)
    {
        if (tablero[columna][f] == VACIO)
        {
            fila_destino = f;
            break;
        }
    }

    // animación desde arriba (FILAS-1 debe ser la parte superior visual)
    for (int f = FILAS - 1; f > fila_destino; f--)
    {
        tablero[columna][f] = turno;
        imprimir_tablero(tablero);
        __delay_ms(DELAY_FICHA);

        tablero[columna][f] = VACIO;
    }

    // fijar ficha
    tablero[columna][fila_destino] = turno;
    imprimir_tablero(tablero);
}


bool detectar_final(char tablero[COLUMNAS][FILAS])
{
    if (detectar_victoria(tablero))
        return true;
    
    if (detectar_empate(tablero))
        return true;
    
    return false;
}

bool detectar_victoria(char tablero[COLUMNAS][FILAS])
{
    LineaGanadora l;

    if (buscar_horizontal(tablero, &l) ||
        buscar_vertical(tablero, &l)   ||
        buscar_diag_arriba(tablero, &l)    ||
        buscar_diag_abajo(tablero, &l))
    {
        animar_linea(&l, tablero);
        animar_relleno_victoria(tablero, l.color);
        return true;
    }

    return false;
}

bool buscar_horizontal(char tablero[COLUMNAS][FILAS], LineaGanadora *l)
{
    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c <= COLUMNAS - LINEA; c++)
        {
            char v = tablero[c][f];
            bool hayLinea = true;
            if (v != VACIO)
            {
                for (int i = 1; i < LINEA; i++)
                    if (v != tablero[c+i][f])
                        hayLinea = false;
            }
            else hayLinea = false;
            
            if (hayLinea)
            {
                for (int i = 0; i < LINEA; i++)
                {
                    l->columna[i] = c + i;
                    l->fila[i] = f;
                }
                l->color = v;
                return true;
            }
        }
    }
    return false;
}

bool buscar_vertical(char tablero[COLUMNAS][FILAS], LineaGanadora *l)
{
    for (int c = 0; c < COLUMNAS; c++)
    {
        for (int f = 0; f <= FILAS - LINEA; f++)
        {
            char v = tablero[c][f];
            bool hayLinea = true;
            if (v != VACIO)
            {
                for (int i = 1; i < LINEA; i++)
                    if (v != tablero[c][f+i])
                        hayLinea = false;
            }
            else hayLinea = false;
            
            if (hayLinea)
            {
                for (int i = 0; i < LINEA; i++)
                {
                    l->columna[i] = c;
                    l->fila[i] = f + i;
                }
                l->color = v;
                return true;
            }
        }
    }
    return false;
}

bool buscar_diag_arriba(char tablero[COLUMNAS][FILAS], LineaGanadora *l)
{
    for (int c = 0; c <= COLUMNAS - LINEA; c++)
    {
        for (int f = 0; f <= FILAS - LINEA; f++)
        {
            char v = tablero[c][f];
            bool hayLinea = true;
            if (v != VACIO)
            {
                for (int i = 1; i < LINEA; i++)
                    if (v != tablero[c+i][f+i])
                        hayLinea = false;
            }
            else hayLinea = false;
            
            if (hayLinea)
            {
                for (int i = 0; i < LINEA; i++)
                {
                    l->columna[i]  = c + i;
                    l->fila[i] = f + i;
                }
                l->color = v;
                return true;
            }
        }
    }
    return false;
}

bool buscar_diag_abajo(char tablero[COLUMNAS][FILAS], LineaGanadora *l)
{
    for (int c = 0; c <= COLUMNAS - LINEA; c++)
    {
        for (int f = LINEA-1; f < FILAS; f++)
        {
            char v = tablero[c][f];
            bool hayLinea = true;
            if (v != VACIO)
            {
                for (int i = 1; i < LINEA; i++)
                    if (v != tablero[c+i][f-i])
                        hayLinea = false;
            }
            else hayLinea = false;
            
            if (hayLinea)
            {
                for (int i = 0; i < LINEA; i++)
                {
                    l->columna[i]  = c + i;
                    l->fila[i] = f - i;
                }
                l->color = v;
                return true;
            }
        }
    }
    return false;
}

void animar_linea(LineaGanadora *l, char tablero[COLUMNAS][FILAS])
{
    for (int p = 0; p < PARPADEOS; p++)
    {
        // apagar
        for (int i = 0; i < LINEA; i++)
            tablero[l->columna[i]][l->fila[i]] = VACIO;

        imprimir_tablero(tablero);
        __delay_ms(DELAY_PARPADEO);

        // encender
        for (int i = 0; i < LINEA; i++)
            tablero[l->columna[i]][l->fila[i]] = l->color;

        imprimir_tablero(tablero);
        __delay_ms(DELAY_PARPADEO);
    }
}

void animar_relleno_victoria(char tablero[COLUMNAS][FILAS], char color)
{
    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            tablero[c][f] = color;
            imprimir_tablero(tablero);
            __delay_ms(DELAY_RELLENO);
        }
    }
}

bool detectar_empate(char tablero[COLUMNAS][FILAS])
{
    for (int c = 0; c < COLUMNAS; c++)
        if (tablero[c][FILAS-1] == VACIO)
            return false;
    
    animar_relleno_empate(tablero);
    return true;
}

void animar_relleno_empate(char tablero[COLUMNAS][FILAS])
{
    for (int f = 0; f < FILAS/2; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            tablero[c][f] = ROJO;
            tablero[c][FILAS-f-1] = AZUL;
            imprimir_tablero(tablero);
            __delay_ms(DELAY_RELLENO);
        }
    }
}
