#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD "build/"
#define TESTS "tests/"
#define INCLUDE "include/"
#define CC "clang"
#define LD "gcc"

static const char *sources[] = {
    "arena",
    "pool",
    "ring",
};

static const char *tests[] = {
    "test_arena",
    "test_pool",
    "test_ring",
};

typedef enum {
    CMD_MAKE,
    CMD_TEST,
    CMD_CLEAN,
    CMD_UNKNOWN,
} command_t;

char *add_extension(const char *name, const char *ext) {
    char *buffer = malloc(strlen(name) + strlen(ext));
    if (!buffer) {
        return NULL;
    }
    sprintf(buffer, "%s.%s", name, ext);
    return buffer;
}

struct command_map_entry {
    const char *name;
    command_t cmd;
} commands_map[] = {
    {"build", CMD_MAKE},
    {"clean", CMD_CLEAN},
    {"test", CMD_TEST},
};

command_t parse_args(int argc, char **argv) {
    if (argc == 1) {
        return CMD_MAKE;
    } else {
        size_t map_len = sizeof(commands_map) / sizeof(struct command_map_entry); 
        for (size_t i = 0; i < map_len; i++) {
            if (!strcmp(argv[1], commands_map[i].name)) {
                return commands_map[i].cmd;
            }
        }
    }
    return CMD_UNKNOWN;
}

int build_all() {
    Nob_Cmd ar_cmd = {0};
    int result = 0;
    size_t sources_len = sizeof(sources)/sizeof(const char*);
    char **ptrs = alloca(sources_len*2);

    nob_cmd_append(&ar_cmd, "ar", "rcs", "libtorin.a");

    size_t i = 0;
    for (; i < sources_len; i++) {
        Nob_Cmd cmd = {0};

        char *src_name = add_extension(sources[i], "c");
        char *obj_name = add_extension(sources[i], "o");

        ptrs[i*2] = src_name;
        ptrs[i*2+1] = obj_name;

        nob_cmd_append(&cmd, CC,
                        "-Wall", "-Wextra","-Wpedantic", "-Og",
                        "-c", src_name,
                        "-o", obj_name,
                        "-I", "include",
                       "-std=c23");

        nob_cmd_append(&ar_cmd, obj_name);

        if (!nob_cmd_run(&cmd)) {
            result = 1;
            goto cleanup;
        }
    }

    if (!nob_cmd_run(&ar_cmd)) {
        result = 1;
        goto cleanup;
    }

cleanup:
    for (size_t j = 0; j < i*2; j++) {
        free(ptrs[j]);
    }

    return result;
}

int test() {
    int result = 0;
    size_t tests_len = sizeof(tests)/sizeof(const char*);
    char **ptrs = alloca(tests_len*sizeof(char*));

    size_t i = 0;
    for (; i < tests_len; i++) {
        Nob_Cmd cmd = {0};

        char *src_name = add_extension(tests[i], "c");
        ptrs[i] = src_name;

        nob_cmd_append(&cmd, CC,
                        "-Wall", "-Wextra","-Wpedantic", "-Og",
                        src_name, "-o", 
                        tests[i], "-I", 
                        "include", "-std=c23",
                       "libtorin.a");

        if (!nob_cmd_run(&cmd)) {
            result = 1;
            goto cleanup;
        }
    }

cleanup:
    for (size_t j = 0; j < i; j++) {
        free(ptrs[j]);
    }

    return result;
}

int clean() {
    int result = 0;
    size_t sources_len = sizeof(sources)/sizeof(const char*);
    size_t i = 0;

    for (; i < sources_len; i++) {
        char *obj_name = add_extension(sources[i], "o");
        nob_delete_file(obj_name);
        free(obj_name);
    }

    for (size_t j = 0; j < sizeof(tests)/sizeof(const char*); j++) {
        nob_delete_file(tests[j]);
    }

    return result;
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    const command_t command = parse_args(argc, argv);
    switch (command) {
    case CMD_MAKE:
        return build_all();
    case CMD_CLEAN:
        return clean();
    case CMD_TEST:
        return test();
    case CMD_UNKNOWN:
        nob_log(NOB_ERROR, "Invalid command: %s", argv[1]);
        return 1;
    }

    return 0;
}
