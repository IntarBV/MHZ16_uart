/*
  MHZ16_uart.h - MH-Z16 CO2 sensor library for ESP32
  by Intar BV
  version 0.1

  License MIT
*/

#ifndef MHZ16_uart_h_
#define MHZ16_uart_h_

#include "Arduino.h"
#ifdef ARDUINO_ARCH_ESP32
	#include "HardwareSerial.h"
#else
	#include "SoftwareSerial.h"
#endif

#define PREHEAT_MS 180000

enum MHZ16_DATA { 
	PPM, TEMPERATURE, STAT 
};

class MHZ16_uart
{
public:
	MHZ16_uart();
	MHZ16_uart(int rx, int tx);
	virtual ~MHZ16_uart();

#ifdef ARDUINO_ARCH_ESP32
	void begin(int rx=-1, int tx=-1, int s=1);
#else
	void begin(int rx=-1, int tx=-1);
#endif
	void setAutoCalibration(boolean autocalib);
	void calibrateZero();
	void calibrateSpan(int ppm);
	
	int getPPM();
	
	boolean isWarming();

protected:
	void writeCommand(uint8_t com[]);
	void writeCommand(uint8_t com[], uint8_t response[]);

private:
	uint8_t MHZ16_checksum( uint8_t com[] );
	int getSerialData();

	static const int REQUEST_CNT = 8;
	static const int RESPONSE_CNT = 9;

	// serial command
	uint8_t getppm[REQUEST_CNT]			= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t zerocalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t spancalib[REQUEST_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
#ifdef ARDUINO_ARCH_ESP32
	int _serialno = 1;
#endif
	int _rx_pin = -1;
	int _tx_pin = -1;
	unsigned long _start_millis = -1;
};

#endif

