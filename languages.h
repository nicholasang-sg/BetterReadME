#pragma once
#include <map>
#include <string>
#include <iostream>

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <pyconfig.h>

namespace Translation {
    inline std::string escapePythonString(const std::string& s) {
        std::string result;
        for (char c : s) {
            if (c == '\\' || c == '\'') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }

    inline std::string translateText(const std::string& text, const std::string& targetLang) {
        std::string result;

        PyStatus status;
        PyConfig config;
        PyConfig_InitPythonConfig(&config);

        config.home = Py_DecodeLocale("./python_embed", nullptr);

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            Py_ExitStatusException(status); 
            return "[Python init failed]";
        }

        PyObject* main_module = PyImport_AddModule("__main__");
        PyObject* global_dict = PyModule_GetDict(main_module);

        std::string pyCode =
            "from deep_translator import GoogleTranslator\n"
            "translated = GoogleTranslator(source='auto', target='''" + targetLang + "''').translate('''" + escapePythonString(text) +"''')\n"
            "print(translated)\n";

        int runResult = PyRun_SimpleStringFlags(pyCode.c_str(), nullptr);
        if (runResult != 0) {
            std::cerr << "Python code execution failed!" << std::endl;
            result = "[Translation failed]";
        } else {
            PyObject* pyTranslated = PyDict_GetItemString(global_dict, "translated");
            if (pyTranslated && PyUnicode_Check(pyTranslated)) {
                result = PyUnicode_AsUTF8(pyTranslated);
            } else {
                result = "[Translation failed]";
            }
        }
        Py_Finalize();
        return result;
    }

    inline std::map<std::string, std::string> getSupportedLanguages() {
        return {
            {"afrikaans", "af"},
            {"albanian", "sq"},
            {"amharic", "am"},
            {"arabic", "ar"},
            {"armenian", "hy"},
            {"assamese", "as"},
            {"aymara", "ay"},
            {"azerbaijani", "az"},
            {"bambara", "bm"},
            {"basque", "eu"},
            {"belarusian", "be"},
            {"bengali", "bn"},
            {"bhojpuri", "bho"},
            {"bosnian", "bs"},
            {"bulgarian", "bg"},
            {"catalan", "ca"},
            {"cebuano", "ceb"},
            {"chichewa", "ny"},
            {"chinese (simplified)", "zh-CN"},
            {"chinese (traditional)", "zh-TW"},
            {"corsican", "co"},
            {"croatian", "hr"},
            {"czech", "cs"},
            {"danish", "da"},
            {"dhivehi", "dv"},
            {"dogri", "doi"},
            {"dutch", "nl"},
            {"english", "en"},
            {"esperanto", "eo"},
            {"estonian", "et"},
            {"ewe", "ee"},
            {"filipino", "tl"},
            {"finnish", "fi"},
            {"french", "fr"},
            {"frisian", "fy"},
            {"galician", "gl"},
            {"georgian", "ka"},
            {"german", "de"},
            {"greek", "el"},
            {"guarani", "gn"},
            {"gujarati", "gu"},
            {"haitian creole", "ht"},
            {"hausa", "ha"},
            {"hawaiian", "haw"},
            {"hebrew", "iw"},
            {"hindi", "hi"},
            {"hmong", "hmn"},
            {"hungarian", "hu"},
            {"icelandic", "is"},
            {"igbo", "ig"},
            {"ilocano", "ilo"},
            {"indonesian", "id"},
            {"irish", "ga"},
            {"italian", "it"},
            {"japanese", "ja"},
            {"javanese", "jw"},
            {"kannada", "kn"},
            {"kazakh", "kk"},
            {"khmer", "km"},
            {"kinyarwanda", "rw"},
            {"konkani", "gom"},
            {"korean", "ko"},
            {"krio", "kri"},
            {"kurdish (kurmanji)", "ku"},
            {"kurdish (sorani)", "ckb"},
            {"kyrgyz", "ky"},
            {"lao", "lo"},
            {"latin", "la"},
            {"latvian", "lv"},
            {"lingala", "ln"},
            {"lithuanian", "lt"},
            {"luganda", "lg"},
            {"luxembourgish", "lb"},
            {"macedonian", "mk"},
            {"maithili", "mai"},
            {"malagasy", "mg"},
            {"malay", "ms"},
            {"malayalam", "ml"},
            {"maltese", "mt"},
            {"maori", "mi"},
            {"marathi", "mr"},
            {"meiteilon (manipuri)", "mni-Mtei"},
            {"mizo", "lus"},
            {"mongolian", "mn"},
            {"myanmar", "my"},
            {"nepali", "ne"},
            {"norwegian", "no"},
            {"odia (oriya)", "or"},
            {"oromo", "om"},
            {"pashto", "ps"},
            {"persian", "fa"},
            {"polish", "pl"},
            {"portuguese", "pt"},
            {"punjabi", "pa"},
            {"quechua", "qu"},
            {"romanian", "ro"},
            {"russian", "ru"},
            {"samoan", "sm"},
            {"sanskrit", "sa"},
            {"scots gaelic", "gd"},
            {"sepedi", "nso"},
            {"serbian", "sr"},
            {"sesotho", "st"},
            {"shona", "sn"},
            {"sindhi", "sd"},
            {"sinhala", "si"},
            {"slovak", "sk"},
            {"slovenian", "sl"},
            {"somali", "so"},
            {"spanish", "es"},
            {"sundanese", "su"},
            {"swahili", "sw"},
            {"swedish", "sv"},
            {"tajik", "tg"},
            {"tamil", "ta"},
            {"tatar", "tt"},
            {"telugu", "te"},
            {"thai", "th"},
            {"tigrinya", "ti"},
            {"tsonga", "ts"},
            {"turkish", "tr"},
            {"turkmen", "tk"},
            {"twi", "ak"},
            {"ukrainian", "uk"},
            {"urdu", "ur"},
            {"uyghur", "ug"},
            {"uzbek", "uz"},
            {"vietnamese", "vi"},
            {"welsh", "cy"},
            {"xhosa", "xh"},
            {"yiddish", "yi"},
            {"yoruba", "yo"},
            {"zulu", "zu"}
        };
    }
}