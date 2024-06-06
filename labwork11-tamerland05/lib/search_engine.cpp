#include "search_engine.h"

SearchEngine::SearchEngine(std::string directory)
        : working_directory_(std::move(directory))
        , indexes_directory_(working_directory_ + "\\search_engine_data")
        , main_block_(indexes_directory_ + "\\1.block") {}

void SearchEngine::DoIndex() {
    if (std::filesystem::exists(indexes_directory_)) {
        std::cerr << "Please, delete old search_engine_data or choose mode \"search\".\n";
        system("pause");
        exit(EXIT_FAILURE);
    }

    uint64_t block_number = 1;
    uint64_t last_file_id = 1;
    uint64_t last_term_id = 1;

    std::filesystem::create_directories(indexes_directory_);
    std::filesystem::recursive_directory_iterator files_iterator(working_directory_), end;
    std::unordered_map<std::string, uint64_t> terms;

    std::fstream paths_file(indexes_directory_ + "\\.paths", std::ios::out | std::ios::binary);
    std::fstream files_file(indexes_directory_ + "\\.files", std::ios::out | std::ios::binary);
    uint64_t file_pos = 0;
    paths_file.seekp(0);
    files_file.seekp(0);

    while (files_iterator != end) {
        std::unordered_map<uint64_t, std::deque<std::pair<uint64_t, std::deque<uint64_t>>>> block;

        while (block.size() < MaxBlockSize && files_iterator != end) {
            if ((*files_iterator).is_directory()) {
                ++files_iterator;
                continue;
            }

            if ((*files_iterator).path() == indexes_directory_) {
                files_iterator.disable_recursion_pending();
            }

            std::string file_path = (*files_iterator++).path().string();

            uint64_t count_of_terms = ParseFile(block, terms, file_path, last_file_id, last_term_id);
            if (count_of_terms) {
                WriteULLToFile(files_file, count_of_terms);
                WriteULLToFile(files_file, file_pos);

                WriteULLToFile(paths_file, file_path.size());
                paths_file.write(file_path.c_str(), static_cast<int64_t>(file_path.size()));

                file_pos += file_path.size() + 1 + Log256(file_path.size());
                last_file_id++;
            }
        }

        WriteBlock(block, block_number++, last_term_id);
    }

    files_file.close();
    paths_file.close();

    while (block_number != 1) {
        MergeBlocks(block_number--);
    }

    std::deque<uint64_t> terms_pos(last_term_id, 0);
    GetTermsPos(terms_pos, last_term_id - 1);

    std::fstream terms_file(indexes_directory_ + "\\.terms", std::ios::out | std::ios::binary);
    terms_file.seekp(0);

    WriteULLToFile(terms_file, last_file_id);
    WriteULLToFile(terms_file, terms.size());

    for (auto& [term, term_id] : terms) {
        WriteULLToFile(terms_file, term.size());
        terms_file.write(term.c_str(), static_cast<int64_t>(term.size()));
        WriteULLToFile(terms_file, terms_pos[term_id]);
    }

    terms_file.close();
}

