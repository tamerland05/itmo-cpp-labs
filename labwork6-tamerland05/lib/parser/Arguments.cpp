#include "Arguments.h"
#include <format>

bool Argument::operator==(const std::string& check_value) const {
    return check_value.length() == 1 && check_value[0] == name_.first
           || check_value == name_.second;
}

std::string Argument::GetDescription(const std::string& type,
                                     const std::string& default_value,
                                     bool is_des) const {
    std::string short_pseud_des = "   ";
    std::string pseud_des = name_.second;
    std::string opt;
    std::string arg_description;
    std::string last_endl;
    std::stringstream result;

    if (name_.first != ' ') {
        short_pseud_des = std::format("-{},", name_.first);
    }
    result << short_pseud_des;

    if (!type.empty()) {
        pseud_des += std::format("=<{}>", type);
    }
    result << pseud_des << ",";

    if (is_default_) {
        opt += std::format("default = {}", default_value);
    }
    if (is_multi_) {
        if (!opt.empty()) {
            opt += ", ";
        }
        opt += std::format("repeated, min args = {}", min_arguments_count_);
    }
    if (is_positional_) {
        if (!opt.empty()) {
            opt += ", ";
        }
        opt += "positional";
    }
    if (!opt.empty()) {
        opt = std::format("[{}]", opt);
    }
    result << opt;

    if (is_des) {
        arg_description = description_;
        last_endl = "\n";
    }
    result << last_endl;

    return result.str();
}

std::string Argument::GetName() const {
    return name_.second;
}

size_t Argument::GetMinArguments() const {
    return min_arguments_count_;
}

bool Argument::IsIndicated() const {
    return is_indicated_;
}

bool Argument::IsDefault() const {
    return is_default_;
}

bool Argument::IsMulti() const {
    return is_multi_;
}

bool Argument::IsPositional() const {
    return is_positional_;
}


IntArgument& IntArgument::SetValue(const int& new_value) {
    if (is_multi_) {
        multi_value_.emplace_back(new_value);
    } else {
        value_ = new_value;
    }
    is_indicated_ = true;

    return *this;
}

IntArgument::IntArgument(const char& short_pseud,
                         const std::string& pseud,
                         const std::string& new_description) {
    name_.first = short_pseud;
    name_.second = pseud;
    description_ = new_description;
    arg_type = 1;
}

IntArgument& IntArgument::StoreValue(int& copy) {
    store_values_.emplace_back(&copy);

    return *this;
}

IntArgument& IntArgument::StoreValues(std::vector<int>& multi_copy) {
    store_multi_values_.emplace_back(&multi_copy);

    return *this;
}

IntArgument& IntArgument::MultiValue(const size_t& min_args_count) {
    is_multi_ = true;
    min_arguments_count_ = min_args_count;

    return *this;
}

IntArgument& IntArgument::Default(const int& def_value) {
    default_value_ = def_value;
    is_default_ = true;

    return *this;
}

IntArgument& IntArgument::Positional() {
    is_positional_ = true;

    return *this;
}

int IntArgument::GetValue(const size_t& index) const {
    if (is_multi_) {
        return multi_value_[index];
    } else {
        return value_;
    }
}

std::vector<int> IntArgument::GetMultiValue() const {
    return multi_value_;
}

int IntArgument::GetDefault() const {
    return default_value_;
}

void IntArgument::SetStores() const {
    if (is_multi_) {
        for (auto& store : store_multi_values_) {
            *store = multi_value_;
        }
    } else {
        for (auto& store : store_values_) {
            *store = value_;
        }
    }
}

StringArgument::StringArgument(const char& short_pseud,
                               const std::string& pseud,
                               const std::string& new_description) {
    name_.first = short_pseud;
    name_.second = pseud;
    description_ = new_description;
    arg_type = 2;
}

StringArgument& StringArgument::SetValue(const std::string& new_value) {
    if (is_multi_) {
        multi_value_.emplace_back(new_value);
    } else {
        value_ = new_value;
    }
    is_indicated_ = true;

    return *this;
}

StringArgument& StringArgument::StoreValue(std::string& copy) {
    store_values_.emplace_back(&copy);

    return *this;
}

StringArgument& StringArgument::StoreValues(std::vector<std::string>& multi_copy) {
    store_multi_values_.emplace_back(&multi_copy);

    return *this;
}

StringArgument& StringArgument::MultiValue(const size_t& min_args_count) {
    is_multi_ = true;
    min_arguments_count_ = min_args_count;

    return *this;
}

StringArgument& StringArgument::Default(const std::string& def_value) {
    default_value_ = def_value;
    is_default_ = true;

    return *this;
}

std::string StringArgument::GetValue(const size_t& index) const {
    if (is_multi_) {
        return multi_value_[index];
    } else {
        return value_;
    }
}

std::string StringArgument::GetDefault() const {
    return default_value_;
}

std::vector<std::string> StringArgument::GetMultiValue() const {
    return multi_value_;
}

void StringArgument::SetStores() const {
    if (is_multi_) {
        for (auto& store : store_multi_values_) {
            *store = multi_value_;
        }
    } else {
        for (auto& store : store_values_) {
            *store = value_;
        }
    }
}

StringArgument& StringArgument::Positional() {
    is_positional_ = true;

    return *this;
}

Flag::Flag(const char& short_pseud,
           const std::string& pseud,
           const std::string& new_description) {
    name_.first = short_pseud;
    name_.second = pseud;
    description_ = new_description;
    arg_type = 3;
}

Flag& Flag::SetValue() {
    value_ = true;

    return *this;
}

Flag& Flag::StoreValue(bool& copy) {
    store_values_.emplace_back(&copy);

    return *this;
}

Flag& Flag::Default(const bool& def_value) {
    default_value_ = def_value;
    is_default_ = true;

    return *this;
}

bool Flag::GetValue() const {
    return value_;
}

bool Flag::GetDefault() const {
    return default_value_;
}

void Flag::SetStores() const {
    for (auto& store : store_values_) {
        *store = value_;
    }
}


Helping::Helping(const char& short_pseud, const std::string& pseud, const std::string& new_description) {
    name_.first = short_pseud;
    name_.second = pseud;
    description_ = new_description;
}

const std::string& Helping::GetHelp() const{
    return description_;
}

