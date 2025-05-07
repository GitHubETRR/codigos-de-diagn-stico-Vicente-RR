#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "color.h"
#define MAX_CARACTERES_NOMBRE 50
#define MAX_CARACTERES_PRESENTACION 1000
#define MAX_CARACTERES_ESTADISTICA 17
#define CANTIDAD_ESTADISTICAS 5
#define CANTIDAD_MOVIMIENTOS 6

#define PORC_SALUD_1 60
#define PORC_SALUD_2 25
#define PORC_SALUD_3 1

#define SIN_PERSONAJE 0
#define SIN_CAMBIO -1
#define JUGADORES 2
#define MAX_PERSONAJES 6
#define MIN_AUMENTO_ESTADISTICAS -6
#define MAX_AUMENTO_ESTADISTICAS 6
#define ATAQUE 0
#define ATAQUE_ESPECIAL 1
#define DEFENSA 2
#define DEFENSA_ESPECIAL 3
#define VELOCIDAD 4

#define FORCEJEO -1
#define CAMBIO -2
#define FORCEJEO_POTENCIA 20
#define FORCEJEO_PERDIDA_SALUD 0.25
#define SIN_VICTORIA -1

typedef enum {
    JUGAR=1,
    INGRESAR,
    MOSTRAR,
    ELIMINAR,
    EDITAR,
    GUARDAR,
    SALIR
}opciones_t;

typedef struct {
    char nombre[MAX_CARACTERES_NOMBRE];

	int potencia;
	int potenciaEspecial;

	float aumentoSalud;
	int aumentoEstadisticas[CANTIDAD_ESTADISTICAS];

	int usos;
	int usosMaximos;
} movimiento_t;

typedef struct personaje {
    int id;
    char nombre[MAX_CARACTERES_NOMBRE];
	char presentacion[MAX_CARACTERES_PRESENTACION];
	
	int salud;
	int saludMaxima;
	
	int ataque;
	int ataqueEspecial;
	int defensa;
	int defensaEspecial;
	int velocidad;
	int aumentoEstadisticas[CANTIDAD_ESTADISTICAS];
	
	movimiento_t movimiento[CANTIDAD_MOVIMIENTOS];

    struct personaje * next;
} personaje_t;

personaje_t *lista_personajes=NULL;
char nombres[JUGADORES][MAX_CARACTERES_NOMBRE];

//MENÚ
void LeerDatos(void);
void Menu(void);
void Pausar(void);
void OrdenarID(void);
void PedirDatos(personaje_t * personaje_ptr);
void IngresarPersonaje(void);
void ImprimirPersonaje(personaje_t * personaje, bool mostrarDatos);
void MostrarLista(bool mostrarDatos);
personaje_t * ApuntarID(int id);
void BuscarPersonaje(bool eliminar);
void Eliminar(personaje_t * personaje);
void GuardarDatos(void);
void LiberarMemoria(void);
void LimpiarTeclado(void);
void LimpiarPantalla(void);

//JUEGO
void Jugar(void);
void Interfaz(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES]);
void ElegirNombres(void);
void ElegirPersonajes(personaje_t personajes[JUGADORES][MAX_PERSONAJES]);
void VaciarEstadisticas(personaje_t personajes[JUGADORES][MAX_PERSONAJES]);
int MenuMovimientos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int jugador, bool usosDisponibles);
void ElegirOpcion(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES], int cambioPersonaje[JUGADORES], int jugador, int opcion, bool usosDisponibles);
void ElegirMovimientos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES], int cambioPersonaje[JUGADORES]);
bool VerificarUsos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int jugador, int personajesJugando[JUGADORES]);
void CambiarPersonajes(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int cambioPersonaje[JUGADORES], int personajesJugando[JUGADORES]);
bool Turno(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES]);
int CompararVelocidades(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES]);
void Atacar(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES]);
void CalcularMultiplicadores(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], float multiplicador[CANTIDAD_ESTADISTICAS-1], int personajeTurno);
void AumentarEstadisticas(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES]);
void AumentarSalud(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES]);

bool VerificarSalud(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES]);
int VerificarVictoria(personaje_t personajes[JUGADORES][MAX_PERSONAJES]);
void ElegirCambio(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int jugador);

int main(void)
{
    LeerDatos();
    Menu();
    LiberarMemoria();
    return 0;
}

