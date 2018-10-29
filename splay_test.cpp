#include <cstdio>
#include <cstring>
#include "headers/splay_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  bool debug = false;
  if (argc > 1 && strcmp(argv[1], "debug") == 0) {
    printf("Debug mode\n");
    debug = true;
  }

  int n;
  int k;
  long long v;
  char cmd[15];
  SplayTree tree = {};
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%14s", cmd);
    if (strcmp(cmd, "add") == 0) {
      scanf("%d %lld", &k, &v);
      tree.add(k, v);
      printf("added\n");
    } else if (strcmp(cmd, "remove") == 0) {
      scanf("%d", &k);
      tree.remove(k);
    } else if (strcmp(cmd, "find") == 0) {
      scanf("%d", &k);
      auto mval = tree.find(k);
      try {
        printf("%lld\n", mval.value());
      } catch (const nothing &e) {
        printf("None\n");
      }
    } else if (strcmp(cmd, "increase") == 0) {
      scanf("%d %lld", &k, &v);
      tree.increase(k, v);
      printf("increased\n");
    } else if (strcmp(cmd, "min") == 0) {
        scanf("%d", &k);
      auto mval = tree.min(k);
      try {
        printf("%lld\n", mval.value());
      } catch (const nothing &e) {
        printf("None\n");
      }
    }
    if (debug) {
      printf("%s\n", tree.show().c_str());
    }
  }
}
