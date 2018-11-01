#ifndef FLF_ALLOCATOR_H
#define FLF_ALLOCATOR_H

#include <climits>
#include <cstdlib>
#include <utility>

#define ALLOC_INIT_CHUNK 1

template <typename T>
struct MemList
{
  MemList(const size_t &size)
    : chunk((T *) malloc(size * sizeof(T))), chunk_size(size), next(nullptr)
  {}

  ~MemList()
  {
    free(chunk);
    if (next != nullptr) delete next;
  }

  T * chunk;
  size_t chunk_size;
  MemList<T> * next;
};

template <typename T>
struct Allocator
{
  Allocator()
    : total_alloc(0), cur_size(0), chunks(nullptr)
  {};

  ~Allocator()
  {
    if (chunks != nullptr) {
      auto l = chunks;
      auto cs = cur_size;
      while (l != nullptr) {
        // call destructors on all the objects already initialized
        T *chunk = l->chunk;

        for (; cs > 0; cs--) {
          chunk[cs-1].~T();
        }

        l = l->next;
        if (l != nullptr) {
          cs = l->chunk_size;
        }
      }

      // free the memory
      delete chunks;
    }
  }

  template <typename... Args>
  T * alloc(Args&&... args)
  {
    if (chunks == nullptr || cur_size == chunks->chunk_size) {
      alloc_new_chunk();
    }

    T *place = &chunks->chunk[cur_size++];
    return new (place) T(std::forward<Args>(args)...);
  }

private:

  MemList<T> * chunks;
  long long total_alloc;
  size_t cur_size;

  void alloc_new_chunk()
  {
    int new_cap = total_alloc > INT_MAX ? INT_MAX : total_alloc == 0 ? ALLOC_INIT_CHUNK : (int) total_alloc;

    auto chunk = new MemList<T>(new_cap);
    chunk->next = chunks;
    chunks = chunk;

    total_alloc += new_cap;
    cur_size = 0;
  }
};

#endif
