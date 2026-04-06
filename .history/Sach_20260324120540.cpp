#include "Sach.h"
#include <cctype>
#include <iostream>
#include <cstring>
#include <limits>
#include "Docgia.h"
#include "time.h"

using namespace std;

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
    p->sttXoa.top = -1; // khoi tao stack sttXoa rong, top = -1 de bieu thi stack rong.

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
    newNode->pNext = nullptr; // pNext luon la nullptr vi no se duoc them vao cuoi danh sach, nen khong con node nao nua sau no, nen pNext cua newNode se la nullptr de bieu thi ket thuc danh sach sau khi them newNode vao cuoi danh sach.

    // danh sach dang rong
    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newNode; // khi them node dau tien vao danh sach, ca pHead va pTail deu tro den node do, vi no la node duy nhat trong danh sach.
    } else {
        list.pTail->pNext = newNode; // cap nhat con tro pNext cua node cuoi cung hien tai (list.pTail) de tro den newNode, noi newNode vao cuoi danh sach.
        list.pTail = newNode;
    }
}

int laySoThuTuLonNhat(const ListSach& ds)
{
    int maxSTT = 0;

    const Sach* temp = ds.pHead;

    while (temp != nullptr)
    {
        const char* pos = strchr(temp->maSach, '_'); // tao bien pos luu dia chi , strchr tra ve con tro den ky tu '_'

        if (pos != nullptr) // check lai co ky tu '_'
        {
            int stt = atoi(pos + 1); // lay phan sau ky tu '_' va chuyen doi sang so nguyen de lay so thu tu hien tai cua cuon sach do.

            if (stt > maxSTT)
                maxSTT = stt;
        }

        temp = temp->pNext;
    }

    return maxSTT;
}

