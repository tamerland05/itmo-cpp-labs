#include "parser/ArgParser.h"
#include "hamming_code.cpp"

#include <string>
#include <cstring>
#include <fstream>

const uint8_t SizeOfHeaderInfo = 7;
const uint8_t SizeOfFileSize = 5;

class Archiver {
public:
    Archiver();
    void Parse(int argc, char* argv[]);
    void Do();

private:
    void CreateArchive();
    static uint64_t GetFileSize(const std::string& file_name);
    void GetHamFromInfo(uint8_t* inf_block, uint8_t* enc_block);
    static void AddFileInfoToHeader(const uint64_t& file_size, const std::string& file_name, std::vector<uint8_t>& output);
    void AddHeaderToArch(const std::vector<uint8_t>& input);
    void AddFileDataToArch(const std::string& file_name, uint64_t file_size);

    static void CopyFiles(const std::string& file_in, const std::string& file_out);
    uint64_t ReadHeader(std::vector<std::pair<uint64_t, std::string>>& files_info);
    void SetHeaderSize(uint64_t size, std::vector<uint8_t> header);
    void ListArchFiles();
    void ExtractFiles();
    void GetInfoFromHam(uint8_t* enc_block, uint8_t* inf_block);
    void PutFileInfo(uint8_t* enc_block, uint8_t* inf_block, std::fstream& file, uint64_t& p_file, uint64_t file_size);
    void AppendFiles();
    void DeleteFile();
    void ConcateArch();

    ArgumentParser::ArgParser parser_for_arch{"Parser for archiver"};
    std::fstream my_arch{};
    std::string my_arch_name{};
    std::vector<std::string> files{};
};