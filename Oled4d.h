/*
    Oled4d.cpp - Oled 4D System libray for Arduino
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

enum OLED4D_OP {
    OP_UNKNOW                  = 0,

    OP_DETECT_BAUDRATE         = 0x55,

    OP_ADD_USER_BMP_CHAR       = 'A',
    OP_SET_BG_COLOR            = 'B',
    OP_PLACE_TEXT_BUTTON       = 'b',
    OP_DRAW_CIRCLE             = 'C',
    OP_BLOCK_COPY_PASTE        = 'c',
    OP_DISPLAY_USER_BMP_CHAR   = 'D',
    OP_ERASE_SCREEN            = 'E',
    OP_FONT                    = 'F',
    OP_DRAW_TRIANGLE           = 'G',
    OP_DRAW_POLYGON            = 'g',
    OP_DISPLAY_IMAGE           = 'I',
    OP_DRAW_LINE               = 'L',
    OP_OPAQUE_TRANSPARENT_TXT  = 'O',
    OP_PUT_PIXEL               = 'P',
    OP_SET_PEN_SIZE            = 'p',
    OP_READ_PIXEL              = 'R',
    OP_DRAW_RECTANGLE          = 'r',
    OP_PLACE_STR_ASCII_TXT     = 'S',
    OP_PLACE_STR_ASCII_TXT_F   = 's',
    OP_PLACE_TEXT_CHAR         = 'T',
    OP_PLACE_TEXT_CHAR_F       = 't',
    OP_VERSION_DEVICE_INFO     = 'V',
    OP_DISPLAY_CONTROL         = 'Y',
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
    COLORMODE_256 = 0x08,
    COLORMODE_65K = 0x10
};

enum OLED4D_CONTROLDISPLAY {
    CONTROLDISPLAY_ON_OFF           = 1,
    CONTROLDISPLAY_SET_CONTRAST     = 2,
    CONTROLDISPLAY_POWERUP_SHUTDOWN = 3,
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
    char command(OLED4D_OP, char[], int, char*);

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

    // User bitmap
    void addBmpChar(char, char[]);
    void displayBmpChar(char, char, char, OLED4D_COLOR);

    // Debug purpose...
    void sendUnknowOp();
};

#define Oled4dHard Oled4d<HardwareSerial>

#ifdef ENABLE_NEW_SOFT_SERIAL
#define Oled4dSoft Oled4d<NewSoftSerial>
#endif

//  Send opcode
#define CMD(op)                     command(op, NULL, 0, NULL);
#define CMD_C(op, param)            command(op, param, 1, NULL);
#define CMD_I(op, param)            char _args[] = { SPLIT(param) }; command(op, _args, 2, NULL);
#define CMD_A(op, param)            command(op, param, sizeof(param), NULL);
#define CMD_A_S(op, param, size)    command(op, param, size, NULL);

template <class T>
void Oled4d<T>::sendUnknowOp() {
    CMD(OP_UNKNOW);
}

template <class T>
Oled4d<T>::Oled4d(HardwareSerial &serial, unsigned char reset_pin) :
    _serial(serial),
    _reset_pin(reset_pin)
{
    _last_status_code = 0;
    pinMode(8, OUTPUT);
};

template <class T>
Oled4d<T>::Oled4d(NewSoftSerial &serial, unsigned char reset_pin) :
    _serial(serial),
    _reset_pin(reset_pin)
{
    _last_status_code = 0;
    pinMode(8, OUTPUT);
};

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
char Oled4d<T>::command(OLED4D_OP op, char args[], int narg, char * str)
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

    CMD(OP_DETECT_BAUDRATE);
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
    CMD_A(OP_DISPLAY_CONTROL, args);
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
    CMD_A(OP_VERSION_DEVICE_INFO, args);

    _serial.print(OP_VERSION_DEVICE_INFO, BYTE);
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
    command(OP_FONT, font);
}

template <class T>
void Oled4d<T>::drawText(char column, char row, OLED4D_FONT font, OLED4D_COLOR color, char *text)
{
    char args[] = { column, row, font, SPLIT(color) };
    command(OP_PLACE_STR_ASCII_TXT_F, args, sizeof(args), text);
}

template <class T>
void Oled4d<T>::drawString(char str, char column, char row, OLED4D_COLOR color)
{
    char args[] = { str, column, row, SPLIT(color) };
    command(OP_PLACE_TEXT_CHAR, args, sizeof(args));
}

template <class T>
void Oled4d<T>::setTextAppearance(OLED4D_TEXTAPPEARANCE appearance)
{
    command(OP_OPAQUE_TRANSPARENT_TXT, appearance);
}

template <class T>
void Oled4d<T>::textButton( OLED4D_BUTTONSTATE state, char x, char y, OLED4D_COLOR button_color,
                            OLED4D_FONT font,
                            OLED4D_COLOR text_color, char width, char height, char * text)
{
    char args[] = { state, x, y, SPLIT(button_color), font, SPLIT(text_color), width, height };
    command(OP_PLACE_TEXT_BUTTON, args, sizeof(args), text);
}



template <class T>
void Oled4d<T>::drawTriangle(char x1, char y1, char x2, char y2, char x3, char y3, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, x3, y3, SPLIT(color) };
    CMD_A(OP_DRAW_TRIANGLE, args);
}

template <class T>
void Oled4d<T>::drawRectangle(char x1, char y1, char x2, char y2, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, SPLIT(color) };
    CMD_A(OP_DRAW_RECTANGLE, args);
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

    CMD_A_S(OP_DRAW_POLYGON, args, size + 3); 
}

template <class T>
char Oled4d<T>::displayImage(char x, char y, char width, char height, OLED4D_COLORMODE color_mode, char * data)
{
    unsigned int i, count = 0;

    _serial.print(OP_DISPLAY_IMAGE, BYTE);

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
    command(OP_SET_PEN_SIZE, pensize);
}

template <class T>
void Oled4d<T>::screenCopyPaste(char xs, char ys, char xd, char yd, char width, char height)
{
    char args[] = { xs, ys, xd, yd, width, height };
    CMD_A(OP_BLOCK_COPY_PASTE, args);
}

template <class T>
void Oled4d<T>::drawCircle(char x, char y, char radius, OLED4D_COLOR color)
{
    char args[] = { x, y, radius, SPLIT(color) };
    CMD_A(OP_DRAW_CIRCLE, args);
}

template <class T>
OLED4D_COLOR Oled4d<T>::readPixel(char x, char y)
{
    OLED4D_COLOR color;
    _serial.print(OP_READ_PIXEL, BYTE);
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

    CMD_A(OP_ADD_USER_BMP_CHAR, args);
}

//  Display bitmap char from index
template <class T>
void Oled4d<T>::displayBmpChar(char index, char x, char y, OLED4D_COLOR color)
{
    char args[] = { index, x, y, SPLIT(color) };
    CMD_A(OP_DISPLAY_USER_BMP_CHAR, args);
}

//  Clear screen
template <class T>
void Oled4d<T>::clear()
{
    CMD(OP_ERASE_SCREEN);
}


template <class T>
void Oled4d<T>::drawLine(char x1, char y1, char x2, char y2, OLED4D_COLOR color)
{
    char args[] = { x1, y1, x2, y2, SPLIT(color) };
    CMD_A(OP_DRAW_LINE, args);
}


template <class T>
void Oled4d<T>::putPixel(char x, char y, OLED4D_COLOR color)
{
    char args[] = { x, y, SPLIT(color) };
    CMD_A(OP_PUT_PIXEL, args);
}

template <class T>
void Oled4d<T>::setBG(int color)
{
    CMD_I(OP_SET_BG_COLOR, color)
}

#endif
