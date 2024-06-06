#include <lib/parser/ArgParser.h>
#include <gtest/gtest.h>


using namespace ArgumentParser;

/*
    Функция принимает в качество аргумента строку, разделяет ее по "пробелу"
    и возвращает вектор полученных слов
*/


TEST(ArgParserTestSuite, EmptyTest) {
    ArgParser parser("My Empty Parser");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StringTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, ShortNameTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('p', "param1");

    ASSERT_TRUE(parser.Parse(SplitString("app -p=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, DefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1").Default("value1");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}


TEST(ArgParserTestSuite, NoDefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_FALSE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StoreValueTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(value, "value1");
}


TEST(ArgParserTestSuite, MultiStringTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);
    parser.AddStringArgument('a', "param2");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1 --param2=value2")));
    ASSERT_EQ(parser.GetStringValue("param2"), "value2");
}


TEST(ArgParserTestSuite, IntTest) {
    ArgParser parser("My Parser");
    parser.AddIntArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=100500")));
    ASSERT_EQ(parser.GetIntValue("param1"), 100500);
}


TEST(ArgParserTestSuite, MultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    parser.AddIntArgument('p', "param1").MultiValue().StoreValues(int_values);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
    ASSERT_EQ(parser.GetIntValue("param1", 0), 1);
    ASSERT_EQ(int_values[1], 2);
    ASSERT_EQ(int_values[2], 3);
}


TEST(ArgParserTestSuite, MinCountMultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    size_t MinArgsCount = 10;
    parser.AddIntArgument('p', "param1").MultiValue(MinArgsCount).StoreValues(int_values);

    ASSERT_FALSE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
}


TEST(ArgParserTestSuite, FlagTest) {
    ArgParser parser("My Parser");
    parser.AddFlag('f', "flag1");

    ASSERT_TRUE(parser.Parse(SplitString("app --flag1")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
}


TEST(ArgParserTestSuite, FlagsTest) {
    ArgParser parser("My Parser");
    bool flag3 ;
    parser.AddFlag('a', "flag1");
    parser.AddFlag('b', "flag2").Default(true);
    parser.AddFlag('c', "flag3").StoreValue(flag3);

    ASSERT_TRUE(parser.Parse(SplitString("app -ac")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
    ASSERT_TRUE(parser.GetFlag("flag2"));
    ASSERT_TRUE(flag3);
}


TEST(ArgParserTestSuite, PositionalArgTest) {
    ArgParser parser("My Parser");
    std::vector<int> values;
    parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

    ASSERT_TRUE(parser.Parse(SplitString("app 1 2 3 4 5")));
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}


TEST(ArgParserTestSuite, HelpTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");

    ASSERT_TRUE(parser.Parse(SplitString("app --help")));
    ASSERT_TRUE(parser.Help());
}


TEST(ArgParserTestSuite, HelpStringTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
    parser.AddFlag('s', "flag1", "Use some logic").Default(true);
    parser.AddFlag('p', "flag2", "Use some logic");
    parser.AddIntArgument("number", "Some Number");


    ASSERT_TRUE(parser.Parse(SplitString("app --help")));
    parser.HelpDescription();
    // Проверка закомментирована намеренно. Ожидается, что результат вызова функции будет приблизительно такой же,
    // но не с точностью до символа

    // ASSERT_EQ(
    //     parser.HelpDescription(),
    //     "My Parser\n"
    //     "Some Description about program\n"
    //     "\n"
    //     "-i,  --input=<string>,  File path for input file [repeated, min args = 1]\n"
    //     "-s,  --flag1,  Use some logic [default = true]\n"
    //     "-p,  --flag2,  Use some logic\n"
    //     "     --number=<int>,  Some Number\n"
    //     "\n"
    //     "-h, --help Display this help and exit\n"
    // );
}

class ParserForArchTestsSuite : public testing::TestWithParam<std::tuple<std::string, bool, bool, bool, bool, bool, bool, std::string, std::vector<std::string>>> {
};

TEST_P(ParserForArchTestsSuite, ParseTest) {
    ArgParser parser_for_arch{"Parser for archiver"};
    parser_for_arch.AddFlag('c', "create", "create of new archive");
    parser_for_arch.AddStringArgument('f', "file", "Path of archive");
    parser_for_arch.AddFlag('l', "list", "Output list of files in archive");
    parser_for_arch.AddFlag('x', "extract", "Extract files from archive");
    parser_for_arch.AddFlag('a', "append", "Append files on archive");
    parser_for_arch.AddFlag('d', "delete", "Delete file from archive");
    parser_for_arch.AddFlag('A', "concatenate", "Merge of two archives");
    parser_for_arch.AddStringArgument("files", "Names of input files").MultiValue().Positional().Default("all");

    ASSERT_TRUE(parser_for_arch.Parse(SplitString(std::get<0>(GetParam()))));

    ASSERT_EQ(parser_for_arch.GetFlag("create"), std::get<1>(GetParam()));
    ASSERT_EQ(parser_for_arch.GetFlag("list"), std::get<2>(GetParam()));
    ASSERT_EQ(parser_for_arch.GetFlag("extract"), std::get<3>(GetParam()));
    ASSERT_EQ(parser_for_arch.GetFlag("append"), std::get<4>(GetParam()));
    ASSERT_EQ(parser_for_arch.GetFlag("delete"), std::get<5>(GetParam()));
    ASSERT_EQ(parser_for_arch.GetFlag("concatenate"), std::get<6>(GetParam()));

    if (!std::get<7>(GetParam()).empty()) {
        ASSERT_EQ(parser_for_arch.GetStringValue("file"), std::get<7>(GetParam()));
    }
    if (!std::get<8>(GetParam())[0].empty()) {
        ASSERT_EQ(parser_for_arch.GetStringMultiValue("files"), std::get<8>(GetParam()));
    }
}

INSTANTIATE_TEST_SUITE_P(
        Group,
        ParserForArchTestsSuite,
        testing::Values(
                // positive
                std::make_tuple(std::string("hamarc --create --file=ARCHIVE FILE1 FILE2 FILE3"),
                                true, false, false, false, false, false, std::string("ARCHIVE"),
                                std::vector<std::string>{"FILE1", "FILE2", "FILE3"}),
                std::make_tuple(std::string("hamarc -l -f=ARCHIVE"),
                                false, true, false, false, false, false, std::string("ARCHIVE"),
                                std::vector<std::string>{""}),
                std::make_tuple(std::string("hamarc --concatenate ARCHIVE1 ARCHIVE2 -f=ARCHIVE3"),
                                false, false, false, false, false, true, std::string("ARCHIVE3"),
                                std::vector<std::string>{"ARCHIVE1", "ARCHIVE2"})
        )
);