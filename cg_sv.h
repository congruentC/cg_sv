// cgsv.h

#ifndef cgsv_H
#define cgsv_H

#include <stddef.h>
typedef struct {
    const char *ptr;
    size_t len;
} cgsv;

#ifdef CGSV_IMPLEMENTATION

// --------------- PRINTING ------------------

// Use with printf as: printf("%.*s", CGSV_FMT(sv));
#define CGSV_FMT(sv) (int)(sv).len, (sv).ptr


// --------------- CONSTRUCTION ---------------

// Returns a view into s. Does not copy or allocate.
// s must outlive the returned view.
cgsv cgsv_from_cstr(const char *s);


// --------------- PREDICATES -----------------

// Returns 1 if a and b contain the same bytes,
// 0 otherwise.
int cgsv_eq(cgsv a, cgsv b);

// Check if sv starts or ends with another view
int cgsv_starts_with(cgsv sv, cgsv prefix);
int cgsv_ends_with(cgsv sv, cgsv prefix);


// --------------- TRIMMING ------------------

// Trim whitespace from either end
cgsv cgsv_trim_left(cgsv sv);
cgsv cgsv_trim_right(cgsv sv);
cgsv cgsv_trim(cgsv sv);


// --------------- SLICING -------------------

// Take the first n or last n characters
cgsv cgsv_take(cgsv sv, size_t n);
cgsv cgsv_take_right(cgsv sv, size_t n);

// Chop n characters off the left or right
cgsv cgsv_chop(cgsv sv, size_t n);
cgsv cgsv_chop_right(cgsv sv, size_t n);


// --------------- SEARCHING -----------------

// Find a character, returns index of
// first occurence or -1 if not found.
ptrdiff_t cgsv_find_c(cgsv sv, char c);

// Check if sv contains another view
int cgsv_contains(cgsv sv, cgsv needle);


// --------------- SPLITTING -----------------

// Split on first occurrence of a delimiter
// puts left side in "out", returns remainder
cgsv cgsv_split(cgsv sv, char delim, cgsv *out);


// -------------------------------------------

#include <string.h>

cgsv cgsv_from_cstr(const char *s){
    cgsv sv_string;
    sv_string.ptr = s;
    sv_string.len = strlen(s);
    return sv_string;
}

int cgsv_eq(cgsv a, cgsv b){
    if (a.len != b.len){
        return 0;
    }
    if (strcmp(a.ptr, b.ptr) == 0){
        return 1;
    }
    return 0;
}

#endif // CGSV_IMPLEMENTATION
#endif // CGSV_H
