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

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

enum DZ_MODE {
    DZ_DEFAULT,
    DZ_AXIAL,
    DZ_RADIAL,
    DZ_SCALED_RADIAL,
    DZ_SLOPED_AXIAL,
    DZ_SLOPED_SCALED_AXIAL,
    DZ_HYBRID,
};

struct GptokeybState
{
    int hotkey_jsdevice;
    int start_jsdevice;
    int textinputinteractivetrigger_jsdevice; // to trigger text input interactive
    int textinputpresettrigger_jsdevice; // to trigger text input preset
    int textinputconfirmtrigger_jsdevice; // to trigger text input confirm via Enter key
    int mouseX = 0;
    int mouseY = 0;
    int current_left_analog_x = 0;
    int current_left_analog_y = 0;
    int current_right_analog_x = 0;
    int current_right_analog_y = 0;
    int current_l2 = 0;
    int current_r2 = 0;
    bool hotkey_pressed = false; // current state of hotkey
    bool hotkey_was_pressed = false; // indicates hotkey button has been pressed, and key may need to be processed on button's release, if hotkey combo isn't triggered
    bool start_pressed = false;
    bool start_was_pressed = false; // indicates start button has been pressed, and key may need to be processed on button's release, if start combo isn't triggered
    bool textinputinteractive_mode_active = false;
    bool textinputinteractivetrigger_pressed = false;
    bool textinputpresettrigger_pressed = false;
    bool textinputconfirmtrigger_pressed = false;
    bool left_analog_was_up = false;
    bool left_analog_was_down = false;
    bool left_analog_was_left = false;
    bool left_analog_was_right = false;
    bool right_analog_was_up = false;
    bool right_analog_was_down = false;
    bool right_analog_was_left = false;
    bool right_analog_was_right = false;
    bool a_hk_was_pressed = false;
    bool b_hk_was_pressed = false;
    bool x_hk_was_pressed = false;
    bool y_hk_was_pressed = false;
    bool l1_hk_was_pressed = false;
    bool r1_hk_was_pressed = false;
    bool l2_was_pressed = false;
    bool l2_hk_was_pressed = false;
    bool r2_was_pressed = false;
    bool r2_hk_was_pressed = false;
    bool hotkey_combo_triggered = false; //keep track of whether a hotkey combo was pressed; if so, don't send hotkey key when hotkey is released
    bool start_combo_triggered = false; //keep track of whether a start combo was pressed; if so, don't send start key when start is released
    short key_to_repeat = 0;
    SDL_TimerID key_repeat_timer_id = 0;
};


struct GptokeybConfig
{
    short back = KEY_ESC;
    bool back_repeat = false;
    short back_modifier = 0;
    short start = KEY_ENTER;
    bool start_repeat = false;
    short start_modifier = 0;
    short guide = KEY_ENTER;
    bool guide_repeat = false;
    short guide_modifier = 0;
    short a = KEY_X;
    short a_hk = KEY_ENTER;
    bool a_repeat = false;
    short a_modifier = 0;
    short a_hk_modifier = 0;
    short b = KEY_Z;
    short b_hk = KEY_ESC;
    bool b_repeat = false;
    short b_modifier = 0;
    short b_hk_modifier = 0;
    short x = KEY_C;
    short x_hk = KEY_C;
    bool x_repeat = false;
    short x_modifier = 0;
    short x_hk_modifier = 0;
    short y = KEY_A;
    short y_hk = KEY_A;
    bool y_repeat = false;
    short y_modifier = 0;
    short y_hk_modifier = 0;
    short l1 = KEY_RIGHTSHIFT;
    short l1_hk = KEY_ESC;
    bool l1_repeat = false;
    short l1_modifier = 0;
    short l1_hk_modifier = 0;
    short l2 = KEY_HOME;
    short l2_hk = KEY_HOME;
    bool l2_repeat = false;
    short l2_modifier = 0;
    short l2_hk_modifier = 0;
    short l3 = BTN_LEFT;
    bool l3_repeat = false;
    short l3_modifier = 0;
    short r1 = KEY_LEFTSHIFT;
    short r1_hk = KEY_ENTER;
    bool r1_repeat = false;
    short r1_modifier = 0;
    short r1_hk_modifier = 0;
    short r2 = KEY_END;
    short r2_hk = KEY_END;
    bool r2_repeat = false;
    short r2_modifier = 0;
    short r2_hk_modifier = 0;
    short r3 = BTN_RIGHT;
    bool r3_repeat = false;
    short r3_modifier = 0;
    short up = KEY_UP;
    bool up_repeat = false;
    short up_modifier = 0;
    short down = KEY_DOWN;
    bool down_repeat = false;
    short down_modifier = 0;
    short left = KEY_LEFT;
    bool left_repeat = false;
    short left_modifier = 0;
    short right = KEY_RIGHT;
    bool right_repeat = false;
    short right_modifier = 0;

    bool left_analog_as_mouse = false;
    bool right_analog_as_mouse = false;
    short left_analog_up = KEY_W;
    bool left_analog_up_repeat = false;
    short left_analog_up_modifier = 0;
    short left_analog_down = KEY_S;
    bool left_analog_down_repeat = false;
    short left_analog_down_modifier = 0;
    short left_analog_left = KEY_A;
    bool left_analog_left_repeat = false;
    short left_analog_left_modifier = 0;
    short left_analog_right = KEY_D;
    bool left_analog_right_repeat = false;
    short left_analog_right_modifier = 0;
    short right_analog_up = KEY_END;
    bool right_analog_up_repeat = false;
    short right_analog_up_modifier = 0;
    short right_analog_down = KEY_HOME;
    bool right_analog_down_repeat = false;
    short right_analog_down_modifier = 0;
    short right_analog_left = KEY_LEFT;
    bool right_analog_left_repeat = false;
    short right_analog_left_modifier = 0;
    short right_analog_right = KEY_RIGHT;
    bool right_analog_right_repeat = false;
    short right_analog_right_modifier = 0;

    DZ_MODE deadzone_mode = DZ_DEFAULT;
    int deadzone_scale = 512;
    int deadzone   = 15000;
    int deadzone_y = 15000;
    int deadzone_x = 15000;
    int deadzone_triggers = 3000;

    int fake_mouse_scale = 512;
    int fake_mouse_delay = 16;

    Uint32 key_repeat_interval = SDL_DEFAULT_REPEAT_INTERVAL * 2; 
    Uint32 key_repeat_delay = SDL_DEFAULT_REPEAT_DELAY; 

    char* text_input_preset;
};


struct config_option
{
    char key[CONFIG_ARG_MAX_BYTES];
    char value[CONFIG_ARG_MAX_BYTES];
};

#endif /* __STRUCTS_H__ */
