// Adapted from readr's Collector.cpp

#include <Rcpp.h>
using namespace Rcpp;
#include "column.h"
#include "string_utils.h"

#include <Rcpp.h>
using namespace Rcpp;

ColumnPtr Column::create(std::string type, Rcpp::List var_opts) {
  if (type == "character") {
    return ColumnPtr(new ColumnCharacter(var_opts));
  } else if (type == "double") {
    return ColumnPtr(new ColumnDouble(var_opts));
  } else if (type == "integer") {
    return ColumnPtr(new ColumnInteger(var_opts));
  }

  Rcpp::stop("Unexpected column type '%s'", type);
}


void Column::add_failure(int line_number, const char* x_start, const char* x_end) {
  if (++failure_count_ <= 5) {
    std::string value;
    value.assign(x_start, x_end - x_start);
    failure_values_.push_back(value);
    failure_rows_.push_back(line_number + 1);
  }
}

std::string Column::describe_failures(std::string var_name) {
  std::ostringstream message;

  message << "In variable '" << var_name << "', could not convert " << failure_count_ <<
    " values to " << getType() << "; Values (and row numbers) of first " <<
      failure_rows_.size() << " failures: ";

  int num_failures = failure_rows_.size();
  for (int i = 0; i < num_failures; ++i) {
    if (i > 0) {
      message << ", ";
    }

    message << "'" << failure_values_[i] << "' (" <<
      failure_rows_[i] << ")";
  }

  return message.str();
}


void ColumnCharacter::setValue(int i, const char* x_start, const char* x_end) {
  // TODO: How would encoding affect this?
  if (trim_ws) IpStringUtils::newtrim(x_start, x_end);
  SET_STRING_ELT(values_, i, Rf_mkCharLen(x_start, x_end - x_start));
}

void ColumnDouble::setValue(int i, const char* x_start, const char* x_end) {
  long double value;
  IpStringUtils::newtrim(x_start, x_end);
  bool success;
  if (x_start == x_end) {
    success = true;
    value = NA_REAL;
  } else {
    success = parseDouble(x_start, x_end, value);
  }

  if (!success) {
    add_failure(i, x_start, x_end);
    value = NA_REAL;
  } else {
    value = value / pow(10, imp_dec);
  }
  REAL(values_)[i] = value;
}

void ColumnInteger::setValue(int i, const char* x_start, const char* x_end) {
  long int value;
  IpStringUtils::newtrim(x_start, x_end);
  bool success;
  if (x_start == x_end) {
    success = true;
    value = NA_INTEGER;
  } else {
    success = parseInteger(x_start, x_end, value);
  }

  if (!success) {
    add_failure(i, x_start, x_end);
    value = NA_INTEGER;
  }
  INTEGER(values_)[i] = value;
}



std::vector<ColumnPtr> createAllColumns(CharacterVector types, Rcpp::List var_opts) {
  int num_cols = types.size();
  std::vector<ColumnPtr> out;

  for (int i = 0; i < num_cols; ++i) {
    out.push_back(Column::create(as<std::string>(types[i]), var_opts[i]));
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
  for (int i = 0; i < num_vars; ++i) {
    if (columns[i]->has_failures()) {
      std::string message = columns[i]->describe_failures(Rcpp::as<std::string>(names[i]));
      Rf_warning(message.c_str());
    }
    out[i] = columns[i]->vector();
  }
  out.attr("names") = names;
  return as_tibble(out);
}
