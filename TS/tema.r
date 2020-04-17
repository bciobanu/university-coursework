# N samples from N(0, 1) using uniform distribution
rejectsample <- function(n) {
  MAGIC <- 1234
  K <- 5

  r <- vector()
  while (length(r) < n) {
    u <- runif(n, 0, 1)
    y <- runif(n, -K, K)
    fy <- dnorm(y)
    gy <- dunif(y, -K, K)
    
    # on average it should take MAGIC steps to get a sample
    r <- c(r, y[u < fy / (MAGIC * gy)])
  }
  r[1:n]
}

N <- 10000
# z ~ N(0, 1)
z <- rejectsample(N)

# x ~ N(0.5, 3)
miu <- 0.5
sigma <- sqrt(3)
x <- miu + sigma * z
hist(x)

# y ~ LN(0.5, 3)
y <- exp(x)
