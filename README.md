# Compiler

**Work in Progress** — not ready for production use.

An educational compiler for a simple **C-like custom language**, targeting **x86-64 assembly (NASM syntax) on Linux**. This project is intended for learning purposes and aims to understand the core concepts of compiler design, including lexing, parsing, code generation.

---

## Language Syntax

This compiler supports a minimal subset of C-like syntax:

```c
{
    int a;
    a = (1 + 2) * 3;
    
    // comments

    int b;
    if (a) {
        b = 1;
    } else {
        b = 0;
    }
}
```

## Build

1. Clone the repository:
```sh
git clone https://github.com/paul-csc/Compiler.git Compiler
cd Compiler
```

2. Configure CMake:
```sh
cmake -S . -B build 
```

3. Compile the project:
```sh
cmake --build build
```

4. Run the compiler:
```sh
./build/Compiler
```

5. Assemble and run the generated assembly (example for main program):
```sh
./test/assemble.sh main
```
