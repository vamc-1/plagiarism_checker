#cat > Dockerfile << 'EOF'
# Use Ubuntu with LLVM 16
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    clang-16 \
    libclang-16-dev \
    llvm-16-dev \
    g++ \
    cmake \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy all files
COPY . .

# Build the project
g++ -std=c++20     -I/usr/lib/llvm-16/include     -L/usr/lib/llvm-16/lib     tokenizer.cpp phase2_1.cpp main.cpp     -o plagiarism_checker     -lclang     -pthread     -Wl,-rpath,/usr/lib/llvm-16/lib

# Set entrypoint
ENTRYPOINT ["./plagiarism_checker"]
#EOF
