#include "generator/generator.h"

int main(int argc, char const *argv[])
{
    std::filesystem::path folder_path;
    if (argc < 2)
    {
        std::cerr << "No path provided, defaulting to ../src\n";
        folder_path = "../src";
    }
    else
    {
        folder_path = argv[1];
    }

    folder_path = folder_path.lexically_normal();

    // Removes trailing slashes
    folder_path = folder_path.filename().empty() ? folder_path.parent_path() : folder_path;

    if (!std::filesystem::exists(folder_path))
    {
        std::cerr << "Error: Path does not exist: " << folder_path << '\n';
        return 1;
    }

    if (!std::filesystem::is_directory(folder_path))
    {
        std::cerr << "Error: Path is not a directory: " << folder_path << '\n';
        return 1;
    }

    std::cout << "Generating code for " << folder_path << '\n';

    Generator generator;
    generator.generate(folder_path);
    return 0;
}