#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

#include "Structs.h"
#include "File.h"
#include "Sach.h"
#include "Docgia.h"
#include "Giaodich.h"

// Ham copy chuoi an toan, dam bao khong bi tran bo nho khi copy tu string sang char array, va dam bao chuoi duoc copy vao char array luon co ky tu ket thuc null ('\0')
static void copyCStringSafe(char* dest, size_t destSize, const string& src) {
    strncpy(dest, src.c_str(), destSize - 1); 
    dest[destSize - 1] = '\0';
}


//////////////////////////////////////////////////////////
//// LOAD DAU SACH
//////////////////////////////////////////////////////////

// 1. Doc file DauSach.txt
// 2. Moi dau sach duoc luu trong mot doi tuong DauSach, sau do them vao danh sach dau sach (ListDauSach) de quan ly cac dau sach trong he thong.
// 3. Moi dau sach se co mot danh sach cac cuon sach (ListSach) de luu tru thong tin ve cac cuon sach con cua dau sach do, thong tin ve ma sach, vi tri va trang thai cua tung cuon sach duoc doc tu file va them vao danh sach cac cuon sach (ListSach) cua dau sach tuong ung.
// 4. Sau khi doc xong file, in ra thong tin cua cac dau sach

bool loadDauSach(const char* filename, ListDauSach& ds)
{
    ifstream file(filename);   

    if (!file.is_open()) 
    {
        cout << "Khong mo duoc file DauSach\n";
        return false;
    }

    string line;
    bool firstLine = true;

    while (getline(file, line))
    {
        if (line.empty()) continue;

        // Loai bo ky tu BOM (Byte Order Mark) cua UTF-8 o dong dau tien
        if (firstLine) {
            if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
                line = line.substr(3);
            }
            firstLine = false;
        }

        stringstream ss(line); // tao doi tuong stringstream ss de xu ly dong duoc luu trong bien line, stringstream cho phep tach dong chuoi thanh cac phan nho hon dua tren cac ky tu dac biet (nhu '|') de lay ra thong tin cua dau sach va thong tin cac cuon sach tuong ung tu file, sau do tao ra cac doi tuong DauSach va Sach tuong ung va them vao danh sach dau sach (ListDauSach) va danh sach cac cuon sach (ListSach) cua tung dau sach do trong he thong quan ly thu vien. Việc sử dụng stringstream giúp cho việc xử lý chuỗi và tách thông tin từ file trở nên dễ dàng và hiệu quả hơn, đồng thời giúp cho mã nguồn trở nên rõ ràng và dễ hiểu hơn khi làm việc với dữ liệu được lưu trữ trong file.

        string ISBN, tenSach, tacGia, theLoai;
        int soTrang, namXB, soLuotMuon;

        getline(ss, ISBN, '|'); // doc thong tin tu dong tu file, tach thong tin cua dau sach theo ky tu '|' de lay ra thong tin ve ISBN, ten sach, tac gia, the loai, so trang, nam xuat ban, so luot muon va tong so sach tuong ung voi dau sach hien tai trong file. Sau khi tach thong tin xong, ta se tao ra mot doi tuong DauSach moi va them vao danh sach dau sach (ListDauSach) de quan ly cac dau sach trong he thong.
        getline(ss, tenSach, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');

        ss >> soTrang;
        ss.ignore(1);
        ss >> namXB;
        ss.ignore(1);
        ss >> soLuotMuon;
        //ss.ignore(1);
        //ss >> tongSoSach;

        if (ds.n >= MAX_DAUSACH) {
            cout << "Vuot gioi han MAX_DAUSACH, bo qua ISBN: " << ISBN << "\n";

            getline(file, line); // #BOOK
            while (getline(file, line)) {
                if (line == "END") break;
            }
            continue;
        }

        DauSach* dsNew = new DauSach;

        copyCStringSafe(dsNew->ISBN, sizeof(dsNew->ISBN), ISBN);
        chuanHoaISBN(dsNew->ISBN);
        copyCStringSafe(dsNew->tenSach, sizeof(dsNew->tenSach), tenSach);
        chuanHoaChuoiSearch(dsNew->tenSach, dsNew->tenSachSearch); 
        copyCStringSafe(dsNew->tacGia, sizeof(dsNew->tacGia), tacGia);
        chuanHoaChuoiSearch(dsNew->tacGia, dsNew->tacGiaSearch);
        copyCStringSafe(dsNew->theLoai, sizeof(dsNew->theLoai), theLoai);

        dsNew->soTrang = soTrang;
        dsNew->namXuatBan = namXB;
        dsNew->soLuotMuon = soLuotMuon;

        dsNew->dsSach.pHead = dsNew->dsSach.pTail = nullptr; 
        dsNew->dsSach.tongSoSach = 0;

        int pos = timViTriChen(ds, dsNew->tenSach); 

        for (int i = ds.n; i > pos; i--) {
            ds.nodes[i] = ds.nodes[i - 1];
        }
        
        ds.nodes[pos] = dsNew;
        ds.n++;

        insertHashISBN(ds, dsNew); 

        getline(file, line); // Bo qua dong #BOOK

        while (getline(file, line))
        {
            if (line == "END") break;

            stringstream ssBook(line); 

            string maSach, viTri;
            int trangThai;

            getline(ssBook, maSach, '|');
            getline(ssBook, viTri, '|');
            ssBook >> trangThai;

            Sach* sach = new Sach;

            copyCStringSafe(sach->maSach, sizeof(sach->maSach), maSach);
            copyCStringSafe(sach->viTri, sizeof(sach->viTri), viTri);

            sach->trangThai = trangThai;
            sach->pNext = nullptr;

            addTailSach(dsNew->dsSach, sach);
        }
    }

    file.close();

    cout << "Load DauSach thanh cong\n";

    return true;
}

