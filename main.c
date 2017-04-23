#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>


long DEFAULT_LIMIT = 128;
long DEFAULT_INCREMENT = 8;
bool DEFAULT_SHOULD_STOP = false;

size_t
env_to_sizet(char* env_var, long def)
{
    char* input = getenv(env_var);
    if (input == NULL)
    {
        return def;
    }

    char* parse_end;
    long retval = strtol(input, &parse_end, 10);
    if (retval < 1 || parse_end == input)
    {
        fprintf(stderr, "Value %s of env var %s is invalid, using default value instead\n", input, env_var);
        return def;
    }

    return (size_t)retval;
}

void
print_curr_mem_usage()
{
    struct rusage usg;
    int err_code;
    if ((err_code = getrusage(RUSAGE_SELF, &usg)) != 0)
    {
        fprintf(stderr, "getrusage failed with %d\n", err_code);
        return;
    }
    fprintf(stdout, "ru_maxrss: %ldmb\n",
            usg.ru_maxrss / 1024);
}

int
loop(size_t limit, size_t increment, bool should_stop)
{
    size_t current_usage = 0;
    int* data;
    while(limit > current_usage)
    {
        if ((data = malloc(1024 * 1024 * increment)) == NULL)
        {
            fprintf(stderr, "failed to malloc %ldMB more at %ld\n", increment, current_usage);
            return -1;
        }
        for (int i=0; i<1024 * 1024 / 4 * increment; ++i)
        {
            data[i] = rand();
        }
        current_usage += increment;
        print_curr_mem_usage();
        printf("Currently written to %ldMB of memory\n", current_usage);

        sleep(1);
    }
    fprintf(stdout, "Will stop hogging memory now, currently hogged %ld\n", current_usage);
    while (!should_stop)
    {
        sleep(1);
    }
    return 0;
}

int
main()
{
    size_t limit = env_to_sizet("MEMORY_LIMIT", DEFAULT_LIMIT);;
    size_t increment = env_to_sizet("MEMORY_INCREMENT", DEFAULT_INCREMENT);
    bool should_stop = !(getenv("SHOULD_CONTINUE") != NULL);

    printf("limit: %ld\n", limit);
    printf("increment: %ld\n", increment);
    printf("should_stop: %s\n", should_stop ? "true" : "false");

    return loop(limit, increment, should_stop);
}
