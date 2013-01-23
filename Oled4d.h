/*
    Oled4d.h - Oled 4D System libray for Arduino
    Copyright (c) 2009 Charles Rincheval.  All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    The latest version of this library can always be found at
    http://www.digitalspirit.org/ 
*/

#define ENABLE_NEW_SOFT_SERIAL

#ifndef Oled4d_h
#define Oled4d_h

#include <wiring.h>

#include "HardwareSerial.h"
#ifdef ENABLE_NEW_SOFT_SERIAL
#include "NewSoftSerial.h"
#endif

//#define OLED4D_RESET_PIN        8
#define OLED4D_DEBUG_PIN        13
#define OLED4D_INIT_DELAY_MS    2000

#define OLED4D_ACK    0x06
#define OLED4D_NACK   0x15

#define OLED4D_OUTPUT_SERIAL_ONLY   0
#define OLED4D_OUTPUT_SERIAL_SCREEN 1

#define OLED4D_MIN_VERTICES     3
#define OLED4D_MAX_VERTICES     7

#define RGB(red, green, blue)   (((red * 31) / 255) << 11) | (((green * 63) / 255) << 5) | ((blue * 31) / 255)

// Command
enum OLED4D_CMD {
    CMD_UNKNOW                  = 0,

    CMD_DETECT_BAUDRATE         = 0x55,

    CMD_ADD_USER_BMP_CHAR       = 'A',
    CMD_SET_BG_COLOR            = 'B',
    CMD_PLACE_TEXT_BUTTON       = 'b',
    CMD_DRAW_CIRCLE             = 'C',
    CMD_BLOCK_COPY_PASTE        = 'c',
    CMD_DISPLAY_USER_BMP_CHAR   = 'D',
    CMD_ERASE_SCREEN            = 'E',
    CMD_FONT                    = 'F',
    CMD_DRAW_TRIANGLE           = 'G',
    CMD_DRAW_POLYGON            = 'g',
    CMD_DISPLAY_IMAGE           = 'I',
    CMD_REPLACE_COLOR         = 'k',
    CMD_DRAW_LINE               = 'L',
    CMD_OPAQUE_TRANSPARENT_TXT  = 'O',
    CMD_PUT_PIXEL               = 'P',
    CMD_SET_PEN_SIZE            = 'p',
    CMD_READ_PIXEL              = 'R',
    CMD_DRAW_RECTANGLE          = 'r',
    CMD_PLACE_STR_ASCII_TXT     = 'S',
    CMD_PLACE_STR_ASCII_TXT_F   = 's',
    CMD_PLACE_TEXT_CHAR         = 'T',
    CMD_PLACE_TEXT_CHAR_F       = 't',
    CMD_VERSION_DEVICE_INFO     = 'V',
    CMD_DISPLAY_CONTROL         = 'Y',

    CMD_SPECIFIC_COMMAND        = '$',
    CMD_EXTENDED_COMMAND        = '@',
//};

// Specific command
//enum OLED4D_CMD_SPECIFIC {
    CMD_S_SCROLL_CONTROL        = 'S',
    CMD_S_DIM_SCREEN_AREA       = 'D',
//};

// Extended command
//enum OLED4D_CMD_EXT {
    CMD_E_INIT_SDCARD           = 'i',
    CMD_E_READ_SECTOR           = '-',
    CMD_E_WRITE_SECTOR          = '-',
    CMD_E_READ_BYTE             = '-',
    CMD_E_WRITE_BYTE            = '-',
    CMD_E_SET_ADRESS            = '-',
    CMD_E_COPY_SCREEN_TO_SD     = 'C',
    CMD_E_DISPLAY_IMG_FROM_SD   = 'I',
};

typedef int             OLED4D_COLOR;
typedef unsigned char   uchar;

enum OLED4D_FONT {
    FONT_5X7 = 0,
    FONT_8X8,
    FONT_8X12
};

enum OLED4D_BUTTONSTATE {
    BUTTON_DOWN = 0,
    BUTTON_UP
};

