/*
  MHZ16_uart.cpp - MH-Z16 CO2 sensor library for ESP-32
  by Intar BV
  version 0.1
  
  License MIT
*/

#include "MHZ16_uart.h"
#include "Arduino.h"


#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

// public

MHZ16_uart::MHZ16_uart(){
}
MHZ16_uart::MHZ16_uart(int rx, int tx){
	begin(rx,tx);
}

MHZ16_uart::~MHZ16_uart(){
}

#ifdef ARDUINO_ARCH_ESP32
void MHZ16_uart::begin(int rx, int tx, int s){
	_rx_pin = rx;
	_tx_pin = tx;
	_start_millis = millis();
	_serialno = s;
}
#else
void MHZ16_uart::begin(int rx, int tx){
	_rx_pin = rx;
	_start_millis = millis();
	_tx_pin = tx;
}
#endif

void MHZ16_uart::calibrateZero() {
	writeCommand( zerocalib );
}

void MHZ16_uart::calibrateSpan(int ppm) {
	if( ppm < 1000 )	return;

	uint8_t com[MHZ16_uart::REQUEST_CNT];
	for(int i=0; i<MHZ16_uart::REQUEST_CNT; i++) {
		com[i] = spancalib[i];
	}
	com[3] = (uint8_t)(ppm/256);
	com[4] = (uint8_t)(ppm%256);
	writeCommand( com );
}

int MHZ16_uart::getPPM() {
	return getSerialData();
}

boolean MHZ16_uart::isWarming(){
	return millis() <= _start_millis + PREHEAT_MS;
}

//protected
void MHZ16_uart::writeCommand(uint8_t cmd[]) {
	writeCommand(cmd,NULL);
}

void MHZ16_uart::writeCommand(uint8_t cmd[], uint8_t* response) {
#ifdef ARDUINO_ARCH_ESP32
	HardwareSerial hserial(_serialno);
	hserial.begin(9600, SERIAL_8N1, _rx_pin, _tx_pin);
#else
	SoftwareSerial hserial(_rx_pin, _tx_pin);
	hserial.begin(9600);
#endif
    hserial.write(cmd, REQUEST_CNT);
	hserial.write(MHZ16_checksum(cmd));
	hserial.flush();
	
	if (response != NULL) {
		int i = 0;
		while(hserial.available() <= 0) {
			if( ++i > WAIT_READ_TIMES ) {
				Serial.println("error: can't get MH-Z16 response.");
				return;
			}
			yield();
			delay(WAIT_READ_DELAY);
		}
		hserial.readBytes(response, MHZ16_uart::RESPONSE_CNT);
	}

}

//private

int MHZ16_uart::getSerialData() {
	uint8_t buf[MHZ16_uart::RESPONSE_CNT];
	for( int i=0; i<MHZ16_uart::RESPONSE_CNT; i++){
		buf[i]=0x0;
	}

	writeCommand(getppm, buf);
	int co2 = 0, co2temp = 0, co2status =  0;

	// parse
	if (buf[0] == 0xff && buf[1] == 0x86 && MHZ16_checksum(buf) == buf[MHZ16_uart::RESPONSE_CNT-1]) {
		co2 = buf[2] * 256 + buf[3];
	} else {
		co2 = co2temp = co2status = -1;
	}
	return co2;
}	

uint8_t MHZ16_uart::MHZ16_checksum( uint8_t com[] ) {
	uint8_t sum = 0x00;
	for ( int i = 1; i < MHZ16_uart::REQUEST_CNT; i++) {
		sum += com[i];
	}
	sum = 0xff - sum + 0x01;
	return sum;
}