//////////////////////////////////////////////////////////
//// LOAD DOC GIA
//////////////////////////////////////////////////////////

bool loadDocGia(const char* filename, QuanLyDocGia& ql)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Khong mo duoc file DocGia\n";
        return false;
    }

    string line; 
    ql.soLuongDocGia = 0; // Reset de dem lai tu dau
    bool firstLine = true;

    while (getline(file, line))
    {
        if (line.empty()) continue;

        // Loai bo ky tu BOM (Byte Order Mark) cua UTF-8 o dong dau tien
        if (firstLine) {
            if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
                line = line.substr(3);
            }
            firstLine = false;
        }

        stringstream ss(line); 

        int maThe;
        string ho, ten, gioitinh;
        int trangThai;

        ss >> maThe;
        ss.ignore(1);

        getline(ss, ho, '|');
        getline(ss, ten, '|');
        getline(ss, gioitinh, '|');

        ss >> trangThai;

        DocGia* dg = taoDocGia(maThe); // Dung ham tao da toi uu
        copyCStringSafe(dg->ho, sizeof(dg->ho), ho);
        copyCStringSafe(dg->ten, sizeof(dg->ten), ten);
        copyCStringSafe(dg->giotinh, sizeof(dg->giotinh), gioitinh);
        dg->trangThaiThe = trangThai;
        dg->soSachDangMuon = 0; // Se dem lai phia duoi

        bool inserted = themDocGia(ql, dg);

        getline(file, line); // #MUONTRA

        if (!inserted) {
            // themDocGia da giai phong dg khi that bai, khong delete nua
            while (getline(file, line)) {
                if (line == "END") break;
            }
            continue;
        }

        while (getline(file, line))
        {
            if (line == "END") break;

            stringstream ssMT(line);
            string maSach, ngayMuonStr, ngayTraStr;
            int trangThaiMT;

            getline(ssMT, maSach, '|');
            getline(ssMT, ngayMuonStr, '|');
            getline(ssMT, ngayTraStr, '|');
            ssMT >> trangThaiMT;

            MuonTra* mt = new MuonTra;
            copyCStringSafe(mt->maSach, sizeof(mt->maSach), maSach);

            // sscanf de chuyen doi ngayMuonStr va ngayTraStr tu dinh dang "dd/MM/yyyy" sang cau truc Ngay.
            sscanf(ngayMuonStr.c_str(), "%d/%d/%d", &mt->ngayMuon.ngay, &mt->ngayMuon.thang, &mt->ngayMuon.nam);
            sscanf(ngayTraStr.c_str(), "%d/%d/%d", &mt->ngayTra.ngay, &mt->ngayTra.thang, &mt->ngayTra.nam);

            mt->trangThai = trangThaiMT;
            mt->pNext = nullptr;

            addTailMuonTra(dg->dsMuonTra, mt);

            // neu dang muon thi tang len 
            if (mt->trangThai == 0) {
                dg->soSachDangMuon++;
            }
        }
    }

    file.close();
    cout << "Load DocGia thanh cong. Tong so: " << ql.soLuongDocGia << "\n";
    return true;
}

