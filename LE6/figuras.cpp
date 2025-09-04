#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

#define PRECISION 2
#define MIN_OPCION 1
#define CARGAR_CIRCULO 1
#define CARGAR_RECTANGULO 2
#define CARGAR_CUADRADO 3
#define VER_CIRCULO 4
#define VER_RECTANGULO 5
#define VER_CUADRADO 6
#define SALIR 7

#define RADIO_DEF 5
#define LADO1_DEF 3
#define LADO2_DEF 7
#define LADO_DEF 2

class figura
{
    private:
        float area;
        float perimetro;
    public:
        void SetArea(float a)
        {
            area = a;
        }
        void SetPerimetro(float p)
        {
            perimetro = p;
        }
        float GetArea()
        {
            return area;
        }
        float GetPerimetro()
        {
            return perimetro;
        }
};

class circulo : public figura
{
    private:
        float radio;
    public:
        void SetRadio(float r)
        {
            radio = r;
            SetArea(M_PI * r * r);
            SetPerimetro(M_PI * r * 2);
        }
        
        float GetRadio()
        {
            return radio;
        }
        
        circulo(float r)
        {
            SetRadio(r);
        }
};

class rectangulo : public figura
{
    private:
        float lado1, lado2;
    public:
        void SetLados(float l1, float l2)
        {
            lado1 = l1;
            lado2 = l2;
            figura::SetArea(l1*l2);
            figura::SetPerimetro(l1*2 + l2*2);
        }
        
        void GetLados(float &l1, float &l2)
        {
            l1 = lado1;
            l2 = lado2;
        }
        
        rectangulo() {} //Constructor por defecto para permitir usar un constructor de cuadrado
        rectangulo(float l1, float l2)
        {
            SetLados(l1, l2);
        }
};

class cuadrado : public rectangulo
{
    private:
        float lado;
    public:
        void SetLado(float l)
        {
            lado = l;
            rectangulo::SetLados(l, l);
        }
        
        float GetLado()
        {
            return lado;
        }
        
        cuadrado(float l)
        {
            SetLado(l);
        }
};

int Menu();
void CargarCirculo(circulo &cir);
void CargarRectangulo(rectangulo &rec);
void CargarCuadrado(cuadrado &cua);
void VerCirculo(circulo cir);
void VerRectangulo(rectangulo rec);
void VerCuadrado(cuadrado cua);

int main()
{
    int opcion;
    circulo cir(RADIO_DEF);
    rectangulo rec(LADO1_DEF, LADO2_DEF);
    cuadrado cua(LADO_DEF);
    
    cout << fixed << setprecision(PRECISION);
    
    do
    {
        opcion = Menu();
        switch (opcion)
        {
            case CARGAR_CIRCULO:
                CargarCirculo(cir);
                break;
            case CARGAR_RECTANGULO:
                CargarRectangulo(rec);
                break;
            case CARGAR_CUADRADO:
                CargarCuadrado(cua);
                break;
            case VER_CIRCULO:
                VerCirculo(cir);
                break;
            case VER_RECTANGULO:
                VerRectangulo(rec);
                break;
            case VER_CUADRADO:
                VerCuadrado(cua);
                break;
        }

    } while (opcion != SALIR);
    cout << "Chau!";
    
    return 0;
}

int Menu()
{
    int opcion;
    bool opValida = false;
    cout << CARGAR_CIRCULO << ". Cargar círculo" << endl;
    cout << CARGAR_RECTANGULO << ". Cargar rectángulo" << endl;
    cout << CARGAR_CUADRADO << ". Cargar cuadrado" << endl;
    cout << VER_CIRCULO << ". Ver círculo" << endl;
    cout << VER_RECTANGULO << ". Ver rectángulo" << endl;
    cout << VER_CUADRADO << ". Ver cuadrado" << endl;
    cout << SALIR << ". Salir" << endl;
    while (!opValida)
    {
        cin >> opcion;
        if ((opcion >= MIN_OPCION) && (opcion <= SALIR))
            opValida = true;
        else
            cout << "Ingresa una opción válida" << endl;
    }
    
    return opcion;
}

void CargarCirculo(circulo &cir)
{
    float radio;
    cout << "Ingresa el radio" << endl;
    cin >> radio;
    cir.SetRadio(radio);
}

void CargarRectangulo(rectangulo &rec)
{
    float lado1, lado2;
    cout << "Ingresa el lado 1" << endl;
    cin >> lado1;
    cout << "Ingresa el lado 2" << endl;
    cin >> lado2;
    rec.SetLados(lado1, lado2);
}

void CargarCuadrado(cuadrado &cua)
{
    float lado;
    cout << "Ingresa el lado" << endl;
    cin >> lado;
    cua.SetLado(lado);
}

void VerCirculo(circulo cir)
{
    float radio, area, perimetro;
    radio = cir.GetRadio();
    area = cir.GetArea();
    perimetro = cir.GetPerimetro();
    cout << "Radio: " << radio << endl;
    cout << "Área: " << area << endl;
    cout << "Perímetro: " << perimetro << endl;
}

void VerRectangulo(rectangulo rec)
{
    float lado1, lado2, area, perimetro;
    rec.GetLados(lado1, lado2);
    area = rec.GetArea();
    perimetro = rec.GetPerimetro();
    cout << "Lado 1: " << lado1 << endl;
    cout << "Lado 2: " << lado2 << endl;
    cout << "Área: " << area << endl;
    cout << "Perímetro: " << perimetro << endl;
}

void VerCuadrado(cuadrado cua)
{
    float lado, area, perimetro;
    lado = cua.GetLado();
    area = cua.GetArea();
    perimetro = cua.GetPerimetro();
    cout << "Lado: " << lado << endl;
    cout << "Área: " << area << endl;
    cout << "Perímetro: " << perimetro << endl;
}
