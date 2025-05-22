#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "color.h"

#define MAX_CHAR 50

#define OP_SELECCIONAR '1'
#define OP_AGREGAR '2'
#define OP_ELIMINAR '3'
#define OP_VACIAR '4'
#define OP_SALIR '5'

#define OP_SELECCIONAR_2 '5'
#define OP_AGREGAR_2 '6'
#define OP_ELIMINAR_2 '7'
#define OP_VACIAR_2 '8'
#define OP_SALIR_2 '9'

#define LISTA_COLEGIO 1
#define LISTA_CURSO 2
#define LISTA_MATERIA 3
#define LISTA_EVALUACION 4
#define LISTA_NOTA 5
#define LISTA_ALUMNO 6

//Estructuras
typedef struct alumno
{
    char nombre[MAX_CHAR];
    struct alumno * next;
    char apellido[MAX_CHAR];
    int id;
    float promedio;
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
    struct evaluacion * next;
    float ponderacion;
    nota_t * _nota;
}evaluacion_t;

typedef struct materia
{
    char nombre[MAX_CHAR];
    struct materia * next;
    evaluacion_t * _evaluacion;
}materia_t;

typedef struct curso
{
    char nombre[MAX_CHAR];
    struct curso * next;
    materia_t * _materia;
    alumno_t * _alumno;
}curso_t;

typedef struct colegio
{
    char nombre[MAX_CHAR];
    struct colegio * next;
    curso_t * _curso;
}colegio_t;

//Prototipos
void MenuColegio(colegio_t * _curso);
void MenuCurso(curso_t * curso);
void MenuMateria(materia_t * materia, alumno_t * _alumno);
void MenuAlumno(alumno_t * alumno, materia_t * _materia);
//las listas son punteros colegio_t provisoriamente para que compile, pero luego se castea
void * Seleccionar(colegio_t * lista, int tipo);
void * Agregar(colegio_t * lista, int tipo);
int ImprimirLista(colegio_t * lista, int tipo);
int PedirUbicacion(int cantidad);

void * BorrarElemento(colegio_t * lista, int tipo);
void BorrarLista(colegio_t * lista, int tipo);
void BorrarNotas(evaluacion_t * evaluacion);
void BorrarEvaluaciones(materia_t * materia);
void BorrarMaterias(curso_t * curso);
void BorrarAlumnos(curso_t * curso);
void BorrarCursos(colegio_t * colegio);

void LimpiarTeclado();
void LimpiarPantalla();

int main()
{
    colegio_t * _colegio = NULL;
    bool salir = false;
    
    while (!salir)
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
                case OP_SELECCIONAR:
                    colegio_t * seleccion = Seleccionar(_colegio, LISTA_COLEGIO);
                    if (seleccion != NULL)
                    {
                        LimpiarPantalla();
                        MenuColegio(seleccion);
                    }
                    else
                    {
                        printf("No hay colegios guardados");
                        getchar();
                    }
                    break;
                case OP_AGREGAR:
                    _colegio = (colegio_t *) Agregar(_colegio, LISTA_COLEGIO);
                    break;
                case OP_ELIMINAR:
                    _colegio = BorrarElemento(_colegio, LISTA_COLEGIO);
                    break;
                case OP_VACIAR:
                        BorrarLista(_colegio, LISTA_COLEGIO);
                        _colegio = NULL;
                    break;
                case OP_SALIR:
                    BorrarLista(_colegio, LISTA_COLEGIO);
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
            LimpiarPantalla();
        } while (!opcionValida);
    }
    
    return 0;
}

void MenuColegio(colegio_t * colegio)
{
    bool salir = false;
    curso_t * _curso = colegio->_curso;
    
    while (!salir)
    {
        char opcion;
        bool opcionValida;
        
        printf(VERDE NEGRITA SUBRAYADO "Colegio: %s\n" RESET, colegio->nombre);
        printf(VERDE NEGRITA "1. Seleccionar curso\n");
        printf("2. Agregar curso\n");
        printf("3. Eliminar curso\n");
        printf("4. Vaciar\n");
        printf("5. Salir\n" RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_SELECCIONAR:
                    curso_t * seleccion = (curso_t *) Seleccionar((colegio_t *) _curso, LISTA_CURSO);
                    if (seleccion != NULL)
                    {
                        LimpiarPantalla();
                        MenuCurso(seleccion);
                    }
                    else
                    {
                        printf("No hay cursos guardados");
                        getchar();
                    }
                    break;
                case OP_AGREGAR:
                    _curso = (curso_t *) Agregar((colegio_t *) _curso, LISTA_CURSO);
                    break;
                case OP_ELIMINAR:
                    _curso = (curso_t *) BorrarElemento((colegio_t *) _curso, LISTA_CURSO);
                    break;
                case OP_VACIAR:
                        BorrarLista((colegio_t *) _curso, LISTA_CURSO);
                        _curso = NULL;
                    break;
                case OP_SALIR:
                    colegio->_curso = _curso;
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
            LimpiarPantalla();
        } while (!opcionValida);
    }
}

