#include <iostream>
#include <fstream>
using namespace std;

#define MAX_CHAR 30
#define MAYORIA_EDAD 18

//Tipos
#define SIN_TIPO 'S'
#define COMIDA 'C'
#define BEBIDA 'B'
#define BEBIDA_ALCOHOLICA 'A'

//Menú
#define MENU_AGREGAR 1
#define MENU_ELIMINAR 2
#define MENU_SALIR 3

//Prototipos
void Agregar();
void Eliminar();

class Producto
{
    protected: //protected permite que las clases hijas accedan
        char nombre[MAX_CHAR];
        int codigo;
        float precio;
    public:
        virtual char Tipo() {return SIN_TIPO;} //virtual indica que el método puede ser redefinido por las clases hijas
        virtual void Mostrar();
};

class Comida : public Producto
{
    private:
        bool sinTacc;
        bool vegano;
    public:
        char Tipo() override {return COMIDA;} //override indica que este método está sobrescribiendo uno virtual de la clase madre
        void Mostrar() override;
};

class Bebida : public Producto
{
    protected:
        int ml;
    public:
        virtual char Tipo() override {return BEBIDA;}
        virtual void Mostrar() override;
};

class BebidaAlcoholica : public Bebida
{
    private:
        float graduacion;
    public:
        char Tipo() override {return BEBIDA_ALCOHOLICA;}
        void Mostrar() override;
};

struct Nodo
{
    Producto * producto = NULL;
    Producto * next = NULL;
}

int main()
{
    Nodo lista = LeerArchivo();
    bool salir = false;
    while (!salir)
    {
        bool opcionValida = true;
        cout << MENU_AGREGAR << ". Agregar" << endl;
        cout << MENU_ELIMINAR << ". Eliminar" << endl;
        cout << MENU_SALIR << ". Salir" << endl;
        do
        {
            int opcion;
            cin >> opcion;
            switch (opcion)
            {
                case MENU_AGREGAR:
                    Agregar();
                    break;
                case MENU_ELIMINAR:
                    Eliminar();
                    break;
                case MENU_SALIR:
                    salir = true;
                    break;
                default:
                    opcionValida = false;
            }
            if (!opcionValida)
                cout << "Opción no válida" << endl;
        } while (!opcionValida);
    }
    Producto a;
    a.Mostrar();
    cout<<a.Tipo();
    Comida b;
    b.Mostrar();
    cout<<b.Tipo();
    Bebida c;
    c.Mostrar();
    cout<<c.Tipo();
    BebidaAlcoholica d;
    d.Mostrar();
    cout<<d.Tipo();
    return 0;
}

Nodo LeerArchivo()
{
    
}

void Agregar()
{
    
}

void Eliminar()
{
    
}

void Producto::Mostrar()
{
    cout << nombre << endl;
    cout << "$" << precio << endl;
    cout << "Código: " << codigo << endl;
}

void Comida::Mostrar()
{
    Producto::Mostrar();
    cout << "Comida" << endl;
    cout << (sinTacc?"Es":"No es") << " apto para celíacos" << endl;
    cout << (vegano?"Es":"No es") << " apto para veganos" << endl;
}

void Bebida::Mostrar()
{
    Producto::Mostrar();
    cout << "Bebida" << endl;
    cout << "Tamaño: " << ml << "ml" << endl;
}

void BebidaAlcoholica::Mostrar()
{
    Bebida::Mostrar();
    cout << "Esta bebida tiene alcohol, prohibida para menores de " << MAYORIA_EDAD << "años" << endl;
    cout << "Graduación alcohólica:" << graduacion << endl;
}
