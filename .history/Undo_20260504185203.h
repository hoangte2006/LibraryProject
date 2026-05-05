#pragma once
#include "Structs.h"
#include <cstring>
#include <cstdio>

// Loai hanh dong co the undo
#define UNDO_THEM_DOCGIA  0
#define UNDO_SUA_DOCGIA   1
#define UNDO_THEM_DAUSACH 2
#define UNDO_SUA_DAUSACH  3
#define UNDO_MUON_SACH    4

struct nodeState {
    int state; // 0,1,2,3,4

    // DocGia
    int  maThe;
    char ho[50], ten[20], giotinh[5];

    // DauSach
    char isbn[20];
    char tenSach[100], tacGia[50], theLoai[30];
    int  soTrang, namXuatBan;

    // MuonTra (dung chung maThe cua DocGia o tren)
    char maSach[25];

    char moTa[120]; // Mo ta hien thi cho nguoi dung
    nodeState* next;

    nodeState() {
        state = -1; next = nullptr; moTa[0] = '\0';
        maThe = -1; ho[0] = '\0'; ten[0] = '\0'; giotinh[0] = '\0';
        isbn[0] = '\0'; tenSach[0] = '\0'; tacGia[0] = '\0'; theLoai[0] = '\0';
        soTrang = 0; namXuatBan = 0; maSach[0] = '\0';
    }
};

struct undoStack {
    nodeState* top = nullptr;
    bool isEmpty() const { return top == nullptr; } // Kiem tra stack rong , bằng 
};

inline bool pushUndo(undoStack& stk, nodeState* node) {
    if (node == nullptr) return false;
    node->next = stk.top;
    stk.top = node;
    return true;
}

inline bool popUndo(undoStack& stk, nodeState& out) {
    if (stk.top == nullptr) return false;
    nodeState* temp = stk.top;
    out = *temp;
    stk.top = stk.top->next;
    delete temp;
    return true;
}

inline void clearUndo(undoStack& stk) {
    nodeState* temp;
    while (stk.top != nullptr) {
        temp = stk.top;
        stk.top = stk.top->next;
        delete temp;
    }
}
