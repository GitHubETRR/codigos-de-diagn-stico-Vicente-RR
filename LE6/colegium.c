#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "color.h"

#define MAX_CHAR 50
#define MIN_NOTA 0.0
#define MAX_NOTA 10.0
#define MIN_PONDERACION 0.0
#define MAX_PONDERACION 1.0

#define OP_NOMBRE '0'
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

#define OP_MODIFICAR '1'

#define LISTA_COLEGIO 1
#define LISTA_CURSO 2
#define LISTA_MATERIA 3
#define LISTA_EVALUACION 4
#define LISTA_NOTA 5
#define LISTA_ALUMNO 6

//Estructuras
typedef struct alumno
{
    struct alumno * next;
    char nombre[MAX_CHAR];
    
    int id;
    float promedio;
}alumno_t;

typedef struct nota
{
    struct nota * next;
    
    int id;
    float valor;
}nota_t;

typedef struct evaluacion
{
    struct evaluacion * next;
    char nombre[MAX_CHAR];
    
    float ponderacion;
    nota_t * _nota;
}evaluacion_t;

typedef struct materia
{
    struct materia * next;
    char nombre[MAX_CHAR];
    
    evaluacion_t * _evaluacion;
}materia_t;

typedef struct curso
{
    struct curso * next;
    char nombre[MAX_CHAR];
    
    materia_t * _materia;
    alumno_t * _alumno;
}curso_t;

typedef struct colegio
{
    struct colegio * next;
    char nombre[MAX_CHAR];
    
    curso_t * _curso;
}colegio_t;

//Prototipos
void MenuColegio(colegio_t * _curso);
void MenuCurso(curso_t * curso);
void MenuMateria(materia_t * materia, alumno_t * _alumno);
void MenuEvaluacion(evaluacion_t * evaluacion, alumno_t * _alumno);
void MenuAlumno(alumno_t * alumno, materia_t * _materia);
//las listas son punteros colegio_t provisoriamente para que compile, pero luego se castea
void CambiarNombre(char nombre[MAX_CHAR]);
void * Seleccionar(colegio_t * lista, int tipo);
void * Agregar(colegio_t * lista, int tipo);
void AgregarID(alumno_t * alumno, alumno_t * lista);
int ImprimirLista(colegio_t * lista, int tipo);
int PedirUbicacion(int cantidad);

void ModificarPonderacion(evaluacion_t * evaluacion);
nota_t * AgregarNota(nota_t * _nota, alumno_t * _alumno);
int ImprimirNotas(nota_t * _nota, alumno_t * _alumno);
float PedirNota(void);

void * BorrarElemento(colegio_t * lista, int tipo);
void BorrarLista(colegio_t * lista, int tipo);
void BorrarNotas(evaluacion_t * evaluacion);
void BorrarEvaluaciones(materia_t * materia);
void BorrarMaterias(curso_t * curso);
void BorrarAlumnos(curso_t * curso);
void BorrarCursos(colegio_t * colegio);

void LimpiarTeclado();
void LimpiarPantalla();
void GuardarArchivo(colegio_t * _colegio);
int Contar(colegio_t * lista);

