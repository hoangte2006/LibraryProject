#pragma once
#include <iostream>
#include <conio.h>
#include <cctype>
#include "Structs.h"

// Kiểm tra ngày hợp lệ
bool kiemTraNgayHopLe(const Ngay& ngay);

int nhapSoNguyenHopLe(const char* thongBao);

// --- Cac ham nhap lieu nang cao (bat tung ky tu) ---
void nhapMa(const char* thongBao, char* result, int maxLen);
void nhapTen(const char* thongBao, char* result, int maxLen);
void nhapChuoiTuDo(const char* thongBao, char* result, int maxLen);
