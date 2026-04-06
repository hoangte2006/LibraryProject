#include "Docgia.h"
#include <iostream>
#include <cstring>

using namespace std;
//
// -- Code Hoang Te -- 
//

// Ham tao ma the moi - tao ma the theo thu tu tang dan
// Tu doc gia so the lon nhat
int taoMaTheMoi(TREE_DocGia root) {
    if (root == nullptr) return 1;

    // uu tien BST
    DocGia* cur = root;
    while (cur->pRight != nullptr)
        cur = cur->pRight;

    return cur->maThe + 1;
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
    
    newDocGia->pRight = nullptr; // tay trái 
    newDocGia->pLeft = nullptr; 
    
    newDocGia->dsMuonTra.pHead = nullptr; 
    newDocGia->dsMuonTra.pTail = nullptr;

    return newDocGia; // trả về con trỏ
}

// Ham themDocGia 
bool themDocGia(TREE_DocGia& root, DocGia* newnode) {
    // newnode là con trỏ đến đối tượng DocGia mới được tạo ra, chứa thông tin của độc giả cần thêm vào cây.
    if (root == nullptr) {
        root = newnode; // gán nút mới làm nút gốc của cây.
        return true; // trả về true để báo hiệu rằng việc thêm độc giả mới đã thành công.
    }

    // nếu mã thẻ của nút mới trùng với mã thẻ của nút gốc, điều này có nghĩa là đã tồn tại một độc giả với cùng mã thẻ trong cây, nên việc thêm độc giả mới sẽ thất bại để tránh trùng lặp thông tin.
    if (newnode->maThe == root->maThe) {
        cout << "Ma the da ton tai! Them doc gia that bai.\n";
        delete newnode; // giải phóng bộ nhớ đã cấp phát cho newnode dòng trên để tránh rò rỉ bộ nhớ, vì chúng ta không thể thêm độc giả mới vào cây do trùng mã thẻ, nên chúng ta cần giải phóng bộ nhớ đã cấp phát cho đối tượng DocGia mới này để tránh lãng phí tài nguyên hệ thống.
        return false; // trả về false để báo hiệu rằng việc thêm độc giả mới đã thất bại do trùng mã thẻ.
    }       

    if (newnode->maThe < root->maThe) {
        // nếu mã thẻ của nút mới nhỏ hơn mã thẻ của nút gốc, điều này có nghĩa là nút mới nên được thêm vào cây con bên trái của nút gốc.
        return themDocGia(root->pLeft, newnode); // gọi đệ quy hàm themDocGia để tiếp tục tìm vị trí thích hợp trong cây con bên trái để thêm nút mới.
    } else {
        // nếu mã thẻ của nút mới lớn hơn mã thẻ của nút gốc, điều này có nghĩa là nút mới nên được thêm vào cây con bên phải của nút gốc.
        return themDocGia(root->pRight, newnode); // gọi đệ quy hàm themDocGia để tiếp tục tìm vị trí thích hợp trong cây con bên phải để thêm nút mới. 
    }
    // mỗi lần gọi đệ quy thì root->pLeft sẻ là root mới, chạy if (root == nullptr) để thêm node.
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
            } else return current; // giong thi tra ve.
    }
    return nullptr; 
}

