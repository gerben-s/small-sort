# small-sort
Benchmarks for various algorithms for sorting small arrays.

CC=clang blaze build -c opt :bench_sort
bazel-bin/bench_sort

# results

```
Benchmark                                        Time           CPU Iterations
-------------------------------------------------------------------------------
BM_SmallSort<gerben::BubbleSort2>/2              3 ns          3 ns  245350000
BM_SmallSort<gerben::BubbleSort2>/4              7 ns          7 ns  101100000
BM_SmallSort<gerben::BubbleSort2>/8             22 ns         21 ns   32500000
BM_SmallSort<gerben::BubbleSort2>/16            77 ns         77 ns    9200000
BM_SmallSort<gerben::BubbleSort2>/32           284 ns        284 ns    2431250
BM_SmallSort<gerben::BubbleSort2>/64          1158 ns       1158 ns     604494
BM_SmallSort<gerben::BubbleSort2>/128         4696 ns       4696 ns     149171
BM_SmallSort<gerben::InsertionSort>/2            8 ns          8 ns   89700000
BM_SmallSort<gerben::InsertionSort>/4           27 ns         27 ns   25975000
BM_SmallSort<gerben::InsertionSort>/8           87 ns         87 ns    8100000
BM_SmallSort<gerben::InsertionSort>/16         227 ns        227 ns    3056250
BM_SmallSort<gerben::InsertionSort>/32         525 ns        524 ns    1334375
BM_SmallSort<gerben::InsertionSort>/64        1264 ns       1264 ns     549824
BM_SmallSort<gerben::InsertionSort>/128       3327 ns       3325 ns     211651
BM_SmallSort<reese::insertion_sort>/2            8 ns          8 ns   84900000
BM_SmallSort<reese::insertion_sort>/4           27 ns         27 ns   25800000
BM_SmallSort<reese::insertion_sort>/8           81 ns         81 ns    8525000
BM_SmallSort<reese::insertion_sort>/16         203 ns        203 ns    3462500
BM_SmallSort<reese::insertion_sort>/32         515 ns        515 ns    1381250
BM_SmallSort<reese::insertion_sort>/64        1419 ns       1419 ns     490468
BM_SmallSort<reese::insertion_sort>/128       4420 ns       4420 ns     158543
BM_SmallSort<gerben::SelectionSort>/2            5 ns          5 ns  136900000
BM_SmallSort<gerben::SelectionSort>/4           16 ns         16 ns   42275000
BM_SmallSort<gerben::SelectionSort>/8           52 ns         52 ns   10000000
BM_SmallSort<gerben::SelectionSort>/16         236 ns        235 ns    2681250
BM_SmallSort<gerben::SelectionSort>/32        1044 ns       1044 ns     684375
BM_SmallSort<gerben::SelectionSort>/64        3062 ns       3061 ns     226490
BM_SmallSort<gerben::SelectionSort>/128       8531 ns       8531 ns      81224
```
