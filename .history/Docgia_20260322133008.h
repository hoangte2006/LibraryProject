#pragma once
#include "Structs.h"

//
// -- Code Hoang Te -- 
//

#include "Docgia.h"
#include <iostream>
#include <cstring>

using namespace std;

// 1. NHOM HAM KHOI TAO VA THEM MOI (CORE)
DocGia* taoDocGia(int maThe);
bool themDocGia(TREE_DocGia& root, DocGia* newnode);
int taoMaTheMoi(TREE_DocGia root); 
void nhapDocGia(TREE_DocGia &root);

// 2. NHOM HAM TIM KIEM VA TRUY XUAT
DocGia* timDocGia(const TREE_DocGia root, int maThe);
int demSachDangMuon(const DocGia* docGia);


// 3. NHOM HAM HIEN THI (DISPLAY) 
void inDanhSachDocGia(const TREE_DocGia root);
voif inTheoTen(TREE_DocGia root);
void (TREE_DocGia root);

// 4. NHOM HAM GIAI PHONG BO NHO (CLEANUP)
void giaiPhongMuonTra(ListMuonTra &list);
void giaiPhongCay(TREE_DocGia &root);

// 5. MUON 
bool dangMuonSach(const DocGia* docGia, const char* maSach);
bool soSanhNgay(Ngay a, Ngay b);


// 6. XOA DOC GIA
bool xoaDocGia(TREE_DocGia &root, int maThe);
void hieuChinhDocGia(TREE_DocGia root, int maThe);
void khoaMoThe(TREE_DocGia root, int maThe);


// 7.SORT
void BSTtoArray(TREE_DocGia root, DocGia* arr, int &index);
int soSanhTen(DocGia* a, DocGia* b);
void sortDocGiaTheoTen(DocGia* arr, int n);
//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//