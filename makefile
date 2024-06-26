include prorab.mk

this_name := tiktaksy

this_srcs += $(call prorab-src-dir,src)

ifeq ($(debug), true)
    this_cxxflags += -DDEBUG
endif

this_cxxflags += -Wall # enable all warnings
this_cxxflags += -Wno-comment # no warnings on nested comments
this_cxxflags += -funsigned-char # the 'char' type is unsigned
this_cxxflags += -Wnon-virtual-dtor # warn if base class has non-virtual destructor
this_cxxflags += -fno-operator-names # do not treat 'and', 'bitand','bitor','compl','not','or','xor' as keywords
this_cxxflags += -Werror # treat warnings as errors
this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing # in order to comply with the c++ standard more strictly
this_cxxflags += -g # include debugging symbols
this_cxxflags += -std=c++17

this_ldlibs += -lruisapp-opengl-xorg -lutki -lGL -lstdc++ -lruis -ltml  -rdynamic -lm

this_out_dir := build

$(eval $(prorab-build-app))
