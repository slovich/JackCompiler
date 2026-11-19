
cmake_minimum_required(VERSION 3.16)

project(JackCompiler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml)

set(CORE_SOURCES

    # AST - Infraestrutura
    core/src/ast/sourcespan.cpp
    core/include/jack/ast/sourcespan.h
    core/src/ast/astnode.cpp
    core/include/jack/ast/astnode.h

    # AST - Estrutura principal
    core/src/ast/programnode.cpp
    core/include/jack/ast/programnode.h
    core/src/ast/classnode.cpp
    core/include/jack/ast/classnode.h

    # AST - Declarações
    core/src/ast/variabledeclarationnode.cpp
    core/include/jack/ast/variabledeclarationnode.h
    core/src/ast/subroutinedeclarationnode.cpp
    core/include/jack/ast/subroutinedeclarationnode.h
    core/src/ast/parameternode.cpp
    core/include/jack/ast/parameternode.h

    # AST - Statements
    core/src/ast/statementnode.cpp
    core/include/jack/ast/statementnode.h
    core/src/ast/letstatementnode.cpp
    core/include/jack/ast/letstatementnode.h
    core/src/ast/ifstatementnode.cpp
    core/include/jack/ast/ifstatementnode.h
    core/src/ast/whilestatementnode.cpp
    core/include/jack/ast/whilestatementnode.h
    core/src/ast/dostatementnode.cpp
    core/include/jack/ast/dostatementnode.h
    core/src/ast/returnstatementnode.cpp
    core/include/jack/ast/returnstatementnode.h

    # AST - Expressions
    core/src/ast/expressionnode.cpp
    core/include/jack/ast/expressionnode.h
    core/src/ast/variablereferencenode.cpp
    core/include/jack/ast/variablereferencenode.h
    core/src/ast/arrayaccessnode.cpp
    core/include/jack/ast/arrayaccessnode.h
    core/src/ast/binaryexpressionnode.cpp
    core/include/jack/ast/binaryexpressionnode.h
    core/src/ast/unaryexpressionnode.cpp
    core/include/jack/ast/unaryexpressionnode.h
    core/src/ast/integerliteralnode.cpp
    core/include/jack/ast/integerliteralnode.h
    core/src/ast/stringliteralnode.cpp
    core/include/jack/ast/stringliteralnode.h
    core/src/ast/keywordconstantnode.cpp
    core/include/jack/ast/keywordconstantnode.h
    core/src/ast/subroutinecallnode.cpp
    core/include/jack/ast/subroutinecallnode.h

    # AST - Enums
    core/include/jack/ast/variablekind.h
    core/include/jack/ast/subroutinekind.h
    core/include/jack/ast/binaryoperator.h
    core/include/jack/ast/unaryoperator.h
    core/include/jack/ast/keywordconstant.h

    # XML
    core/include/jack/xml/geradorxmllexico.h
    core/src/xml/geradorxmllexico.cpp
    core/include/jack/xml/geradorxmlsintatico.h
    core/src/xml/geradorxmlsintatico.cpp

    # Lexer
    core/include/jack/lexer/token.h
    core/src/lexer/token.cpp
    core/include/jack/lexer/analisadorlexico.h
    core/src/lexer/analisadorlexico.cpp

    # Parser
    core/include/jack/parser/analisadorsintatico.h
    core/src/parser/analisadorsintatico.cpp
)

add_library(jack_core STATIC
    ${CORE_SOURCES}
    core/include/jack/parser/syntaxresult.h
    core/include/jack/ast/parenthesizedexpressionnode.h
    core/src/ast/parenthesizedexpressionnode.cpp
)

target_include_directories(jack_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
)

set(PROJECT_SOURCES

    # Aplicação Qt
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui

    src/realcelexico.cpp
    src/realcelexico.h

    src/realcesintatico.cpp
    src/realcesintatico.h

    src/dialogsobre.cpp
    src/dialogsobre.h
    src/dialogsobre.ui

    assets.qrc
)


if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(compilador
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


    )
else()
    if(ANDROID)
        add_library(compilador SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(compilador
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(compilador PRIVATE
    jack_core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
)

if(QT_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.compilador)
endif()

set_target_properties(compilador PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_executable(jack_cli
    cli/src/main.cpp
    cli/src/terminal.cpp
    cli/include/jack/cli/terminal.h
    cli/include/jack/cli/menu.h
    cli/src/menu.cpp
    cli/include/jack/cli/application.h
    cli/src/application.cpp
)

set_target_properties(jack_cli PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)

target_include_directories(jack_cli PRIVATE
    cli/include
)

target_compile_definitions(jack_cli PRIVATE
    JACK_PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(jack_cli PRIVATE
    jack_core
)

include(GNUInstallDirs)

install(TARGETS compilador
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(compilador)
endif()