enum OLED4D_PENSIZE {
    PENSIZE_SOLID = 0,
    PENSIZE_WIREFRAME
};

enum OLED4D_TEXTAPPEARANCE {
    TEXTAPPEARANCE_TRANSPARENT = 0,
    TEXTAPPEARANCE_OPAQUE
};

enum OLED4D_COLORMODE {
    COLORMODE_256 = 8,
    COLORMODE_65K = 16
};

enum OLED4D_CONTROLDISPLAY {
    CONTROLDISPLAY_ON_OFF           = 1,
    CONTROLDISPLAY_SET_CONTRAST     = 2,
    CONTROLDISPLAY_POWERUP_SHUTDOWN = 3,
};

enum OLED4D_SCROLL_SPEED {
    SCROLL_SPEED_NONE = 0,
    SCROLL_SPEED_FAST,
    SCROLL_SPEED_NORMAL,
    SCROLL_SPEED_SLOW,
};

struct Oled4d_info {
    int device_type;
    int hardware_rev;
    int firmware_rev;
    int horizontal_res;
    int vertical_res;
};

#define SPLIT(w)    highByte(w), lowByte(w)


template <class T>
class Oled4d
{
private:
    T _serial;
    unsigned char _reset_pin;
    int _last_status_code;

public:
    Oled4d(HardwareSerial &, unsigned char);
#ifdef ENABLE_NEW_SOFT_SERIAL
    Oled4d(NewSoftSerial &, unsigned char);
#endif

    // System
    char command(OLED4D_CMD, char[], int, char*);

    bool getLastStatus();
    int debugStatus();

    void init();
    void reset();
    int getResponse();

    // Misc
    void displayControl(OLED4D_CONTROLDISPLAY, char);
    void displayOn()                            { displayControl(CONTROLDISPLAY_ON_OFF, true); };
    void displayOff()                           { displayControl(CONTROLDISPLAY_ON_OFF, false); };
    void displayPowerUp()                       { displayControl(CONTROLDISPLAY_POWERUP_SHUTDOWN, true); };
    void displayShutdown()                      { displayControl(CONTROLDISPLAY_POWERUP_SHUTDOWN, false); };
    void setContrast(unsigned char contrast)    { displayControl(CONTROLDISPLAY_SET_CONTRAST, contrast); };

    void getInfo(bool, Oled4d_info &);

    void clear();
    OLED4D_COLOR getRGB(int, int, int);

    // Text
    void setFont(OLED4D_FONT);
    void drawText(char, char, OLED4D_FONT, OLED4D_COLOR, char *);
    void drawString(char, char, char, OLED4D_COLOR); 
    void setTextAppearance(OLED4D_TEXTAPPEARANCE);

    void textButton(OLED4D_BUTTONSTATE, char, char, OLED4D_COLOR, OLED4D_FONT, OLED4D_COLOR, char, char, char *);

    // Graphic
    void setBG(OLED4D_COLOR);
    void drawLine(char, char, char, char, OLED4D_COLOR);
    void putPixel(char, char, OLED4D_COLOR);

    void drawCircle(char, char, char, OLED4D_COLOR);
    void drawTriangle(char, char, char, char, char, char, OLED4D_COLOR);
    void setPenSize(OLED4D_PENSIZE);
    void screenCopyPaste(char, char, char, char, char, char);
    void drawRectangle(char, char, char, char, OLED4D_COLOR);
    OLED4D_COLOR readPixel(char, char);

    void drawPolygon(char, char *, OLED4D_COLOR);
    char displayImage(char, char, char, char, OLED4D_COLORMODE, char *);
    void replaceColor(char, char, char, char, OLED4D_COLOR, OLED4D_COLOR);

    // User bitmap
    void addBmpChar(char, char[]);
    void displayBmpChar(char, char, char, OLED4D_COLOR);

    // Debug purpose...
    void sendUnknowOp();

    // Specific command
    void writeOledRegister();
    void scrollScreen(OLED4D_SCROLL_SPEED);
    void dimScreenArea(char, char, char, char); // Don't work...

