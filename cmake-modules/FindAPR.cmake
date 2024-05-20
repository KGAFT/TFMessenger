if(NOT DEFINED APR_HOME AND DEFINED $ENV{APR_HOME})
    set(APR_HOME $ENV{APR_HOME})
endif()
if(DEFINED APR_HOME)
    set(APR_FOUND "YES")
    set(APR_INCLUDE_DIR ${APR_HOME}/include)
    set(APR_LIB_NAMES ${APR_LIB_NAMES} apr-1 apr apr-2 apr1 apr2)
    find_library(APR_LIBRARY NAMES ${APR_LIB_NAMES} PATHS ${APR_HOME}/lib ${APR_HOME}/bin)
endif ()

if(NOT DEFINED APR_FOUND)
    find_path(APR_INCLUDE_DIR apr.h
            /usr/local/include/apr-2
            /usr/local/include/apr-2.0
            /usr/include/apr-2
            /usr/include/apr-2.0
            /usr/local/apr/include/apr-2
            /usr/local/include/apr
            /usr/local/include/apr
            /usr/include/apr
            /usr/include/apr
            /usr/local/apr/include/apr
            "C:/Program Files (x86)/APR/include/apr-2"
            "C:/Program Files (x86)/APR/include/apr"
            "C:/Program Files (x86)/APR/include"
            "C:/Program Files (x86)/apr/include/apr-2"
            "C:/Program Files (x86)/apr/include/apr"
            "C:/Program Files (x86)/apr-2/include/apr-2"
            "C:/Program Files/APR/include/apr-2"
            "C:/Program Files/APR/include/apr"
            "C:/Program Files/APR/include"
            "C:/Program Files/apr/include/apr-2"
            "C:/Program Files/apr/include/apr"
            "C:/Program Files/apr-2/include/apr-2"
            C:/APR/include/apr-2
            C:/APR/include/apr
            C:/APR/include
            C:/apr/include/apr-2
            C:/apr/include/apr
            C:/apr-2/include/apr-2
    )
    set(APR_LIB_NAMES ${APR_LIB_NAMES} apr-1 apr apr-2 apr1 apr2)
    find_library(APR_LIBRARY NAMES ${APR_LIB_NAMES} PATHS
            /usr/local/lib
            /usr/local/lib/apr-2
            /usr/local/lib/apr-2.0
            /usr/lib
            /usr/lib/apr-2
            /usr/lib/apr-2.0
            /usr/lib/apr/lib
            /usr/lib/apr-2/lib
            /usr/local/lib
            /usr/local/lib/apr
            /usr/local/lib/apr
            /usr/lib
            /usr/lib/apr
            /usr/lib/apr
            /usr/lib/apr/lib
            /usr/lib/apr/lib
            /usr/local/lib64
            /usr/local/lib64/apr-2
            /usr/local/lib64/apr-2.0
            /usr/lib64
            /usr/lib64/apr-2
            /usr/lib64/apr-2.0
            /usr/lib64/apr/lib64
            /usr/lib64/apr-2/lib64
            /usr/local/lib64
            /usr/local/lib64/apr
            /usr/local/lib64/apr
            /usr/lib64
            /usr/lib64/apr
            /usr/lib64/apr
            /usr/lib64/apr/lib64
            /usr/lib64/apr/lib64
            /usr/local/bin
            /usr/local/bin/apr-2
            /usr/local/bin/apr-2.0
            /usr/bin
            /usr/bin/apr-2
            /usr/bin/apr-2.0
            /usr/bin/apr/bin
            /usr/bin/apr-2/bin
            /usr/local/bin
            /usr/local/bin/apr
            /usr/local/bin/apr
            /usr/bin
            /usr/bin/apr
            /usr/bin/apr
            /usr/bin/apr/bin
            /usr/bin/apr/bin
            "C:/Program Files (x86)/APR/lib/apr-2"
            "C:/Program Files (x86)/APR/lib/apr"
            "C:/Program Files (x86)/APR/lib"
            "C:/Program Files (x86)/apr/lib/apr-2"
            "C:/Program Files (x86)/apr/lib/apr"
            "C:/Program Files (x86)/apr-2/lib/apr-2"
            "C:/Program Files/APR/lib/apr-2"
            "C:/Program Files/APR/lib/apr"
            "C:/Program Files/APR/lib"
            "C:/Program Files/apr/lib/apr-2"
            "C:/Program Files/apr/lib/apr"
            "C:/Program Files/apr-2/lib/apr-2"
            C:/APR/lib/apr-2
            C:/APR/lib/apr
            C:/APR/lib
            C:/apr/lib/apr-2
            C:/apr/lib/apr
            C:/apr-2/lib/apr-2
            "C:/Program Files (x86)/APR/bin/apr-2"
            "C:/Program Files (x86)/APR/bin/apr"
            "C:/Program Files (x86)/APR/bin"
            "C:/Program Files (x86)/apr/bin/apr-2"
            "C:/Program Files (x86)/apr/bin/apr"
            "C:/Program Files (x86)/apr-2/bin/apr-2"
            "C:/Program Files/APR/bin/apr-2"
            "C:/Program Files/APR/bin/apr"
            "C:/Program Files/APR/bin"
            "C:/Program Files/apr/bin/apr-2"
            "C:/Program Files/apr/bin/apr"
            "C:/Program Files/apr-2/bin/apr-2"
            C:/APR/bin/apr-2
            C:/APR/bin/apr
            C:/APR/bin
            C:/apr/bin/apr-2
            C:/apr/bin/apr
            C:/apr-2/bin/apr-2
    )

    if (${APR_LIBRARY} AND ${APR_INCLUDE_DIR})
        SET(APR_LIBRARIES ${APR_LIBRARY})
        SET(APR_FOUND "YES")
    endif ()
endif ()
if(APR_FOUND)
    add_library(apr IMPORTED)
    set_target_properties(apr PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${APR_INCLUDE_DIR}
            INTERFACE_LINK_LIBRARIES ${APR_LIBRARY}
    )
endif ()
