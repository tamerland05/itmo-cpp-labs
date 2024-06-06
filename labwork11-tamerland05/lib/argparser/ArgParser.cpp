#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& parser_name) : parser_name_(parser_name) {}

ArgParser::~ArgParser() {
    delete helping_;
}

std::string_view ArgParser::GetArgName(const std::string_view& argument, bool& result) {
    size_t pos_eq = argument.find('=');
    size_t pos_def = argument.rfind('-');

    if (pos_eq != std::string::npos && pos_def != std::string::npos) {
        return argument.substr(pos_def + 1, pos_eq - pos_def - 1);
    } else if (pos_def != std::string::npos) {
        return argument.substr(pos_def + 1, argument.size() - pos_def - 1);
    } else {
        result = false;

        return "";
    }
}

std::string_view ArgParser::GetArgValue(const std::string_view& argument, bool& result) {
    size_t pos_eq = argument.find('=');

    if (pos_eq != std::string::npos) {
        return argument.substr(pos_eq + 1, argument.length() - pos_eq - 1);
    } else {
        result = false;

        return "";
    }
}

IntArgument* ArgParser::FindIntArgument(const std::string& arg_name) {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 1 && *arg == arg_name) {
            return reinterpret_cast<IntArgument*>(arg);
        }
    }

    return nullptr;
}

StringArgument* ArgParser::FindStringArgument(const std::string& arg_name) {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 2 && *arg == arg_name) {
            return reinterpret_cast<StringArgument*>(arg);
        }
    }

    return nullptr;
}

Flag* ArgParser::FindFlag(const std::string& arg_name) {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 3 && *arg == arg_name) {
            return reinterpret_cast<Flag*>(arg);
        }
    }

    return nullptr;
}


Helping* ArgParser::FindHelp(const std::string& arg_name) {
    if (helping_ && *helping_ == arg_name) {
        return helping_;
    }

    return nullptr;
}

IntArgument* ArgParser::FindIntPositional() {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 1 && arg->IsPositional()) {
            return reinterpret_cast<IntArgument*>(arg);
        }
    }

    return nullptr;
}

StringArgument* ArgParser::FindStringPositional() {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 2 && arg->IsPositional()) {
            return reinterpret_cast<StringArgument*>(arg);
        }
    }

    return nullptr;
}

IntArgument& ArgParser::AddIntArgument(const std::string& pseud,
                                       const std::string& description) {
    IntArgument* new_int_argument = new IntArgument(' ', pseud, description);
    arguments_.emplace_back(new_int_argument);

    return *new_int_argument;
}

IntArgument& ArgParser::AddIntArgument(char short_pseud,
                                       const std::string& pseud,
                                       const std::string& arg_description) {
    IntArgument* new_int_argument = new IntArgument(short_pseud, pseud, arg_description);
    arguments_.emplace_back(new_int_argument);

    return *new_int_argument;
}

StringArgument& ArgParser::AddStringArgument(const std::string& pseud,
                                             const std::string& arg_description) {
    StringArgument* new_string_argument = new StringArgument(' ', pseud, arg_description);
    arguments_.emplace_back(new_string_argument);

    return *new_string_argument;
}

StringArgument& ArgParser::AddStringArgument(const char& short_pseud,
                                             const std::string& pseud,
                                             const std::string& arg_description) {
    StringArgument* new_string_argument = new StringArgument(short_pseud, pseud, arg_description);
    arguments_.emplace_back(new_string_argument);

    return *new_string_argument;
}

Flag& ArgParser::AddFlag(const std::string& pseud,
                         const std::string& arg_description) {
    Flag* new_flag = new Flag(' ', pseud, arg_description);
    arguments_.emplace_back(new_flag);

    return *new_flag;
}

Flag& ArgParser::AddFlag(const char& short_pseud,
                         const std::string& pseud,
                         const std::string& arg_description) {
    Flag* new_flag = new Flag(short_pseud, pseud, arg_description);
    arguments_.emplace_back(new_flag);

    return *new_flag;
}

Helping& ArgParser::AddHelp(const std::string& pseud,
                            const std::string& arg_description) {
    helping_ = new Helping(' ', pseud, arg_description);

    return *helping_;
}

Helping& ArgParser::AddHelp(const char& short_pseud,
                            const std::string& pseud,
                            const std::string& arg_description) {
    helping_ = new Helping(short_pseud, pseud, arg_description);

    return *helping_;
}

int ArgParser::GetIntValue(const std::string& name_of_param, size_t index) {
    IntArgument* argument = FindIntArgument(name_of_param);

    if (argument->IsMulti()) {
        return argument->GetValue(index);
    } else {
        return argument->GetValue();
    }
}

std::string ArgParser::GetStringValue(const std::string& name_of_param, size_t index) {
    const StringArgument* argument = FindStringArgument(name_of_param);

    if (argument->IsMulti()) {
        return argument->GetValue(index);
    } else {
        return argument->GetValue();
    }
}