void LeerDatos(void)
{
    FILE *fp = fopen("lista.bin", "rb"); //fp=file pointer
    if (fp == NULL) {
        printf("El archivo de lista no existe, creando uno nuevo...\n");
        fp = fopen("lista.bin", "wb+");  // Crear el archivo si no existe
        if (fp == NULL)
        {
            printf("Error: No se pudo crear el archivo.\n");
            exit(1);
        }
    }

    bool salir=false;
    do{
        personaje_t * personaje_ptr= (personaje_t *)malloc(sizeof(personaje_t)); //personaje_ptr apunta a uno nuevo creado con malloc
        if(fread(personaje_ptr, sizeof(personaje_t), 1, fp)==1){ //fread guarda en la memoria de personaje_ptr los datos del personaje guardado, y en caso de que existan dichos datos...
            if(lista_personajes==NULL){ //Si es el primer personaje guardado de la lista, se indica al apuntado por personaje_ptr como el primero
                lista_personajes=personaje_ptr;
                lista_personajes->next=NULL;
            
            }
            else //Si ya existen otros personajes en la lista...
            {
                personaje_t * lista = lista_personajes; //Crea otro puntero temporal para no perder el valor guardado en el global
                while(lista->next!=NULL){ //Recorre toda la lista, hasta que el puntero lista apunte al último personaje
                    lista=lista->next;
                }
                lista->next=personaje_ptr; //Hace que el último personaje de la lista tenga al nuevo como siguiente, convirtiendo al nuevo en el último de la lista
                personaje_ptr->next=NULL; //Finalmente, el nuevo personaje tiene a NULL como siguiente, por lo que es el último
            }
        }else{ //En caso de que no existan datos de otro personaje, se libera de la memoria dinámica
            free(personaje_ptr);
            salir=true;
        }
    }while(!salir);
    fclose(fp);
    OrdenarID();
}

void Menu(void)
{
    opciones_t op;
    do
    {
        printf(VERDE NEGRITA SUBRAYADO "Menu\n" RESET);
        printf(VERDE NEGRITA "1_Jugar\n");
        printf("2_Ingresar un personaje\n");
        printf("3_Mostrar toda la lista\n");
        printf("4_Eliminar un personaje\n");
        printf("5_Editar un personaje\n");
        printf("6_Guardar los personajes\n");
        printf("7_Salir\n" RESET);
        scanf("%d", &op);
        LimpiarTeclado();
        LimpiarPantalla();
        switch (op)
        {
            case JUGAR:
                if (lista_personajes==NULL)
                {
                    printf("No hay personajes en el sistema\n");
                }
                else
                {
                    Jugar();
                }
                break;
            case INGRESAR:
                 IngresarPersonaje();
                break;
            case MOSTRAR:
                 MostrarLista(true);
                break;
            case ELIMINAR:
                BuscarPersonaje(true);
                break;
            case EDITAR:
                BuscarPersonaje(false);
                break;
            case GUARDAR:
                GuardarDatos();
                break;
        }
        Pausar();
    }while(op!=SALIR);
}

void Pausar(void)
{
    printf("Presione enter para continuar...\n");
    /*
    En caso de que se saltee algún texto, agregar estas líneas
    fflush(stdin);
    getchar();
    */
    LimpiarTeclado();
    LimpiarPantalla();
}

void OrdenarID(void)
{
    personaje_t * lista=lista_personajes;
    for (int id=1; lista!=NULL; id++)
    {
        lista->id=id;
        lista=lista->next;
    }
}

void PedirDatos(personaje_t * personaje_ptr)
{
    printf(ROJO NEGRITA SUBRAYADO "Nombre:" RESET ROJO " ");
    //scanf("%s",personaje_ptr->nombre);
    fgets(personaje_ptr->nombre, sizeof(personaje_ptr->nombre), stdin); //fgets permite incluir espacios
    personaje_ptr->nombre[strcspn(personaje_ptr->nombre, "\n")] = 0; //Evita un salto de línea innecesario
    printf(RESET NEGRITA SUBRAYADO "Presentación:" RESET " ");
    //scanf("%s",personaje_ptr->presentacion);
    fgets(personaje_ptr->presentacion, sizeof(personaje_ptr->presentacion), stdin);
    personaje_ptr->presentacion[strcspn(personaje_ptr->presentacion, "\n")] = 0;
    printf(VERDE NEGRITA "Salud máxima: " RESET VERDE);
    scanf("%i",&personaje_ptr->saludMaxima);
    printf(VERDE NEGRITA "Ataque: " RESET VERDE);
    scanf("%i",&personaje_ptr->ataque);
    printf(VERDE NEGRITA "Ataque especial: " RESET VERDE);
    scanf("%i",&personaje_ptr->ataqueEspecial);
    printf(VERDE NEGRITA "Defensa: " RESET VERDE);
    scanf("%i",&personaje_ptr->defensa);
    printf(VERDE NEGRITA "Defensa especial: " RESET VERDE);
    scanf("%i",&personaje_ptr->defensaEspecial);
    printf(VERDE NEGRITA "Velocidad: " RESET VERDE);
    scanf("%i",&personaje_ptr->velocidad);
    LimpiarTeclado();
    for (int i=0; i<CANTIDAD_MOVIMIENTOS; i++)
    {
        printf(RESET "-\n");
        printf(CELESTE NEGRITA SUBRAYADO "MOVIMIENTO %i:\n" RESET, (i+1));
        printf(CELESTE NEGRITA "Nombre: " RESET CELESTE);
        //scanf("%s",personaje_ptr->movimiento[i].nombre);
        fgets(personaje_ptr->movimiento[i].nombre, sizeof(personaje_ptr->movimiento[i].nombre), stdin);
        personaje_ptr->movimiento[i].nombre[strcspn(personaje_ptr->movimiento[i].nombre, "\n")] = 0;
        printf(CELESTE NEGRITA "Potencia: " RESET CELESTE);
        scanf("%i",&personaje_ptr->movimiento[i].potencia);
        printf(CELESTE NEGRITA "Potencia especial: " RESET CELESTE);
        scanf("%i",&personaje_ptr->movimiento[i].potenciaEspecial);
        printf(VERDE NEGRITA "Aumento de salud: " RESET VERDE);
        scanf("%f",&personaje_ptr->movimiento[i].aumentoSalud);
        printf(VERDE NEGRITA "Aumento de ataque: " RESET VERDE);
        scanf("%i",&personaje_ptr->movimiento[i].aumentoEstadisticas[ATAQUE]);
        printf(VERDE NEGRITA "Aumento de ataque especial: " RESET VERDE);
        scanf("%i",&personaje_ptr->movimiento[i].aumentoEstadisticas[ATAQUE_ESPECIAL]);
        printf(VERDE NEGRITA "Aumento de defensa: " RESET VERDE);
        scanf("%i",&personaje_ptr->movimiento[i].aumentoEstadisticas[DEFENSA]);
        printf(VERDE NEGRITA "Aumento de defensa especial: " RESET VERDE);
        scanf("%i",&personaje_ptr->movimiento[i].aumentoEstadisticas[DEFENSA_ESPECIAL]);
        printf(VERDE NEGRITA "Aumento de velocidad: " RESET VERDE);
        scanf("%i",&personaje_ptr->movimiento[i].aumentoEstadisticas[VELOCIDAD]);
        printf(RESET NEGRITA "Usos: " RESET);
        scanf("%i",&personaje_ptr->movimiento[i].usosMaximos);
        LimpiarTeclado();
    }
}

