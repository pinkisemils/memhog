start = [
        'FROM frolvlad/alpine-gcc\n',
        'RUN mkdir -p /opt/memoryhog\n'
    ]
end = [
    'RUN cd /opt/memoryhog/ && gcc -Wall -O3 main.c -o memoryhog\n',
    'CMD ["/opt/memoryhog/memoryhog"]',
    ]
def into_docker_line(l):
    l = l.replace('\n', '')
    return "echo \"{}\" >> /opt/memoryhog/main.c && \\\n".format(l)

with open('Dockerfile', 'w') as df:
    df.writelines(start)
    with open('main.c') as inf:
        buf = 'RUN '
        for line in inf.readlines():
            l = line.replace('\n', ' ')
            l = line.replace("'", "\\'")
            l = line.replace('"', '\\"')
            buf = buf + into_docker_line(l)
        df.write(buf + '\n')
        df.write("echo lol\n")
    df.writelines(end)