int main()
{
    colegio_t * _colegio = NULL;
    bool salir = false;
    
    while (!salir)
    {
        char opcion;
        bool opcionValida;
        
        printf(VERDE NEGRITA SUBRAYADO "Menú\n" RESET);
        printf(VERDE NEGRITA);
        printf("%c. Seleccionar colegio\n", OP_SELECCIONAR);
        printf("%c. Agregar colegio\n", OP_AGREGAR);
        printf("%c. Eliminar colegio\n", OP_ELIMINAR);
        printf("%c. Vaciar\n", OP_VACIAR);
        printf("%c. Salir\n", OP_SALIR);
        printf(RESET);
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
                    GuardarArchivo(_colegio);
                    BorrarLista(_colegio, LISTA_COLEGIO);
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
        } while (!opcionValida);
        LimpiarPantalla();
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
        printf(VERDE NEGRITA);
        printf("%c. Cambiar nombre\n", OP_NOMBRE);
        printf("%c. Seleccionar curso\n", OP_SELECCIONAR);
        printf("%c. Agregar curso\n", OP_AGREGAR);
        printf("%c. Eliminar curso\n", OP_ELIMINAR);
        printf("%c. Vaciar\n", OP_VACIAR);
        printf("%c. Salir\n", OP_SALIR);
        printf(RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_NOMBRE:
                    CambiarNombre(colegio->nombre);
                    break;
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
        } while (!opcionValida);
        LimpiarPantalla();
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
        printf(VERDE NEGRITA);
        printf("%c. Cambiar nombre\n", OP_NOMBRE);
        printf("%c. Seleccionar materia\n", OP_SELECCIONAR);
        printf("%c. Agregar materia\n", OP_AGREGAR);
        printf("%c. Eliminar materia\n", OP_ELIMINAR);
        printf("%c. Vaciar materias\n", OP_VACIAR);
        printf("%c. Seleccionar alumno\n", OP_SELECCIONAR_2);
        printf("%c. Agregar alumno\n", OP_AGREGAR_2);
        printf("%c. Eliminar alumno\n", OP_ELIMINAR_2);
        printf("%c. Vaciar alumnos\n", OP_VACIAR_2);
        printf("%c. Salir\n", OP_SALIR_2);
        printf(RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_NOMBRE:
                    CambiarNombre(curso->nombre);
                    break;
                case OP_SELECCIONAR:
                    materia_t * seleccionMateria = (materia_t *) Seleccionar((colegio_t *) _materia, LISTA_MATERIA);
                    if (seleccionMateria != NULL)
                    {
                        LimpiarPantalla();
                        MenuMateria(seleccionMateria, _alumno);
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
                        printf("No hay alumnos guardados");
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
        } while (!opcionValida);
        LimpiarPantalla();
    }
}

void MenuMateria(materia_t * materia, alumno_t * _alumno)
{
    bool salir = false;
    evaluacion_t * _evaluacion = materia->_evaluacion;
    
    while (!salir)
    {
        char opcion;
        bool opcionValida;
        
        printf(VERDE NEGRITA SUBRAYADO "Materia: %s\n" RESET, materia->nombre);
        printf(VERDE NEGRITA);
        printf("%c. Cambiar nombre\n", OP_NOMBRE);
        printf("%c. Seleccionar evaluación\n", OP_SELECCIONAR);
        printf("%c. Agregar evaluación\n", OP_AGREGAR);
        printf("%c. Eliminar evaluación\n", OP_ELIMINAR);
        printf("%c. Vaciar\n", OP_VACIAR);
        printf("%c. Salir\n", OP_SALIR);
        printf(RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_NOMBRE:
                    CambiarNombre(materia->nombre);
                    break;
                case OP_SELECCIONAR:
                    evaluacion_t * seleccion = (evaluacion_t *) Seleccionar((colegio_t *) _evaluacion, LISTA_EVALUACION);
                    if (seleccion != NULL)
                    {
                        LimpiarPantalla();
                        MenuEvaluacion(seleccion, _alumno);
                    }
                    else
                    {
                        printf("No hay evaluaciones guardadas");
                        getchar();
                    }
                    break;
                case OP_AGREGAR:
                    _evaluacion = (evaluacion_t *) Agregar((colegio_t *) _evaluacion, LISTA_EVALUACION);
                    break;
                case OP_ELIMINAR:
                    _evaluacion = (evaluacion_t *) BorrarElemento((colegio_t *) _evaluacion, LISTA_EVALUACION);
                    break;
                case OP_VACIAR:
                        BorrarLista((colegio_t *) _evaluacion, LISTA_EVALUACION);
                        _evaluacion = NULL;
                    break;
                case OP_SALIR:
                    materia->_evaluacion = _evaluacion;
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
        } while (!opcionValida);
        LimpiarPantalla();
    }
}