void themCuonSach(DauSach* dauSach) {
    if (dauSach == nullptr) return; // kiểm tra con tro dauSach có hợp lệ hay không.

    Sach* newSach = new Sach; // tạo một con trỏ newSach và cấp phát bộ nhớ cho một đối tượng Sach mới. Đây sẽ là cuốn sách mới mà chúng ta muốn thêm vào danh sách sách của đầu sách hiện tại. Sau khi cấp phát bộ nhớ, newSach sẽ trỏ đến vùng nhớ chứa thông tin của cuốn sách mới mà chúng ta sẽ nhập liệu từ người dùng.
    
    int stt = laySTTMoi(dauSach);
    // sprintf(newSach->maSach,"%s_%04d", dauSach->ISBN, stt); // ham sprintf de tao maSach cho cuon sach moi theo dinh dang "ISBN_STT", trong do ISBN la ma ISBN cua dau sach, va STT la so thu tu duoc tao ra tu ham laySTTMoi de dam bao tinh duy nhat cho maSach cua cuon sach moi trong danh sach sach cua dau sach do. Việc sử dụng sprintf giúp chúng ta dễ dàng tạo ra mã sách theo định dạng mong muốn một cách nhanh chóng và chính xác, đồng thời đảm bảo rằng mã sách được tạo ra có cấu trúc nhất quán và dễ dàng quản lý trong hệ thống quản lý thư viện.
    snprintf(newSach->maSach, sizeof(newSach->maSach), "%s_%04d", dauSach->ISBN, stt); // an toan hon khi su dung snprintf de tranh tran bo nho, ham snprintf se giup chung ta tao maSach cho cuon sach moi theo dinh dang "ISBN_STT", trong do ISBN la ma ISBN cua dau sach, va STT la so thu tu duoc tao ra tu ham laySTTMoi de dam bao tinh duy nhat cho maSach cua cuon sach moi trong danh sach sach cua dau sach do. Việc sử dụng snprintf giúp chúng ta dễ dàng tạo ra mã sách theo định dạng mong muốn một cách nhanh chóng và chính xác, đồng thời đảm bảo rằng mã sách được tạo ra có cấu trúc nhất quán và dễ dàng quản lý trong hệ thống quản lý thư viện, đồng thời tránh được các vấn đề liên quan đến tràn bộ nhớ có thể xảy ra khi sử dụng sprintf nếu kích thước của chuỗi kết quả vượt quá kích thước của mảng đích.
    // %s_%04d in 4 chu so neu thieu thi in ra 0, neu stt = 23 thi in ra 0023, neu stt = 1234 thi in ra 1234.

    cout << "Ma sach duoc tao: " << newSach->maSach << endl;
    
    cout << "Nhap vi tri: ";
    cin.getline(newSach->viTri, sizeof(newSach->viTri));

    newSach->trangThai = 0; // mặc định khi thêm mới, cuốn sách có trạng thái "cho mượn được".
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

            const char* pos = strchr(current->maSach, '_'); // don gian la tra ve con tro den ky tu '_' trong maSach, neu tim thay, neu khong tim thay thi tra ve nullptr, vi maSach duoc tao ra theo dinh dang "ISBN_STT", nen chung ta can tach phan STT tu maSach de dua vao ham pushSTTXoa de luu lai so thu tu cua cuon sach da bi xoa, de sau nay khi them cuon sach moi, chung ta co the su dung lai so thu tu da bi xoa nay de tao maSach cho cuon sach moi theo dinh dang "ISBN_STT", giup cho viec quan ly maSach cua cac cuon sach trong danh sach sach cua dau sach do duoc don gian va hieu qua hon.
            // sau dong nay da lay duoc STT 

            if (pos != nullptr)
            {
                int stt = atoi(pos + 1);
                pushSTTXoa(dauSach->sttXoa, stt);            
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


// tim ten sach thi dung binary search.
// tim theo ISBN thi duyet o(n)
Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay)
{ 
    dauSachTimThay = nullptr;

    // tach isbn tu maSach (AAAA_0001)
    char isbn [20];
    int i = 0;
    // chi lay phan chu cua maSach de tim dau sach, vi maSach duoc tao ra tu ma the doc gia + ISBN, nen chung ta can tach phan ISBN de tim dau sach chua cuon sach can tim.
    while (maSach[i] != '_' && maSach[i] != '\0' && i < sizeof(isbn) - 1)  // sizeof(isbn) - 1 la de dam bao rang khi tach phan ISBN tu maSach, chuoi isbn duoc tao ra se khong vuot qua kich thuoc cua mang isbn, va con lai 1 ky tu de gan ky tu null '\0' o cuoi chuoi sau khi tach xong, de dam bao tinh an toan khi su dung sau nay.
    {
        isbn[i] = maSach[i];
        i++;
    }
    isbn[i] = '\0'; // Dam bao chuoi isbn ket thuc bang ky tu null

    // tim dau sach theo ISBN
    for (int j = 0; j < ds.n; j++) {
        if (ds.nodes[j] == nullptr) continue; // check lai de tranh truong hop co dau sach bi xoa (nullptr) trong danh sach, neu co thi bo qua de tranh loi khi truy cap vao dau sach do.
        if (strcmp(ds.nodes[j]->ISBN, isbn) == 0) {
            dauSachTimThay = ds.nodes[j]; // bien dauSachTimThay se duoc gan gia tri cua dau sach tim thay, giup cho viec tra ve cuon sach sau nay de cap nhat trang thai khi muon sach.
            break;
        }
    }

    if (dauSachTimThay == nullptr) {
        return nullptr; 
    }

    // duyet danh sach sach cua dau sach tim thay de tim cuon sach co maSach trung khop.
    Sach* temp = dauSachTimThay->dsSach.pHead;
    while (temp != nullptr) {
        if (strcmp(temp->maSach, maSach) == 0) {
            return temp; // con tro ve cuon sach tim thay.
        }
        temp = temp->pNext; 
    
    }
    return nullptr;

}

void toLowerCase(char* str) { // ham toLowerCase de chuyen doi chuoi ve chu thuong, giup cho viec tra cuu theo ten sach duoc linh hoat hon, khong phan biet chu hoa chu thuong trong qua trinh tra cuu.
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]); // de bao dam tinh an toan khi su dung ham tolower, chung ta ep kieu ky tu sang unsigned char truoc khi truyen vao ham tolower, de tranh cac van de lien quan den gia tri am cua char khi no duoc truyen vao ham tolower, dieu nay giup cho ham tolower hoat dong chinh xac va an toan hon khi chuyen doi chuoi ve chu thuong.
    }
}

