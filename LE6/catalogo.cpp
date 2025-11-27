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

#define SI 'S'
#define NO 'N'

//Menú
#define MENU_VER 1
#define MENU_AGREGAR 2
#define MENU_ELIMINAR 3
#define MENU_SALIR 4

class Producto
{
    protected: //protected permite que las clases hijas accedan
        string nombre;
        int codigo;
        float precio;
    public:
        Producto * next = NULL;
        virtual char Tipo() {return SIN_TIPO;} //virtual indica que el método puede ser redefinido por las clases hijas
        virtual void NuevosDatos();
        virtual void Imprimir(ostream& salida);
        virtual void GuardarArchivo(ofstream& archivo);
        virtual void LeerArchivo(ifstream& archivo);
};

class Comida : public Producto
{
    private:
        bool sinTacc;
        bool vegano;
    public:
        char Tipo() override {return COMIDA;} //override indica que este método está sobrescribiendo uno virtual de la clase madre
        void NuevosDatos() override;
        void Imprimir(ostream& salida) override;
        void GuardarArchivo(ofstream& archivo) override;
        void LeerArchivo(ifstream& archivo) override;
};

class Bebida : public Producto
{
    protected:
        int ml;
    public:
        virtual char Tipo() override {return BEBIDA;}
        virtual void NuevosDatos() override;
        virtual void Imprimir(ostream& salida) override;
        virtual void GuardarArchivo(ofstream& archivo) override;
        virtual void LeerArchivo(ifstream& archivo) override;
};

class BebidaAlcoholica : public Bebida
{
    private:
        float graduacion;
    public:
        char Tipo() override {return BEBIDA_ALCOHOLICA;}
        void NuevosDatos() override;
        void Imprimir(ostream& salida) override;
        void GuardarArchivo(ofstream& archivo) override;
        void LeerArchivo(ifstream& archivo) override;
};

//Prototipos
void GuardarArchivo(Producto * lista);
Producto * LeerArchivo();
void ImprimirLista(ostream& salida, Producto * lista);
Producto * Agregar(Producto * lista);
char PedirTipo();
void Eliminar();
void LimpiarBuffer();

int main()
{
    Producto * lista = LeerArchivo();
    bool salir = false;
    while (!salir)
    {
        bool opcionValida = true;
        cout << MENU_VER << ". Ver lista" << endl;
        cout << MENU_AGREGAR << ". Agregar" << endl;
        cout << MENU_ELIMINAR << ". Eliminar" << endl;
        cout << MENU_SALIR << ". Salir" << endl;
        do
        {
            int opcion;
            cin >> opcion;
            LimpiarBuffer();
            switch (opcion)
            {
                case MENU_VER:
                    ImprimirLista(cout, lista);
                    break;
                case MENU_AGREGAR:
                    lista = Agregar(lista);
                    break;
                case MENU_ELIMINAR:
                    Eliminar();
                    break;
                case MENU_SALIR:
                    salir = true;
                    if (lista != NULL)
                        GuardarArchivo(lista);
                    break;
                default:
                    opcionValida = false;
            }
            if (!opcionValida)
                cout << "Opción no válida" << endl;
        } while (!opcionValida);
    }
    return 0;
}

void GuardarArchivo(Producto * lista)
{
    ofstream archivoTXT("catalogo.txt");
    if (archivoTXT.is_open())
    {
        ImprimirLista(archivoTXT, lista);
        archivoTXT.close();
    }
    else
        cout << "Error al guardar el archivo de texto" << endl;
    
    ofstream archivo("catalogo.bin");
    if (archivo.is_open())
    {
        while (lista != NULL)
        {
            char tipo = lista->Tipo();
            archivo.write(&tipo, sizeof(tipo)); //char*, size_t
            lista->GuardarArchivo(archivo);
            lista = lista->next;
        }
        archivo.close();
    }
    else
        cout << "Error al guardar el archivo binario" << endl;
}

Producto * LeerArchivo()
{
    Producto * lista = NULL;
    ifstream archivo("catalogo.bin");
    if (archivo.is_open())
    {
        char tipo;
        Producto * nuevoElemento;
        while (archivo.read(&tipo, sizeof(tipo))) //char*, size_t
        {
            switch (tipo)
            {
                case SIN_TIPO:
                    nuevoElemento = new Producto;
                    break;
                case COMIDA:
                    nuevoElemento = new Comida;
                    break;
                case BEBIDA:
                    nuevoElemento = new Bebida;
                    break;
                case BEBIDA_ALCOHOLICA:
                    nuevoElemento = new BebidaAlcoholica;
                    break;
            }
            nuevoElemento->next = lista;
            nuevoElemento->LeerArchivo(archivo);
            
            lista = nuevoElemento;
        }
        archivo.close();
    }
    return lista;
}

void ImprimirLista(ostream& salida, Producto * lista)
{
    if (lista != NULL)
    {
        salida << "CATÁLOGO" << endl;
        salida << "-" << endl;
        while (lista != NULL)
        {
            lista->Imprimir(salida);
            salida << "-" << endl;
            lista = lista->next;
        }
        
    }
    else
        cout << "No hay elementos" << endl;
}

Producto * Agregar(Producto * lista)
{
    char tipo = PedirTipo();
    Producto * nuevoElemento;
    switch (tipo)
    {
        case SIN_TIPO:
            nuevoElemento = new Producto;
            break;
        case COMIDA:
            nuevoElemento = new Comida;
            break;
        case BEBIDA:
            nuevoElemento = new Bebida;
            break;
        case BEBIDA_ALCOHOLICA:
            nuevoElemento = new BebidaAlcoholica;
            break;
    }
    nuevoElemento->NuevosDatos();
    nuevoElemento->next = lista;
    lista = nuevoElemento;
    return lista;
}