void MenuEvaluacion(evaluacion_t * evaluacion, alumno_t * _alumno)
{
    /*
    typedef struct evaluacion
{
    char nombre[MAX_CHAR];
    struct evaluacion * next;
    float ponderacion;
    nota_t * _nota;
}evaluacion_t;
    */
    bool salir = false;
    nota_t * _nota = evaluacion->_nota;
    
    while (!salir)
    {
        char opcion;
        bool opcionValida;
        
        printf(VERDE NEGRITA SUBRAYADO "Evaluacion: %s\n" RESET, evaluacion->nombre);
        printf(VERDE NEGRITA);
        printf("%c. Cambiar nombre\n", OP_NOMBRE);
        printf("%c. Modificar ponderación\n", OP_MODIFICAR);
        printf("%c. Agregar/modificar notas\n", OP_AGREGAR);
        printf("%c. Eliminar notas\n", OP_ELIMINAR);
        printf("%c. Vaciar notas\n", OP_VACIAR);
        printf("%c. Salir\n", OP_SALIR);
        printf(RESET);
        do
        {
            opcionValida = true;
            scanf("%c", &opcion);
            LimpiarTeclado();
            
            switch (opcion)
            {
                case OP_NOMBRE:
                    CambiarNombre(evaluacion->nombre);
                    break;
                case OP_MODIFICAR:
                    ModificarPonderacion(evaluacion);
                    break;
                case OP_AGREGAR:
                    _nota = AgregarNota(_nota, _alumno);
                    break;
                case OP_ELIMINAR:
                    
                    break;
                case OP_VACIAR:
                        BorrarLista((colegio_t *) _nota, LISTA_NOTA);
                        _nota = NULL;
                    break;
                case OP_SALIR:
                    evaluacion->_nota = _nota;
                    salir = true;
                    break;
                default:
                    opcionValida = false;
                    printf("Opción no válida\n");
            }
        } while (!opcionValida);
        LimpiarPantalla();
    }
}

void MenuAlumno(alumno_t * alumno, materia_t * _materia)
{
    
}

void CambiarNombre(char nombre[MAX_CHAR])
{
    printf(AZUL NEGRITA "Escribí el nuevo nombre para " RESET CELESTE NEGRITA SUBRAYADO "%s\n" RESET, nombre);
    fgets(nombre, sizeof(char[MAX_CHAR]), stdin);
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
        case LISTA_ALUMNO:
            tamanio = sizeof(alumno_t);
            break;
    }
    colegio_t * nuevo = (colegio_t *) malloc(tamanio);
    if (tipo == LISTA_ALUMNO)
        AgregarID((alumno_t *) nuevo, (alumno_t *) lista);
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

