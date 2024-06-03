// RandomSE.cpp
#include <RcppArmadillo.h>
using namespace Rcpp;

// [[Rcpp::depends(RcppArmadillo)]]

// Function to sample without replacement
// [[Rcpp::export]]
IntegerVector sampleWithoutReplacement(int n, int k) {
    Function sample_function("sample");
    IntegerVector result = as<IntegerVector>(sample_function(n, k, false));
    return result;
}

// Function for Ordinary Least Squares (OLS) estimates
// [[Rcpp::export]]
NumericVector ols(NumericMatrix X, NumericVector y) {
    arma::mat Xmat = as<arma::mat>(X);
    arma::vec yvec = as<arma::vec>(y);
    arma::vec beta = inv(Xmat.t() * Xmat) * Xmat.t() * yvec;
    return wrap(beta);
}

// Function to multiply matrix by vector
// [[Rcpp::export]]
NumericVector matrixVectorMultiply(NumericMatrix mat, NumericVector vec) {
    int nrow = mat.nrow();
    int ncol = mat.ncol();

    if (ncol != vec.size()) {
        stop("Matrix columns must match vector length for multiplication.");
    }

    NumericVector result(nrow);
    for (int i = 0; i < nrow; i++) {
        double sum = 0.0;
        for (int j = 0; j < ncol; j++) {
            sum += mat(i, j) * vec[j];
        }
        result[i] = sum;
    }

    return result;
}

// Function to calculate correlation
// [[Rcpp::export]]
double corr(NumericVector x, NumericVector y) {
    int n = x.size();

    if (n != y.size()) {
        stop("Vectors must have the same length for correlation calculation.");
    }

    double mean_x = mean(x);
    double mean_y = mean(y);

    double numerator = 0.0;
    double denominator_x = 0.0;
    double denominator_y = 0.0;

    for (int i = 0; i < n; i++) {
        numerator += (x[i] - mean_x) * (y[i] - mean_y);
        denominator_x += pow(x[i] - mean_x, 2);
        denominator_y += pow(y[i] - mean_y, 2);
    }

    double correlation = numerator / sqrt(denominator_x * denominator_y);
    return correlation;
}

// Function to subset rows of a matrix
// [[Rcpp::export]]
NumericMatrix subsetRows(NumericMatrix mat, IntegerVector rows_to_keep) {
    arma::mat mat_arma = as<arma::mat>(mat);
    arma::uvec indices = as<arma::uvec>(rows_to_keep) - 1; // Adjust for 0-based index
    arma::mat result = mat_arma.rows(indices);
    return wrap(result);
}

// Main RandomSE function
// [[Rcpp::export]]
NumericVector RandomSE(NumericMatrix X, NumericVector Y, int subsamp, int B1, int B2) {
    int n = X.nrow(); 
    int p = X.ncol(); 

    NumericVector beta(p);
    
    for (int j = 0; j < B2; ++j) {
        double r_best = -1.0;
        NumericVector beta_best;

        for (int i = 0; i < B1; ++i){
            IntegerVector ind = sampleWithoutReplacement(n, subsamp);
            NumericMatrix X_sub = subsetRows(X, ind);
            NumericVector Y_sub = Y[ind-1];
            NumericVector beta_sub = ols(X_sub, Y_sub);
            NumericVector Y_hat = matrixVectorMultiply(X_sub, beta_sub);
            double r_sub = corr(Y_sub, Y_hat);

            if (r_sub > r_best) {
                r_best = r_sub;
                beta_best = beta_sub;
            }
        }

        beta = beta + beta_best;
    }
    NumericVector beta_rs = beta / B2;
    return beta_rs;
}

