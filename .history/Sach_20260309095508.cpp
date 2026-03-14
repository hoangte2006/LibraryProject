#include "Sach.h"
#include <iostream>
#include <cstring>
#include <limits>
#include "Docgia.h"

using namespace std;

const int MAX_SACH_MUON = 3;

//
// -- Code Hoang Te -- 
//


int timViTriChen(const ListDauSach &ds, const char* tenSach) {
    int left = 0;
    int right = ds.n - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1); // Tinh mid tranh tran so (overflow)
        int cmp = strcmp(ds.nodes[mid]->tenSach, tenSach);// đi đén địa chỉ đó để thực hiện so sánh.

        if (cmp == 0) // trả về 0 khi nội dung giống nghe.
            return mid;
        else if (cmp < 0 ) // ví dụ: "A" < "B")- giá trị của bảng mã ASCII
            left = mid + 1;
        else if (cmp > 0) // ví dụ: "C" > "B")
            right = mid - 1;
    }
    return left; // dừng lại vị trí lớn hơn tên sách.
}

DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->ISBN, ISBN) == 0) 
            return ds.nodes[i];
    }
    return nullptr;
}

void themDauSach(ListDauSach &ds) {    
    if (ds.n >= MAX_DAUSACH) {
        cout << "Danh sach dau sach da day!\n";
        return;
    }

    DauSach* p = new DauSach; // tạo một con trỏ p và cấp phát bộ nhớ cho một đối tượng DauSach mới. Đây sẽ là đầu sách mới mà chúng ta muốn thêm vào danh sách đầu sách hiện tại. Sau khi cấp phát bộ nhớ, p sẽ trỏ đến vùng nhớ chứa thông tin của đầu sách mới mà chúng ta sẽ nhập liệu từ người dùng.

    cout << "Nhap ISBN: ";
    cin.getline(p->ISBN, 20); 

    // kiểm tra trùng ISBN
    if (timTheoISBN(ds, p->ISBN) != nullptr) {
        cout << "ISBN da ton tai! Them dau sach that bai.\n";
        delete p; // giải phóng bộ nhớ đã cấp phát cho p dòng trên để tránh rò rỉ bộ nhớ.
        return;
    }

    cout << "Nhap ten sach: ";
    cin.getline(p->tenSach, 100); // giới hạn 99 ký tự, 1 ký tự \n

    cout << "Nhap tac gia: ";
    cin.getline(p->tacGia, 50);

    cout << "Nhap the loai: ";
    cin.getline(p->theLoai, 30);

    cout << "Nhap so trang: ";
    cin >> p->soTrang;

    cout << "Nhap nam xuat ban: ";
    cin >> p->namXuatBan;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    p->soLuotMuon = 0;
    p->dsSach.pHead = p->dsSach.pTail = nullptr;
    p->dsSach.tongSoSach = 0;

    // mấy trường hợp 0,1,2, pos = ds.n xử lý ngon lành.
    int pos = timViTriChen(ds, p->tenSach); 

    for (int i = ds.n; i > pos; i--) {
        ds.nodes[i] = ds.nodes[i - 1];
    }
    ds.nodes[pos] = p;
    ds.n++;

    cout << "Them dau sach thanh cong" << endl;
}

void inDanhSachDauSach(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach nay rong! \n";
        return;
    }

    for (int i = 0; i < ds.n; i++) {
        cout << i + 1 << ". "
             << ds.nodes[i]->tenSach
             << " | ISBN: " << ds.nodes[i]->ISBN
             << " | So cuon: " << ds.nodes[i]->dsSach.tongSoSach
             << endl;    }
}

void addTailSach(ListSach &list, Sach* newSach) {
    newSach->pNext = nullptr;
    // ds rong 
    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newSach;
    } else { 
        list.pTail->pNext = newSach;
        list.pTail = newSach;
    }

    list.tongSoSach++;
}

void addTailMuonTra(ListMuonTra &list, MuonTra* newNode) {
    newNode->pNext = nullptr;

    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newNode;
    } else {
        list.pTail->pNext = newNode;
        list.pTail = newNode;
    }
}

