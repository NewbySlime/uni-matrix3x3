#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <conio.h>

#define JARAK_ANGKA 2
#define PANJANG_ANGKA 7

#define CHAR_ENTER 13
#define CHAR_BACKSPACE 8
#define MISCCHAR_UP 72
#define MISCCHAR_DOWN 80
#define MISCCHAR_LEFT 75
#define MISCCHAR_RIGHT 77

#define iomovedown_r(n) printf("\x1b[%dE", n)
#define iomoveup_r(n) printf("\x1b[%dF", n)
#define iomoveright(n) printf("\x1b[%dC", n)
#define iomoveleft(n) printf("\x1b[%dD", n)
#define iomoveup(n) printf("\x1b[%dA", n)
#define iomovedown(n) printf("\x1b[%dB", n)


template<typename val, int sizexy> class mat{
  protected:
    std::vector<val> konten;

    // bisa untuk sarrus
    // jika diluar batas, bakal disesuaikan
    int getIndex(int x, int y){
      x = x%sizexy;
      x = x < 0? x+sizexy: x;
      y = y%sizexy;
      y = y < 0? y+sizexy: y;
      return x + y*sizexy;
    }

    void _construct(){
      konten = std::vector<val>();
      konten.resize(sizexy*sizexy);
      for(int i = 0; i < konten.size(); i++)
        konten[i] = 0;
    }

    void _construct(std::vector<val> &angkaM){
      konten = std::vector<val>();
      konten.resize(sizexy*sizexy);
      int i = 0;
      for(; i < angkaM.size() && i < konten.size(); i++)
        konten[i] = angkaM[i];
      
      for(; i < konten.size(); i++)
        konten[i] = 0;
    }


  public:
    mat(){_construct();}
    mat(std::vector<val> &angkaM){_construct(angkaM);}

    val& num(int x, int y){
      return konten[getIndex(x,y)];
    }

    virtual val determinan(){
      return 0;
    }

    void input(){
      std::cout << "\npencet 'q' jika sudah selesai mengedit matriks\npencet Enter jika ingin mengedit salah satu angka\ngunakan arrow keys untuk navigasi tiap angka dalam matriks\n\n\n" << *this << std::flush;
      iomoveup(sizexy);

      int offsetawal = JARAK_ANGKA+1;
      iomoveright(offsetawal);
      
      int offsetx = 0;
      int offsety = 0;
      
      char c = 0;
      while((c = _getch()) != 'q'){
        switch(c){
          //lainnya
          break; case 0:{
            switch((c = _getch())){
              break; case MISCCHAR_UP:
                if(offsety > 0){
                  iomoveup(1);
                  offsety -= 1;
                }
              
              break; case MISCCHAR_DOWN:
                if(offsety < sizexy){
                  iomovedown(1);
                  offsety += 1;
                }
              
              break; case MISCCHAR_LEFT:
                if(offsetx > 0){
                  iomoveleft(JARAK_ANGKA+PANJANG_ANGKA);
                  offsetx -= 1;
                }

              break; case MISCCHAR_RIGHT:
                if(offsetx < sizexy){
                  iomoveright(JARAK_ANGKA+PANJANG_ANGKA);
                  offsetx += 1;
                }
            }
          }
          
          //enter
          break; case CHAR_ENTER:{
            const std::string prompt = "Pencet Enter lagi jika sudah selesai mengedit.";
            iomoveup_r(offsety+1);
            std::cout << prompt << std::flush;
            iomovedown_r(offsety+1);
            iomoveright(offsetawal+(offsetx*(JARAK_ANGKA+PANJANG_ANGKA)));

            bool adaKoma = false;
            std::string numstrclr;
            numstrclr.insert(numstrclr.cbegin(), PANJANG_ANGKA, ' ');
            std::cout << numstrclr << std::flush;
            iomoveleft(numstrclr.length());

            std::string numstr;
            numstr.reserve(PANJANG_ANGKA);
            while((c = _getch()) != CHAR_ENTER){
              if(c >= '0' && c <= '9' && numstr.length() < PANJANG_ANGKA){
                numstr += c;
                std::cout << c << std::flush;
              }
              else
                switch(c){
                  break; case '-':{
                    if(numstr.length() <= 0 && numstr.length() < PANJANG_ANGKA){
                      numstr += '-';
                      std::cout << c << std::flush;
                    }
                  }

                  break; case '.':{
                    if(!adaKoma && numstr.length() < PANJANG_ANGKA){
                      numstr += '.';
                      std::cout << c << std::flush;
                      adaKoma = true;
                    }
                  }

                  break; case CHAR_BACKSPACE:{
                    if(numstr.length() > 0){
                      char currch = numstr[numstr.length()-1];
                      numstr.erase(numstr.cend()-1, numstr.cend());

                      iomoveleft(1);
                      std::cout << ' ' << std::flush;
                      iomoveleft(1);
                      
                      if(currch == '.')
                        adaKoma = false;
                    }
                  }
                }
            }

            iomoveleft(numstr.length());
            std::istringstream iss(numstr);
            iss >> num(offsetx, offsety);

            iomoveup_r(offsety+1);
            std::string clrprompt;
            clrprompt.insert(clrprompt.cbegin(), prompt.length(), ' ');
            std::cout << clrprompt << std::flush;
            iomovedown_r(offsety+1);
            iomoveright(offsetawal+(offsetx*(JARAK_ANGKA+PANJANG_ANGKA)));
          }
        }
      }

      iomovedown_r(sizexy-offsety);
      std::cout << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, mat<val, sizexy>& m){
      std::string pemisah;
      pemisah.insert(pemisah.cbegin(), JARAK_ANGKA, ' ');
      for(int i = 0; i < sizexy; i++){
        os << "|" << pemisah;
        for(int o = 0; o < sizexy; o++){
          std::ostringstream strstr("", std::ios_base::ate);
          strstr << m.num(o,i);
          std::string numstr = strstr.str();
          if(numstr.length() > PANJANG_ANGKA)
            numstr.erase(numstr.cbegin()+PANJANG_ANGKA, numstr.cend());
          else if(numstr.length() < PANJANG_ANGKA)
            numstr.insert(numstr.length(), PANJANG_ANGKA-numstr.length(), ' ');

          os << numstr << pemisah;
        }
        
        os << "|\n";
      }

      return os;
    }

    void multiplyWith(mat<val, sizexy> m){
      mat<val, sizexy> thismat = *this;
      for(int i = 0; i < sizexy; i++)
        for(int o = 0; o < sizexy; o++){
          val res = 0;
          for(int n = 0; n < sizexy; n++)
            res += thismat.num(n, i) * m.num(o, n);
          
          num(o, i) = res;
        }
    }

    template<typename numtype> void multiplyWithk(numtype k){
      for(int i = 0; i < sizexy; i++)
        for(int o = 0; o < sizexy; o++)
          num(o,i) *= k;
    }

    void addWith(mat<val, sizexy> m){
      for(int i = 0; i < sizexy; i++)
        for(int o = 0; o < sizexy; o++)
          num(o,i) += m.num(o,i);
    }

    void subrtractWith(mat<val, sizexy> m){
      for(int i = 0; i < sizexy; i++)
        for(int o = 0; o < sizexy; o++)
          num(o,i) -= m.num(o,i);
    }
};

