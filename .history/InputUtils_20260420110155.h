#pragma once
#include "Structs.h"
#include <iostream>

bool kiemTraNgayHopLe(const Ngay& ngay);
int nhapSoNguyenHopLe(const char* thongBao);

bool nhapMa(const char* thongBao, char* result, int maxLen, const char* initialValue = "");
bool nhapTen(const char* thongBao, char* result, int maxLen, const char* initialValue = "");
bool nhapChuoiTuDo(const char* thongBao, char* result, int maxLen, const char* initialValue = "");

// --- Form Input Functions ---
#define INPUT_OK 0
#define INPUT_CANCEL -1
#define INPUT_UP -2
#define INPUT_DOWN -3

int nhapChuoiForm(int x, int y, char* result, int maxLen, bool chiNhanSo = false, bool chiNhanChu = false);
int nhapSoForm(int x, int y, int& result);
