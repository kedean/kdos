#pragma once
#include "../kernel/types.h"

uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port, uint8_t data);
uint8_t port_word_in(uint16_t port);
void port_word_out(uint16_t port, uint8_t data);

void io_wait();

void PIC_remap(uint8_t offset1, uint8_t offset2);