void IngresarPersonaje(void) //Considerando que el usuario conoce todas las limitaciones
{
    personaje_t * personaje_ptr = (personaje_t *)malloc(sizeof(personaje_t));
    if(personaje_ptr==NULL)
    {
        printf("Out of Memory");
        exit(1);
    }

    PedirDatos(personaje_ptr);

    printf("\n");
    //Coloca al nuevo personaje al final de la fila
    personaje_ptr->next=NULL;
    if(lista_personajes==NULL)
    {
        lista_personajes=personaje_ptr;
    }
    else
    {
        personaje_t * lista=lista_personajes;
        while (lista->next!=NULL)
            lista=lista->next;
        lista->next=personaje_ptr;
    }
    OrdenarID();
}

void ImprimirPersonaje(personaje_t * personaje, bool mostrarDatos)
{
    printf("\n--------------------\n");
    printf(NEGRITA SUBRAYADO "ID:" RESET " %i\n",personaje->id);
    printf(ROJO NEGRITA SUBRAYADO "Nombre:" RESET ROJO " %s\n" RESET,personaje->nombre);
    if (mostrarDatos)
    {
        printf(NEGRITA "Presentación:" RESET " %s\n",personaje->presentacion);
        printf(VERDE NEGRITA SUBRAYADO "\nESTADÍSTICAS:\n" RESET);
        printf(VERDE NEGRITA "Salud: " RESET "%i\n",personaje->saludMaxima);
        printf(VERDE NEGRITA "Ataque: " RESET "%i\n",personaje->ataque);
        printf(VERDE NEGRITA "Ataque Especial: " RESET "%i\n",personaje->ataqueEspecial);
        printf(VERDE NEGRITA "Defensa: " RESET "%i\n",personaje->defensa);
        printf(VERDE NEGRITA "Defensa Especial: " RESET "%i\n",personaje->defensaEspecial);
        printf(VERDE NEGRITA "Velocidad: " RESET "%i\n",personaje->velocidad);
        for (int i=0; i<CANTIDAD_MOVIMIENTOS; i++)
        {
            printf("-\n");
            printf(CELESTE NEGRITA SUBRAYADO "Movimiento %i:" RESET CELESTE " %s\n", (i+1), personaje->movimiento[i].nombre);
            printf(CELESTE NEGRITA "Potencia / Potencia Especial:" RESET " %i / %i\n", personaje->movimiento[i].potencia, personaje->movimiento[i].potenciaEspecial);
            printf(VERDE NEGRITA "Aumento de SALUD / ATAQUE / ATAQUE ESPECIAL / DEFENSA / DEFENSA ESPECIAL / VELOCIDAD:" RESET " %.2f / %i / %i / %i / %i / %i\n", personaje->movimiento[i].aumentoSalud, personaje->movimiento[i].aumentoEstadisticas[ATAQUE], personaje->movimiento[i].aumentoEstadisticas[ATAQUE_ESPECIAL], personaje->movimiento[i].aumentoEstadisticas[DEFENSA], personaje->movimiento[i].aumentoEstadisticas[DEFENSA_ESPECIAL], personaje->movimiento[i].aumentoEstadisticas[VELOCIDAD]);
            printf(NEGRITA "Usos:" RESET " %i\n", personaje->movimiento[i].usosMaximos);
        }
    }
    printf("--------------------\n");
}

