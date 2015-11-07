#include <cstddef> // for std::ptrdiff_t
#include <limits> // for std::numeric_limits<...>::infinity()
#include <algorithm> // for std::fill_n
#include <stdexcept> // for std::runtime_error
#include <string> // for std::string
#include <math.h> // for std::string

#include "../common.h"

#define fc_isnan(X) ((X)!=(X))

// Microsoft Visual Studio does not have fenv.h
#ifdef _MSC_VER
#if (_MSC_VER == 1500 || _MSC_VER == 1600)
#define NO_INCLUDE_FENV
#endif
#endif
#ifndef NO_INCLUDE_FENV
#include <fenv.h>
#endif

#include <cfloat> // also for DBL_MAX, DBL_MIN
#ifndef DBL_MANT_DIG
#error The constant DBL_MANT_DIG could not be defined.
#endif
#define T_FLOAT_MANT_DIG DBL_MANT_DIG

#ifndef LONG_MAX
#include <climits>
#endif
#ifndef LONG_MAX
#error The constant LONG_MAX could not be defined.
#endif
#ifndef INT_MAX
#error The constant INT_MAX could not be defined.
#endif

#ifndef INT32_MAX
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#endif

#ifndef HAVE_DIAGNOSTIC
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 6))
#define HAVE_DIAGNOSTIC 1
#endif
#endif

#ifndef HAVE_VISIBILITY
#if __GNUC__ >= 4
#define HAVE_VISIBILITY 1
#endif
#endif

/* Since the public interface is given by the Python respectively R interface,
 * we do not want other symbols than the interface initalization routines to be
 * visible in the shared object file. The "visibility" switch is a GCC concept.
 * Hiding symbols keeps the relocation table small and decreases startup time.
 * See http://gcc.gnu.org/wiki/Visibility
 */
#if HAVE_VISIBILITY
#pragma GCC visibility push(hidden)
#endif

typedef int_fast32_t t_index;
#ifndef INT32_MAX
#define MAX_INDEX 0x7fffffffL
#else
#define MAX_INDEX INT32_MAX
#endif
#if (LONG_MAX < MAX_INDEX)
#error The integer format "t_index" must not have a greater range than "long int".
#endif
#if (INT_MAX > MAX_INDEX)
#error The integer format "int" must not have a greater range than "t_index".
#endif
typedef float t_float;

/* Method codes.

   These codes must agree with the METHODS array in fastcluster.R and the
   dictionary mthidx in fastcluster.py.
*/
enum method_codes {
  // non-Euclidean methods
  METHOD_METR_SINGLE           = 0,
  METHOD_METR_COMPLETE         = 1,
  METHOD_METR_AVERAGE          = 2,
  METHOD_METR_WEIGHTED         = 3,
  METHOD_METR_WARD             = 4,
  METHOD_METR_WARD_D           = METHOD_METR_WARD,
  METHOD_METR_CENTROID         = 5,
  METHOD_METR_MEDIAN           = 6,
  METHOD_METR_WARD_D2          = 7,

  MIN_METHOD_CODE              = 0,
  MAX_METHOD_CODE              = 7
};

enum method_codes_vector {
  // Euclidean methods
  METHOD_VECTOR_SINGLE         = 0,
  METHOD_VECTOR_WARD           = 1,
  METHOD_VECTOR_CENTROID       = 2,
  METHOD_VECTOR_MEDIAN         = 3,

  MIN_METHOD_VECTOR_CODE       = 0,
  MAX_METHOD_VECTOR_CODE       = 3
};

enum {
   // Return values
  RET_SUCCESS        = 0,
  RET_MEMORY_ERROR   = 1,
  RET_STL_ERROR      = 2,
  RET_UNKNOWN_ERROR  = 3
 };

