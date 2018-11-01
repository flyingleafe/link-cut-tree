#include <cstdio>
#include <cstring>
#include "headers/link_cut_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  int n, m;
  int u, v;
  long long c;
  char cmd[15];
  LinkCutTree tree;

  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%lld", &c);
    tree.newNode(c);
  }

  scanf("%d", &m);
  for (int i = 0; i < m; i++) {
    scanf("%14s", &cmd);
    if (strcmp(cmd, "get") == 0) {
      scanf("%d", &u);
      auto mv = tree.get(u);
      try {
        printf("%lld\n", mv.value());
      } catch (const nothing &e) {
        printf("None\n");
      }
    } else if (strcmp(cmd, "add") == 0) {
      scanf("%d %lld", &u, &c);
      tree.addLC(u, c);
      printf("added\n");
    } else if (strcmp(cmd, "min") == 0) {
      scanf("%d", &u);
      auto mv = tree.minLC(u);
      try {
        printf("%lld\n", mv.value());
      } catch (const nothing &e) {
        printf("None\n");
      }
    } else if (strcmp(cmd, "link") == 0) {
      scanf("%d %d", &u, &v);
      tree.link(u, v);
      printf("linked\n");
    } else if (strcmp(cmd, "cut") == 0) {
      scanf("%d", &u);
      tree.cut(u);
      printf("cut\n");
    } else if (strcmp(cmd, "lca") == 0) {
      scanf("%d %d", &u, &v);
      auto mv = tree.lca(u, v);
      try {
        printf("%d\n", mv.value());
      } catch (const nothing &e) {
        printf("None\n");
      }
    }
  }
}
