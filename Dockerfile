FROM ubuntu:16.04
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get -y install gcc
RUN mkdir -p /opt/memoryhog
ADD main.c /opt/memoryhog
RUN cd /opt/memoryhog/ && gcc -Wall -O3 main.c -o memoryhog
CMD ["/opt/memoryhog/memoryhog"]

