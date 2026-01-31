FROM gcc:11.2

WORKDIR /app

COPY cpu-scheduling.cpp cpu-scheduling.h Makefile ./

RUN make

ENTRYPOINT ["./cpu-scheduling"]