void themCuonSach(DauSach* dauSach) {
    if (dauSach == nullptr) return; // kiểm tra con tro dauSach có hợp lệ hay không.

    Sach* newSach = new Sach; // tạo một con trỏ newSach và cấp phát bộ nhớ cho một đối tượng Sach mới. Đây sẽ là cuốn sách mới mà chúng ta muốn thêm vào danh sách sách của đầu sách hiện tại. Sau khi cấp phát bộ nhớ, newSach sẽ trỏ đến vùng nhớ chứa thông tin của cuốn sách mới mà chúng ta sẽ nhập liệu từ người dùng.
    
    cout << "Nhap ma sach: ";
    cin.getline(newSach->maSach, sizeof(newSach->maSach));    // có nghĩa là khi người dùng nhập mã sách, giá trị đó sẽ được lưu vào thành phần maSach của cấu trúc Sach mà newSach đang trỏ tới.

    Sach* temp = dauSach->dsSach.pHead; // lấy con trỏ tạm để duyệt danh sách sách của đầu sách hiện tại.
    while (temp != nullptr) {
        if (strcmp(temp->maSach, newSach->maSach) == 0) {
            cout << "Ma sach da ton tai! Them cuon sach that bai.\n";
            delete newSach; // giải phóng bộ nhớ đã cấp phát cho newSach dòng trên để tránh rò rỉ bộ nhớ.
            return;
        }
        temp = temp->pNext; // di chuyển con trỏ tạm đến cuốn sách tiếp theo trong danh sách.
    }
    
    cout << "Nhap vi tri: ";
    cin.getline(newSach->viTri, sizeof(newSach->viTri));

    //newSach->trangThai = 0; // mặc định khi thêm mới, cuốn sách có trạng thái "cho mượn được".
    newSach->pNext = nullptr; // khởi tạo con trỏ pNext của newSach thành nullptr vì nó sẽ là cuốn sách cuối cùng trong danh sách.

    addTailSach(dauSach->dsSach, newSach);
    
    cout << "Them cuon sach thanh cong!\n";
}

bool xoaCuonSach(DauSach* dauSach, const char* maSach) {
    if (dauSach == nullptr || dauSach->dsSach.pHead == nullptr) return false; 

    Sach* prev = nullptr; // con trỏ prev để theo dõi cuốn sách trước đó trong danh sách khi duyệt.
    // con trỏ current để duyệt danh sách sách của đầu sách hiện tại, bắt đầu từ cuốn sách đầu tiên (pHead).
    Sach* current = dauSach->dsSach.pHead; 

    while (current != nullptr) {
        // so sánh mã sách của cuốn sách hiện tại với mã sách cần xóa. Nếu tìm thấy cuốn sách có mã sách trùng khớp, tiến hành xóa cuốn sách đó khỏi danh sách.
        if (strcmp(current->maSach, maSach) == 0) {
            // Không thể xoá nếu cuốn sách đang được mượn (trangThai == 1).
            if (current->trangThai == 1) {
                cout << "Khong the xoa. Sach dang duoc muon!\n";
                return false;
            }
            // neu cuon sach can xoa la cuon sach duy nhat trong danh sach (cung la pHead va pTail), cap nhat ca pHead va pTail ve nullptr de bieu thi danh sach sach cua dau sach do da rong sau khi xoa cuon sach nay.
            if (current == dauSach->dsSach.pHead && current == dauSach->dsSach.pTail) {
                dauSach->dsSach.pHead = nullptr;
                dauSach->dsSach.pTail = nullptr;
                
            } else {
                // nếu cuốn sách cần xóa là cuốn sách đầu tiên trong danh sách (pHead), cập nhật pHead để trỏ đến cuốn sách tiếp theo.
                if (current == dauSach->dsSach.pHead) {
                    dauSach->dsSach.pHead = current->pNext; // cập nhật pHead để trỏ đến cuốn sách tiếp theo sau khi xóa cuốn sách đầu tiên.
                } else {
                    // nếu cuốn sách cần xóa không phải là cuốn sách đầu tiên, 
                    // cập nhật con trỏ pNext của cuốn sách trước đó (prev) để trỏ đến cuốn sách tiếp theo sau khi xóa cuốn sách hiện tại.
                    prev->pNext = current->pNext; 
                }
                // nếu cuốn sách cần xóa là cuốn sách cuối cùng trong danh sách (pTail), cập nhật pTail để trỏ đến cuốn sách trước đó.
                if (current == dauSach->dsSach.pTail) {
                    dauSach->dsSach.pTail = prev; // cập nhật pTail để trỏ đến cuốn sách trước đó sau khi xóa cuốn sách cuối cùng.
                }
            }

            delete current; // giải phóng bộ nhớ đã cấp phát cho cuốn sách hiện tại sau khi xóa nó khỏi danh sách.
            dauSach->dsSach.tongSoSach--;
            cout << "Xoa cuon sach thanh cong!\n";
            return true; // trả về true để báo hiệu rằng cuốn sách đã được xóa thành công.
        }
        prev = current; // cập nhật con trỏ prev để trỏ đến cuốn sách hiện tại trước khi di chuyển con trỏ current đến cuốn sách tiếp theo trong danh sách.
        current = current->pNext; // di chuyển con trỏ current đến cuốn sách tiếp theo trong danh sách để tiếp tục tìm kiếm cuốn sách cần xóa.
    }
    return false; // trả về false nếu không tìm thấy cuốn sách có mã sách trùng khớp trong danh sách, tức là cuốn sách cần xóa không tồn tại.
}

