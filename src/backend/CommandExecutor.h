// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>
#include <memory>
#include "Command.h"
#include <set>

namespace pdCalc {

class UserInterface;

class CommandExecutor
{
    class CommandExecutorImpl;

public:
    explicit CommandExecutor(UserInterface& ui);
    ~CommandExecutor();

    void commandEntered(const std::string& command);

private:
    CommandExecutor(const CommandExecutor&) = delete;
    CommandExecutor(CommandExecutor&&) = delete;
    CommandExecutor& operator=(const CommandExecutor&) = delete;
    CommandExecutor& operator=(CommandExecutor&&) = delete;


    std::unique_ptr<CommandExecutorImpl> pimpl_;
};

}

#endif
