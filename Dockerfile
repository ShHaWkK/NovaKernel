FROM debian:bookworm
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    grub-pc-bin \
    xorriso \
    qemu-system-x86 \
    rustc \
    cargo && rm -rf /var/lib/apt/lists/*
WORKDIR /src
CMD ["bash"]
