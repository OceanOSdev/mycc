#pragma once
#ifndef FINALIZED_BODY_H
#define FINALIZED_BODY_H

#include <vector>
#include <string>

namespace JVMProcessor {

/**
 * @brief The payload delivered by the JAsmBuilder after finalizing
 * the emitted instruction list.
 */
class FinalizedBody {
private:
    std::vector<std::string> m_body;
public:
    FinalizedBody(std::vector<std::string> body);

    /**
     * @brief A list of stringified instructions.
     */
    std::vector<std::string> body() const;
};

}

#endif