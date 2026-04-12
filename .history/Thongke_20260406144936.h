#pragma once
#include "Structs.h"
#include <ctime>

// --- CAC HAM THONG KE DOC GIA QUA HAN (DUNG QUICK SORT) ---
int tinhQuaHanMax(const DocGia* docGia, const Ngay& ngayHienTai);
void swapQH(DocGiaQuaHan &a, DocGiaQuaHan &b);
int soSanhQH(DocGiaQuaHan &a, DocGiaQuaHan &b);
int medianOfThreeQH(DocGiaQuaHan arr[], int left, int right);
int partitionQH(DocGiaQuaHan arr[], int left, int right);
void quickSortQH(DocGiaQuaHan arr[], int left, int right);
void lietKeDocGiaQuaHan(TREE_DocGia root);

// --- CAC HAM THONG KE TOP 10 SACH (DUNG HEAP SORT) ---
bool lonHon(DauSach* a, DauSach* b);
void heapify(DauSach* arr[], int n, int i);
void buildHeap(DauSach* arr[], int n);
void inTop10SachMuonNhieu(const ListDauSach &ds);

#endif
