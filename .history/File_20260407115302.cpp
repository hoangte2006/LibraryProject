#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

#include "Structs.h"
#include "File.h"
#include "Sach.h"
#include "Docgia.h"
#include "Giaodich.h"


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

    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line); // tao doi tuong stringstream ss de xu ly dong duoc luu trong bien line, stringstream cho phep tach dong chuoi thanh cac phan nho hon dua tren cac ky tu dac biet (nhu '|') de lay ra thong tin cua dau sach va thong tin cac cuon sach tuong ung tu file, sau do tao ra cac doi tuong DauSach va Sach tuong ung va them vao danh sach dau sach (ListDauSach) va danh sach cac cuon sach (ListSach) cua tung dau sach do trong he thong quan ly thu vien. Việc sử dụng stringstream giúp cho việc xử lý chuỗi và tách thông tin từ file trở nên dễ dàng và hiệu quả hơn, đồng thời giúp cho mã nguồn trở nên rõ ràng và dễ hiểu hơn khi làm việc với dữ liệu được lưu trữ trong file.

        string ISBN, tenSach, tacGia, theLoai;
        int soTrang, namXB, soLuotMuon, tongSoSach;

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

        DauSach* dsNew = new DauSach;

        strcpy(dsNew->ISBN, ISBN.c_str()); // .c_str() do ham strcpy yeu câu dia chi 
        strcpy(dsNew->tenSach, tenSach.c_str());
        strcpy(dsNew->tacGia, tacGia.c_str());
        strcpy(dsNew->theLoai, theLoai.c_str());

        dsNew->soTrang = soTrang;
        dsNew->namXuatBan = namXB;
        dsNew->soLuotMuon = soLuotMuon;

        dsNew->dsSach.pHead = dsNew->dsSach.pTail = nullptr; // tro vao nullptr de biet la chua co cuon sach nao duoc them vao dau sach nay.
        dsNew->dsSach.tongSoSach = 0;

        dsNew->sttXoa.top = -1;

        ds.nodes[ds.n++] = dsNew;   // tang so luong dau sach len sau khi them dau sach moi vao danh sach dau sach.

        getline(file, line); // #BOOK

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

            strcpy(sach->maSach, maSach.c_str());
            strcpy(sach->viTri, viTri.c_str());

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

bool loadDocGia(const char* filename, TREE_DocGia& root)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Khong mo duoc file DocGia\n";
        return false;
    }

    string line; // bien tam de doc tung dong trong file, sau do se duoc xu ly de tao ra cac doi tuong DocGia va MuonTra tuong ung.

    while (getline(file, line)) // doc tung dong trong file cho den khi het file, moi dong duoc luu vao bien line de xu ly.
    {
        if (line.empty()) continue;

        stringstream ss(line); // tao doi tuong stringstream ss de xu ly dong duoc luu trong bien line, stringstream cho phep tach dong chuoi thanh cac phan nho hon dua tren cac ky tu dac biet (nhu '|') de lay ra thong tin cua doc gia va thong tin muon tra tuong ung.

        int maThe;
        string ho, ten, gioitinh;
        int trangThai;

        ss >> maThe;
        ss.ignore(1);

        getline(ss, ho, '|');
        getline(ss, ten, '|');
        getline(ss, gioitinh, '|');

        ss >> trangThai;

        DocGia* dg = new DocGia;

        dg->maThe = maThe;

        strcpy(dg->ho, ho.c_str());
        strcpy(dg->ten, ten.c_str());
        strcpy(dg->giotinh, gioitinh.c_str());

        dg->trangThaiThe = trangThai;

        dg->pLeft = nullptr; // khoi tao con tro pLeft va pRight cua doc gia moi duoc tao thanh nullptr de biet rang chua co doc gia nao duoc them vao cay nhi phan tim kiem doc gia nay.
        dg->pRight = nullptr;

        dg->dsMuonTra.pHead = dg->dsMuonTra.pTail = nullptr;

        themDocGia(root, dg);

        getline(file, line); // #MUONTRA

        while (getline(file, line))
        {
            if (line == "END") break; // khi doc den dong "END" thi dung vong lap nay, tiep tuc doc dong tiep theo de tao ra cac doi tuong MuonTra tuong ung voi doc gia hien tai.

            stringstream ssMT(line);

            string maSach, ngayMuonStr, ngayTraStr;
            int trangThaiMT;

            getline(ssMT, maSach, '|');
            getline(ssMT, ngayMuonStr, '|');
            getline(ssMT, ngayTraStr, '|');

            ssMT >> trangThaiMT;

            MuonTra* mt = new MuonTra;

            strcpy(mt->maSach, maSach.c_str());

            // sscanf la ham trong thu vien <cstdio> de chuyen doi chuoi thanh cac gia tri so nguyen, ham nay duoc su dung de chuyen doi chuoi ngayMuonStr va ngayTraStr co dinh dang "dd/mm/yyyy" thanh cac gia tri so nguyen ngay, thang, nam de luu tru trong cac truong ngayMuon va ngayTra cua doi tuong MuonTra hien tai (mt) de sau nay co the su dung thong tin ve ngay muon va ngay tra cua muon tra do trong cac phep toan lien quan den muon tra sach, nhu kiem tra sach qua han hay khong, tinh so ngay muon, tinh tien phat neu co, v.v.
            sscanf(ngayMuonStr.c_str(), "%d/%d/%d",
                   &mt->ngayMuon.ngay,
                   &mt->ngayMuon.thang,
                   &mt->ngayMuon.nam);

            sscanf(ngayTraStr.c_str(), "%d/%d/%d",
                   &mt->ngayTra.ngay,
                   &mt->ngayTra.thang,
                   &mt->ngayTra.nam);

            mt->trangThai = trangThaiMT;

            mt->pNext = nullptr;

            addTailMuonTra(dg->dsMuonTra, mt);
        }
    }

    file.close();

    cout << "Load DocGia thanh cong\n";

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

        // Ngay muon
        file << (head->ngayMuon.ngay < 10 ? "0" : "") << head->ngayMuon.ngay << "/";
        file << (head->ngayMuon.thang < 10 ? "0" : "") << head->ngayMuon.thang << "/";
        file << head->ngayMuon.nam << "|";

        // Ngay tra
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

void ghiDanhSachSach(ofstream &file, ListSach dsSach)
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

bool saveDauSach(const char* filename, ListDauSach ds)
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