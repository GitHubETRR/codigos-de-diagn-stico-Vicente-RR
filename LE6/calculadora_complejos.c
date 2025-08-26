#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define CARTESIANA '1'
#define POLAR '2'

#define CARGAR_A 1
#define CARGAR_B 2
#define SUMAR 3
#define RESTAR 4
#define MULTIPLICAR 5
#define DIVIDIR 6

class complejo
{
    private:
        float x, y;
    public:
        complejo();
        void Guarda(float x2, float y2);
        void Cartesiana(float &x2, float &y2);
        void Polar(float &mod, float &ang);
};

complejo::complejo()
{
    x = 0;
    y = 0;
}

void complejo::Guarda(float x2, float y2)
{
    x = x2;
    y = y2;
}

void complejo::Cartesiana(float &x2, float &y2)
{
    x2 = x;
    y2 = y;
}

void complejo::Polar(float &mod, float &ang)
{
    mod = sqrt(x*x + y*y);
    ang = atan2(y, x);
}

complejo operator + (complejo a, complejo b){
    complejo res;
    float aX, aY, bX, bY, resX, resY;
    a.Cartesiana(aX, aY);
    b.Cartesiana(bX, bY);
    resX = aX + bX;
    resY = aY + bY;
    res.Guarda(resX, resY);
    return res;
};

complejo operator - (complejo a, complejo b){
    complejo res;
    float aX, aY, bX, bY, resX, resY;
    a.Cartesiana(aX, aY);
    b.Cartesiana(bX, bY);
    resX = aX - bX;
    resY = aY - bY;
    res.Guarda(resX, resY);
    return res;
};

complejo operator * (complejo a, complejo b){
    complejo res;
    float aMod, aAng, bMod, bAng, resMod, resAng, resX, resY;
    a.Polar(aMod, aAng);
    b.Polar(bMod, bAng);
    resMod = aMod * bMod;
    resAng = aAng + bAng;
    resX = resMod * cos(resAng);
    resY = resMod * sin(resAng);
    res.Guarda(resX, resY);
    return res;
};

complejo operator / (complejo a, complejo b){
    complejo res;
    float aMod, aAng, bMod, bAng, resMod, resAng, resX, resY;
    a.Polar(aMod, aAng);
    b.Polar(bMod, bAng);
    resMod = aMod / bMod;
    resAng = aAng - bAng;
    resX = resMod * cos(resAng);
    resY = resMod * sin(resAng);
    res.Guarda(resX, resY);
    return res;
};

int Menu(complejo a, complejo b);
void ImprimirComplejo(complejo z);
complejo PedirDatos();
bool PedirForma();
void PedirCartesiana(complejo &z);
void PedirPolar(complejo &z);
void Sumar(complejo a, complejo b);
void Restar(complejo a, complejo b);
void Multiplicar(complejo a, complejo b);
void Dividir(complejo a, complejo b);
void LimpiarPantalla();

int main()
{
    complejo a;
    complejo b;
    bool salir = false;
    
    do
    {
        int opcion = Menu(a, b);
        switch (opcion)
        {
            case CARGAR_A:
            a = PedirDatos();
            break;
            
            case CARGAR_B:
            b = PedirDatos();
            break;
            
            case SUMAR:
            Sumar(a, b);
            break;
            
            case RESTAR:
            Restar(a, b);
            break;
            
            case MULTIPLICAR:
            Multiplicar(a, b);
            break;
            
            case DIVIDIR:
            Dividir(a, b);
            break;
            
            default:
            salir = true;
            break;
        }
    } while (!salir);

    return 0;
}

int Menu(complejo a, complejo b)
{
    int opcion;
    cout << "Calculadora de números complejos" << endl;
    cout << fixed << setprecision(2);
    cout << "A: ";
    ImprimirComplejo(a);
    cout << "B: ";
    ImprimirComplejo(b);
    cout << CARGAR_A << ". Cargar el complejo A" << endl;
    cout << CARGAR_B << ". Cargar el complejo B" << endl;
    cout << SUMAR << ". Sumar complejos A+B" << endl;
    cout << RESTAR << ". Restar complejos A-B" << endl;
    cout << MULTIPLICAR << ". Multiplicar complejos A*B" << endl;
    cout << DIVIDIR << ". Dividir complejos A/B" << endl;
    cout << "Otra tecla para salir" << endl;
    cin >> opcion;
    LimpiarPantalla();
    return opcion;
}

void ImprimirComplejo(complejo z)
{
    float x, y, mod, ang;
    z.Cartesiana(x, y);
    z.Polar(mod, ang);
    ang *= 180.0 / M_PI;
    cout << x << " + j*" << y << " / " << mod << " * e^j" << ang << "°" << endl;
}

complejo PedirDatos()
{
    complejo z;
    bool formaCartesiana = PedirForma();
    if (formaCartesiana)
        PedirCartesiana(z);
    else
        PedirPolar(z);
    return z;
}

bool PedirForma()
{
    char opcion;
    bool opcionValida = false;
    bool formaCartesiana;
    
    cout << "Enviá " << CARTESIANA << " para ingresar en forma cartesiana" << endl;
    cout << "Enviá " << POLAR << " para ingresar en forma polar" << endl;
    do
    {
        cin >> opcion;
        if ((opcion == CARTESIANA) || (opcion == POLAR))
            opcionValida = true;
        else
            cout << "Opción no válida" << endl;
    } while (!opcionValida);
    if (opcion == CARTESIANA)
        formaCartesiana = true;
    else
        formaCartesiana = false;
    LimpiarPantalla();
    
    return formaCartesiana;
}

void PedirCartesiana(complejo &z)
{
    float x, y;
    cout << "Ingresa la parte real" << endl;
    cin >> x;
    cout << "Ingresa la parte imaginaria" << endl;
    cin >> y;
    z.Guarda(x, y);
}

void PedirPolar(complejo &z)
{
    float mod, ang, x, y;
    cout << "Ingresa el módulo" << endl;
    cin >> mod;
    cout << "Ingresa el ángulo (grados)" << endl;
    cin >> ang;
    ang *= M_PI / 180;
    x = mod * cos(ang);
    y = mod * sin(ang);
    z.Guarda(x, y);
}

void Sumar(complejo a, complejo b)
{
    complejo res = a + b;
    cout << "Resultado de A+B:" << endl;
    ImprimirComplejo(res);
}

void Restar(complejo a, complejo b)
{
    complejo res = a - b;
    cout << "Resultado de A-B:" << endl;
    ImprimirComplejo(res);
}

void Multiplicar(complejo a, complejo b)
{
    complejo res = a * b;
    cout << "Resultado de A*B:" << endl;
    ImprimirComplejo(res);
}

void Dividir(complejo a, complejo b)
{
    complejo res = a / b;
    cout << "Resultado de A/B:" << endl;
    ImprimirComplejo(res);
}

void LimpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