uint64_t SearchEngine::ParseFile(std::unordered_map<uint64_t, FilesInfo>& block,
                                 std::unordered_map<std::string, uint64_t>& terms,
                                 const std::string& file_path, uint64_t file_id, uint64_t& last_term_number) {
    std::fstream file(file_path, std::ios::in | std::ios::binary);
    file.seekg(0);

    char temp;
    std::string term;
    uint64_t counter_strings = 0;
    uint64_t counter_terms = 0;

    while (!file.eof()) {
        file.get(temp);
        if (temp < 32 && temp != '\n' && temp != '\t') continue;

        if ((temp == ' ' || temp == '\n' || temp == '\t') && !term.empty()) {
            ++counter_terms;
            auto term_id = last_term_number;

            if (!terms.contains(term)) {
                terms[term] = last_term_number++;
            } else {
                term_id = terms[term];
            }

            if (!block.contains(term_id)) {
                block[term_id] = {{file_id, {counter_strings}}};
            } else if (block[term_id].back().first != file_id) {
                block[term_id].emplace_back(std::pair<uint64_t, std::deque<uint64_t>>{file_id, {counter_strings}});
            } else {
                block[term_id].back().second.emplace_back(counter_strings);
            }

            term = "";
        } else if (temp != ' ' && temp != '\n' && temp != '\t'){
            term += temp;
        }

        if (temp == '\n') {
            ++counter_strings;
        }
    }

    if (!term.empty()) {
        ++counter_terms;
        auto term_id = last_term_number;

        if (!terms.contains(term)) {
            terms[term] = last_term_number++;
        } else {
            term_id = terms[term];
        }

        if (!block.contains(term_id)) {
            block[term_id] = {{file_id, {counter_strings}}};
        } else if (block[term_id].back().first != file_id) {
            block[term_id].back().second = {counter_strings};
        } else {
            block[term_id].back().second.emplace_back(counter_strings);
        }
    }

    file.close();

    return counter_terms;
}

void SearchEngine::WriteBlock(const std::unordered_map<uint64_t, FilesInfo>& block,
                              uint64_t block_number, uint64_t last_term_number) {
    std::fstream block_file(indexes_directory_ + "\\" + std::to_string(block_number) + ".block",
                            std::ios::out | std::ios::binary);
    block_file.seekp(0);

    for (uint64_t term_id = 0; term_id <= last_term_number; ++term_id) {
        if (!block.contains(term_id)) continue;

        const auto& files_info = block.at(term_id);
        WriteULLToFile(block_file, term_id);
        uint64_t files_info_size = 0;
        std::deque<uint64_t> file_info_sizes;

        for (auto& [file_id, occurrences] : files_info) {
            uint64_t file_info_size = 0;
            for (auto& occur : occurrences) {
                file_info_size += 1 + Log256(occur);
            }
            file_info_sizes.emplace_back(file_info_size);

            files_info_size += 1 + Log256(file_id) + 1 + Log256(file_info_size) + file_info_size;
        }
        WriteULLToFile(block_file, files_info_size);

        uint64_t p = 0;
        for (auto& [file_id, occurrences] : files_info) {
            WriteULLToFile(block_file, file_info_sizes[p++]);
            WriteULLToFile(block_file, file_id);

            for (auto occur : occurrences) {
                WriteULLToFile(block_file, occur);
            }
        }
    }

    block_file.close();
}

uint64_t SearchEngine::Log256(uint64_t x) {
    uint64_t res = 0;
    int f = 0;

    while (x) {
        if (x % 256 == 0) {
            f = 1;
        } else {
            f = 0;
        }

        x /= 256;
        ++res;
    }

    return res + f;
}

void SearchEngine::WriteULLToFile(std::fstream& file, uint64_t x) {
    char len = 0;
    char tmp[8] = {0};

    for (int i = 0; i < 8 && x; ++i) {
        tmp[i] = static_cast<char>(x % 256);
        x /= 256;
        ++len;
    }

    file.put(len);
    for (int i = 0; i < len; ++i) {
        file.put(tmp[len - i - 1]);
    }
}

