#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// Read data from given file name
void ReadData(const std::string &file_name, std::vector<long> *vect_data) {
  std::ifstream fptr;
  long data;

  vect_data->clear();
  fptr.open(file_name.c_str(), std::ifstream::in);
  while (fptr >> data) {
    vect_data->push_back(data);
  }
  fptr.close();
}

// Divide and conquer method
// Conquer
long Merge(long *vect_data, long lb, long mid, long re) {
  long count = 0;

  long *lf = new long [mid - lb];
  for (long it = lb; it != mid; ++it)
    lf[it - lb] = vect_data[it];

  long *rh = new long [re - mid];
  for (long it = mid; it != re; ++it)
    rh[it - mid] = vect_data[it];

  long i(0), j(0), index(lb);

  while (i < mid - lb && j < re - mid) {
    if (lf[i] < rh[j]) {
      vect_data[index] = lf[i++];
    }
    else {
      count += mid - lb - i;
      vect_data[index] = rh[j++];
    }
    index++;
  }

  if (i < mid - lb) {
    for (; i != mid - lb; ++i)
      vect_data[index++] = lf[i];
  }

  if (j < re - mid) {
    for(; j != re - mid; ++j)
      vect_data[index++] = rh[j];
  }

  delete [] lf;
  delete [] rh;
  return count;
}

// Divide
long CountInversion(long *vect_data, long lb, long re) {
  long count = 0;
  if (re == lb + 1)
    return 0;

  long mid = (lb + re) / 2;
  // left count
  count += CountInversion(vect_data, lb, mid);
  // right count
  count += CountInversion(vect_data, mid, re);
  // Merge
  count += Merge(vect_data, lb, mid, re);
  return count;
}

int main() {
  std::string file;
  std::cout << "Input File: ";
  std::cin >> file;
  std::vector<long> vect_data;
  ReadData(file, &vect_data);
  std::cout << "Data Read Finished" << std::endl;
  long no_inversion = CountInversion(&vect_data[0], 0, vect_data.size());
  std::cout << no_inversion << std::endl;
  return 0;
}
