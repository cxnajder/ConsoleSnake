PROJECT_NAME = ConsoleSnake
BUILD_DIR = Build/MakeBuild/
SRC_DIR = Src/

$(BUILD_DIR)$(PROJECT_NAME): $(SRC_DIR)$(PROJECT_NAME).cpp
	@mkdir -p $(BUILD_DIR)
	g++ $< -o $@