void MenuCurso(curso_t * curso)
{
    bool salir = false;
    materia_t * _materia = curso->_materia;
    alumno_t * _alumno = curso->_alumno;
    
    while (!salir)
    {
        char opcion;
        bool opcionValida;
        
        printf(VERDE NEGRITA SUBRAYADO "Curso: %s\n" RESET, curso->nombre);
        printf(VERDE NEGRITA "1. Seleccionar materia\n");
        printf("2. Agregar materia\n");
        printf("3. Eliminar materia\n");
        printf("4. Vaciar materias\n");
        printf("5. Seleccionar alumno\n");
        printf("6. Agregar alumno\n");
        printf("7. Eliminar alumno\n");
        printf("8. Vaciar alumnos\n");
        printf("9. Salir\n" RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_SELECCIONAR:
                    materia_t * seleccionMateria = (materia_t *) Seleccionar((colegio_t *) _materia, LISTA_MATERIA);
                    if (seleccionMateria != NULL)
                    {
                        LimpiarPantalla();
                        MenuMateria(seleccionMateria, curso->_alumno);
                    }
                    else
                    {
                        printf("No hay materias guardadas");
                        getchar();
                    }
                    break;
                case OP_AGREGAR:
                    _materia = (materia_t *) Agregar((colegio_t *) _materia, LISTA_MATERIA);
                    break;
                case OP_ELIMINAR:
                    _materia = (materia_t *) BorrarElemento((colegio_t *) _materia, LISTA_MATERIA);
                    break;
                case OP_VACIAR:
                        BorrarLista((colegio_t *) _materia, LISTA_MATERIA);
                        _materia = NULL;
                    break;
                case OP_SELECCIONAR_2:
                    alumno_t * seleccionAlumno = (alumno_t *) Seleccionar((colegio_t *) _alumno, LISTA_ALUMNO);
                    if (seleccionAlumno != NULL)
                    {
                        LimpiarPantalla();
                        MenuAlumno(seleccionAlumno, curso->_materia);
                    }
                    else
                    {
                        printf("No hay cursos guardados");
                        getchar();
                    }
                    break;
                case OP_AGREGAR_2:
                    _alumno = (alumno_t *) Agregar((colegio_t *) _alumno, LISTA_ALUMNO);
                    break;
                case OP_ELIMINAR_2:
                    _alumno = (alumno_t *) BorrarElemento((colegio_t *) _alumno, LISTA_ALUMNO);
                    break;
                case OP_VACIAR_2:
                        BorrarLista((colegio_t *) _alumno, LISTA_ALUMNO);
                        _alumno = NULL;
                    break;
                case OP_SALIR_2:
                    curso->_materia = _materia;
                    curso->_alumno = _alumno;
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
            LimpiarPantalla();
        } while (!opcionValida);
    }
}

void MenuMateria(materia_t * materia, alumno_t * _alumno)
{
    
}

void MenuAlumno(alumno_t * alumno, materia_t * _materia)
{
    
}

void * Seleccionar(colegio_t * lista, int tipo)
{
    int cantidad = ImprimirLista(lista, tipo);
    if (cantidad>0)
    {
        int ubicacion = PedirUbicacion(cantidad);
        for (int i=0; i<ubicacion; i++)
        {
            lista = lista->next;
        }
    }
    else
    {
        lista = NULL;
    }
    return lista;
}

