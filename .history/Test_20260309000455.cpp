#include <iostream>
#include <conio.h>
#include <cctype>

using namespace std;

void redrawLine(const char* buffer, int len, int vt)
{
    cout << "\rTest nhap ma: ";

    for (int i = 0; i < len; i++)
        cout << buffer[i];

    cout << "   "; // xóa dư

    cout << "\rTest nhap ma: ";

    for (int i = 0; i < vt; i++)
        cout << buffer[i];

    cout.flush();
}

void nhapMa(char* result, int maxLen)
{
    int len = 0;
    int vt = 0;

    while (true)
    {
        int c = _getch();

        // ENTER
        if (c == 13)
        {
            if (len > 0)
            {
                result[len] = '\0';
                cout << endl;
                return;
            }
            continue;
        }

        // PHÍM ĐẶC BIỆT
        if (c == 0 || c == 224)
        {
            int key = _getch();

            // LEFT
            if (key == 75 && vt > 0)
                vt--;

            // RIGHT
            else if (key == 77 && vt < len)
                vt++;

            // DELETE
            else if (key == 83 && vt < len)
            {
                for (int i = vt; i < len - 1; i++)
                    result[i] = result[i + 1];
                len--;
            }

            redrawLine(result, len, vt);
            continue;
        }

        // BACKSPACE
        if (c == 8)
        {
            if (vt > 0)
            {
                for (int i = vt - 1; i < len - 1; i++)
                    result[i] = result[i + 1];

                vt--;
                len--;
            }

            redrawLine(result, len, vt);
            continue;
        }

        // CHỈ NHẬN A-Z 0-9 - _
        if (isalnum(c) || c == '-' || c == '_')
        {
            if (len < maxLen - 1)
            {
                if (isalpha(c))
                    c = toupper(c);

                for (int i = len; i > vt; i--)
                    result[i] = result[i - 1];

                result[vt] = (char)c;
                vt++;
                len++;
            }
            else
            {
                cout << "\a"; // beep khi đầy
            }

            redrawLine(result, len, vt);
        }
    }
}

void testNhapLieu()
{
    const int GIOI_HAN = 11;
    char ma[GIOI_HAN];

    cout << "Test nhap ma: ";
    nhapMa(ma, GIOI_HAN);

    cout << "=> Ma vua nhap: [" << ma << "]" << endl;
}