#include <TReloj.h>

//------------------------------

TReloj::TReloj() {
}

//------------------------------

void TReloj::leer_hora() {
	byte temp_buffer;	
	
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(0x02);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS_RELOJ, 1);
	temp_buffer = Wire.read();

	hora = bcdToDec(temp_buffer & 0b00111111);	
}

//------------------------------

void TReloj::leer_minutos() {
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

void TReloj::setear_hora(byte bValor) {
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

void TReloj::setear_minutos(byte bValor) {
	Wire.beginTransmission(ADDRESS_RELOJ);
	Wire.write(uint8_t(0x01));
	Wire.write(decToBcd(bValor));	
	Wire.endTransmission();
}

//------------------------------

void TReloj::leer_temperatura() {
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

	leer_minutos();

	// 34;

	if ((minutos == acumulador ) || (acumulador == 0)) {
		acumulador = minutos + intervalo;
		Serial.println(acumulador, DEC);
		Serial.println(minutos, DEC);
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