    // Extended command
    void copyScreenToSD(char, char, char, char, char, char, char);
    void displayImgFromSD(char, char, char, char, OLED4D_COLORMODE, char, char, char);
};

#define Oled4dHard Oled4d<HardwareSerial>

#ifdef ENABLE_NEW_SOFT_SERIAL
#define Oled4dSoft Oled4d<NewSoftSerial>
#endif

//  Send opcode
#define OLED4D_SEND(op)                     command(op, NULL, 0, NULL);
#define OLED4D_SEND_C(op, param)            command(op, param, 1, NULL);
#define OLED4D_SEND_I(op, param)            char _args[] = { SPLIT(param) }; command(op, _args, 2, NULL);
#define OLED4D_SEND_A(op, param)            command(op, param, sizeof(param), NULL);
#define OLED4D_SEND_A_S(op, param, size)    command(op, param, size, NULL);

template <class T>
Oled4d<T>::Oled4d(HardwareSerial &serial, unsigned char reset_pin) :
    _serial(serial),
    _reset_pin(reset_pin)
{
    _last_status_code = 0;
    pinMode(reset_pin, OUTPUT);
};

template <class T>
Oled4d<T>::Oled4d(NewSoftSerial &serial, unsigned char reset_pin) :
    _serial(serial),
    _reset_pin(reset_pin)
{
    _last_status_code = 0;
    pinMode(reset_pin, OUTPUT);
};

template <class T>
void Oled4d<T>::sendUnknowOp() {
    OLED4D_SEND(CMD_UNKNOW);
}

template <class T>
int Oled4d<T>::debugStatus()
{
    return _last_status_code;
}

template <class T>
bool Oled4d<T>::getLastStatus()
{
    return (_last_status_code == OLED4D_ACK) ? true : false;
}

//  Send opcode with array and string
template <class T>
char Oled4d<T>::command(OLED4D_CMD op, char args[], int narg, char * str)
{
    int i = 0;

    _serial.print(op, BYTE);

    for (; i < narg; i++)
        _serial.print(args[i], BYTE);

    if (str) {
        do {
            _serial.print(*str, BYTE);
            str++;
        } while (*str);
        _serial.print(0x00, BYTE);
    }

    delay(20);
    _last_status_code = getResponse();
    return _last_status_code;
}

//  Hard reset
template <class T>
void Oled4d<T>::reset()
{
    digitalWrite(_reset_pin, LOW);
    delay(20);
    digitalWrite(_reset_pin, HIGH);
    delay(20);
}

template <class T>
void Oled4d<T>::init()
{
    unsigned int status = OLED4D_NACK;

    reset();

    delay(OLED4D_INIT_DELAY_MS);

    OLED4D_SEND(CMD_DETECT_BAUDRATE);
}

//  Get response from Oled screen
template <class T>
int Oled4d<T>::getResponse()
{
    while (!_serial.available());
    return _serial.read();
}

template <class T>
void Oled4d<T>::displayControl(OLED4D_CONTROLDISPLAY control, char value)
{
    char args[] = { control, value };
    OLED4D_SEND_A(CMD_DISPLAY_CONTROL, args);
}

// Get 16bits value from RGB (0 to 63, 565 format)
template <class T>
int Oled4d<T>::getRGB(int red, int green, int blue)
{
	int outR = (red * 31) / 255;
	int outG = (green * 63) / 255;
	int outB = (blue * 31) / 255;
	return (outR << 11) | (outG << 5) | outB;
}

template <class T>
void Oled4d<T>::getInfo(bool output, Oled4d_info &info)
{
    char args[] = { (output ? OLED4D_OUTPUT_SERIAL_SCREEN : OLED4D_OUTPUT_SERIAL_ONLY) };
    OLED4D_SEND_A(CMD_VERSION_DEVICE_INFO, args);

    _serial.print(CMD_VERSION_DEVICE_INFO, BYTE);
    _serial.print((output ? OLED4D_OUTPUT_SERIAL_SCREEN : OLED4D_OUTPUT_SERIAL_ONLY), BYTE);

    info.device_type = _serial.read();
    info.hardware_rev = _serial.read();
    info.firmware_rev = _serial.read();
    info.horizontal_res = _serial.read();
    info.vertical_res = _serial.read();
}

