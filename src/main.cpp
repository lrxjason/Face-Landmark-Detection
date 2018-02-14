#include <cstdio>
#include "common.hpp"

using namespace std;
using namespace lbf;

int camera(int argc, char *argv[]);

static void usage(const char* cmd) {
    printf("Usage: %s prepare/train/test/run/live/camera\n", cmd);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
    } else if (strcmp(argv[1], "camera") == 0) {
        return camera(argc-2, argv+2);
    } else {
        printf("Unsupport command %s\n", argv[1]);
        usage(argv[0]);
    }
    return 0;
}
