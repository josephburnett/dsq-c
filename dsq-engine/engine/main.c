#include "common.h"
#include "interface.h"

int main(int argc, char* argv[])
{

    //console();

    //testsuite();

    if (argc == 2) testfile(argv[1]);
    else printf("No input file\n");

    return 0;
}



