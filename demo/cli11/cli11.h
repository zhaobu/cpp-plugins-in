// #include "CLI11.hpp"
#ifdef CLI11_SINGLE_FILE
#include "CLI11.hpp"
#else
#include "CLI/CLI.hpp"
#endif

#include "CLI/Timer.hpp"
#include "subcommand/subcommand.h"
#include "utils/logger.h"

#include <iostream>
#include <memory>
#include <string>

using namespace std;

int test1(int argc, char** argv) {
    CLI::App app{ "App description" };
    // Define options
    int      p = 0;
    app.add_option("-p", p, "Parameter");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Parameter value: " << p << std::endl;
    return 0;
}

int test2(int argc, char** argv) {
    CLI::App app{};                         // 软件描述出现在第一行打印
    app.footer("My footer 最后一行打印");   // 最后一行打印
    app.get_formatter()->column_width(40);  // 列的宽度
    app.require_subcommand(1);              // 表示运行命令需要且仅需要一个子命令

    auto sub1 = app.add_subcommand("sub1", "subcommand1");
    auto sub2 = app.add_subcommand("sub2", "subcommand1");
    sub1->fallthrough();  // 当出现的参数子命令解析不了时,返回上一级尝试解析
    sub2->fallthrough();

    // 定义需要用到的参数
    string      filename;
    int         threads = 10;
    int         mode    = 0;
    vector<int> barcodes;
    bool        reverse = false;
    string      outPath;

    if(sub1) {
        // 第一个参数不加-, 表示位置参数,位置参数按出现的顺序来解析
        // 这里还检查了文件是否存在,已经是必须参数
        sub1->add_option("file", filename, "Position paramter")->check(CLI::ExistingFile)->required();

        // 检查参数必须大于0
        sub1->add_option("-n,-N", threads, "Set thread number")->check(CLI::PositiveNumber);
    }
    if(sub2) {
        // 设置范围
        sub2->add_option("-e,-E", mode, "Set mode")->check(CLI::Range(0, 3));
        // 将数据放到vector中,并限制可接受的长度
        sub2->add_option("-b", barcodes, "Barcodes info:start,len,mismatch")->expected(3, 6);
    }
    // 添加flag,有就是true
    app.add_flag("-r,-R", reverse, "Apply reverse");
    // 检查目录是否存在
    app.add_option("-o", outPath, "Output path")->check(CLI::ExistingDirectory);

    CLI11_PARSE(app, argc, argv);

    // 判断哪个子命令被使用
    if(sub1->parsed()) {
        cout << "Got sub1" << endl;
        cout << "filename:" << filename << endl;
        cout << "threads:" << threads << endl;
    } else if(sub2->parsed()) {
        cout << "Got sub2" << endl;
        cout << "mode:" << mode << endl;
        cout << "barcodes:";
        for(auto& b : barcodes)
            cout << b << " ";
        cout << endl;
    }
    cout << endl << "Comman paras" << endl;
    cout << "reverse:" << reverse << endl;
    cout << "outPath:" << outPath << endl;

    return 0;
}

int test3(int argc, char** argv) {
    CLI::App app("K3Pi goofit fitter");
    app.set_help_all_flag("--help-all", "Expand all help");
    app.add_flag("--random", "Some random flag");
    CLI::App* start = app.add_subcommand("start", "A great subcommand");
    CLI::App* stop  = app.add_subcommand("stop", "Do you really want to stop?");
    app.require_subcommand(1);  // 1 or more

    std::string file;
    start->add_option("-f,--file", file, "File name");

    CLI::Option* s = stop->add_flag("-c,--count", "Counter");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Working on --file from start: " << file << std::endl;
    std::cout << "Working on --count from stop: " << s->count() << ", direct count: " << stop->count("--count")
              << std::endl;
    std::cout << "Count of --random flag: " << app.count("--random") << std::endl;
    for(auto* subcom : app.get_subcommands())
        std::cout << "Subcommand: " << subcom->get_name() << std::endl;

    return 0;
}

