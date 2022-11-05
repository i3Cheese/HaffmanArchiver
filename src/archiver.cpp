#include <cstring>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include "archive.h"
#include "unarchive.h"

class BadArgumentsError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

void PrintHelp() {
    static const std::string HELP_STRING =
        "Usage: \n"
        "Archive:  archiver -c output_file file1 [file2 [file3 [...]]] \n"
        "Unarchive:  archiver -d path \n";
    std::cout << HELP_STRING;
}

void ParseArgsAndDo(int argc, char** argv) {
    if (argc < 2) {
        throw BadArgumentsError("Command Not Found");
    }
    if (strcmp(argv[1], "-d") == 0) {
        if (argc == 3) {
            std::string archive = argv[2];
            std::cout << "Unarchive \"" + archive + "\"\n";
            Unarchive(std::filesystem::path(archive));
        } else {
            throw BadArgumentsError("Unvalid number of arguments");
        }
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc >= 4) {
            std::string archive = argv[2];
            std::cout << "Archive to \"" + archive + "\"\n";
            std::vector<std::filesystem::path> files;
            for (int i = 3; i < argc; ++i) {
                files.emplace_back(argv[i]);
            }
            Archive(files, std::filesystem::path(archive));
        } else {
            throw BadArgumentsError("Unvalid number of arguments");
        }
    } else if (strcmp(argv[1], "-h") == 0) {
        PrintHelp();
    } else {
        throw BadArgumentsError("Invalid command");
    }
}

int main(int argc, char** argv) {
    try {
        ParseArgsAndDo(argc, argv);
    } catch (BadArgumentsError& e) {
        std::cout << "Bad arguments: " << e.what() << "\n";
        PrintHelp();
    } catch (std::exception& e) {
        std::cout << "Error while processing: " << e.what() << "\n";
    }
    return 0;
}
