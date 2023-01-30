/* Copyright (c) 2021-2023
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
#
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
#
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301 USA
#
* Authored by: Kris Henriksen <krishenriksen.work@gmail.com>
#
* AnberPorts-Keyboard-Mouse
* 
* Part of the code is from from https://github.com/krishenriksen/AnberPorts/blob/master/AnberPorts-Keyboard-Mouse/main.c (mostly the fake keyboard)
* Fake Xbox code from: https://github.com/Emanem/js2xbox
* 
* Modified (badly) by: Shanti Gilbert for EmuELEC
* Modified further by: Nikolai Wuttke for EmuELEC (Added support for SDL and the SDLGameControllerdb.txt)
* Modified further by: Jacob Smith
* 
* Any help improving this code would be greatly appreciated! 
* 
* DONE: Xbox360 mode: Fix triggers so that they report from 0 to 255 like real Xbox triggers
*       Xbox360 mode: Figure out why the axis are not correctly labeled?  SDL_CONTROLLER_AXIS_RIGHTX / SDL_CONTROLLER_AXIS_RIGHTY / SDL_CONTROLLER_AXIS_TRIGGERLEFT / SDL_CONTROLLER_AXIS_TRIGGERRIGHT
*       Keyboard mode: Add a config file option to load mappings from.
*       add L2/R2 triggers
* 
*/

#include "gptokeyb.h"

const int maxKeysNoExtendedSymbols = 69;        //number of keys available for interactive text input
const int maxKeysWithSymbols = 96;              //number of keys available for interactive text input with extra symbols
int maxKeys = maxKeysNoExtendedSymbols;
const int maxChars = 20;                        // length of text in characters that can be entered
int character_set[maxKeysWithSymbols];          // keys that can be selected in text input interactive mode
bool character_set_shift[maxKeysWithSymbols];   // indicate which keys require shift

int current_character = 0; 
int current_key[maxChars];                      // current key selected for each key


void initialiseCharacters()
{
  if (textinputinteractive_noautocapitals) {
    current_key[0] = 26; // if environment variable has been set to disable capitalisation of first characters start with all lower case  
  } else {
    current_key[0] = 0; // otherwise start with upper case for 1st character
  }
  for (int ii = 1; ii < maxChars; ii++) { // start with lower case for other character onwards
    current_key[ii] = 26;
  }

}

