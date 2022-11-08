//
// Created by sakkammadam on 11/6/22.
//
#include "FileProcessorBase.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <tuple>

class FileProcessorMapOutput: public FileProcessorBase {
public:
    // This constructor is used to support mapper operation
    FileProcessorMapOutput(const std::string &operation, const std::map<std::string, std::vector<std::vector<std::vector<std::tuple<std::string, int, int>>>>> &map_raw): FileProcessorBase(){
        this->setOperation(operation);
        this->setRawMapperOutput(map_raw);
    }

    // Override virtual method from the base class
    // This implementation will write the mapper results to disk and save the mapper directory as a private data member
    virtual void runOperation() {
        std::cout << "Operation: " << this->getOperation() << std::endl;
        std::cout << "Proceeding to write Mapper data output to file system...." << std::endl;
        if (this->getOperation() == "mapper") {
            // Declare variable mapData
            // Key -> FileName
            // Value -> Vector containing tuples -> (Token, 1, Originating Partition the token (line belonged to)
            std::map<std::string, std::vector<std::vector<std::vector<std::tuple<std::string, int, int>>>>> mapData = this->getRawMapperOutput();
            // Declare tempDirectory
            std::string rootTempDirectory;
            // We will be iterating over the mapData map
            for (const auto &itr: mapData) {
                std::string fullyQualifiedFileName = itr.first;
                std::string fileDirectory = fullyQualifiedFileName.substr(0, fullyQualifiedFileName.rfind('/') + 1);
                rootTempDirectory = fileDirectory + "temp_mapper/";
                std::string baseFileName = fullyQualifiedFileName.substr(fullyQualifiedFileName.rfind('/') + 1);
                std::string tempDirectory = rootTempDirectory + baseFileName + "/";
                std::cout << "Operating on Mapper output from " << fullyQualifiedFileName << std::endl;
                std::cout << "Originating directory: " << fileDirectory << std::endl;
                std::cout << "Creating a temp directory to host mapper output: " << tempDirectory << std::endl;
                // The above three lines should be logged
                // Creating temp directory if temp directory is not there already
                this->createDirectory(tempDirectory);
                // Let's iterate over the entire mapper data going against each partition
                for (size_t partition = 0; partition < itr.second.size(); ++partition) {
                    // we will process all data within partition vector by writing to a file
                    // declare an ostream
                    std::ofstream outputFile;
                    // create an empty file against the temp directory
                    outputFile.open(tempDirectory + baseFileName + "." + std::to_string(partition));
                    // iterate over each partition vector
                    for (const auto &segment: itr.second[partition]) {
                        //outputFile << std::get<0>(itr.second[partition]) << std::endl;
                        for (const auto &token: segment) {
                            std::string prepRow = '(' +
                                                  std::get<0>(token) + ',' +
                                                  std::to_string((char) std::get<1>(token)) + ')';
                            outputFile << prepRow << std::endl;
                        }
                    }
                    // close the output file
                    outputFile.close();
                }
            }
            // save the root temp directory to private data member!
            this->setMapperOutputDirectory(rootTempDirectory);
        }
    }
};


// Class factories - constructor
extern "C" FileProcessorBase* createInputObj(
        const std::string &operation,
        const std::map<std::string, std::vector<std::vector<std::vector<std::tuple<std::string, int, int>>>>> &map_raw){
    return new FileProcessorMapOutput(operation, map_raw);
}

// Class factories - destructor
extern "C" void removeInputObj(FileProcessorBase* fpObj){
    delete fpObj;
}