FROM frolvlad/alpine-gcc
RUN mkdir -p /opt/memoryhog
ADD main.c /opt/memoryhog
RUN cd /opt/memoryhog/ && gcc -Wall -O3 main.c -o memoryhog
CMD ["/opt/memoryhog/memoryhog"]

