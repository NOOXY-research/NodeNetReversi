#ifndef ann_h
#define ann_h

#include <iostream>
#include "matrix.h"
using namespace std;

class ANN {
  public:
    ANN();
    ANN(int layers_size, int *neurons_size);
    ANN(int layers_size, int *neurons_size, matrix *weight, matrix *bias);
    ANN(const ANN& ann1);
    ANN& operator =(const ANN& ann1);
    friend ANN operator +(const ANN& ann1, const ANN& ann2);
    friend ANN operator -(const ANN& ann1, const ANN& ann2);
    friend ANN operator /(const ANN& ann1, double x1);
    friend ANN operator *(double x1, const ANN& ann1);
    friend ANN operator *(const ANN& ann1, double x1);
    friend bool operator ==(const ANN& ann1, const ANN& ann2);
    ~ANN();
    int set(matrix *weight, matrix *bias);
    int randomweight();
    int print();
    int print_detail();
    int save_to_file(string filename);
    int load_from_file(string filename);
    int train(matrix input, matrix output, double speed);
    int train_OBP(matrix input, matrix output, double speed);
    int train_method_batch(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name);
    int train_method_random(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name);
    matrix feed(matrix input);
  private:
    int layers_size, *neurons_size;
    matrix *weight;
    matrix *bias;
};

#endif //ann_h
