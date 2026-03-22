#include "Docgia.h"
#include <iostream>
#include <cstring>

using namespace std;
//
// -- Code Hoang Te -- 
//

int timMaxMaThe(TREE_DocGia root, int &maxID) {
    if (root == nullptr) return 1;

    DocGia* cur = root;
    while (cur->pRight != nullptr)
        cur = cur->pRight;
    
}

int taoMaTheMoi(TREE_DocGia root) {
    int maxID = 0;
    timMaxMaThe(root, maxID);
    return maxID + 1; // ma the moi se la maxID + 1 de dam bao tinh duy nhat va tang dan cua ma the doc gia.
}

void nhapDocGia(TREE_DocGia &root) {
    int maThe = taoMaTheMoi(root);

    DocGia* docgia = taoDocGia(maThe);

    cin.ignore();

    cout << "Ma the tu dong: " << maThe << endl;

    cout << "Nhap ho: ";
    cin.getline(docgia->ho, 50);

    cout << "Nhap ten: ";
    cin.getline(docgia->ten, 20);

    cout << "Nhap gioi tinh: ";
    cin.getline(docgia->giotinh, 5);

    if (themDocGia(root, docgia)) {
        cout << "Them doc gia thanh cong!\n";
    }
}


DocGia* taoDocGia(int maThe) {
    DocGia* newDocGia = new DocGia; // tạo một con trỏ newDocGia và cấp phát bộ nhớ cho một đối tượng DocGia mới. Đây sẽ là độc giả mới mà chúng ta muốn thêm vào cây độc giả hiện tại. Sau khi cấp phát bộ nhớ, newDocGia sẽ trỏ đến vùng nhớ chứa thông tin của độc giả mới mà chúng ta sẽ nhập liệu từ người dùng.

    newDocGia->maThe = maThe;
    newDocGia->trangThaiThe = 1; // mặc định khi tạo mới là thẻ đang hoạt động
    
    // con trỏ pRight được khởi tạo là nullptr, cho biết rằng chưa có con trỏ nào được gán vào nó.
    // hiểu đơn giản là khi tạo mới một đối tượng DocGia, con trỏ pRight sẽ không trỏ đến bất kỳ đối tượng nào khác, và được đặt thành nullptr để biểu thị rằng nó chưa được sử dụng hoặc chưa có giá trị hợp lệ.
    newDocGia->pRight = nullptr; // tay trái 
    newDocGia->pLeft = nullptr; // tay phải của đọc giả mới.
    
    // khởi tạo con trỏ pHead của danh sách mượn trả là nullptr, cho biết rằng chưa có giao dịch mượn trả nào được thêm vào danh sách này.
    newDocGia->dsMuonTra.pHead = nullptr; 
    newDocGia->dsMuonTra.pTail = nullptr;

    return newDocGia; // trả về con trỏ đến đối tượng DocGia mới được tạo ra, 
    // cho phép người gọi có thể sử dụng đối tượng này để thực hiện các thao tác tiếp theo như thêm vào cây 
    // hoặc quản lý thông tin của độc giả.
}

// Viết thuần sẻ là DocGia* &root: tác động trực tiếp lên ô nhớ.
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

DocGia* timDocGia(const TREE_DocGia root, int maThe) {
    /*/ Quy tắc là: Đi hết bên trái, rồi mới in gốc, rồi mới đi bên phải.
    
    if (root == nullptr) return nullptr; // trả về nullptr nếu cây rỗng hoặc đã duyệt hết cây mà không tìm thấy độc giả có mã thẻ trùng khớp.   

    if (maThe == root->maThe) {
        return root; // trả về địa chỉ của nút hiện tại.
    }

    if (maThe < root->maThe) {
        return timDocGia(root->pLeft, maThe); // gọi đệ quy hàm timDocGia để tiếp tục tìm kiếm trong cây con bên trái của nút hiện tại.
    } else {
        return timDocGia(root->pRight, maThe); // gọi đệ quy hàm timDocGia để tiếp tục tìm kiếm trong cây con bên phải của nút hiện tại.
    }
    /*/

    // viết thuần tuý bằng cách duyệt vòng lặp
    DocGia* current = root; // khởi tạo con trỏ current, an toàn hơn dùng root trực tiếp để tránh thay đổi root khi duyệt cây.
    while (current != nullptr) {
        if (maThe < current->maThe) {
            current = current->pLeft; 
        } else 
            if (maThe > current->maThe) {
                current = current->pRight; 
            } else return current; // trả về địa chỉ của nút hiện tại nếu mã thẻ trùng khớp.
    
    }
    return nullptr; // trả về nullptr nếu đã duyệt hết cây mà không tìm thấy độc giả có mã thẻ trùng khớp.
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
        if (temp->trangThai == 0 || temp->trangThai == 2) {
            count++;
        }
        temp = temp->pNext;
    }

    return count;
}

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

bool dangMuonSach(const DocGia* docGia, const char* maSach) {
    if (docGia == nullptr) return false; // check an toan khi docGia la nullptr, neu docGia la nullptr thi khong co thong tin muon tra nao de kiem tra, nen tra ve false de biet rang khong tim thay thong tin muon tra cho doc gia do.

    MuonTra* temp = docGia->dsMuonTra.pHead; 
  
    // trung ma va da muon thi trangThai se la 0, neu trangThai la 1 thi da tra, neu trangThai la 2 thi lam mat, nen chi can check trangThai == 0 la dang muon.
    while (temp != nullptr) {
        if (strcmp(temp->maSach, maSach) == 0 && temp->trangThai != 1) { // kiem tra neu ma sach trong thong tin muon tra trung khop voi ma sach can kiem tra, va trang thai cua thong tin muon tra do khong phai la da tra (trangThai != 1), thi co nghia la doc gia dang muon cuon sach do, nen tra ve true de biet rang doc gia dang muon cuon sach do.  
            return true;
        }   
        temp = temp->pNext;
    }

    return false; 
}

