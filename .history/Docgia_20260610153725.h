#pragma once
#include "Structs.h"

// Khoi tao va luu tru ma the
void khoiTaoKhoMaThe(QuanLyDocGia& ql);
void luuKhoMaThe(QuanLyDocGia& ql);
int taoMaTheMoi(QuanLyDocGia& ql);

// Quan ly Doc gia (Su dung struct QuanLyDocGia moi)
DocGia* taoDocGia(int maThe);
bool themDocGia(QuanLyDocGia &ql, DocGia* newnode);
bool xoaDocGia(QuanLyDocGia &ql, int maThe);
void khoaMoThe(TREE_DocGia root, int maThe);

// Tim kiem va In an
DocGia* timDocGia(const TREE_DocGia root, int maThe);
void inDanhSachDocGia(const TREE_DocGia root);
void inTheoTen(QuanLyDocGia &ql);

// Giai phong bo nho
void giaiPhongMuonTra(ListMuonTra &list);
void giaiPhongCay(TREE_DocGia &root);

// Ho tro sap xep
void BSTtoArray(TREE_DocGia root, DocGia* arr[], int &index);
        cout << "\n   (T) Them | (S) Sua | (X) Xoa | (K) Khoa/Mo | (F) Tim | (I) In DS | (Ctrl+Z) Hoan tac | ESC: Quay lai\n";

