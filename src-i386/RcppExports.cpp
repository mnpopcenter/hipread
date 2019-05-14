// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "hipread_types.h"
#include <Rcpp.h>

using namespace Rcpp;

// read_chunked_long
void read_chunked_long(CharacterVector filename, Environment callback, int chunksize, CharacterVector var_names, CharacterVector var_types, List rt_info_, List var_pos_info_, List var_opts_, int skip, bool isGzipped, CharacterVector encoding, bool progress);
RcppExport SEXP _hipread_read_chunked_long(SEXP filenameSEXP, SEXP callbackSEXP, SEXP chunksizeSEXP, SEXP var_namesSEXP, SEXP var_typesSEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP skipSEXP, SEXP isGzippedSEXP, SEXP encodingSEXP, SEXP progressSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< Environment >::type callback(callbackSEXP);
    Rcpp::traits::input_parameter< int >::type chunksize(chunksizeSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_names(var_namesSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_types(var_typesSEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    Rcpp::traits::input_parameter< bool >::type progress(progressSEXP);
    read_chunked_long(filename, callback, chunksize, var_names, var_types, rt_info_, var_pos_info_, var_opts_, skip, isGzipped, encoding, progress);
    return R_NilValue;
END_RCPP
}
// read_chunked_list
void read_chunked_list(CharacterVector filename, Environment callback, int chunksize, List var_names_, List var_types_, List rt_info_, List var_pos_info_, List var_opts_, int skip, bool isGzipped, CharacterVector encoding, bool progress);
RcppExport SEXP _hipread_read_chunked_list(SEXP filenameSEXP, SEXP callbackSEXP, SEXP chunksizeSEXP, SEXP var_names_SEXP, SEXP var_types_SEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP skipSEXP, SEXP isGzippedSEXP, SEXP encodingSEXP, SEXP progressSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< Environment >::type callback(callbackSEXP);
    Rcpp::traits::input_parameter< int >::type chunksize(chunksizeSEXP);
    Rcpp::traits::input_parameter< List >::type var_names_(var_names_SEXP);
    Rcpp::traits::input_parameter< List >::type var_types_(var_types_SEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    Rcpp::traits::input_parameter< bool >::type progress(progressSEXP);
    read_chunked_list(filename, callback, chunksize, var_names_, var_types_, rt_info_, var_pos_info_, var_opts_, skip, isGzipped, encoding, progress);
    return R_NilValue;
END_RCPP
}
// read_freqs
RObject read_freqs(CharacterVector filename, CharacterVector var_names, List rt_info_, List var_pos_info_, bool isGzipped, bool progress);
RcppExport SEXP _hipread_read_freqs(SEXP filenameSEXP, SEXP var_namesSEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP isGzippedSEXP, SEXP progressSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_names(var_namesSEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< bool >::type progress(progressSEXP);
    rcpp_result_gen = Rcpp::wrap(read_freqs(filename, var_names, rt_info_, var_pos_info_, isGzipped, progress));
    return rcpp_result_gen;
END_RCPP
}
// read_long
RObject read_long(CharacterVector filename, CharacterVector var_names, CharacterVector var_types, List rt_info_, List var_pos_info_, List var_opts_, int skip, int n_max, bool isGzipped, CharacterVector encoding, bool progress);
RcppExport SEXP _hipread_read_long(SEXP filenameSEXP, SEXP var_namesSEXP, SEXP var_typesSEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP skipSEXP, SEXP n_maxSEXP, SEXP isGzippedSEXP, SEXP encodingSEXP, SEXP progressSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_names(var_namesSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_types(var_typesSEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    Rcpp::traits::input_parameter< int >::type n_max(n_maxSEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    Rcpp::traits::input_parameter< bool >::type progress(progressSEXP);
    rcpp_result_gen = Rcpp::wrap(read_long(filename, var_names, var_types, rt_info_, var_pos_info_, var_opts_, skip, n_max, isGzipped, encoding, progress));
    return rcpp_result_gen;
END_RCPP
}
// read_list
RObject read_list(CharacterVector filename, List var_names_, List var_types_, List rt_info_, List var_pos_info_, List var_opts_, int skip, int n_max, bool isGzipped, CharacterVector encoding, bool progress);
RcppExport SEXP _hipread_read_list(SEXP filenameSEXP, SEXP var_names_SEXP, SEXP var_types_SEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP skipSEXP, SEXP n_maxSEXP, SEXP isGzippedSEXP, SEXP encodingSEXP, SEXP progressSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< List >::type var_names_(var_names_SEXP);
    Rcpp::traits::input_parameter< List >::type var_types_(var_types_SEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    Rcpp::traits::input_parameter< int >::type n_max(n_maxSEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    Rcpp::traits::input_parameter< bool >::type progress(progressSEXP);
    rcpp_result_gen = Rcpp::wrap(read_list(filename, var_names_, var_types_, rt_info_, var_pos_info_, var_opts_, skip, n_max, isGzipped, encoding, progress));
    return rcpp_result_gen;
END_RCPP
}
// start_yield
XPtrDataSource start_yield(CharacterVector filename, bool isGzipped, int skip);
RcppExport SEXP _hipread_start_yield(SEXP filenameSEXP, SEXP isGzippedSEXP, SEXP skipSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< bool >::type isGzipped(isGzippedSEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    rcpp_result_gen = Rcpp::wrap(start_yield(filename, isGzipped, skip));
    return rcpp_result_gen;
END_RCPP
}
// reset_yield
void reset_yield(XPtrDataSource data, int skip);
RcppExport SEXP _hipread_reset_yield(SEXP dataSEXP, SEXP skipSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrDataSource >::type data(dataSEXP);
    Rcpp::traits::input_parameter< int >::type skip(skipSEXP);
    reset_yield(data, skip);
    return R_NilValue;
END_RCPP
}
// yield_is_done
bool yield_is_done(XPtrDataSource data);
RcppExport SEXP _hipread_yield_is_done(SEXP dataSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrDataSource >::type data(dataSEXP);
    rcpp_result_gen = Rcpp::wrap(yield_is_done(data));
    return rcpp_result_gen;
END_RCPP
}
// next_yield_long
RObject next_yield_long(XPtrDataSource data, CharacterVector var_names, CharacterVector var_types, List rt_info_, List var_pos_info_, List var_opts_, int yield_size, CharacterVector encoding);
RcppExport SEXP _hipread_next_yield_long(SEXP dataSEXP, SEXP var_namesSEXP, SEXP var_typesSEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP yield_sizeSEXP, SEXP encodingSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrDataSource >::type data(dataSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_names(var_namesSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type var_types(var_typesSEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type yield_size(yield_sizeSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    rcpp_result_gen = Rcpp::wrap(next_yield_long(data, var_names, var_types, rt_info_, var_pos_info_, var_opts_, yield_size, encoding));
    return rcpp_result_gen;
END_RCPP
}
// next_yield_list
RObject next_yield_list(XPtrDataSource data, List var_names_, List var_types_, List rt_info_, List var_pos_info_, List var_opts_, int yield_size, CharacterVector encoding);
RcppExport SEXP _hipread_next_yield_list(SEXP dataSEXP, SEXP var_names_SEXP, SEXP var_types_SEXP, SEXP rt_info_SEXP, SEXP var_pos_info_SEXP, SEXP var_opts_SEXP, SEXP yield_sizeSEXP, SEXP encodingSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrDataSource >::type data(dataSEXP);
    Rcpp::traits::input_parameter< List >::type var_names_(var_names_SEXP);
    Rcpp::traits::input_parameter< List >::type var_types_(var_types_SEXP);
    Rcpp::traits::input_parameter< List >::type rt_info_(rt_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_pos_info_(var_pos_info_SEXP);
    Rcpp::traits::input_parameter< List >::type var_opts_(var_opts_SEXP);
    Rcpp::traits::input_parameter< int >::type yield_size(yield_sizeSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type encoding(encodingSEXP);
    rcpp_result_gen = Rcpp::wrap(next_yield_list(data, var_names_, var_types_, rt_info_, var_pos_info_, var_opts_, yield_size, encoding));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_hipread_read_chunked_long", (DL_FUNC) &_hipread_read_chunked_long, 12},
    {"_hipread_read_chunked_list", (DL_FUNC) &_hipread_read_chunked_list, 12},
    {"_hipread_read_freqs", (DL_FUNC) &_hipread_read_freqs, 6},
    {"_hipread_read_long", (DL_FUNC) &_hipread_read_long, 11},
    {"_hipread_read_list", (DL_FUNC) &_hipread_read_list, 11},
    {"_hipread_start_yield", (DL_FUNC) &_hipread_start_yield, 3},
    {"_hipread_reset_yield", (DL_FUNC) &_hipread_reset_yield, 2},
    {"_hipread_yield_is_done", (DL_FUNC) &_hipread_yield_is_done, 1},
    {"_hipread_next_yield_long", (DL_FUNC) &_hipread_next_yield_long, 8},
    {"_hipread_next_yield_list", (DL_FUNC) &_hipread_next_yield_list, 8},
    {NULL, NULL, 0}
};

RcppExport void R_init_hipread(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}