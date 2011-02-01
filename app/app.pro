include(../lab.pri)

TARGET = lab

unix {
QMAKE_LEX = flex
QMAKE_YACC = bison
LEXSOURCES += shader/glsl.l
YACCSOURCES += shader/glsl.y
}
!unix {
DEFINES += YY_NO_UNISTD_H
HEADERS += glsl_yacc.h
SOURCES += glsl_yacc.cpp glsl_lex.cpp
}

HEADERS += \
    lab_tr1.hpp \
    forward.hpp \
    highlighter.hpp \
    watcher.hpp \
    state.hpp \
    light.hpp \
    camera.hpp \
    object3d.hpp \
    editor.hpp \
    renderpass.hpp \
    scene.hpp \
    project.hpp \
    glwidget.hpp \
    properties.hpp \
    mainwindow.hpp \
    opengl.hpp \
    texture.hpp \
    fbo.hpp \
    welcome.hpp \
    shader/grammar.hpp \
    shader/lexer.hpp \
    shader/error.hpp \
    shader/compiler_output_parser.hpp \
    shader/uniform.hpp \
    shader/shader.hpp \
    shader/program.hpp \
    render_pass_properties.hpp \
    wrap_glext.h \
    new_wizard.hpp \
    log.hpp \
    shader/sandbox_compiler.hpp \
    material.hpp \
    model.hpp \
    animation.hpp \
    obj_importer.hpp \
    utils.hpp \
    buffer_object.hpp

SOURCES += \
    highlighter.cpp \
    watcher.cpp \
    main.cpp \
    state.cpp \
    light.cpp \
    camera.cpp \
    object3d.cpp \
    editor.cpp \
    renderpass.cpp \
    scene.cpp \
    project.cpp \
    glwidget.cpp \
    properties.cpp \
    mainwindow.cpp \
    opengl.cpp \
    texture.cpp \
    fbo.cpp \
    welcome.cpp \
    shader/grammar.cpp \
    shader/error.cpp \
    shader/compiler_output_parser.cpp \
    shader/uniform.cpp \
    shader/shader.cpp \
    shader/program.cpp \
    render_pass_properties.cpp \
    wrap_glext.c \
    new_wizard.cpp \
    log.cpp \
    shader/sandbox_compiler.cpp \
    material.cpp \
    model.cpp \
    animation.cpp \
    obj_importer.cpp \
    utils.cpp \
    buffer_object.cpp

FORMS += \
    mainwindow.ui \
    welcome.ui \
    new_wizard.ui

win32 {
    CONFIG(release, debug|release):LIBS += ../ext/release/ext.lib ../shaderdb/release/shaderdb.lib
    CONFIG(debug, debug|release):LIBS += ../ext/debug/ext.lib ../shaderdb/debug/shaderdb.lib
}
!win32 {
    LIBS += ../ext/libext.a ../shaderdb/libshaderdb.a
}
PRE_TARGETDEPS += $$LIBS

RESOURCES += \
    ../gfx/gfx.qrc
