// Adapted from readr's Collector.h

#ifndef HIPREAD_COLUMN_H_
#define HIPREAD_COLUMN_H_

#include "boost.h"

#include <Rcpp.h>

class Column;
typedef boost::shared_ptr<Column> ColumnPtr;

class Column {
protected:
  Rcpp::RObject values_;
  int n_;
  std::vector<std::string> failure_values_;
  std::vector<int> failure_rows_;
  int failure_count_;

public:
  Column(SEXP values) :
    values_(values), n_(0), failure_count_(0){}

  virtual void setValue(int i, const char* x_start, const char* x_end) = 0;

  virtual std::string getType() {return "unknown";}

  int size() {
    return n_;
  }

  void resize(int n) {
    if (n == n_)
      return;

    n_ = n;
    values_ = Rf_lengthgets(values_, n);
  }

  static ColumnPtr create(std::string type, Rcpp::List var_opts);

  Rcpp::RObject vector() {
    return values_;
  }

  void add_failure(int line_number, const char* x_start, const char* x_end);

  bool has_failures() {
    return failure_count_ > 0;
  }

  std::string describe_failures(std::string var_name);


};

class ColumnCharacter : public Column {
private:
  bool trim_ws;
public:
  ColumnCharacter(Rcpp::List opts_) : Column(Rcpp::CharacterVector()) {
    trim_ws = opts_["trim_ws"];
  }
  void setValue(int i, const char* x_start, const char* x_end);
  std::string getType() {return "character";}
};

class ColumnDouble : public Column {
private:
  int imp_dec;
public:
  ColumnDouble(Rcpp::List opts_) : Column(Rcpp::DoubleVector()) {
    imp_dec = opts_["imp_dec"];
  }
  void setValue(int i, const char* x_start, const char* x_end);
  std::string getType() {return "double";}
};


class ColumnInteger : public Column {
public:
  ColumnInteger(Rcpp::List opts_) : Column(Rcpp::IntegerVector()) {}
  void setValue(int i, const char* x_start, const char* x_end);
  std::string getType() {return "integer";}
};

std::vector<ColumnPtr> createAllColumns(Rcpp::CharacterVector types, Rcpp::List var_opts);
void resizeAllColumns(std::vector<ColumnPtr>& columns, int n);
void clearAllColumns(std::vector<ColumnPtr>&);
Rcpp::RObject columnsToDf(std::vector<ColumnPtr> columns, Rcpp::CharacterVector names);

#endif