void AgregarID(alumno_t * alumno, alumno_t * lista)
{
    int id = 0;
    while (lista != NULL)
    {
        if (lista->id >= id)
        {
            id = lista->id + 1;
        }
        lista = lista->next;
    }
    alumno->id = id;
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

void ModificarPonderacion(evaluacion_t * evaluacion)
{
    float ponderacion;
    bool valorValido;
    
    do
    {
        valorValido = true;
        printf("Introducí la ponderación de " CELESTE NEGRITA SUBRAYADO "%s" RESET "en formato float (ej: 0.1 = 10%%)\n", evaluacion->nombre);
        scanf("%f", &ponderacion);
        LimpiarTeclado();
        if ((ponderacion < MIN_PONDERACION) || (ponderacion > MAX_PONDERACION))
        {
            printf(ROJO "La ponderación debe estar entre 0 y 1\n" RESET);
            valorValido = false;
        }
    } while (!valorValido);
    evaluacion->ponderacion = ponderacion;
}

nota_t * AgregarNota(nota_t * _nota, alumno_t * _alumno)
{
    nota_t * notaAux = _nota;
    int cantidad = ImprimirNotas(_nota, _alumno);
    int ubicacion = PedirUbicacion(cantidad);
    for (int i=0; i<ubicacion; i++)
    {
        _alumno = _alumno->next;
    }
    while ((notaAux != NULL) && (notaAux->id != _alumno->id))
    {
        notaAux = notaAux->next;
    }
    if (notaAux == NULL)
    {
        notaAux = _nota;
        _nota = (nota_t *) malloc(sizeof(nota_t));
        _nota->next = notaAux;
        _nota->id = _alumno->id;
    }
    _nota->valor = PedirNota();
    return _nota;
}

int ImprimirNotas(nota_t * _nota, alumno_t * _alumno)
{
    int tamanio = 0;
    while (_alumno != NULL)
    {
        printf("%i - %s", (tamanio+1), _alumno->nombre);
        nota_t * listaNotas = _nota; //Se usa esta lista auxiliar para no perder el inicio de la lista
        while (listaNotas != NULL)
        {
            if (listaNotas->id == _alumno->id)
                printf("%.2f", listaNotas->valor);
            listaNotas = listaNotas->next;
        }
        printf("\n");
        _alumno = _alumno->next;
        tamanio++;
    }
    return tamanio;
}

float PedirNota(void)
{
    bool valorValido;
    float valor;
    do
    {
        valorValido = true;
        printf("Ingresá la nueva nota (entre %f y %f)\n", MIN_NOTA, MAX_NOTA);
        scanf("%f", &valor);
        LimpiarTeclado();
        if ((valor < MIN_NOTA) || (valor > MAX_NOTA))
        {
            printf("Valor no válido\n");
            valorValido = false;
        }
    } while (!valorValido);
    return valor;
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

void GuardarArchivo(colegio_t * _colegio)
{
    FILE * fp = fopen("datos.bin", "wb");
    int cantidadColegios = Contar(_colegio);
    fwrite(&cantidadColegios, sizeof(int), 1, fp);
    while (_colegio != NULL)
    {
        fwrite(_colegio->nombre, sizeof(char), MAX_CHAR, fp);
        
        curso_t * _curso = _colegio->_curso;
        int cantidadCursos = Contar((colegio_t *) _curso);
        fwrite(&cantidadCursos, sizeof(int), 1, fp);
        while (_curso != NULL)
        {
            fwrite(_curso->nombre, sizeof(char), MAX_CHAR, fp);
            
            materia_t * _materia = _curso->_materia;
            int cantidadMaterias = Contar((colegio_t *) _materia);
            fwrite(&cantidadMaterias, sizeof(int), 1, fp);
            while (_materia != NULL)
            {
                fwrite(_materia->nombre, sizeof(char), MAX_CHAR, fp);
                
                evaluacion_t * _evaluacion = _materia->_evaluacion;
                int cantidadEvaluaciones = Contar((colegio_t *) _evaluacion);
                fwrite(&cantidadEvaluaciones, sizeof(int), 1, fp);
                while (_evaluacion != NULL)
                {
                    fwrite(_evaluacion->nombre, sizeof(char), MAX_CHAR, fp);
                    fwrite(&(_evaluacion->ponderacion), sizeof(float), 1, fp);
                    
                    nota_t * _nota = _evaluacion->_nota;
                    int cantidadNotas = Contar((colegio_t *) _nota);
                    fwrite(&cantidadNotas, sizeof(int), 1, fp);
                    while (_nota != NULL)
                    {
                        fwrite(&(_nota->id), sizeof(int), 1, fp);
                        fwrite(&(_nota->valor), sizeof(float), 1, fp);
                        _nota = _nota->next;
                    }
                    _evaluacion = _evaluacion->next;
                }
                _materia = _materia->next;
            }
            
            alumno_t * _alumno = _curso->_alumno;
            int cantidadAlumnos = Contar((colegio_t *) _alumno);
            fwrite(&cantidadAlumnos, sizeof(int), 1, fp);
            while (_alumno != NULL)
            {
                fwrite(_alumno->nombre, sizeof(char), MAX_CHAR, fp);
                fwrite(&(_alumno->id), sizeof(int), 1, fp);
                fwrite(&(_alumno->promedio), sizeof(float), 1, fp);
                _alumno = _alumno->next;
            }
            _curso = _curso->next;
        }
        _colegio = _colegio->next;
    }
}

int Contar(colegio_t * lista)
{
    int cantidad=0;
    while (lista != NULL)
    {
        cantidad++;
        lista = lista->next;
    }
    return cantidad;
}
