#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include <string>
#include <unordered_map>
#include <deque>
#include <vector>
#include <stack>
#include <bits/stdc++.h>
#include <cmath>

const uint64_t MaxBlockSize = 1 << 16;
const long double k = 2;
const long double b = 0.75;

typedef std::deque<std::pair<uint64_t, std::deque<uint64_t>>> FilesInfo;

struct Conjunction {

    std::vector<Conjunction> operands_;

    explicit Conjunction(std::vector<Conjunction>& operands) : operands_(std::move(operands)) {};

    Conjunction(const Conjunction& copy) : operands_(copy.operands_) {};

    Conjunction operator*(const Conjunction& rhs) {
        std::vector<Conjunction> result;

        for (auto& i : operands_) {
            for (auto& j : rhs.operands_) {
                Conjunction a = i * j;
                result.emplace_back(a);
            }
        }

        return Conjunction(result);
    }
};

struct Term {
    uint64_t frequency_{};
    uint64_t documents_{};
};

struct FileSearchInfo {
    uint64_t pos{};
    uint64_t num_of_terms{};
    uint64_t score{};

    std::deque<uint64_t> term_poss{};

    static bool Comp(FileSearchInfo& a, FileSearchInfo& b) {
        return a.score > b.score;
    }
};

class SearchEngine {
private:
    std::string working_directory_;
    std::string indexes_directory_;
    std::string main_block_;
    uint64_t N_{};
    long double avgdl_{};

    static uint64_t ParseFile(std::unordered_map<uint64_t, FilesInfo>&,
                              std::unordered_map<std::string, uint64_t>&,
                              const std::string&, uint64_t, uint64_t&);

    void WriteBlock(const std::unordered_map<uint64_t, FilesInfo>&, uint64_t, uint64_t);

    static uint64_t Log256(uint64_t x);

    static void WriteULLToFile(std::fstream&, uint64_t x);

    static uint64_t ReadULLFromFile(std::fstream&);

    static void WriteFilesInfo(std::fstream&, std::fstream&, uint64_t);

    void MergeBlocks(uint64_t);

    void GetTermsPos(std::deque<uint64_t>&, uint64_t);

    static void CheckRequest(std::vector<std::string>& request);

    static void ExpandRequest(std::vector<std::string>& request, std::deque<std::deque<std::string>>& result);

    void CountConjunctScore(std::deque<std::string>&, std::deque<FileSearchInfo>&);

    void GetPos(std::deque<std::string>&, std::deque<uint64_t>&);

    void GetFilesInfo(std::deque<FileSearchInfo>&);

public:
    explicit SearchEngine(std::string);

    void DoIndex();

    void DoSearch(std::vector<std::string>&);
};