// Độ phức tập xấu nhất O(n), tốt nhất là O(1) khi đầu sách không có cuốn sách nào hoặc cuốn sách cần xóa là cuốn sách đầu tiên trong danh sách.
bool xoaDauSach(ListDauSach &ds, const char* ISBN) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->ISBN, ISBN) == 0) {
            DauSach* toDelete = ds.nodes[i]; // mục đích của việc tạo con trỏ toDelete là để lưu trữ địa chỉ của đầu sách cần xóa, giúp chúng ta dễ dàng truy cập và thao tác với đầu sách đó trong quá trình xóa, bao gồm việc kiểm tra trạng thái của các cuốn sách con, giải phóng bộ nhớ cho các cuốn sách con và cuối cùng là giải phóng bộ nhớ cho chính đầu sách cần xóa.
            // toDelete sau mỗi vòng lặp sẽ trỏ đến đầu sách có ISBN trùng khớp với ISBN cần xóa, và sau khi tìm thấy đầu sách đó, chúng ta sẽ sử dụng con trỏ toDelete để thực hiện các thao tác xóa một cách thuận tiện và rõ ràng hơn.

            Sach* temp = toDelete->dsSach.pHead; // lấy con trỏ tạm để duyệt danh sách sách của đầu sách cần xóa.

            // viet tam ham nay de day, chua co chuc nang.
            if (toDelete->dsSach.tongSoSach > 0) { 
                cout << "Khong the xoa dau sach vi van con cuon sach!\n";
                return false;
            }

            delete toDelete; // giải phóng bộ nhớ đã cấp phát cho đầu sách cần xóa sau khi đã xoá hết các cuốn sách của nó.

            // dịch mảng, dùng toDelete nên không bị mất địa chỉ của đầu sách cần xóa, giúp chúng ta dễ dàng thực hiện việc giải phóng bộ nhớ cho đầu sách đó sau khi đã xoá hết các cuốn sách con của nó.
            for (int j = i; j < ds.n - 1; j++) {
                ds.nodes[j] = ds.nodes[j + 1];
            }
            
            ds.n--;
            ds.nodes[ds.n] = nullptr; // đặt con trỏ cuối cùng của mảng thành nullptr để tránh trỏ đến vùng nhớ đã giải phóng.
            cout << "Xoa dau sach thanh cong!\n";
            return true; // trả về true để báo hiệu rằng đầu sách đã được xóa thành công.
        }
    }

    cout << "Khong tim thay dau sach co ISBN: " << ISBN << ". Xoa that bai.\n";
    return false; // trả về false nếu không tìm thấy đầu sách có ISBN trùng kh
}

