

#' @title add powers
#' @export
add_powers <- function(lo, hi, mult = 8) {
  
  lo <- as.integer(lo)
  hi <- as.integer(hi)
  mult <- as.integer(mult)
  
  stopifnot(lo >= 0)
  stopifnot(hi >= lo)
  stopifnot(mult >= 2)
  
  kmax <- .Machine$integer.max
  
  # space out the values in multiples of mult, to get from lo to hi, will have about lo * mult^x = hi, solve for x
  dst <- lo * cumprod(x = c(1, rep(x = mult, each = floor(log(x = hi/lo, base = mult)))))
  
  if (any(dst > kmax)) {
    stop("some elements of generated range larger than greatest representable integer!")
  }
  
  return(as.integer(dst))
}


#' @title add negated powers
#' @export
add_negated_powers <- function(lo, hi, mult = 8) {
  
  lo <- as.integer(lo)
  hi <- as.integer(hi)
  mult <- as.integer(mult)
  
  # //   BM_CHECK_GT(lo, std::numeric_limits<T>::min());
  # //   BM_CHECK_GT(hi, std::numeric_limits<T>::min());
  # //   BM_CHECK_GE(hi, lo);
  # //   BM_CHECK_LE(hi, 0);
  
  hi_inner = min(hi, -1L);
  
  lo_complement <- -lo
  hi_complement <- -hi_inner
  
  dst <- add_powers(lo = hi_complement, hi = lo_complement, mult = mult)
  dst <- rev(-dst)
  
  return(as.integer(dst))
}

#' @title create a range
#' @export
add_range <- function(lo, hi, mult = 8) {
  
  lo <- as.integer(lo)
  hi <- as.integer(hi)
  mult <- as.integer(mult)
  
  stopifnot(hi >= lo)
  stopifnot(mult >= 2)
  
  dst <- lo
  
  # Handle lo == hi as a special case, so we then know
   # lo < hi and so it is safe to add 1 to lo and subtract 1
  # from hi without falling outside of the range of ints.
  if (lo == hi) {
    return(dst)
  }
  
  # ensure that lo_inner <= hi_inner below
  if (lo + 1L == hi) {
    dst <- c(dst, hi)
    return(dst)
  }
  
  # Add all powers of 'mult' in the range [lo+1, hi-1] (inclusive).
  lo_inner = lo + 1;
  hi_inner = hi - 1;
  
  # insert negative values
  if (lo_inner < 0L) {
    dst_n <- add_negated_powers(lo = lo_inner, hi = min(hi_inner, -1L), mult = mult)
    dst <- c(dst, dst_n)
  }
  
  # treat 0 as a special case
  if (lo < 0L & hi >= 0L) {
    dst <- c(dst, 0L)
  }
  
  # insert positive values
  if (hi_inner > 0L) {
    dst_p <- add_powers(lo = max(lo_inner, 1L), hi = hi_inner, mult = mult)
    dst <- c(dst, dst_p)
  }
  
  # add "hi" if different from last value
  if (hi != dst[length(dst)]) {
    dst <- c(dst, hi)
  }
  
  return(dst)
}