#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <future>
#include <chrono>

bool isPrime(int n) {
  if (n <= 1 || n % 2 == 0) return false;

  for (int i = 3; i * i <= n; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
}

void compute(int start, int end, int threadId, const std::vector<int>& primes, std::vector<int>& result, int& partialSum) {
  for (int i = start; i <= end; ++i) {
    if (isPrime(i)) {
      result.emplace_back(i);
      partialSum += i;
    }
  }
}

int main() {
  const int num = 8;
  const int S = 1;
  const int E = 100000000;

  std::vector<std::future<void>> futures;
  std::vector<std::vector<int>> partial(num);
  std::vector<int> sums(num);
  int totalPrimes = 0;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num; ++i) {
    int start = S + (i * (E - S + 1) / num);
    int end = S + ((i + 1) * (E - S + 1) / num) - 1;
    futures.emplace_back(std::async(std::launch::async, compute, start, end, i, std::cref(partial[i]), std::ref(partial[i]), std::ref(sums[i])));
  }

  // Wait for all futures to finish
  for (auto& future : futures) future.wait();

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::ofstream myfile("primes.txt");
  myfile << "<" << duration.count() << "ms> ";

  for (const auto& list : partial) totalPrimes += list.size();
  myfile << "<" << totalPrimes << "> ";

  int primeSum = std::accumulate(sums.begin(), sums.end(), 0);
  myfile << "<" << primeSum << ">\n<";

  std::vector<int> destinationVector(partial[num - 1].end() - 10, partial[num - 1].end());
  for (int i = 0; i < 10; ++i) myfile << destinationVector[i] << " ";
  myfile << ">" << std::endl;

  return 0;
}
