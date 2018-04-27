#include <fcntl.h> //_O_U16TEXT
#include <io.h>    //_setmode()
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

struct SINHVIEN {
	wchar_t MSSV[10];
	wchar_t HoTen[30];
	wchar_t Khoa[30];
	int Khoas;
	wchar_t Email[40];
	wchar_t NgaySinh[20];
	wchar_t HinhAnh[30];
	wchar_t MoTaBanThan[1000];
	wchar_t **Sothich;
	int nSothich;
};

void Docso(FILE*fin, int& n)
{
	wchar_t ch = fgetwc(fin);
	if (ch == '"')
	{
		fwscanf_s(fin, L"%d", &n);
		fseek(fin, 2L, SEEK_CUR);
	}
	else
	{
		fseek(fin, -1L, SEEK_CUR);
		fwscanf_s(fin, L"%d", &n);
		fseek(fin, 1L, SEEK_CUR);
	}
}

void Docthongtin(FILE*fin, wchar_t a[MAX_STR_LEN])
{
	wchar_t ch = fgetwc(fin);
	int i = 0;
	while (1)
	{
		if (ch == L',' || ch == WEOF)
			break;
		a[i] = ch;
		i++;
		ch = fgetwc(fin);
	}
	a[i] = '\0';
}

void DocMotabanthan(FILE*fin, wchar_t a[MAX_STR_LEN])
{
	wchar_t ch = fgetwc(fin);
	int i = 0;
	while (1)
	{
		if (ch == L',' || ch == WEOF || ch == '\n')
			break;
		a[i] = ch;
		i++;
		ch = fgetwc(fin);
	}
	a[i] = '\0';
}

int Demsothich(FILE*fin)
{
	int count1 = 0;
	while (1)
	{
		wchar_t ch = fgetwc(fin);
		if (ch == L',')
			count1++;
		else if (ch == L'\n' || ch == WEOF)
			break;
	}
	return count1;
}

void Docsothich(FILE*fin, wchar_t** &Sothich, int &nSothich)
{
	fseek(fin, -1L, SEEK_CUR);
	int Vitri = ftell(fin);
	nSothich = Demsothich(fin);
	if (nSothich == 0)
		Sothich = NULL;
	else
	{
		Sothich = (wchar_t**)malloc(nSothich*sizeof(wchar_t*));
		fseek(fin, Vitri, SEEK_SET);
		for (int i = 0; i < nSothich; i++)
		{
			Sothich[i] = (wchar_t*)malloc(1000 * sizeof(wchar_t));
			fwscanf(fin, L",%[^,\n]", Sothich[i]);
		}
	}
}

int Docsinhvien(FILE*fin, SINHVIEN &Viet)
{
	int nSothich;
	Docthongtin(fin, Viet.MSSV);
	Docthongtin(fin, Viet.HoTen);
	Docthongtin(fin, Viet.Khoa);
	Docso(fin, Viet.Khoas);
	Docthongtin(fin, Viet.Email);
	Docthongtin(fin, Viet.NgaySinh);
	Docthongtin(fin, Viet.HinhAnh);
	DocMotabanthan(fin, Viet.MoTaBanThan);
	Docsothich(fin, Viet.Sothich, nSothich);
	return nSothich;
}

int Demsinhvien(FILE*fin)
{
	wchar_t chr[1024];
	int count = 0;
	while (feof(fin) == NULL)
	{
		fgetws(chr, 1024, fin);
		count++;
	}
	return count - 1;
}

SINHVIEN* Docdanhsachsinhvien(FILE* fin, int &n)
{
	n = Demsinhvien(fin);
	fseek(fin, 0, 0);
	SINHVIEN* sv = (SINHVIEN*)malloc(n * sizeof(SINHVIEN));
	for (int i = 0; i < n; i++)
	{
		if (i != 0)
			fseek(fin, 2L, SEEK_CUR);
		sv[i].nSothich = Docsinhvien(fin, sv[i]);
	}
	fclose(fin);
	return  sv;
}

