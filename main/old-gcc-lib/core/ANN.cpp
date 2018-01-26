#include <iostream>
#include <fstream> //file
#include <iomanip>//setprecision
#include <math.h>//pow()
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "matrix.h"
#include "ANN.h"
using namespace std;

ANN::ANN () {
  this->layers_size = 1;
  this->neurons_size = new int[1];
  this->weight = new matrix[1];
  this->bias = new matrix[1];
}
ANN::ANN(int layers_size, int *neurons_size) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = layers_size;
  this->neurons_size = new int[layers_size];
  this->weight = new matrix[layers_size - 1];
  this->bias = new matrix[layers_size - 1];
  for(i = 0; i < layers_size; i++) {
    this->neurons_size[i] = neurons_size[i];
    if(i < layers_size -1) {
      matrix new_matrix1(neurons_size[i], neurons_size[i + 1]);
      this->weight[i] = new_matrix1;
      matrix new_matrix2(1, neurons_size[i + 1]);
      this->bias[i] = new_matrix2;
    }
  }
}
ANN::ANN(int layers_size, int *neurons_size, matrix *weight, matrix *bias) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = layers_size;
  this->neurons_size = new int[layers_size];
  this->weight = new matrix[layers_size - 1];
  this->bias = new matrix[layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = neurons_size[i];
    if(i < this->layers_size -1) {
      if (weight[i].get_row() != neurons_size[i] || weight[i].get_column() != neurons_size[i + 1])
        cout << "ann error: weights not fit neurons size" << endl;
      if (bias[i].get_row() != 1 || bias[i].get_column() != neurons_size[i + 1])
        cout << "ann error: bias not fit neurons size" << endl;
      this->weight[i] = weight[i];
      this->bias[i] = bias[i];
    }
  }
}
ANN::ANN(const ANN& ann1) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = ann1.layers_size;
  this->neurons_size = new int[ann1.layers_size];
  this->weight = new matrix[ann1.layers_size - 1];
  this->bias = new matrix[ann1.layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = ann1.neurons_size[i];
    if(i < this->layers_size -1) {
      this->weight[i] = ann1.weight[i];
      this->bias[i] = ann1.bias[i];
    }
  }
}
ANN& ANN::operator =(const ANN& ann1) {
  if(&ann1 == this)
    return *this;
  int i, j, neurons_size_sum = 0;
  delete [] neurons_size;
  delete [] weight;
  delete [] bias;
  this->layers_size = ann1.layers_size;
  this->neurons_size = new int[ann1.layers_size];
  this->weight = new matrix[ann1.layers_size - 1];
  this->bias = new matrix[ann1.layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = ann1.neurons_size[i];
    if(i < this->layers_size -1) {
      this->weight[i] = ann1.weight[i];
      this->bias[i] = ann1.bias[i];
    }
  }
  return *this;
}
ANN operator +(const ANN& ann1,const ANN& ann2) {
  int i, j;
  ANN answer;
  if(ann1 == ann2) {
    answer = ann1;
    for(i = 0; i < ann1.layers_size - 1; i++) {
      answer.weight[i] = ann1.weight[i] + ann2.weight[i];
      answer.bias[i] = ann1.bias[i] + ann2.bias[i];
    }
  }
  else {
    cout << "ann error: operator -" << endl;
  }
  return answer;
}
ANN operator -(const ANN& ann1,const ANN& ann2) {
  int i, j;
  ANN answer;
  if(ann1 == ann2) {
    answer = ann1;
    for(i = 0; i < ann1.layers_size - 1; i++) {
      answer.weight[i] = ann1.weight[i] - ann2.weight[i];
      answer.bias[i] = ann1.bias[i] - ann2.bias[i];
    }
  }
  else {
    cout << "ann error: operator -" << endl;
  }
  return answer;
}
ANN operator /(const ANN& ann1, double x1) {
  int i, j;
  ANN answer;
  answer = ann1;
  if(x1 == 0) {
    cout << "ann error: operator /" << endl;
  }
  for(i = 0; i < ann1.layers_size - 1; i++) {
    answer.weight[i] = ann1.weight[i] / x1;
    answer.bias[i] = ann1.bias[i] / x1;
  }
  return ann1;
}
ANN operator *(double x1,const ANN& ann1) {
  int i, j;
  ANN answer;
  answer = ann1;
  for(i = 0; i < ann1.layers_size - 1; i++) {
    answer.weight[i] = ann1.weight[i] * x1;
    answer.bias[i] = ann1.bias[i] * x1;
  }
  return ann1;
}
ANN operator *(const ANN& ann1, double x1) {
  int i, j;
  ANN answer;
  answer = ann1;
  for(i = 0; i < ann1.layers_size - 1; i++) {
    answer.weight[i] = ann1.weight[i] * x1;
    answer.bias[i] = ann1.bias[i] * x1;
  }
  return ann1;
}
bool operator ==(const ANN& ann1, const ANN& ann2) {
  int i, j;
  bool answer = true;
  if(ann1.layers_size != ann2.layers_size) {
    answer = false;
  }
  else {
    for(i = 0; i < ann1.layers_size; i++) {
      if(ann1.neurons_size[i] != ann2.neurons_size[i]) {
        answer = false;
      }
    }
  }
  return answer;
}
ANN::~ANN () {
  delete [] weight;
  delete [] bias;
  delete [] neurons_size;
}
int ANN::set(matrix *weight, matrix *bias) {
  int i, j;
  for(i = 0; i < this->layers_size; i++) {
    if(i < this->layers_size -1) {
      if (weight[i].get_row() != this->neurons_size[i] || weight[i].get_column() != this->neurons_size[i + 1])
        cout << "ann error: weights not fit neurons size" << endl;
        if (bias[i].get_row() != 1 || bias[i].get_column() != this->neurons_size[i + 1])
          cout << "ann error: bias not fit neurons size" << endl;
      this->weight[i] = weight[i];
      this->bias[i] = bias[i];
    }
  }
  return 0;
}
int ANN::randomweight() {
  int i, j;
  for(i = 0; i < this->layers_size - 1; i++) {
    matrix new_matrix1(this->neurons_size[i], this->neurons_size[i + 1]);
    this->weight[i] = new_matrix1;
    matrix new_matrix2(1, this->neurons_size[i + 1]);
    this->bias[i] = new_matrix2;
  }
  return 0;
}
int ANN::print_detail() {
  int i, j;
  cout<<">>>-----ANN info detail-----"<<endl;
  for(i = 0; i < this->layers_size; i++) {
    cout<<">>>";
    cout << "N(" <<  i << "): " << this->neurons_size[i]<<endl;
    if(i < this->layers_size -1) {
      cout<<">>>weight"<<endl;
      this->weight[i].print();
      cout<<">>>bias"<<endl;
      this->bias[i].print();
    }
  }
  return 0;
}
int ANN::print() {
  int i, j;
  cout<<">>>-----ANN info-----"<<endl;
  cout<<">>>";
  cout << "layers size: " << this->layers_size << endl;
  for(i = 0; i < this->layers_size; i++) {
    cout<<">>>";
    cout << "N(" <<  i << "): " << this->neurons_size[i]<<endl;
  }
  return 0;
}
int ANN::save_to_file(string filename) {
  int i, j;
  ofstream myfile ((filename + ".node").c_str());
  myfile << this->layers_size;
  for(i = 0; i < this->layers_size; i++) {
    myfile << " " << this->neurons_size[i];
  }
  myfile << endl;
  for(i = 0; i < this->layers_size - 1; i++) {
    myfile << this->weight[i];
  }
  for(i = 0; i < this->layers_size - 1; i++) {
    myfile << this->bias[i];
  }
  myfile.close();
  return 0;
}
int ANN::load_from_file(string filename) {
  int i, j;
  ifstream myfile ((filename + ".node").c_str());
  if(myfile.is_open()) {
    delete [] this->neurons_size;
    delete [] this->weight;
    delete [] this->bias;
    myfile >> this->layers_size;
    this->neurons_size = new int[this->layers_size];
    this->weight = new matrix[this->layers_size - 1];
    this->bias = new matrix[this->layers_size - 1];
    for(i = 0; i < this->layers_size; i++) {
      myfile >> this->neurons_size[i];
    }
    for(i = 0; i < this->layers_size - 1; i++) {
      myfile >> this->weight[i];
    }
    for(i = 0; i < this->layers_size - 1; i++) {
      myfile >> this->bias[i];
    }
  }
  else {
    return -1;
  }
  return 0;
}
int ANN::train(matrix input, matrix output,double speed) {
  if (input.get_column() != this->neurons_size[0]) {
    cout << "ann error: neurons size not fit" << endl;
    matrix err(0, 0);
    return -1;
  }
  int i, j;
  matrix *delta = new matrix[this->layers_size], *dj_dweight = new matrix[this->layers_size - 1],
   *a = new matrix[this->layers_size], *z = new matrix[this->layers_size];
  matrix a1;
  z[0] = input;
  a1 = input.transfer(sigmoid);
  matrix bias_formator(input.get_row(), 1, 1);
  for(i = 0; i < this->layers_size -1; i++) {
    a[i] = a1;
    z[i + 1] = a1 * weight[i] + bias_formator * bias[i];
    a1 = (a[i] * weight[i] + bias_formator * bias[i]).transfer(sigmoid);
  }
  delta[this->layers_size - 1] = multi(-(output.transfer(sigmoid) - a1), z[this->layers_size - 1].transfer(dsigmoid));
  for(i = this->layers_size - 1; i > 0 ; i--) { // i start max layers_size
    dj_dweight[i - 1] = a[i - 1].transpose() * delta[i];
    delta[i - 1] = multi(delta[i] * weight[i - 1].transpose(), z[i - 1].transfer(dsigmoid));
  }
  for(i = 0; i < this->layers_size - 1; i++) {
    matrix bias_delta_formator(1, input.get_row(), 1);
    this->weight[i] = this->weight[i] - speed * dj_dweight[i];
    this->bias[i] = this->bias[i] - speed * bias_delta_formator * delta[i + 1];
    // (speed * bias_delta_formator * delta[i + 1]).print();
  }
  delete [] delta;
  delete [] dj_dweight;
  delete [] a;
  delete [] z;
  return 0;
}
int ANN::train_OBP(matrix input, matrix output,double speed) {//abandoned
  if (input.get_column() != this->neurons_size[0]) {
    cout << "ann error: neurons size not fit" << endl;
    matrix err(0, 0);
    return -1;
  }
  int i, j;
  matrix *delta = new matrix[this->layers_size], *dj_dweight = new matrix[this->layers_size - 1],
   *a = new matrix[this->layers_size], *z = new matrix[this->layers_size];
  matrix a1;
  z[0] = input;
  a1 = input.transfer(sigmoid);
  for(i = 0; i < this->layers_size -1; i++) {
    a[i] = a1;
    z[i + 1] = a1 * weight[i];
    a1 = (a[i] * weight[i]).transfer(sigmoid);
  }
  matrix cost;
  cost = cost_OBP(output.transfer(sigmoid), a1);
  delta[this->layers_size - 1] = multi(-(cost), z[this->layers_size - 1].transfer(dsigmoid));
  for(i = this->layers_size - 1; i > 0 ; i--) { // i start max layers_size
    dj_dweight[i - 1] = a[i - 1].transpose() * delta[i];
    delta[i - 1] = multi(delta[i] * weight[i - 1].transpose(), z[i - 1].transfer(dsigmoid));
  }
  for(i = 0; i < this->layers_size - 1; i++) {
    this->weight[i] = this->weight[i] - fliter_max_value(speed * dj_dweight[i], 0.05);
    // cout << (speed * dj_dweight[i]).length() << " " << flush;
  }
  delete [] delta;
  delete [] dj_dweight;
  delete [] a;
  delete [] z;
  return 0;
}
int ANN::train_method_batch(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name) {
  int i, j, k;
  ANN good;
  // this->train(input, output, speed);
  double good_err = 99999, firsterr = (this->feed(input) - output.transfer(sigmoid)).length();
  int count = 0;
  int use_cache = 0;
  int cache_count = 0;
  double speed_max = speed;
  matrix *weight_cache = new matrix[this->layers_size];

  // optimize
  // for(i = 0; i < 2; i++){
  //   cout << ">>>optimizing training(" << i + 1 << "/2)..." << endl;
  //   for(j = 0; j < input.get_row(); j++) {
  //     for(k = 0; k < 100; k++) {
  //       this->train_OBP(input.get_row_as_matrix(j + 1), output.get_row_as_matrix(j + 1), 0.3);
  //     }
  //     cout << "(" << j + 1<< "/" << input.get_row() << ")..." << flush;
  //   }
  //   cout << endl;
  // }
  // optimize
  cout << "[ " << fixed << setprecision(6) << firsterr << " first      ] ";
  cout << "********************************************************************************" << endl;
  good = (*this);
  while((this->feed(input) - output.transfer(sigmoid)).length() > err && (count < max_times || max_times == -1)) {
    if(count % loop == 0) {
      use_cache = 0;
      cout << fixed;
      cout << setprecision(6);
      if ( good_err == (this->feed(input) - output.transfer(sigmoid)).length()) {
        cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " same       ] ";
      }
      else if ( good_err > (this->feed(input) - output.transfer(sigmoid)).length()) {
	      // cout << "cache used: " << cache_count << " ";
	      cache_count = 0;
	      for(i = 0; i < this->layers_size - 1; i++) {
	        weight_cache[i] = this->weight[i] - good.weight[i];
	        use_cache = 1;
	      }
        while(use_cache == 1) {
          int i;
          double err_temp = 0;
          err_temp = ((this->feed(input) - output.transfer(sigmoid)).length());
          for(i = 0; i < layers_size - 1; i++) {
            this->weight[i] = this->weight[i] + weight_cache[i];
          }
          if((this->feed(input) - output.transfer(sigmoid)).length() >= err_temp) {
            for(i = 0; i < layers_size - 1; i++) {
	            this->weight[i] = this->weight[i] - weight_cache[i];
            }
            //cout << "bad_cache result: " << (this->feed(input) - output.transfer(sigmoid)).length() << endl;
	          use_cache = 0;
          }
          else {
            cache_count++;
            if(cache_count >= 100000) {
              use_cache = 0;
            }
          }
        };
        if(cache_count > 0) {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " o.k. boost ] ";
        }
        else {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " o.k.       ] ";
        }
        good_err = (this->feed(input) - output.transfer(sigmoid)).length();
        good = (*this);
      }
      else if ( good_err < (this->feed(input) - output.transfer(sigmoid)).length()) {
        for(i = 0; i < this->layers_size - 1; i++) {
          weight_cache[i] = this->weight[i] - good.weight[i];
          use_cache = 1;
        }
        cache_count = 0;
        while(use_cache == 1) {
          int i;
          double err_temp = 0;
          err_temp = ((this->feed(input) - output.transfer(sigmoid)).length());
          for(i = 0; i < layers_size - 1; i++) {
            this->weight[i] = this->weight[i] + weight_cache[i];
          }
          if((this->feed(input) - output.transfer(sigmoid)).length() >= err_temp) {
            for(i = 0; i < layers_size - 1; i++) {
              this->weight[i] = this->weight[i] - weight_cache[i];
            }
            use_cache = 0;
          }
          else {
            cache_count++;
            if(cache_count >= 100000) {
              use_cache = 0;
            }
          }
        };
        if(cache_count > 0) {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " bad boost  ] ";
        }
        else {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " bad        ] ";
        }
      }
      for (int i = 0; i < ((this->feed(input) - output.transfer(sigmoid)).length() / firsterr) * 80; i++) {
        if(i == 80) {
          break;
        }
        cout << "*";
      }
      cout << endl;
      // (*this) = good;
      // speed = speed_max * ((rand() % 999) * 0.001 );
      good.save_to_file(ann_name + "_latest");
    }
    this->train(input, output, speed);
    count ++;
  }
  delete [] weight_cache;
  this->print();
  cout << ">>>-----origin out-----" << endl;
  (output).print();
  cout << ">>>-----feed out-----" << endl;
  ((this->feed(input)).transfer(logit)).print();
  cout << ">>>-----err value-----" << endl;
  cout << fixed;
  cout << setprecision(10);
  cout << ">>>" << (this->feed(input) - output.transfer(sigmoid)).length() << endl;
  cout << ">>>-----try times-----" << endl;
  cout << count << endl;
  if (count < max_times) {
    return count;
  }
  else {
    return -1;
  }
}
matrix ANN::feed(matrix input) {
  if (input.get_column() != this->neurons_size[0]) {
    cout << "ann error: neurons size not fit" << endl;
    matrix err(0, 0);
    return err;
  }
  int i, j;
  matrix a1, a2;
  matrix bias_formator(input.get_row(), 1, 1);
  a1 = input.transfer(sigmoid);
  for(i = 0; i < this->layers_size -1 ; i++) {
    a2 = (a1 * this->weight[i] + bias_formator * this->bias[i]).transfer(sigmoid);
    a1 = a2;
  }
  return a1;
}
int ANN::train_method_random(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name) {
  int i, j, k;
  ANN good;
  // this->train(input, output, speed);
  double good_err = 99999, firsterr = (this->feed(input) - output.transfer(sigmoid)).length();
  int count = 0;
  int use_cache = 0;
  int cache_count = 0;
  // optimize
  cout << "[ " << fixed << setprecision(6) << firsterr << " first ] ";
  cout << "********************************************************************************" << endl;
  good = (*this);
  while((this->feed(input) - output.transfer(sigmoid)).length() > err && (count < max_times || max_times == -1)) {
    if(count % loop == 0) {
      use_cache = 0;
      cout << fixed;
      cout << setprecision(6);
      if ( good_err == (this->feed(input) - output.transfer(sigmoid)).length()) {
        cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " same  ] ";
      }
      else if ( good_err > (this->feed(input) - output.transfer(sigmoid)).length()) {
	      cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " o.k.  ] ";
        good_err = (this->feed(input) - output.transfer(sigmoid)).length();
        good = (*this);
      }
      else if ( good_err < (this->feed(input) - output.transfer(sigmoid)).length()) {
        cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " bad   ] ";
      }
      for (int i = 0; i < ((this->feed(input) - output.transfer(sigmoid)).length() / firsterr) * 80; i++) {
        if(i == 80) {
          break;
        }
        cout << "*";
      }
      cout << endl;
      good.save_to_file(ann_name + "_latest");
    }
    this->train(input.get_row_as_matrix(rand() % input.get_row() + 1), output.get_row_as_matrix(rand() % input.get_row() + 1), speed);
    count ++;
  }
  this->print();
  cout << ">>>-----origin out-----" << endl;
  (output).print();
  cout << ">>>-----feed out-----" << endl;
  ((this->feed(input)).transfer(logit)).print();
  cout << ">>>-----err value-----" << endl;
  cout << fixed;
  cout << setprecision(10);
  cout << ">>>" << (this->feed(input) - output.transfer(sigmoid)).length() << endl;
  cout << ">>>-----try times-----" << endl;
  cout << count << endl;
  if (count < max_times) {
    return count;
  }
  else {
    return -1;
  }
}
