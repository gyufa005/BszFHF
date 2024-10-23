/***************************************************************************//**
 * @file     segmentlcd_individual.h
 * @brief    EFM32 Segment LCD Display driver extension
 *
 * @details  The extension makes it possible to turn on or off segments
 *           individually on the lower (7 alphanumeric characters) and
 *           on the upper (4 seven-segment digits) part of the LCD.
 *
 * Modified on: 2020. nov. 03.
 *              Fixed an array over indexing error in SegmentLCD_UpperSegments()
 *              reported by MIHALIK Mark
 *
 * Created on:  2018. oct. 15.
 *
 * Author:      NASZALY Gabor <naszaly@mit.bme.hu>
 * Based on:    Earlier implementation by SCHERER Balazs <scherer@mit.bme.hu>
 ******************************************************************************/

/* ----------------------------------------------------------------------------
 *  INCLUDES
 * ------------------------------------------------------------------------- */

#include "segmentlcd_individual.h"
#include <stdbool.h>
#include <em_lcd.h>

/* ----------------------------------------------------------------------------
 *  DEFINES
 * ------------------------------------------------------------------------- */

#define SEGMENT_LCD_UPPER_DISPLAY_DEF {                           \
      {                                                           \
        .com[0] = 7, .com[1] = 5, .com[2] = 2, .com[3] = 1,       \
        .bit[0] = 35, .bit[1] = 35, .bit[2] = 35, .bit[3] = 35,   \
        .com[4] = 3, .com[5] = 6, .com[6] = 4,                    \
        .bit[4] = 35, .bit[5] = 35, .bit[6] = 35,                 \
      },                                                          \
      {                                                           \
        .com[0] = 7, .com[1] = 5, .com[2] = 2, .com[3] = 1,       \
        .bit[0] = 36, .bit[1] = 36, .bit[2] = 36, .bit[3] = 36,   \
        .com[4] = 3, .com[5] = 6, .com[6] = 4,                    \
        .bit[4] = 36, .bit[5] = 36, .bit[6] = 36,                 \
      },                                                          \
      {                                                           \
        .com[0] = 7, .com[1] = 5, .com[2] = 2, .com[3] = 1,       \
        .bit[0] = 37, .bit[1] = 37, .bit[2] = 37, .bit[3] = 37,   \
        .com[4] = 3, .com[5] = 6, .com[6] = 4,                    \
        .bit[4] = 37, .bit[5] = 37, .bit[6] = 37,                 \
      },                                                          \
      {                                                           \
        .com[0] = 7, .com[1] = 5, .com[2] = 2, .com[3] = 1,       \
        .bit[0] = 38, .bit[1] = 38, .bit[2] = 38, .bit[3] = 38,   \
        .com[4] = 3, .com[5] = 6, .com[6] = 4,                    \
        .bit[4] = 38, .bit[5] = 38, .bit[6] = 38,                 \
      }                                                           \
    }

#define SEGMENT_LCD_LOWER_DISPLAY_DEF {                           \
      { /* 1 */                                                   \
        .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 13, .bit[1] = 14, .bit[2] = 14, .bit[3] = 14,   \
        .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
        .bit[4] = 13, .bit[5] = 13, .bit[6] = 13, .bit[7] = 13,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 14, .bit[9] = 14, .bit[10] = 14, .bit[11] = 14, \
        .com[12] = 5, .com[13] = 6,                               \
        .bit[12] = 13, .bit[13] = 13                              \
      },                                                          \
      { /* 2 */                                                   \
        .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 15, .bit[1] = 16, .bit[2] = 16, .bit[3] = 16,   \
        .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
        .bit[4] = 15, .bit[5] = 15, .bit[6] = 15, .bit[7] = 15,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 16, .bit[9] = 16, .bit[10] = 16, .bit[11] = 16, \
        .com[12] = 5, .com[13] = 6,                               \
        .bit[12] = 15, .bit[13] = 15                              \
      },                                                          \
      { /* 3 */                                                   \
        .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 17, .bit[1] = 18, .bit[2] = 18, .bit[3] = 18,   \
        .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
        .bit[4] = 17, .bit[5] = 17, .bit[6] = 17, .bit[7] = 17,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 18, .bit[9] = 18, .bit[10] = 18, .bit[11] = 18, \
        .com[12] = 5, .com[13] = 6,                               \
        .bit[12] = 17, .bit[13] = 17                              \
      },                                                          \
      { /* 4 */                                                   \
        .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 19, .bit[1] = 28, .bit[2] = 28, .bit[3] = 28,   \
        .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
        .bit[4] = 19, .bit[5] = 19, .bit[6] = 19, .bit[7] = 19,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 28, .bit[9] = 28, .bit[10] = 28, .bit[11] = 28, \
        .com[12] = 5, .com[13] = 6,                               \
        .bit[12] = 19, .bit[13] = 19                              \
      },                                                          \
      { /* 5 */                                                   \
        .com[0] = 0, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 29, .bit[1] = 30, .bit[2] = 30, .bit[3] = 30,   \
        .com[4] = 6, .com[5] = 2, .com[6] = 3, .com[7] = 1,       \
        .bit[4] = 29, .bit[5] = 29, .bit[6] = 29, .bit[7] = 29,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 30, .bit[9] = 30, .bit[10] = 30, .bit[11] = 30, \
        .com[12] = 4, .com[13] = 5,                               \
        .bit[12] = 29, .bit[13] = 29                              \
      },                                                          \
      { /* 6 */                                                   \
        .com[0] = 0, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 31, .bit[1] = 32, .bit[2] = 32, .bit[3] = 32,   \
        .com[4] = 6, .com[5] = 2, .com[6] = 3, .com[7] = 1,       \
        .bit[4] = 31, .bit[5] = 31, .bit[6] = 31, .bit[7] = 31,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 32, .bit[9] = 32, .bit[10] = 32, .bit[11] = 32, \
        .com[12] = 4, .com[13] = 5,                               \
        .bit[12] = 31, .bit[13] = 31                              \
      },                                                          \
      { /* 7 */                                                   \
        .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
        .bit[0] = 33, .bit[1] = 34, .bit[2] = 34, .bit[3] = 34,   \
        .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
        .bit[4] = 33, .bit[5] = 33, .bit[6] = 33, .bit[7] = 33,   \
        .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
        .bit[8] = 34, .bit[9] = 34, .bit[10] = 34, .bit[11] = 34, \
        .com[12] = 5, .com[13] = 6,                               \
        .bit[12] = 33, .bit[13] = 33                              \
      }                                                           \
    }

