
#ifndef SCANNER_HH
#define SCANNER_HH
#include "parser.tab.hh"
#include <map>

class driver;

#define YY_DECL yy::parser::symbol_type yylex (driver& drv)

YY_DECL;

class driver
{
    driver();
    std::map<std::string, int> variables;
    int result;
    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;
    void scan_begin ();
    void scan_end();
    bool trace_scanning;
};

#endif