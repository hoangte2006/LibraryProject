#pragma once

#define MAX_DAUSACH 1000

// 1. Cau truc Ngay thang 

struct Ngay {
    int ngay, thang, nam;
};

// 2. Danh sach MUONTRA (Lien ket don - pTail) 

struct MuonTra {
    char maSach[25];   

    Ngay ngayMuon;
    Ngay ngayTra;

    int trangThai = 0;     // 0: dang muon, 1: da tra, 2: lam mat 
    
    MuonTra* pNext;
};

struct ListMuonTra {
    MuonTra *pHead = nullptr, *pTail = nullptr; // Them pTail de AddTail la O(1)
};

// 3. Danh sach Doc gia 

struct DocGia {
    int maThe;         
    char ho[50], ten[20], giotinh[5]; 
    int trangThaiThe = 1;  // 0: khoa, 1: hoat dong 

    ListMuonTra dsMuonTra; 
    DocGia *pLeft, *pRight;
    //DocGia* pLeft;
    //DocGia* pRight;
};
typedef DocGia* TREE_DocGia;

// 4. Danh muc sach (Lien ket don - Luu cac cuon sach cu the) 

struct Sach {
    char maSach[25];   
    int trangThai = 0;     // 0: cho muon duoc, 1: da muon, 2: thanh ly 
    char viTri[20];
    
    Sach* pNext;
};

struct ListSach {
    Sach *pHead = nullptr, *pTail = nullptr;
    int tongSoSach = 0;
};

// 5. Dau sach (Mang con tro)
//  AE luu y: chen vao theo thu tu tang dan
struct DauSach {
    char ISBN[20], tenSach[100], tacGia[50], theLoai[30];
    int soTrang, namXuatBan; 
    int soLuotMuon = 0; // đếm top 10;

    ListSach dsSach;     
    
    
};

struct ListDauSach {
    int n = 0;
    DauSach* nodes[MAX_DAUSACH]; 
};

// 
//  6. Cau Truc Khacs
//

struct StackSTT {
    int data[1000];
    int top; // -1: stack rong, 0: 1 phan tu, 1: 2 phan tu, ...
};
