/***************************************************************************//**
 * @file     segmentlcd_individual.h
 * @brief    EFM32 Segment LCD Display driver extension, header file
 *
 * @details  The extension makes it possible to turn on or off segments
 *           individually on the lower (7 alphanumeric characters) and
 *           on the upper (4 seven-segment digits) part of the LCD.
 *
 * Created on:  2018. oct. 15.
 * Author:      NASZALY Gabor <naszaly@mit.bme.hu>
 * Based on:    Earlier implementation by SCHERER Balazs <scherer@mit.bme.hu>
 ******************************************************************************/

#ifndef SRC_SEGMENTLCD_INDIVIDUAL_H_
#define SRC_SEGMENTLCD_INDIVIDUAL_H_

/* ----------------------------------------------------------------------------
 *  INCLUDES
 * ------------------------------------------------------------------------- */
#include <stdint.h>

/* ----------------------------------------------------------------------------
 *  DEFINES
 * ------------------------------------------------------------------------- */

/**************************************************************************//**
 * @brief
 *   Defines the number of seven-segment digits located in the upper part of
 *   the LCD.
 ******************************************************************************/
#define SEGMENT_LCD_NUM_OF_UPPER_CHARS 4

/**************************************************************************//**
 * @brief
 *   Defines the number of 14-segment alphanumeric characters located in the
 *   lower part of the LCD.
 ******************************************************************************/
#define SEGMENT_LCD_NUM_OF_LOWER_CHARS 7

/* ----------------------------------------------------------------------------
 *  TYPEDEFS
 * ------------------------------------------------------------------------- */

/**************************************************************************//**
 * @brief
 *   Typedef to store the segment data of a 7-segment digit.
 *
 * @details
 *   The typedef uses an 8 bit integer to store the data. As we need only 7
 *   segments, the MSB is unused. The bits are mapped to the segments in the
 *   following way:
 *
 *    --- 0 (a) ---
 *   |              |
 *   |5 (f)         |1 (b)
 *   |              |
 *    --- 6 (g) ---
 *   |              |
 *   |4 (e)         |2 (c)
 *   |              |
 *    --- 3 (d) ---
 *
 *   The bits can be accessed in two ways:
 *
 *    1. Using a single, 8 bit value (raw).
 *    2. Accessing individual bits using bit fields (a..g).
 *
 ******************************************************************************/
typedef union
{
   uint8_t raw;
   struct {
   uint8_t a : 1;
   uint8_t b : 1;
   uint8_t c : 1;
   uint8_t d : 1;
   uint8_t e : 1;
   uint8_t f : 1;
   uint8_t g : 1;
   };
} SegmentLCD_UpperCharSegments_TypeDef;

/**************************************************************************//**
 * @brief
 *   Typedef to store the segment data of a 14-segment alphanumeric
 *   character.
 *
 * @details
 *   The typedef uses a 16 bit integer to store the data. As we need only 14
 *   segments, the upper two bits are unused. The bits are mapped to the
 *   segments in the following way:
 *
 *    --------- 0,a --------
 *
 *   |     \7,h  |8,j  /    |
 *   |5,f   \    |    /9,k  |1,b
 *   |       \   |   /      |
 *
 *    --- 6,g --   -- 10,m --
 *
 *   |      /    |    \11,n |
 *   |4,e  /13,q |12,p \    |2,c
 *   |    /      |      \   |
 *
 *    --------- 3,d --------
 *
 *   The bits can be accessed in two ways:
 *
 *    1. Using a single, 16 bit value (raw).
 *    2. Accessing individual bits using bit fields (a..q).
 *
 ******************************************************************************/
typedef union
{
   uint16_t raw;
   struct {
   uint16_t a : 1;
   uint16_t b : 1;
   uint16_t c : 1;
   uint16_t d : 1;
   uint16_t e : 1;
   uint16_t f : 1;
   uint16_t g : 1;
   uint16_t h : 1;
   uint16_t j : 1;
   uint16_t k : 1;
   uint16_t m : 1;
   uint16_t n : 1;
   uint16_t p : 1;
   uint16_t q : 1;
   };
} SegmentLCD_LowerCharSegments_TypeDef;

/* ----------------------------------------------------------------------------
 *  FUNCTION PROTOTYPES
 * ------------------------------------------------------------------------- */

/**************************************************************************//**
 * @brief
 *   Update the upper (four, seven-segment digits) part of the LCD using
 *   segment data passed as a parameter.
 *
 * @param[in] upperCharSegments
 *   A pointer to an array of SegmentLCD_UpperCharSegments_TypeDef elements. The
 *   size of the array should be SEGMENT_LCD_NUM_OF_UPPER_CHARS! Each element in
 *   this array holds the segment data of a digit. Each bit in the segment data
 *   corresponds to a segment in the given digit (1: turn on, 0: turn off).
 *
 * @return
 *   None.
 *
 *****************************************************************************/
void SegmentLCD_UpperSegments(
      SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS]
);

/**************************************************************************//**
 * @brief
 *   Update the lower (seven, 14-segment alphanumeric characters) part of the
 *   LCD using segment data passed as a parameter.
 *
 * @param[in] lowerCharSegments
 *   A pointer to an array of SegmentLCD_LowerCharSegments_TypeDef elements. The
 *   size of the array should be SEGMENT_LCD_NUM_OF_LOWER_CHARS! Each element in
 *   this array holds the segment data of a character. Each bit in the segment
 *   data corresponds to a segment in the given character (1: turn on,
 *   0: turn off).
 *
 * @return
 *   None.
 *
 *****************************************************************************/
void SegmentLCD_LowerSegments(
      SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS]
);

#endif /* SRC_SEGMENTLCD_INDIVIDUAL_H_ */
