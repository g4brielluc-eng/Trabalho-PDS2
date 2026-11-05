CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -g -O0 --coverage
INCLUDES := -Iinclude -Itests

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
SRC_DIR := src
TEST_DIR := tests

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TEST_SRCS := $(wildcard $(TEST_DIR)/teste_*.cpp)

# NOVO: Variáveis para isolar o motor de testes
MAIN_TEST_SRC := $(TEST_DIR)/main_tests.cpp
MAIN_TEST_OBJ := $(OBJ_DIR)/main_tests.o

.PHONY: all objs test coverage clean run

all: test

$(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

# Compila cada .cpp de src/ isoladamente.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	-$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# NOVO: Regra exclusiva para compilar o main_tests apenas uma vez
$(MAIN_TEST_OBJ): $(MAIN_TEST_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

objs: $(OBJS)

# MODIFICADO: Agora o target test depende do MAIN_TEST_OBJ e linka ele com cada teste
test: objs $(MAIN_TEST_OBJ) | $(BUILD_DIR)
	@status=0; \
	for t in $(TEST_SRCS); do \
		name=$$(basename $$t .cpp); \
		avail_objs=$$(ls $(OBJ_DIR)/*.o 2>/dev/null | grep -v "main_tests.o" | grep -v "main.o"); \
		if $(CXX) $(CXXFLAGS) $(INCLUDES) $$t $(MAIN_TEST_OBJ) $$avail_objs -o $(BUILD_DIR)/$$name 2>$(BUILD_DIR)/$$name.log; then \
			echo "=== $$name: PASSOU na compilacao/link, rodando ==="; \
			./$(BUILD_DIR)/$$name || status=1; \
		else \
			echo "=== $$name: nao foi possivel linkar (log em $(BUILD_DIR)/$$name.log) ==="; \
		fi; \
	done; \
	exit $$status

# Gera relatorio de cobertura (requer gcovr: pip install gcovr)
coverage: test
	gcovr --root . --filter 'src/' --filter 'include/' -e 'src/main.cpp' --print-summary -o $(BUILD_DIR)/coverage.txt
	gcovr --root . --filter 'src/' --filter 'include/' -e 'src/main.cpp' --html --html-details -o $(BUILD_DIR)/coverage.html
	@echo "Resumo em $(BUILD_DIR)/coverage.txt, relatorio em $(BUILD_DIR)/coverage.html"

# NOVO: Compila a aplicação final e executa o menu
run: objs | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(BUILD_DIR)/oficina
	./$(BUILD_DIR)/oficina

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BUILD_DIR)/teste_* $(BUILD_DIR)/*.log $(BUILD_DIR)/coverage.* $(BUILD_DIR)/oficina
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
