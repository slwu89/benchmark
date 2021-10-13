## ranges

The purpose of this package will be to provide an easy way to make flexible
numeric ranges for benchmarking numerical code in R. Benchmarking packages in
R include:

  * [bench](https://github.com/r-lib/bench)
  * [microbenchmark](https://github.com/joshuaulrich/microbenchmark/)
  * [rbenchmark](https://cran.r-project.org/package=rbenchmark)
  
This package _isn't_ for benchmarking, just to set up ranges.
  
The ranges should be what [Google benchmark](https://github.com/google/benchmark/blob/main/docs/user_guide.md#passing-arguments)
can generate. They can be found in 2 files, [benchmark_register.h](https://github.com/slwu89/benchmark/blob/main/src/benchmark_register.h)
and [benchmark_register.cc](https://github.com/slwu89/benchmark/blob/main/src/benchmark_register.cc).

Let's look at how the Google benchmark library sets up a single range:

```
Benchmark* Benchmark::Range(int64_t start, int64_t limit) {
  BM_CHECK(ArgsCnt() == -1 || ArgsCnt() == 1);
  std::vector<int64_t> arglist;
  AddRange(&arglist, start, limit, range_multiplier_);

  for (int64_t i : arglist) {
    args_.push_back({i});
  }
  return this;
}
```

