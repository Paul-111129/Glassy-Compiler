#include "Generator.h"
#include "Parser.h"
#include "Tokenizer.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#include <format>

int main() {
    const char* inputFileName = "src.glassy";
    const char* outputFileName = "out.asm";

    // open files
    const std::filesystem::path path = std::filesystem::current_path();

    std::ifstream inputFile(path / inputFileName, std::ios::in | std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open file: " << inputFileName << "\n";
        return 1;
    }

    std::ofstream outputFile(path / outputFileName);
    if (!outputFile) {
        std::cerr << "Failed to create file: " << outputFileName << "\n";
        return 1;
    }

    std::string input =
        std::string(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());

    Glassy::Tokenizer tokenizer(input);
    const auto tokens = tokenizer.Tokenize();

    for (int i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        std::cout << std::format("{:>3}: {:<10} {:<10} [Ln {:>3}, Col {:>3}]\n", i + 1, token.ToStr(),
            token.value.value_or(""), token.location.line, token.location.column);
    }

    Glassy::Parser parser(tokenizer.Tokenize());

    auto program = parser.ParseProgram();

    Glassy::Generator generator(program);
    outputFile << generator.GenerateAsm();

    std::cin.get();
    return 0;
}
