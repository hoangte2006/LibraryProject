#pragma once
#include <fstream>
#include "Structs.h"

bool loadDauSach(const char* filename, ListDauSach& ds);
bool loadDocGia(const char* filename, QuanLyDocGia& ql);

bool saveDauSach(const char* filename, ListDauSach ds);
bool saveDocGia(const char* filename, TREE_DocGia root);

void capNhatTrangThaiSachDangMuon(TREE_DocGia root, ListDauSach& ds);
