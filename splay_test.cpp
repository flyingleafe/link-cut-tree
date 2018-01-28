#include <iostream>
#include <string>
#include <cstring>
#include "headers/splay_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  bool debug = false;
  if (argc > 1 && strcmp(argv[1], "debug") == 0) {
    cout << "Debug mode\n";
    debug = true;
  }

  int n;
  int k;
  long long v;
  string cmd;
  SplayTree tree = {};
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> cmd;
    if (cmd.compare("add") == 0) {
      cin >> k >> v;
      tree.add(k, v);
      cout << "added\n";
    } else if (cmd.compare("remove") == 0) {
      cin >> k;
      tree.remove(k);
    } else if (cmd.compare("find") == 0) {
      cin >> k;
      int val = tree.find(k).value_or(-1);
      if (val != -1) {
        cout << val << endl;
      } else {
        cout << "None\n";
      }
    }
    if (debug) {
      cout << tree.show() << endl;
    }
  }
}
