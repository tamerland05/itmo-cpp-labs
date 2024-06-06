#include "archiver.h"

Archiver::Archiver() {
    parser_for_arch.AddFlag('c', "create", "create of new archive");
    parser_for_arch.AddStringArgument('f', "file", "Path of archive");
    parser_for_arch.AddFlag('l', "list", "Output list of files in archive");
    parser_for_arch.AddFlag('x', "extract", "Extract files from archive");
    parser_for_arch.AddFlag('a', "append", "Append files on archive");
    parser_for_arch.AddFlag('d', "delete", "Delete file from archive");
    parser_for_arch.AddFlag('A', "concatenate", "Merge of two archives");
    parser_for_arch.AddStringArgument("files", "Names of input files").MultiValue().Positional().Default("all");
}

void Archiver::Parse(int argc, char* argv[]) {
    parser_for_arch.Parse(std::vector<std::string>(argv, argv + argc));
}

void Archiver::Do() {
    if (parser_for_arch.GetFlag("create")) {
        CreateArchive();
    } else if (parser_for_arch.GetFlag("list")) {
        ListArchFiles();
    } else if (parser_for_arch.GetFlag("extract")) {
        ExtractFiles();
    } else if (parser_for_arch.GetFlag("append")) {
        AppendFiles();
    } else if (parser_for_arch.GetFlag("delete")) {
        DeleteFile();
    } else if (parser_for_arch.GetFlag("concatenate")) {
        ConcateArch();
    }
}

void Archiver::CreateArchive() {
    std::vector<std::string> files = parser_for_arch.GetStringMultiValue("files");
    uint64_t header_size{};
    std::vector<uint8_t> header(SizeOfHeaderInfo);

    for (const auto& path : files) {
        std::string file_name{};
        if (path.rfind('\\') != std::string::npos) {
            file_name = path.substr(path.rfind('\\') + 1, path.length() - path.rfind('\\'));
        } else {
            file_name = path;
        }
        AddFileInfoToHeader(GetFileSize(path), file_name, header);
        header_size += SizeOfFileSize + file_name.length() + 1;
    }

    for (int i = SizeOfHeaderInfo; i > 0; --i) {
        header[SizeOfHeaderInfo - i] = static_cast<uint8_t>(header_size % (1ll * 1 << SizeOfByte * i) >> (SizeOfByte * (i - 1)));
    }

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::out | std::ios::binary);

    AddHeaderToArch(header);
    for (const auto& file_name : files) {
        AddFileDataToArch(file_name, GetFileSize(file_name));
    }

    my_arch.close();
}

void Archiver::AddFileInfoToHeader(const uint64_t& file_size, const std::string& file_name, std::vector<uint8_t>& output) {

    for (int i = SizeOfFileSize; i > 0; --i) {
        output.emplace_back((file_size % (1ll * 1 << (SizeOfByte * i)) >> (SizeOfByte * (i - 1))));
    }

    output.emplace_back(file_name.length());

    for (auto chr : file_name) {
        output.emplace_back(static_cast<uint8_t>(chr));
    }
}

void Archiver::GetHamFromInfo(uint8_t* inf_block, uint8_t* enc_block) {
    memset(enc_block, 0, LengthOfEBlock / SizeOfByte);

    HammingCoding(inf_block, enc_block);
    for (int i = 0; i < LengthOfEBlock / 8; ++i) {
        my_arch << enc_block[i];
    }
    memset(inf_block, 0, LengthOfIBlock / SizeOfByte);
}

void Archiver::AddHeaderToArch(const std::vector<uint8_t>& input) {
    uint8_t inf_block[LengthOfIBlock / SizeOfByte] = {0};
    uint8_t enc_block[LengthOfEBlock / SizeOfByte] = {0};
    int p_block = 0;

    for (auto i : input) {
        if (p_block == LengthOfIBlock / SizeOfByte) {
            p_block = 0;
            GetHamFromInfo(inf_block, enc_block);
        }
        inf_block[p_block++] = i;
    }
    if (p_block) {
        GetHamFromInfo(inf_block, enc_block);
    }
}

void Archiver::AddFileDataToArch(const std::string& file_name, uint64_t file_size) {
    std::fstream file(file_name, std::ios::in | std::ios::binary);
    file.seekg(0);
    if (!file) {
        std::cerr << "The " << file_name << " file does not exist";
        exit(EXIT_FAILURE);
    }

    uint8_t inf_block[LengthOfIBlock / SizeOfByte] = {0};
    uint8_t enc_block[LengthOfEBlock / SizeOfByte] = {0};
    int p_block = 0;
    uint64_t p_file = 0;

    while (p_file < file_size) {
        if (p_block >= LengthOfIBlock / SizeOfByte) {
            p_block = 0;
            GetHamFromInfo(inf_block, enc_block);
        }
        inf_block[p_block++] = file.get();
        ++p_file;
    }
    if (p_block) {
        GetHamFromInfo(inf_block, enc_block);
    }

    file.close();
}

