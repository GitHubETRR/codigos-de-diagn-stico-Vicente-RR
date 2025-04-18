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
colegio_t * AgregarColegio(colegio_t * _colegio);
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
                
                break;
            case OP_AGREGAR_COLEGIO:
                _colegio = AgregarColegio(_colegio);
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

colegio_t * AgregarColegio(colegio_t * _colegio)
{
    int ubicacion = 0;
    colegio_t * nuevoColegio = (colegio_t *) malloc(sizeof(colegio_t));
    if (_colegio != NULL)
    {
        int cantidadColegios = ImprimirLista(_colegio, LISTA_COLEGIO);
        ubicacion = PedirUbicacion(cantidadColegios);
        colegio_t * aux1 = _colegio;
        for (int i=0; i<(ubicacion-1); i++)
            aux1 = aux1->next;
        colegio_t * aux2 = aux1->next;
        if (ubicacion==0)
        {
            _colegio = nuevoColegio;
            _colegio->next = aux1;
        }
        else
        {
            aux1->next = nuevoColegio;
            nuevoColegio->next = aux2;
        }
    }
    else
    {
        _colegio = nuevoColegio;
        _colegio->next = NULL;
    }
    printf("Nombre: ");
    fgets(nuevoColegio->nombre, sizeof(nuevoColegio->nombre), stdin);
    
    return _colegio;
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
