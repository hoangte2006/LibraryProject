#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
#include <iostream>
#include <cstring>

using namespace std;
//
// -- Code Hoang Te -- 
//

#include <cstdlib>


// ---(Loc va Xao) ---
static int khoMaThe[SO_LUONG_MA];
static int topKho = -1;
static bool usedMaThe[GIOI_HAN_MANG]; // false: chua su dung

void taoMangCanBang(int arr[], int start, int end, int* result, int& index) {
    if (start > end) return;
    int mid = start + (end - start) / 2;
    result[index++] = arr[mid];
    taoMangCanBang(arr, start, mid - 1, result, index);
    taoMangCanBang(arr, mid + 1, end, result, index);
}

void danhDauMaThe(TREE_DocGia root) {
    if (root == nullptr) return;
    danhDauMaThe(root->pLeft);
    usedMaThe[root->maThe] = true;
    danhDauMaThe(root->pRight);
}

void khoiTaoKhoMaThe(TREE_DocGia root) {
    for (int i = 0; i < GIOI_HAN_MANG; i++) usedMaThe[i] = false;
    danhDauMaThe(root);
    
    int maTheTrong[SO_LUONG_MA]; 
    int count = 0;
    for (int i = MIN_MA_THE; i <= MAX_MA_THE; i++) {
        if (!usedMaThe[i]) {
            maTheTrong[count++] = i;
        }
    }
    
    int* balancedArr = new int[count];
    int index = 0;
    // Chia doi de tao thu tu cay nhi phan tim kiem can bang hoan hao (O(logN))
    taoMangCanBang(maTheTrong, 0, count - 1, balancedArr, index);
    
    topKho = -1;
    for (int i = count - 1; i >= 0; i--) { // Đẩy ngược để lúc lấy ra lấy theo thứ tự từ gốc
        khoMaThe[++topKho] = balancedArr[i];
    }
    delete[] balancedArr;
}

