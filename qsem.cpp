#include "qsem.h"
#include "logging/logger.h"
#include "logging/part_three_info.h"

namespace QuickSemanticAnalyzer {

void log_analysis(Logging::Logger* logger, Logging::PartThreeInfoList* info_list) {
    const std::string tab = "  "; 
    for (auto gsi : info_list->global_struct_info) {
        logger->log_info("Global struct ");
        logger->log_info_nl(gsi->struct_name);
        for (auto member : gsi->struct_members) {
            logger->log_info(tab);
            logger->log_info(member->var_type);
            logger->log_info(" ");
            logger->log_info_nl(member->var_name);
        }
        logger->log_info_nl("");
    }

    logger->log_info_nl("Global variables");
    for (auto gvi: info_list->global_var_info) {
        logger->log_info(tab);
        logger->log_info(gvi->var_type);
        logger->log_info(" ");
        logger->log_info(gvi->var_name);
        if (gvi->initialized) {
            logger->log_info(" (initialized)");
        }
        logger->log_info_nl("");
    }
    logger->log_info_nl("");

    for (auto fi : info_list->func_info) {
        logger->log_info("Function ");
        logger->log_info(fi->func_name);
        logger->log_info(", returns ");
        logger->log_info_nl(fi->func_type);
        logger->log_info_nl(tab + "Parameters");
        for (auto param : fi->param_info) {
            logger->log_info(tab+tab);
            logger->log_info(param->var_type);
            logger->log_info(" ");
            logger->log_info_nl(param->var_name);
        }
        logger->log_info_nl("");

        for (auto loc_struct : fi->local_struct_info) {
            logger->log_info(tab + "Local struct ");
            logger->log_info_nl(loc_struct->struct_name);
            for (auto member : loc_struct->struct_members) {
                logger->log_info(tab + tab);
                logger->log_info(member->var_type);
                logger->log_info(" ");
                logger->log_info_nl(member->var_name);
            }
            logger->log_info_nl("");
        }

        logger->log_info_nl(tab+ "Local variables");
        for (auto loc_var : fi->local_var_info) {
            logger->log_info(tab+tab);
            logger->log_info(loc_var->var_type);
            logger->log_info(" ");
            logger->log_info(loc_var->var_name);
            if (loc_var->initialized) {
                logger->log_info(" (initialized)");
            }
            logger->log_info_nl("");
        }
        logger->log_info_nl("");

        logger->log_info_nl(tab+"Statements");
        for (auto stmt : fi->stmt_info) {
            logger->log_info(tab+tab);
            logger->log_info("Expression on line ");
            logger->log_info(std::to_string(stmt->line_no));
            logger->log_info(" has type ");
            logger->log_info_nl(stmt->stmt_type);
        }
        logger->log_info_nl("");
    }
}

}