void inDanhSachDocGia(const TREE_DocGia root) {
    // đệ quy tới khi gặp nút nullptr, tức là đã duyệt hết cây hoặc cây rỗng, thì dừng lại và trả về để không in gì cả.
    // trở lên lần đệ quy trước đó, khi đã in xong danh sách độc giả trong cây con bên trái, sẽ in thông tin của nút gốc hiện tại, sau đó tiếp tục đệ quy để in danh sách độc giả trong cây con bên phải.
    if (root == nullptr) return; 
    inDanhSachDocGia(root->pLeft); // gọi đệ quy từ cây con bên trái để in danh sách độc giả trong cây con bên trái trước khi in thông tin của nút gốc.

    cout << "Ma the: " << root->maThe 
         << " | Ho ten: " << root->ho << " " << root->ten 
         << " | Gioi tinh: " << root->giotinh 
         << " | Trang thai the: " << (root->trangThaiThe == 1 ? "Hoat dong" : "Khoa") 
         << endl; // in thông tin của nút gốc sau khi đã in xong danh sách độc giả trong cây con bên trái.  
        
    inDanhSachDocGia(root->pRight); // khi đã in xong thông tin của nút gốc, tiếp tục gọi đệ quy từ cây con bên phải để in danh sách độc giả trong cây con bên phải.
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
        MuonTra* next = temp->pNext; // Luu dia chi cua muonTra tiep theo
        delete temp;    // giai phong bo nho da cap phat cho muonTra hien tai
        temp = next;    // di chuyen con tro temp den muonTra tiep theo de tiep tuc giai phong
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
    if (docGia == nullptr) return false; // check an toan khi docGia la nullptr, neu docGia la nullptr thi khong co thong tin muon tra nao de kiem tra, nen tra ve false de biet rang khong tim thay thong tin muon tra cho doc gia do.

    MuonTra* temp = docGia->dsMuonTra.pHead; 
  
    // trung ma va da muon thi trangThai se la 0, neu trangThai la 1 thi da tra, neu trangThai la 2 thi lam mat, nen chi can check trangThai == 0 la dang muon.
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
    if (root == nullptr) return false; // tra ve false neu cay rong, khong tim thay doc gia de xoa.

    if (maThe < root->maThe) {
        return xoaDocGia(root->pLeft, maThe); // goi de quy de xoa trong cay con ben trai neu maThe can xoa nho hon maThe cua node goc.
    } else if (maThe > root->maThe) {
        return xoaDocGia(root->pRight, maThe); // goi de quy de xoa trong cay con ben phai neu maThe can xoa lon hon maThe cua node goc.
    } 
    else {
        // tim thay:
        if (root->dsMuonTra.pHead != nullptr) { // neu doc gia can xoa dang muon sach, thi khong cho xoa de tranh mat sach.
            cout << "Khong the xoa doc gia nay vi dang muon sach!\n";
            return false; // tra ve false de biet rang khong the xoa doc gia do vi dang muon sach.
        }

        // case 1; khong co con 
        if (root->pLeft == nullptr && root->pRight == nullptr) {
            delete root; // giai phong bo nho cho node can xoa.
            root = nullptr; // dat con tro root ve nullptr de biet rang node do da duoc xoa khoi cay.
            return true; // tra ve true de biet rang da xoa doc gia thanh cong.
        }

        // case 2; co 1 con phair
        else if (root->pLeft == nullptr) {
            DocGia* temp = root; // luu dia chi cua node can xoa vao con tro temp de sau khi giai phong bo nho cho node do, ta se khong the truy cap den node do nua, nen can luu tru dia chi cua node do truoc khi giai phong bo nho.
            root = root->pRight;
            delete temp;
            return true;
        }

        // case 2; co 1 con trai
        else if (root->pRight == nullptr) {
            DocGia* temp = root; // luu dia chi cua node can xoa vao con tro temp de sau khi giai phong bo nho cho node do, ta se khong the truy cap den node do nua, nen can luu tru dia chi cua node do truoc khi giai phong bo nho.
            root = root->pLeft;
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

                delete root;
                root = replacement;
            } 
            else {
                // node the mang sau ben trai cua node can xoa
                replacementParent->pLeft = replacement->pRight;

                replacement->pLeft = root->pLeft;
                replacement->pRight = root->pRight;

                delete root;
                root = replacement;
            }
        }
        return true; // xoa thanh cong 
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
    return i + 1; // +1 la vi tri sau khi sap xep, pivot se nam o vi tri i+1.
}

void quickSortDocGia(DocGia* arr[], int left, int right) {
    if (left >= right) return;

    // tam 10-15 la nguyen tac chung de chon ngung de quy va chuyen sang insertion sort cho mang nho, vi insertion sort se nhanh hon quick sort khi sap xep mang nho do insertion sort co chi phi thuc hien thap hon quick sort trong truong hop mang nho, do do khi sap xep mang nho thi insertion sort se hieu qua hon quick sort, nen khi do dai cua mang con lai (right - left + 1) nho hon hoac bang 10 thi ta se chuyen sang insertion sort de tang hieu suat sap xep.
    if (right - left + 1 <= 10) {
        insertionSortDocGia(&arr[left], right - left + 1); // su dung insertion sort cho mang nho de tang hieu suat.
    } 
    else {
            int pivotCurrent = partition(arr, left, right);
            quickSortDocGia(arr, left, pivotCurrent - 1);
            quickSortDocGia(arr, pivotCurrent + 1, right);
    }
}

//-------------------

void inTheoTen(TREE_DocGia root)
{
    DocGia* arr[MAX_DOCGIA];
    int n = 0;

    BSTtoArray(root, arr, n);
    if (n > 0)
        quickSortDocGia(arr, 0, n - 1);

    for (int i = 0; i < n; i++) {
        cout << arr[i]->maThe << " - "
             << arr[i]->ho << " " << arr[i]->ten << endl;
    }
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

int tinhQuaHan


//-------------------


//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//