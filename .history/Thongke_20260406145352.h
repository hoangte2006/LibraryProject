#pragma once
#include "Structs.h"

// ================= QUA HAN =================

// Tinh so ngay qua han lon nhat cua 1 doc gia
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

// So sanh luot muon (heap)
bool (DauSach* a, DauSach* b);

// Heap
void heapify(DauSach* arr[], int n, int i);
void buildHeap(DauSach* arr[], int n);

// In top 10
void inTop10SachMuonNhieu(const ListDauSach &ds);