// self-destructing array pointer
template <typename type>
class auto_array_ptr{
private:
  type * ptr;
  auto_array_ptr(auto_array_ptr const &); // non construction-copyable
  auto_array_ptr& operator=(auto_array_ptr const &); // non copyable
public:
  auto_array_ptr()
    : ptr(NULL)
  { }
  template <typename index>
  auto_array_ptr(index const size)
    : ptr(new type[size])
  { }
  template <typename index, typename value>
  auto_array_ptr(index const size, value const val)
    : ptr(new type[size])
  {
    std::fill_n(ptr, size, val);
  }
  ~auto_array_ptr() {
    delete [] ptr; }
  void free() {
    delete [] ptr;
    ptr = NULL;
  }
  template <typename index>
  void init(index const size) {
    ptr = new type [size];
  }
  template <typename index, typename value>
  void init(index const size, value const val) {
    init(size);
    std::fill_n(ptr, size, val);
  }
  inline operator type *() const { return ptr; }
};

struct node {
  t_index node1, node2;
  t_float dist;

  /*
  inline bool operator< (const node a) const {
    return this->dist < a.dist;
  }
  */

  inline friend bool operator< (const node a, const node b) {
    return (a.dist < b.dist);
  }
};

class cluster_result {
private:
  auto_array_ptr<node> Z;
  t_index pos;

public:
  cluster_result(const t_index size)
    : Z(size)
    , pos(0)
  {}

  void append(const t_index node1, const t_index node2, const t_float dist) {
    Z[pos].node1 = node1;
    Z[pos].node2 = node2;
    Z[pos].dist  = dist;
    ++pos;
  }

  node * operator[] (const t_index idx) const { return Z + idx; }

  /* Define several methods to postprocess the distances. All these functions
     are monotone, so they do not change the sorted order of distances. */

  void sqrt() const {
    for (node * ZZ=Z; ZZ!=Z+pos; ++ZZ) {
      ZZ->dist = ::sqrt(ZZ->dist);
    }
  }

  void sqrt(const t_float) const { // ignore the argument
    sqrt();
  }

  void sqrtdouble(const t_float) const { // ignore the argument
    for (node * ZZ=Z; ZZ!=Z+pos; ++ZZ) {
      ZZ->dist = ::sqrt(2*ZZ->dist);
    }
  }

  #ifdef R_pow
  #define my_pow R_pow
  #else
  #define my_pow pow
  #endif

  void power(const t_float p) const {
    t_float const q = 1/p;
    for (node * ZZ=Z; ZZ!=Z+pos; ++ZZ) {
      ZZ->dist = my_pow(ZZ->dist,q);
    }
  }

  void plusone(const t_float) const { // ignore the argument
    for (node * ZZ=Z; ZZ!=Z+pos; ++ZZ) {
      ZZ->dist += 1;
    }
  }

  void divide(const t_float denom) const {
    for (node * ZZ=Z; ZZ!=Z+pos; ++ZZ) {
      ZZ->dist /= denom;
    }
  }
};

/*
  Lookup function for a union-find data structure.

  The function finds the root of idx by going iteratively through all
  parent elements until a root is found. An element i is a root if
  nodes[i] is zero. To make subsequent searches faster, the entry for
  idx and all its parents is updated with the root element.
 */
class union_find {
private:
  auto_array_ptr<t_index> parent;
  t_index nextparent;

public:
  union_find(const t_index size)
    : parent(size>0 ? 2*size-1 : 0, 0)
    , nextparent(size)
  { }

  t_index Find (t_index idx) const {
    if (parent[idx] != 0 ) { // a → b
      t_index p = idx;
      idx = parent[idx];
      if (parent[idx] != 0 ) { // a → b → c
        do {
          idx = parent[idx];
        } while (parent[idx] != 0);
        do {
          t_index tmp = parent[p];
          parent[p] = idx;
          p = tmp;
        } while (parent[p] != idx);
      }
    }
    return idx;
  }

  t_index Union (const t_index node1, const t_index node2) {
    parent[node1] = parent[node2] = nextparent++;
    return parent[node1];
  }
};

void MST_linkage_core(const t_index N, const t_float * const D,
                             cluster_result & Z2);