void traCuuTheoTenNangCao(const ListDauSach &ds, const char* key) {
    bool found = false;
    
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    char keyLower[100]; 
    strncpy(keyLower, key, sizeof(keyLower)-1); // sao chep key vao keyLower de giu nguyen key ban dau, tranh viec thay doi key khi toLowerCase se anh huong den cac thao tac khac sau nay neu co su dung lai key. -1 la chua cho ky tu null
    keyLower[sizeof(keyLower)-1] = '\0'; // dam bao tinh an toan khi su dung sau nay, tranh viec keyLower khong ket thuc bang ky tu null '\0'
    toLowerCase(keyLower); // chuyen keyLower ve chu thuong truc tiep de so sanh khi tim kiem, giup cho viec tra cuu theo ten sach duoc linh hoat hon, khong phan biet chu hoa chu thuong trong qua trinh tra cuu.

    cout << "Cac dau sach co ten tuong doi giong voi: " << key << "\n";
    for (int i = 0; i < ds.n; i++) { 

        if (ds.nodes[i] == nullptr) continue; // check lai de tranh truong hop co dau sach bi xoa (nullptr) trong danh sach, neu co thi bo qua de tranh loi khi truy cap vao dau sach do.

        char tenSachLower[100]; // mang luu ten sach dang thuong
        strncpy(tenSachLower, ds.nodes[i]->tenSach, sizeof(tenSachLower)-1);
        tenSachLower[sizeof(tenSachLower)-1] = '\0';
        toLowerCase(tenSachLower);

        // neu ten sach sau khi chuyen ve chu thuong co chua keyLower (cung da chuyen ve chu thuong), thi in ra thong tin dau sach do. Ham strstr se tra ve con tro den vi tri xuat hien dau tien cua keyLower trong tenSachLower neu tim thay, va tra ve nullptr neu khong tim thay, nen khi kiem tra dieu kien if (strstr(tenSachLower, keyLower) != nullptr) la de kiem tra xem tenSachLower co chua keyLower hay khong.
        // strstr la ham tim chuoi con.
        if (strstr(tenSachLower, keyLower) != nullptr) {
            cout << "- " << ds.nodes[i]->tenSach << " (ISBN: " << ds.nodes[i]->ISBN << ")\n";
            found = true;
        }
    }
    if (!found) {
        cout << "Khong tim thay dau sach co ten giong voi: " << key << "\n";
    }
}

// Ham layNgayHienTai de lay ngay thang nam hien tai tu he thong
//     struct tm* now = localtime(&t);

Ngay layNgayHienTai() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    Ngay ngayHT;
    ngayHT.ngay = now->tm_mday;
    ngayHT.thang = now->tm_mon + 1; 
    ngayHT.nam = now->tm_year + 1900;
    return ngayHT;
}
 }
int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai) {
    return 0;
}