bool soSanhNgay(Ngay a, Ngay b) {
    if (b.nam < a.nam) 
        return false;
    if (b.nam == a.nam && b.thang < a.thang) 
        return false;
    if (b.nam == a.nam && b.thang == a.thang && b.ngay < a.ngay) 
        return false;

    return true;
}

DocGia* timMin(TREE_DocGia root)
{
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
        }

        // case 2; co 1 con trai
        else if (root->pRight == nullptr) {
            DocGia* temp = root; // luu dia chi cua node can xoa vao con tro temp de sau khi giai phong bo nho cho node do, ta se khong the truy cap den node do nua, nen can luu tru dia chi cua node do truoc khi giai phong bo nho.
            root = root->pLeft;
            delete temp;
        }

        // case 3;  2 con
        // quy tat don gian: chon X nho nhat trong nhanh phai node 3 nhung lon lon node 3.
        else {
            DocGia* temp = timMin(root->pRight); // tim node co gia tri nho nhat trong cay con ben phai cua node can xoa, node do se la node the mang de thay the cho node can xoa trong cay nhi phan tim kiem, vi khi thay the node can xoa bang node the mang, thi van dam bao duoc tinh chat cua cay nhi phan tim kiem la: cac node trong cay con ben trai cua mot node se co gia tri nho hon gia tri cua node do, va cac node trong cay con ben phai se co gia tri lon hon gia tri cua node do.
            // temp nhan dia chi X.
            root->maThe = temp->maThe; // gan gia tri maThe cua node the mang vao node can xoa, de node can xoa se co gia tri maThe moi
            strcpy(root->ho, temp->ho);
            strcpy(root->ten, temp->ten);
            strcpy(root->giotinh, temp->giotinh);
            root->trangThaiThe = temp->trangThaiThe;

            // root->dsMuonTra = temp->dsMuonTra; lam the nay sai boi vi: dsMuonTra la mot danh sach lien ket don, neu lam the nay thi root va temp se cung tro den cung mot danh sach muon tra, nen khi thay doi danh sach muon tra cua root thi se lam thay doi danh sach muon tra cua temp, va nguoc lai, do do ta khong the lam the nay de gan dsMuonTra cua temp vao dsMuonTra cua root, ma phai xoa node temp trong cay nhi phan tim kiem sau khi da gan gia tri cua temp vao root, de tranh viec node temp van con trong cay nhi phan tim kiem sau khi da duoc gan gia tri cua temp vao root, de dam bao tinh chinh xac va tinh toan dung khi xoa node can xoa trong cay nhi phan tim kiem.
             // sau khi gan gia tri cua node the mang vao node can xoa, ta se goi de quy ham xoaDocGia de xoa node the mang (temp) trong cay nhi phan tim kiem, vi sau khi da gan gia tri cua node the mang vao node can xoa, thi node the mang se khong con la node the mang nua, ma se la node bi trung (vi co cung gia tri maThe), nen ta can goi de quy ham xoaDocGia de xoa node the mang (temp) trong cay nhi phan tim kiem, de dam bao tinh chinh xac va tinh toan dung khi xoa node can xoa trong cay nhi phan tim kiem.

            // xoa node X
            xoaDocGia(root->pRight, temp->maThe);
        }
    }

    return true; // xoa thanh cong 
}

void hieuChinhDocGia(TREE_DocGia root, int maThe)
{
    DocGia* docgia = timDocGia(root, maThe);

    if (docgia == nullptr) {
        cout << "Khong tim thay doc gia!\n";
        return;
    }

    cout << "Nhap ho moi: ";
    cin.getline(docgia->ho, 50);

    cout << "Nhap ten moi: ";
    cin.getline(docgia->ten, 20);

    cout << "Nhap gioi tinh: ";
    cin.getline(docgia->giotinh, 5);

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

void BSTtoArray(TREE_DocGia root, DocGia* arr[], int &index) {
    if (root == nullptr) return;
    
    BSTtoArray(root->pLeft, arr, index);
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

int partition(DocGia* arr[], int left, int right) {
    int pivotIndex = medianOfThree(arr, left, right);

    swap(arr[pivotIndex], arr[right]);

    DocGia* pivot = arr[right];
    int i = left - 1;   

    for (int j = left; j < right; j++) {
        if (soSanhTen(arr[j], pivot) < 0) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

void quickSortDocGia(DocGia* arr[], int left, int right) {
    if (left >= right) return;

    // tam 10-15 la nguyen tac chung de chon ngung de quy va chuyen sang insertion sort cho mang nho, vi insertion sort se nhanh hon quick sort khi sap xep mang nho do insertion sort co chi phi thuc hien thap hon quick sort trong truong hop mang nho, do do khi sap xep mang nho thi insertion sort se hieu qua hon quick sort, nen khi do dai cua mang con lai (right - left + 1) nho hon hoac bang 10 thi ta se chuyen sang insertion sort de tang hieu suat sap xep.
    if (right - left + 1 <= 10) {
        insertionSortDocGia(&arr[left], right - left + 1); // su dung insertion sort cho mang nho de tang hieu suat.
    } 
    else {
        if (left < right) {
            int pivotCurrent = partition(arr, left, right);
            quickSortDocGia(arr, left, pivotCurrent - 1);
            quickSortDocGia(arr, pivotCurrent + 1, right);
        }
    }
}

//-------------------

void inTheoTen(TREE_DocGia root)
{
    DocGia* arr[1000];
    int n = 0;

    BSTtoArray(root, arr, n);
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

//-------------------


//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//