uint64_t Archiver::GetFileSize(const std::string& file_name) {
    std::fstream file(file_name);
    file.seekp(0, std::ios_base::end);
    if (!file) {
        std::cerr << "The " << file_name << " file does not exist";
        exit(EXIT_FAILURE);
    }

    return file.tellg();
}

uint64_t Archiver::ReadHeader(std::vector<std::pair<uint64_t, std::string>>& files_info) {
    uint64_t header_info{};
    uint8_t enc_block[LengthOfEBlock / SizeOfByte] = {0};
    uint8_t inf_block[LengthOfIBlock / SizeOfByte] = {0};
    std::vector<uint8_t> header_data{};

    GetInfoFromHam(enc_block, inf_block);
    for (int i = SizeOfHeaderInfo; i < LengthOfIBlock / 8; ++i) {
        header_data.emplace_back(inf_block[i]);
    }
    for (int i = SizeOfHeaderInfo; i > 0; --i) {
        header_info += inf_block[SizeOfHeaderInfo - i] << (8 * (i - 1));
    }

    uint64_t num_of_blocks = GetHammingLength(header_info);
    for (int i = 1; i < num_of_blocks; ++i) {
        GetHamFromInfo(enc_block, inf_block);
        for (int j = SizeOfHeaderInfo; j < LengthOfIBlock / 8; ++j) {
            header_data.emplace_back(inf_block[j]);
        }
    }

    uint64_t file_size{};
    int h = 0;
    while (h < header_data.size()) {
        for (int j = SizeOfFileSize; j > 0; --j) {
            file_size += header_data[h++] << (8 * (j - 1));
        }
        if (!file_size && !header_data[h]) {
            break;
        }
        files_info.emplace_back(std::pair(file_size, ""));
        files_info.back().second = std::string(header_data.data() + h + 1,
                                           header_data.data() + h + 1 + header_data[h]);
        h += 1 + header_data[h];
        file_size = 0;
    }

    return header_info;
}

void Archiver::ListArchFiles() {
    std::vector<std::string> files;
    std::vector<std::pair<uint64_t, std::string>> files_info;

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::in | std::ios::binary);
    ReadHeader(files_info);
    for (const auto& file_inf : files_info) {
        std::cout << file_inf.second << '\n';
    }
}

void Archiver::ExtractFiles() {
    std::vector<std::string> files;
    std::vector<std::pair<uint64_t, std::string>> files_info;

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::in | std::ios::binary);
    ReadHeader(files_info);

    std::string file_path{};
    if (parser_for_arch.GetStringValue("file").find('\\') != std::string::npos) {
        std::string arch_path = parser_for_arch.GetStringValue("file");
        file_path = arch_path.substr(0, arch_path.rfind('\\') + 1);
    }

    uint8_t enc_block[LengthOfEBlock / SizeOfByte] = {0};
    uint8_t inf_block[LengthOfIBlock / SizeOfByte] = {0};
    for (const auto& file_inf : files_info) {
        uint64_t num_of_blocks = (file_inf.first + (LengthOfIBlock) / 8 - 1) / (LengthOfIBlock / 8);
        std::fstream file(file_path + file_inf.second, std::ios::out | std::ios::binary);
        uint64_t p_file = 0;
        for (int block = 0; block < num_of_blocks; ++block) {
            PutFileInfo(enc_block, inf_block, file, p_file, file_inf.first);
        }
        file.close();
    }

    my_arch.close();
}

void Archiver::GetInfoFromHam(uint8_t* enc_block, uint8_t* inf_block) {
    memset(inf_block, 0, LengthOfIBlock / SizeOfByte);
    int j = 0;
    for (int i = 0; i < LengthOfEBlock / 8; ++i) {
        enc_block[i] = my_arch.get();
        ++j;
    }
    HammingCoding(enc_block, inf_block, false);
    memset(enc_block, 0, LengthOfEBlock / SizeOfByte);
}

void Archiver::PutFileInfo(uint8_t* enc_block, uint8_t* inf_block, std::fstream& file, uint64_t& p_file, uint64_t file_size) {
    memset(inf_block, 0, LengthOfIBlock / SizeOfByte);
    int p_block = 0;

    for (int i = 0; i < LengthOfEBlock / SizeOfByte; ++i) {
        enc_block[p_block++] = my_arch.get();
    }

    HammingCoding(enc_block, inf_block, false);

    for (int i = 0; (i < LengthOfIBlock / SizeOfByte) && p_file < file_size; ++i, ++p_file) {
        file << inf_block[i];
    }
    memset(enc_block, 0, LengthOfEBlock / SizeOfByte);
}

