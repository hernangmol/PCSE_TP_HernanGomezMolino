/*============================================================================
 * Módulo para prueba de driver para sensor DS18B20.
 * Archivo: PCSE_TP.c
 * Fecha 08/10/2021
 * Autor: Hernán Gomez Molino
 *===========================================================================*/

/*=====[Inclusión de dependencias]===========================================*/

#include "PCSE_TP.h"
#include "DS18B20.h"
#include "sapi.h"
#include "miApp_UART.h"

int main( void )
{
/*=====[Declaración e Inicialización de variables locales]===================*/

	delay_t hDelay;
	OWbus_t* punteroBus = NULL;
	int temp = 0;
	int devices = 0;
	int i = 0;
	int j = 0;
	int status;
	int byte;

/*=====[Bloque de ejecución por única vez]===================================*/

	boardInit();
	uartInit( UART_USB, 115200);
	// Led rojo indica ERROR!
	gpioInit( LEDR, GPIO_OUTPUT );
	// Led verde indica comunicación con sensor
	gpioInit( LEDG, GPIO_OUTPUT );
	// Se inicializa el bus OneWire
	punteroBus = OWinit(GPIO2);
	// chequeo de GPIO habilitado en LUT
	if(punteroBus == NULL)
	{
		// Led RGB en rojo= ERROR!
		gpioWrite( LEDR, true);
		// bloqueo del programa
		while(TRUE);
	}
	// ciclo de refresco de consola en 1 seg
	delayInit( &hDelay, 500 );
    // Se limpia la pantalla
	UART_clearScreen();
	 // Se imprime el encabezado
    UART_printHeader();
    status = -1;

/*=====[Bloque de ejecución cíclica]=========================================*/

	while( true )
	{
		if( delayRead( &hDelay ) )
		{
			switch(status)
			{
				case -1:
				{
					 status = UART_getCmd(& byte);
				}
				break;
				case 0:
				{
					devices=OWsearch(punteroBus,ALL_DEVICES);
					if(devices > 0)
				   	{
				   		printf("ROM:");
				 	   	for(i=0;i<8;i++)
				   		printf("%X-",OneWireStruct->ROM_NO[i] );
				 	   	printf("\n\r");

				   	}
				   	else
				   	{
				   		printf("busqueda finalizada\n\r");
				   		status = UART_getCmd(& byte);
				   		status = -1;
				   	}
				}
				break;
				case 1:
				case 2:
				case 3:
				case 4:
				{
					devices=OWsearch(punteroBus,0xF0);
					if(devices > 0)
					{
						printf("ROM:");
						for(i=0;i<8;i++)
						{
							printf("%X-",OneWireStruct->ROM_NO[i] );
							sensor[j].ROM_NO[i] = OneWireStruct->ROM_NO[i];
						}
						j++;
						printf("\n\r");
						if (j >= status)
						{
							j = 0;
							status = 5;
						}
					}
					else
					{
						printf("busqueda finalizada\n\r");
						status = UART_getCmd(& byte);
						status = -1;
					}
				break;
				case 5:
					UART_clearScreen();
					printf("TERMOMETRO MULTIZONA\n");
					for(j=0;j<4;j++)
					{
						if(sensor[j].ROM_NO[0]==0x28)
						{
							sensor[j].temp = DSreadROMtemp(punteroBus->OWport,punteroBus->OWpin, j);
							printf("\r zona %d (%X)-------- \n",j+1,sensor[j].ROM_NO[7]);
						}
						else j=4;

					}
					status = 6;
				case 6:
					temp = 0;
					DSmultipleConv(punteroBus->OWport,punteroBus->OWpin);
					for(j=0;j<4;j++)
					{
						if(sensor[j].ROM_NO[0]==0x28)
						{
							sensor[j].temp = DSreadROMtemp(punteroBus->OWport,punteroBus->OWpin, j);
						}
						else j=4;
					}
					// comando ESC
					uartWriteByte(UART_USB, 27);
					// Reposiciona el cursor
					uartWriteString(UART_USB, "[2;19H");
					//printf("prueba\n");
					for(j=0;j<4;j++)
					{
						if(sensor[j].ROM_NO[0]==0x28)
						//if(TRUE)
						{
							printf("%d\n",sensor[j].temp);
							// comando ESC
							uartWriteByte(UART_USB, 27);
							// Reposiciona el cursor
							uartWriteString(UART_USB, "[2D");
						}
						else
						{
							j=4;
						}
					}
					break;

				default:
				{
					 UART_clearScreen();
					 printf("Default :%d \n\r", status);
				}
			}
		}
	}



    }
   return 0;

}
