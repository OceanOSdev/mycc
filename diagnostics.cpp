#include "diagnostics.h"

Location::Location(int lineno, std::string file) :
    line_number(lineno), file_name(file) {}


Diagnostic::Diagnostic(Location loc, std::string msg) :
    m_location(loc), m_message(msg) {}

/*
 * The location in where this diagnostic was reported.
 */
Location Diagnostic::location() const {
    return m_location;
}

/*
 * The message describing this diagnostic.
 */
std::string Diagnostic::message() const {
    return m_message;
}

DiagnosticsList::DiagnosticsList() {}

std::vector<Diagnostic*> DiagnosticsList::diagnostics() const {
    return m_diagnostics;
}