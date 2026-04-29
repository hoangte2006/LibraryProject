#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
#include "InputUtils.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// ---(Loc va Xao bang thuat toan LCG) ---

static int currentX = 0;

void khoiTaoKhoMaThe(TREE_DocGia root) { 
    ifstream fileConfig("Input_file/config_ma.txt");
    if (fileConfig.is_open()) {
        fileConfig >> currentX;
        fileConfig.close();
    } else {
        // lan chay dau tien khi chua co file config
        // ramdom bat ki bang thoi gian hien tai
        currentX = time(nullptr) % SO_LUONG_MA; 
    }
}

// Ham nay de goi khi tat phan mem (luu lai currentX)
void luuKhoMaThe() {
    ofstream fileConfig("Input_file/config_ma.txt");
    if (fileConfig.is_open()) {
        fileConfig << currentX;
        fileConfig.close();
    }
}

// Tao ma the moi bang LCG, tra ve -1 neu khong con ma nao trong
int taoMaTheMoi(TREE_DocGia root) { 
    const int m = SO_LUONG_MA;
    const int a = 61;
    const int c = 17;

    for (int attempts = 0; attempts < m; attempts++) { 
        currentX = (currentX * a + c) % m;
        int maTheMoi = currentX + MIN_MA_THE;
        if (timDocGia(root, maTheMoi) == nullptr) {
            return maTheMoi;
        }
    }
    return -1; // Toan bo chu ky LCG da duoc dung (khong xay ra voi MAX_DOCGIA=1000)
}

// Ham tao mot doc gia moi voi ma the duoc truyen vao
// va cac truong thong tin con lai duoc khoi tao mac dinh
// sau do tra ve con tro den doc gia moi duoc tao ra.
DocGia* taoDocGia(int maThe) {
    DocGia* newDocGia = new DocGia; 

    newDocGia->maThe = maThe;
    newDocGia->trangThaiThe = 1; 
    newDocGia->soSachDangMuon = 0; // O(1) dem sach dang muon
    
    newDocGia->pRight = nullptr; 
    newDocGia->pLeft = nullptr; 
    
    newDocGia->dsMuonTra.pHead = nullptr; 
    newDocGia->dsMuonTra.pTail = nullptr;

    return newDocGia; 
}

bool themDocGia(QuanLyDocGia &ql, DocGia* newnode) {
    if (ql.root == nullptr) {
        ql.root = newnode;
        ql.soLuongDocGia++;
        return true;
    }

    TREE_DocGia current = ql.root;
    TREE_DocGia parent = nullptr;
    while (current != nullptr) {
        if (newnode->maThe == current->maThe) {
            cout << "Ma the da ton tai! Them doc gia that bai.\n";
            delete newnode;
            return false;
        }
        parent = current;
        if (newnode->maThe < current->maThe)
            current = current->pLeft;
        else
            current = current->pRight;
    }

    if (newnode->maThe < parent->maThe)
        parent->pLeft = newnode;
    else
        parent->pRight = newnode;

    ql.soLuongDocGia++;
    return true;
}

// Ham de quy thuc hien xoa node bang cach cat noi con tro (Giong nguyen ban code cu cua ban)
bool xoaDocGiaHelper(TREE_DocGia &root, int maThe) {
    if (root == nullptr) return false;

    // 1. Tim node can xoa
    if (maThe < root->maThe) {
        return xoaDocGiaHelper(root->pLeft, maThe); 
    } else if (maThe > root->maThe) {
        return xoaDocGiaHelper(root->pRight, maThe); 
    } 
    else {
        // DA TIM THAY NODE CAN XOA

        // Kiem tra dieu kien: Da co lich su giao dich thi khong cho xoa
        // (Chuyen cai nay tu timDocGia vao luon day de khoi mat cong tim 2 lan)
        if (root->dsMuonTra.pHead != nullptr) {
            cout << "Loi: Khong the xoa! Doc gia nay da co lich su giao dich.\n";
            return false;
        }

        // Case 1: Khong co con (Node la)
        if (root->pLeft == nullptr && root->pRight == nullptr) {
            delete root; 
            root = nullptr; // tham chieu &root se cap nhat con tro cua node cha thanh nullptr
            return true; 
        }

        // Case 2: Co 1 con phai
        else if (root->pLeft == nullptr) {
            DocGia* temp = root; 
            root = root->pRight; // keo con phai len the cho
            delete temp;
            return true;
        }

        // Case 2: Co 1 con trai
        else if (root->pRight == nullptr) {
            DocGia* temp = root; 
            root = root->pLeft; // keo con trai len the cho
            delete temp;
            return true;
        }

        // Case 3: Co 2 con
        else {
            // Dung 2 con tro de di tim node the mang (Nho nhat ben phai)
            DocGia* replacementParent = root;
            DocGia* replacement = root->pRight;

            while (replacement->pLeft != nullptr) {
                replacementParent = replacement;
                replacement = replacement->pLeft;
            }

            if (replacementParent == root) {
                // Node the mang la con phai truc tiep cua node can xoa
                replacement->pLeft = root->pLeft;

                delete root;
                root = replacement; // Dua node the mang len the cho node xoa
            } 
            else {
                // Node the mang nam sau ben trai
                replacementParent->pLeft = replacement->pRight; // Noi con phai cua the mang vao cha no

                replacement->pLeft = root->pLeft;
                replacement->pRight = root->pRight;

                delete root;
                root = replacement; // Dua node the mang len the cho node xoa
            }
        }
        return true; 
    }
}