template <class T>
void Oled4d<T>::setFont(OLED4D_FONT font)
{
    OLED4D_SEND_I(CMD_FONT, font);
}

template <class T>
void Oled4d<T>::drawText(char column, char row, OLED4D_FONT font, OLED4D_COLOR color, char *text)
{
    char args[] = { column, row, font, SPLIT(color) };
    command(CMD_PLACE_STR_ASCII_TXT_F, args, sizeof(args), text);
}

template <class T>
void Oled4d<T>::drawString(char str, char column, char row, OLED4D_COLOR color)
{
    char args[] = { str, column, row, SPLIT(color) };
    command(CMD_PLACE_TEXT_CHAR, args, sizeof(args));
}

template <class T>
void Oled4d<T>::setTextAppearance(OLED4D_TEXTAPPEARANCE appearance)
{
    OLED4D_SEND_I(CMD_OPAQUE_TRANSPARENT_TXT, appearance);
}

template <class T>
void Oled4d<T>::textButton( OLED4D_BUTTONSTATE state, char x, char y, OLED4D_COLOR button_color,
                            OLED4D_FONT font,
                            OLED4D_COLOR text_color, char width, char height, char * text)
{
    char args[] = { state, x, y, SPLIT(button_color), font, SPLIT(text_color), width, height };
    command(CMD_PLACE_TEXT_BUTTON, args, sizeof(args), text);
}



template <class T>
void Oled4d<T>::drawTriangle(char x1, char y1, char x2, char y2, char x3, char y3, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, x3, y3, SPLIT(color) };
    OLED4D_SEND_A(CMD_DRAW_TRIANGLE, args);
}

template <class T>
void Oled4d<T>::drawRectangle(char x1, char y1, char x2, char y2, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, SPLIT(color) };
    OLED4D_SEND_A(CMD_DRAW_RECTANGLE, args);
}

template <class T>
void Oled4d<T>::drawPolygon(char count, char * coord, OLED4D_COLOR color)
{
    unsigned char size = count * 2;
    unsigned char c = 0;
    char * args = (char *)malloc(size + 3);

    // Oled4d limit
    if (count <= OLED4D_MIN_VERTICES || count > OLED4D_MAX_VERTICES) {
        return;
    }

    args[c] = count;

    for (c++; c < size; c++) {
        args[c] = *coord++;
    }

    args[c + 1] = highByte(color);
    args[c + 2] = lowByte(color);

    OLED4D_SEND_A_S(CMD_DRAW_POLYGON, args, size + 3); 
}

template <class T>
char Oled4d<T>::displayImage(char x, char y, char width, char height, OLED4D_COLORMODE color_mode, char * data)
{
    unsigned int i, count = 0;

    _serial.print(CMD_DISPLAY_IMAGE, BYTE);

    _serial.print(x, BYTE);
    _serial.print(y, BYTE);
    _serial.print(width, BYTE);
    _serial.print(height, BYTE);

    _serial.print(color_mode, BYTE);

    count = width * height;
    if (color_mode == COLORMODE_65K) {
        count = count * 2;
    }
    
    for (; i < count; i++) {
        _serial.print(data[i], BYTE);
    }

    delay(20);
    return getResponse();
}

template <class T>
void Oled4d<T>::setPenSize(OLED4D_PENSIZE pensize)
{
    OLED4D_SEND_I(CMD_SET_PEN_SIZE, pensize);
}

template <class T>
void Oled4d<T>::screenCopyPaste(char xs, char ys, char xd, char yd, char width, char height)
{
    char args[] = { xs, ys, xd, yd, width, height };
    OLED4D_SEND_A(CMD_BLOCK_COPY_PASTE, args);
}