void initialiseCharacterSet()
{
  character_set[0]=char_to_keycode("a"); //capital letters
  character_set_shift[0]=true;
  character_set[1]=char_to_keycode("b");
  character_set_shift[1]=true;
  character_set[2]=char_to_keycode("c");
  character_set_shift[2]=true;
  character_set[3]=char_to_keycode("d");
  character_set_shift[3]=true;
  character_set[4]=char_to_keycode("e");
  character_set_shift[4]=true;
  character_set[5]=char_to_keycode("f");
  character_set_shift[5]=true;
  character_set[6]=char_to_keycode("g");
  character_set_shift[6]=true;
  character_set[7]=char_to_keycode("h");
  character_set_shift[7]=true;
  character_set[8]=char_to_keycode("i");
  character_set_shift[8]=true;
  character_set[9]=char_to_keycode("j");
  character_set_shift[9]=true;
  character_set[10]=char_to_keycode("k");
  character_set_shift[10]=true;
  character_set[11]=char_to_keycode("l");
  character_set_shift[11]=true;
  character_set[12]=char_to_keycode("m");
  character_set_shift[12]=true;
  character_set[13]=char_to_keycode("n");
  character_set_shift[13]=true;
  character_set[14]=char_to_keycode("o");
  character_set_shift[14]=true;
  character_set[15]=char_to_keycode("p");
  character_set_shift[15]=true;
  character_set[16]=char_to_keycode("q");
  character_set_shift[16]=true;
  character_set[17]=char_to_keycode("r");
  character_set_shift[17]=true;
  character_set[18]=char_to_keycode("s");
  character_set_shift[18]=true;
  character_set[19]=char_to_keycode("t");
  character_set_shift[19]=true;
  character_set[20]=char_to_keycode("u");
  character_set_shift[20]=true;
  character_set[21]=char_to_keycode("v");
  character_set_shift[21]=true;
  character_set[22]=char_to_keycode("w");
  character_set_shift[22]=true;
  character_set[23]=char_to_keycode("x");
  character_set_shift[23]=true;
  character_set[24]=char_to_keycode("y");
  character_set_shift[24]=true;
  character_set[25]=char_to_keycode("z");
  character_set_shift[25]=true;
  character_set[26]=char_to_keycode("a"); //lower case
  character_set_shift[26]=false;
  character_set[27]=char_to_keycode("b");
  character_set_shift[27]=false;
  character_set[28]=char_to_keycode("c");
  character_set_shift[28]=false;
  character_set[29]=char_to_keycode("d");
  character_set_shift[29]=false;
  character_set[30]=char_to_keycode("e");
  character_set_shift[30]=false;  
  character_set[31]=char_to_keycode("f");
  character_set_shift[31]=false;
  character_set[32]=char_to_keycode("g");
  character_set_shift[32]=false;
  character_set[33]=char_to_keycode("h");
  character_set_shift[33]=false;
  character_set[34]=char_to_keycode("i");
  character_set_shift[34]=false;
  character_set[35]=char_to_keycode("j");
  character_set_shift[35]=false;
  character_set[36]=char_to_keycode("k");
  character_set_shift[36]=false;
  character_set[37]=char_to_keycode("l");
  character_set_shift[37]=false;
  character_set[38]=char_to_keycode("m");
  character_set_shift[38]=false;
  character_set[39]=char_to_keycode("n");
  character_set_shift[39]=false;
  character_set[40]=char_to_keycode("o");
  character_set_shift[40]=false;
  character_set[41]=char_to_keycode("p");
  character_set_shift[41]=false;
  character_set[42]=char_to_keycode("q");
  character_set_shift[42]=false;
  character_set[43]=char_to_keycode("r");
  character_set_shift[43]=false;
  character_set[44]=char_to_keycode("s");
  character_set_shift[44]=false;
  character_set[45]=char_to_keycode("t");
  character_set_shift[45]=false;
  character_set[46]=char_to_keycode("u");
  character_set_shift[46]=false;
  character_set[47]=char_to_keycode("v");
  character_set_shift[47]=false;
  character_set[48]=char_to_keycode("w");
  character_set_shift[48]=false;
  character_set[49]=char_to_keycode("x");
  character_set_shift[49]=false;
  character_set[50]=char_to_keycode("y");
  character_set_shift[50]=false;
  character_set[51]=char_to_keycode("z");
  character_set_shift[51]=false;
  character_set[52]=char_to_keycode("0");
  character_set_shift[52]=false;
  character_set[53]=char_to_keycode("1");
  character_set_shift[53]=false;
  character_set[54]=char_to_keycode("2");
  character_set_shift[54]=false;
  character_set[55]=char_to_keycode("3");
  character_set_shift[55]=false;
  character_set[56]=char_to_keycode("4");
  character_set_shift[56]=false;
  character_set[57]=char_to_keycode("5");
  character_set_shift[57]=false;
  character_set[58]=char_to_keycode("6");
  character_set_shift[58]=false;
  character_set[59]=char_to_keycode("7"); 
  character_set_shift[59]=false;
  character_set[60]=char_to_keycode("8"); 
  character_set_shift[60]=false;
  character_set[61]=char_to_keycode("9"); 
  character_set_shift[61]=false;
  character_set[62]=char_to_keycode("space"); 
  character_set_shift[62]=false;
  character_set[63]=char_to_keycode("."); 
  character_set_shift[63]=false;
  character_set[64]=char_to_keycode(","); 
  character_set_shift[64]=false;
  character_set[65]=char_to_keycode("-"); 
  character_set_shift[65]=false;
  character_set[66]=char_to_keycode("_"); 
  character_set_shift[66]=true;
  character_set[67]=char_to_keycode("("); 
  character_set_shift[67]=true;
  character_set[68]=char_to_keycode(")");  
  character_set_shift[68]=true;

  if (textinputinteractive_extrasymbols) {
    maxKeys = maxKeysWithSymbols;
    character_set[69]=char_to_keycode("@");  
    character_set_shift[69]=true;
    character_set[70]=char_to_keycode("#");  
    character_set_shift[70]=true;
    character_set[71]=char_to_keycode("%");  
    character_set_shift[71]=true;
    character_set[72]=char_to_keycode("&");  
    character_set_shift[72]=true;
    character_set[73]=char_to_keycode("*");  
    character_set_shift[73]=true;
    character_set[74]=char_to_keycode("-");  
    character_set_shift[74]=false;
    character_set[75]=char_to_keycode("+");  
    character_set_shift[75]=true;
    character_set[76]=char_to_keycode("!");  
    character_set_shift[76]=true;
    character_set[77]=char_to_keycode("\"");  
    character_set_shift[77]=true;
    character_set[78]=char_to_keycode("\'");  
    character_set_shift[78]=false;
    character_set[79]=char_to_keycode(":");  
    character_set_shift[79]=true;
    character_set[80]=char_to_keycode(";");  
    character_set_shift[80]=false;
    character_set[81]=char_to_keycode("/");  
    character_set_shift[81]=false;
    character_set[82]=char_to_keycode("?");  
    character_set_shift[82]=true;
    character_set[83]=char_to_keycode("~");  
    character_set_shift[83]=true;
    character_set[84]=char_to_keycode("`");  
    character_set_shift[84]=false;
    character_set[85]=char_to_keycode("|");  
    character_set_shift[85]=true;
    character_set[86]=char_to_keycode("{");  
    character_set_shift[86]=true;
    character_set[87]=char_to_keycode("}");  
    character_set_shift[87]=true;
    character_set[88]=char_to_keycode("$");  
    character_set_shift[88]=true;
    character_set[89]=char_to_keycode("^");  
    character_set_shift[89]=true;
    character_set[90]=char_to_keycode("=");  
    character_set_shift[90]=false;
    character_set[91]=char_to_keycode("[");  
    character_set_shift[91]=false;
    character_set[92]=char_to_keycode("]");  
    character_set_shift[92]=false;
    character_set[93]=char_to_keycode("\\");  
    character_set_shift[93]=false;
    character_set[94]=char_to_keycode("<");  
    character_set_shift[94]=true;
    character_set[95]=char_to_keycode(">");  
    character_set_shift[95]=true;
  }
  initialiseCharacters();
}