void MostrarLista(bool mostrarDatos)
{
    personaje_t * lista=lista_personajes;
    while(lista!=NULL)
    {
        ImprimirPersonaje(lista, mostrarDatos);
        lista=lista->next;
    }
}

personaje_t * ApuntarID(int id)
{
    personaje_t * lista=lista_personajes;
    while ((lista!=NULL)&&(lista->id!=id))
        lista=lista->next;
    
    return lista;
}

void BuscarPersonaje(bool eliminar)
{
    int id;
    char opcion;
    personaje_t * personaje_ptr;

    printf(NEGRITA "ID: " RESET);
    scanf("%i", &id);
    LimpiarTeclado();
    personaje_ptr = ApuntarID(id);
    if (personaje_ptr==NULL)
        printf(ROJO "No se encontró el personaje\n" RESET);
    else
    {
        if (eliminar)
            printf(ROJO NEGRITA "Eliminar a ");
        else
            printf(AMARILLO NEGRITA "Editar a ");
        printf(RESET CELESTE "%s" RESET "(S/N)", personaje_ptr->nombre);
        scanf("%c", &opcion);
        LimpiarTeclado();
        if ((opcion=='S')||(opcion=='s'))
        {
            if (eliminar)
                Eliminar(personaje_ptr);
            else
                PedirDatos(personaje_ptr);
        }
    }
}

void Eliminar(personaje_t * personaje)
{
    if (personaje==lista_personajes)
        lista_personajes=lista_personajes->next;
    else
    {
        personaje_t * lista=lista_personajes;
        while (lista->next!=personaje)
        {
            lista=lista->next;
            if (lista->next==NULL)
            {
                printf("Error: No se encontró al personaje en la lista");
                exit(1);
            }
        }
        lista->next=(lista->next)->next;
    }
    printf("Personaje eliminado exitosamente\n");
    free(personaje);
    OrdenarID();
}

void GuardarDatos(void)
{
    FILE * fp= fopen ("lista.bin", "wb");
    personaje_t * lista=lista_personajes;
    while(lista!=NULL){
        fwrite((personaje_t *)lista, sizeof(personaje_t), 1, fp);
        lista=lista->next;
    }
    fclose(fp);
    printf(VERDE NEGRITA "Datos guardados\n" RESET);
}