void SearchEngine::MergeBlocks(uint64_t last_block_number) {
    std::fstream new_block(indexes_directory_ + "\\tmp.block", std::ios::out | std::ios::binary);
    std::fstream block_a(indexes_directory_ + "\\" + std::to_string(last_block_number) + ".block",
                         std::ios::in | std::ios::binary);
    std::fstream block_b(indexes_directory_ + "\\" + std::to_string(last_block_number - 1) + ".block",
                         std::ios::in | std::ios::binary);
    new_block.seekp(0);
    block_a.seekg(0);
    block_b.seekg(0);

    uint64_t term_id_a = ReadULLFromFile(block_a);
    uint64_t term_id_b = ReadULLFromFile(block_b);
//38
    while (term_id_a && term_id_b && !block_a.eof() && !block_b.eof()) {
        if (term_id_a < term_id_b) {
            uint64_t files_info_size = ReadULLFromFile(block_a);
            WriteULLToFile(new_block, term_id_a);
            WriteULLToFile(new_block, files_info_size);

            WriteFilesInfo(new_block, block_a, files_info_size);

            term_id_a = ReadULLFromFile(block_a);
        } else if (term_id_b < term_id_a) {
            uint64_t files_info_size = ReadULLFromFile(block_b);
            WriteULLToFile(new_block, term_id_b);
            WriteULLToFile(new_block, files_info_size);

            WriteFilesInfo(new_block, block_b, files_info_size);

            term_id_b = ReadULLFromFile(block_b);
        } else {
            uint64_t files_info_size_a = ReadULLFromFile(block_a);
            uint64_t files_info_size_b = ReadULLFromFile(block_b);
            WriteULLToFile(new_block, term_id_a);
            WriteULLToFile(new_block, files_info_size_a + files_info_size_b);

            WriteFilesInfo(new_block, block_a, files_info_size_a);
            WriteFilesInfo(new_block, block_b, files_info_size_b);

            term_id_a = ReadULLFromFile(block_a);
            term_id_b = ReadULLFromFile(block_b);
        }
    }

    while (term_id_a && !block_a.eof()) {
        uint64_t info_size = ReadULLFromFile(block_a);
        WriteULLToFile(new_block, term_id_a);
        WriteULLToFile(new_block, info_size);

        WriteFilesInfo(new_block, block_a, info_size);

        term_id_a = ReadULLFromFile(block_a);
    }
    while (term_id_b && !block_b.eof()) {
        uint64_t info_size = ReadULLFromFile(block_b);
        WriteULLToFile(new_block, term_id_b);
        WriteULLToFile(new_block, info_size);

        WriteFilesInfo(new_block, block_b, info_size);

        term_id_b = ReadULLFromFile(block_b);
    }

    block_a.close();
    block_b.close();
    new_block.close();

    std::filesystem::remove(indexes_directory_ + "\\" + std::to_string(last_block_number - 1) + ".block");
    std::filesystem::remove(indexes_directory_ + "\\" + std::to_string(last_block_number) + ".block");
    std::filesystem::rename(indexes_directory_ + "\\tmp.block",
                            indexes_directory_ + "\\" + std::to_string(last_block_number - 1) + ".block");
}

uint64_t SearchEngine::ReadULLFromFile(std::fstream& file) {
    uint64_t res = 0;
    char tmp;
    file.get(tmp);
    auto len = static_cast<uint8_t>(tmp);

    for (int i = 0; i < len; ++i) {
        file.get(tmp);
        res += static_cast<uint8_t>(tmp) * (1 << (8 * (len - i - 1)));
    }

    return res;
}

void SearchEngine::WriteFilesInfo(std::fstream& file_out, std::fstream& file_in, uint64_t info_size) {
    char tmp;

    while (info_size--) {
        file_in.get(tmp);
        file_out.put(tmp);
    }
}

void SearchEngine::GetTermsPos(std::deque<uint64_t>& terms_pos, uint64_t last_term_id) {
    std::fstream main_block(main_block_, std::ios::in | std::ios::binary);
    main_block.seekg(0);
    uint64_t term_id = ReadULLFromFile(main_block);
    uint64_t cur_pos = 0;

    while (last_term_id--) {
        terms_pos[term_id] = cur_pos;

        uint64_t info_size = ReadULLFromFile(main_block);
        main_block.seekg(static_cast<int64_t>(info_size), std::ios::cur);
        cur_pos += 1 + Log256(term_id) + 1 + Log256(info_size) + info_size;

        term_id = ReadULLFromFile(main_block);
    }

    main_block.close();
}


