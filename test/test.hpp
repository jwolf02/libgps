#ifndef __TEST_HPP
#define __TEST_HPP

#include <stdexcept>

#define ASSERT(condition)                                           \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string(" assertion failed: ")  \
                              + std::string(#condition)             \
    );                                                              \
  }                                                                 \
}

#define ASSERT_EQUAL(x, y)                                          \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( x ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( y ) )             \
    );                                                              \
  }                                                                 \
}

#define ASSERT_EQUAL_FLOAT(x, y)                                    \
{                                                                   \
  if(std::abs(x - y) > 0.000001)                                    \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( x ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( y ) )             \
    );                                                              \
  }                                                                 \
}

#define ASSERT_NO_EXCEPTION(command)                                \
{                                                                   \
    try {                                                           \
        command;                                                    \
    } catch (...) {                                                 \
        throw std::runtime_error(   std::string( __FILE__ )         \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": exception occurred when calling " )                 \
                              + #command);                          \
    }                                                               \
}

#endif // __TEST_HPP