char PedirTipo()
{
    cout << "Envíe un caracter según el tipo de producto" << endl;
    cout << "Genérico:" << SIN_TIPO << endl;
    cout << "Comida:" << COMIDA << endl;
    cout << "Bebida:" << BEBIDA << endl;
    cout << "Bebida alcohólica:" << BEBIDA_ALCOHOLICA << endl;
    char tipo;
    bool tipoValido;
    do
    {
        tipoValido = true;
        cin >> tipo;
        LimpiarBuffer();
        if ((tipo != SIN_TIPO) && (tipo != COMIDA) && (tipo != BEBIDA) && (tipo != BEBIDA_ALCOHOLICA))
        {
            tipoValido = false;
            cout << "Opción no válida" << endl;
        }
    } while (!tipoValido);
    return tipo;
}

void Eliminar()
{
    
}

void Producto::NuevosDatos()
{
    if (next != NULL)
        codigo = (next->codigo)+1;
    else
        codigo = 0;
    cout << "Introduzca el nombre: ";
    getline(cin, nombre);
    LimpiarBuffer();
    cout << "Introduzca el precio: ";
    cin >> precio;
    LimpiarBuffer();
}

bool SiONo()
{
    bool siONo;
    bool opcionValida;
    cout << "SI:" << SI << " / NO:" << NO << " :";
    do
    {
        char opcion;
        opcionValida = true;
        cin >> opcion;
        LimpiarBuffer();
        if ((opcion == SI) || (opcion == NO))
            siONo = (opcion==SI)?true:false;
        else
        {
            opcionValida = false;
            cout << "Opción no válida" << endl;
        }
    } while (!opcionValida);
    return siONo;
}

void Comida::NuevosDatos()
{
    Producto::NuevosDatos();
    cout << "¿Está libre de TACC? ";
    sinTacc = SiONo();
    cout << "¿Es apto para veganos? ";
    vegano = SiONo();
}

void Bebida::NuevosDatos()
{
    Producto::NuevosDatos();
    cout << "Introduzca el tamaño (ml): ";
    cin >> ml;
    LimpiarBuffer();
}

void BebidaAlcoholica::NuevosDatos()
{
    Bebida::NuevosDatos();
    cout << "Introduzca la graduación alcohólica: ";
    cin >> graduacion;
    LimpiarBuffer();
}

void Producto::Imprimir(ostream& salida)
{
    salida << nombre << endl;
    salida << "$" << precio << endl;
    salida << "Código: " << codigo << endl;
}

void Comida::Imprimir(ostream& salida)
{
    Producto::Imprimir(salida);
    salida << "Comida" << endl;
    salida << (sinTacc?"Es":"No es") << " apto para celíacos" << endl;
    salida << (vegano?"Es":"No es") << " apto para veganos" << endl;
}

void Bebida::Imprimir(ostream& salida)
{
    Producto::Imprimir(salida);
    salida << "Bebida" << endl;
    salida << "Tamaño: " << ml << "ml" << endl;
}

void BebidaAlcoholica::Imprimir(ostream& salida)
{
    Bebida::Imprimir(salida);
    salida << "Esta bebida tiene alcohol, prohibida para menores de " << MAYORIA_EDAD << "años" << endl;
    salida << "Graduación alcohólica:" << graduacion << endl;
}

void Producto::GuardarArchivo(ofstream& archivo)
{
    size_t caracteresNombre = nombre.size();
    archivo.write((char*)&caracteresNombre, sizeof(caracteresNombre));
    archivo.write(nombre.data(), caracteresNombre);
    
    archivo.write((char*)(&codigo), sizeof(codigo));
    archivo.write((char*)(&precio), sizeof(precio));
}

void Comida::GuardarArchivo(ofstream& archivo)
{
    Producto::GuardarArchivo(archivo);
    archivo.write((char*)(&sinTacc), sizeof(sinTacc));
    archivo.write((char*)(&vegano), sizeof(vegano));
}

void Bebida::GuardarArchivo(ofstream& archivo)
{
    Producto::GuardarArchivo(archivo);
    archivo.write((char*)(&ml), sizeof(ml));
}

void BebidaAlcoholica::GuardarArchivo(ofstream& archivo)
{
    Bebida::GuardarArchivo(archivo);
    archivo.write((char*)(&graduacion), sizeof(graduacion));
}

void Producto::LeerArchivo(ifstream& archivo)
{
    size_t caracteresNombre;
    archivo.read((char*)(&caracteresNombre), sizeof(caracteresNombre));
    
    nombre.resize(caracteresNombre);
    archivo.read(&nombre[0], caracteresNombre);
    
    archivo.read((char*)(&codigo), sizeof(codigo));
    archivo.read((char*)(&precio), sizeof(precio));
}


void Comida::LeerArchivo(ifstream& archivo)
{
    Producto::LeerArchivo(archivo);
    archivo.read((char*)(&sinTacc), sizeof(sinTacc));
    archivo.read((char*)(&vegano), sizeof(vegano));
}

void Bebida::LeerArchivo(ifstream& archivo)
{
    Producto::LeerArchivo(archivo);
    archivo.read((char*)(&ml), sizeof(ml));
}

void BebidaAlcoholica::LeerArchivo(ifstream& archivo)
{
    Bebida::LeerArchivo(archivo);
    archivo.read((char*)(&graduacion), sizeof(graduacion));
}

void LimpiarBuffer()
{
    while (getchar() != '\n' && !feof(stdin));
}
