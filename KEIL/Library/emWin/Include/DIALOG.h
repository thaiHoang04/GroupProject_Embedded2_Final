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
File        : Dialog.h
Purpose     : Dialog box include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef DIALOG_H
#define DIALOG_H

#include "WM.h"

#include "BUTTON.h"
#include "CALENDAR.h"
#include "CHECKBOX.h"
#include "CHOOSECOLOR.h"
#include "CHOOSEFILE.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "GAUGE.h"
#include "GRAPH.h"
#include "HEADER.h"
#include "ICONVIEW.h"
#include "IMAGE.h"
#include "KEYBOARD.h"
#include "KNOB.h"
#include "LISTBOX.h"
#include "LISTVIEW.h"
#include "LISTWHEEL.h"
#include "MENU.h"
#include "MOVIE.h"
#include "MULTIEDIT.h"
#include "MULTIPAGE.h"
#include "PROGBAR.h"
#include "QRCODE.h"
#include "RADIO.h"
#include "ROTARY.h"
#include "SCROLLBAR.h"
#include "SCROLLER.h"
#include "SLIDER.h"
#include "SPINBOX.h"
#include "SWIPELIST.h"
#include "SWITCH.h"
#include "TEXT.h"
#include "TICKER.h"
#include "TREEVIEW.h"
#include "WHEEL.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       WINDOW API
*/
WM_HWIN   WINDOW_CreateEx         (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK * cb);
WM_HWIN   WINDOW_CreateUser       (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, WM_CALLBACK * cb, int NumExtraBytes);
WM_HWIN   WINDOW_CreateIndirect   (const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);
GUI_COLOR WINDOW_GetBkColor       (WM_HWIN hObj);
GUI_COLOR WINDOW_GetDefaultBkColor(void);
int       WINDOW_GetUserData      (WM_HWIN hObj, void * pDest, int NumBytes);
void      WINDOW_SetBkColor       (WM_HWIN hObj, GUI_COLOR Color);
void      WINDOW_SetDefaultBkColor(GUI_COLOR Color);
int       WINDOW_SetUserData      (WM_HWIN hObj, const void * pSrc, int NumBytes);

void WINDOW_Callback(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // DIALOG_H

/*************************** End of file ****************************/
