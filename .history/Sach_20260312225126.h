#pragma once
#include "Structs.h"

// CAC THAO TAC TREN DAU SACH
int timViTriChen(const ListDauSach &ds, const char* tenSach);
DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN);
void themDauSach(ListDauSach &ds);
void inDanhSachDauSach(const ListDauSach &ds);
bool xoaDauSach(ListDauSach &ds, const char* ISBN);


// CAC THAO TAC TREN CUON SACH
void themCuonSach(DauSach* dauSach);
bool xoaCuonSach(DauSach* dauSach, const char* maSach);
void inDanhSachCuonSach(const DauSach* dauSach);


// CAC THAO TAC TIM KIEM

// Tra cuu dau sach theo ten sach
void traCuuTheoTen(const ListDauSach &ds, const char* tenSach); 

Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay);

// CAC THAO TAC MUON TRA SACH
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayMuon);
bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra);
void addTailMuonTra(ListMuonTra &list, MuonTra* newNode);
void addTailSach(ListSach &list, Sach* newSach);


// THAO TAC LAY MA SACH VÀ TAI SU DUNG MA
void pushSTTXoa(int stt);
int laySTTMoi(ListSach &ds);

// GIAI PHONG
void giaiPhongDanhSachDauSach(ListDauSach &ds);




//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//