bool coSachQuaHan(const DocGia* docGia, const Ngay& ngayHienTai) {
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        if (temp->trangThai == 0 && soSanhNgay(temp->ngayMuon, ngayHienTai) == false) {
            if (tinhSoNgay(temp->ngayMuon, ngayHienTai) > HAN_MUON) {
                return true; // Doc gia co sach qua han
            }
        }
        temp = temp->pNext;
    }
    return false;
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

    if (dangMuonSach(docGia, maSach)) {
        cout << "Doc gia da muon sach nay roi!\n";
        return false;
    }   

    if (demSachDangMuon(docGia) >= MAX_SACH_MUON) {
        cout << "Doc gia da muon qua 3 cuon! Khong the muon them!. Khoa the!\n";
        docGia->trangThaiThe = 0; // khoa the doc gia vi muon qua 3 cuon sach, vuot qua gioi han cho phep trong quy dinh cua thu vien, de bao ve tai san sach va khuyen khich doc gia tra sach dung han.
        return false;
    }

    if (coSachQuaHan(docGia, ngayMuon)) {
        cout << "Doc gia co sach qua han! Khong the muon sach!\n";
        return false;
    }

    // tim cuon sach theo maSach, va cung luc tim duoc dau sach chua cuon sach do de sau nay tang luot muon khi muon sach thanh cong.
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
    muonTra->trangThai = 0; // danh dau da muon la 0.
    muonTra->pNext = nullptr; //

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

    if (docGia->dsMuonTra.pHead == nullptr) {
        cout << "Doc gia chua muon sach nao!\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;

    while (muonTra != nullptr) {
    if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) { 

        if (!soSanhNgay(muonTra->ngayMuon, ngayTra)) {
            cout << "Ngay tra khong hop le!\n";
            return false;
        }

        DauSach* dauSach = nullptr;
        Sach* sachTra = timSachTheoMa(ds, maSach, dauSach);

        if (sachTra == nullptr) {
            cout << "Khong tim thay sach trong he thong!\n";
            return false;
        }

        if (sachTra->trangThai != 1) {
            cout << "Sach khong o trang thai dang muon!\n";
            return false;
        }
            
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

void pushSTTXoa(StackSTT &s, int stt)
{
    if (s.top < MAX_STT_STACK - 1) { // check lai dieu kien de tranh tran stack, neu stack da day thi khong the push them stt vao stack, dieu nay giup cho viec quan ly stack sttXoa duoc hieu qua hon, tranh cac van de lien quan den tran stack co the xay ra neu push them stt vao stack khi no da day.
        s.data[++s.top] = stt; // tang top len 1 va gan stt vao vi tri top hien tai trong mang data cua stack, dieu nay giup cho viec luu tru so thu tu cua cuon sach da bi xoa vao stack sttXoa mot cach don gian va hieu qua, de sau nay khi them cuon sach moi, chung ta co the su dung lai so thu tu da bi xoa nay de tao maSach cho cuon sach moi theo dinh dang "ISBN_STT", giup cho viec quan ly maSach cua cac cuon sach trong danh sach sach cua dau sach do duoc don gian va hieu qua hon.
    } else {
        cout << "Stack sttXoa da day! Khong the luu them STT da xoa.\n";
    }
}

int laySTTMoi(DauSach* dauSach)
{
    if (dauSach->sttXoa.top >= 0) // tai su dung lai so thu tu da bi xoa de tao maSach cho cuon sach moi.
        return dauSach->sttXoa.data[dauSach->sttXoa.top--];

    return laySoThuTuLonNhat(dauSach->dsSach) + 1;
}

bool baoMatSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach)
{
    DauSach* dauSach = nullptr;
    Sach* sach = timSachTheoMa(ds, maSach, dauSach);    
    
    if (sach == nullptr) {
        cout << "Khong tim thay sach\n";
        return false;
    }

    if (sach->trangThai != 1) {
        cout << "Sach khong o trang thai dang muon!\n";
        return false;
    }

    DocGia* docGia = timDocGia(root, maThe);

    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia\n";
        return false;
    }

    if (!dangMuonSach(docGia, maSach)) {
        cout << "Doc gia khong muon sach nay!\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;

    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 &&
            muonTra->trangThai == 0)
        { 
            if (sach == nullptr) {
                cout << "Khong tim thay sach\n";
                return false;
            }

            sach->trangThai = 2; // thanh ly
            muonTra->trangThai = 2; // mat sach
            // dauSach->dsSach.tongSoSach--; mat sach != xoa sach   

            cout << "Da ghi nhan sach bi mat!\n";
            return true;
        }

        muonTra = muonTra->pNext;
    }

    cout << "Khong tim thay phieu muon\n";
    return false;
}


//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//