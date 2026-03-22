#include "Docgia.h"
#include <iostream>
#include <cstring>

using namespace std;
//
// -- Code Hoang Te -- 
//

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

int taoMaTheMoi(TREE_DocGia root) {
    if (root == nullptr) return 1; // nếu cây rỗng, trả về mã thẻ mới là 1.

    int maxMaThe = root->maThe; // khởi tạo maxMaThe với mã thẻ của nút gốc hiện tại.

    int leftMax = taoMaTheMoi(root->pLeft); // đệ quy để tìm mã thẻ lớn nhất trong cây con bên trái.
    int rightMax = taoMaTheMoi(root->pRight); // đệ quy để tìm mã thẻ lớn nhất trong cây con bên phải.

    if (leftMax > maxMaThe) {
        maxMaThe = leftMax; // cập nhật maxMaThe nếu mã thẻ lớn nhất trong cây con bên trái lớn hơn maxMaThe hiện tại.
    }

    if (rightMax > maxMaThe) {
        maxMaThe = rightMax; // cập nhật maxMaThe nếu mã thẻ lớn nhất trong cây con bên phải lớn hơn maxMaThe hiện tại.
    }

    return maxMaThe + 1; // trả về mã thẻ mới là mã thẻ lớn nhất tìm được cộng thêm 1, đảm bảo rằng mã thẻ mới sẽ là duy nhất và không trùng lặp với bất kỳ mã thẻ nào đã tồn tại trong cây.
}



//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//