void LiberarMemoria(void)
{
    personaje_t * lista=lista_personajes;

    while(lista!=NULL)
    {
        lista=lista_personajes->next; //lista apunta al next del que se guarda en lista_personajes, de modo que no se pierda
        free(lista_personajes);
        lista_personajes=lista;
    }
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

void Jugar(void)
{
    bool jugar=true;
    personaje_t personajes[JUGADORES][MAX_PERSONAJES];
    int movimientos[JUGADORES];
    int personajesJugando[JUGADORES];
    int cambioPersonaje[JUGADORES];
    int ganador;

    ElegirNombres();
    ElegirPersonajes(personajes);
    VaciarEstadisticas(personajes);
    do
    {
        ElegirMovimientos(personajes, personajesJugando, movimientos, cambioPersonaje);
        CambiarPersonajes(personajes, cambioPersonaje, personajesJugando);
        jugar=Turno(personajes, personajesJugando, movimientos);
    } while (jugar);
    ganador = VerificarVictoria(personajes);
    printf(VIOLETA NEGRITA "%s gana la partida\n" RESET, nombres[ganador]);
}

void Interfaz(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES])
{
    int personajesVivos;
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        personajesVivos=0;
        for (int i=0; i<MAX_PERSONAJES; i++)
        {
            if ((personajes[jugador][i].id!=SIN_PERSONAJE)&&(personajes[jugador][i].salud>0))
                personajesVivos++;
        }
        printf(VIOLETA NEGRITA F_NEGRO "%s (%i)" RESET "\n", nombres[jugador], personajesVivos);
        printf(CELESTE NEGRITA "%s " RESET, personajes[jugador][personajesJugando[jugador]].nombre);
        if (((100*personajes[jugador][personajesJugando[jugador]].salud) / personajes[jugador][personajesJugando[jugador]].saludMaxima)>PORC_SALUD_1)
            printf(VERDE NEGRITA "%i%\n" RESET, (100*personajes[jugador][personajesJugando[jugador]].salud / personajes[jugador][personajesJugando[jugador]].saludMaxima)); //Verde
        else
        {
            if (((100*personajes[jugador][personajesJugando[jugador]].salud) / personajes[jugador][personajesJugando[jugador]].saludMaxima)>PORC_SALUD_2)
                printf(AMARILLO NEGRITA "%i%\n" RESET, (100*personajes[jugador][personajesJugando[jugador]].salud / personajes[jugador][personajesJugando[jugador]].saludMaxima)); //Amarillo
            else
            {
                if (((100*personajes[jugador][personajesJugando[jugador]].salud) / personajes[jugador][personajesJugando[jugador]].saludMaxima)>PORC_SALUD_3)
                    printf(ROJO NEGRITA "%i%\n" RESET, (100*personajes[jugador][personajesJugando[jugador]].salud / personajes[jugador][personajesJugando[jugador]].saludMaxima)); //Rojo
                else
                    printf(F_ROJO NEGRITA "1%" RESET "\n");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void ElegirNombres(void)
{
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        printf(VIOLETA NEGRITA "Jugador %i" RESET ", introducí tu nombre (máximo %i caracteres)\n" VIOLETA, jugador, MAX_CARACTERES_NOMBRE);
        fgets(nombres[jugador], sizeof(nombres[jugador]), stdin);
        nombres[jugador][strcspn(nombres[jugador], "\n")] = 0;
        //scanf("%s", nombres[jugador]);
        LimpiarTeclado();
        LimpiarPantalla();
        printf(RESET);
    }
}

void ElegirPersonajes(personaje_t personajes[JUGADORES][MAX_PERSONAJES])
{
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        MostrarLista(false);
        for (int personaje=0; personaje<MAX_PERSONAJES; personaje++)
        {
            int id;
            bool respuestaValida;
            do
            {
                respuestaValida = true;
                printf(VIOLETA NEGRITA "%s" RESET ", introducí el ID del %i° personaje\n", nombres[jugador], (personaje+1));
                if (personaje!=0) //Si no es el primer personaje, porque el equipo no puede estar vacío
                    printf(NEGRITA "Enviá %i para terminar el equipo\n" RESET, SIN_PERSONAJE);
                scanf("%i", &id);
                if ((id==SIN_PERSONAJE)&&(personaje!=0))
                {
                    while (personaje<MAX_PERSONAJES)
                    {
                        personajes[jugador][personaje].id=SIN_PERSONAJE;
                        personaje++;
                    }
                }
                else
                {
                    if (ApuntarID(id)!=NULL)
                        personajes[jugador][personaje] = *ApuntarID(id);
                    else
                    {
                        respuestaValida=false;
                        printf(ROJO "No se encontró el personaje\n" RESET);
                    }
                }
            } while (!respuestaValida);
        }
        LimpiarPantalla();
    }
}

void VaciarEstadisticas(personaje_t personajes[JUGADORES][MAX_PERSONAJES])
{
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        for (int personaje=0; personaje<MAX_PERSONAJES; personaje++)
        {
            personajes[jugador][personaje].salud = personajes[jugador][personaje].saludMaxima;
            for (int estadistica=0; estadistica<CANTIDAD_ESTADISTICAS; estadistica++)
            {
                personajes[jugador][personaje].aumentoEstadisticas[estadistica] = 0;
            }
            for (int movimiento=0; movimiento<CANTIDAD_MOVIMIENTOS; movimiento++)
            {
                personajes[jugador][personaje].movimiento[movimiento].usos = personajes[jugador][personaje].movimiento[movimiento].usosMaximos;
            }
        }
    }
}

int MenuMovimientos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int jugador, bool usosDisponibles)
{
    int opcion=1;

    Interfaz(personajes, personajesJugando);
    printf(VIOLETA NEGRITA "%s\n" RESET, nombres[jugador]);
    for (int movimiento=0; movimiento<CANTIDAD_MOVIMIENTOS; movimiento++)
    {
        if (personajes[jugador][personajesJugando[jugador]].movimiento[movimiento].usos>0)
            printf(CELESTE NEGRITA "%i_%s - %i/%i\n" RESET, opcion, personajes[jugador][personajesJugando[jugador]].movimiento[movimiento].nombre, personajes[jugador][personajesJugando[jugador]].movimiento[movimiento].usos, personajes[jugador][personajesJugando[jugador]].movimiento[movimiento].usosMaximos);
        else
            printf(NEGRO NEGRITA "%s - 0/0\n" RESET, personajes[jugador][personajesJugando[jugador]].movimiento[movimiento].nombre);
        opcion++;
    }
    for (int personaje=0; personaje<MAX_PERSONAJES; personaje++)
    {
        if (personajes[jugador][personaje].id!=SIN_PERSONAJE)
        {
            if (personaje!=personajesJugando[jugador])
            {
                if (personajes[jugador][personaje].salud>0)
                    printf(AMARILLO NEGRITA "%i_%s - %i%\n" RESET, opcion, personajes[jugador][personaje].nombre, (100*personajes[jugador][personaje].salud/personajes[jugador][personaje].saludMaxima));
                else
                    printf(NEGRO NEGRITA "%s - 0%\n" RESET, personajes[jugador][personaje].nombre);
            }
            opcion++;
        }
    }
    if (!usosDisponibles)
        printf("%i_Forcejeo\n", opcion);
    return opcion;
}

