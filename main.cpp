#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <numeric>

bool isPrime(int n) {
  if (n<=1) return false;

  for (int i=2;i<=n/2;i++){
    if(n%i==0) return false;
  }
  return true;
}


void compute(int start, int end, int threadId, std::vector<int>& result, int& partialSum) {
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            result.push_back(i);
            partialSum +=i;
        }
    }
}

int main() {
  using namespace std;
  
  //n is number of Threads, (S,E) is the range.
  const int num = 8;
  const int S = 1;
  const int E = 100000000;

  vector<thread> threads;
  vector<vector<int>> partial(num);
  vector<int> sums(num);
  int totalPrimes=0;

  auto start = chrono::high_resolution_clock::now();
  
  for (int i = 0; i < num; ++i) {
    int start = S + (i * (E - S + 1) / num);
    int end = S + ((i + 1) * (E - S + 1) / num) - 1;
    threads.emplace_back(compute, start, end, i, ref(partial[i]), ref(sums[i]));
  }
  
  // Wait for all threads to finish
  for (auto& thread : threads) thread.join();
  
  //finish time
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  ofstream myfile;
  myfile.open("primes.txt");
  myfile << "<" << duration.count() << "ms> ";
  
  for (const auto& list : partial) totalPrimes+=list.size();
  myfile << "<" << totalPrimes << "> ";
  
  int primeSum=accumulate(sums.begin(), sums.end(), 0);
  myfile << "<" << primeSum << ">\n<";
  
  vector<int> destinationVector(partial[num-1].end() - 10, partial[num-1].end());
  for (int i = 0; i < 10; ++i) myfile << destinationVector[i] << " ";
  myfile << ">" << endl;
  
  myfile.close();
  return 0;
}
