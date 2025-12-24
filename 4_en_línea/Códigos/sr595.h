/*
Esta biblioteca está pensada para controlar registros de desplazamiento 74HC595 con 3 pines de un PIC
Autor: Vicente Rodriguez Rapetti
*/

#ifndef SR595_H
#define SR595_H

#include <xc.h>
#include <stdint.h>

/* ========== CONFIGURACIÓN DEL USUARIO ========== */

// El usuario DEBE definir estos macros antes de incluir la biblioteca
#ifndef SR595_DATA
#error "Definir SR595_DATA"
#endif

#ifndef SR595_CLK
#error "Definir SR595_CLK"
#endif

#ifndef SR595_LATCH
#error "Definir SR595_LATCH"
#endif

#ifndef SR595_NUMBER
#error "Definir SR595_NUMBER"
#endif

/* ========== API PÚBLICA ========== */

void sr595_latch(void);
void sr595_shiftByte(uint8_t data);
void sr595_update(uint8_t *buffer);
void sr595_resetAll(void);
void sr595_setAll(void);

#endif