bool ArgParser::GetFlag(const std::string& name_of_param) {
    const Flag* argument = FindFlag(name_of_param);

    return argument->GetValue();
}

bool ArgParser::Help() const {
    return helping_;
}

std::string ArgParser::HelpDescription() const {
    std::string arg_descriptions;
    std::string parser_help;
    std::string help;
    std::stringstream result;

    if (helping_) {
        help = helping_->GetDescription("", "", false) + "Display this help and exit";
        parser_help = helping_->GetHelp();
    }

    result << parser_name_ << "\n" << parser_help << "\n\n";

    for (auto& arg : arguments_) {
        if (arg->arg_type == 1) {
            result <<
            arg->GetDescription("int",
                                std::to_string(reinterpret_cast<IntArgument*>(arg)->GetDefault()));
        } else if (arg->arg_type == 2) {
            result <<
            arg->GetDescription("string",
                                reinterpret_cast<StringArgument*>(arg)->GetDefault());

        } else {
            result <<
            arg->GetDescription("flag",
                                std::to_string(reinterpret_cast<Flag*>(arg)->GetDefault()));
        }
    }

    result << "\n" << help;
    return result.str();
}

bool ArgParser::Parse(const std::vector<std::string>& argv) {
    bool result = true;
    bool is_help = false;

    CheckArguments(argv, result, is_help);

    if (!result) {
        return false;
    } else if (!is_help) {
        SetArguments(result);
    } else {
        std::cout << HelpDescription() << '\n';

        system("pause");
        exit(EXIT_SUCCESS);
    }

    return result;
}

void ArgParser::CheckArguments(const std::vector<std::string> &argv, bool& result, bool& is_help) {
    std::string arg_name;
    std::string arg_value;

    for (const auto& argument : argv) {
        if (argument == argv[0]) {
            continue;
        }

        arg_value = GetArgValue(argument, result);
        arg_name = GetArgName(argument, result);

        IntArgument* int_argument = FindIntArgument(arg_name);
        StringArgument* string_argument = FindStringArgument(arg_name);
        Helping* help = FindHelp(arg_name);
        IntArgument* positional_int = FindIntPositional();
        StringArgument* positional_string = FindStringPositional();

        if (help) {
            result = Help();
            is_help = true;
            return;
        }

        if (string_argument) {
            if (!result) {
                std::cerr << std::format("You aren't input the value for string argument {}.\n",
                                         arg_name);
                result = false;
                return;
            } else if (!string_argument->IsIndicated() || string_argument->IsMulti()) {
                string_argument->SetValue(arg_value);
            } else {
                std::cerr << std::format("Non-multi string argument {} has already indicated.\n",
                                         arg_name);
                result = false;
                return;
            }
        } else if (int_argument) {
            if (!result
                || !std::all_of(std::begin(arg_value), std::end(arg_value), isdigit)) {
                std::cerr << std::format("You aren't input the value for int argument {}.\n",
                                         arg_name);
                result = false;
                return;
            } else if (!int_argument->IsIndicated() || int_argument->IsMulti()) {
                int_argument->SetValue(stoi(arg_value));
            } else {
                std::cerr << std::format("Non-multi int argument {} has already indicated.\n",
                                         arg_name);
                result = false;
                return;
            }
        } else if (positional_int &&
                   std::all_of(std::begin(argument), std::end(argument), isdigit)) {
            positional_int->SetValue(stoi(argument));
            result = true;
        } else if (positional_string) {
            positional_string->SetValue(argument);
            result = true;
        } else {
            std::cerr << std::format("Argument {} is not exists.\n", arg_name);
            result = false;
            return;
        }
    }
}

void ArgParser::SetArguments(bool& result) {
    for (auto& arg : arguments_) {
        if (arg->arg_type == 1) {
            IntArgument& argument = *reinterpret_cast<IntArgument*>(arg);
            if (argument.arg_type == 0 || !argument.IsIndicated() ||
                argument.IsMulti() &&
                argument.GetMultiValue().size() < argument.GetMinArguments()) {
                if (argument.IsDefault()) {
                    argument.SetValue(argument.GetDefault());
                } else {
                    std::cerr << std::format("You should input the value for int argument {}.\n",
                                             argument.GetName());
                    result = false;
                    return;
                }

            }
            argument.SetStores();

        } else if (arg->arg_type == 2) {
            StringArgument &argument = *reinterpret_cast<StringArgument *>(arg);
            if (!argument.IsIndicated() ||
                argument.IsMulti() && argument.GetMultiValue().size() < argument.GetMinArguments()) {
                if (argument.IsDefault()) {
                    argument.SetValue(argument.GetDefault());
                } else {
                    std::cerr << std::format("You should input the value for string argument {}.\n",
                                             argument.GetName());
                    result = false;
                    return;
                }
            }

            argument.SetStores();
        } else {
            Flag& argument = *reinterpret_cast<Flag*>(arg);
            if (argument.IsDefault() && argument.GetDefault()) {
                argument.SetValue();
            }

            argument.SetStores();
        }
    }
}
