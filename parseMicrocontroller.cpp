/*
 * This file is part of the umake distribution (https://github.com/nimo-labs/umake).
 * Copyright (c) 2020 Nimolabs Ltd. www.nimo.uk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"
#include <string>
#include <fstream>
#include <iostream>

#include "parseMicrocontroller.h"

using namespace rapidjson;
using namespace std;

static bool isHostLinux(void)
{
#if defined(__linux)
    return 1;
#else
    return 0;
#endif
}

void parseMicrocontroller(rapidjson::Document& uMakefile, std::ofstream& makefile, unsigned char customLinker)
{
    rapidjson::Document microController;
    string tmpString;

    /*Get microcontroller definition*/
    string fn = "./umake/nimolib/uC/uc_";
    fn.append(uMakefile["microcontroller"].GetString());
    if (isHostLinux())
        fn.append("-linux");
    else
        cout << " not linux\n";

    /*This should be a defined in uMakefile*/
    fn.append("-gcc");
    fn.append(".json");

    ifstream ifs(fn);
    IStreamWrapper isw(ifs);
    microController.ParseStream(isw);
    /********************/

    if (!microController.HasMember("microcontroller"))
    {
        cout << "Error " << fn << " is missing microcontroller field\n";
        exit(1);
    }
    tmpString = microController["microcontroller"].GetString();
    if (0 != tmpString.compare(uMakefile["microcontroller"].GetString()))
    {
        cout << "Error " << fn << " incorrect microcontroller defined.\n";
        exit(1);
    }

    if (microController.HasMember("microcontroller"))
    {
        makefile << "# Microcontroller defines\n";
        assert(microController.HasMember("defines"));
        /*Iterate through defines array*/
        const Value &defines = microController["defines"];
        assert(defines.IsArray());
        for (SizeType i = 0; i < defines.Size(); i++) // Uses SizeType instead of size_t
        {
            makefile << "DEFINES += -D" << defines[i].GetString() << endl;
        }
        makefile << endl;

        makefile << "# Microcontroller cflags\n";
        assert(microController.HasMember("cflags"));
        /*Iterate through defines array*/
        const Value &cflags = microController["cflags"];
        assert(cflags.IsArray());
        for (SizeType i = 0; i < cflags.Size(); i++) // Uses SizeType instead of size_t
        {
            makefile << "CFLAGS += " << cflags[i].GetString() << endl;
        }
        makefile << endl;

        makefile << "# Microcontroller ldflags\n";
        assert(microController.HasMember("ldflags"));
        /*Iterate through defines array*/
        const Value &ldflags = microController["ldflags"];
        assert(ldflags.IsArray());
        for (SizeType i = 0; i < ldflags.Size(); i++) // Uses SizeType instead of size_t
        {
            makefile << "LDFLAGS += " << ldflags[i].GetString() << endl;
        }

        if ((microController.HasMember("linkerFile")) && (0 == customLinker))
        {
            assert(microController.HasMember("linkerFile"));
            makefile << "# Linker file\n";
            makefile << "LDFLAGS += -Wl,--script=" << microController["linkerFile"].GetString() << "\n";
        }
        if (microController.HasMember("startupFile"))
        {
            assert(microController.HasMember("startupFile"));
            makefile << "# Startup file\n";
            makefile << "SRCS += " << microController["startupFile"].GetString() << "\n";
        }
        makefile << endl;
    }
}