void Archiver::AppendFiles() {
    std::vector<std::pair<uint64_t, std::string>> files_info;
    std::vector<uint8_t> new_header(SizeOfHeaderInfo);

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::in | std::ios::binary);
    uint64_t header_size = ReadHeader(files_info);
    for (const auto& i : files_info) {
        AddFileInfoToHeader(i.first, i.second, new_header);
    }

    std::fstream haf_cash(parser_for_arch.GetStringValue("file") + "_cash",
                          std::ios::out | std::ios::binary);
    uint8_t c;
    while (!my_arch.eof()) {
        c = my_arch.get();
        if (!my_arch.eof()) {
            haf_cash << c;
        }
    }
    my_arch.close();
    haf_cash.close();

    std::string path(parser_for_arch.GetStringValue("files"));
    std::string file_name{};
    if (path.rfind('\\') != std::string::npos) {
        file_name = path.substr(path.rfind('\\') + 1, path.length() - path.rfind('\\'));
    } else {
        file_name = path;
    }
    AddFileInfoToHeader(GetFileSize(path), file_name, new_header);
    header_size += SizeOfFileSize + file_name.length() + 1;

    for (int i = SizeOfHeaderInfo; i > 0; --i) {
        new_header[SizeOfHeaderInfo - i] = static_cast<uint8_t>(header_size % (1ll * 1 << SizeOfByte * i) >> (SizeOfByte * (i - 1)));
    }

    haf_cash = std::fstream(parser_for_arch.GetStringValue("file") + "_cash",
                          std::ios::in | std::ios::binary);
    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::out | std::ios::binary);

    AddHeaderToArch(new_header);
    while (!haf_cash.eof()) {
        c = haf_cash.get();
        if (!haf_cash.eof()) {
            my_arch << c;
        }
    }
    AddFileDataToArch(path, GetFileSize(path));

    my_arch.close();
    haf_cash.close();
    remove((parser_for_arch.GetStringValue("file") + "_cash").c_str());
}

void Archiver::DeleteFile() {
    std::vector<std::pair<uint64_t, std::string>> files_info;
    std::vector<uint8_t> new_header(SizeOfHeaderInfo);

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::in | std::ios::binary);
    uint64_t header_size = ReadHeader(files_info);
    bool is_it = false;
    uint64_t start;
    uint64_t end;

    for (const auto& i : files_info) {
        if (i.second == parser_for_arch.GetStringValue("files")) {
            is_it = true;
            end = GetHammingLength(i.first);
            header_size -= SizeOfFileSize - 1 - i.second.length();
        } else {
            AddFileInfoToHeader(i.first, i.second, new_header);
            if (is_it) {
                start += GetHammingLength(i.first);
            }
        }
    }
    end += start;
    for (int i = SizeOfHeaderInfo; i > 0; --i) {
        new_header[SizeOfHeaderInfo - i] = static_cast<uint8_t>(header_size % (1ll * 1 << SizeOfByte * i) >> (SizeOfByte * (i - 1)));
    }

    std::fstream haf_cash(parser_for_arch.GetStringValue("file") + "_cash",
                          std::ios::out | std::ios::binary);
    uint8_t c;
    uint64_t p = 0;
    while (!my_arch.eof()) {
        c = my_arch.get();
        if (!my_arch.eof() && (p < start || p > end)) {
            haf_cash << c;
        }
        ++p;
    }
    my_arch.close();
    haf_cash.close();

    haf_cash = std::fstream(parser_for_arch.GetStringValue("file") + "_cash",
                            std::ios::in | std::ios::binary);
    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::out | std::ios::binary);

    AddHeaderToArch(new_header);
    while (!haf_cash.eof()) {
        c = haf_cash.get();
        if (!haf_cash.eof()) {
            my_arch << c;
        }
    }

    my_arch.close();
    haf_cash.close();
    remove((parser_for_arch.GetStringValue("file") + "_cash").c_str());
}



void Archiver::ConcateArch() {
    std::vector<std::pair<uint64_t, std::string>> files_info;
    std::vector<uint8_t> new_header(SizeOfHeaderInfo);
    uint64_t header_size{};

    std::fstream haf_cash(parser_for_arch.GetStringValue("file") + "_cash",
                          std::ios::in | std::ios::binary);
    uint8_t c;
    for (const auto& arch : parser_for_arch.GetStringMultiValue("files")) {
        my_arch = std::fstream(arch, std::ios::in | std::ios::binary);

        header_size += ReadHeader(files_info);
        while (!my_arch.eof()) {
            c = my_arch.get();
            if (!my_arch.eof()) {
                haf_cash << c;
            }
        }

        for (const auto &i: files_info) {
            AddFileInfoToHeader(i.first, i.second, new_header);
        }
        files_info.clear();
        my_arch.close();
    }

    for (int i = SizeOfHeaderInfo; i > 0; --i) {
        new_header[SizeOfHeaderInfo - i] = static_cast<uint8_t>(header_size % (1ll * 1 << SizeOfByte * i) >> (SizeOfByte * (i - 1)));
    }

    my_arch = std::fstream(parser_for_arch.GetStringValue("file"), std::ios::out | std::ios::binary);

    AddHeaderToArch(new_header);
    while (!haf_cash.eof()) {
        c = haf_cash.get();
        if (!haf_cash.eof()) {
            my_arch << c;
        }
    }

    my_arch.close();
    haf_cash.close();
    remove((parser_for_arch.GetStringValue("file") + "_cash").c_str());
}
