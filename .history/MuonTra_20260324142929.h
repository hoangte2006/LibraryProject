#pragma once
#include "Structs.h"
#include <ctime>

// NHOM THOI GIAN:
Ngay layNgayHienTai();
time_t chuyenNgaySangTimeT(const Ngay&);
int tinhSoNgay(const Ngay&, const Ngay&);

// LOGIC MUON TRA:
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayMuon) {
bool traSach(TREE_DocGia root, ListDauSach &ds);
bool baoMatSach(TREE_DocGia root, ListDauSach &ds);

// RULE NGHIEP VU:
bool coSachQuaHan(const DocGia* docGia, const Ngay& ngayHienTai);