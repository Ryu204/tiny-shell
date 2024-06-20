#include "delete_file.h"
#include "../os/operations.h"

enum run_result run_del(const os_char *filename) {
    bool result = delete_file(filename);
    return result ? RUN_OK : RUN_FAILED;
}