int test4(int argc, char** argv) {
    CLI::AutoTimer give_me_a_name("This is a timer");

    CLI::App app("K3Pi goofit fitter");
    app.set_help_all_flag("--help-all", "Expand all help");
    app.require_subcommand(1);  // 1 or more

    CLI::App_p   impOpt = std::make_shared<CLI::App>("Important", "do some important things");
    std::string  file;
    CLI::Option* opt = impOpt->add_option("-f,--file,file", file, "File name")->required();

    int          count{ 0 };
    CLI::Option* copt   = impOpt->add_flag("-c,--count", count, "Counter")->required();

    CLI::App_p otherOpt = std::make_shared<CLI::App>("Other", "do some other things");
    double     value{ 0.0 };  // = 3.14;
    otherOpt->add_option("-d,--double", value, "Some Value");

    // add the subapps to the main one
    app.add_subcommand(impOpt);
    app.add_subcommand(otherOpt);

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        return app.exit(e);
    }

    std::cout << "Working on file: " << file << ", direct count: " << impOpt->count("--file")
              << ", opt count: " << opt->count() << std::endl;
    std::cout << "Working on count: " << count << ", direct count: " << impOpt->count("--count")
              << ", opt count: " << copt->count() << std::endl;
    std::cout << "Some value: " << value << std::endl;

    return 0;
}

int test5(int argc, char** argv) {
    CLI::AutoTimer give_me_a_name("This is a timer");

    CLI::App app("K3Pi goofit fitter");

    std::string  file;
    CLI::Option* opt = app.add_option("-f,--file,file", file, "File name")->required()->group("Important");

    int          count{ 0 };
    CLI::Option* copt = app.add_flag("-c,--count", count, "Counter")->required()->group("Important");

    double value{ 0.0 };  // = 3.14;
    app.add_option("-d,--double", value, "Some Value")->group("Other");

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        return app.exit(e);
    }

    std::cout << "Working on file: " << file << ", direct count: " << app.count("--file")
              << ", opt count: " << opt->count() << std::endl;
    std::cout << "Working on count: " << count << ", direct count: " << app.count("--count")
              << ", opt count: " << copt->count() << std::endl;
    std::cout << "Some value: " << value << std::endl;

    return 0;
}

int test6(int argc, char** argv) {

    CLI::App app("data output specification");
    app.set_help_all_flag("--help-all", "Expand all help");

    auto* format = app.add_option_group("output_format", "formatting type for output");
    auto* target = app.add_option_group("output target", "target location for the output");
    bool  csv{ false };
    bool  human{ false };
    bool  binary{ false };
    format->add_flag("--csv", csv, "specify the output in csv format");
    format->add_flag("--human", human, "specify the output in human readable text format");
    format->add_flag("--binary", binary, "specify the output in binary format");
    // require one of the options to be selected
    format->require_option(1);
    std::string fileLoc;
    std::string networkAddress;
    target->add_option("-o,--file", fileLoc, "specify the file location of the output");
    target->add_option("--address", networkAddress, "specify a network address to send the file");

    // require at most one of the target options
    target->require_option(0, 1);
    CLI11_PARSE(app, argc, argv);

    std::string format_type = (csv) ? std::string("CSV") : ((human) ? "human readable" : "binary");
    std::cout << "Selected " << format_type << " format" << std::endl;
    if(!fileLoc.empty()) {
        std::cout << " sent to file " << fileLoc << std::endl;
    } else if(!networkAddress.empty()) {
        std::cout << " sent over network to " << networkAddress << std::endl;
    } else {
        std::cout << " sent to std::cout" << std::endl;
    }

    return 0;
}

enum class Level : int { High, Medium, Low };

