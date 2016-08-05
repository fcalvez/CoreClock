#pragma once

#include "spr_numbers.h"

// returns the line[line] of srpite[index]
uint8_t getSpriteLine(uint8_t sprite_num, uint8_t line_num) {

	// 1 to 10 => sprites for 1, 2, 3, .. 0
	if (sprite_num >= 0 && sprite_num < 10)
		// 1 sprite = 8 bytes
		return pgm_read_byte(spr_numbers + sprite_num * 8 + line_num);

	// default = empty line
	return 0;
}
