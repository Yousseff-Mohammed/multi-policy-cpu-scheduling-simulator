# CPU Scheduling Docker Image

This repository contains a Dockerized CPU Scheduling simulation written in C++. It supports scheduling policies such as FCFS, RR, SPN, SRT, HRRN, FB-1, FB-2i, and Aging.

## Docker Image

You can pull the image from Docker Hub: [https://hub.docker.com/repository/docker/y0uss3ff/cpu-scheduler/general]

## Build Locally

If you want to build the Docker image locally:

```bash
git clone <repository-url>
cd <local-repository-name>
docker build -t cpu-scheduling .
```

## Containerize the image and provide input

`docker run -i cpu-scheduling < input.txt`

input.txt example:
```bash
trace
1
20
5
A,0,3
B,2,6
C,4,4
D,6,5
E,8,2
```
