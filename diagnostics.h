#pragma once
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <string>

struct Location {
    const int line_number;
    const std::string file_name;

    Location(int lineno, std::string file);
};

class Diagnostic {
private:
    Location m_location;
    std::string m_message;
public:
    Diagnostic(Location loc, std::string msg);

    /*
     * The location in where this diagnostic was reported.
     */
    Location location() const;

    /*
     * The message describing this diagnostic.
     */
    std::string message() const;

};

class DiagnosticsList {
private:
    std::vector<Diagnostic*> m_diagnostics;

    void report(Location loc, std::string msg);
public:
    DiagnosticsList();

    std::vector<Diagnostic*> diagnostics() const;

};

#endif