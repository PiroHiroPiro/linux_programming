FROM ubuntu:latest

RUN mkdir /root/workspace

WORKDIR /root/workspace

RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get -y install sudo
RUN sudo apt-get -y install build-essential
RUN sudo apt-get -y install git
RUN sudo apt-get -y install vim
RUN sudo apt-get -y install man
RUN sudo apt-get -y install manpages-dev
RUN sudo apt-get -y install strace
RUN sudo apt-get -y install gdb
RUN sudo apt-get -y install tree
RUN sudo apt-get -y install psmisc
RUN sudo apt-get -y install openbsd-inetd
RUN sudo apt-get -y install xinetd
RUN sudo apt-get -y install net-tools
