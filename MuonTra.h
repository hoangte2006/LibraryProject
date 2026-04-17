#pragma once
#include "Structs.h"
#include <ctime>

// NHOM THOI GIAN:
Ngay layNgayHienTai();
time_t chuyenNgaySangTimeT(const Ngay& ngay);
int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai);
int tinhQuaHanMax(const DocGia* docGia, const Ngay& ngayHT);

// LOGIC MUON TRA:
bool muonSach(DocGia* docGia, ListDauSach &ds, const char* maSach);
bool traSach(DocGia* docGia, ListDauSach &ds, const char* maSach, Ngay ngayTra);
bool baoMatSach(DocGia* docGia, ListDauSach &ds, const char* maSach);

// RULE NGHIEP VU:
bool coSachQuaHan(const DocGia* docGia);
void xemSachDangMuon(const DocGia* docGia, const ListDauSach& ds);
