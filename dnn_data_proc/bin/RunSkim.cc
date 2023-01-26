//#include "cms_runII_data_proc/processing/interface/file_looper.hh"
#include "dnn_data_proc/src/data_proc.cc"
#include <iostream>
#include <string>


void show_help() {
    /* Show help for input arguments */

    std::cout << "-y : Year\n";
    std::cout << "-c : Channel\n";
    std::cout << "--sample : sample name\n";
    std::cout << "--sum_w : sum of weights of name\n";
    std::cout << "-i : input file";
    std::cout << "-o : output file";
}

std::map<std::string, std::string> get_options(int argc, char* argv[]) {
    /*Interpret input arguments*/

    std::map<std::string, std::string> options;
    options.insert(std::make_pair("-y", "")); // Year
    options.insert(std::make_pair("-c", "")); // Channel
    options.insert(std::make_pair("--sample", "")); // sample str. 
    options.insert(std::make_pair("--sum_w", "1.0")); // sample sum_w. 
    options.insert(std::make_pair("-i", "")); // input file name
    options.insert(std::make_pair("-o", "")); // output file name

    if (argc >= 2) { //Check if help was requested
        std::string option(argv[1]);
        if (option == "-h" || option == "--help") {
            show_help();
            options.clear();
            return options;
        }
    }

    for (int i = 1; i < argc; i = i+2) {
        std::string option(argv[i]);
        std::string argument(argv[i+1]);
        if (option == "-h" || option == "--help" || argument == "-h" || argument == "--help") { // Check if help was requested
            show_help();
            options.clear();
            return options;
        }
        options[option] = argument;
    }
    return options;
}

int main(int argc, char *argv[]) {
    std::map<std::string, std::string> options = get_options(argc, argv); // Parse arguments
    if (options.size() == 0) return 1;

    FileLooper file_looper;
    std::cout << "Using sumw of " << std::stof(options["--sum_w"]) << std::endl;
    bool ok = skim_file(options["-i"], options["-o"], options["-c"], options["-y"],
                                    options["--sample"], std::stof(options["--sum_w"]))
    if (ok) std::cout << "File loop ran ok!\n";
    return 0;
}