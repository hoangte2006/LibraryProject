#pragma once

#define MAX_DAUSACH 1000
#define MAX_STT_STACK 1000
#define MAX_SACH_MUON 3
#define MAX_DOCGIA 1000
#define HAN_MUON 7
#define INSERTION_THRESHOLD 16

// Cau hinh danh sach ma the
#define MIN_MA_THE 1000
#define MAX_MA_THE 9999
#define SO_LUONG_MA 9000
#define GIOI_HAN_MANG 10000

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
};
typedef DocGia* TREE_DocGia;

struct DocGiaQuaHan {
    DocGia* dg;
    int soNgayQuaHan;
};

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

struct StackSTT {
    int data[MAX_STT_STACK];
    int top; // -1: stack rong, 0: 1 phan tu, 1: 2 phan tu, ...
};

struct DauSach {
    char ISBN[20], tenSach[100], tacGia[50], theLoai[30];
    char tenSachSearch[100]; // tenSach da chuan hoa lowercase de tra cuu nhanh
    int soTrang, namXuatBan; 
    int soLuotMuon = 0; // dem top 10

    ListSach dsSach;     
    
    StackSTT sttXoa; // de tai su dung STT
};

const int HASH_SIZE = 2003; 

struct HashEntry {
    char isbn[20];
    DauSach* dauSachPtr;
    bool daCoDuLieu = false; // false: chua co du lieu
    bool daXoa = false; // true: bi xoa
};

struct ListDauSach {
    int n = 0;
    DauSach* nodes[MAX_DAUSACH]; 
    HashEntry hashTable[HASH_SIZE]; // Bang bam de tim ISBN trong O(1)
};