/* ----------------------------------------------------------------------------
 *  TYPEDEFS
 * ------------------------------------------------------------------------- */

 /**************************************************************************//**
  * @brief Defines segment COM and BIT fields numeric display
  *****************************************************************************/
 typedef struct {
   uint8_t com[7]; /**< LCD COM line (for multiplexing) */
   uint8_t bit[7]; /**< LCD bit number */
 } Upper_TypeDef;

/**************************************************************************//**
 * @brief
 * Defines each text symbol's segment in terms of COM and BIT numbers,
 * in a way that we can enumerate each bit for each text segment in the
 * following bit pattern:
 * @verbatim
 *  -------0------
 *
 * |   \7  |8  /9 |
 * |5   \  |  /   |1
 *
 *  --6---  ---10--
 *
 * |    /  |  \11 |
 * |4  /13 |12 \  |2
 *
 *  -------3------
 * @endverbatim
 * E.g.: First text character bit pattern #3 (above) is
 * Segment 1D for Display
 * Location COM 3, BIT 0
 *****************************************************************************/
typedef struct {
  uint8_t com[14]; /**< LCD COM line (for multiplexing) */
  uint8_t bit[14]; /**< LCD bit number */
} Lower_TypeDef;

/* ----------------------------------------------------------------------------
 *  GLOBALS
 * ------------------------------------------------------------------------- */

/**< Numbers on display */
Upper_TypeDef  Number[SEGMENT_LCD_NUM_OF_UPPER_CHARS] = SEGMENT_LCD_UPPER_DISPLAY_DEF;
/**< Text on display */
Lower_TypeDef    Text[SEGMENT_LCD_NUM_OF_LOWER_CHARS] = SEGMENT_LCD_LOWER_DISPLAY_DEF;

/* ----------------------------------------------------------------------------
 *  FUNCTION DEFINITIONS
 * ------------------------------------------------------------------------- */

void SegmentLCD_UpperSegments(SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS])
{
    uint32_t com, bit;
    uint8_t s, c;
    /* If an update is in progress we must block, or there might be tearing */
    LCD_SyncBusyDelay(0xFFFFFFFF);

    /* Freeze LCD to avoid partial updates */
    LCD_FreezeEnable(true);

    for (c = 0; c < SEGMENT_LCD_NUM_OF_UPPER_CHARS; c++)
    {
       for (s = 0; s < 7; s++)
          {
            bit = Number[c].bit[s];
            com = Number[c].com[s];

             if (upperCharSegments[c].raw & (1 << s))
             {
                /* Turn on segment */
                LCD_SegmentSet(com, bit, true);
             } else {
                /* Turn off segment */
                LCD_SegmentSet(com, bit, false);
             }
          }
    }

    LCD_FreezeEnable(false);
}

void SegmentLCD_LowerSegments(SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS])
{
    uint32_t com, bit;
    uint8_t s, c;
    /* If an update is in progress we must block, or there might be tearing */
    LCD_SyncBusyDelay(0xFFFFFFFF);

    /* Freeze LCD to avoid partial updates */
    LCD_FreezeEnable(true);

    for (c = 0; c < SEGMENT_LCD_NUM_OF_LOWER_CHARS; c++)
    {
       for (s = 0; s < 14; s++)
          {
            bit = Text[c].bit[s];
            com = Text[c].com[s];

             if (lowerCharSegments[c].raw & (1 << s))
             {
                /* Turn on segment */
                LCD_SegmentSet(com, bit, true);
             } else {
                /* Turn off segment */
                LCD_SegmentSet(com, bit, false);
             }
          }
    }

    LCD_FreezeEnable(false);
}

