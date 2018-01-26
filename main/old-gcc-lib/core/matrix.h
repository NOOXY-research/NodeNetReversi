#ifndef matrix_h
#define matrix_h

#include <iostream>
using namespace std;

double sigmoid(double x1);
double dsigmoid(double x1);
double logit(double x1);
class matrix {
  public:
    matrix();
    matrix(const matrix& m1);
    matrix& operator =(const matrix& m1);
    ~matrix();
    matrix(int row, int column);
    matrix(int row, int column, double value);
    matrix(int row, int column, double* value);
    int setmatrix(int row, int column, double* a);
    int setmatrix(double* a);
    int get_row();
    int get_column();
    double get_element(int row, int column);
    matrix get_row_as_matrix(int row);
    int print();
    int save_to_file(string filename);
    int load_from_file(string filename);
    double length();
    matrix transfer(double (*function)(double));
    matrix transpose();
    friend matrix multi(const matrix& m1, const matrix& m2);
    friend matrix cost_OBP(const matrix& m1, const matrix& m2); //OBP = optical backpropagation
    friend matrix fliter_max_value(const matrix& m1, double x1);
    friend matrix operator +(const matrix& m1, const matrix& m2);
    friend matrix operator -(const matrix& m1, const matrix& m2);
    friend matrix operator *(const matrix& m1, const matrix& m2);
    friend matrix operator *(const double& x1, const matrix& m1);
    friend matrix operator *(const matrix& m1, const double& x1);
    friend matrix operator /(const double& x1, const matrix& m1);
    friend matrix operator /(const matrix& m1, const double& x1);
    friend matrix operator -(const matrix& m1);
    friend bool operator ==(const matrix& m1, const matrix& m2);
    friend ostream& operator<<(ostream &out, const matrix& m1);
    friend istream& operator>>(istream &in, matrix& m1);
  private:
    int row, column;
    double *a;
};

#endif //matrix_h
