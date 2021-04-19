#pragma once
#ifndef QSEM_H
#define QSEM_H

#include <string>

namespace Logging {
    class Logger;
    struct PartThreeInfoList;
}

namespace QuickSemanticAnalyzer {
    void log_analysis(Logging::Logger* logger, Logging::PartThreeInfoList* info_list);
}

#endif