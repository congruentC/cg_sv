// cg_sv.h

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
// Passing a NULL pointer is not a valid input.
cgsv cgsv_from_cstr(const char *s);


// --------------- PREDICATES -----------------

// Returns 1 if a and b contain the same bytes,
// 0 otherwise.
int cgsv_eq(cgsv a, cgsv b);

// Check if sv starts or ends with another view.
// Returns 1 if so, 0 otherwise.
// NULL pointer handling left to caller's responsibility.
int cgsv_starts_with(cgsv sv, cgsv prefix);
int cgsv_ends_with(cgsv sv, cgsv suffix);


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
// first occurrence or -1 if not found.
ptrdiff_t cgsv_find_c(cgsv sv, char c);

// Check if sv contains another view.
// Return 1 if found, 0 otherwise.
int cgsv_contains(cgsv sv, cgsv needle);


// --------------- SPLITTING -----------------

// Split on first occurrence of a delimiter
// puts left side in "out", returns remainder.
// if delimiter is not found, put entire sv into out
// and return empty sv.
cgsv cgsv_split(cgsv sv, char delim, cgsv *out);


// -------------------------------------------

#include <string.h>
#include <ctype.h>

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
    if (memcmp(a.ptr, b.ptr, a.len) == 0){
        return 1;
    }
    return 0;
}

int cgsv_starts_with(cgsv sv, cgsv prefix){
    if (sv.len < prefix.len){
        return 0;
    }

    return memcmp(sv.ptr, prefix.ptr, prefix.len) == 0;
}

int cgsv_ends_with(cgsv sv, cgsv suffix){
    if (sv.len < suffix.len){
        return 0;
    }
    return memcmp(sv.ptr + (sv.len - suffix.len), suffix.ptr, suffix.len) == 0;
}

cgsv cgsv_trim_left(cgsv sv){
    while (sv.len > 0 && isspace((unsigned char)*sv.ptr)){
       sv.ptr++;
       sv.len--;
    }
    return sv;
}

cgsv cgsv_trim_right(cgsv sv){
    while (sv.len > 0 && isspace((unsigned char)sv.ptr[sv.len -1])){
        sv.len--;
    }
    return sv;
}

cgsv cgsv_trim(cgsv sv){
    sv = cgsv_trim_right(sv);
    sv = cgsv_trim_left(sv);
    return sv;
}

cgsv cgsv_take(cgsv sv, size_t n){
    if (n > sv.len){
        n = sv.len;
    }
    sv.len = n;
    return sv;
}

cgsv cgsv_take_right(cgsv sv, size_t n){
    if (n > sv.len){
        n = sv.len;
    }
    size_t amount = sv.len - n;
    sv.ptr += amount;
    sv.len -= amount;
    return sv;
}

cgsv cgsv_chop(cgsv sv, size_t n){
    if (n > sv.len){
        n = sv.len;
    }
    sv = cgsv_take_right(sv, sv.len - n);
    return sv;
}

cgsv cgsv_chop_right(cgsv sv, size_t n){
    if (n > sv.len){
        n = sv.len;
    }
    sv = cgsv_take(sv, sv.len - n);
    return sv;
}

ptrdiff_t cgsv_find_c(cgsv sv, char c){
    size_t i = 0;
    while (i < sv.len){
        if (sv.ptr[i] == c){
           return i;
        }
        i++;
    }
    return -1;
}


int cgsv_contains(cgsv sv, cgsv needle){
    if (needle.len > sv.len || needle.len == 0){
        return 0;
    }
    size_t limit = sv.len - needle.len;
    for (size_t i = 0; i <= limit; i++){
        if (memcmp(sv.ptr + i, needle.ptr, needle.len) == 0){
            return 1;
        }
    }
    return 0;
}

// --------------- SPLITTING -----------------

// Split on first occurrence of a delimiter
// puts left side in "out", returns remainder
cgsv cgsv_split(cgsv sv, char delim, cgsv *out){
    ptrdiff_t offset = 0;
    offset = cgsv_find_c(sv, delim);
    if (offset < 0){
        *out = sv;
        sv.len = 0;
        return sv;
    }
    *out = cgsv_take(sv, (size_t)offset);
    sv = cgsv_chop(sv, offset + 1);
    return sv;
}


#endif // CGSV_IMPLEMENTATION
#endif // CGSV_H
