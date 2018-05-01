// Adapted from readr's Collector.cpp

#include <Rcpp.h>
using namespace Rcpp;
#include "column.h"
#include "string_utils.h"

#include <Rcpp.h>
using namespace Rcpp;

ColumnPtr Column::create(std::string type) {
  if (type == "character") {
    return ColumnPtr(new ColumnCharacter());
  } else if (type == "double") {
    return ColumnPtr(new ColumnDouble());
  } else if (type == "integer") {
    return ColumnPtr(new ColumnInteger());
  }

  Rcpp::stop("Unexpected column type '%s'", type);
}


void Column::add_failure(int line_number, std::string value) {
  if (++failure_count_ <= 5) {
    failure_values_.push_back(value);
    failure_rows_.push_back(line_number + 1);
  }
}

std::string Column::describe_failures(std::string var_name) {
  std::ostringstream message;

  message << "In variable '" << var_name << "', could not convert " << failure_count_ <<
    " values to " << getType() << "; Values (and row numbers) of first " <<
      failure_rows_.size() << " failures: ";

  for (int i = 0; i < failure_rows_.size(); ++i) {
    if (i > 0) {
      message << ", ";
    }

    message << "'" << failure_values_[i] << "' (" <<
      failure_rows_[i] << ")";
  }

  return message.str();
}


void ColumnCharacter::setValue(int i, std::string x) {
  // TODO: How would encoding affect this?
  IpStringUtils::trim(x);
  SET_STRING_ELT(values_, i, Rf_mkChar(x.c_str()));
}

void ColumnDouble::setValue(int i, std::string x) {
  // TODO: Implicit decimals
  long double value;
  IpStringUtils::trim(x);
  const char* start = x.c_str();
  const char* end = start + x.size();
  bool success = parseDouble(start, end, value);

  if (!success) {
    add_failure(i, x);
    value = NA_REAL;
  }
  REAL(values_)[i] = value;
}

void ColumnInteger::setValue(int i, std::string x) {
  long int value;
  IpStringUtils::trim(x);
  const char* start = x.c_str();
  const char* end = start + x.size();
  bool success = parseInteger(start, end, value);

  if (!success) {
    add_failure(i, x);
    value = NA_INTEGER;
  }
  INTEGER(values_)[i] = value;
}



std::vector<ColumnPtr> createAllColumns(CharacterVector types) {
  int num_cols = types.size();
  std::vector<ColumnPtr> out;

  for (int i = 0; i < num_cols; ++i) {
    out.push_back(Column::create(as<std::string>(types[i])));
  }

  return out;
}

void resizeAllColumns(std::vector<ColumnPtr>& columns, int n) {
  int num_cols = columns.size();

  for (int i = 0; i < num_cols; ++i) {
    columns[i]->resize(n);
  }
}

void clearAllColumns(std::vector<ColumnPtr>& columns, int n) {
  int num_cols = columns.size();

  for (int i = 0; i < num_cols; ++i) {
    columns[i]->resize(0);
  }
}


static Function as_tibble("as_tibble", Environment::namespace_env("tibble"));

RObject columnsToDf(std::vector<ColumnPtr> columns, Rcpp::CharacterVector names) {
  int num_vars = columns.size();
  List out(num_vars);
  for (int i; i < num_vars; ++i) {
    if (columns[i]->has_failures()) {
      std::string message = columns[i]->describe_failures(Rcpp::as<std::string>(names[i]));
      Rf_warning(message.c_str());
    }
    out[i] = columns[i]->vector();
  }
  out.attr("names") = names;
  return as_tibble(out);
}
