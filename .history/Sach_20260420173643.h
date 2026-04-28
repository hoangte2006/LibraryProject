#pragma once
#include "Structs.h"

// CAC THAO TAC TREN DAU SACH
DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN);
void inDanhSachDauSach(const ListDauSach &ds);
bool xoaDauSach(ListDauSach &ds, const char* ISBN);

// BANG BAM (HASH TABLE) CHO ISBN
unsigned long hashStringISBN(const char* str);
void insertHashISBN(ListDauSach &ds, DauSach* dsPtr);
void xoaHashISBN(ListDauSach &ds, const char* isbn);


// CAC THAO TAC TREN CUON SACH
void themCuonSach(DauSach* dauSach);
bool xoaCuonSach(DauSach* dauSach, const char* maSach);
void inDanhSachCuonSach(const DauSach* dauSach);


// CAC THAO TAC TIM KIEM
int timViTriChen(const ListDauSach &ds, const char* tenSach);

void capNhatTenSachSearch(DauSach* dauSach);
void capNhatTacGiaSearch(DauSach* dauSach);

void chuanHoaISBN(char* isbn);
// Ham ho tro tach ma ISBN
void tachMaISBN(const char* maSach, char* isbnOut);

Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay);

void traCuuTheoTenNangCao(const ListDauSach &ds, const char* key);
void traCuuTheoTacGia(const ListDauSach &ds, const char* key);
void toLowerCase(char* str);
void chuanHoaChuoiSearch(const char* src, char* dest);

// CAC THAO TAC MUON TRA SACH
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach);
void addTailMuonTra(ListMuonTra &list, MuonTra* newNode);
void addTailSach(ListSach &list, Sach* newSach);

bool baoMatSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach);

// THAO TAC LAY MA SACH VÀ TAI SU DUNG MA
int laySTTMoi(DauSach* dauSach);

// GIAI PHONG
void giaiPhongDanhSachDauSach(ListDauSach &ds);


// Sort
void inTheoTheLoai_TrongDoTenTangDan(ListDauSach &ds) ;
void hybridSortDauSach(DauSach* arr[], int left, int right) ;
int partitionDauSach(DauSach* arr[], int left, int right) ;
int medianOfThree(DauSach* arr[], int left, int right) ;
void insertionSortDauSach(DauSach* arr[], int left, int right) ;
void swapDauSach(DauSach* &a, DauSach* &b) ;
int soSanhTheLoai(DauSach* a, DauSach* b) ;
