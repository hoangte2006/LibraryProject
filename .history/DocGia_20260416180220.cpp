#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
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

// O(1) tao ma the moi
int taoMaTheMoi(TREE_DocGia root) {
    int m = SO_LUONG_MA;
    int a = 61;
    int c = 17;
    
    int maTheMoi = -1;
    
    while (true) {
        currentX = (currentX * a + c) % m;
        maTheMoi = currentX + MIN_MA_THE;

        if (timDocGia(root, maTheMoi) == nullptr) {
            break; 
        }
    }
    
    return maTheMoi;
}

// Ham nhap rieng
void nhapThongTinDocGia(DocGia* dg)
{
    cout << "Nhap ho: ";
    cin.getline(dg->ho, 50);

    cout << "Nhap ten: ";
    cin.getline(dg->ten, 20);

    cout << "Nhap gioi tinh: ";
    cin.getline(dg->giotinh, 5);
}

// Ham nhap thong tin doc gia
void nhapDocGia(QuanLyDocGia &ql) {
    int maThe = taoMaTheMoi(ql.root);

    DocGia* docgia = taoDocGia(maThe);

    cout << "Ma the tu dong: " << maThe << endl;

    nhapThongTinDocGia(docgia);

    if (themDocGia(ql, docgia)) {
        cout << "Them doc gia thanh cong!\n";
    }
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

// Ham thuc hien xoa node tren BST (Viet theo cach tuong minh nhat)
DocGia* thucHienXoa(TREE_DocGia root, int maThe, bool &isDeleted) {
    if (root == nullptr) return nullptr;

    // 1. Tim node can xoa
    if (maThe < root->maThe) {
        root->pLeft = thucHienXoa(root->pLeft, maThe, isDeleted);
    } else if (maThe > root->maThe) {
        root->pRight = thucHienXoa(root->pRight, maThe, isDeleted);
    } else {
        // DA TIM THAY NODE CAN XOA

        // Buoc A: Kiem tra dieu kien logic (Co lich su muon tra thi khong cho xoa)
        if (root->dsMuonTra.pHead != nullptr) {
            cout << "Loi: Khong the xoa! Doc gia nay da co lich su giao dich.\n";
            isDeleted = false;
            return root;
        }

        isDeleted = true;
        DocGia* nodeXoa = root;

        // Buoc B: Xu ly 3 truong hop xoa

        // TH1: Node la (Khong co con nao)
        if (root->pLeft == nullptr && root->pRight == nullptr) {
            delete nodeXoa;
            return nullptr; // Tra ve NULL de node cha cap nhat lai con tro
        }

        // TH2: Node co 1 con (Con phai)
        else if (root->pLeft == nullptr) {
            DocGia* temp = root->pRight;
            delete nodeXoa;
            return temp; // Tra ve con phai de cha noi vao
        }

        // TH2: Node co 1 con (Con trai)
        else if (root->pRight == nullptr) {
            DocGia* temp = root->pLeft;
            delete nodeXoa;
            return temp; // Tra ve con trai de cha noi vao
        }

        // TH3: Node co 2 con
        else {
            // Tim node the mang. Node nho nhat ben phai
            DocGia* successor = root->pRight;
            while (successor->pLeft != nullptr) {
                successor = successor->pLeft;
            }

            // Copy thong tin tu successor sang node hien tai
            root->maThe = successor->maThe;
            strcpy(root->ho, successor->ho);
            strcpy(root->ten, successor->ten);
            strcpy(root->giotinh, successor->giotinh);
            root->trangThaiThe = successor->trangThaiThe;
            root->soSachDangMuon = successor->soSachDangMuon;
            root->dsMuonTra = successor->dsMuonTra; // Chuyen list

            // QUAN TRONG: De xoa node successor ma khong lam mat list vua copy
            // Ta phai "cat" list cua successor ra truoc khi goi lenh xoa no
            successor->dsMuonTra.pHead = successor->dsMuonTra.pTail = nullptr;

            // Xoa node successor thuc su o phia duoi
            // Dung bien tam dummy de ham de quy khong tru soLuongDocGia lan nua
            bool dummy = false;
            root->pRight = thucHienXoa(root->pRight, successor->maThe, dummy);
        }
    }
    return root;
}
bool xoaDocGia(QuanLyDocGia &ql, int maThe) {
    bool isDeleted = false;
    ql.root = thucHienXoa(ql.root, maThe, isDeleted);
    
    if (isDeleted) {
        ql.soLuongDocGia--;
        return true;
    }
    return false;
}

void hieuChinhDocGia(TREE_DocGia root, int maThe)
{
    DocGia* docgia = timDocGia(root, maThe);

    if (docgia == nullptr) {
        cout << "Khong tim thay doc gia!\n";
        return;
    }   

    nhapThongTinDocGia(docgia);

    cout << "Cap nhat thanh cong!\n";
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
    int cmp = strcmp(a->ten, b->ten);
    if (cmp == 0)
        return strcmp(a->ho, b->ho);
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
    
    return mid;
}

// Ham partition se chon pivot la phan tu cuoi cung
int partition(DocGia* arr[], int left, int right) {
    int pivotIndex = medianOfThree(arr, left, right);

    swapDocGia(arr[pivotIndex], arr[right]);

    DocGia* pivot = arr[right];
    int i = left - 1;   

    for (int j = left; j < right; j++) {
        if (soSanhTen(arr[j], pivot) < 0) {
            i++;
            swapDocGia(arr[i], arr[j]);
        }
    }
    swapDocGia(arr[i + 1], arr[right]); // A<= povit <=B
    return i + 1; 
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

int countDocGia(TREE_DocGia root) {
    if (root == nullptr) return 0;
    return 1 + countDocGia(root->pLeft) + countDocGia(root->pRight);
}

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

    for (int i = 0; i < n; i++) {
        cout << arr[i]->maThe << " - "
             << arr[i]->ho << " " << arr[i]->ten << endl;
    }
    delete[] arr; 
}

//-------------------
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
//-------------------