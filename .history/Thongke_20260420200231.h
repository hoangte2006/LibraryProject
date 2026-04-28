#pragma once
#include "Structs.h"

// Thong ke Doc gia
void lietKeDocGiaQuaHan(QuanLyDocGia &ql); // Cap nhat tham so QuanLyDocGia

// Thong ke Sach
void inTop10SachMuonNhieu(const ListDauSach &ds);
void thongKeSoLuongTheoTheLoai(const ListDauSach &ds);

// Ham tro giup cap nhat luot muon (Dung khi load file)
void capNhatSoLuotMuonCuaDauSach(TREE_DocGia root, ListDauSach &ds);