void * Agregar(colegio_t * lista, int tipo)
{
    colegio_t * aux1=NULL;
    colegio_t * aux2=NULL;
    int tamanio=0;
    switch (tipo)
    {
        case LISTA_COLEGIO:
            tamanio = sizeof(colegio_t);
            break;
        case LISTA_CURSO:
            tamanio = sizeof(curso_t);
            break;
        case LISTA_MATERIA:
            tamanio = sizeof(materia_t);
            break;
        case LISTA_EVALUACION:
            tamanio = sizeof(evaluacion_t);
            break;
        case LISTA_NOTA:
            tamanio = sizeof(nota_t);
            break;
        case LISTA_ALUMNO:
            tamanio = sizeof(alumno_t);
            break;
    }
    colegio_t * nuevo = (colegio_t *) malloc(tamanio);;
    int ubicacion = 0;
    
    if (lista != NULL)
    {
        int cantidad = ImprimirLista(lista, tipo);
        ubicacion = PedirUbicacion(cantidad+1); //+1 para que se pueda poner al final
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

int ImprimirLista(colegio_t * lista, int tipo)
{
    int tamanio = 0;
    while (lista != NULL)
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
        printf("Elegí la ubicación en la lista (entre 1 y %i)\n", cantidad);
        scanf("%i", &ubicacion);
        LimpiarTeclado();
        ubicacion--;
        if ((ubicacion<0)||(ubicacion>=cantidad))
        {
            printf("Ubicación no válida\n");
            opcionValida = false;
        }
    } while (!opcionValida);
    return ubicacion;
}

void * BorrarElemento(colegio_t * lista, int tipo)
{
    colegio_t * elemento = lista;
    colegio_t * previo;
    
    int cantidad = ImprimirLista(lista, tipo);
    int ubicacion = PedirUbicacion(cantidad);
    for (int i=0; i<ubicacion; i++)
        elemento = elemento->next;
    if (elemento==lista)
        lista = elemento->next;
    else
    {
        previo = lista;
        while (previo->next != elemento)
            previo = previo->next;
        previo->next = elemento->next;
    }
    switch (tipo)
    {
        case LISTA_COLEGIO:
            BorrarCursos((colegio_t *) elemento);
            break;
        case LISTA_CURSO:
            BorrarMaterias((curso_t *) elemento);
            BorrarAlumnos((curso_t *) elemento);
            break;
        case LISTA_MATERIA:
            BorrarEvaluaciones((materia_t *) elemento);
            break;
        case LISTA_EVALUACION:
            BorrarNotas((evaluacion_t *) elemento);
            break;
    }
    free(elemento);
    return lista;
}

void BorrarLista(colegio_t * lista, int tipo)
{
    switch (tipo)
    {
        case LISTA_COLEGIO:
            while (lista != NULL)
            {
                BorrarCursos((colegio_t *) lista);
                colegio_t * aux = lista->next;
                free((colegio_t *) lista);
                lista = aux;
            }
            break;
        case LISTA_CURSO:
            while (lista != NULL)
            {
                BorrarMaterias((curso_t *) lista);
                BorrarAlumnos((curso_t *) lista);
                colegio_t * aux = lista->next;
                free((curso_t *) lista);
                lista = aux;
            }
            break;
        case LISTA_MATERIA:
            while (lista != NULL)
            {
                BorrarEvaluaciones((materia_t *) lista);
                colegio_t * aux = lista->next;
                free((materia_t *) lista);
                lista = aux;
            }
            break;
        case LISTA_EVALUACION:
            while (lista != NULL)
            {
                BorrarNotas((evaluacion_t *) lista);
                colegio_t * aux = lista->next;
                free((evaluacion_t *) lista);
                lista = aux;
            }
            break;
        case LISTA_NOTA:
            while (lista != NULL)
            {
                colegio_t * aux = lista->next;
                free((nota_t *) lista);
                lista = aux;
            }
            break;
        case LISTA_ALUMNO:
            while (lista != NULL)
            {
                colegio_t * aux = lista->next;
                free((alumno_t *) lista);
                lista = aux;
            }
            break;
    }
}

void BorrarNotas(evaluacion_t * evaluacion)
{
    BorrarLista((colegio_t *) (evaluacion->_nota), LISTA_NOTA);
}

void BorrarEvaluaciones(materia_t * materia)
{
    BorrarLista((colegio_t *) (materia->_evaluacion), LISTA_EVALUACION);
}

void BorrarMaterias(curso_t * curso)
{
    BorrarLista((colegio_t *) (curso->_materia), LISTA_MATERIA);
}

void BorrarAlumnos(curso_t * curso)
{
    BorrarLista((colegio_t *) (curso->_alumno), LISTA_ALUMNO);
}

void BorrarCursos(colegio_t * colegio)
{
    BorrarLista((colegio_t *) (colegio->_curso), LISTA_CURSO);
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
