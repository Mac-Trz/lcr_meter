#include "main.h"
#include "ssd1306_oled.h"
#include <stdlib.h>
#include <string.h>

#define I2C_HANDLE hi2c1
extern I2C_HandleTypeDef I2C_HANDLE;

const unsigned char cmdInit[]={
	0xAE,				//display off
	0xA8,0x1F,	//set mux
	0xD3,0x00,	//set display offset
	0x40, 			//setdisplay line
	0xA0,0xA1, 	//set segment remap
	0xC0,0xC8, 	//set com output scan direction
	0xDA,0x02,	//set com pins hardware
	0x81,0x7F, 	//set contrast
	0xA4, 			//disable ignoring GRAM / (disable entire display on)
	0xA6, 			//normall display
	0xD5,0x80, 	//set freq
	0x8D,0x14, 	//enable charge pump
	0xAF 				//display on
};


const unsigned char OledFontTable[FONT_LEN][FONT_SIZE]={

		{0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
		{0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
		{0x42, 0x61, 0x51, 0x49, 0x46},   // 2
		{0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
		{0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
		{0x27, 0x45, 0x45, 0x45, 0x39},   // 5
		{0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
		{0x01, 0x71, 0x09, 0x05, 0x03},   // 7
		{0x36, 0x49, 0x49, 0x49, 0x36},   // 8
		{0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
		{0xFC, 0x24, 0x24, 0x24, 0x18},   // p
		{0x7C, 0x08, 0x04, 0x04, 0x78},   // n
		{0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
		{0x7C, 0x04, 0x18, 0x04, 0x78},   // m
		{0x7F, 0x09, 0x09, 0x09, 0x01},   // F
		{0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
		{0x00, 0x60, 0x60, 0x00, 0x00}    // .
};






HAL_StatusTypeDef OLED_send_command(unsigned char com){
	HAL_StatusTypeDef ret=1;
	unsigned char buf[2]={0};
	buf[0]=SINGLE_COMMAND;
	buf[1]=com;

	ret=HAL_I2C_Master_Transmit(&I2C_HANDLE,SSD_1306_ADDR,buf,sizeof(buf),HAL_MAX_DELAY);
	return ret;

}

HAL_StatusTypeDef OLED_send_data(unsigned char* dat,unsigned int sz){
	HAL_StatusTypeDef ret=1;
	unsigned char *buf=malloc(sizeof(char)*(sz+1));
	if(buf==NULL)return HAL_ERROR;
	if(sz>1){
		buf[0]=CONTINOUS_DATA;
	}
	else{
		buf[0]=SINGLE_DATA;
	}
	memcpy(&buf[1],dat,sz*sizeof(char));
	ret=HAL_I2C_Master_Transmit(&I2C_HANDLE,SSD_1306_ADDR,buf,sz+1,HAL_MAX_DELAY);
	free(buf);
	return ret;

}


HAL_StatusTypeDef OLED_set_start_end_page_mode_hor(unsigned int page_start,unsigned int page_end){
	HAL_StatusTypeDef ret;
	ret=OLED_send_command(SetPageAddress);
	if(ret!=0)return ret;

	ret=OLED_send_command(page_start);
	if(ret!=0)return ret;

	ret=OLED_send_command(page_end);
	return ret;

}
HAL_StatusTypeDef OLED_set_start_end_col_mode_hor(unsigned int col_start,unsigned int col_end){
	HAL_StatusTypeDef ret;
	ret=OLED_send_command(SetColAddress);
	if(ret!=0)return ret;

	ret=OLED_send_command(col_start);
	if(ret!=0)return ret;

	ret=OLED_send_command(col_end);
	return ret;
}




HAL_StatusTypeDef OLED_set_memory_addresing_mode(addr_mode_t mode){
	HAL_StatusTypeDef ret;

	ret=OLED_send_command(SetMemoryAddrMode);
	if(ret!=0)return ret;

	OLED_send_command(mode);
	return ret;
}

HAL_StatusTypeDef OLED_reset_RAM(){
	HAL_StatusTypeDef ret;

	unsigned char buf[1024]={0};
	ret=OLED_set_memory_addresing_mode(Horizontal);
	if(ret!=0)return ret;

	ret=OLED_set_start_end_col_mode_hor(0,127);
	if(ret!=0)return ret;

	ret=OLED_set_start_end_page_mode_hor(0,7);

	ret=OLED_send_data(buf,1024);
	if(ret!=0)return ret;

	ret=OLED_set_start_end_col_mode_hor(0,127);
	if(ret!=0)return ret;

	ret=OLED_set_start_end_page_mode_hor(0,7);
	return ret;

}

HAL_StatusTypeDef OLED_init(){
	HAL_StatusTypeDef ret=3;
	for(int i=0;i<sizeof(cmdInit);i++){
		ret=OLED_send_command(cmdInit[i]);
		if(ret!=0)return ret;
	}
	return ret;

}


HAL_StatusTypeDef print_calc(unsigned int numb,unsigned int* col, unsigned int line){
	HAL_StatusTypeDef ret;
	unsigned int temp=0;
	unsigned int buf[MAX_NUMB]={1};
	unsigned int count=0,id=0;
	int i=0;

	if(numb==0){
		ret=OLED_printch(0, col, line);

	}
	else{
		while( (numb>=1) && (count<MAX_NUMB)){
			temp=numb;
			numb=numb/10;
			buf[count]=temp-numb*10;
			count++;
		}
		count--;

		for(i=count;i>=0;i--){
			id=buf[i];
			ret=OLED_printch(id, col, line);
			if(ret!=0)return ret;

		}
	}
	return ret;
}


HAL_StatusTypeDef OLED_printch(unsigned int id, unsigned int* col_start, unsigned int line){
    unsigned int j,i=0;
	unsigned int col=*col_start;
	unsigned char dat[4]={0};
	HAL_StatusTypeDef ret;
	ret=OLED_set_start_end_page_mode_hor(line*2,line*2+1);
	if(ret!=0)return ret;

	for(i=0;i<FONT_SIZE;i++){
		ret=OLED_set_start_end_col_mode_hor(col,col+1);
		if(ret!=0)return ret;
		dat[0]=0;
		dat[2]=0;

		for(j=0;j<4;j++){
			dat[0]|=((OledFontTable[id][i]>>j) & 0x01)*(3<<(j*2));
			dat[2]|=((OledFontTable[id][i]>>(j+4)) & 0x01)*(3<<(j*2));
		}

		dat[1]=dat[0];
		dat[3]=dat[2];

		ret=OLED_send_data(dat,4);
		if(ret!=0)return ret;

		col+=2;
	}
	(*col_start)=(*col_start)+12;
	return ret;

}
