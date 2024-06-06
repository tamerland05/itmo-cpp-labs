#pragma once

#include <vector>
#include <string>
#include <format>
#include <sstream>

class Argument {
public:
    bool operator==(const std::string& check_value) const;

    [[nodiscard]] std::string GetDescription(const std::string& type,
                                             const std::string& default_value,
                                             bool is_des=true) const;
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] size_t GetMinArguments() const;
    [[nodiscard]] bool IsIndicated() const;
    [[nodiscard]] bool IsDefault() const;
    [[nodiscard]] bool IsMulti() const;
    [[nodiscard]] bool IsPositional() const;
    short arg_type{};

protected:
    std::pair<char, std::string> name_{};
    std::string description_{};

    size_t min_arguments_count_{};
    bool is_indicated_{};
    bool is_default_{};
    bool is_multi_{};
    bool is_positional_{};
};

class IntArgument : public Argument {
public:
    explicit IntArgument(const char& short_pseud,
                         const std::string& pseud,
                         const std::string& new_description);

    IntArgument& SetValue(const int& new_value);
    IntArgument& StoreValue(int& copy);
    IntArgument& StoreValues(std::vector<int>& multi_copy);
    IntArgument& MultiValue(const size_t& min_args_count=1);
    IntArgument& Default(const int& def_value);
    IntArgument& Positional();

    [[nodiscard]] int GetValue(const size_t& index=0) const;
    [[nodiscard]] int GetDefault() const;
    [[nodiscard]] std::vector<int> GetMultiValue() const;
    void SetStores() const;

private:
    int value_{};
    int default_value_{};
    std::vector<int> multi_value_{};

    std::vector<int*> store_values_{};
    std::vector<std::vector<int>*> store_multi_values_{};
};

class StringArgument : public Argument {
public:
    explicit StringArgument(const char& short_pseud,
                            const std::string& pseud,
                            const std::string& new_description);

    StringArgument& SetValue(const std::string& new_value);
    StringArgument& StoreValue(std::string& copy);
    StringArgument& StoreValues(std::vector<std::string>& multi_copy);
    StringArgument& MultiValue(const size_t& min_args_count=1);
    StringArgument& Default(const std::string& def_value);
    StringArgument& Positional();

    [[nodiscard]] std::string GetValue(const size_t& index=0) const;
    [[nodiscard]] std::string GetDefault() const;
    [[nodiscard]] std::vector<std::string> GetMultiValue() const;
    void SetStores() const;

private:
    std::string value_{};
    std::string default_value_{};
    std::vector<std::string> multi_value_{};

    std::vector<std::string*> store_values_{};
    std::vector<std::vector<std::string>*> store_multi_values_{};
};

class Flag : public Argument {
public:
    explicit Flag(const char& short_pseud,
                  const std::string& pseud,
                  const std::string& new_description);

    Flag& SetValue();
    Flag& StoreValue(bool& copy);
    Flag& Default(const bool& def_value);

    [[nodiscard]] bool GetValue() const;
    [[nodiscard]] bool GetDefault() const;
    void SetStores() const;

private:
    bool value_{};
    bool default_value_{};

    std::vector<bool*> store_values_{};
};

class Helping : public Argument {
public:
    explicit Helping(const char& short_pseud,
                     const std::string& pseud,
                     const std::string& new_description);

    [[nodiscard]] const std::string& GetHelp() const;
};