int test7(int argc, char** argv) {

    CLI::App app;

    Level                        level{ Level::Low };
    // specify string->value mappings
    std::map<std::string, Level> map{ { "high", Level::High }, { "medium", Level::Medium }, { "low", Level::Low } };
    // CheckedTransformer translates and checks whether the results are either in one of the strings or in one of the
    // translations already
    app.add_option("-l,--level", level, "Level settings")
        ->required()
        ->transform(CLI::CheckedTransformer(map, CLI::ignore_case));

    CLI11_PARSE(app, argc, argv);

    // CLI11's built in enum streaming can be used outside CLI11 like this:
    using CLI::enums::operator<<;
    std::cout << "Enum received: " << level << std::endl;

    return 0;
}

class MyFormatter : public CLI::Formatter {
public:
    MyFormatter() : Formatter() {}
    std::string make_option_opts(const CLI::Option*) const override {
        return " OPTION";
    }

    std::string make_option_usage(const CLI::Option* opt) const override {
        std::cout << "opt name =" << opt->get_name();
        return "load index to share memory Example:    ./SwiftDuck shm -S -i ./hg19_index.bin";
    }
};

int test8(int argc, char** argv) {
    CLI::App app;
    app.set_help_all_flag("--help-all", "Show all help");
    auto fmt = std::make_shared<MyFormatter>();
    fmt->column_width(15);
    app.formatter(fmt);

    app.add_flag("--flag", "This is a flag");

    auto* sub1 = app.add_subcommand("one", "Description One");
    sub1->add_flag("--oneflag", "Some flag");
    auto* sub2 = app.add_subcommand("two", "Description Two");
    sub2->add_flag("--twoflag", "Some other flag");

    CLI11_PARSE(app, argc, argv);

    std::cout << "This app was meant to show off the formatter, run with -h" << std::endl;

    return 0;
}

int test9(int argc, char** argv) {

    CLI::App app("Prefix command app");
    app.prefix_command();

    std::vector<int> vals;
    app.add_option("--vals,-v", vals)->expected(-1);

    CLI11_PARSE(app, argc, argv);

    std::vector<std::string> more_comms = app.remaining();

    std::cout << "Prefix";
    for(int v : vals)
        std::cout << ": " << v << " ";

    std::cout << std::endl << "Remaining commands: ";

    for(const auto& com : more_comms)
        std::cout << com << " ";
    std::cout << std::endl;

    return 0;
}

int test10(int argc, char** argv) {
    CLI::App app("callback_passthrough");
    app.allow_extras();
    std::string argName;
    std::string val;
    app.add_option("--argname", argName, "the name of the custom command line argument");
    app.callback([&app, &val, &argName]() {
        if(!argName.empty()) {
            CLI::App subApp;
            subApp.add_option("--" + argName, val, "custom argument option");
            subApp.parse(app.remaining_for_passthrough());
        }
    });

    // ./cli11.out --argname foo --foo bar
    CLI11_PARSE(app, argc, argv);
    std::cout << "the value is now " << val << '\n';
    return 0;
}

int test11(int argc, char** argv) {
    // 解析参数和执行命令分离
    CLI::App app("Prefix command app");
    app.set_help_all_flag("--help-all", "Expand all help");

    app.prefix_command();
    // app.allow_extras();

    auto opt_a = setup_subcommand_a(app);
    auto opt_b = setup_subcommand_b(app);

    app.require_subcommand();

    CLI11_PARSE(app, argc, argv);

    std::vector<std::string> more_comms = app.remaining();

    // SPDLOG_INFO("Prefix args={}", vals);

    SPDLOG_INFO("Remaining commands={}", more_comms);

    // Call the setup functions for the subcommands.
    // They are kept alive by a shared pointer in the
    // lambda function held by CLI11

    for(auto* subcom : app.get_subcommands()) {
        auto sub_name = subcom->get_name();
        SPDLOG_INFO("subcmd {} execute", sub_name);
        if(sub_name.compare("run") == 0) {
            run_subcommand_a(opt_a);
        } else if(sub_name.compare("index") == 0) {
            run_subcommand_b(opt_b);
        }
    }

    return 0;
}