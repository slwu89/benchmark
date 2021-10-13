

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


# hi = bitwShiftL(a = 8, n = 10)
# lo = 8
# mult= 2
# 
# log(x = hi/lo, base = mult)


