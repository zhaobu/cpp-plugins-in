// Copyright (c) 2017-2023, University of Cincinnati, developed by Henry
// Schreiner under NSF AWARD 1414736 and by the respective contributors. All
// rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "subcommand.h"

#include "utils/logger.h"

#include <iostream>
#include <memory>

std::shared_ptr<SubcommandAOptions> opt_a;
std::shared_ptr<SubcommandAOptions> opt_b;

/// Set up a subcommand and capture a shared_ptr to a struct that holds all its
/// options. The variables of the struct are bound to the CLI options. We use a
/// shared ptr so that the addresses of the variables remain for binding, You
/// could return the shared pointer if you wanted to access the values in main.
std::shared_ptr<SubcommandAOptions>& setup_subcommand_a(CLI::App& app) {
    // Create the option and subcommand objects.
    auto opt                            = std::make_shared<SubcommandAOptions>();

    auto* sub                           = app.add_subcommand("run", "compress or decompress");

    std::vector<std::string> more_comms = app.remaining();

    // SPDLOG_INFO("Prefix args={}", vals);

    SPDLOG_INFO("a Remaining commands={}", more_comms);

    // Add options to sub, binding them to opt.
    sub->add_option("-f,--file", opt->file, "File name");
    sub->add_flag("--with-foo", opt->with_foo, "Counter");

    // Set the run function as callback to be called when this subcommand is
    // issued. sub->callback([opt]() { run_subcommand_a(*opt); });
    opt_a = opt;
    // sub->parse(app.remaining());
    return opt_a;
}

/// The function that runs our code.
/// This could also simply be in the callback lambda itself,
/// but having a separate function is cleaner.
void run_subcommand_a(std::shared_ptr<SubcommandAOptions> opt) {
    if(opt == nullptr) {
        opt = opt_a;
    }

    // Do stuff...
    std::cout << "run_subcommand_a Working on file: " << opt->file << std::endl;
    if(opt->with_foo) {
        std::cout << "run_subcommand_a Using foo!" << std::endl;
    }
}

std::shared_ptr<SubcommandAOptions>& setup_subcommand_b(CLI::App& app) {
    // Create the option and subcommand objects.
    auto opt  = std::make_shared<SubcommandAOptions>();

    auto* sub = app.add_subcommand("index", "create index file");

    // Add options to sub, binding them to opt.
    sub->add_option("-f,--file", opt->file, "File name");
    sub->add_flag("--with-foo", opt->with_foo, "Counter");
    // sub->parse(app.remaining());

    // Set the run function as callback to be called when this subcommand is
    // issued. sub->callback([opt]() { run_subcommand_b(*opt); });
    opt_b = opt;

    return opt_b;
}

/// The function that runs our code.
/// This could also simply be in the callback lambda itself,
/// but having a separate function is cleaner.
void run_subcommand_b(std::shared_ptr<SubcommandAOptions> opt) {
    // Do stuff...
    std::cout << "run_subcommand_b Working on file: " << opt->file << std::endl;
    if(opt->with_foo) {
        std::cout << "run_subcommand_b Using foo!" << std::endl;
    }
}