/* 
 * File:   Stepper.h
 * Author: hansb
 *
 * Created on 6 de marzo de 2020, 12:29 PM
 */

#ifndef __STEPPER_H_
#define	__STEPPER_H_

//******************************************************************************
#ifndef  _XTAL_FREQ 
#define _XTAL_FREQ 8000000
#endif
//******************************************************************************

#ifndef PUERTO
#define PUERTO PORTB             //PUERTO DE CONEXIÓN DEL STEPPER CON EL PIC
#endif

//******************************************************************************
#ifndef speed
#define speed 5             //VELOCIDAD DE ROTACION
#endif

//******************************************************************************

void FULL_DRIVE(uint8_t direccion);
void HALF_DRIVE(uint8_t direccion);
void WAVE_DRIVE(uint8_t direccion);


#endif	/* STEPPER_H */

