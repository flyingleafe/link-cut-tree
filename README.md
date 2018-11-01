Link-cut tree implementation
===

[Link-cut tree](https://en.wikipedia.org/wiki/Link/cut_tree) implementation
which supports basic operations (adding a vertex, link, cut) as well as the
following extra operations:

- `add(v, c)` - increases the values of vertex `v` and all its ancestors by the
  value `c`
- `min(v)` - finds the minimum value of vertices among `v` and all its ancestors

Uses a [splay tree](https://en.wikipedia.org/wiki/Splay_tree) with implicit keys
as an underlying data structure. Used splay tree implementation supports basic
operations (lookup, split, merge) as well as the following extra operations:

- `increase(from, to, c)` - increases the values of all elements in range
  `[from, to]` by the value `c`
- `min(k)` - finds the minimum in range `[0, k]`

Implementation is based on [this
explanation](http://neerc.ifmo.ru/wiki/index.php?title=Link-Cut_Tree).
