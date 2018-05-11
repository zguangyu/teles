find_path(MySQLCPP_INCLUDE_DIR NAMES cppconn/driver.h
    PATHS
    $ENV{MYSQL_INCLUDE_DIR}
    $ENV{MYSQL_DIR}/include
    /usr/local/mysql/include
    /opt/mysql/mysql/include
)

set(MySQLCPP_LIB_PATHS
    $ENV{MYSQL_DIR}/libmysql_r/.libs
    $ENV{MYSQL_DIR}/lib
    $ENV{MYSQL_DIR}/lib/mysql
    /usr/local/mysql/lib
    /opt/mysql/mysql/lib
)

find_library(MySQLCPP_LIBRARIES NAMES mysqlcppconn
    PATHS
    ${MYSQLCPP_LIB_PATHS}
)

if (TARGET mysqlcpp)
    # avoid errors defining targets twice
    return()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQLCPP
    FOUND_VAR MySQLCPP_FOUND
    REQUIRED_VARS MySQLCPP_LIBRARIES MySQLCPP_INCLUDE_DIR)

add_library(mysqlcpp SHARED IMPORTED)

set_property(TARGET mysqlcpp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${MySQLCPP_INCLUDE_DIR})
set_property(TARGET mysqlcpp PROPERTY IMPORTED_LOCATION ${MySQLCPP_LIBRARIES})

mark_as_advanced(MySQLCPP_INCLUDE_DIR MySQLCPP_LIBRARIES)
