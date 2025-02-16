/*
 * LCD.h
 *
 *  Created on: Feb 16, 2025
 *      Author: Navier
 */

#ifndef STM_LCD_H_
#define STM_LCD_H_


void LCD_Init();
void LCD_Clear();
void LCD_PutCursor(int row, int col);
void LCD_Send_String(char *str);


#endif /* STM_LCD_H_ */
