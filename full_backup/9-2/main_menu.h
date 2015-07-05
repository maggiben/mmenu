///////////////////////////////////////////////////////////////////////////////////
// Título:      		main_menu.c                                              //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		A customizable Menu                                      //
// Org. Date:           17/01/2005                                               //
// Last Modification:   13/03/2008                                               //
// Ver:                 0.9.1                                                    //
// compiler:            uses ansi-C / C99 tested with LCC & Pellesc              //
// Author mail:         benjaminmaggi@gmail.com                                  //
// License:             GNU                                                      //
//                                                                               //
// ***************************************************************************** //
// * Detais & compilating information                                          * //
// * I've compiled and linked this using lcc but should work in other c/c++    * //
// * compiler.                                                                 * //
// * be carful debugging the hooked function it may cause a GPF                * //
// * you can turn hooks off 									               * // 
// ***************************************************************************** //
//                                                                               //
// License:                                                                      //
// This program is free software; you can redistribute it                        //
// and/or modify it under the terms of the GNU General Public                    //
// License as published by the Free Software Foundation;                         //
// either version 2 of the License, or (at your option) any          	         //
// later version.                                                    	         //
//                                                                    	         //
// This program is distributed in the hope that it will be useful,   	         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    	         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     	         //
// GNU General Public License for more details.                      	         //
//                                                                               //
// ***************************************************************************** //
//                                                                               //
// CHANGE LOG:                                                                   //
//                                                                               //
// * Entry for: 4/12/2006                                                        //
//    Located some bugs free(menus) caused errros                                //
//    Working on MFT_SEPARATOR                                                   //
//    Heavy testing under XP moving from w98                                     //
//    Maybe Vista ?                                                              //
//    New features transparency ?                                                //
//    new icons are 24bpp + 256b alpha those need testing                        //
//    A faulty casting caused some bugs (HMENU) casted to -> (UINT) shit !       //
//                                                                               //
// * Entry for 11/DIC/2006                                                       //
//   * I've been thinking this a lot, and this proget needs to support more      //
//   image formats, the best lib i could find if freeImage, wich supports        //
//   more than 80 image formats, the pros are obious the bad thing is that       //
//   now we need to carry this heavy +1mb dll every were, it was not an easy     //
//   desition but finally i thought it was nessesary                             //
//   and in order to mantein compatibility i choose to write a function          //
//   to switch between components                                                //
//                                                                               //
//   * menu_imageOut & menu_imageOvr two new type of images one is for           //
//   menu items: selected and not.                                               //
//   * WM_MENUDRAG and WM_MENUGETOBJECT                                          //
//                                                                               //
// * Entry for 14/DIC/2006                                                       //
//	 * The fixed some GDI leaking problems                                       //
//                                                                               //
// * Entry for 17/DIC/2006                                                       //
//   * Added two new functions:                                                  //
//     insert_menuItem                                                           //
//	   append_menuItem                                                           //
//	   delete_menuItem                                                           //
//     These functions provide a standard framework to the application           //
//                                                                               //
// * Entry for 13/03/2008                                                        //
//   * Research on Drag&Drop                                                     //
//                                                                               //
// * Entry for 14/03/2008                                                        //
//   * Added CS_DROPSHADOW to MainForm                                           //
//                                                                               //
// * Entry for 18/03/2008                                                        //
//   * Working on MF_MENUBARBREAK                                                //
//                                                                               //
// * Entry for 20/03/2008                                                        //
//   * Working on MEASUREITEM                                                    //
//   * Rewrite the Sidebar code & draw functions                                 //
//   * Worked on Parse XML to proper detect item type                            //
//                                                                               //
// * Entry for 21/03/2008                                                        //
//   * Complete rewrite of the MSXML wrapper, drop disphelper                    //
//                                                                               //
// * Entry for 29/03/2008                                                        //
//   * Started creating the Menu Manager                                         //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_MENU_h__
#define __MAIN_MENU_h__

#ifndef __LCC__
#define __LCC__
#endif

BOOL create_Controls(HINSTANCE main_inst, HWND hwnd_parent, int skin_tpy);

LRESULT CALLBACK	ButtonProc					(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
RECT				MapDialogControlRect		(HWND, int, BOOL);

//BOOL LoadSong(HWND LB_Parent_Hwnd);

#endif
