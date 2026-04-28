#pragma once
#include <fstream>
#include "Structs.h"

// Load du lieu
bool loadDauSach(const char* filename, ListDauSach& ds);
bool loadDocGia(const char* filename, QuanLyDocGia& ql); // Su dung QuanLyDocGia

// Save du lieu
bool saveDauSach(const char* filename, const ListDauSach& ds);
bool saveDocGia(const char* filename, TREE_DocGia root);

// Dong bo trang thai
void capNhatTrangThaiSachDangMuon(TREE_DocGia root, ListDauSach& ds);