void inDanhSachCuonSach(const DauSach* dauSach) {
    if (dauSach == nullptr) {
        cout << "Dau sach khong ton tai!\n";
        return;
    }

    Sach* temp = dauSach->dsSach.pHead; // tạo temp cho an toàn vì con trỏ pHead có thể bị thay đổi trong quá trình duyệt nếu chúng ta không cẩn thận, đặc biệt là khi thực hiện các thao tác như xóa cuốn sách. Bằng cách sử dụng con trỏ temp để duyệt danh sách sách của đầu sách, chúng ta đảm bảo rằng con trỏ pHead của đầu sách vẫn giữ nguyên giá trị ban đầu và không bị ảnh hưởng bởi quá trình duyệt, giúp tránh được các lỗi tiềm ẩn liên quan đến việc thay đổi con trỏ gốc trong quá trình duyệt danh sách.
    // tạo temp giúp chúng ta có thể duyệt qua danh sách sách của đầu sách một cách an toàn mà không lo bị mất địa chỉ của cuốn sách đầu tiên trong danh sách, điều này rất quan trọng để đảm bảo rằng chúng ta có thể truy cập đầy đủ thông tin của tất cả các cuốn sách con của đầu sách đó khi in danh sách cuốn sách.

    if (temp == nullptr) {
        cout << " DAU SACH NAY CHUA CO CUON SACH NAO!\n";
        return;
    }

    cout << "\n--- DANH SACH CAC CUON SACH (ISBN: " << dauSach->ISBN << ") ---\n";
    cout << "Ten sach: " << dauSach->tenSach << "\n";
    cout << "----------------------------------------------------------\n";

    int stt = 1; // biến stt để đánh số thứ tự cho các cuốn sách trong danh sách
    while (temp != nullptr) {
        cout << stt << ". Ma sach: " << temp->maSach
            << " | Vi tri: " << temp->viTri
            << " | Trang thai: ";
        switch (temp->trangThai) {
            case 0: cout << "Cho muon duoc"; break;
            case 1: cout << "Da muon"; break;
            case 2: cout << "Thanh ly"; break;
            default: cout << "Khong xac dinh"; break;
        }

        cout << endl;
        stt++;
        temp = temp->pNext;
    }

}

void traCuuTheoTen(const ListDauSach &ds, const char* tenSach) {
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    int left = 0;
    int right = ds.n - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);
        int cmp = strcmp(ds.nodes[mid]->tenSach, tenSach);
    
        if (cmp == 0) {
            const DauSach* found = ds.nodes[mid];
            cout << "DA TIM THAY DAU SACH!\n";
            cout << "ISBN: " << found->ISBN << "\n";
            cout << "Ten sach: " << found->tenSach << "\n"; 
            cout << "Tac gia: " << found->tacGia << "\n";
            cout << "The loai: " << found->theLoai << "\n";
            cout << "So trang: " << found->soTrang << "\n";
            cout << "Nam xuat ban: " << found->namXuatBan << "\n";
            cout << "So luot muon: " << found->soLuotMuon << "\n";
            inDanhSachCuonSach(found);
            return;
        } else {
            if (cmp < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    cout << "Khong tim thay dau sach co ten: " << tenSach << "\n";

}

Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay)
{
    for (int i = 0; i < ds.n; i++)
    {
        Sach* temp = ds.nodes[i]->dsSach.pHead;

        while (temp != nullptr)
        {
            if (strcmp(temp->maSach, maSach) == 0) 
            {
                dauSachTimThay = ds.nodes[i]; // gán đầu sách chứa cuốn sách tìm được vào con trỏ dauSachTimThay để trả về thông tin đầu sách đó cùng với cuốn sách.
                // Do return temp , can gan con tro dauSachTimThay truoc de luu tru thong tin dau sach tim thay, neu khong gan truoc khi return temp thi sau khi return temp, con tro temp se bi xoa va khong the truy cap vao dau sach tim thay de tra ve thong tin dau sach do cung voi cuon sach tim thay.
                return temp;
            }

            temp = temp->pNext;
        }
    }

    dauSachTimThay = nullptr;
    return nullptr;
}

bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayMuon) {
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    if (docGia->trangThaiThe == 0) {
        cout << "The doc gia da bi khoa. Khong the muon sach!\n";
        return false;
    }

    if (demSachDangMuon(docGia) >= MAX_SACH_MUON) {
        cout << "Doc gia da muon toi da so sach cho phep. Khong the muon them!\n";
        return false;
    }
    // Tìm cuốn sách theo mã sách và lấy đầu sách chứa cuốn sách đó.
    DauSach* dauSach = nullptr;
    Sach* sachMuon = timSachTheoMa(ds, maSach, dauSach);

    if (sachMuon == nullptr) {
        cout << "Khong tim thay cuon sach co ma: " << maSach << "\n";
        return false;
    }

    if (sachMuon->trangThai != 0) {
        cout << "Cuon sach khong co san de muon!\n";
        return false;
    }
    
    // Toa moi mot muonTra moi de luu thong tin muon sach.
    MuonTra* muonTra = new MuonTra; 

    //strcpy(muonTra->maSach, maSach); // Sao chép để khi sachMuon bị thay đổi trạng thái hoặc bị xóa, thông tin mã sách vẫn được lưu trữ chính xác trong muonTra của độc giả.
    // Việc sao chép mã sách vào muonTra giúp đảm bảo rằng thông tin về cuốn sách mà độc giả đã mượn được lưu trữ một cách chính xác và không bị ảnh hưởng bởi các thay đổi sau này đối với đối tượng Sach trong danh sách sách của đầu sách.
    // tránh việc trỏ vào vùng nhớ bị xóa hoặc bị thay đổi trạng thái của cuốn sách sau khi đã mượn, điều này giúp duy trì tính toàn vẹn của thông tin mượn sách trong hệ thống quản lý thư viện.

    strncpy(muonTra->maSach, maSach, sizeof(muonTra->maSach)-1);
    // tam thoi dung cai nay, sap toi lam nhapma rang buoc se giup
    muonTra->maSach[sizeof(muonTra->maSach)-1] = '\0'; // van de cua strncpy la no khong tu dong them ky tu ket thuc chuoi '\0' neu do dai chuoi nguon vuot qua kich thuoc cua chuoi dich, nen can phai gan ky tu '\0' o cuoi de dam bao tinh an toan khi su dung sau nay.

    muonTra->ngayMuon = ngayMuon;
    muonTra->trangThai = 0;
    muonTra->pNext = nullptr;

    // thêm vào cuối danh sách
    addTailMuonTra(docGia->dsMuonTra, muonTra);

    // cap nhat trang thai sach sau khi muon
    sachMuon->trangThai = 1;

    // tang luot muon
    dauSach->soLuotMuon++;

    cout << "Muon sach thanh cong!\n";
    return true;
}

bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra) {
    // Tim Doc Gia 
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;
    while (muonTra != nullptr) {
    if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) {

        DauSach* dauSach = nullptr;
        Sach* sachTra = timSachTheoMa(ds, maSach, dauSach);

        if (sachTra == nullptr) {
            cout << "Khong tim thay sach trong he thong!\n";
            return false;
        }

        if (sachTra->trangThai != 1)
            return false;
            
        // chỉ cập nhật khi chắc chắn tìm được sách
        muonTra->trangThai = 1;
        muonTra->ngayTra = ngayTra;
        sachTra->trangThai = 0;

        cout << "Tra sach thanh cong!\n";
        return true;
    }
        muonTra = muonTra->pNext;
    }
    cout << "Khong tim thay thong tin muon sach cho ma sach: " << maSach << " va ma the: " << maThe << "\n";
    return false;
}

void giaiPhongDanhSachDauSach(ListDauSach &ds) {
    for (int i = 0; i < ds.n; i++) {
        Sach* temp = ds.nodes[i]->dsSach.pHead;

        while (temp != nullptr) {
            Sach* next = temp->pNext;
            delete temp;
            temp = next;
        }

        delete ds.nodes[i];
        ds.nodes[i] = nullptr; // cho an toan
    }

    ds.n = 0;
}

//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//