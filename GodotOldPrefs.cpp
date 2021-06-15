/*
The MIT License (MIT)

Copyright (c) 2016-2021 Egor Yakovlev, PigelPix Games, github.com/pigelpixgames (@pigelpixgames)

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

#include "GodotOldPrefs.h"

#include "core/os/os.h"
#include "core/os/file_access.h"

#if defined _WIN32
#include <registry.cpp>
#include <base64.cpp>
#elif defined __APPLE__
#include <Plist.cpp>
#include <PlistDate.cpp>
#include <pugixml.cpp>
#elif defined __linux__
#include <pugixml.cpp>
#include <base64.cpp>
#endif

void GodotOldPrefs::setCredentials(String company, String product) {
    companyName = company.utf8().get_data();
    productName = product.utf8().get_data();
}

int GodotOldPrefs::getInt(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
#if defined __linux__
        pugi::xml_document doc;
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName + "/prefs";
        std::string fpathth = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName;
        if (FileAccess::exists(String(fpathth.c_str())) == false) {
            if (FileAccess::exists(String(fpath.c_str())) == false) {
                return 0;
            }
        } else {
            fpath = fpathth;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        pugi::xml_parse_result result = doc.load((const char *)filedtb.ptr());
        if (!result) {
            return 0;
        }
        for (pugi::xml_node pref: doc.first_child().children("pref")) {
            if ((std::string)pref.attribute("name").value() == namepref && (std::string)pref.attribute("type").value() == "int") {
                return pref.text().as_int();
            }
        }
#elif defined _WIN32
        std::string path = "Software\\" + companyName + "\\" + productName;
        for (std::string propname: winreg::enumerateProperties(HKEY_CURRENT_USER, path, KEY_WOW64_64KEY)) {
            std::string proppattern = namepref + "_h";
            if (propname.substr(0, proppattern.size()) == proppattern && (propname.size() - proppattern.size()) == 10) {
                return winreg::getDword(HKEY_CURRENT_USER, path, propname, KEY_WOW64_64KEY);
            }
        }
#elif defined __APPLE__
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return 0;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        if (dict.find(namepref) != dict.end()) {
            return boost::any_cast<const int64_t&>(dict.find(namepref)->second);
        }
#endif
    }
    return 0;
}

String GodotOldPrefs::getString(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
#if defined __linux__
        pugi::xml_document doc;
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName + "/prefs";
        std::string fpathth = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName;
        if (FileAccess::exists(String(fpathth.c_str())) == false) {
            if (FileAccess::exists(String(fpath.c_str())) == false) {
                return String("");
            }
        } else {
            fpath = fpathth;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        pugi::xml_parse_result result = doc.load((const char *)filedtb.ptr());
        if (!result) {
            return String("");
        }
        for (pugi::xml_node pref: doc.first_child().children("pref")) {
            if ((std::string)pref.attribute("name").value() == namepref && (std::string)pref.attribute("type").value() == "string") {
                return String(b64decode(pref.text().get()).c_str());
            }
        }
#elif defined _WIN32
        std::string path = "Software\\" + companyName + "\\" + productName;
        for (std::string propname: winreg::enumerateProperties(HKEY_CURRENT_USER, path, KEY_WOW64_64KEY)) {
            std::string proppattern = namepref + "_h";
            if (propname.substr(0, proppattern.size()) == proppattern && (propname.size() - proppattern.size()) == 10) {
                return String(winreg::getString(HKEY_CURRENT_USER, path, propname, "0", KEY_WOW64_64KEY).c_str());
            }
        }
#elif defined __APPLE__
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return String("");
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        if (dict.find(namepref) != dict.end()) {
            return String(boost::any_cast<const std::string&>(dict.find(namepref)->second).c_str());
        }
#endif
    }
    return String("");
}

double GodotOldPrefs::getFloat(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
#if defined __linux__
        pugi::xml_document doc;
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName + "/prefs";
        std::string fpathth = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName;
        if (FileAccess::exists(String(fpathth.c_str())) == false) {
            if (FileAccess::exists(String(fpath.c_str())) == false) {
                return 0;
            }
        } else {
            fpath = fpathth;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        pugi::xml_parse_result result = doc.load((const char *)filedtb.ptr());
        if (!result) {
            return 0;
        }
        for (pugi::xml_node pref: doc.first_child().children("pref")) {
            if ((std::string)pref.attribute("name").value() == namepref && (std::string)pref.attribute("type").value() == "float") {
                return pref.text().as_double();
            }
        }
#elif defined _WIN32
        std::string path = "Software\\" + companyName + "\\" + productName;
        for (std::string propname: winreg::enumerateProperties(HKEY_CURRENT_USER, path, KEY_WOW64_64KEY)) {
            std::string proppattern = namepref + "_h";
            if (propname.substr(0, proppattern.size()) == proppattern && (propname.size() - proppattern.size()) == 10) {
                double retdig;
                unsigned long type;
                std::string regdata = b64decode(winreg::getAsBase64ByteArray(HKEY_CURRENT_USER, path, propname, "0", type, KEY_WOW64_64KEY));
                memcpy(&retdig, regdata.c_str(), 8);
                return retdig;
            }
        }
#elif defined __APPLE__
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return 0;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        if (dict.find(namepref) != dict.end()) {
            return boost::any_cast<const double&>(dict.find(namepref)->second);
        }
#endif
    }
    return 0;
}

#if defined __APPLE__
String GodotOldPrefs::getFloatAsString(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return String("0");
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        std::vector<char> filedata;
        Plist::writePlistXML(filedata, dict);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load((const char *)filedata.data());
        if (!result) {
            return String("0");
        }
        for (pugi::xml_node pref: doc.child("plist").child("dict").children()) {
            if ((std::string)pref.name() == "key" && (std::string)pref.text().get() == namepref && (std::string)pref.next_sibling().name() == "real") {
                return String(pref.next_sibling().text().get());
            }
        }
    }
    return String("0");
}
#endif

bool GodotOldPrefs::getBool(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
#if defined __linux__
        pugi::xml_document doc;
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName + "/prefs";
        std::string fpathth = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName;
        if (FileAccess::exists(String(fpathth.c_str())) == false) {
            if (FileAccess::exists(String(fpath.c_str())) == false) {
                return false;
            }
        } else {
            fpath = fpathth;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        pugi::xml_parse_result result = doc.load((const char *)filedtb.ptr());
        if (!result) {
            return false;
        }
        for (pugi::xml_node pref: doc.first_child().children("pref")) {
            if ((std::string)pref.attribute("name").value() == namepref) {
                if ((std::string)pref.attribute("type").value() == "int") {
                     if (pref.text().as_int() > 0) {
                         return true;
                     }
                }
                if ((std::string)pref.attribute("type").value() == "bool") {
                    return pref.text().as_bool();
                }
            }
        }
#elif defined _WIN32
        std::string path = "Software\\" + companyName + "\\" + productName;
        for (std::string propname: winreg::enumerateProperties(HKEY_CURRENT_USER, path, KEY_WOW64_64KEY)) {
            std::string proppattern = namepref + "_h";
            if (propname.substr(0, proppattern.size()) == proppattern && (propname.size() - proppattern.size()) == 10) {
                if (winreg::getDword(HKEY_CURRENT_USER, path, propname, KEY_WOW64_64KEY) > 0) {
                    return true;
                }
            }
        }
#elif defined __APPLE__
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return false;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        if (dict.find(namepref) != dict.end()) {
            return boost::any_cast<const bool&>(dict.find(namepref)->second);
        }
#endif
    }
    return false;
}

bool GodotOldPrefs::keyExists(String name) {
    std::string namepref = name.utf8().get_data();
    if (companyName != "" && productName != "") {
#if defined __linux__
        pugi::xml_document doc;
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName + "/prefs";
        std::string fpathth = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/.config/unity3d/" + companyName + "/" + productName;
        if (FileAccess::exists(String(fpathth.c_str())) == false) {
            if (FileAccess::exists(String(fpath.c_str())) == false) {
                return false;
            }
        } else {
            fpath = fpathth;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        pugi::xml_parse_result result = doc.load((const char *)filedtb.ptr());
        if (!result) {
            return false;
        }
        for (pugi::xml_node pref: doc.first_child().children("pref")) {
            if ((std::string)pref.attribute("name").value() == namepref) {
                return true;
            }
        }
#elif defined _WIN32
        std::string path = "Software\\" + companyName + "\\" + productName;
        for (std::string propname: winreg::enumerateProperties(HKEY_CURRENT_USER, path, KEY_WOW64_64KEY)) {
            std::string proppattern = namepref + "_h";
            if (propname.substr(0, proppattern.size()) == proppattern && (propname.size() - proppattern.size()) == 10) {
                return true;
            }
        }
#elif defined __APPLE__
        std::string fpath = std::string(OS::get_singleton()->get_environment(String("HOME")).utf8().get_data()) + "/Library/Preferences/unity." + companyName + "." + productName + ".plist";
        if (FileAccess::exists(String(fpath.c_str())) == false) {
            return false;
        }
        Error err;
        Vector<uint8_t> filedtb = FileAccess::get_file_as_array(String(fpath.c_str()), &err);
        std::map<std::string, boost::any> dict;
        Plist::readPlist((const char *)filedtb.ptr(), filedtb.size(), dict);
        if (dict.find(namepref) != dict.end()) {
            return true;
        }
#endif
    }
    return false;
}

void GodotOldPrefs::_bind_methods() {
    ObjectTypeDB::bind_method("setCredentials", &GodotOldPrefs::setCredentials);
    ObjectTypeDB::bind_method("getInt", &GodotOldPrefs::getInt);
    ObjectTypeDB::bind_method("getString", &GodotOldPrefs::getString);
    ObjectTypeDB::bind_method("getFloat", &GodotOldPrefs::getFloat);
#if defined __APPLE__
    ObjectTypeDB::bind_method("getFloatAsString", &GodotOldPrefs::getFloatAsString);
#endif
    ObjectTypeDB::bind_method("getBool", &GodotOldPrefs::getBool);
    ObjectTypeDB::bind_method("keyExists", &GodotOldPrefs::keyExists);
}

GodotOldPrefs::GodotOldPrefs() {
    companyName = "";
    productName = "";
}
