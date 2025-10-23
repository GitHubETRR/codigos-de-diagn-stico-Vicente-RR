import os
from random import shuffle

def imp_interfaz():
    imp_descartadas()
    imp_columnas()
    imp_pozo()
    imp_movimientos()

def imp_descartadas():
    for i in range(NUM_PALOS):
        print(f"{descartadas[i]:2d}", PALOS[i], sep='', end=' ')
    print("\n ", ORO, "   ", COPA, "   ", ESPADA, "   ", BASTO, "\n", sep='')

def imp_columnas():
    tamaño = max([len(columna) for columna in columnas])
    for i in range(tamaño):
        for columna in columnas:
            if len(columna) > i:
                print('|', f"{columna[i][NUM]:2d}", columna[i][PALO], sep='', end=' ')
            else:
                print('|    ', end='')
        print('|')
    print("  ", COL_1, "    ", COL_2, "    ", COL_3, "    ", COL_4, "\n", sep='')

def imp_pozo():
    if pozoDisponible:
        if len(pozo) > 0:
            print("Pozo (P): ", pozo[-1][NUM], pozo[-1][PALO], sep='')
        else:
            print("No hay cartas en el pozo")
    else:
        print("Tapando el pozo (P): ", pozo[-1][NUM], pozo[-1][PALO], sep='')
    print("Restantes en el mazo:", len(mazo))

def imp_movimientos():
    print(TOMAR + " Tomar carta del mazo")
    print(APOYAR + "X Apoyar carta en el pozo")
    print(DESCARTAR + "X Descartar carta")
    print(MOVER_POZO + "X Mover la carta del pozo")
    print(MOVER + "XX Mover una carta")

def pedir_movimiento():
    movimiento = input("Ingresá tu movimiento: ")
    if movimiento[0] == TOMAR and len(mazo) > 0 and pozoDisponible):
        tomar()
    elif movimiento[0] == APOYAR and pozoDisponible:
        SEGUIR
    
    

def limpiar_pantalla():
    os.system('cls' if os.name == 'nt' else 'clear')
    
PALOS = ["💰", "🍷", "🔪", "🌿"]
NUM_CARTAS = 12
NUM_PALOS = 4
NUM_COLUMNAS = 4
NUM = 0
PALO = 1

ORO = 'A'
COPA = 'B'
ESPADA = 'C'
BASTO = 'D'
COL_1 = 'E'
COL_2 = 'F'
COL_3 = 'G'
COL_4 = 'H'



TOMAR = '+'
APOYAR = '|'
DESCARTAR = '-'
MOVER_POZO = '.'
MOVER = ','

mazo = [(valor, palo) for palo in PALOS for valor in range(1, (NUM_CARTAS+1))]
columnas = [[] for i in range(NUM_COLUMNAS)]
descartadas = [0 for i in range(NUM_PALOS)]
pozo = []
pozoDisponible = True

shuffle(mazo)
columnas[0].append(mazo[0])
columnas[0].append(mazo[1])
columnas[2].append(mazo[2])
columnas[3].append(mazo[3])
columnas[3].append(mazo[4])
columnas[3].append(mazo[5])
imp_interfaz()
pedir_movimiento()
