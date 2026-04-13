#pragma once
#include <iostream>
#include <conio.h>
#include <cctype>


#define KEY_UP    1001
#define KEY_DOWN  1002
#define KEY_LEFT  1003
#define KEY_RIGHT 1004
#define KEY_ENTER 13

inline int getch_raw()
{
    return _getch();
}

// Kiểm tra ngày hợp lệ
bool kiemTraNgayHopLe(const Ngay& ngay);

inline int getKey()
{
    int ch = _getch();

    if (ch == 0 || ch == 224)
    {
        ch = _getch();

        switch (ch)
        {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
        }
    }

    return ch;
}