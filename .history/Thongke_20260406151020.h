#pragma once
#include "Structs.h"

// ================= QUA HAN =================

int tinhQuaHanMax(const DocGia* docGia, const Ngay& ngayHienTai);

// Liet ke doc gia qua han
void lietKeDocGiaQuaHan(TREE_DocGia root);


// ================= QUICK SORT QUA HAN =================

void swapQH(DocGiaQuaHan &a, DocGiaQuaHan &b);

int soSanhQH(DocGiaQuaHan &a, DocGiaQuaHan &b);

int medianOfThreeQH(DocGiaQuaHan arr[], int left, int right);

int partitionQH(DocGiaQuaHan arr[], int left, int right);

void quickSortQH(DocGiaQuaHan arr[], int left, int right);


// ================= TOP 10 SACH =================

bool luotMuonLonHon(DauSach* a, DauSach* b);

void heapify(DauSach* arr[], int n, int i);
void buildHeap(DauSach* arr[], int n);

void inTop10SachMuonNhieu(const ListDauSach &ds);

void capNhatSoLuotMuon(TREE_DocGia root, ListDauSach &ds);
