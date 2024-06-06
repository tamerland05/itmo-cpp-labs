#pragma once

#include "Arguments.h"

#include <memory>
#include <algorithm>
#include <iostream>

namespace ArgumentParser {
class ArgParser {
public:
    explicit ArgParser(const std::string& parser_name);
    ~ArgParser();

    static std::string_view GetArgName(const std::string_view& argument, bool& result);
    static std::string_view GetArgValue(const std::string_view& argument, bool& result);
    IntArgument* FindIntArgument(const std::string& arg_name);
    StringArgument* FindStringArgument(const std::string& arg_name);
    Flag* FindFlag(const std::string& arg_name);
    Helping* FindHelp(const std::string& arg_name);
    IntArgument* FindIntPositional();
    StringArgument* FindStringPositional();

    IntArgument& AddIntArgument(const std::string& pseud,
                                const std::string& arg_description="Simple int argument");
    IntArgument& AddIntArgument(char short_pseud,
                                const std::string& pseud,
                                const std::string& arg_description="Simple int argument");
    StringArgument& AddStringArgument(const std::string& pseud,
                                      const std::string& arg_description="Simple string argument");
    StringArgument& AddStringArgument(const char& short_pseud,
                                      const std::string& pseud,
                                      const std::string& arg_description="Simple string argument");
    Flag& AddFlag(const std::string& pseud,
                  const std::string& arg_description="Simple flag");
    Flag& AddFlag(const char& short_pseud,
                  const std::string& pseud,
                  const std::string& arg_description="Simple flag");
    Helping& AddHelp(const std::string& pseud,
                     const std::string& arg_description="Simple help");
    Helping& AddHelp(const char& short_pseud,
                     const std::string& pseud,
                     const std::string& arg_description="Simple help");

    [[nodiscard]] int GetIntValue(const std::string& name_of_param, size_t index=0);
    [[nodiscard]] std::string GetStringValue(const std::string& name_of_param, size_t index=0);
    [[nodiscard]] bool GetFlag(const std::string& name_of_param);
    [[nodiscard]] bool Help() const;
    [[nodiscard]] std::string HelpDescription() const;
    std::vector<std::string> SplitString(const std::string& str);


    bool Parse(const std::vector<std::string>& argv);
    void CheckArguments(const std::vector<std::string>& argv, bool& result, bool& is_help);
    void SetArguments(bool& result);

private:
    std::string parser_name_{};

    std::vector<Argument*> arguments_{};
    Helping* helping_{};
};

} // namespace ArgumentParser
