#ifndef FLF_ALLOCATOR_H
#define FLF_ALLOCATOR_H

#include <climits>

#define ALLOC_INIT_CHUNK 1

template <typename T>
struct MemList
{
  MemList(const int &size)
    : chunk(new T[size]),  next(nullptr)
  {}

  ~MemList()
  {
    delete[] chunk;
    if (next != nullptr) delete next;
  }

  T * chunk;
  MemList<T> * next;
};

template <typename T>
struct Allocator
{
  Allocator()
    : total_alloc(0), cur_cap(0), cur_size(0), chunks(nullptr)
  {};

  ~Allocator()
  {
    if (chunks != nullptr) delete chunks;
  }

  T * alloc()
  {
    if (chunks == nullptr || cur_size == cur_cap) {
      alloc_new_chunk();
    }
    return &chunks->chunk[cur_size++];
  }

private:

  MemList<T> * chunks;
  long long total_alloc;
  int cur_cap;
  int cur_size;

  void alloc_new_chunk()
  {
    int new_cap = total_alloc > INT_MAX ? INT_MAX : total_alloc == 0 ? ALLOC_INIT_CHUNK : (int) total_alloc;

    auto chunk = new MemList<T>(new_cap);
    chunk->next = chunks;
    chunks = chunk;

    total_alloc += new_cap;
    cur_cap = new_cap;
    cur_size = 0;
  }
};

#endif
