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

std::vector<config_option> parseConfigFile(const char* path)
{
    std::vector<config_option> result;

    FILE* fp;

    if ((fp = fopen(path, "r+")) == NULL) {
        perror("fopen()");
        return result;
    }

    while (1) {
        result.emplace_back();
        auto& co = result.back();

        if (fscanf(fp, "%s = %s", &co.key[0], &co.value[0]) != 2) {
            if (feof(fp)) {
                break;
            }

            if (co.key[0] == '#') {
                while (fgetc(fp) != '\n') {
                    // Do nothing (to move the cursor to the end of the line).
                }
                result.pop_back();
                continue;
            }

            perror("fscanf()");
            result.pop_back();
            continue;
        }
    }

    fclose(fp);

    return result;
}


void readConfigFile(const char* config_file)
{
    const auto parsedConfig = parseConfigFile(config_file);
    for (const auto& co : parsedConfig) {
        if (strcmp(co.key, "back") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.back_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.back_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.back_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.back_modifier = KEY_LEFTSHIFT;
            } else {
                config.back = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "guide") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.guide_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.guide_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.guide_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.guide_modifier = KEY_LEFTSHIFT;
            } else {
                config.guide = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "start") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.start_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.start_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.start_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.start_modifier = KEY_LEFTSHIFT;
            } else {
                config.start = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "a") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.a_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.a_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.a_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.a_modifier = KEY_LEFTSHIFT;
            } else {
                config.a = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "a_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.a_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.a_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.a_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.a_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "b") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.b_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.b_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.b_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.b_modifier = KEY_LEFTSHIFT;
            } else {
                config.b = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "b_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.b_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.b_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.b_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.b_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "x") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.x_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.x_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.x_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.x_modifier = KEY_LEFTSHIFT;
            } else {
                config.x = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "x_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.x_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.x_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.x_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.x_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "y") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.y_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.y_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.y_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.y_modifier = KEY_LEFTSHIFT;
            } else {
                config.y = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "y_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.y_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.y_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.y_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.y_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "l1") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.l1_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.l1_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.l1_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.l1_modifier = KEY_LEFTSHIFT;
            } else {
                config.l1 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "l1_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.l1_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.l1_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.l1_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.l1_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "l2") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.l2_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.l2_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.l2_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.l2_modifier = KEY_LEFTSHIFT;
            } else {
                config.l2 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "l2_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.l2_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.l2_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.l2_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.l2_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "l3") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.l3_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.l3_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.l3_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.l3_modifier = KEY_LEFTSHIFT;
            } else {
                config.l3 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "r1") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.r1_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.r1_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.r1_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.r1_modifier = KEY_LEFTSHIFT;
            } else {
                config.r1 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "r1_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.r1_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.r1_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.r1_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.r1_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "r2") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.r2_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.r2_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.r2_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.r2_modifier = KEY_LEFTSHIFT;
            } else {
                config.r2 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "r2_hk") == 0) {
            if (strcmp(co.value, "add_alt") == 0) {
                config.r2_hk_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.r2_hk_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.r2_hk_modifier = KEY_LEFTSHIFT;
            } else {
                config.r2_hk = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "r3") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.r3_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.r3_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.r3_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.r3_modifier = KEY_LEFTSHIFT;
            } else {
                config.r3 = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "up") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.up_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.up_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.up_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.up_modifier = KEY_LEFTSHIFT;
            } else {
                config.up = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "down") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.down_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.down_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.down_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.down_modifier = KEY_LEFTSHIFT;
            } else {
                config.down = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "left") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.left_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.left_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.left_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.left_modifier = KEY_LEFTSHIFT;
            } else {
                config.left = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "right") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.right_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.right_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.right_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.right_modifier = KEY_LEFTSHIFT;
            } else {
                config.right = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "left_analog_up") == 0) {
          if (strcmp(co.value, "mouse_movement_up") == 0) {
            config.left_analog_as_mouse = true;
          } else {
            if (strcmp(co.value, "repeat") == 0) {
                config.left_analog_up_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.left_analog_up_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.left_analog_up_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.left_analog_up_modifier = KEY_LEFTSHIFT;
            } else {
                config.left_analog_up = char_to_keycode(co.value);
            }
          }
        } else if (strcmp(co.key, "left_analog_down") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.left_analog_down_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.left_analog_down_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.left_analog_down_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.left_analog_down_modifier = KEY_LEFTSHIFT;
            } else {
                config.left_analog_down = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "left_analog_left") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.left_analog_left_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.left_analog_left_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.left_analog_left_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.left_analog_left_modifier = KEY_LEFTSHIFT;
            } else {
                config.left_analog_left = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "left_analog_right") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.left_analog_right_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.left_analog_right_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.left_analog_right_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.left_analog_right_modifier = KEY_LEFTSHIFT;
            } else {
                config.left_analog_right = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "right_analog_up") == 0) {
            if (strcmp(co.value, "mouse_movement_up") == 0) {
                config.right_analog_as_mouse = true;
            } else {
                if (strcmp(co.value, "repeat") == 0) {
                    config.right_analog_up_repeat = true;
                } else if (strcmp(co.value, "add_alt") == 0) {
                    config.right_analog_up_modifier = KEY_LEFTALT;
                } else if (strcmp(co.value, "add_ctrl") == 0) {
                    config.right_analog_up_modifier = KEY_LEFTCTRL;
                } else if (strcmp(co.value, "add_shift") == 0) {
                    config.right_analog_up_modifier = KEY_LEFTSHIFT;
                } else {
                    config.right_analog_up = char_to_keycode(co.value);
                }
            }
        } else if (strcmp(co.key, "right_analog_down") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.right_analog_down_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.right_analog_down_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.right_analog_down_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.right_analog_down_modifier = KEY_LEFTSHIFT;
            } else {
                config.right_analog_down = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "right_analog_left") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.right_analog_left_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.right_analog_left_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.right_analog_left_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.right_analog_left_modifier = KEY_LEFTSHIFT;
            } else {
                config.right_analog_left = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "right_analog_right") == 0) {
            if (strcmp(co.value, "repeat") == 0) {
                config.right_analog_right_repeat = true;
            } else if (strcmp(co.value, "add_alt") == 0) {
                config.right_analog_right_modifier = KEY_LEFTALT;
            } else if (strcmp(co.value, "add_ctrl") == 0) {
                config.right_analog_right_modifier = KEY_LEFTCTRL;
            } else if (strcmp(co.value, "add_shift") == 0) {
                config.right_analog_right_modifier = KEY_LEFTSHIFT;
            } else {
                config.right_analog_right = char_to_keycode(co.value);
            }
        } else if (strcmp(co.key, "deadzone_y") == 0) {
            config.deadzone_y = atoi(co.value);
        } else if (strcmp(co.key, "deadzone_x") == 0) {
            config.deadzone_x = atoi(co.value);
        } else if (strcmp(co.key, "deadzone_triggers") == 0) {
            config.deadzone_triggers = atoi(co.value);
        } else if (strcmp(co.key, "mouse_scale") == 0) {
            config.fake_mouse_scale = atoi(co.value);
        } else if (strcmp(co.key, "mouse_delay") == 0) {
            config.fake_mouse_delay = atoi(co.value);
        } else if (strcmp(co.key, "repeat_delay") == 0) {
            config.key_repeat_delay = atoi(co.value);
        } else if (strcmp(co.key, "repeat_interval") == 0) {
            config.key_repeat_interval = atoi(co.value);
        } 
    }
}
