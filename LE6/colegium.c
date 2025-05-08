#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "color.h"

#define MAX_CHAR 50

#define OP_SELECCIONAR_COLEGIO '1'
#define OP_AGREGAR_COLEGIO '2'
#define OP_ELIMINAR_COLEGIO '3'
#define OP_VACIAR_COLEGIOS '4'
#define OP_CERRAR_PROGRAMA '5'

#define LISTA_COLEGIO 1
#define LISTA_CURSO 2

//Estructuras
typedef struct alumno
{
    char nombre[MAX_CHAR];
    char apellido[MAX_CHAR];
    int id;
    float promedio;
    struct alumno * next;
}alumno_t;

typedef struct nota
{
    int id;
    float valor;
    struct nota * next;
}nota_t;

typedef struct evaluacion
{
    char nombre[MAX_CHAR];
    float ponderacion;
    nota_t * _nota;
    struct evaluacion * next;
}evaluacion_t;

typedef struct materia
{
    char nombre[MAX_CHAR];
    evaluacion_t * _evaluacion;
    struct materia * next;
}materia_t;

typedef struct curso
{
    char nombre[MAX_CHAR];
    materia_t * _materia;
    alumno_t * _alumno;
    struct curso * next;
}curso_t;

typedef struct colegio
{
    char nombre[MAX_CHAR];
    curso_t * _curso;
    struct colegio * next;
}colegio_t;

//Prototipos
colegio_t * Menu(colegio_t * _colegio);
void MenuColegio(curso_t * _curso);
void * Seleccionar(colegio_t * lista, int tipo);
void * Agregar(colegio_t * lista, int tipo);
int ImprimirLista(colegio_t * lista, int tipo);
int PedirUbicacion(int cantidad);
void LimpiarTeclado();
void LimpiarPantalla();

int main()
{
    colegio_t * _colegio = NULL;
    while (true)
    {
        _colegio = Menu(_colegio);
    }
    return 0;
}

colegio_t * Menu(colegio_t * _colegio)
{
    char opcion;
    bool opcionValida;
    
    printf(VERDE NEGRITA SUBRAYADO "Menú\n" RESET);
    printf(VERDE NEGRITA "1. Seleccionar colegio\n");
    printf("2. Agregar colegio\n");
    printf("3. Eliminar colegio\n");
    printf("4. Vaciar\n");
    printf("5. Salir\n" RESET);
    do
    {
        opcionValida = true;
        scanf("%c", &opcion);
        LimpiarTeclado();
        
        switch (opcion)
        {
            case OP_SELECCIONAR_COLEGIO:
                colegio_t * seleccion = Seleccionar(_colegio, LISTA_COLEGIO);
                if (seleccion != NULL)
                    MenuColegio(seleccion->_curso);
                break;
            case OP_AGREGAR_COLEGIO:
                _colegio = (colegio_t *) Agregar(_colegio, LISTA_COLEGIO);
                break;
            case OP_ELIMINAR_COLEGIO:
                
                break;
            case OP_VACIAR_COLEGIOS:
                
                break;
            case OP_CERRAR_PROGRAMA:
                
                break;
            default:
                opcionValida = false;
                printf("Opción no válida\n");
        }
    } while (!opcionValida);
    
    return _colegio;
}

void MenuColegio(curso_t * _curso)
{
    
}

void * Seleccionar(colegio_t * lista, int tipo)
{
    switch (tipo)
    {
        case LISTA_COLEGIO:
            lista = (colegio_t *) lista;
            break;
        case LISTA_CURSO:
            lista = (curso_t *) lista;
            break;
        default:
            printf("Error inesperado");
            exit(1);
            break;
    }
    int cantidad = ImprimirLista(lista, tipo);
    if (cantidad>0)
    {
        int ubicacion = PedirUbicacion(cantidad-1); //-1 para que no se pueda elegir más que el último
        for (int i=0; i<ubicacion; i++)
        {
            lista = lista->next;
        }
        printf("Selección: %s\n", lista->nombre);
    }
    else
    {
        lista = NULL;
    }
    return lista;
}

void * Agregar(colegio_t * lista, int tipo) //las listas son punteros colegio_t provisoriamente para que compile, pero luego se castea
{
    colegio_t * nuevo=NULL;
    colegio_t * aux1=NULL;
    colegio_t * aux2=NULL;
    switch (tipo)
    {
        case LISTA_COLEGIO:
            lista = (colegio_t *) lista;
            nuevo = (colegio_t *) malloc(sizeof(colegio_t));
            aux1 = (colegio_t *) aux1;
            aux2 = (colegio_t *) aux2;
            break;
        case LISTA_CURSO:
            lista = (curso_t *) lista;
            nuevo = (curso_t *) malloc(sizeof(curso_t));
            aux1 = (curso_t *) aux1;
            aux2 = (curso_t *) aux2;
            break;
        default:
            printf("Error inesperado");
            exit(1);
            break;
    }
    int ubicacion = 0;
    
    if (lista != NULL)
    {
        int cantidad = ImprimirLista(lista, tipo);
        ubicacion = PedirUbicacion(cantidad);
        aux1 = lista;
        for (int i=0; i<(ubicacion-1); i++)
            aux1 = aux1->next;
        aux2 = aux1->next;
        if (ubicacion==0)
        {
            lista = nuevo;
            lista->next = aux1;
        }
        else
        {
            aux1->next = nuevo;
            nuevo->next = aux2;
        }
    }
    else
    {
        lista = nuevo;
        lista->next = NULL;
    }
    printf("Nombre: ");
    fgets(nuevo->nombre, sizeof(nuevo->nombre), stdin);
    
    return lista;
}

int ImprimirLista(colegio_t * lista, int tipo) //la lista es un puntero colegio_t provisoriamente, pero luego se castea
{
    switch (tipo)
    {
        case LISTA_COLEGIO:
            lista = (colegio_t *) lista;
            break;
        case LISTA_CURSO:
            lista = (curso_t *) lista;
            break;
        default:
            printf("Error inesperado");
            exit(1);
            break;
    }

    int tamanio=0;
    while (lista!=NULL)
    {
        printf("%i - %s\n", (tamanio+1), lista->nombre);
        lista = lista->next;
        tamanio++;
    }
    return tamanio;
}

int PedirUbicacion(int cantidad)
{
    int ubicacion;
    bool opcionValida;
    do
    {
        opcionValida = true;
        printf("Elegí la ubicación en la lista (entre 1 y %i)\n", (cantidad+1)); //+1 para que se pueda poner al final
        scanf("%i", &ubicacion);
        LimpiarTeclado();
        ubicacion--;
        if ((ubicacion<0)||(ubicacion>cantidad))
        {
            printf("Ubicación no válida\n");
            opcionValida = false;
        }
    } while (!opcionValida);
    return ubicacion;
}

void LimpiarTeclado()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
