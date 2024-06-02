#pragma once

#include "./LexycalAnalyser/LexycalAnalyzer.hpp"
#include <memory>

class Compiler
{
private:
    LexycalAnalyzer m_lexicalAnalyzer;

public:
    Compiler();
    std::optional<Error> Build(std::string query);
};