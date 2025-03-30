#Este código es el proyecto final del curso Python Essentials
from random import randrange

def display_board(board):
    # La función acepta un parámetro el cual contiene el estado actual del tablero
    # y lo muestra en la consola.
    
    for row in range(3):
        print("+-------+-------+-------+")
        print("|       |       |       |")
        for column in range(3):
            print("|   ", board[row][column], "   ", sep='', end='')
        print("|")
        print("|       |       |       |")
    print("+-------+-------+-------+")

def enter_move(board):
    # La función acepta el estado actual del tablero y pregunta al usuario acerca de su movimiento,  
    # verifica la entrada y actualiza el tablero acorde a la decisión del usuario.
    valid_move = False
    while not valid_move:
        try:
            move = int(input("Ingresá el número de casillero para marcar: "))
            valid_move = check_move(board, (move-1)) #move-1 porque los casilleros no empiezan desde 0
            if valid_move:
                board[(move-1)//3][(move-1)%3] = 'O'
            else:
                print("Por favor, ingresá un dato válido")
        except:
            print("Por favor, ingresá un dato válido")

def check_move(board, move):
    move = move // 3, move % 3
    if move in make_list_of_free_fields(board):
        return True
    else:
        return False

def make_list_of_free_fields(board):
    # La función examina el tablero y construye una lista de todos los cuadros vacíos. 
    # La lista esta compuesta por tuplas, cada tupla es un par de números que indican la fila y columna.
    free_fields = []
    for row in range(3):
        for column in range(3):
            if board[row][column] != 'X' and board[row][column] != 'O':
                free_fields.append((row, column))
    return free_fields

def victory_for(board, sign):
    # La función analiza el estatus del tablero para verificar si 
    # el jugador que utiliza las 'O's o las 'X's ha ganado el juego.
    
    for a in range(3): #Verifica horizontales y verticales
        victory_horizontal = True
        victory_vertical = True
        for b in range(3):
            if board[a][b] != sign:
                victory_horizontal = False
            if board[b][a] != sign:
                victory_vertical = False
            
        if victory_horizontal or victory_vertical:
            return True
    
    if board[1][1] == sign: #Verifica diagonales
        if board[0][0] == board[2][2] == sign or \
           board[0][2] == board[2][0] == sign:
               return True
    return False

def draw_move(board):
    # La función dibuja el movimiento de la máquina y actualiza el tablero.
    free_fields = make_list_of_free_fields(board)
    valid_move = False
    while not valid_move:
        row = randrange(3)
        column = randrange(3)
        if (row, column) in free_fields:
            board[row][column] = 'X'
            valid_move = True


board = [[str(row*3+column+1) for column in range(3)] for row in range(3)]

board[1][1] = 'X'
turn = 'O'
playing = True
while playing:
    display_board(board)
    if turn == 'O':
        enter_move(board)
    else:
        draw_move(board)
    if victory_for(board, turn):
        display_board(board)
        print(turn, "gana la partida")
        playing = False
    elif len(make_list_of_free_fields(board)) == 0:
        display_board(board)
        print("Empate")
        playing = False
    
    if turn == 'O':
        turn = 'X'
    else:
        turn = 'O'
