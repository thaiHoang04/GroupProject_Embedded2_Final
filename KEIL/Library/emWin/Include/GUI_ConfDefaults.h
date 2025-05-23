/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2023  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.46 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. 
This file may
only be used in accordance with the following terms:

The  software has  been licensed by SEGGER Software GmbH to Nuvoton Technology Corporation
at the address: No. 4, Creation Rd. III, Hsinchu Science Park, Taiwan
for the purposes  of  creating  libraries  for its 
Arm Cortex-A, Arm Cortex-M and  Arm9 32-bit microcontrollers, commercialized and distributed by Nuvoton Technology Corporation
under  the terms and conditions  of  an  End  User  
License  Agreement  supplied  with  the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M, Cortex-A, and ARM9 32-bit series microcontroller designed and manufactured by Nuvoton Technology Corporation.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2025-09-04
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUI_ConfDefaults.h
Purpose     : Defaults for GUI config switches.
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/

#ifndef  GUI_CONFDEFAULTS_H
#define  GUI_CONFDEFAULTS_H

#include "GUIConf.h"

#ifndef   GUI_SUPPORT_BIDI
  #define GUI_SUPPORT_BIDI 1
#endif

#ifndef   GUI_USE_BIDI2
  #define GUI_USE_BIDI2 1
#endif

#ifndef   LCD_MAX_LOG_COLORS
  #define LCD_MAX_LOG_COLORS 256
#else
  #if (LCD_MAX_LOG_COLORS > 256)
    #error The value of LCD_MAX_LOG_COLORS must be <= 256!
  #endif
#endif

#define LCD_PIXELINDEX U32

#ifndef LCD_YMAG
  #define LCD_YMAG 1
#endif
#ifndef LCD_XMAG
  #define LCD_XMAG 1
#endif

#ifndef   GUI_SIM_SUPPORT_EMBOS
  #if (defined(_WIN64) || defined(__LP64__))
    #define GUI_SIM_SUPPORT_EMBOS 0
  #else
    #define GUI_SIM_SUPPORT_EMBOS 1
  #endif
#endif

/**********************************************************************
*
*       Defaults for config switches
*
***********************************************************************
*/

/* ATTENTION: This define swaps the meaning of a logical color from
   ABGR to ARGB.
   
   It further swaps the meaning of a transparent pixel:
   ABGR: 0x00 means opaque, 0xFF means transparent
   ARGB: 0x00 means transparent, 0xFF means opaque (default)
*/
#ifndef GUI_USE_ARGB
  #define GUI_USE_ARGB 1
#endif

/* Define "universal pointer". Normally, this is not needed (define will expand to nothing)
   However, on some systems (AVR - IAR compiler) it can be necessary ( -> __generic),
   since a default pointer can access RAM only, not the built-in Flash
*/
#ifndef GUI_UNI_PTR
  #define GUI_UNI_PTR /* Remains only for compatibility purpose, no longer used in emWin */
#endif

/* Define const storage. Normally, this is not needed (define will expand to const)
   However, on some systems (AVR - IAR compiler) it can be necessary ( -> __flash const),
   since otherwise constants are copied into RAM
*/
#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#ifndef GUI_USE_MEMDEV_1BPP_FOR_SCREEN
  #define GUI_USE_MEMDEV_1BPP_FOR_SCREEN 1
#endif

#ifndef GUI_BIDI_MAX_CHARS_PER_LINE
  #if GUI_USE_BIDI2
    #define GUI_BIDI_MAX_CHARS_PER_LINE 200
  #else
    #define GUI_BIDI_MAX_CHARS_PER_LINE  80
  #endif
#endif

#ifndef GUI_WINSUPPORT
  #define GUI_WINSUPPORT      0
#endif

#ifndef GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH   0
#endif

#ifndef GUI_SUPPORT_MOUSE
  #define GUI_SUPPORT_MOUSE   0
#endif

#ifndef GUI_SUPPORT_MEMDEV
  #define GUI_SUPPORT_MEMDEV  0
#endif

#ifndef GUI_SUPPORT_AA
  #define GUI_SUPPORT_AA      1
#endif

#ifndef GUI_OS
  #define GUI_OS              0
#endif

#ifndef GUI_NUM_LAYERS
  #define GUI_NUM_LAYERS      4
#endif

#ifndef GUI_SUPPORT_CURSOR
  #define GUI_SUPPORT_CURSOR  (GUI_SUPPORT_MOUSE | GUI_SUPPORT_TOUCH)
#endif

#ifndef   GUI_CURSOR_LAYER
  #define GUI_CURSOR_LAYER 0
#endif

#ifndef GUI_SUPPORT_ROTATION
  #define GUI_SUPPORT_ROTATION 1
#endif

/* In order to avoid warnings for undefined parameters */
#ifndef GUI_USE_PARA
  #define GUI_USE_PARA(para) (void)para
#endif

/* Default for types */
#ifndef GUI_TIMER_TIME
  #define GUI_TIMER_TIME int  /* default is to use 16 bits for 16 bit CPUs,
	                           32 bits on 32 bit CPUs for timing */
#endif

/* Types used for memory allocation */
#define GUI_ALLOC_DATATYPE   I32
#define GUI_ALLOC_DATATYPE_U U32

#ifndef   GUI_MAX_XBF_BYTES
  #define GUI_MAX_XBF_BYTES 200
#endif

#ifndef   GUI_MEMSET
  #define GUI_MEMSET memset
#endif

#ifndef GUI_MEMCPY
  #define GUI_MEMCPY memcpy
#endif

#ifdef WIN32
   #if defined(_MSC_VER)
     //
     // MS VS <= 2010 standard library does not include ceilf/floorf/sqrtf
     //
     #if (_MSC_VER <= 1600)
       #ifndef GUI_CEIL
         #define GUI_CEIL ceil
       #endif
       #ifndef GUI_FLOOR
         #define GUI_FLOOR floor
       #endif
       #ifndef GUI_SQRT
         #define GUI_SQRT sqrt
       #endif
     #endif
   #endif
#endif 

#ifndef GUI_CEIL
  #define GUI_CEIL ceilf
#endif

#ifndef GUI_FLOOR
  #define GUI_FLOOR floorf
#endif

#ifndef GUI_SQRT
  #define GUI_SQRT sqrtf
#endif

/* Optional custom drawing of memory devices */
#ifndef   GUI_MEMDEV_SUPPORT_CUSTOMDRAW
  #define GUI_MEMDEV_SUPPORT_CUSTOMDRAW 0
#endif

#endif   /* ifdef GUI_CONFDEFAULTS_H */

/*************************** End of file ****************************/