// O(1) pop
int taoMaTheMoi(TREE_DocGia root) {
    if (topKho >= 0) {
        return khoMaThe[topKho--]; 
    }
    return -1;
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
void nhapDocGia(TREE_DocGia &root) {
    int maThe = taoMaTheMoi(root);

    DocGia* docgia = taoDocGia(maThe);

    cout << "Ma the tu dong: " << maThe << endl;

    nhapThongTinDocGia(docgia);

    if (themDocGia(root, docgia)) {
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
    
    newDocGia->pRight = nullptr; 
    newDocGia->pLeft = nullptr; 
    
    newDocGia->dsMuonTra.pHead = nullptr; 
    newDocGia->dsMuonTra.pTail = nullptr;

    return newDocGia; 
}

bool themDocGia(TREE_DocGia& root, DocGia* newnode) {
    if (root == nullptr) {
        root = newnode;
        return true;
    }

    if (newnode->maThe == root->maThe) {
        cout << "Ma the da ton tai! Them doc gia that bai.\n";
        delete newnode;
        return false;
    }       

    if (newnode->maThe < root->maThe) {
        return themDocGia(root->pLeft, newnode);
    } else {
        return themDocGia(root->pRight, newnode);
    }
}


// Ham tra ve con tro den doc gia co ma the giong ma the truyen vao.
DocGia* timDocGia(const TREE_DocGia root, int maThe) {
    /*/    
    if (root == nullptr) return nullptr; 

    if (maThe == root->maThe) {
        return root; // trả về địa chỉ của nút hiện tại.
    }

    if (maThe < root->maThe) {
        return timDocGia(root->pLeft, maThe); 
    } else {
        return timDocGia(root->pRight, maThe); 
    }
    /*/

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

void inDanhSachDocGia(const TREE_DocGia root) {
    if (root == nullptr) return; 

    inDanhSachDocGia(root->pLeft);

    cout << "Ma the: " << root->maThe 
         << " | Ho ten: " << root->ho << " " << root->ten 
         << " | Gioi tinh: " << root->giotinh 
         << " | Trang thai the: " << (root->trangThaiThe == 1 ? "Hoat dong" : "Khoa") 
         << endl; 
        
    inDanhSachDocGia(root->pRight);
}


int demSachDangMuon(const DocGia* docGia) {
    if (docGia == nullptr) return 0;

    int count = 0;
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        if (temp->trangThai == 0) {
            count++;
        }
        temp = temp->pNext;
    }

    return count;
}

// Ham nay lam nhiem vu giai phong bo nho cho toan bo danh sach muon tra cua mot doc gia.
// duoc goi khi xoa doc gia de tranh mat sach va giai phong bo nho da cap phat cho cac thong tin muon tra cua doc gia do sau khi da xoa doc gia khoi cay.
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

// Ham dangMuonSach kiem tra xem doc gia co dang muon cuon sach co ma maSach hay khong.
// neu co thi tra ve true, neu khong thi tra ve false.
bool dangMuonSach(const DocGia* docGia, const char* maSach) {
    if (docGia == nullptr) return false; 

    MuonTra* temp = docGia->dsMuonTra.pHead; 
  
    while (temp != nullptr) {
        if (strcmp(temp->maSach, maSach) == 0 && temp->trangThai == 0) {
            return true;
        }   
        temp = temp->pNext;
    }

    return false; 
}

bool soSanhNgay(Ngay muon, Ngay tra) {
    if (tra.nam < muon.nam) 
        return false;
    if (tra.nam == muon.nam && tra.thang < muon.thang) 
        return false;
    if (tra.nam == muon.nam && tra.thang == muon.thang && tra.ngay < muon.ngay) 
        return false;

    return true;
}

// Ham tim node trai nho nhat lon hon node can xoa.
DocGia* timMin(TREE_DocGia root)
{
    if (root == nullptr) return nullptr; 
    while (root->pLeft != nullptr)
        root = root->pLeft;
    return root;
}

// 3 truong hop: xoa node la node la, node co 1 con, node co 2 con.
bool xoaDocGia(TREE_DocGia &root, int maThe) {
    if (root == nullptr) return false;

    if (maThe < root->maThe) {
        return xoaDocGia(root->pLeft, maThe); 
    } else if (maThe > root->maThe) {
        return xoaDocGia(root->pRight, maThe); 
    } 
    else {

        // Yeu cau Thầy: Có lịch sử (kể cả đã trả) pHead trống mới được xóa
        if (root->dsMuonTra.pHead != nullptr) {
            cout << "Khong the xoa doc gia nay vi da co lich su muon tra (Da tra sach cung khong the xoa)!\n";
            return false;
        }

        // tai su dung
        khoMaThe[++topKho] = root->maThe;

        // case 1; khong co con 
        if (root->pLeft == nullptr && root->pRight == nullptr) {
            giaiPhongMuonTra(root->dsMuonTra);
            delete root; 
            root = nullptr; 
            return true; 
        }

        // case 2; co 1 con phai
        else if (root->pLeft == nullptr) {
            DocGia* temp = root; // luu dia chi cua node can xoa 
            root = root->pRight;
            giaiPhongMuonTra(temp->dsMuonTra);
            delete temp;
            return true;
        }

        // case 2; co 1 con trai
        else if (root->pRight == nullptr) {
            DocGia* temp = root; // luu dia chi cua node can xoa 
            root = root->pLeft;
            giaiPhongMuonTra(temp->dsMuonTra);
            delete temp;
            return true;
        }

        // case 3;  2 con
        else {
            DocGia* replacementParent = root;
            DocGia* replacement = root->pRight;

            while (replacement->pLeft != nullptr) {
                replacementParent = replacement;
                replacement = replacement->pLeft;
            }

            if (replacementParent == root) {
                // node the mang la con phai truc tiep cua node can xoa
                replacement->pLeft = root->pLeft;

                giaiPhongMuonTra(root->dsMuonTra);
                delete root;
                root = replacement;
            } 
            else {
                // node the mang sau ben trai cua node can xoa
                replacementParent->pLeft = replacement->pRight;

                replacement->pLeft = root->pLeft;
                replacement->pRight = root->pRight;

                giaiPhongMuonTra(root->dsMuonTra); 
                delete root;
                root = replacement;
            }
        }
        return true; 
    }
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
// Dung neu mang day,tranh de quy them
// Nho goi ham co: int n = 0; BSTtoArray(root, arr, n);
void BSTtoArray(TREE_DocGia root, DocGia* arr[], int &index) {
    if (root == nullptr || index >= MAX_DOCGIA) return;    

    BSTtoArray(root->pLeft, arr, index);

    if (index < MAX_DOCGIA)
        arr[index++] = root; // luu dia chi;

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
// FOR se sap xep lai mang sao cho cac phan tu nho hon pivot nam ben trai va cac phan tu lon hon pivot nam ben phai
// sau do tra ve chi so cua pivot sau khi da duoc sap xep lai.
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

int demDocGia(TREE_DocGia root) {
    if (root == nullptr) return 0;
    return 1 + demDocGia(root->pLeft) + demDocGia(root->pRight);
}

void BSTtoArrayDynamic(TREE_DocGia root, DocGia** arr, int &index) {
    if (root == nullptr) return;    
    BSTtoArrayDynamic(root->pLeft, arr, index);
    arr[index++] = root;
    BSTtoArrayDynamic(root->pRight, arr, index);
}

void inTheoTen(TREE_DocGia root)
{
    int tongSo = demDocGia(root);
    if (tongSo == 0) {
        cout << "Danh sach rong!\n";
        return;
    }
    
    // Dung cap phat 
    DocGia** arr = new DocGia*[tongSo];
    int n = 0;
    BSTtoArrayDynamic(root, arr, n);
    
    quickSortDocGia(arr, 0, n - 1);

    for (int i = 0; i < n; i++) {
        cout << arr[i]->maThe << " - "
             << arr[i]->ho << " " << arr[i]->ten << endl;
    }
    delete[] arr; // Giai phong bo nho
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
