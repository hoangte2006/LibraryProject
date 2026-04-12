#pragma once
#include "Structs.h"

// -- Code Hoang Te --

// THAO TAC FILE DOC GIA
bool loadDauSach(const char* filename, ListDauSach& ds);
bool loadDocGia(const char* filename, TREE_DocGia& root);
void capNhatTrangThaiSachDangMuon(TREE_DocGia root, ListDauSach& ds);

// SAVE
bool saveDauSach(const char* filename, ListDauSach ds);
bool saveDocGia(const char* filename, TREE_DocGia root);
void ghiDanhSachSach(ofstream &file, ListSach dsSach);
void ghiDocGia(ofstream &file, TREE_DocGia root);
void ghiDanhSachMuonTra(ofstream &file, MuonTra* head);
    
