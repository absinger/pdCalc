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

#ifndef HYPERBOLIC_LN_PLUGIN_H
#define HYPERBOLIC_LN_PLUGIN_H

#include <string>
#include <memory>
#include "backend/Plugin.h"

class HyperbolicLnPlugin : public pdCalc::Plugin
{
    class HyperbolicLnPluginImpl;
public:
    HyperbolicLnPlugin();
    ~HyperbolicLnPlugin();

    const PluginDescriptor& getPluginDescriptor() const override;
    const PluginButtonDescriptor* getPluginButtonDescriptor() const override;
    pdCalc::Plugin::ApiVersion apiVersion() const;

private:
    std::unique_ptr<HyperbolicLnPluginImpl> pimpl_;
};

extern "C" void* AllocPlugin();
extern "C" void DeallocPlugin(void*);

#endif