template<typename val> class mat2x2: public mat<val, 2>{
  public:
    mat2x2(){this->_construct();}
    mat2x2(std::vector<val> &array){this->_construct(array);}

    template<typename numtype> mat2x2<val> operator*(numtype k){
      mat2x2<val> hasil = *this;
      hasil.multiplyWithk(k);
      return hasil;
    }

    mat2x2<val> operator*(mat2x2<val> m){
      mat2x2<val> hasil = *this;
      hasil.multiplyWith(m);
      return hasil;
    }

    mat2x2<val> operator+(mat2x2<val> m){
      mat2x2<val> hasil = *this;
      hasil.addWith(m);
      return hasil;
    }

    mat2x2<val> operator-(mat2x2<val> m){
      mat2x2<val> hasil = *this;
      hasil.subrtractWith(m);
      return hasil;
    }

    virtual val determinan(){
      val hasil = 0;
      int i = 0;
      for(int u = 1; u > -2; u -= 2){
        val r = u;
        for(int o = 0; o < 2; o++)
          r *= this->num(i+(o*u),o);

        hasil += r;
        i++;
      }

      return hasil;
    }
};

template<typename val> class mat3x3: public mat<val, 3>{
  public:
    mat3x3(){this->_construct();}
    mat3x3(std::vector<val> &array){this->_construct(array);};
    template<typename valt> mat3x3(mat3x3<valt> m){
      this->_construct();
      for(int i = 0; i < 3; i++)
        for(int o = 0; o < 3; o++)
          this->num(o,i) = (val)m.num(o,i);
    }

    template<typename numtype> mat3x3<val> operator*(numtype k){
      mat3x3<val> hasil = *this;
      hasil.multiplyWithk(k);
      return hasil;
    }

    mat3x3<val> operator*(mat3x3<val> m){
      mat3x3<val> hasil = *this;
      hasil.multiplyWith(m);
      return hasil;
    }

    mat3x3<val> operator+(mat3x3<val> m){
      mat3x3<val> hasil = *this;
      hasil.addWith(m);
      return hasil;
    }

    mat3x3<val> operator-(mat3x3<val> m){
      mat3x3<val> hasil = *this;
      hasil.subrtractWith(m);
      return hasil;
    }

    val determinan(){
      val hasil = 0;

      for(int u = 1; u > -2; u -= 2)
        for(int i = 0; i < 3; i++){
          val r = u;
          for(int o = 0; o < 3; o++)
            r *= this->num(i+(o*u), o);

          hasil += r;
        }

      return hasil;
    }

    mat2x2<val> minor(int x, int y){
      mat2x2<val> res{};
      int ix = 0, iy = 0;
      for(int i = 0; i < 3; i++){
        if(i == y)
          continue;
        
        for(int o = 0; o < 3; o++){
          if(o == x)
            continue;
          
          res.num(ix, iy) = this->num(o, i);
          ix++;
        }
        
        iy++;
      }

      return res;
    }

    mat3x3<val> koefisien(){
      mat3x3<val> res{};
      for(int i = 0; i < 3; i++)
        for(int o = 0; o < 3; o++){
          mat2x2<val> m = minor(o,i);
          res.num(o,i) = (int)pow(-1, i+o) * minor(o,i).determinan();
        }
      
      return res;
    }

    mat3x3<val> transpose(){
      mat3x3<val> res{};
      for(int i = 0; i < 3; i++)
        for(int o = 0; o < 3; o++)
          res.num(o,i) = this->num(i,o);

      return res;
    } 

    mat3x3<float> invers(){
      mat3x3<val> adj = koefisien().transpose();
      val det = determinan();
      return (mat3x3<float>)adj * ((float)1/det);
    }
};

