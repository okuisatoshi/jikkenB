FROM ubuntu:26.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
  curl git gcc make tree bat file util-linux-extra htop \
  clang clangd clang-format llvm lld libclang-rt-dev-wasm32 wasi-libc \
  g++-aarch64-linux-gnu g++-x86-64-linux-gnu \
  qemu-user graphviz libsixel-bin sudo
RUN echo "ubuntu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/ubuntu && \
    echo "alias bat='batcat --theme zenburn'" >> /root/.bashrc && \
    echo "alias bat='batcat --theme zenburn'" >> /home/ubuntu/.bashrc
USER ubuntu
WORKDIR /workspace

