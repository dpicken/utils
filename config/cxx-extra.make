CXXFLAGS += -Wno-padded

cxxflags_extra_clang++ :=
cxxflags_extra_clang++ += -Wno-missing-prototypes
cxxflags_extra_clang++ += -Wno-missing-braces

cxxflags_extra_g++ :=
cxxflags_extra_g++ += -Wno-invalid-offsetof

CXXFLAGS += $(cxxflags_extra_$(CXX))
