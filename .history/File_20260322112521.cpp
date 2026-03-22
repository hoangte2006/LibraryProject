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

        stringstream ss(line);

        string ISBN, tenSach, tacGia, theLoai;
        int soTrang, namXB, soLuotMuon, tongSoSach;

        getline(ss, ISBN, '|');
        getline(ss, tenSach, '|');
        getline(ss, tacGia, '|');
        getline(ss, theLoai, '|');

        ss >> soTrang;
        ss.ignore(1);
        ss >> namXB;
        ss.ignore(1);
        ss >> soLuotMuon;
        ss.ignore(1);
        ss >> tongSoSach;

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

void capNhatTrangThaiSachDangMuon(TREE_DocGia root, ListDauSach& ds)
{
    if (root == nullptr) return;

    capNhatTrangThaiSachDangMuon(root->pLeft, ds);

    MuonTra* mt = root->dsMuonTra.pHead;

    while (mt != nullptr)
    {
        if (mt->trangThai == 0) // chi cap nhat trang thai cua sach dang duoc muon (trangThai == 0) sang da muon (trangThai == 1) trong danh sach dau sach, de thong tin ve trang thai cua sach trong he thong luon duoc cap nhat chinh xac.
        {
            DauSach* dauSach = nullptr; // khai bao con tro dauSach de luu dia chi cua dau sach tim thay trong danh sach dau sach khi tim theo ma sach tu thong tin muon tra (mt) hien tai.

            Sach* s = timSachTheoMa(ds, mt->maSach, dauSach); // ham timSachTheoMa se tra ve con tro s tro den cuon sach co ma sach trung khop voi ma sach trong thong tin muon tra hien tai (mt->maSach), va cung cap dia chi cua dau sach chua cuon sach do qua con tro dauSach. Neu tim thay cuon sach co ma sach trung khop, s se khac nullptr va ta se cap nhat trang thai cua cuon sach do sang da muon (trangThai = 1) de dong bo thong tin trang thai cua sach trong he thong sau khi da load du lieu tu file.

            if (s != nullptr) // kiem tra neu tim thay cuon sach co ma sach trung khop trong danh sach dau sach, neu s khac nullptr thi cap nhat trang thai cua cuon sach do sang da muon (trangThai = 1) de dong bo thong tin trang thai cua sach trong he thong sau khi da load du lieu tu file.
            {
                s->trangThai = 1; // cap nhat trang thai cua cuon sach do sang da muon (trangThai = 1) de dong bo thong tin trang thai cua sach trong he thong sau khi da load du lieu tu file, chi cap nhat khi tim thay cuon sach co ma sach trung khop voi ma sach trong thong tin muon tra hien tai (mt->maSach) de dam bao thong tin trang thai cua sach trong he thong luon duoc cap nhat chinh xac sau khi load du lieu tu file.
            }
        }

        mt = mt->pNext;
    }

    capNhatTrangThaiSachDangMuon(root->pRight, ds); // tiep tuc de quy sang cay con ben phai de cap nhat trang thai cua sach dang muon trong cay con ben phai, de dam bao thong tin trang thai cua sach trong he thong luon duoc cap nhat chinh xac sau khi load du lieu tu file.
}