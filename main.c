#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>


long DEFAULT_LIMIT = 1024;
long DEFAULT_INCREMENT = 1;
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
    fprintf(stdout, "ru_maxrss: %ldmb\tru_ixrss: %ldmb\tru_idrss: %ldmb\tru_isrss: %ldmb\n",
            usg.ru_maxrss / 1024,
            usg.ru_ixrss / 1024,
            usg.ru_idrss / 1024,
            usg.ru_isrss / 1024);
}

int
loop(size_t limit, size_t increment, bool should_stop)
{
    size_t current_usage = 0;
    char* data;
    while(limit > current_usage)
    {
        if ((data = malloc(1024 * 1024 * increment)) == NULL)
        {
            fprintf(stderr, "failed to malloc %ldMB more at %ld\n", increment, current_usage);
            return -1;
        }
        for (int i=0; i<1024 * 1024 * increment; ++i)
        {
            data[i] = 1;
        }
        print_curr_mem_usage();

        current_usage += increment;
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