void ElegirOpcion(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES], int cambioPersonaje[JUGADORES], int jugador, int opcion, bool usosDisponibles)
{
    int eleccion;
    bool opcionValida=false;

    do
    {
        scanf("%i", &eleccion);
        if ((eleccion==opcion)&&(!usosDisponibles))
        {
            movimientos[jugador]=FORCEJEO;
            opcionValida=true;
        }
        else
        {
            if ((eleccion>0)&&(eleccion<opcion)&&((eleccion-CANTIDAD_MOVIMIENTOS-1)!=personajesJugando[jugador])) //Se encuentra en el rango de opciones
            {
                if (eleccion<=CANTIDAD_MOVIMIENTOS) //se eligió un movimiento
                {
                    if (personajes[jugador][personajesJugando[jugador]].movimiento[(eleccion-1)].usos > 0)
                    {
                        movimientos[jugador]=eleccion-1;
                        personajes[jugador][personajesJugando[jugador]].movimiento[movimientos[jugador]].usos--;
                        opcionValida=true;
                    }
                    else
                        printf(ROJO "No te quedan usos para este movimiento\n" RESET);
                }
                else //se eligió un personaje
                {
                    if (personajes[jugador][eleccion-CANTIDAD_MOVIMIENTOS-1].salud>0)
                    {
                        cambioPersonaje[jugador]=eleccion-CANTIDAD_MOVIMIENTOS-1;
                        movimientos[jugador]=CAMBIO;
                        opcionValida=true;
                    }
                    else
                        printf(ROJO "%s está debilitado\n" RESET, personajes[jugador][eleccion-CANTIDAD_MOVIMIENTOS-1].nombre);
                }
            }
            else
                printf(ROJO "Introducí una opción válida\n" RESET);
        }
    } while (!opcionValida);
    LimpiarTeclado();
}

void ElegirMovimientos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES], int cambioPersonaje[JUGADORES])
{
    int opcion;
    bool usosDisponibles; //Si quedan usos disponibles para cualquier movimiento

    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        cambioPersonaje[jugador] = SIN_CAMBIO;
        usosDisponibles = VerificarUsos(personajes, jugador, personajesJugando);
        opcion = MenuMovimientos(personajes, personajesJugando, jugador, usosDisponibles);

        ElegirOpcion(personajes, personajesJugando, movimientos, cambioPersonaje, jugador, opcion, usosDisponibles);
        
        LimpiarPantalla();
    }
}

bool VerificarUsos(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int jugador, int personajesJugando[JUGADORES])
{
    bool usosDisponibles=false;
    for (int i=0; i<CANTIDAD_MOVIMIENTOS; i++)
    {
        if (personajes[jugador][personajesJugando[jugador]].movimiento[i].usos>0)
            usosDisponibles=true;
    }
    
    return usosDisponibles;
}

void CambiarPersonajes(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int cambioPersonaje[JUGADORES], int personajesJugando[JUGADORES])
{
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        if (cambioPersonaje[jugador]!=SIN_CAMBIO)
        {
            printf(VIOLETA NEGRITA "%s" RESET VERDE " cambió a " CELESTE NEGRITA "%s" RESET VERDE " por " CELESTE NEGRITA "%s\n" RESET, nombres[jugador], personajes[jugador][personajesJugando[jugador]].nombre, personajes[jugador][cambioPersonaje[jugador]].nombre);
            personajesJugando[jugador] = cambioPersonaje[jugador];
        }
    }
}

bool Turno(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES]) //Se almacena el nuevo estado de ambos personajes en un vector temporalmente para poder devolver ambos
{
    int personajeTurno=CompararVelocidades(personajes, personajesJugando);
    bool jugar;
    
    for (int t=0; t<JUGADORES; t++)
    {
        if (movimientos[personajeTurno]!=CAMBIO)
        {
            Atacar(personajes, personajesJugando, personajeTurno, movimientos);
            if (movimientos[personajeTurno]==FORCEJEO)
            {
                personajes[personajeTurno][personajesJugando[personajeTurno]].salud -= personajes[personajeTurno][personajesJugando[personajeTurno]].saludMaxima * FORCEJEO_PERDIDA_SALUD;
                if (personajes[personajeTurno][personajesJugando[personajeTurno]].salud<0)
                    personajes[personajeTurno][personajesJugando[personajeTurno]].salud=0;
            }
            else
            {
                AumentarEstadisticas(personajes, personajesJugando, personajeTurno, movimientos);
                AumentarSalud(personajes, personajesJugando, personajeTurno, movimientos);
            }
        }
        jugar=VerificarSalud(personajes, personajesJugando, movimientos);
        personajeTurno = !personajeTurno;
        if (!jugar)
            t=JUGADORES;
    }
    Pausar();
    return jugar;
}

