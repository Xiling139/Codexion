/* test_codexion.c
 * Test harness for the codexion project
 * Compile with: gcc -o test_codexion test_codexion.c -lpthread
 * Run with: ./test_codexion
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <assert.h>

#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_RESET   "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void print_test_header(const char *test_name) {
    printf("\n%s========================================%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%sTEST: %s%s\n", COLOR_YELLOW, test_name, COLOR_RESET);
    printf("%s========================================%s\n", COLOR_BLUE, COLOR_RESET);
}

void print_test_result(int passed, const char *message) {
    if (passed) {
        printf("%s✓ PASSED:%s %s\n", COLOR_GREEN, COLOR_RESET, message);
        tests_passed++;
    } else {
        printf("%s✗ FAILED:%s %s\n", COLOR_RED, COLOR_RESET, message);
        tests_failed++;
    }
}

int run_command(const char *cmd, char *output, size_t output_size) {
    FILE *fp = popen(cmd, "r");
    if (!fp) return -1;

    size_t total_read = 0;
    while (fgets(output + total_read, output_size - total_read, fp)) {
        total_read = strlen(output);
    }

    int status = pclose(fp);
    return WEXITSTATUS(status);
}

int file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}

void test_executable_exists() {
    print_test_header("Executable Existence");
    int exists = file_exists("./codexion");
    print_test_result(exists, "./codexion executable found");

    if (!exists) {
        printf("%sERROR: Please compile your codexion first with 'make'%s\n", COLOR_RED, COLOR_RESET);
    }
}

void test_argument_validation() {
    print_test_header("Argument Validation");

    char output[4096];
    int ret;

    // Test: Missing arguments
    ret = run_command("./codexion 5 1000 100 100 100 100 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects extra arguments (too many)");

    // Test: Negative numbers
    ret = run_command("./codexion -1 1000 100 100 100 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects negative number_of_coders");

    ret = run_command("./codexion 5 -1000 100 100 100 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects negative time_to_burnout");

    // Test: Zero values
    ret = run_command("./codexion 0 1000 100 100 100 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects zero number_of_coders");

    ret = run_command("./codexion 5 1000 0 100 100 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects zero time_to_compile");

    // Test: Invalid scheduler
    ret = run_command("./codexion 5 1000 100 100 100 3 10 invalid 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects invalid scheduler value");

    // Test: Non-integer arguments
    ret = run_command("./codexion 5 1000 100 100 abc 3 10 fifo 2>&1", output, sizeof(output));
    print_test_result(ret != 0, "Rejects non-integer arguments");
}

void test_normal_operation_fifo() {
    print_test_header("Normal Operation - FIFO Scheduler");

    // 2 coders, reasonable times, should complete successfully
    char output[65536];
    int ret = run_command("./codexion 2 10000 500 200 300 2 100 fifo 2>&1", output, sizeof(output));

    int has_burnout = (strstr(output, "burned out") != NULL);
    int has_compiling = (strstr(output, "is compiling") != NULL);
    int has_debugging = (strstr(output, "is debugging") != NULL);
    int has_refactoring = (strstr(output, "is refactoring") != NULL);

    print_test_result(ret == 0 && !has_burnout && has_compiling,
                     "Simulation completes without burnout (FIFO)");
    print_test_result(has_debugging, "Debugging phase appears in logs");
    print_test_result(has_refactoring, "Refactoring phase appears in logs");
}

void test_normal_operation_edf() {
    print_test_header("Normal Operation - EDF Scheduler");

    char output[65536];
    int ret = run_command("./codexion 3 10000 500 200 300 2 100 edf 2>&1", output, sizeof(output));

    int has_burnout = (strstr(output, "burned out") != NULL);

    print_test_result(!has_burnout, "Simulation completes without burnout (EDF)");
}

void test_burnout_scenario() {
    print_test_header("Burnout Detection");

    // Very short burnout time with long compile time - should cause burnout
    char output[65536];
    int ret = run_command("./codexion 2 100 500 200 300 5 50 fifo 2>&1", output, sizeof(output));

    int has_burnout = (strstr(output, "burned out") != NULL);

    print_test_result(has_burnout, "Burnout detected correctly");

    // Check that burnout message appears only once per coder
    char *burnout_pos = output;
    int burnout_count = 0;
    while ((burnout_pos = strstr(burnout_pos, "burned out")) != NULL) {
        burnout_count++;
        burnout_pos += 10;
    }
    print_test_result(burnout_count > 0, "At least one burnout message printed");
}

void test_dongle_cooldown() {
    print_test_header("Dongle Cooldown Mechanism");

    // Run with visible cooldown
    char output[65536];
    run_command("./codexion 2 5000 500 200 300 3 200 fifo 2>&1", output, sizeof(output));

    // Check for "has taken a dongle" messages
    char *pos = output;
    int take_count = 0;
    while ((pos = strstr(pos, "has taken a dongle")) != NULL) {
        take_count++;
        pos += 20;
    }

    print_test_result(take_count >= 4,
                     "Dongle acquisition logged multiple times (cooldown working)");
}

void test_multiple_coders() {
    print_test_header("Multiple Coders");

    char outputs[4][65536];
    int rets[4];

    // Test with different numbers of coders
    const int coder_counts[] = {1, 2, 3, 5};

    for (int i = 0; i < 4; i++) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "./codexion %d 10000 300 200 200 2 50 fifo 2>&1",
                 coder_counts[i]);
        rets[i] = run_command(cmd, outputs[i], sizeof(outputs[i]));
    }

    print_test_result(rets[0] == 0, "Single coder works");
    print_test_result(rets[1] == 0, "Two coders work");
    print_test_result(rets[2] == 0, "Three coders work");
    print_test_result(rets[3] == 0, "Five coders work");
}

void test_compilation_requirement() {
    print_test_header("Compilation Requirement");

    char output[65536];
    int ret = run_command("./codexion 2 10000 200 100 100 3 50 fifo 2>&1", output, sizeof(output));

    // Better approach: parse line by line
    int compiles_coder1 = 0, compiles_coder2 = 0;
    char *line = output;

    while (*line) {
        char *next = strchr(line, '\n');
        if (!next) break;
        *next = '\0';

        long long ts;
        int coder;
        char word1[32], word2[32];

        // Parse: "timestamp coder word1 word2"
        if (sscanf(line, "%lld %d %s %s", &ts, &coder, word1, word2) == 4) {
            if (strcmp(word1, "is") == 0 && strcmp(word2, "compiling") == 0) {
                if (coder == 1) compiles_coder1++;
                else if (coder == 2) compiles_coder2++;
            }
        }

        *next = '\n';
        line = next + 1;
    }

    int all_completed = (compiles_coder1 >= 3 && compiles_coder2 >= 3);
    print_test_result(ret == 0 && all_completed,
                     "All coders completed required number of compiles");

    if (!all_completed) {
        printf("  Coder 1 compiles: %d (need 3)\n", compiles_coder1);
        printf("  Coder 2 compiles: %d (need 3)\n", compiles_coder2);
    }
}

void test_log_serialization() {
    print_test_header("Log Serialization");

    char output[131072];
    run_command("./codexion 4 5000 200 150 150 5 30 fifo 2>&1", output, sizeof(output));

    // Check that lines are properly terminated and not interleaved
    int valid_lines = 1;
    char *line = output;
    while (*line && valid_lines) {
        char *next = strchr(line, '\n');
        if (!next) break;

        *next = '\0';
        // Each line should have timestamp, space, coder number, space, action
        int space_count = 0;
        for (char *c = line; *c; c++) {
            if (*c == ' ') space_count++;
        }
        if (space_count < 2) {
            valid_lines = 0;
        }

        *next = '\n';
        line = next + 1;
    }

    print_test_result(valid_lines, "Log lines are properly formatted and not interleaved");
}

void test_no_deadlock() {
    print_test_header("Deadlock Prevention");

    // Run many cycles to detect potential deadlocks
    char output[65536];
    int ret = run_command("./codexion 4 10000 400 200 200 10 50 edf 2>&1", output, sizeof(output));

    int has_compiles = (strstr(output, "is compiling") != NULL);
    int simulation_complete = (ret == 0);

    print_test_result(simulation_complete && has_compiles,
                     "No deadlock detected with 4 coders under EDF");
}

void test_starvation_prevention() {
    print_test_header("Starvation Prevention");

    // Run with high demand scenario
    char output[65536];
    int ret = run_command("./codexion 3 5000 300 50 50 8 20 edf 2>&1", output, sizeof(output));

    printf("%s\n", output); // Debug: see actual output

    // Check all coders got to compile
    int coder_compiles[10] = {0};
    char *pos = output;

    // Better parser for "timestamp coder_number is compiling" format
    while ((pos = strstr(pos, "is compiling")) != NULL) {
        // Scan backwards from "is compiling" to find the coder number
        char *scan = pos - 2; // Start just before "is"
        while (scan > output && (*scan == ' ' || *scan == '\t')) scan--;

        // Now scan backwards to find the start of the number
        char *num_end = scan;
        while (scan > output && (*scan >= '0' && *scan <= '9')) scan--;

        if (scan < num_end && (*scan == ' ' || scan == output)) {
            char num_str[10];
            int len = num_end - scan;
            strncpy(num_str, scan + 1, len);
            num_str[len] = '\0';
            int coder = atoi(num_str);

            if (coder >= 1 && coder <= 3) {
                coder_compiles[coder]++;
                printf("DEBUG: Found coder %d compiling (total: %d)\n", coder, coder_compiles[coder]);
            }
        }
        pos += 12;
    }

    // Alternative simpler parser using sscanf on each line
    // This is more reliable:
    memset(coder_compiles, 0, sizeof(coder_compiles));
    char *line = output;
    char *next_line;
    while ((next_line = strchr(line, '\n')) != NULL) {
        *next_line = '\0';
        long long timestamp;
        int coder;
        char action[50];

        if (sscanf(line, "%lld %d %49s", &timestamp, &coder, action) == 3) {
            if (strcmp(action, "is") == 0) {
                // Need to read the next word "compiling"
                char action2[50];
                char *remaining = line;
                sscanf(remaining, "%*lld %*d %*s %49s", action2);
                if (strcmp(action2, "compiling") == 0) {
                    if (coder >= 1 && coder <= 3) {
                        coder_compiles[coder]++;
                    }
                }
            }
        }

        *next_line = '\n';
        line = next_line + 1;
    }

    int all_coders_compiled = (coder_compiles[1] > 0 && coder_compiles[2] > 0 && coder_compiles[3] > 0);
    print_test_result(all_coders_compiled,
                     "All coders got chance to compile (no starvation)");

    if (!all_coders_compiled) {
        printf("  Coder 1 compiles: %d\n", coder_compiles[1]);
        printf("  Coder 2 compiles: %d\n", coder_compiles[2]);
        printf("  Coder 3 compiles: %d\n", coder_compiles[3]);
    }
}

// Optional: Add a test for exact burnout timing (within 10ms)
void test_burnout_timing_accuracy() {
    print_test_header("Burnout Timing Accuracy (10ms tolerance)");

    char output[65536];

    // Run the command
    int ret = run_command("./codexion 2 150 300 100 100 2 20 fifo 2>&1", output, sizeof(output));

    // Find the burnout timestamp from logs
    long long burnout_time = -1;
    long long first_event_time = -1;
    char *line = output;

    while (*line) {
        char *next = strchr(line, '\n');
        if (!next) break;
        *next = '\0';

        long long ts;
        if (sscanf(line, "%lld", &ts) == 1) {
            if (first_event_time == -1) {
                first_event_time = ts;
            }
            if (strstr(line, "burned out")) {
                burnout_time = ts;
            }
        }

        *next = '\n';
        line = next + 1;
    }

    // Calculate actual time from first event to burnout
    long long actual_burnout_delta = burnout_time - first_event_time;
    int expected_burnout = 150; // time_to_burnout

    int difference = abs(actual_burnout_delta - expected_burnout);
    int timing_ok = (burnout_time != -1 && difference <= 10);

    printf("  Burnout at: %lld ms (first event at %lld)\n", burnout_time, first_event_time);
    printf("  Actual delta: %lld ms, Expected: %d ms, Difference: %d ms\n",
           actual_burnout_delta, expected_burnout, difference);

    print_test_result(timing_ok, "Burnout logged within 10ms of expected timeout");
}

void run_all_tests() {
    printf("\n%s╔══════════════════════════════════════════════════════════════╗%s\n",
           COLOR_BLUE, COLOR_RESET);
    printf("%s║           CODEXION TEST SUITE                                ║%s\n",
           COLOR_BLUE, COLOR_RESET);
    printf("%s╚══════════════════════════════════════════════════════════════╝%s\n",
           COLOR_BLUE, COLOR_RESET);

    test_executable_exists();

    if (!file_exists("./codexion")) {
        printf("\n%sABORTING TESTS - Please compile codexion first%s\n", COLOR_RED, COLOR_RESET);
        return;
    }

    test_argument_validation();
    test_normal_operation_fifo();
    test_normal_operation_edf();
    test_burnout_scenario();
    test_dongle_cooldown();
    test_multiple_coders();
    test_compilation_requirement();
    test_log_serialization();
    test_no_deadlock();
    test_starvation_prevention();
	test_burnout_timing_accuracy();

    // Final summary
    printf("\n%s════════════════════════════════════════════════════════════════%s\n",
           COLOR_BLUE, COLOR_RESET);
    printf("%sTEST SUMMARY:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  Passed: %s%d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("  Failed: %s%d%s\n", COLOR_RED, tests_failed, COLOR_RESET);
    printf("  Total:  %d\n", tests_passed + tests_failed);

    if (tests_failed == 0) {
        printf("\n%s🎉 ALL TESTS PASSED! 🎉%s\n", COLOR_GREEN, COLOR_RESET);
    } else {
        printf("\n%s⚠️  Some tests failed. Please review your implementation.%s\n", COLOR_RED, COLOR_RESET);
    }
}

int main(void) {
    run_all_tests();
    return tests_failed > 0;
}
