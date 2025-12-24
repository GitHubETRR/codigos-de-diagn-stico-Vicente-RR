/*
Este proyecto es un 4 en línea programado para PIC16F628A con un tablero LED RB 7x6 conectados a registros 74HC595
*/

#define SR595_DATA PORTAbits.RA0
#define SR595_CLK PORTAbits.RA1
#define SR595_LATCH PORTAbits.RA2
#define SR595_NUMBER 12

#define C1 PORTBbits.RB1
#define C2 PORTBbits.RB2
#define C3 PORTBbits.RB3
#define C4 PORTBbits.RB4
#define C5 PORTBbits.RB5
#define C6 PORTBbits.RB6
#define C7 PORTBbits.RB7

#define FILAS 6
#define COLUMNAS 7

#if (2 * FILAS) != SR595_NUMBER
#error "SR595_NUMBER no coincide con 2 * FILAS"
#endif

#define VACIO 0
#define ROJO 1
#define AZUL 2


#include <xc.h>
#include <stdbool.h>
#include "sr595.h"

void jugar_partida(char tablero[COLUMNAS][FILAS]);
void limpiar_tablero(char tablero[COLUMNAS][FILAS]);
void actualizar_registros(char tablero[COLUMNAS][FILAS]);

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
    } while (!detectar_victoria(tablero));
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
    uint8_t buffer[SR595_NUMBER];
    for (int i = 0; i < SR595_NUMBER; i++)
        buffer[i] = 0;
    
    for (int columna = 0; columna < COLUMNAS; columna++)
    {
        for (int fila = 0; fila < FILAS; fila++)
        {
            if (tablero[columna][fila] == ROJO)
                buffer[fila] |= (1 << columna); //Se enciende un bit en la posición columna
            if (tablero[columna][fila] == AZUL)
                buffer[FILAS + fila] |= (1 << columna); //Los registros azules le siguen a los rojos
        }
    }
    sr595_update(buffer);
}

int detectar_pulsadores()
{
    int pulsador;
    while ((C1) && (C2) && (C3) && (C4) && (C5) && (C6) && (C7)) {}
    if (~C1)
        pulsador = 0;
    else if (~C2)
        pulsador = 1;
    else if (~C3)
        pulsador = 2;
    else if (~C4)
        pulsador = 3;
    else if (~C5)
        pulsador = 4;
    else if (~C6)
        pulsador = 5;
    else
        pulsador = 6;
    
    return pulsador;
}

bool columna_disponible(int columna, char tablero[COLUMNAS][FILAS])
{
    return (tablero[columna][FILAS-1] == VACIO);
}

void colocar_ficha(int columna, int turno, char tablero[COLUMNAS][FILAS])
{
    
}

bool detectar_victoria(char tablero[COLUMNAS][FILAS])
{
    
}