//////////////////////////////////////////////////////////
//// CAP NHAT TRANG THAI SACH
//////////////////////////////////////////////////////////

// tai sao phai lam viec nay: sau khi load du lieu tu file, thong tin ve trang thai cua sach trong he thong co the khong duoc cap nhat chinh xac neu chi load du lieu ma khong cap nhat trang thai cua sach dang muon trong cay nhi phan tim kiem doc gia, vi khi load du lieu tu file, ta chi load thong tin ve doc gia va thong tin muon tra cua doc gia, ma chua cap nhat trang thai cua sach trong danh sach dau sach theo thong tin muon tra cua doc gia. Do do, sau khi load du lieu tu file, ta can phai de quy qua cay nhi phan tim kiem doc gia de cap nhat trang thai cua sach trong danh sach dau sach theo thong tin muon tra cua doc gia, de dam bao thong tin trang thai cua sach trong he thong luon duoc cap nhat chinh xac sau khi load du lieu tu file.   
void capNhatTrangThaiSachDangMuon(TREE_DocGia root, ListDauSach& ds)
{
    if (root == nullptr) return;

    capNhatTrangThaiSachDangMuon(root->pLeft, ds);

    MuonTra* mt = root->dsMuonTra.pHead;

    while (mt != nullptr)
    {
        if (mt->trangThai == 0) 
        {
            DauSach* dauSach = nullptr;

            Sach* sach = timSachTheoMa(ds, mt->maSach, dauSach); 

            if (sach != nullptr)
            {
                sach->trangThai = 1; 
            }
        }

        mt = mt->pNext;
    }

    capNhatTrangThaiSachDangMuon(root->pRight, ds); 
}

// - --------------------------------------------------------- Save 

void ghiDanhSachMuonTra(ofstream &file, MuonTra* head)
{
    while (head != nullptr)
    {
        file << head->maSach << "|";

        file << (head->ngayMuon.ngay < 10 ? "0" : "") << head->ngayMuon.ngay << "/";
        file << (head->ngayMuon.thang < 10 ? "0" : "") << head->ngayMuon.thang << "/";
        file << head->ngayMuon.nam << "|";

        file << (head->ngayTra.ngay < 10 ? "0" : "") << head->ngayTra.ngay << "/";
        file << (head->ngayTra.thang < 10 ? "0" : "") << head->ngayTra.thang << "/";
        file << head->ngayTra.nam << "|";

        file << head->trangThai << endl;

        head = head->pNext;
    }
}

void ghiDocGia(ofstream &file, TREE_DocGia root)
{
    if (root == nullptr) return;

    ghiDocGia(file, root->pLeft);

    // Ghi thong tin doc gia
    file << root->maThe << "|"
         << root->ho << "|"
         << root->ten << "|"
         << root->giotinh << "|"
         << root->trangThaiThe << endl;

    file << "#MUONTRA" << endl;

    ghiDanhSachMuonTra(file, root->dsMuonTra.pHead);

    file << "END" << endl;

    ghiDocGia(file, root->pRight);
}

bool saveDocGia(const char* filename, TREE_DocGia root)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Khong mo duoc file DocGia de ghi\n";
        return false;
    }

    ghiDocGia(file, root);

    file.close();

    cout << "Da luu DocGia\n";
    return true;
}

// -------- ------------------------------------------------- Save DauSach

void ghiDanhSachSach(ofstream &file, const ListSach& dsSach)
{
    Sach* p = dsSach.pHead;

    while (p != nullptr)
    {
        file << p->maSach << "|"
             << p->viTri << "|"
             << p->trangThai << endl;

        p = p->pNext;
    }
}

bool saveDauSach(const char* filename, const ListDauSach& ds)
{
    ofstream file(filename); 

    if (!file.is_open())
    {
        cout << "Khong mo duoc file DauSach de ghi\n";
        return false;
    }

    for (int i = 0; i < ds.n; i++)
    {
        DauSach* d = ds.nodes[i];

        file << d->ISBN << "|"
             << d->tenSach << "|"
             << d->tacGia << "|"
             << d->theLoai << "|"
             << d->soTrang << "|"
             << d->namXuatBan << "|"
             << d->soLuotMuon << "|"
             << d->dsSach.tongSoSach << endl;

        file << "#BOOK" << endl; 

        ghiDanhSachSach(file, d->dsSach);

        file << "END" << endl;
    }

    file.close();

    cout << "Da luu DauSach\n";
    return true;
}