CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
INCLUDES = -Isrc

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SOURCES = \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/DoublyLinkedList.cpp \
	$(SRC_DIR)/Document.cpp \
	$(SRC_DIR)/Command.cpp \
	$(SRC_DIR)/UndoRedoManager.cpp \
	$(SRC_DIR)/SearchEngine.cpp \
	$(SRC_DIR)/FileManager.cpp \
	$(SRC_DIR)/Editor.cpp \
	$(SRC_DIR)/UserInterface.cpp

OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TEST_SOURCES = \
	$(TEST_DIR)/test_main.cpp \
	$(SRC_DIR)/DoublyLinkedList.cpp \
	$(SRC_DIR)/Document.cpp \
	$(SRC_DIR)/Command.cpp \
	$(SRC_DIR)/UndoRedoManager.cpp \
	$(SRC_DIR)/SearchEngine.cpp \
	$(SRC_DIR)/FileManager.cpp \
	$(SRC_DIR)/Editor.cpp

TEST_OBJECTS = $(TEST_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean test run dirs

all: dirs lineforge

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)

lineforge: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Test target
test: dirs lineforge_tests
	./lineforge_tests

lineforge_tests: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: lineforge
	./lineforge

clean:
	rm -rf $(BUILD_DIR) lineforge lineforge_tests
