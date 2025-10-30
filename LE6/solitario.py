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
            print("Pozo (", POZO, "): ", pozo[-1][NUM], pozo[-1][PALO], sep='')
        else:
            print("No hay cartas en el pozo")
    else:
        print("Tapando el pozo (", POZO, "): ", pozo[-1][NUM], pozo[-1][PALO], sep='')
    print("Restantes en el mazo:", len(mazo))

def imp_movimientos():
    print(TOMAR + " Tomar carta del mazo")
    print(APOYAR + "X Apoyar carta en el pozo")
    print(DESCARTAR + "X Descartar carta")
    print(MOVER_POZO + "X Mover la carta del pozo")
    print(MOVER + "XX Mover una carta")

def pedir_movimiento():
    movimiento = input("Ingresá tu movimiento: ")
    mov = movimiento[0] if len(movimiento) > 0 else False
    A = movimiento[1] if len(movimiento) > 1 else False
    B = movimiento[2] if len(movimiento) > 2 else False
    if mov == TOMAR and puedoTomar():
        tomar()
    elif mov == APOYAR and puedoApoyar(A):
        apoyar(A)
    elif mov == DESCARTAR and puedoDescartar(A):
        descartar(A)
    elif mov == MOVER_POZO and puedoMover(POZO, A):
        mover(POZO, A)
    elif mov == MOVER and puedoMover(A, B):
        mover(A, B)

def puedoTomar():
    return len(mazo) > 0 and pozoDisponible

def tomar():
    pozo.append(mazo[-1])
    mazo.pop()

def puedoApoyar(A):
    return pozoDisponible \
    and (A in CLAVES_PALOS and CLAVES[A] > 0 \
    or A in CLAVES_COLUMNAS and len(CLAVES[A]) > 0)

def apoyar(A):
    if A in CLAVES_PALOS:
        carta = (CLAVES[A], PALOS[CLAVES_PALOS.index[A]])
        CLAVES[A] -= 1
    else:
        carta = CLAVES[A][-1]
        CLAVES[A].pop()
    pozo.append(carta)
    pozoDisponible = False

def puedoDescartar(A):
    return (A in CLAVES_COLUMNAS or A == POZO) \
    and descartadas[PALOS.index[CLAVES[A][-1][PALO]]] == CLAVES[A][-1][NUM] -1 #La última descartada del palo es la anterior

def descartar(A):
    if A == POZO:
        pozoDisponible = True
    descartadas[PALOS.index[CLAVES[A][-1][PALO]]] += 1
    CLAVES[A].pop()

def puedoMover(A, B):
    return B == POZO and puedoApoyar(A) \
    or B in CLAVES_PALOS and puedoDescartar(A) and CLAVES[A][-1][PALO] == PALOS[CLAVES_PALOS.index(B)] \
    or B in CLAVES_COLUMNAS \
    and ((A in CLAVES_COLUMNAS or A == POZO) and CLAVES[A][-1][NUM] == CLAVES[B][-1][NUM] - 1 and CLAVES[A][-1][PALO] != PALOS[CLAVES_PALOS.index(B)] \
    or #SEGUIR con A in CLAVES_PALOS (descartar)

def mover(A, B):
    if A == POZO:
        pozoDisponible = True
    if B == POZO:
        apoyar(B)
    if B in CLAVES_PALOS:
        descartar(A)
    

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
POZO = 'P'
CLAVES_PALOS = [ORO, COPA, ESPADA, BASTO]
CLAVES_COLUMNAS = [COL_1, COL_2, COL_3, COL_4]



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

CLAVES = {
    ORO : descartadas[0],
    COPA : descartadas[1],
    ESPADA : descartadas[2],
    BASTO : descartadas[3],
    COL_1 : columnas[0],
    COL_2 : columnas[1],
    COL_3 : columnas[2],
    COL_4 : columnas[3],
    POZO : pozo
}

shuffle(mazo)
columnas[0].append(mazo[0])
columnas[0].append(mazo[1])
columnas[2].append(mazo[2])
columnas[3].append(mazo[3])
columnas[3].append(mazo[4])
columnas[3].append(mazo[5])
imp_interfaz()
pedir_movimiento()