void WritetoHTML(FILE*outFile, SINHVIEN sv)
{
	fwprintf(outFile, L"<!DOCTYPE html PUBLIC \" -//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
	fwprintf(outFile, L"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	fwprintf(outFile, L"    <head>\n");
	fwprintf(outFile, L"        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	fwprintf(outFile, L"        <link rel=\"stylesheet\" type=\"text/css\" href=\"personal.css\" />\n");
	fwprintf(outFile, L"        <title>HCMUS - %s</title>\n", sv.HoTen);
	fwprintf(outFile, L"    </head>\n");
	fwprintf(outFile, L"    <body>\n");
	fwprintf(outFile, L"        <div class=\"Layout_container\">\n");
	fwprintf(outFile, L"            <!-- Begin Layout Banner Region -->\n");
	fwprintf(outFile, L"            <div class=\"Layout_Banner\" >\n");
	fwprintf(outFile, L"                <div><img id=\"logo\" src=\"Images/logo.jpg\" height=\"105\" /></div>\n");
	fwprintf(outFile, L"                <div class=\"Header_Title\">TRƯỜNG ĐẠI HỌC KHOA HỌC TỰ NHIÊN </div>\n");
	fwprintf(outFile, L"            </div>\n");
	fwprintf(outFile, L"            <!-- End Layout Banner Region -->\n");
	fwprintf(outFile, L"            <!-- Begin Layout MainContents Region -->\n");
	fwprintf(outFile, L"                <div class=\"Layout_MainContents\">\n");
	fwprintf(outFile, L"                <!-- Begin Below Banner Region -->\n");
	fwprintf(outFile, L"                <div class=\"Personal_Main_Information\">\n");
	fwprintf(outFile, L"                    <!-- Begin Thông tin cá nhân c?a th?y cô ----------------------------------------------------------------------------------------- -->\n");
	fwprintf(outFile, L"                    <div class = \"Personal_Location\">\n");
	fwprintf(outFile, L"                        <img src=\"Images/LogoFooter.jpg\" width=\"27\" height=\"33\" />\n");
	fwprintf(outFile, L"                        <span class=\"Personal_FullName\">%s - %s</span>\n", sv.HoTen, sv.MSSV);
	fwprintf(outFile, L"                        <div class=\"Personal_Department\">%s</div>\n", sv.Khoa);
	fwprintf(outFile, L"                        <br />\n");
	fwprintf(outFile, L"                        <div class=\"Personal_Phone\">\n");
	fwprintf(outFile, L"                             Email: %s\n", sv.Email);
	fwprintf(outFile, L"                        </div>\n");
	fwprintf(outFile, L"                        <br />\n");
	fwprintf(outFile, L"                        <br />\n");
	fwprintf(outFile, L"                    </div>\n");
	fwprintf(outFile, L"                    <!-- End Thông tin cá nhân c?a th?y cô ----------------------------------------------------------------------------------------- -->\n");
	fwprintf(outFile, L"                    <div class=\"Personal_HinhcanhanKhung\">\n");
	fwprintf(outFile, L"                        <img class=\"Personal_Hinhcanhan\" src=\"Images/%s.jpg\" />\n", sv.HinhAnh);
	fwprintf(outFile, L"                    </div>\n");
	fwprintf(outFile, L"                </div>\n");
	fwprintf(outFile, L"                <!-- End Below Banner Region -->\n");
	fwprintf(outFile, L"                <!-- Begin MainContents Region -->\n");
	fwprintf(outFile, L"                <div class=\"MainContain\">\n\n");
	fwprintf(outFile, L"                    <!-- Begin Top Region -->\n");
	fwprintf(outFile, L"                    <div class =\"MainContain_Top\">\n\n");
	fwprintf(outFile, L"                        <div class=\"InfoGroup\">Thông tin cá nhân</div>\n");
	fwprintf(outFile, L"                        <div>\n");
	fwprintf(outFile, L"                            <ul class=\"TextInList\">\n");
	fwprintf(outFile, L"                                <li>Họ và tên: %s</li>\n", sv.HoTen);
	fwprintf(outFile, L"                                <li>MSSV: %s</li>\n", sv.MSSV);
	fwprintf(outFile, L"                                <li>Sinh viên khoa:  %s</li>\n", sv.Khoa);
	fwprintf(outFile, L"                                <li>Ngày sinh: %s</li>\n", sv.NgaySinh);
	fwprintf(outFile, L"                                <li>Email: %s</li>\n", sv.Email);
	fwprintf(outFile, L"                            </ul>\n");
	fwprintf(outFile, L"                         </div>\n");
	fwprintf(outFile, L"                         <div class=\"InfoGroup\">Sở thích</div>\n");
	fwprintf(outFile, L"                         <div>\n");
	fwprintf(outFile, L"                            <ul class=\"TextInList\">\n");
	for (int j = 0; j < sv.nSothich; j++)
		fwprintf(outFile, L"                               <li>Sở thích %d: %s</li>\n", j + 1, sv.Sothich[j]);
	fwprintf(outFile, L"                            </ul>\n");
	fwprintf(outFile, L"                        </div>\n");
	fwprintf(outFile, L"                        <div class=\"InfoGroup\">Mô tả</div>\n");
	fwprintf(outFile, L"                        <div class=\"Description\">\n");
	fwprintf(outFile, L"                            Tôi là một người %s\n", sv.MoTaBanThan);
	fwprintf(outFile, L"                        </div>\n\n");
	fwprintf(outFile, L"                    </div>\n");
	fwprintf(outFile, L"                </div>\n");
	fwprintf(outFile, L"            </div>\n\n");
	fwprintf(outFile, L"            <!-- Begin Layout Footer -->\n");
	fwprintf(outFile, L"            <div class=\"Layout_Footer\">\n");
	fwprintf(outFile, L"                <div class=\"divCopyright\">\n");
	fwprintf(outFile, L"                    <br />\n");
	fwprintf(outFile, L"                    <img src=\"Images/LogoFooter_gray.jpg\" width=\"34\" height=\"41\" />\n");
	fwprintf(outFile, L"                    <br />\n");
	fwprintf(outFile, L"                    <br />\n");
	fwprintf(outFile, L"                    @2013</br>\n");
	fwprintf(outFile, L"                    Đồ án giữa kỳ</br>\n");
	fwprintf(outFile, L"                Kỹ thuật lập trình</br>\n");
	fwprintf(outFile, L"                TH2012/03</br>\n");
	fwprintf(outFile, L"                1712904 - Nguyễn Bách Việt</br>\n");
	fwprintf(outFile, L"                </div>\n");
	fwprintf(outFile, L"            </div>\n");
	fwprintf(outFile, L"            <!-- End Layout Footer -->\n");
	fwprintf(outFile, L"        </div>\n");
	fwprintf(outFile, L"    </body>\n");
	fwprintf(outFile, L"</html>\n");
	fclose(outFile);
}

wint_t StrLen(wchar_t *c)
{
	wint_t count = 0;
	while (c[count] != L'\0')
	{
		count++;
	}
	return count;
}

wchar_t *TenFile(wchar_t *str)
{
	wint_t a = StrLen(str);
	wchar_t *str1 = new wchar_t[a + 5];

	for (wint_t i = 0; i < a; i++)
	{
		str1[i] = str[i];
	}
	str1[a] = L'.';
	str1[a + 1] = L'h';
	str1[a + 2] = L't';
	str1[a + 3] = L'm';
	str1[a + 4] = L'l';
	str1[a + 5] = L'\0';
	return str1;
}

void XuatFile(SINHVIEN *sv, int n)
{
	for (int i = 0; i < n; i++)
	{
		wchar_t *fname = TenFile(sv[i].MSSV);

		FILE* fout = _wfopen(fname, L"w, ccs=UTF-8");
		if (fout == NULL)
		{
			wprintf(L"Không thể tạo file %s\n", fname);
		}
		WritetoHTML(fout, sv[i]);
	}
}

void Menu()
{
	wprintf(L"\n1.MSSV\n");
	wprintf(L"2.Họ Tên\n");
	wprintf(L"3.Khoa\n");
	wprintf(L"4.Khóa\n");
	wprintf(L"5.Email\n");
	wprintf(L"6.Ngày Sinh\n");
	wprintf(L"7.Mô tả bản thân\n");
	wprintf(L"8.Sở Thích\n");
	wprintf(L"9.Tât cả thông tin\n");
	wprintf(L"0.Thoát\n");
}

void Selection(int &n, FILE*fin)
{
	wprintf(L"Nhập lựa chọn chủa bạn: ");
	wscanf_s(L"%d", &n);

	switch (n){
	case 1:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.MSSV: %s\n", i + 1, sv[i].MSSV);
			   }
			   free(sv); break;
	}
	case 2:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.Họ Tên: %s\n", i + 1, sv[i].HoTen);
			   }
			   free(sv); break;
	}
	case 3:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.Khoa: %s\n", i + 1, sv[i].Khoa);
			   }
			   free(sv); break;
	}
	case 4:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.Khóa: %d\n", i + 1, sv[i].Khoas);
			   }
			   free(sv); break;
	}
	case 5:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.Email: %s\n", i + 1, sv[i].Email);
			   }
			   free(sv); break;
	}
	case 6:
	{
			  SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			  for (int i = 0; i < n; i++)
			  {
				  wprintf(L"%d.Ngày Sinh: %s\n", i + 1, sv[i].NgaySinh);
			  }
			  free(sv); break;
	}
	case 7:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   wprintf(L"%d.Mô tả bản thân: %s\n", i + 1, sv[i].MoTaBanThan);
			   }
			   free(sv); break;
	}
	case 8:{
			   SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			   for (int i = 0; i < n; i++)
			   {
				   for (int j = 0; j < sv[i].nSothich; j++)
					   wprintf(L"%d.Sở thích: %s\n", i + 1, sv[i].Sothich[j]);
			   }
			   free(sv); break;
	}
	case 9:
	{
			  SINHVIEN* sv = Docdanhsachsinhvien(fin, n);
			  for (int i = 0; i < n; i++)
			  {
				  wprintf(L"\n");
				  wprintf(L"%d.MSSV: %s\n", i + 1, sv[i].MSSV);
				  wprintf(L"Họ Tên: %s\n", sv[i].HoTen);
				  wprintf(L"Khoa: %s\n", sv[i].Khoa);
				  wprintf(L"Khóa: %d\n", sv[i].Khoas);
				  wprintf(L"Email: %s\n", sv[i].Email);
				  wprintf(L"Ngày Sinh: %s\n", sv[i].NgaySinh);
				  wprintf(L"Hình Ảnh: %s\n", sv[i].HinhAnh);
				  wprintf(L"Mô Tả Bản Thân: %s\n", sv[i].MoTaBanThan);
				  for (int j = 0; j < sv[i].nSothich; j++)
					  wprintf(L"Sở thích: %s\n", sv[i].Sothich[j]);
			  }
			  XuatFile(sv, n);
			  free(sv);
			  break;
	}
	case 0:
		break;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT); //needed for output
	_setmode(_fileno(stdin), _O_U16TEXT); //needed for input

	wprintf(L"\t\tWEBSITE SINH VIÊN\n\n ");
start:
	// đọc file tiếng Việt
	FILE* fin = _wfopen(L"Input.csv", L"r, ccs=UTF-8");
	if (!fin) {
		wprintf(L"Không thể đọc file Input.csv\n");
	}
	int n;
	Menu();
	Selection(n, fin);
	goto start;

	wprintf(L"Chương trình kết thúc.\n");
}