void SearchEngine::DoSearch(std::vector<std::string>& request) {
    if (!std::filesystem::exists(indexes_directory_)) {
        std::cerr << "Please, index your directory.\n";
        system("pause");
        exit(EXIT_FAILURE);
    }

    CheckRequest(request);

    std::deque<FileSearchInfo> files_info;
    GetFilesInfo(files_info);

    {
        std::deque<std::deque<std::string>> expanded_request;
        ExpandRequest(request, expanded_request);
        request = {};

        for (auto& i : expanded_request) {
            CountConjunctScore(i, files_info);
        }
    }

    std::sort(files_info.begin(), files_info.end(), FileSearchInfo::Comp);

    files_info = {files_info.begin(), files_info.begin() + std::min(5ull, files_info.size())};

    std::fstream files_paths(indexes_directory_ + + "\\.paths", std::ios::in | std::ios::binary);
    for (auto& file : files_info) {
        if (!file.score) continue;

        files_paths.seekg(static_cast<int64_t>(file.pos));
        uint64_t file_path_size = ReadULLFromFile(files_paths);
        std::string file_path;
        char tmp;

        for (uint64_t i = 0; i < file_path_size; ++i) {
            files_paths.get(tmp);
            file_path += tmp;
        }

        std::cout << file_path << ": \n";
        for (auto& pos : file.term_poss) {
            std::cout << pos + 1 << '\n';
        }

        std::cout << '\n';
    }

    files_paths.close();
}

void SearchEngine::CheckRequest(std::vector<std::string>& request) {
    std::stack<bool> brackets;
    bool is_pref_operation = true;

    if (request[0] == "AND" || request[0] == "OR") {
        std::cerr << "Please, input correct search request without starting with operation.\n";
        system("pause");
        exit(EXIT_FAILURE);
    } else if (request.back() == "AND" || request.back() == "OR") {
        std::cerr << "Please, input correct search request without ending with operation.\n";
        system("pause");
        exit(EXIT_FAILURE);
    }

    for (const auto& s : request) {
        if (s.starts_with('(')) {
            brackets.emplace(true);
        }

        if (s.ends_with(')')) {
            if (brackets.empty()) {
                std::cerr << "Please, input correct search request with opening all brackets.\n";
                system("pause");
                exit(EXIT_FAILURE);
            } else {
                brackets.pop();
            }
        }

        if (s == "And" || s == "ANd" || s == "aNd" || s == "aND" || s == "anD"
            || s == "Or" || s == "oR") {
            std::cerr << "Please, input correct search request with uppercase " + s + ".\n";
            system("pause");
            exit(EXIT_FAILURE);
        } else if (s == "AND" || s == "OR") {
            if (is_pref_operation) {
                std::cerr << "Please, input correct search request without two options one by one.\n";
                system("pause");
                exit(EXIT_FAILURE);
            } else {
                is_pref_operation = true;
            }
        } else if (s != "(" && s != ")") {
            if (!is_pref_operation) {
                std::cerr << "Please, input correct search request with options between terms.\n";
                system("pause");
                exit(EXIT_FAILURE);
            }
            is_pref_operation = false;
        }
    }

    if (!brackets.empty()) {
        std::cerr << "Please, input correct search request with closing all brackets.\n";
        system("pause");
        exit(EXIT_FAILURE);
    }
}

void SearchEngine::ExpandRequest(std::vector<std::string>& request, std::deque<std::deque<std::string>>& result) {
    for (const auto& s : request) {
        if (s == "AND" || s == "OR") {
        } else if (s == "(" && s == ")") {
        } else if (s.starts_with('(')) {
            result.emplace_back(std::deque<std::string>({s.substr(1)}));
        } else if (s.ends_with(')')) {
            result.emplace_back(std::deque<std::string>({s.substr(0, s.size() - 1)}));
        } else {
            result.emplace_back(std::deque<std::string>({s}));
        }
    }
}

