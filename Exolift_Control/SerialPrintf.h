#pragma once

#include <string.h>
#include <stdlib.h>

#ifndef SERIAL_PRINTF_BUFFER_SIZE
#define SERIAL_PRINTF_BUFFER_SIZE 128
#endif

void SerialPrintf(const char *fmt, ...) {
	char buf[SERIAL_PRINTF_BUFFER_SIZE];
	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, SERIAL_PRINTF_BUFFER_SIZE, fmt, args);
	Serial.print(buf);
	va_end(args);
}
