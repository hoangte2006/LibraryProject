#include "Thongke.h"
#include <iostream>
#include <ctime>

using namespace std;


// ===============================
// 1. Tong so dau sach
// ===============================
int tongSoDauSach(const ListDauSach &ds)
{
    return ds.n;
}


// ===============================
// 2. Tong so cuon sach
// ===============================
int tongSoCuonSach(const ListDauSach &ds)
{
    int tong = 0;

    for (int i = 0; i < ds.n; i++)
    {
        tong += ds.nodes[i]->dsSach.tongSoSach;
    }

    return tong;
}


// ===============================
// 3. Dem doc gia (duyet BST)
// ===============================
int tongSoDocGia(TREE_DocGia root)
{
    if (root == NULL)
        return 0;

    return 1 + tongSoDocGia(root->pLeft) + tongSoDocGia(root->pRight);
}


// ===============================
// 4. Top 10 sach muon nhieu nhat (O(N))
// ===============================
void top10Sach(const ListDauSach &ds)
{
    DauSach* top[10];
    int size = 0;

    for (int i = 0; i < ds.n; i++)
    {
        DauSach* cur = ds.nodes[i];

        // chen vao mang top[]
        int j = size - 1;

        while (j >= 0 && top[j]->soLuotMuon < cur->soLuotMuon)
        {
            if (j + 1 < 10)
                top[j + 1] = top[j];
            j--;
        }

        if (j + 1 < 10)
        {
            top[j + 1] = cur;
            if (size < 10) size++;
        }
    }

    cout << "\n===== TOP 10 SACH MUON NHIEU NHAT =====\n";

    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << ". "
             << top[i]->tenSach
             << " | So luot muon: "
             << top[i]->soLuotMuon
             << endl;
    }
}


// ===============================
// Ham tinh so ngay (don gian hoa)
// ===============================
int tinhNgay(const Ngay &d)
{
    return d.nam * 365 + d.thang * 30 + d.ngay;
}


// ===============================
// 5. Doc gia muon qua han (>7 ngay)
// ===============================

Ngay layNgayHienTai()
{
    time_t t = time(0);
    tm* now = localtime(&t);

    Ngay d;
    d.ngay = now->tm_mday;
    d.thang = now->tm_mon + 1;
    d.nam = now->tm_year + 1900;

    return d;
}
void docGiaQuaHan(TREE_DocGia root)
{
    if (root == NULL)
        return;

    docGiaQuaHan(root->pLeft);

    MuonTra* mt = root->dsMuonTra.pHead;
    
    Ngay today = layNgayHienTai();

    while (mt != NULL)
    {
        if (mt->trangThai == 0) // dang muon
        {
            int soNgay = tinhNgay(today) - tinhNgay(mt->ngayMuon);

            if (soNgay > 7)
            {
                cout << "Doc gia: "
                     << root->ho << " "
                     << root->ten
                     << " | Ma the: "
                     << root->maThe
                     << " | Qua han: " << soNgay << " ngay"
                     << endl;
            }
        }

        mt = mt->pNext;
    }

    docGiaQuaHan(root->pRight);
}