void SearchEngine::CountConjunctScore(std::deque<std::string>& conjunct, std::deque<FileSearchInfo>& files_info) {
    FilesInfo files_info_a;
    FilesInfo files_info_b;

    {
        std::deque<uint64_t> terms_pos;
        GetPos(conjunct, terms_pos);
        if (terms_pos.size() < conjunct.size()) {
            return;
        }

        std::fstream block = std::fstream(main_block_, std::ios::in | std::ios::binary);
        for (auto& pos : terms_pos) {
            block.seekg(static_cast<int64_t>(pos));
            ReadULLFromFile(block); // term_id
            uint64_t files_info_size = ReadULLFromFile(block);

            while (files_info_size) {
                uint64_t file_info_size = ReadULLFromFile(block);
                uint64_t file_id = ReadULLFromFile(block);

                files_info_b.emplace_back(std::make_pair(file_id, std::deque<uint64_t>{}));
                files_info_size -= 1 + Log256(file_info_size) + 1 + Log256(file_id) + file_info_size;

                while (file_info_size) {
                    uint64_t pos_in_file = ReadULLFromFile(block);
                    files_info_b.back().second.emplace_back(pos_in_file);

                    file_info_size -= 1 + Log256(pos_in_file);
                }
            }
            if (!files_info_a.empty()) {
                FilesInfo tmp;
                auto p_a = files_info_a.begin();
                auto p_b = files_info_b.begin();

                while (p_a != files_info_a.end() && p_b != files_info_b.end()) {
                    if (p_a->first < p_b->first) {
                        files_info_a.erase(p_a);
                    } else if (p_b->first < p_a->first) {
                        ++p_b;
                    } else {
                        p_a->second.insert(p_a->second.end(), p_b->second.begin(), p_b->second.end());
                        ++p_b;
                        ++p_a;
                    }
                }

                files_info_a.erase(p_a, files_info_a.end());

                files_info_b = {};
            } else {
                files_info_a = files_info_b;
            }
            if (files_info_b.empty()) {
                files_info_a = {};
                return;
            }
        }
        block.close();
    }

    long double num_of_docs = files_info_a.size();
    for (auto& [file_id, term_poss] : files_info_a) {
        long double f = term_poss.size();

        auto score = static_cast<uint64_t>(
                std::log((N_ - num_of_docs + 0.5) / (num_of_docs + 0.5)) * f * (k + 1) /
                (f + k * (1 - b + b * files_info[file_id].num_of_terms / avgdl_))
                );

        files_info[file_id].score += score;
        files_info[file_id].term_poss.insert(files_info[file_id].term_poss.end(), term_poss.begin(), term_poss.end());
    }
}

void SearchEngine::GetPos(std::deque<std::string>& conjuncts, std::deque<uint64_t>& terms_pos) {
    std::unordered_set<std::string> terms(conjuncts.begin(), conjuncts.end());

    std::fstream terms_file = std::fstream(indexes_directory_ + "\\.terms", std::ios::in | std::ios::binary);
    ReadULLFromFile(terms_file);
    uint64_t num_of_terms = ReadULLFromFile(terms_file);

    uint64_t term_size;
    uint64_t term_pos;
    std::string term;
    char tmp;

    while (num_of_terms--) {
        term_size = ReadULLFromFile(terms_file);

        for (uint64_t i = 0; i < term_size; ++i) {
            terms_file.get(tmp);
            term += tmp;
        }

        term_pos = ReadULLFromFile(terms_file);
        if (terms.contains(term)) {
            terms_pos.emplace_back(term_pos);
        }

        term = "";
    }

    terms_file.close();
}

void SearchEngine::GetFilesInfo(std::deque<FileSearchInfo>& files_info) {
    std::fstream terms_file = std::fstream(indexes_directory_ + "\\.terms", std::ios::in | std::ios::binary);
    N_ = ReadULLFromFile(terms_file);
    terms_file.close();

    std::fstream files_file(indexes_directory_ + "\\.files", std::ios::in | std::ios::binary);
    files_info = std::deque<FileSearchInfo>(N_, FileSearchInfo{});

    for (auto& file_info : files_info) {
        file_info.num_of_terms = ReadULLFromFile(files_file);
        file_info.pos = ReadULLFromFile(files_file);

        avgdl_ += file_info.num_of_terms;
    }
    files_file.close();

    avgdl_ /= N_;
}
