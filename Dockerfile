# Не просто создаём образ, но даём ему имя build
FROM ubuntu:22.04

RUN apt update && \
    apt install -y \
    cmake \
    libompl-dev \
    build-essential \
    libboost-serialization-dev \
    libboost-filesystem-dev

RUN mkdir app \
    app/build

# Папка data больше не нужна
COPY ./src /app/src
COPY ./include /app/include
COPY ./data /app/data
COPY ./pathfinding /app/pathfinding
COPY CMakeLists.txt /app/

RUN cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

# Запускаем игровой сервер
ENTRYPOINT ["/app/build/BPR_NAMI"] 