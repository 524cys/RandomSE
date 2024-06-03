# RandomSE

Random Subsampling Ensemble method for recovering regression coefficients in the partially shuffled linear regression problem.

# Installation

To use the `RandomSE` function, you need to have the Rcpp library installed. You can install it from CRAN using the following command:

```R
install.packages("Rcpp")
```

Then, load the library in R:
```R
library("Rcpp")
```

You can then source the RandomSE.rcpp file using sourceCpp():
```R
sourceCpp("C:/RandomSE.cpp")
```

# Usage
Here's an example of how to use the RandomSE function:
```R
# Generate sample data
X <- matrix(rnorm(100), nrow = 100, ncol = 5)
e <- rnorm(100)
beta <- c(4, 2, 0, -2, 6)
Y <- X%*%beta + e

# Call the RandomSE function
beta.rs <- RandomSE(X, Y, subsamp = 10, B1 = 100, B2 = 100)

# Print the result
print(beta.rs)
```

