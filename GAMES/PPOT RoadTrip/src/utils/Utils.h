#pragma once

// Extract individual digits of a uint8_t -------------------------------------

template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint8_t value) {

    for(uint8_t i = 0; i < size; ++i) {
        buffer[i] = value % 10;
        value /= 10;
    }

}

// Extract individual digits of a uint16_t
template< size_t size > void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

    for(uint8_t i = 0; i < size; ++i) {
        buffer[i] = value % 10;
        value /= 10;
    }

}
