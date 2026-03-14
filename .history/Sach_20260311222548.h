#pragma once
#include "Structs.h"

// CÁC THAO TÁC TRÊN ĐẦU SÁCH
int timViTriChen(const ListDauSach &ds, const char* tenSach);
DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN);
void themDauSach(ListDauSach &ds);
void inDanhSachDauSach(const ListDauSach &ds);
bool xoaDauSach(ListDauSach &ds, const char* ISBN);


// CÁC THAO TÁC TRÊN CUỐN SÁCH
void themCuonSach(DauSach* dauSach);
bool xoaCuonSach(DauSach* dauSach, const char* maSach);
void inDanhSachCuonSach(const DauSach* dauSach);


// CÁC THAO TÁC TRA CỨU
void traCuuTheoTen(const ListDauSach &ds, const char* tenSach);
Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay);

// CÁC THAO MUON TRẢ SÁCH
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayMuon);
bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra);
void addTailMuonTra(ListMuonTra &list, MuonTra* newNode);
void addTailSach(ListSach &list, Sach* newSach);


// THAO TAC K

// GIAI PHONG
void giaiPhongDanhSachDauSach(ListDauSach &ds);




//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//