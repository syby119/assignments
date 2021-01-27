#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>

#define INFO(message) { \
    std::cout << "[INFO] " << __FILE__ << "(" << __LINE__ << ")" \
              << " `" << __FUNCTION__ << "`: " << message << std::endl; \
}

#define WARN(message) { \
    std::cerr << "[WARNING] " << __FILE__ << "(" << __LINE__ << ")" \
              << " `" << __FUNCTION__ << "`: " << message << std::endl; \
}

#define THROW(message) { \
    throw std::runtime_error( \
        std::string(__FILE__) + "(" + std::to_string(__LINE__) + ")" \
        + " `" + __FUNCTION__ + "`: " + message); \
}

#define FATAL(message) { \
    std::cerr << "[FATAL] "<<  __FILE__ << "(" << __LINE__ << ")" \
              << " `" << __FUNCTION__ << "`: " << message << std::endl; \
    exit(-1); \
}