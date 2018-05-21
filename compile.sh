rootcint -f mydict.C -c Tree.hh Box.hh treeAppaLinkDef.h

g++ `root-config --cflags` -o program Tree.cpp Box.cpp mydict.C `root-config --glibs`

./program
