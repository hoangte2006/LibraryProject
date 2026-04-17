#include <iostream>
#include <conio.h>
#include <cctype>
#include "InputUtils.h"

using namespace std;

void testNhapLieu()
{
    const int GIOI_HAN = 11;
    char ma[GIOI_HAN];
    nhapMa("Test nhap ma: ", ma, GIOI_HAN);
    cout << "=> Ma vua nhap: [" << ma << "]" << endl;

    const int GIOI_HAN_TEN = 50;
    char ten[GIOI_HAN_TEN];
    nhapTen("Test nhap ten: ", ten, GIOI_HAN_TEN);
    cout << "=> Ten vua nhap: [" << ten << "]" << endl;
}