template <class T>
void Oled4d<T>::drawCircle(char x, char y, char radius, OLED4D_COLOR color)
{
    char args[] = { x, y, radius, SPLIT(color) };
    OLED4D_SEND_A(CMD_DRAW_CIRCLE, args);
}

template <class T>
OLED4D_COLOR Oled4d<T>::readPixel(char x, char y)
{
    OLED4D_COLOR color;
    _serial.print(CMD_READ_PIXEL, BYTE);
    _serial.print(x, BYTE);
    _serial.print(y, BYTE);

    while (!_serial.available());

    color = color << _serial.read();    // Todo: tester !
    color = _serial.read() & 0xF;

    return color;
}

//  Send bitmap char (8 char) to index
template <class T>
void Oled4d<T>::addBmpChar(char index, char bitmap[])
{
    char c = 0;
    char args[9];

    // Prepare arg
    args[0] = index; 
    for (c = 0; c < 8; c++) {
        args[c + 1] = bitmap[c];
    }

    OLED4D_SEND_A(CMD_ADD_USER_BMP_CHAR, args);
}

//  Display bitmap char from index
template <class T>
void Oled4d<T>::displayBmpChar(char index, char x, char y, OLED4D_COLOR color)
{
    char args[] = { index, x, y, SPLIT(color) };
    OLED4D_SEND_A(CMD_DISPLAY_USER_BMP_CHAR, args);
}

//  Clear screen
template <class T>
void Oled4d<T>::clear()
{
    OLED4D_SEND(CMD_ERASE_SCREEN);
}


template <class T>
void Oled4d<T>::drawLine(char x1, char y1, char x2, char y2, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, SPLIT(color) };
    OLED4D_SEND_A(CMD_DRAW_LINE, args);
}


template <class T>
void Oled4d<T>::putPixel(char x, char y, OLED4D_COLOR color)
{
    char args[] = { x, y, SPLIT(color) };
    OLED4D_SEND_A(CMD_PUT_PIXEL, args);
}

template <class T>
void Oled4d<T>::setBG(int color)
{
    OLED4D_SEND_I(CMD_SET_BG_COLOR, color)
}

template <class T>
void Oled4d<T>::dimScreenArea(char column, char row, char width, char height) {
    char args[] = { CMD_S_DIM_SCREEN_AREA, column, row, width, height };
    OLED4D_SEND_A(CMD_SPECIFIC_COMMAND, args);
}

template <class T>
void Oled4d<T>::scrollScreen(OLED4D_SCROLL_SPEED speed) {

    char _nabled[] = { CMD_S_SCROLL_CONTROL, 0, 0 };
    char scroll_speed[] = { CMD_S_SCROLL_CONTROL, 2, speed };

    if (speed) {
        _nabled[2] = 1;
    }

    OLED4D_SEND_A(CMD_SPECIFIC_COMMAND, _nabled);

    if (speed) {
        OLED4D_SEND_A(CMD_SPECIFIC_COMMAND, scroll_speed);
    }
}

template <class T>
void Oled4d<T>::copyScreenToSD(char x, char y, char width, char height, char sector0, char sector1, char sector2)
{
    char args[] = { CMD_E_COPY_SCREEN_TO_SD, x, y, width, height, sector0, sector1, sector2 };
    OLED4D_SEND_A(CMD_EXTENDED_COMMAND, args);
}

template <class T>
void Oled4d<T>::displayImgFromSD(char x, char y, char width, char height, OLED4D_COLORMODE color_mode, char sector0, char sector1, char sector2)
{
    char args[] = { CMD_E_DISPLAY_IMG_FROM_SD, x, y, width, height, color_mode, sector0, sector1, sector2 };
    OLED4D_SEND_A(CMD_EXTENDED_COMMAND, args);
}

template <class T>
void Oled4d<T>::replaceColor(char x1, char y1, char x2, char y2, OLED4D_COLOR old_color, OLED4D_COLOR new_color)
{
    char args[] = { x1, y1, x2, y2, SPLIT(old_color), SPLIT(new_color) };
    OLED4D_SEND_A(CMD_REPLACE_COLOR, args);
}

#endif
