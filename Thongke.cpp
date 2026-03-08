#include "Thongke.h"
#include <iostream>

using namespace std;


// ===============================
// 1. Tong so dau sach
// ===============================
int tongSoDauSach(ListDauSach ds)
{
    return ds.n;
}


// ===============================
// 2. Tong so cuon sach
// ===============================
int tongSoCuonSach(ListDauSach ds)
{
    int tong = 0;

    for (int i = 0; i < ds.n; i++)
    {
        Sach* p = ds.nodes[i]->dsSach.pHead;

        while (p != NULL)
        {
            tong++;
            p = p->pNext;
        }
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
// 4. Top 10 sach muon nhieu nhat
// ===============================
void top10Sach(ListDauSach ds)
{
    if (ds.n == 0)
    {
        cout << "Khong co dau sach nao.\n";
        return;
    }

    DauSach* temp[MAX_DAUSACH];

    for (int i = 0; i < ds.n; i++)
        temp[i] = ds.nodes[i];

    // sap xep giam dan theo soLuotMuon
    for (int i = 0; i < ds.n - 1; i++)
    {
        for (int j = i + 1; j < ds.n; j++)
        {
            if (temp[i]->soLuotMuon < temp[j]->soLuotMuon)
            {
                DauSach* t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    int limit = ds.n < 10 ? ds.n : 10;

    cout << "\n===== TOP 10 SACH MUON NHIEU NHAT =====\n";

    for (int i = 0; i < limit; i++)
    {
        cout << i + 1 << ". "
             << temp[i]->tenSach
             << " | Tac gia: " << temp[i]->tacGia
             << " | So luot muon: " << temp[i]->soLuotMuon
             << endl;
    }
}


// ===============================
// 5. Doc gia muon qua han
// ===============================
void docGiaQuaHan(TREE_DocGia root)
{
    if (root == NULL)
        return;

    docGiaQuaHan(root->pLeft);

    MuonTra* mt = root->dsMuonTra.pHead;

    while (mt != NULL)
    {
        if (mt->trangThai == 0)   // dang muon
        {
            cout << "Doc gia: "
                 << root->ho << " "
                 << root->ten
                 << " | Ma the: "
                 << root->maThe
                 << endl;
        }

        mt = mt->pNext;
    }

    docGiaQuaHan(root->pRight);
}
