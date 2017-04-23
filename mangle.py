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
    return "RUN echo '{}' >> /opt/memoryhog/main.c\n".format(l)

with open('Dockerfile', 'w') as df:
    df.writelines(start)
    with open('main.c') as inf:
        buf = '/*w*/'
        for line in inf.readlines():
            if line[0] == '#':
                df.write(into_docker_line(line))
            else:
                l = line.replace('\n', ' ')
                buf = buf + l
        df.write(into_docker_line(buf))
    df.writelines(end)
