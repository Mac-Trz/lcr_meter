/*
 * ssd1306_oled.h
 *
 *  Created on: Jul 29, 2024
 *      Author: mac
 */
#include "stm32f1xx_hal.h"

#ifndef SRC_SSD1306_OLED_H_
#define SRC_SSD1306_OLED_H_

#define CONTINOUS_DATA 0x40 //0b01000000
#define CONTINOUS_COMMAND 0x00 //0b00000000
#define SINGLE_DATA 0xC0 //0b11000000
#define SINGLE_COMMAND 0X80 //0b10000000

#define SSD_1306_ADDR 120


#define SetMemoryAddrMode 0x20
#define SetColAddress 0x21
#define SetPageAddress 0x22

#define FONT_SIZE 5
#define FONT_LEN 17
#define MAX_NUMB 3
typedef enum {Horizontal=0,Vertical=1,Page=2} addr_mode_t;

HAL_StatusTypeDef OLED_send_command(unsigned char com);
HAL_StatusTypeDef OLED_send_data(unsigned char* dat,unsigned int sz);
HAL_StatusTypeDef OLED_set_start_end_page_mode_hor(unsigned int page_start,unsigned int page_end);
HAL_StatusTypeDef OLED_set_start_end_col_mode_hor(unsigned int col_start,unsigned int col_end);
HAL_StatusTypeDef OLED_set_memory_addresing_mode(addr_mode_t mode);
HAL_StatusTypeDef OLED_reset_RAM();
HAL_StatusTypeDef OLED_init();
HAL_StatusTypeDef print_calc(unsigned int numb,unsigned int* col, unsigned int line);
HAL_StatusTypeDef OLED_printch(unsigned int id, unsigned int* col_start, unsigned int line);

#endif /* SRC_SSD1306_OLED_H_ */
