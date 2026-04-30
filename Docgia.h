#pragma once
#include "Structs.h"

// Khoi tao va luu tru ma the
void khoiTaoKhoMaThe();
void luuKhoMaThe();
int taoMaTheMoi();

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

// Can bang cay sau khi load file
void rebuildCayCanBang(QuanLyDocGia &ql);
