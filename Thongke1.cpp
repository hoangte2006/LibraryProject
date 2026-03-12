// 1. Tổng số đầu sách (số dòng trong danh sách sách)
int tongSoDauSach(const vector<Sach>& dsSach) {
    return (int)dsSach.size();
}
 
// 2. Tổng số cuốn sách (tổng SoLuong)
int tongSoCuonSach(const vector<Sach>& dsSach) {
    int tong = 0;
    for (const auto& s : dsSach) tong += s.SoLuong;
    return tong;
}
 
// 3. Tổng số độc giả
int tongSoDocGia(const vector<DocGia>& dsDocGia) {
    return (int)dsDocGia.size();
}
 
// 4. Top 10 sách mượn nhiều nhất theo SoLuotMuon
void hienThiTop10Sach(const vector<Sach>& dsSach) {
    printTitle("TOP 10 SACH MUON NHIEU NHAT");
 
    vector<Sach> sorted = dsSach;
    sort(sorted.begin(), sorted.end(), [](const Sach& a, const Sach& b) {
        return a.SoLuotMuon > b.SoLuotMuon;
    });
 
    int limit = min(10, (int)sorted.size());
 
    cout << left
         << setw(4)  << "STT"
         << setw(12) << "Ma Sach"
         << setw(35) << "Ten Sach"
         << setw(20) << "Tac Gia"
         << setw(10) << "So Luot"
         << "\n";
    printLine('-');
 
    for (int i = 0; i < limit; i++) {
        cout << left
             << setw(4)  << (i + 1)
             << setw(12) << sorted[i].MaSach
             << setw(35) << sorted[i].TenSach
             << setw(20) << sorted[i].TacGia
             << setw(10) << sorted[i].SoLuotMuon
             << "\n";
    }
    printLine();
}
 
// 5. Danh sách độc giả đang mượn quá hạn
void hienThiDocGiaQuaHan(const vector<PhieuMuon>& dsPhieu,
                          const vector<DocGia>&   dsDocGia,
                          const vector<Sach>&     dsSach) {
    printTitle("DANH SACH DOC GIA MUON QUA HAN");