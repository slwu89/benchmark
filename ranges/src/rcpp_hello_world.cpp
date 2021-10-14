#include <vector>
#include <algorithm>
#include <limits>

#include <Rcpp.h>
using namespace Rcpp;


std::vector<int>::iterator add_powers_internal(std::vector<int>* dst, const int lo, const int hi, const int mult);
void add_negated_powers_internal(std::vector<int>* dst, const int lo, const int hi, const int mult);

//' @title add powers cpp
//' @examples add_powers_cpp(lo = 8, hi = bitwShiftL(a = 8, n = 10), mult = 8)
//' @export
// [[Rcpp::export]]
std::vector<int> add_powers_cpp(const int lo, const int hi, const int mult) {
  
  // BM_CHECK_GE(lo, 0);
  // BM_CHECK_GE(hi, lo);
  // BM_CHECK_GE(mult, 2);
  
  std::vector<int> dst;
  add_powers_internal(&dst, lo, hi, mult);
  
  return dst;
}

std::vector<int>::iterator add_powers_internal(std::vector<int>* dst, const int lo, const int hi, const int mult) {
  
  // BM_CHECK_GE(lo, 0);
  // BM_CHECK_GE(hi, lo);
  // BM_CHECK_GE(mult, 2);
  const size_t start_offset = dst->size();
  
  static const int kmax = std::numeric_limits<int>::max();
  
  // Rcpp::Rcout << "calling add_powers with lo: " << lo << " and hi: " << hi << " and mult: " << mult << "\n";
  
  // Space out the values in multiples of "mult"
  for (int i = 1; i <= hi; i *= mult) {
    // Rcpp::Rcout << "iterating, i: " << i << "\n"; 
    if (i >= lo) {
      dst->push_back(i);
    }
    // Break the loop here since multiplying by
    // 'mult' would move outside of the range of T
    if (i > kmax / mult) break;
  }
  return dst->begin() + start_offset;
}

//' @title add negated powers cpp
//' @examples add_negated_powers_cpp(lo = -bitwShiftL(a = 8, n = 10), hi = 0, mult = 8)
//' @export
// [[Rcpp::export]]
std::vector<int> add_negated_powers_cpp(const int lo, const int hi, const int mult) {
  
  //   BM_CHECK_GT(lo, std::numeric_limits<T>::min());
  //   BM_CHECK_GT(hi, std::numeric_limits<T>::min());
  //   BM_CHECK_GE(hi, lo);
  //   BM_CHECK_LE(hi, 0);
  
  std::vector<int> dst;
  add_negated_powers_internal(&dst, lo, hi, mult);
  
  return dst;
}

void add_negated_powers_internal(std::vector<int>* dst, const int lo, const int hi, const int mult) {
  
  int hi_inner = std::min(hi, -1);
  
  // Add positive powers, then negate and reverse.
  // Casts necessary since small integers get promoted
  // to 'int' when negating.
  const auto lo_complement = -lo;
  const auto hi_complement = -hi_inner;
  
  const auto it = add_powers_internal(dst, hi_complement, lo_complement, mult);
  
  std::for_each(it, dst->end(), [](int& t) { t *= -1; });
  std::reverse(it, dst->end());
  
}


//' @title create a range
//' @examples add_range_cpp(lo = -bitwShiftL(a = 8, n = 10), hi = bitwShiftL(a = 8, n = 10), mult = 8)
//' @export
// [[Rcpp::export]]
std::vector<int> add_range_cpp(const int lo, const int hi, const int mult = 8) {
  // static_assert(std::is_integral<T>::value && std::is_signed<T>::value,
  //               "Args type must be a signed integer");
  
  // BM_CHECK_GE(hi, lo);
  // BM_CHECK_GE(mult, 2);
  
  std::vector<int> dst;
  
  // Add "lo"
  dst.push_back(lo);
  
  // Handle lo == hi as a special case, so we then know
  // lo < hi and so it is safe to add 1 to lo and subtract 1
  // from hi without falling outside of the range of T.
  if (lo == hi) return dst;
  
  // Ensure that lo_inner <= hi_inner below.
  if (lo + 1 == hi) {
    dst.push_back(hi);
    return dst;
  }
  
  // Add all powers of 'mult' in the range [lo+1, hi-1] (inclusive).
  const auto lo_inner = lo + 1;
  const auto hi_inner = hi - 1;
  
  // Insert negative values
  if (lo_inner < 0) {
    add_negated_powers_internal(&dst, lo_inner, std::min(hi_inner, int{-1}), mult);
  }
  
  // Treat 0 as a special case (see discussion on #762).
  if (lo < 0 && hi >= 0) {
    dst.push_back(0);
  }
  
  // Insert positive values
  if (hi_inner > 0) {
    add_powers_internal(&dst, std::max(lo_inner, int{1}), hi_inner, mult);
  }
  
  // Add "hi" (if different from last value).
  if (hi != dst.back()) {
    dst.push_back(hi);
  }
  return dst;
}
