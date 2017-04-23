# What even is this?
It's a docker container that will take up more memory as times goes on.
By default, it will increase it's memory consumption by 8MB every second,
until it hits 128MB and then it will return. If it fails to `malloc()`
at any point in time, it will return with a non-zero return code.

# What's this mangle.py?
If one changes the C code, in order to make this Dockerfile distributable
easily, `mangle.py` will generate a new Dockerfile with the C code already
_embedded_. So, to generate the Dockerfile, just run `python mangle.py`.

# How to make it do more or less?
Here's a list of environment variables that will change it's behaviour:
- `MEMORY_LIMIT` - setting this to a positive integer will set the limit
on how much memory the container eventually will allocate in megabytes.
- `MEMORY_INCREMENT` - setting this to a postivie integer will change the
amount of megabytes the container will allocate every second.
- `SHOULD_CONTINUE` - setting this environment variable will force the
process to loop forever when it hits it's memory limit. It won't allocate
any more. If an eventual death is required, set the `MEMORY_LIMIT` variable
big enough.

# Why?
Because testing.