int CompararVelocidades(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES])
{
    float velocidad[JUGADORES];
    int personajeTurno;
    
    for (int personaje=0; personaje<JUGADORES; personaje++)
    {
        if (personajes[personaje][personajesJugando[personaje]].aumentoEstadisticas[VELOCIDAD]<0)
            velocidad[personaje]=2.0/(2.0-(float)personajes[personaje][personajesJugando[personaje]].aumentoEstadisticas[VELOCIDAD]);
        else
            velocidad[personaje]=(2.0+(float)personajes[personaje][personajesJugando[personaje]].aumentoEstadisticas[VELOCIDAD])/2.0;
        
        velocidad[personaje]*=personajes[personaje][personajesJugando[personaje]].velocidad;
    }
    if (velocidad[0]==velocidad[1]) personajeTurno = rand()%JUGADORES;
    else
        personajeTurno=(velocidad[1]>velocidad[0]);
    
    return personajeTurno;
}

void Atacar(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES])
{
    float multiplicador[CANTIDAD_ESTADISTICAS-1]; //Menos la velocidad, que ya fue calculada
    int personajeRival = !personajeTurno;
    int dano = 0; //daño
    
    CalcularMultiplicadores(personajes, personajesJugando, multiplicador, personajeTurno);
    if (movimientos[personajeTurno]==FORCEJEO)
    {
        printf(CELESTE NEGRITA "%s" RESET " usó " CELESTE DESVANECIDO "Forcejeo\n" RESET, personajes[personajeTurno][personajesJugando[personajeTurno]].nombre);
        dano += multiplicador[ATAQUE] / multiplicador[DEFENSA] * FORCEJEO_POTENCIA * personajes[personajeTurno][personajesJugando[personajeTurno]].ataque / personajes[personajeRival][personajesJugando[personajeRival]].defensa;
        dano += multiplicador[ATAQUE_ESPECIAL] / multiplicador[DEFENSA_ESPECIAL] * FORCEJEO_POTENCIA * personajes[personajeTurno][personajesJugando[personajeTurno]].ataqueEspecial / personajes[personajeRival][personajesJugando[personajeRival]].defensaEspecial;
    }
    else
    {
        printf(CELESTE NEGRITA "%s" RESET " usó " CELESTE "%s\n" RESET, personajes[personajeTurno][personajesJugando[personajeTurno]].nombre, personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].nombre);
        dano += multiplicador[ATAQUE] / multiplicador[DEFENSA] * personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].potencia * personajes[personajeTurno][personajesJugando[personajeTurno]].ataque / personajes[personajeRival][personajesJugando[personajeRival]].defensa;
        dano += multiplicador[ATAQUE_ESPECIAL] / multiplicador[DEFENSA_ESPECIAL] * personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].potenciaEspecial * personajes[personajeTurno][personajesJugando[personajeTurno]].ataqueEspecial / personajes[personajeRival][personajesJugando[personajeRival]].defensaEspecial;
    }
    personajes[personajeRival][personajesJugando[personajeRival]].salud -= dano;
    if (dano>0)
    {
        int porcentajeDano = (100*dano)/personajes[personajeRival][personajesJugando[personajeRival]].saludMaxima;
        printf(CELESTE NEGRITA "%s" RESET " perdió " ROJO "%i%" RESET " de salud\n", personajes[personajeRival][personajesJugando[personajeRival]].nombre, porcentajeDano);
    }
    if (personajes[personajeRival][personajesJugando[personajeRival]].salud<0)
        personajes[personajeRival][personajesJugando[personajeRival]].salud=0;
}

void CalcularMultiplicadores(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], float multiplicador[CANTIDAD_ESTADISTICAS-1], int personajeTurno)
{
    int personajeRival = !personajeTurno;
    
    multiplicador[ATAQUE]=(float) personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[ATAQUE];
    multiplicador[ATAQUE_ESPECIAL]=(float) personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[ATAQUE_ESPECIAL];
    multiplicador[DEFENSA]=(float) personajes[personajeRival][personajesJugando[personajeRival]].aumentoEstadisticas[DEFENSA];
    multiplicador[DEFENSA_ESPECIAL]=(float) personajes[personajeRival][personajesJugando[personajeRival]].aumentoEstadisticas[DEFENSA_ESPECIAL];
    for (int i=0; i<(CANTIDAD_ESTADISTICAS-1); i++)
    {
        if (multiplicador[i]<0)
            multiplicador[i]=2.0/(2.0-multiplicador[i]);
        else
            multiplicador[i]=(2.0+multiplicador[i])/2.0;
    }
}

