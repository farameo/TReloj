#ifndef __TRELOJ__
#define __TRELOJ__

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>	

#define ADDRESS_RELOJ 0x68

class TReloj {
private:
	byte TReloj::decToBcd(byte val);
	byte TReloj::bcdToDec(byte val);
	byte acumulador;

public:
	byte hora;						// contiene la HORA del reloj al ejecutar leer_hora();
	byte minutos;					// contiene los MINUTOS hora del reloj al ejecutar leer_minutos();
	float fTemperatura;				// contiene la TEPERATURA que infora el reloj al ejecutar leer_temperatura();

	byte intervalo;					// intervalo utilizado para "huboCambio" debe ser entre los valores 1 y 60.

	TReloj();

	void leer_hora() ;
	void leer_minutos();
	void leer_temperatura();
	void setClockMode() ;			//setea el reloj para que trabaje en formato 24 horas.

	void setear_hora(byte bValor);		// ejemplo: setear_hora(17) setea la hora del reloj sin cambiar los minutos.
	void setear_minutos(byte bValor);	// ejemplo: setear_minutos(23) setea los minutos del reloj sin cambiar la hora.

	bool huboCambio();					// chequear si hubo cambio de minutos con respecto al valor "intervalo"

};

#endif