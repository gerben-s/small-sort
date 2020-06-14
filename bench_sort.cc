// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <random>

#include <benchmark/benchmark.h>

constexpr int FLAGS_number = 100000;

template <typename T>
size_t SearchLeaf(T* arr, size_t idx, size_t n) {
  auto left = 2 * idx;
  while (left < n) {
    auto right = left + 1;
    auto l = arr[left];
    auto r = arr[right];
    bool is_smaller = l < r;
    auto max = is_smaller ? r : l;
    arr[idx] = max;
    idx = is_smaller ? right : left;
    left = 2 * idx;
  }
  if (left == n) {
    arr[idx] = arr[left];
    idx = left;
  }
  return idx;
}

template <typename T>
void SiftDown(T* arr, size_t idx, size_t n, T elem) {
  auto left = 2 * idx;
  while (left < n) {
    auto right = left + 1;
    auto l = arr[left];
    auto r = arr[right];
    bool is_smaller = l < r;
    auto max = is_smaller ? r : l;
    auto next_idx = is_smaller ? right : left;
    is_smaller = max < elem;
    arr[idx] = is_smaller ? elem : max;
    elem = is_smaller ? max : elem;
    idx = next_idx;
    left = 2 * next_idx;
  }
  if (left == n) {
    auto l = arr[left];
    bool is_smaller = l < elem;
    arr[idx] = is_smaller ? elem : l;
    elem = is_smaller ? l : elem;
    idx = left;    
  }
  arr[idx] = elem;
}

template <typename T>
void Insert(T* arr, T cur, size_t root, size_t idx) {
  auto parent = idx / 2;
  if (parent >= root) {
    auto tmp = arr[parent];
    if (tmp < cur) {
      arr[idx] = tmp;
      return Insert(arr, cur, root, parent);
    }
  }
  arr[idx] = cur;
}

template <typename T>
void Heapify(T* arr, size_t n) {
  for (int i = n / 2; i >= 1; i--) SiftDown(arr, i, n, arr[i]);
}

template <typename T>
bool IsHeap(T* arr, size_t n) {
  for (size_t i = 1; i <= n / 2; i++) {
    if (arr[i] < arr[2 * i]) return false;
    if (2 * i + 1 <= n && arr[i] < arr[2 * i + 1]) return false;
  }
  return true;
}

template <typename T>
void HeapSort(T* arr, size_t n) {
  arr -= 1;
  Heapify(arr, n);
  for (size_t i = n; i > 1; i--) {
    auto tmp = arr[i];
    arr[i] = arr[1];
#if 0
    SiftDown(arr, 1, i - 1, tmp);
#elif 1
    if (i >= 33) {
      auto idx1 = SearchLeaf(arr, 1, 31);
      if (arr[idx1 >> 1] < tmp) {
        Insert(arr, tmp, 1, idx1);
        continue;
      }
      auto tmp2 = arr[i - 1];
      arr[i - 1] = arr[1];
      auto idx2 = SearchLeaf(arr, 1, 15);
      if (idx2 == idx1 >> 1) {
        // Colliding path
        SiftDown(arr, idx1, i - 2, tmp);
        SiftDown(arr, idx2, i - 2, tmp2);
      } else {
        
      }
    } else {
      SiftDown(arr, 1, i - 1, tmp);
    }
#else
    auto leaf = SearchLeaf(arr, 1, i - 1);
    Insert(arr, tmp, 1, leaf);
#endif
  }
}

namespace reese {
  
// This code is copy pasted from a reddit post by bjorn-reese
// https://www.reddit.com/r/cpp/comments/gtkrtg/hoares_rebuttal_and_bubble_sorts_comeback/fta6sai
template <typename RandomAccessIterator, typename Compare>
RandomAccessIterator push_sorted(RandomAccessIterator first,
                                 RandomAccessIterator last,
                                 Compare compare)
{
    auto length = last - first;
    auto temp = std::move(first[length]);
    while (compare(temp, first[length - 1]))
    {
        first[length] = std::move(first[length - 1]);
        --length;
        if (length == 0)
            break;
    }
    first[length] = std::move(temp);
    return first + length;
}

template <typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Compare compare)
{
    if (last - first <= 1)
        return;

    for (auto current = first + 1; current != last; ++current)
    {
        push_sorted(first, current, compare);
    }
}

}  // namespace reese

namespace gerben {
  
template <typename RandomIt, typename Compare>
void InsertionSort(RandomIt first, RandomIt last, Compare comp) {
  auto n = last - first;
  auto f = first[0];
  for (decltype(n) i = 1; i < n; i++) {
    auto x = first[i];
    if (__builtin_expect(comp(x, f), 0)) {
      // Move the whole array
      for (auto j = i; j > 0; j--) first[j] = first[j - 1];
      first[0] = f = x;
    } else {
      // Move part of the array.
      auto j = i;
      for (; comp(x, first[j - 1]); j--) first[j] = first[j - 1];
      first[j] = x;
    }
  }
}

template <typename RandomIt, typename Compare>
void SelectionSort(RandomIt first, RandomIt last, Compare comp) {
  auto n = last - first;
  for (decltype(n) i = 0; i < n; i++) {
    auto f = first[i];
    auto min = f;
    auto idx = i;
    for (auto j = i + 1; j < n; j++) {
      auto tmp = first[j];
      bool is_smaller = comp(tmp, min);
      min = is_smaller ? tmp : min;
      idx = is_smaller ? j : i;
    }
    first[i] = min;
    first[idx] = f;
  }
}

template <typename RandomIt, typename Compare>
void BubbleSort2(RandomIt first, RandomIt last, Compare comp = std::less<>{}) {
  auto n = last - first;
  for (auto i = n; i > 1; i -= 2) {
    auto x = first[0];
    auto y = first[1];
    if (y < x) std::swap(x, y);
    for (decltype(n) j = 2; j < i; j++) {
      auto z = first[j];
      bool is_smaller = comp(z, y);
      auto w = is_smaller ? z : y;
      y = is_smaller ? y : z;
      is_smaller = comp(z, x);
      first[j - 2] = is_smaller ? z : x;
      x = is_smaller ? x : w;
    }
    first[i - 2] = x;
    first[i - 1] = y;
  }
}

}  // namespace gerben

template<void (*small_sort)(int*, int*, std::less<>)>
void BM_SmallSort(benchmark::State& state) {
  int n = state.range(0);
  std::vector<int> buf(FLAGS_number);
  std::mt19937 rnd;
  std::uniform_int_distribution<int> dist(0, 100000000);
  auto num_batches = FLAGS_number / n;
  // Make sure to benchmark in large batches otherwise the overhead of pausing and
  // resuming timer makes the benchmark inaccurate for sorting a few elements.
  while (state.KeepRunningBatch(num_batches)) {
    state.PauseTiming();
    // Generate completely random data for new batch of sorts. Each sort must be
    // random otherwise the branch predictor will quickly learn sequences perfectly.
    for (auto& x : buf) x = dist(rnd);
    state.ResumeTiming();
    for (int i = 0; i + n <= FLAGS_number; i += n) {
      small_sort(buf.data() + i, buf.data() + i + n, std::less<>{});
    }
  }
}
BENCHMARK_TEMPLATE(BM_SmallSort, gerben::BubbleSort2)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_SmallSort, gerben::InsertionSort)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_SmallSort, reese::insertion_sort)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_SmallSort, gerben::SelectionSort)->RangeMultiplier(2)->Range(2, 128);

int main(int argc, char* argv[]) {
  benchmark::Initialize(&argc, argv);

  benchmark::RunSpecifiedBenchmarks();

  return 0;
}
