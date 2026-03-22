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

// 2. NHOM HAM TIM KIEM VA TRUY XUAT
DocGia* timDocGia(const TREE_DocGia root, int maThe);
int demSachDangMuon(const DocGia* docGia);


// 3. NHOM HAM HIEN THI (DISPLAY) 
void inDanhSachDocGia(const TREE_DocGia root);


// 4. NHOM HAM GIAI PHONG BO NHO (CLEANUP)
void giaiPhongMuonTra(ListMuonTra &list);
void giaiPhongCay(TREE_DocGia &root);

// 5. MUON 
bool dangMuonSach(const DocGia* docGia, const char* maSach);
bool soSanhNgay(Ngay a, Ngay b);

//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//