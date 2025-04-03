/*
a.	¿Qué valor tiene b al finalizar el programa?
0.003
b.	¿Qué valor tiene c al finalizar el programa?
0.003
c.	¿Qué valor tiene (*pa) al finalizar el programa?
0.002
d.	¿Qué valor tiene (*pb) al finalizar el programa?
0.003

Código para comprobar
*/
#include <stdio.h>

int main()
{
	float a = 0.001, b = 0.003;
	float c, *pa, *pb;

	pa = &a;
	*pa = 2 * a;
	pb = &b;
	c = 3 * (*pb - *pa);
	
	printf("b: %f\n", b);
	printf("c: %f\n", c);
	printf("(*pa): %f\n", (*pa));
	printf("(*pb): %f\n", (*pb));

	return 0;
}
