#include <TReloj.h>

//------------------------------

TReloj::TReloj() {
	intervalo = 1;
	inicio = true;
}

//------------------------------

void TReloj::leerHoras() {
	byte temp_buffer;	
	
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(0x02);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS_RELOJ, 1);
	temp_buffer = Wire.read();

	horas = bcdToDec(temp_buffer & 0b00111111);	
}

//------------------------------

void TReloj::leerMinutos() {
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(0x01);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS_RELOJ, 1);
	minutos = bcdToDec(Wire.read());
}

//------------------------------

void TReloj::setClockMode() {
	
	byte temp_buffer;

	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x02));
	Wire.endTransmission();
	Wire.requestFrom(ADDRESS_RELOJ, 1);
	temp_buffer = Wire.read();

	// setear en 24 hs.
	temp_buffer = temp_buffer & 0b10111111;

	// Write the byte
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x02));
	Wire.write(temp_buffer);
	Wire.endTransmission();
}

//------------------------------

void TReloj::setearHoras(byte bValor) {
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x02));
	Wire.endTransmission();

	bValor = decToBcd(bValor) & 0b10111111;
	
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x02));
	Wire.write(bValor);
	Wire.endTransmission();
}

//------------------------------

void TReloj::setearMinutos(byte bValor) {
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x01));
	Wire.write(decToBcd(bValor));	
	Wire.endTransmission();
}

//------------------------------

void TReloj::leerTemperatura() {
	// Checks the internal thermometer on the DS3231 and returns the 
	// temperature as a floating-point value.
	byte temp;
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x11));
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS_RELOJ, 2);
	temp = Wire.read();	// Here's the MSB
	Serial.println(temp, DEC);
	fTemperatura = float(temp) + 0.25*(Wire.read()>>6);

}

//------------------------------

bool TReloj::huboCambio() {

	leerHoras();
	leerMinutos();

	if ((minutos == acumulador ) || (inicio)) {
		acumulador = minutos + intervalo;
		if (acumulador >= 60) { 
			acumulador = acumulador - 60;
		}

		Serial.print("Horas: ");
		Serial.println(horas, DEC);
		Serial.print("Minutos: ");
		Serial.println(minutos, DEC);

		Serial.print("Acumulador: ");
		Serial.println(acumulador, DEC);

		inicio = false;
		return true;
	} else {
		return false;
	}

}

//------------------------------

byte TReloj::decToBcd(byte val) {
	return ( (val/10*16) + (val%10) );
}

byte TReloj::bcdToDec(byte val) {
	return ( (val/16*10) + (val%16) );
}