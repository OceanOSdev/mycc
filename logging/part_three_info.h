#pragma once
#ifndef PART_THREE_INFO_H
#define PART_THREE_INFO_H

#include <vector>
#include <string>

namespace Logging {

// NOTE: This is pretty hacky, and slightly inconsistent 

struct PartThreeVariableInfo {
    PartThreeVariableInfo() {}
    PartThreeVariableInfo(std::string _type, std::string _name, bool _init) :
        var_type(_type), var_name(_name), initialized(_init) {}
    std::string var_type;
    std::string var_name;
    bool initialized;
};

struct PartThreeStructInfo {
    std::string struct_name;
    std::vector<PartThreeVariableInfo*> struct_members;
};

struct PartThreeStatementInfo {
    PartThreeStatementInfo() {}
    PartThreeStatementInfo(int lineno, std::string _type) :
    line_no(lineno), stmt_type(_type) {}
    int line_no;
    std::string stmt_type;
};

struct PartThreeFunctionInfo {
    std::string func_name;
    std::string func_type;

    std::vector<PartThreeVariableInfo*> param_info;
    std::vector<PartThreeStructInfo*> local_struct_info;
    std::vector<PartThreeVariableInfo*> local_var_info;
    std::vector<PartThreeStatementInfo*> stmt_info;
};

struct PartThreeInfoList {
private:
PartThreeFunctionInfo* m_func_builder;
PartThreeStructInfo* m_struct_builder;
void reset_func_builder() { m_func_builder = new PartThreeFunctionInfo();}
void reset_struct_builder() { m_struct_builder = new PartThreeStructInfo();}
public:
    PartThreeInfoList() :
    m_func_builder(new PartThreeFunctionInfo()),
    m_struct_builder(new PartThreeStructInfo()) {}

    std::vector<PartThreeVariableInfo*> global_var_info; 
    std::vector<PartThreeStructInfo*> global_struct_info;
    std::vector<PartThreeFunctionInfo*> func_info;
    bool ignore = false; // For when the binder binds conditional expressions in loop statements or anything in funciton prototypes, set this to true

    void add_var_info(std::string _type, std::string _name, bool init, bool global);

    void struct_builder_add_name(std::string name);

    void struct_builder_add_member(std::string _type, std::string _name);

    void struct_builder_build(bool global, bool eject = false);

    void func_builder_add_name(std::string name);

    void func_builder_add_type(std::string type);

    void func_builder_add_param(std::string _type, std::string _name);

    void func_builder_add_statement(int lineno, std::string stmt_type);

    void func_builder_build();
};


}

#endif