/*
all map format (Y,X) function format (x,y)
copyright(c) 2017 MAGNET inc.
YOU O:2   COM @:2
   X 0 1 2 3 4 5 6 7
 Y ------------------
 0|                  |
 1|                  |
 2|                  |
 3|        O @       |
 4|        @ O       |
 5|                  |
 6|                  |
 7|                  |
   ------------------
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>//setprecision
#include <math.h>//pow()
#include <stdlib.h>     /* srand, rand */
#include "matrix.h"
#include "ANN.h"

using namespace std;
char version [128] = "NodeNetC version.";
int com();
class reversi {
  private:
    int map[8][8];
    int benefit_map[8][8];
    int benefit(int, int, int, int, int);
    bool check(int, int, int, int, int, int); //recursion
  public:
    int reverse();
    reversi();
    int clr(); int print();
    int value(int, int);
    int set(int, int, int);
    int mypoint()    ; int compoint();
    int getbenefit(int, int, int);
    int generatekey(char *&key);
    int setbykey(char *key);
    int printkey();
    int can_set(int);
};
reversi::reversi() {}
int reversi::reverse() {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      this->map[i][j] *= -1;
    }
  }
  return 0;
}
int reversi::printkey () {
  char *A=new char[65];
  (*this).generatekey(A);
  cout<<"0123456701234567012345670123456701234567012345670123456701234567"<<endl;
  cout<<A<<endl;
  return 0;
}
int reversi::can_set(int sti) {
  int counter = 0;
  int i=0, j=0;
  for (i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      if((*this).getbenefit(i, j, sti) != -1 ) {
        counter++;
      }
    }
  }
  return counter;
}
int reversi::generatekey (char *&key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch ((*this).map[j][i]) {
        case 1:
          key[i*8+j]='A';
        break;
        case 0:
          key[i*8+j]='N';
        break;
        case -1:
          key[i*8+j]='B';
        break;
      }
    }
  }
  key[64]='\0';
  return 0;
}
int reversi::setbykey (char *key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch (key[i*8+j]) {
        case 'A':
          (*this).map[j][i]=1;
        break;
        case 'N':
          (*this).map[j][i]=0;
        break;
        case 'B':
          (*this).map[j][i]=-1;
        break;
      }
    }
  }
  return 0;
}
int reversi::mypoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == 1)temp++; }}
    return temp;
}
int reversi::compoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == -1)temp++; }}
    return temp;
}
int reversi::clr() {
    int i, j;
    for (i=0; i<8; i++) { for (j=0; j<8; j++)  { map[i][j]=0; } }
    map[3][3]=1 ;  map[3][4]=-1 ;  map[4][3]=-1 ;  map[4][4]=1;//default
    return 0;
}
int reversi::value(int x, int y) {return map[y][x]; }
int reversi::print() {
    int i, j;
    printf("copyright(c) 2017 NOOXY inc.\nHelp: first input X , press \"Enter\",  and then input Y.\nYOU O:%d   COM @:%d \n", mypoint(), compoint());
    printf("\n");
    printf("   X 0 1 2 3 4 5 6 7 \n");
    printf(" Y ------------------ \n");
    for (i=0; i<8; i++) {
        printf("%2d| ", i);
        for (j=0; j<8; j++) {
            switch(map[i][j]) {
                case 1:
                    printf(" O");
                    break;
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf(" @");
                    break;
            }
        }
        printf(" |\n");
    }
    printf("   ------------------\n");
    return 0;
}
int reversi::set(int X,  int Y,  int sti) {
    bool temp=0;
    if (map[Y][X] == 0) {
        if (map[Y-1][X-1] == sti*-1 && (*this).check(X-1,  Y-1, -1, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X] == sti*-1 && (*this).check(X, Y-1, 0, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X+1] == sti*-1 && (*this).check(X+1, Y-1, 1, -1, sti, 1)) {temp=1; }
        if (map[Y][X-1] == sti*-1 && (*this).check(X-1, Y, -1, 0, sti, 1)) {temp=1; }
        if (map[Y][X+1] == sti*-1 && (*this).check(X+1, Y, 1, 0, sti, 1)) {temp=1; }
        if (map[Y+1][X-1] == sti*-1 && (*this).check(X-1, Y+1, -1, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X] == sti*-1 && (*this).check(X, Y+1, 0, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X+1] == sti*-1 && (*this).check(X+1, Y+1, 1, 1, sti, 1)) {temp=1; }
    }
    if(temp == 1) {map[Y][X]=sti; }else {return -1;}
    if(temp == 1)return 0;
    return 1;
}
bool reversi::check(int x, int y, int nx, int ny, int sti, int set) {//(start X, Y) (Next X, Y) (mychess) (set or not)
    if(x<0||x>7||y<0||y>7) {return 0; }
    if((*this).value(x, y) == 0)  {return 0; }
    if((*this).value(x, y) == sti)  {return 1; }
    if((*this).value(x, y) == -sti) {
        bool a=(*this).check(x+nx, y+ny, nx, ny, sti, set);
        if(set && a) {map[y][x]=sti; }
        return a;
    }
    return 0;
}
int reversi::getbenefit (int x, int y, int sti) {
  int i, j;
  for (i=0; i<8; i++) {for(j=0; j<8; j++) {(*this).benefit_map[i][j]=-1; }}
  for (i=0; i<8; i++) {
      for (j=0; j<8; j++) {
          if((*this).value(j, i) == sti) {
              (*this).benefit(j, i, -1, -1, sti); (*this).benefit(j, i,  0, -1, sti);  (*this).benefit(j, i,  1, -1, sti);
              (*this).benefit(j, i, -1,  0, sti);                              (*this).benefit(j, i,  1,  0, sti);
              (*this).benefit(j, i, -1,  1, sti); (*this).benefit(j, i,  0,  1, sti);  (*this).benefit(j, i,  1,  1, sti);
          }
      }
  }
  return (*this).benefit_map[y][x];
}
int reversi::benefit(int x, int y, int nx, int ny,int sti) {
    int value=0;
    bool temp=1;
    x+=nx; y+=ny;
    while(temp) {
        if(x<8 && x>=0 && y<8 && y>=0) {
            if((*this).value(x, y) == sti) {(*this).benefit_map[y][x]=-1; temp=0; }
            if((*this).value(x, y) == 0) {
              if((*this).benefit_map[y][x]==-1 && value > 0) {
                (*this).benefit_map[y][x]= 0;
              }
              (*this).benefit_map[y][x]+=value; temp=0;
            }
            if((*this).value(x, y) == -sti) {(*this).benefit_map[y][x]=-1; value++; x+=nx; y+=ny; }
        }else {temp=0; }
    }
    return 0;
}
reversi GlobleReversiMap;
//-------------------------------com----------------------------------
int com() {
  int i=0, j=0, temp[3]= {-1, 0, 0};
  double temp_value=-1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(GlobleReversiMap.getbenefit(i, j, -1) != -1 ) {
        double value=0;
        value=double(GlobleReversiMap.getbenefit(i, j, -1));
        //corner
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          if (value<20) {
            value=25;
          }
        }
        //border
        else if (i==0) {
            if (GlobleReversiMap.value(0, 0) == 1 || GlobleReversiMap.value(0, 7) == 1) {
              value=0;
            }
        }
        else if (i==7) {
            if (GlobleReversiMap.value(7, 0) == 1 || GlobleReversiMap.value(7, 7) == 1) {
              value=0;
            }
        }
        else if (j==0) {
            if (GlobleReversiMap.value(0, 0) == 1 || GlobleReversiMap.value(7, 0) == 1) {
              value=0;
            }
        }
        else if (j==7) {
            if (GlobleReversiMap.value(0, 7) == 1 || GlobleReversiMap.value(7, 7) == 1) {
              value=0;
            }
        }
        //subcorder
        if (  (i==1 && j==0)||(i==0 && j==1)||(i==1 && j==1)) {
          if (GlobleReversiMap.value(0, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==6 && j==0)||(i==6 && j==1)||(i==7 && j==1)) {
          if (GlobleReversiMap.value(7, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==0 && j==6)||(i==1 && j==6)||(i==1 && j==7)) {
          if (GlobleReversiMap.value(0, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==7 && j==6)||(i==6 && j==6)||(i==6 && j==7)) {
          if (GlobleReversiMap.value(7, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        //inner corner
        if ((i==2 && j==2)||(i==5 && j==2)||(i==2 && j==5)||(i==5 && j==5)) {
          value = value*2.5;
        }
        //inner border
        if (
          (i==2 && j==3)||(i==2 && j==4)||
          (i==3 && j==5)||(i==4 && j==5)||
          (i==5 && j==3)||(i==5 && j==4)||
          (i==3 && j==2)||(i==4 && j==2)
        ) {
          value = value + 2;
        }
        if (value >= temp_value) {
          temp_value = value; temp[1]=i; temp[2]=j;
        }
      }
    }
  }
  if(temp_value>=0) {
    GlobleReversiMap.set(temp[1], temp[2], -1);
  }
  else {
  }
  return 0;
}
int com_json() {
  int i=0, j=0, temp[3]= {-1, 0, 0};
  double temp_value=-1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(GlobleReversiMap.getbenefit(i, j, -1) != -1 ) {
        double value=0;
        value=double(GlobleReversiMap.getbenefit(i, j, -1));
        //corner
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          if (value<20) {
            value=25;
          }
        }
        //border
        else if (i==0) {
            if (GlobleReversiMap.value(0, 0) == 1 || GlobleReversiMap.value(0, 7) == 1) {
              value=0;
            }
        }
        else if (i==7) {
            if (GlobleReversiMap.value(7, 0) == 1 || GlobleReversiMap.value(7, 7) == 1) {
              value=0;
            }
        }
        else if (j==0) {
            if (GlobleReversiMap.value(0, 0) == 1 || GlobleReversiMap.value(7, 0) == 1) {
              value=0;
            }
        }
        else if (j==7) {
            if (GlobleReversiMap.value(0, 7) == 1 || GlobleReversiMap.value(7, 7) == 1) {
              value=0;
            }
        }
        //subcorder
        if (  (i==1 && j==0)||(i==0 && j==1)||(i==1 && j==1)) {
          if (GlobleReversiMap.value(0, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==6 && j==0)||(i==6 && j==1)||(i==7 && j==1)) {
          if (GlobleReversiMap.value(7, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==0 && j==6)||(i==1 && j==6)||(i==1 && j==7)) {
          if (GlobleReversiMap.value(0, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==7 && j==6)||(i==6 && j==6)||(i==6 && j==7)) {
          if (GlobleReversiMap.value(7, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        //inner corner
        if ((i==2 && j==2)||(i==5 && j==2)||(i==2 && j==5)||(i==5 && j==5)) {
          value = value*2.5;
        }
        //inner border
        if (
          (i==2 && j==3)||(i==2 && j==4)||
          (i==3 && j==5)||(i==4 && j==5)||
          (i==5 && j==3)||(i==5 && j==4)||
          (i==3 && j==2)||(i==4 && j==2)
        ) {
          value = value + 2;
        }

        if (value >= temp_value) {
          temp_value = value; temp[1]=i; temp[2]=j;
        }
      }
    }
  }
  if(temp_value>=0) {
    int p1,p2;
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    GlobleReversiMap.set(temp[1], temp[2], -1);
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<temp[1]<<"\",\n"
    <<"    \"y\": \""<<temp[2]<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }else {
    int p1,p2;
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<-1<<"\",\n"
    <<"    \"y\": \""<<-1<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }
  return 0;
}
//---------------------------reversi_ANN------------------------------
class reversi_ANN {
  public:
    matrix render_outmatrix(double *map_raw, int row, int *column_loc, double *render_value, double distance);
    int json();
    int json2();
    int train(string filename, double distance, int use_old_base);
    int train_ramdom();
    int Battle_first();
    int Battle_second();
};
matrix reversi_ANN::render_outmatrix(double *map_raw, int row, int *column_loc, double *render_value, double distance) {
  int i, j;
  double sum;
  double *map = new double[9999];
  matrix answer;
  sum = (1.0 / (1.0 - distance));
  for(i = 0; i < row * 64; i++) {
    map[i] = map_raw[i];
  }
  for(i = 0; i < row; i++) {
    map[i * 64 + column_loc[i]] = 0;
  }
  for(i = 0; i < row; i++) {
    map[i * 64 + column_loc[i]] = render_value[0] * pow(distance, (row - (i + 1))) + map_raw[i * 64 + column_loc[i]] * ( 1 - pow(distance, (row - (i + 1))));
  }
  // for(i = 0; i < row; i++) {
  //   for(j = i; j >= 0; j--) {
  //     map[j * 64 + column_loc[j]] += (pow(distance, i - j) / sum) * render_value[i];
  //     // cout << " " << j * 64 + column_loc[j] << " " << (pow(distance, i - j) / sum) * render_value[i] << endl;
  //   }
  // }
  // for(i = 0; i < row; i++) {
  //   map[i * 64 + column_loc[i]] = map_raw[i * 64 + column_loc[i]] * 0.9 + map[i * 64 + column_loc[i]] * 0.1;
  // }
  // if(distance <= 0.1) {
  //   for(i = 0; i < row; i++) {
  //     for(j = i; j >= 0; j--) {
  //       if (map[j * 64 + column_loc[j]] < 0.5){
  //         map[j * 64 + column_loc[j]] = 0.5;
  //       }
  //     }
  //   }
  // }
  answer.setmatrix(row, 64, map);
  return answer;
}
int reversi_ANN::Battle_first() {
  GlobleReversiMap.reverse();
  ANN myann;
  myann.load_from_file("reversi_base");
  int count = 0, i, j, x , y, location[65];
  matrix IN, OUT;
  matrix print;
  double in_temp[64], out_temp[64];
  matrix IN_temp;
  matrix OUT_temp;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      in_temp[i * 8 + j] = GlobleReversiMap.value(i, j);
    }
  }
  IN_temp.setmatrix(1, 64, in_temp);
  OUT_temp = (myann.feed(IN_temp)).transfer(logit);
  //clip output
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      if(GlobleReversiMap.getbenefit(j, i, 1) == -1) {
        out_temp[i * 8 + j] = -1;
      }
      else {
        if(OUT_temp.get_element(1, i * 8 + j + 1) <= 1 && OUT_temp.get_element(1, i * 8 + j + 1) >= 0) {
          out_temp[i * 8 + j] = OUT_temp.get_element(1, i * 8 + j + 1);
        }
        else if(OUT_temp.get_element(1, i * 8 + j + 1) > 1) {
          out_temp[i * 8 + j] = 1;
        }
        else {
          out_temp[i * 8 + j] = 0;
        }
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          out_temp[i * 8 + j] = 1;
        }
      }
    }
  }
  // search small
  double temp_value = -1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(out_temp[i * 8 + j] >= 0 ) {
        double value = 0;
        value = out_temp[i * 8 + j];
        if (value > temp_value) {
          temp_value = value;
          x = j;
          y = i;
        }
      }
    }
  }
  if(temp_value >= 0) {
    GlobleReversiMap.set(x, y, 1);
    GlobleReversiMap.reverse();
  }
  else {
    GlobleReversiMap.reverse();
  }
  return 0;
}
int reversi_ANN::Battle_second() {
  ANN myann;
  myann.load_from_file("reversi_base_2");
  int count = 0, i, j, x , y, location[65];
  matrix IN, OUT;
  matrix print;
  double in_temp[64], out_temp[64];
  matrix IN_temp;
  matrix OUT_temp;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      in_temp[i * 8 + j] = GlobleReversiMap.value(i, j);
    }
  }
  IN_temp.setmatrix(1, 64, in_temp);
  OUT_temp = (myann.feed(IN_temp)).transfer(logit);
  //clip output
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      if(GlobleReversiMap.getbenefit(j, i, 1) == -1) {
        out_temp[i * 8 + j] = -1;
      }
      else {
        if(OUT_temp.get_element(1, i * 8 + j + 1) <= 1 && OUT_temp.get_element(1, i * 8 + j + 1) >= 0) {
          out_temp[i * 8 + j] = OUT_temp.get_element(1, i * 8 + j + 1);
        }
        else if(OUT_temp.get_element(1, i * 8 + j + 1) > 1) {
          out_temp[i * 8 + j] = 1;
        }
        else {
          out_temp[i * 8 + j] = 0;
        }
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          out_temp[i * 8 + j] = 1;
        }
      }
    }
  }
  // search small
  double temp_value = -1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(out_temp[i * 8 + j] >= 0 ) {
        double value = 0;
        value = out_temp[i * 8 + j];
        if (value > temp_value) {
          temp_value = value;
          x = j;
          y = i;
        }
      }
    }
  }
  if(temp_value >= 0) {
    GlobleReversiMap.set(x, y, 1);
  }
  else {

  }
  return 0;
}
int reversi_ANN::json() {
  GlobleReversiMap.reverse();
  ANN myann;
  myann.load_from_file("reversi");
  int count = 0, i, j, x , y, location[65];
  matrix IN, OUT;
  matrix print;
  double in_temp[64], out_temp[64];
  matrix IN_temp;
  matrix OUT_temp;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      in_temp[i * 8 + j] = GlobleReversiMap.value(i, j);
    }
  }
  IN_temp.setmatrix(1, 64, in_temp);
  OUT_temp = (myann.feed(IN_temp)).transfer(logit);
  //clip output
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      if(GlobleReversiMap.getbenefit(i, j, 1) == -1) {
        out_temp[i * 8 + j] = -999;
      }
      else {
        if(OUT_temp.get_element(1, i * 8 + j + 1) <= 5 && OUT_temp.get_element(1, i * 8 + j + 1) >= -5) {
          out_temp[i * 8 + j] = OUT_temp.get_element(1, i * 8 + j + 1);
        }
        else if(OUT_temp.get_element(1, i * 8 + j + 1) > 5) {
          out_temp[i * 8 + j] = 5;
        }
        else {
          out_temp[i * 8 + j] = -5;
        }
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          out_temp[i * 8 + j] = 6;
        }
        if ((i==1 && j==1)||(i==6 && j==1)||(i==1 && j==6)||(i==6 && j==6)) {
          out_temp[i * 8 + j] -= 1;
        }
      }
    }
  }
  // search big
  double temp_value = -999;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(out_temp[i * 8 + j] > -999 ) {
        double value = 0;
        value = out_temp[i * 8 + j];
        if (value > temp_value) {
          temp_value = value;
          x = i;
          y = j;
        }
      }
    }
  }
  if(temp_value > -999) {
    int p1,p2;
    GlobleReversiMap.reverse();
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    GlobleReversiMap.reverse();
    GlobleReversiMap.set(x, y, 1);
    GlobleReversiMap.reverse();
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<x<<"\",\n"
    <<"    \"y\": \""<<y<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }
  else {
    int p1,p2;
    GlobleReversiMap.reverse();
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<-1<<"\",\n"
    <<"    \"y\": \""<<-1<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }
  return 0;
}
int reversi_ANN::json2() {
  string debugstr;
  GlobleReversiMap.reverse();
  ANN valuenetwork;
  ANN policynetwork;
  valuenetwork.load_from_file("reversi_value");
  policynetwork.load_from_file("reversi_policy");
  int count = 0, i, j, x, y, location[65];
  matrix IN, OUT;
  matrix print;
  double out_value_policy[64], in_value[64], in_policy[64];
  matrix IN_value;
  matrix OUT_value;
  matrix IN_policy;
  matrix OUT_policy;
  for(i = 0;i < 8; i++) {
    for(j = 0;j < 8; j++) {
      in_policy[i * 8 + j] = GlobleReversiMap.value(i, j);
    }
  }
  IN_policy.setmatrix(1, 64, in_policy);
  OUT_policy = (policynetwork.feed(IN_policy)).transfer(logit);
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 8; j++) {
      if(GlobleReversiMap.getbenefit(i, j, 1) == -1) {
        out_value_policy[i * 8 + j] = -999;
      }
      else {
        reversi valuenetworkmap;
        char *key=new char[65];
        double test;
        GlobleReversiMap.generatekey(key);
        valuenetworkmap.setbykey(key);
        // valuenetworkmap.print();
        test = valuenetworkmap.set(i, j, 1);
        for(x = 0;x < 8; x++) {
          for(y = 0;y < 8; y++) {
            in_value[x * 8 + y] = valuenetworkmap.value(x, y);
          }
        }
        // valuenetworkmap.print();
        IN_value.setmatrix(1, 64, in_value);
        OUT_value = (valuenetwork.feed(IN_value)).transfer(logit);
        // out_value_policy[i * 8 + j] = OUT_value.get_element(1, 1);
        double value1, value2, policy;
        for(x = 0;x < 8; x++) {
          for(y = 0;y < 8; y++) {
            in_value[x * 8 + y] = valuenetworkmap.value(x, y);
          }
        }
        // valuenetworkmap.print();
        IN_value.setmatrix(1, 64, in_value);
        OUT_value = (valuenetwork.feed(IN_value)).transfer(logit);
        value1 = OUT_value.get_element(1, 1);
        for(x = 0;x < 8; x++) {
          for(y = 0;y < 8; y++) {
            in_value[x * 8 + y] = -valuenetworkmap.value(x, y);
          }
        }
        // valuenetworkmap.print();
        IN_value.setmatrix(1, 64, in_value);
        OUT_value = (valuenetwork.feed(IN_value)).transfer(logit);
        value2 = -OUT_value.get_element(1, 1);
        policy = OUT_policy.get_element(1, i * 8 + j + 1);
        policy = 0;
        out_value_policy[i * 8 + j] = value1 + policy;
        if ((i==1 && j==1)||(i==6 && j==1)||(i==1 && j==6)||(i==6 && j==6)) {
          out_value_policy[i * 8 + j] -= 2.5;
        }
        else if ((valuenetworkmap.set(0, 0, -1) >= 0)||(valuenetworkmap.set(0, 7, -1) >= 0)||(valuenetworkmap.set(7, 0, -1) >= 0)||(valuenetworkmap.set(7, 7, -1) >= 0)) {
          out_value_policy[i * 8 + j] -= 2.5;
        }
        ostringstream s;
        s << "{ \"DropPoint\": "<<"[" << i << ", " << j << ", " << test << "], \"Value\": "<< value1 <<", \"Policy\": "<< policy << ", \"Sum\": " << out_value_policy[i * 8 + j] <<"},\n";
        debugstr += s.str();
      }
    }
  }
  // search big
  double temp_value = -999;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(out_value_policy[i * 8 + j] > -999 ) {
        double value = 0;
        value = out_value_policy[i * 8 + j];
        if (value > temp_value) {
          temp_value = value;
          x = i;
          y = j;
        }
      }
    }
  }
  if(temp_value > -999) {
    int p1,p2;
    GlobleReversiMap.reverse();
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    GlobleReversiMap.reverse();
    GlobleReversiMap.set(x, y, 1);
    GlobleReversiMap.reverse();
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<x<<"\",\n"
    <<"    \"y\": \""<<y<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"debug\": [\n"<<debugstr<<" [\"end\"]],\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }
  else {
    int p1,p2;
    GlobleReversiMap.reverse();
    p1=GlobleReversiMap.mypoint();
    p2=GlobleReversiMap.compoint();
    char *A=new char[65];
    GlobleReversiMap.generatekey(A);
    char *B=new char[65];
    GlobleReversiMap.generatekey(B);
    cout
    <<"{\n"
    <<"    \"key1\": \""<<A<<"\",\n"
    <<"    \"key2\": \""<<B<<"\",\n"
    <<"    \"p1_1\": \""<<p1<<"\",\n"
    <<"    \"p2_1\": \""<<p2<<"\",\n"
    <<"    \"p1_2\": \""<<GlobleReversiMap.mypoint()<<"\",\n"
    <<"    \"p2_2\": \""<<GlobleReversiMap.compoint()<<"\",\n"
    <<"    \"x\": \""<<-1<<"\",\n"
    <<"    \"y\": \""<<-1<<"\",\n"
    <<"    \"version\": \""<<version<<"\",\n"
    <<"    \"message\": \""<<"OK"<<"\",\n"
    <<"    \"debug\": [\n"<<debugstr<<" [\"end\"]],\n"
    <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
    <<"}";
  }
  return 0;
}
int reversi_ANN::train(string filename, double distance, int use_old_base) {
  ANN myann;
  myann.load_from_file(filename);
  myann.print();
  GlobleReversiMap.print();
  int count = 0, i, j, x , y, location[65];
  double in[9999], out[9999], render_value[65];
  matrix IN, OUT;
  matrix print;
  if (use_old_base) {
    com();
  }
  else {
    this->Battle_first();
  }
  GlobleReversiMap.print();
  while(GlobleReversiMap.can_set(1) || GlobleReversiMap.can_set(-1)) {
    double in_temp[64], out_temp[64];
    matrix IN_temp;
    matrix OUT_temp;
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
        in[i * 8 + j + count * 64] = GlobleReversiMap.value(i, j);
        in_temp[i * 8 + j] = GlobleReversiMap.value(i, j);
      }
    }
    IN_temp.setmatrix(1, 64, in_temp);
    OUT_temp = (myann.feed(IN_temp)).transfer(logit);
    //clip output
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
        if(GlobleReversiMap.getbenefit(j, i, 1) == -1) {
          out[i * 8 + j + count * 64] = 0;
          out_temp[i * 8 + j] = -1;
        }
        else {
          if(OUT_temp.get_element(1, i * 8 + j + 1) <= 1 && OUT_temp.get_element(1, i * 8 + j + 1) >= 0) {
            out_temp[i * 8 + j] = OUT_temp.get_element(1, i * 8 + j + 1);
            out[i * 8 + j + count * 64] = OUT_temp.get_element(1, i * 8 + j + 1);
          }
          else if (OUT_temp.get_element(1, i * 8 + j + 1) > 1) {
            out_temp[i * 8 + j] = 1;
            out[i * 8 + j + count * 64] = 1;
          }
          if (OUT_temp.get_element(1, i * 8 + j + 1) < 0.0001) {
            out_temp[i * 8 + j] = 0.0001 * ((rand() % 10 + 1) / 10.0);
            // out[i * 8 + j + count * 64] =  0.1 * ((rand() % 10 + 1) / 10.0);
          }
          if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
            out_temp[i * 8 + j] = 1;
            out[i * 8 + j + count * 64] = 1;
          }
        }
      }
    }
    // search small
    double temp_value = -1;
    for (i=0; i<8; i++) {
      for(j=0; j<8; j++) {
        if(out_temp[i * 8 + j] >= 0 ) {
          double value = 0;
          value = out_temp[i * 8 + j];
          if (value > temp_value) {
            temp_value = value;
            x = j;
            y = i;
          }
        }
      }
    }
    if(temp_value >= 0) {
      int mypoint_before, compoint_before, mypoint_init, compoint_init;
      int com_can_set;
      // mypoint_init = GlobleReversiMap.mypoint();
      // compoint_init = GlobleReversiMap.compoint();
      location[count] = x + y * 8;
      cout << "\n------step: " << count + 1 << "------" << endl;
      cout << "clip result: " << endl;
      print.setmatrix(8, 8, out_temp);
      print.print();
      GlobleReversiMap.set(x, y, 1);
      // mypoint_before = GlobleReversiMap.mypoint();
      // compoint_before = GlobleReversiMap.compoint();
      com_can_set = GlobleReversiMap.can_set(-1);
      GlobleReversiMap.print();
      com();
      GlobleReversiMap.print();
      // if(GlobleReversiMap.can_set(1) - com_can_set > 0) {
      //   render_value[count] = 1;
      // }
      // else if(GlobleReversiMap.can_set(1) - com_can_set == 0) {
      //   render_value[count] = 0.5;
      // }
      // else {
      //   render_value[count] = 0;
      // }
      // cout << "\ncan set YOU: " << GlobleReversiMap.can_set(1) << " COM:" << com_can_set << endl;
      cout << "------------\n" << endl;
      count++;
    }
    else {
      com();
      GlobleReversiMap.print();
    }
  }
  if(GlobleReversiMap.mypoint() > GlobleReversiMap.compoint()) {
    render_value[0] = 1;
  }
  else if(GlobleReversiMap.mypoint() < GlobleReversiMap.compoint()) {
    render_value[0] = 0;
  }
  else {
    render_value[0] = 0.5;
  }
  IN.setmatrix(count, 64, in);
  OUT = this->render_outmatrix(out, count , location, render_value , distance);
  IN.save_to_file("in");
  OUT.save_to_file("out");
  return 0;
}
//--------------------------------------------------------------------
int main() {
  // cuda_init();
  reversi_ANN myreversi_ANN;
  //cout<< "---reversi trainer---" <<endl;
  //double distance, error, trytime;
  //int count = 0, use_old_base = 0;
  //cout << "old base (1/0): " << endl;
  //cin >> use_old_base;
  //cout << "distance (0.9): " << endl;
  //cin >> distance;
  //cout << "error (0.03): " << endl;
  //cin >> error;
  //cout << "try time: " << endl;
  //cin >> trytime;
  //  while(1) {
  //    double err;
  //    char A[65] = "NNNNNNNNNNNNNNNNNNNNNNNNNNNABNNNNNNBANNNNNNNNNNNNNNNNNNNNNNNNNNN";
  //    GlobleReversiMap.setbykey(A);
  //    matrix IN, OUT;
  //    ANN myann;
  //    myreversi_ANN.train("reversi" + to_string(count), distance, use_old_base);
  //    IN.load_from_file("in");
  //    OUT.load_from_file("out");
  //    myann.load_from_file("reversi" + to_string(count));
  //    err = pow(pow(error, 2) * IN.get_row(), 0.5);
  //    cout << ">>>whole min error: " << fixed << setprecision(6) << err << endl;
  //    myann.train_method_batch(IN, OUT, err, trytime, 0.01, 100, "reversi" + to_string(count + 1));
  //    myann.save_to_file("reversi" + to_string(count + 1));
  //    count++;
  //  }

    // cout << "<<< test start >>> <<< test start >>> <<< test start >>> <<< test start >>> <<< test start >>> <<< test start >>> <<< test start >>> <<< test start >>>" << endl;
    // char A[65] = "NNNNNNNNNNNNNNNNNNNNNNNNNNNABNNNNNNBANNNNNNNNNNNNNNNNNNNNNNNNNNN";
    // GlobleReversiMap.setbykey(A);
    // while(GlobleReversiMap.can_set(1) || GlobleReversiMap.can_set(-1)) {
    //   GlobleReversiMap.print();
    //   cout << "old AI >>>" << endl;
    //   com();
    //   GlobleReversiMap.print();
    //   GlobleReversiMap.reverse();
    //   cout << "first com >>>" << endl;
    //   myreversi_ANN.Battle_first();
    //   GlobleReversiMap.reverse();
    // }
    // GlobleReversiMap.print();
    // cout << "<<< test 2: first com vs old AI >>>" << endl;
    // GlobleReversiMap.setbykey(A);
    // while(GlobleReversiMap.can_set(1) || GlobleReversiMap.can_set(-1)) {
    //   GlobleReversiMap.print();
    //   GlobleReversiMap.reverse();
    //   cout << "first com >>>" << endl;
    //   myreversi_ANN.Battle_first();
    //   GlobleReversiMap.reverse();
    //   GlobleReversiMap.print();
    //   cout << "old AI >>>" << endl;
    //   com();
    // }
    // GlobleReversiMap.print();
    // cout << "<<< test 3: first com vs second com >>>" << endl;
    // GlobleReversiMap.setbykey(A);
    // while(GlobleReversiMap.can_set(1) || GlobleReversiMap.can_set(-1)) {
    //   GlobleReversiMap.print();
    //   GlobleReversiMap.reverse();
    //   cout << "first com >>>" << endl;
    //   myreversi_ANN.Battle_first();
    //   GlobleReversiMap.reverse();
    //   GlobleReversiMap.print();
    //   GlobleReversiMap.reverse();
    //   cout << "second com >>>" << endl;
    //   myreversi_ANN.Battle_second();
    //   GlobleReversiMap.reverse();
    // }
    // GlobleReversiMap.print();
    // cout << "<< test 4: second com vs first com >>>" << endl;
    // GlobleReversiMap.setbykey(A);
    // while(GlobleReversiMap.can_set(1) || GlobleReversiMap.can_set(-1)) {
    //   GlobleReversiMap.print();
    //   cout << "second com >>>" << endl;
    //   myreversi_ANN.Battle_second();
    //   GlobleReversiMap.print();
    //   cout << "first com >>>" << endl;
    //   myreversi_ANN.Battle_first();
    // }
    // GlobleReversiMap.print();

     GlobleReversiMap.clr();
     int X, Y, temp=1, pass;
     char *A=new char[65];
     string B="NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN";
     cin>>pass;
     cin>>B;
     cin>>X>>Y;
     strcpy(A,B.c_str());
     GlobleReversiMap.setbykey(A);
     if (GlobleReversiMap.can_set(1) == 0 || pass==1) {
       myreversi_ANN.json2();
       return 0;
     }
     if (GlobleReversiMap.set(X, Y, 1) == -1) {
       cout
       <<"{\n"
       <<"    \"version\": \""<<version<<"\",\n"
       <<"    \"message\": \""<<"err"<<"\",\n"
       <<"    \"copyright\": \""<<"copyright(c) 2017-2018 NOOXY inc."<<"\"\n"
       <<"}";
       return 0;
     }
     if(GlobleReversiMap.mypoint() + GlobleReversiMap.compoint() > 100) {
       com_json();
     }
     else {
       myreversi_ANN.json2();
     }
}
