# changed from https://stackoverflow.com/questions/2394609/makefile-header-dependencies
CXX = c++
# CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow --std=c++11
CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wno-missing-braces -Wpedantic -Wshadow --std=c++11 -g
LINKER_FLAGS = -L/usr/lib -lstdc++ -lm

BIN = renderer
BUILD_DIR = ./built

CPP = main.cpp geometry.cpp buffer.cpp component.cpp input_assembler.cpp semantic.cpp shader.cpp framebuffer.cpp rasterizer.cpp output_merger.cpp primitive_assembler.cpp pipeline.cpp texture.cpp shader_processor.cpp model.cpp
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

$(BIN) : $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN) : $(OBJ)
	# Create build directories - same structure as sources.
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LINKER_FLAGS)

# Include all .d files
-include $(DEP)

# The potential dependency on header files is covered by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	# The -MMD flags additionaly creates a .d file with the same name as the .o file.
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY : run
run: $(BIN)
	$(BUILD_DIR)/$(BIN)

.PHONY : clean
clean:
	rm -rf $(BUILD_DIR) fb_color.bmp fb_depth.bmp
