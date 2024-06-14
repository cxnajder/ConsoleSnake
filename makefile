PROJECT_NAME := ConsoleSnake
SRC := $(PROJECT_NAME).cpp
BUILD_DIR := Build/MakeBuild
TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	g++ $(SRC) -o $(TARGET)