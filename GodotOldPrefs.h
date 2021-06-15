/*
The MIT License (MIT)

Copyright (c) 2016-2021 Egor Yakovlev, PigelPix Games, github.com/pigelpixgames

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GODOTOLDPREFS_H
#define GODOTOLDPREFS_H

#include "core/reference.h"

#include <string>
#include <iostream>

#if defined __linux__
#include <base64.hpp>
#include <pugixml.hpp>
#elif defined __APPLE__
#include <boost/any.hpp>
#include <Plist.hpp>
#include <PlistDate.hpp>
#include <base64.hpp>
#include <pugixml.hpp>
#elif defined _WIN32
#include <locale>
#include <codecvt>
#include <registry.h>
#endif

class GodotOldPrefs : public Reference {
    OBJ_TYPE(GodotOldPrefs, Reference);
    
    std::string companyName;
    std::string productName;

protected:
    static void _bind_methods();

public:
    void setCredentials(String company, String product);
    int getInt(String name);
    String getString(String name);
#if defined __APPLE__
    String getFloatAsString(String name);
#endif
    double getFloat(String name);
    bool getBool(String name);
    bool keyExists(String name);

    GodotOldPrefs();
};

#endif
