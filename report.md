Link-cut tree (Coursework report)
===

Advanced Data Structures course, ITMO University, Computer Technologies department
Author: Dmitrii Mukhutdinov

## Abstract

Link-cut tree is a data structure for representing a _forest_ which allows for
efficient (`O(log n)` amortized) operations of _linking_ (attaching a tree to a
given node), _cutting_ (disconnecting a given subtree from its parent) and
_finding the root_ of the tree a given node belongs to.

Additionally, link-cut tree supports operations of adding a constant value `c`
to all the nodes on path from root to a given node `u`, and also an operation of
finding a minimum on path from root to a given node `u`. Those operations also
have `O(log n)` complexity.

This implementation of link-cut tree uses a custom implementation of splay tree
as an underlying data structure. The implementation is written in C++.

## Theory

The key idea behind link-cut tree is splitting a tree into a set of disjoint
paths. Each path is stored as a splay tree with implicit keys. Edges which are
not included into any individual path are represented via separate `_pathparent`
links, stored in the roots of splay trees which represent paths.

### Splay trees, operations `add(v, c)` and `min(v)`

[Splay trees](https://en.wikipedia.org/wiki/Splay_tree) are self-balanced
binary search trees which support operations `split` and `merge`. The key
feature of a splay trees is the fact that every access to a given node in a tree
results in this node becoming a root of the tree. The node is moved to a root
with an operation called _splaying_, which use particular sequences of tree
rotations in order to achieve `O(log n)` amortized bound on the height of the
tree.

With splay trees, operations `split` and `merge` are trivial:

- `split(v)`: splay `v` to the root and cut off one of its children
- `merge(v, u)`: find the minimal node in `u` subtree, splay it to the top,
  attach `v` subtree as its left child.

#### Modification 1: implicit keys

Splay trees are commonly used as key-value maps, where nodes contain pairs
`(key, value)` and are ordered in the tree according to `key`s. However, splay
trees can also be used as sequence containers (like arrays, but with efficient
`split` and `merge` operations). In order to achieve this, in each node we store
_sizes_ of corresponding subtrees. Locating an element with index `i` is then
done as follows:

- If the size of _left_ child of current subtree is less than `i`, find node
  with index `i` in the left child.
- If the size of _left_ child of current subtree is equal to `i`, then current
  node is the node we're looking for.
- Otherwise, find node with index `i - l - 1` in the right child, where `l` is
  the size of left subtree.

Sizes should be updated during tree rotations, but this is trivial.

#### Modification 2: support for `add(v, c)` and `min(v)`

We want to efficiently support the following operations:

- `add(v, c)` - add the value `c` to the prefix of a sequence up to `v`
- `min(v)` - find the minimum on the prefix up to `v`

In order to do that, we introduce the following modifications:

- Instead of storing actual values `w(u)` in nodes, we store `Δw(u) = w(u) -
  w(parent(u))`. If `u` is the root, `Δw(u) = w(u)`. Therefore, the actual value
  `w(u)` can be calculated as the sum of `Δw(v)` values for all ancestors of
  `u`, including `u` itself.
- We will support `min(u)` values for subtrees, but not directly. Instead, we
  will store `Δmin(u) = min(u) - w(u) = min {0, Δmin(l) + Δw(l), Δmin(r) +
  Δw(r)}`. This value can be easily updated during rotations, and in combination
  with `Δw` values gives us actual minumums for subtrees. Therefore, minumum on
  prefix is then calculated by splaying given node `v` and comparing minumum on
  its left subtree (`min(l) = Δmin(l) + Δw(l)`) with `w(v) = Δw(v)`.
  
### Making a link-cut tree using splay trees.

The key operation in a link-cut tree is `expose(v)`, which reorganizes the tree
in such a way that the node `v` lies on the _primary path_ (a disjoint path
which contains the root of link-cut tree), and also becomes a root in splay tree
representing the primary path. `expose(v)` operation is done by following the
`_pathparent` links on path to the root and merging paths connected by those
links.

When the `expose(v)` operation is implemented, operations `add(v, c)` and
`min(v)` on paths from root are trivial: perform `expose(v)` and use `add(v, c)`
and `min(v)` operations on a splay tree representing the primary path. `link(u,
v)` is also trivial: set `pathparent(v) = u` and `expose(v)`. `cut(v)` is then
just `expose(v)` and `split(v)`.

## Implementation

### Splay node

Files [splay_node.h](./headers/splay_node.h) and
[splay_node.cpp](./src/splay_node.cpp) contain the definition of `SplayNode`
structure as well as basic operations: `splay`, `split`, `merge`, `find`, `min`,
`add`.

### Allocator

In order to avoid performance issues and segmentation faults associated with
large amounts of memory allocations, a simple custom allocator has been
implemented ([allocator.h](./headers/allocator.h)). The allocator allocates
increasingly large chunks of memory and calls object constructors on demand. The
allocator does not perform runtime deallocations; all memory is deallocated at
once in allocator's destructor.

### Splay tree

A standalone implementation of a splay tree
([splay_tree.h](./headers/splay_tree.h),
[splay_tree.cpp](./headers/splay_tree.cpp)) is written in order to test the
behavior of basic splay tree functions separately. A corresponding executable
([splay_test.cpp](splay_test.cpp)) reads the initial array and the list of
operations (`find`, `increase`, `min`) from stdin, and writes results of
operations to stdout. A Python script
[test_splay_gen.py](./tests/test_splay_gen.py) randomly generates the program
input and correct output using naive array implementation. Script usage:

```
python test_splay_gen.py <test_case> <n> <m> [inc_min]
```

This will create a file `<test_case>.in` which defines an array of size `n` with
`m` operations on it, and a corresponding file `<test_case>.out` with a correct
output. If optional parameter `inc_min` is not provided, the input file will
only contain `find` operations.

### Link-cut tree

An implementation of a link-cut tree
([link_cut_tree.h](./headers/link_cut_tree.h),
[link_cut_tree.cpp](./headers/link_cut_tree.cpp)) depends on splay node
implementation and allocator. A corresponding executable
([link_cut_test.cpp](link_cut_test.cpp)) reads the number of nodes, their
initial values and the list of operations (`get`, `add`, `min`, `link`, `cut`,
`lca`) on them from stdin, and writes results of operations to stdout. A Python
script [test_lc_gen.py](./tests/test_lc_gen.py) randomly generates the program
input and correct output using naive forest implementation. Script usage:

```
python test_lc_gen.py <test_case> <n> <m>
```

This will create a file `<test_case>.in` which defines `n` node values with
`m` operations on the forest, and a corresponding file `<test_case>.out` with a correct
output.

## Building the project

Project uses Cmake. You need to have `cmake` and `make` installed. Also, some
features of C++17 (i. e. `optional`) are used, so `gcc >= 7.x.x` is required.

```
cmake .
make
```

## Performance

```
> cd tests
> python test_lc_gen.py test_lc_max 1000000 1000000
... about an hour passes ...
> wc -l test_lc_max.in
2000002 test_lc_max.in
> cd ..
> cat tests/test_lc_max.in | time ./link_cut_test > result.out
./link_cut_test > result.out  1.32s user  0.05s system  99% cpu  1.374 total
> diff result.out tests/test_lc_max.out


```

I. e. a million random operations on a forest containing a million nodes are
performed in less than 2 seconds.
