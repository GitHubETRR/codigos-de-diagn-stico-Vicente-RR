/*
Este proyecto es un 4 en línea programado para PIC16F628A con un tablero LED RB 7x6 conectados a registros 74HC595
*/

//Hardware
#define SR595_DATA PORTAbits.RA0
#define SR595_CLK PORTAbits.RA1
#define SR595_LATCH PORTAbits.RA2
#define SR595_NUMBER 12

#define _XTAL_FREQ 4000000

#define C1 PORTBbits.RB1
#define C2 PORTBbits.RB2
#define C3 PORTBbits.RB3
#define C4 PORTBbits.RB4
#define C5 PORTBbits.RB5
#define C6 PORTBbits.RB6
#define C7 PORTBbits.RB7

//Tablero
#define FILAS 6
#define COLUMNAS 7
#define LINEA 4

#if (2 * FILAS) != SR595_NUMBER
#error "SR595_NUMBER no coincide con 2 * FILAS"
#endif

#define VACIO 0
#define ROJO 1
#define AZUL 2

    //Decodificación
#define MAP(f, c, color) (0b10000000 | ((color == ROJO ? 0 : 1) << 6) | ((f-1) << 3) | (c-1))
#define MAP_X         (0x00)   // salida no usada
#define N_SALIDAS SR595_NUMBER * 8

const uint8_t mapa[N_SALIDAS] = {
    MAP_X, MAP(1,4,AZUL), MAP(1,3,ROJO), MAP(1,3,AZUL), MAP(1,2,ROJO), MAP(1,2,AZUL), MAP(1,1,ROJO), MAP(1,1,AZUL), 
    MAP_X, MAP(2,4,AZUL), MAP(2,3,ROJO), MAP(2,3,AZUL), MAP(2,2,ROJO), MAP(2,2,AZUL), MAP(2,1,ROJO), MAP(2,1,AZUL), 
    MAP_X, MAP(3,4,AZUL), MAP(3,3,ROJO), MAP(3,3,AZUL), MAP(3,2,ROJO), MAP(3,2,AZUL), MAP(3,1,ROJO), MAP(3,1,AZUL), 
    MAP_X, MAP(4,4,AZUL), MAP(4,3,ROJO), MAP(4,3,AZUL), MAP(4,2,ROJO), MAP(4,2,AZUL), MAP(4,1,ROJO), MAP(4,1,AZUL), 
    MAP_X, MAP(5,4,AZUL), MAP(5,3,ROJO), MAP(5,3,AZUL), MAP(5,2,ROJO), MAP(5,2,AZUL), MAP(5,1,ROJO), MAP(5,1,AZUL), 
    MAP_X, MAP(6,4,AZUL), MAP(6,3,ROJO), MAP(6,3,AZUL), MAP(6,2,ROJO), MAP(6,2,AZUL), MAP(6,1,ROJO), MAP(6,1,AZUL), 
    MAP_X, MAP(6,7,ROJO), MAP(6,7,AZUL), MAP(6,6,ROJO), MAP(6,6,AZUL), MAP(6,5,ROJO), MAP(6,5,AZUL), MAP(6,4,ROJO), 
    MAP_X, MAP(5,7,ROJO), MAP(5,7,AZUL), MAP(5,6,ROJO), MAP(5,6,AZUL), MAP(5,5,ROJO), MAP(5,5,AZUL), MAP(5,4,ROJO), 
    MAP_X, MAP(4,7,ROJO), MAP(4,7,AZUL), MAP(4,6,ROJO), MAP(4,6,AZUL), MAP(4,5,ROJO), MAP(4,5,AZUL), MAP(4,4,ROJO), 
    MAP_X, MAP(3,7,ROJO), MAP(3,7,AZUL), MAP(3,6,ROJO), MAP(3,6,AZUL), MAP(3,5,ROJO), MAP(3,5,AZUL), MAP(3,4,ROJO), 
    MAP_X, MAP(2,7,ROJO), MAP(2,7,AZUL), MAP(2,6,ROJO), MAP(2,6,AZUL), MAP(2,5,ROJO), MAP(2,5,AZUL), MAP(2,4,ROJO), 
    MAP_X, MAP(1,7,ROJO), MAP(1,7,AZUL), MAP(1,6,ROJO), MAP(1,6,AZUL), MAP(1,5,ROJO), MAP(1,5,AZUL), MAP(1,4,ROJO), 
};

//Animaciones
#define DELAY_FICHA 200
#define PARPADEOS 5
#define DELAY_PARPADEO 200
#define DELAY_RELLENO 40
#define DELAY_ANTIRREBOTE 20

//Compilación
#include <xc.h>
#include <stdbool.h>
#include "sr595.h"

typedef struct {
    char columna[LINEA];
    char fila[LINEA];
    char color;
} LineaGanadora;

void jugar_partida(char tablero[COLUMNAS][FILAS]);
void limpiar_tablero(char tablero[COLUMNAS][FILAS]);
void actualizar_registros(char tablero[COLUMNAS][FILAS]);

int detectar_pulsadores();
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
    TRISA = 0b11111000;
    TRISB = 0b11111111;
    CMCON = 0b00000111;
    OPTION_REG = 0b01111111;
    char tablero[COLUMNAS][FILAS];
    while (1)
    {
        limpiar_tablero(tablero);
        jugar_partida(tablero);
        detectar_pulsadores();
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
            columna = detectar_pulsadores();
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
    actualizar_registros(tablero);
}

void actualizar_registros(char tablero[COLUMNAS][FILAS])
{
    uint8_t buffer[SR595_NUMBER] = {0};

    for (int i = 0; i < N_SALIDAS; i++)
    {
        uint8_t m = mapa[i];
        if (m & 0b1000000)
        {
            char columna   =  m & 0b00000111;
            char fila  = (m >> 3) & 0b000111;
            char color = (m & 0b01000000) ? AZUL : ROJO;

            if (tablero[columna][fila] == color)
                buffer[i >> 3] |= (1 << (i & 0b00000111));
        }
    }

    sr595_update(buffer);
}

int detectar_pulsadores()
{
    int pulsador;
    while ((C1) && (C2) && (C3) && (C4) && (C5) && (C6) && (C7)) {}
    if (!C1)
        pulsador = 0;
    else if (!C2)
        pulsador = 1;
    else if (!C3)
        pulsador = 2;
    else if (!C4)
        pulsador = 3;
    else if (!C5)
        pulsador = 4;
    else if (!C6)
        pulsador = 5;
    else
        pulsador = 6;
        
    while (!C1 || !C2 || !C3 || !C4 || !C5 || !C6 || !C7)
        __delay_ms(DELAY_ANTIRREBOTE);
    
    return pulsador;
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
        actualizar_registros(tablero);
        __delay_ms(DELAY_FICHA);

        tablero[columna][f] = VACIO;
    }

    // fijar ficha
    tablero[columna][fila_destino] = turno;
    actualizar_registros(tablero);
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

        actualizar_registros(tablero);
        __delay_ms(DELAY_PARPADEO);

        // encender
        for (int i = 0; i < LINEA; i++)
            tablero[l->columna[i]][l->fila[i]] = l->color;

        actualizar_registros(tablero);
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
            actualizar_registros(tablero);
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
            actualizar_registros(tablero);
            __delay_ms(DELAY_RELLENO);
        }
    }
}
