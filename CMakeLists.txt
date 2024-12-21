cmake_minimum_required(VERSION 3.10)
project(StudentManagementSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add MySQL Connector include directories
include_directories(/usr/local/mysql-connector-c++-9.1.0/include)

include_directories(/usr/local/mysql-connector-c++-9.1.0/include/jdbc)

# Link MySQL Connector library
link_directories(/usr/local/mysql-connector-c++-9.1.0/lib64)

# Add executable
add_executable(main main.cpp)

# Link MySQL Connector library
target_link_libraries(main mysqlcppconn)
