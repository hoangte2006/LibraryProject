#pragma once
#include "Structs.h"
#include <iostream>

bool kiemTraNgayHopLe(const Ngay& ngay);
int nhapSoNguyenHopLe(const char* thongBao);

bool nhapMa(const char* thongBao, char* result, int maxLen, const char* initialValue = "");
bool nhapTen(const char* thongBao, char* result, int maxLen, const char* initialValue = "");
bool nhapChuoiTuDo(const char* thongBao, char* result, int maxLen, const char* initialValue = "");
