from random import choice

VIDAS = 6
DIBUJOS = ['''
  +---+
  |   |
      |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
  |   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\\  |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\\  |
 /    |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\\  |
 / \\  |
      |
=========''']

def Jugar():
    palabra = ElegirPalabra()
    descubiertas = [False for i in palabra]
    letras = []
    vidasUsadas = 0
    
    while vidasUsadas < VIDAS and False in descubiertas:
        Dibujar(vidasUsadas, palabra, descubiertas)
        letraIngresada = PedirLetra(letras)
        letras.append(letraIngresada)
        if letraIngresada not in palabra:
            vidasUsadas += 1
        else:
            for i in range(len(palabra)):
                if palabra[i] == letraIngresada:
                    descubiertas[i] = True
    if vidasUsadas == VIDAS:
        print(DIBUJOS[VIDAS])
        print("Perdiste")
    else:
        print("Ganaste")
    print("La palabra era:", palabra)

def ElegirPalabra():
    archivo = open("palabras.txt", "r", encoding="utf-8") #Abre el archivo en modo lectura (caracteres codificados en utf-8)
    lista = archivo.readlines() #Guarda una lista de todas las palabras separadas por un \n
    palabra = choice(lista).strip() #Elige un elemento aleatorio de la lista. strip elimina el salto de linea
    archivo.close()
    return palabra

def Dibujar(vidasUsadas, palabra, descubiertas):
    print(DIBUJOS[vidasUsadas])
    print()
    for i in range(len(palabra)):
        if (descubiertas[i]):
            print(palabra[i], end=' ')
        else:
            print('_', end=' ')
    print()

def PedirLetra(letras):
    for l in letras:
        print(l, end=', ')
    print()
    valida = False
    while not valida:
        letraIngresada = input("Ingrese una letra: ").lower()
        if len(letraIngresada) == 1 and letraIngresada.isalpha() and letraIngresada not in letras: #isalpha verifica que sea una letra
            valida = True
        else:
            print("No es válida")
    return letraIngresada


jugar = True
while jugar:
    Jugar()
    if (input("Ingrese 1 para jugar de nuevo: ") != '1'):
        jugar = False
print("Chau")
