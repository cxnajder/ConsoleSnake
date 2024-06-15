PROJECT_NAME = ConsoleSnake
BUILD_DIR = Build/MakeBuild/

$(BUILD_DIR)$(PROJECT_NAME): $(PROJECT_NAME).cpp
	@mkdir -p $(BUILD_DIR)
	g++ $(PROJECT_NAME).cpp -o $@