// Ham xoaDocGia goi tu ngoai Menu
bool xoaDocGia(QuanLyDocGia &ql, int maThe) {
    if (xoaDocGiaHelper(ql.root, maThe)) {
        ql.soLuongDocGia--;
        return true;
    }
    return false;
}

// Ham tra ve con tro den doc gia co ma the giong ma the truyen vao.
DocGia* timDocGia(const TREE_DocGia root, int maThe) {
    // Tranh de quy
    DocGia* current = root; 
    while (current != nullptr) {
        if (maThe < current->maThe) {
            current = current->pLeft; 
        } 
        else if (maThe > current->maThe) {
                current = current->pRight; 
            } else return current; 
    }
    return nullptr; 
}

static void inMotDocGia(const DocGia* dg) {
    string hoTen = string(dg->ho) + " " + string(dg->ten);
    if (hoTen.length() > 25) hoTen = hoTen.substr(0, 22) + "...";

    cout << " │ " << left << setw(10) << dg->maThe
         << " │ " << left << setw(25) << hoTen
         << " │ " << left << setw(10) << dg->giotinh
         << " │ " << left << setw(15) << (dg->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │" << endl;
}

static void inDanhSachDocGia_DeQuy(const TREE_DocGia root) {
    if (root == nullptr) return;
    inDanhSachDocGia_DeQuy(root->pLeft);
    inMotDocGia(root);
    inDanhSachDocGia_DeQuy(root->pRight);
}

void inDanhSachDocGia(const TREE_DocGia root) {
    if (root == nullptr) {
        cout << "Danh sach doc gia rong!\n";
        return;
    }

    cout << " ┌" << string(12, '-') << "┬" << string(27, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐" << endl;
    cout << " │ " << left << setw(10) << "Ma The"
         << " │ " << left << setw(25) << "Ho Ten"
         << " │ " << left << setw(10) << "Gioi Tinh"
         << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
    cout << " ├" << string(12, '-') << "┼" << string(27, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤" << endl;

    inDanhSachDocGia_DeQuy(root);

    cout << " └" << string(12, '-') << "┴" << string(27, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘" << endl;
}

// Ham nay lam nhiem vu giai phong bo nho cho toan bo danh sach muon tra cua mot doc gia.
void giaiPhongMuonTra(ListMuonTra &list) {
    MuonTra* temp = list.pHead;

    while (temp != nullptr) {
        MuonTra* next = temp->pNext; 
        delete temp;    
        temp = next;    
    }


    list.pHead = list.pTail = nullptr;
}


void giaiPhongCay(TREE_DocGia &root) {
    if (root == nullptr) return;
    // Trai -> Phai -> Node (Du lieu)

    giaiPhongCay(root->pLeft);
    giaiPhongCay(root->pRight);

    giaiPhongMuonTra(root->dsMuonTra);

    delete root;
    root = nullptr;
}

void khoaMoThe(TREE_DocGia root, int maThe)
{
    DocGia* docgia = timDocGia(root, maThe);

    if (docgia == nullptr) {
        cout << "Khong tim thay doc gia!\n";
        return;
    }

    if (docgia->trangThaiThe == 1) {
        docgia->trangThaiThe = 0;
    } else {
        docgia->trangThaiThe = 1;
    }

    cout << "Cap nhat trang thai the thanh cong! Trang thai moi: " << (docgia->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << endl;
}

// Ham duyet LNR lay dia chi cac node vao mang con tro. 
void BSTtoArray(TREE_DocGia root, DocGia* arr[], int &index) {
    if (root == nullptr) return;    

    BSTtoArray(root->pLeft, arr, index);

    arr[index++] = root; // luu dia chi (da dọn MAX_DOCGIA)

    BSTtoArray(root->pRight, arr, index);
}

// neu soSanhTen(a, b) < 0 thi a nho hon b, neu soSanhTen(a, b) > 0 thi a lon hon b, neu soSanhTen(a, b) == 0 thi a bang b.
int soSanhTen(DocGia* a, DocGia* b)
{
    int cmp = stricmp(a->ten, b->ten);
    if (cmp == 0)
        return stricmp(a->ho, b->ho);
    return cmp;
}

void swapDocGia(DocGia* &a, DocGia* &b) {
    DocGia* temp = a;
    a = b;
    b = temp;
}

//--------------------
// Quick Sort - chon kieu trung vi 3 phan tu de lam giam thoi gian chay va tranh truong hop xau nhat khi chon pivot la phan tu cuoi cung.

int medianOfThree(DocGia* arr[], int left, int right) {
    int mid = left + (right - left) / 2;

    if (soSanhTen(arr[left], arr[mid]) > 0)
        swapDocGia(arr[left], arr[mid]);
    
    if (soSanhTen(arr[left], arr[right]) > 0)
        swapDocGia(arr[left], arr[right]);

    if (soSanhTen(arr[mid], arr[right]) > 0)
        swapDocGia(arr[mid], arr[right]);
    
    return mid; // A < B < C thi lay B
}

// Ham partition se chon pivot la phan tu cuoi cung
int partition(DocGia* arr[], int left, int right) {
    int pivotIndex = medianOfThree(arr, left, right);

    swapDocGia(arr[pivotIndex], arr[right]);

    DocGia* pivot = arr[right];
    int i = left - 1;   // lúc này i 

    for (int j = left; j < right; j++) {
        if (soSanhTen(arr[j], pivot) < 0) {
            i++;
            swapDocGia(arr[i], arr[j]);
        }
    }
    swapDocGia(arr[i + 1], arr[right]); // A<= povit <=B
    return i + 1; 
}

// dung Insertion Sort cho mang nho
void insertionSortDocGia(DocGia* arr[], int n) {
    for (int i = 1; i < n; i++) {
        DocGia* key = arr[i];
        int j = i - 1;

        while (j >= 0 && soSanhTen(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void quickSortDocGia(DocGia* arr[], int left, int right) {
    if (left >= right) return;

    if (right - left + 1 <= INSERTION_THRESHOLD) {
        insertionSortDocGia(&arr[left], right - left + 1); 
    } 
    else {
            int pivotCurrent = partition(arr, left, right);
            quickSortDocGia(arr, left, pivotCurrent - 1);
            quickSortDocGia(arr, pivotCurrent + 1, right);
    }
}

//-------------------

void inTheoTen(QuanLyDocGia &ql)
{
    if (ql.soLuongDocGia == 0) {
        cout << "Danh sach doc gia rong!\n";
        return;
    }
    
    DocGia** arr = new DocGia*[ql.soLuongDocGia];
    int n = 0;

    BSTtoArray(ql.root, arr, n);
    if (n > 0)
        quickSortDocGia(arr, 0, n - 1);

    cout << "\n--- DANH SACH DOC GIA SAP XEP THEO TEN ---\n";
    cout << " ┌" << string(12, '-') << "┬" << string(40, '-') << "┐" << endl;
    cout << " │ " << left << setw(10) << "Ma The" << " │ " << left << setw(38) << "Ho Ten" << " │" << endl;
    cout << " ├" << string(12, '-') << "┼" << string(40, '-') << "┤" << endl;

    for (int i = 0; i < n; i++) {
        string hoTen = string(arr[i]->ho) + " " + string(arr[i]->ten);
        if (hoTen.length() > 38) hoTen = hoTen.substr(0, 35) + "...";
        cout << " │ " << left << setw(10) << arr[i]->maThe 
             << " │ " << left << setw(38) << hoTen << " │" << endl;
    }

    cout << " └" << string(12, '-') << "┴" << string(40, '-') << "┘" << endl;

    delete[] arr; 
}