void AumentarEstadisticas(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES])
{
    char nombreEstadistica[CANTIDAD_ESTADISTICAS][MAX_CARACTERES_ESTADISTICA] = {"ATAQUE", "ATAQUE ESPECIAL", "DEFENSA", "DEFENSA ESPECIAL", "VELOCIDAD"};
    for (int i=0; i<CANTIDAD_ESTADISTICAS; i++)
    {
        if ((personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].aumentoEstadisticas[i]>0) && (personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]<MAX_AUMENTO_ESTADISTICAS))
            printf(VERDE NEGRITA "%s" RESET " de " CELESTE NEGRITA "%s" RESET " aumentó\n", nombreEstadistica[i], personajes[personajeTurno][personajesJugando[personajeTurno]].nombre);
        
        if ((personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].aumentoEstadisticas[i]<0) && (personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]>MAX_AUMENTO_ESTADISTICAS))
            printf(VERDE NEGRITA "%s" RESET " de " CELESTE NEGRITA "%s" RESET " disminuyó\n", nombreEstadistica[i], personajes[personajeTurno][personajesJugando[personajeTurno]].nombre);

        personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i] += personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].aumentoEstadisticas[i];
        
        if (personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]<MIN_AUMENTO_ESTADISTICAS)
            personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]=MIN_AUMENTO_ESTADISTICAS;
        if (personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]>MAX_AUMENTO_ESTADISTICAS)
            personajes[personajeTurno][personajesJugando[personajeTurno]].aumentoEstadisticas[i]=MAX_AUMENTO_ESTADISTICAS;
    }
}

void AumentarSalud(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int personajeTurno, int movimientos[JUGADORES])
{
    personajes[personajeTurno][personajesJugando[personajeTurno]].salud+=personajes[personajeTurno][personajesJugando[personajeTurno]].saludMaxima * personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].aumentoSalud;
    if (personajes[personajeTurno][personajesJugando[personajeTurno]].salud > personajes[personajeTurno][personajesJugando[personajeTurno]].saludMaxima)
        personajes[personajeTurno][personajesJugando[personajeTurno]].salud = personajes[personajeTurno][personajesJugando[personajeTurno]].saludMaxima;

    int porcentajeSalud = (int)(100*personajes[personajeTurno][personajesJugando[personajeTurno]].movimiento[movimientos[personajeTurno]].aumentoSalud);
    if (porcentajeSalud>0)
        printf("La salud de " CELESTE NEGRITA "%s" RESET " aumentó un " VERDE NEGRITA "%i%\n" RESET, personajes[personajeTurno][personajesJugando[personajeTurno]].nombre, porcentajeSalud);
    if (porcentajeSalud<0)
        printf("La salud de " CELESTE NEGRITA "%s" RESET " disminuyó un " ROJO NEGRITA "%i%\n" RESET, personajes[personajeTurno][personajesJugando[personajeTurno]].nombre, (-porcentajeSalud));
}

bool VerificarSalud(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int movimientos[JUGADORES])
{
    int victoria=VerificarVictoria(personajes);
    bool jugar=true;
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        if (personajes[jugador][personajesJugando[jugador]].salud==0)
        {
            printf(ROJO NEGRITA "%s" RESET ROJO " se debilitó\n" RESET, personajes[jugador][personajesJugando[jugador]].nombre);
            Pausar();
            if (victoria==SIN_VICTORIA)
            {
                ElegirCambio(personajes, personajesJugando, jugador);
                movimientos[jugador]=CAMBIO;
            }
            else
            {
                jugar=false;
                jugador=JUGADORES; //Para salir del for
            }
        }
    }
    return jugar;
}

int VerificarVictoria(personaje_t personajes[JUGADORES][MAX_PERSONAJES])
{
    int victoria=SIN_VICTORIA;
    for (int jugador=0; jugador<JUGADORES; jugador++)
    {
        if (victoria==SIN_VICTORIA)
        {
            victoria=!jugador;
            for (int i=0; i<MAX_PERSONAJES; i++)
            {
                if ((personajes[jugador][i].id!=SIN_PERSONAJE)&&(personajes[jugador][i].salud>0))
                    victoria=SIN_VICTORIA;
            }
        }
    }
    return victoria;
}

void ElegirCambio(personaje_t personajes[JUGADORES][MAX_PERSONAJES], int personajesJugando[JUGADORES], int jugador)
{
    int eleccion;
    bool eleccionValida;

    printf(VIOLETA NEGRITA "%s" RESET ", elegí un personaje para reemplazarlo\n", nombres[jugador]);
    for (int i=0; i<MAX_PERSONAJES; i++)
    {
        if ((personajes[jugador][i].id!=SIN_PERSONAJE)&&(personajes[jugador][i].salud>0))
        {
            printf(AMARILLO NEGRITA "%i_%s - %i/%i%\n" RESET, (i+1), personajes[jugador][i].nombre, personajes[jugador][i].salud, personajes[jugador][i].saludMaxima);
        }
    }
    do
    {
        eleccionValida=false;
        scanf("%i", &eleccion);
        eleccion--; //Para que el primero sea 0 en lugar de 1
        if ((eleccion>=0)&&(eleccion<MAX_PERSONAJES)&&(personajes[jugador][eleccion].salud>0)&&(personajes[jugador][eleccion].id!=SIN_PERSONAJE))
            eleccionValida=true;
        else
            printf(ROJO "Ingresá una opción válida\n" RESET);
    } while (!eleccionValida);
    personajesJugando[jugador]=eleccion;
    LimpiarPantalla();
}