int main(){
  system("cls");
  std::cout << "Program untuk melakukan kalkulasi matriks 3x3.\nPilih salah satu kalkulasi yang ingin dipakai\n\n"
    << "1. Pertambahan\n2. Pengurangan\n3. Perkalian\n4. Determinan\n5. Invers\n" << std::flush;

  int pilihan = 0;
  std::cin >> pilihan;
  
  mat3x3<float> m1{}, m2{}, res{};
  float resf = 0;

  system("cls");
  std::cout << "Matriks 1\n";
  m1.input();

  switch(pilihan){
    break; case 1:{
      system("cls");
      std::cout << "Matriks 2\n";
      m2.input();
      
      res = m1 + m2;
    }

    break; case 2:{
      system("cls");
      std::cout << "Matriks 2\n";
      m2.input();

      res = m1 - m2;
    }

    break; case 3:{
      system("cls");
      std::cout << "Matriks 2\n";
      m2.input();

      res = m1 * m2;
    }

    break; case 4:{
      resf = m1.determinan();
    }

    break; case 5:{
      res = m1.invers();
    }
  }

  system("cls");
  if(pilihan == 4)
    std::cout << "Hasil determinan: " << resf << std::endl;
  else 
    std::cout << "\nHasil matriks:\n" << res << std::endl;
}