/*============================================================================
 * Módulo de comunicación protocolo 1 Wire.
 * Archivo: DS18B20.h
 * Versión: 1.2
 * Fecha 08/10/2021
 * Autor: Hernán Gomez Molino
 *===========================================================================*/


#ifndef MIS_PROGS_PCSE_TP_FINAL_INC_DS18B20_H_
#define MIS_PROGS_PCSE_TP_FINAL_INC_DS18B20_H_

#include "oneWire.h"

/*=====[Declaración de tipos de datos públicos]==============================*/

typedef struct
{
	char ROM_NO[8];
	char ubic;
	char  temp;

} 	tempSens_t;

tempSens_t sensor[4];

/*=====[Declaración de funciones públicas]===================================*/

int  DSreadScratch(char *, int, int);
int  DSreadTemperature(int, int);
void DSmultipleConv(int, int);
int  DSreadROMtemp(int , int ,int);

#endif /* MIS_PROGS_TP_PDM_INC_SMTB_ONEWIRE_H_ */
