# RandomSE

Random Subsampling Ensemble method for recovering regression coefficients in the partially shuffled linear regression problem.

# Installation

To use the `RandomSE` function in R, you need to have the Rcpp library installed. You can install it from CRAN using the following command:

```R
install.packages("Rcpp")
```

Then, load the library:
```R
library("Rcpp")
```

You can then source the RandomSE.cpp file using sourceCpp():
```R
sourceCpp("C:/RandomSE.cpp")
```

# Usage
Here's an example of how to use the RandomSE function:
```R
# Generate sample data
n <- 100; p <- 5
X <- matrix(rnorm(100), nrow = n, ncol = p)
e <- rnorm(n)
beta <- c(4, 2, 0, -2, 6)
Y <- X%*%beta + e

# Partially shuffle the label Y
ratio <- 0.3  # shuffled ratio
k <- n * ratio
indices_to_shuffle <- sample(length(Y), k)
Y[indices_to_shuffle] <- sample(Y[indices_to_shuffle])

# Call the RandomSE function
beta.rs <- RandomSE(X, Y, subsamp = 10, B1 = 100, B2 = 100)

# Print the result
print(beta.rs)
```

