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

.PHONY: all objs test coverage clean

all: test

$(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

# Compila cada .cpp de src/ isoladamente. O "-" na frente faz o make
# ignorar erro de UMA classe e seguir compilando as outras (rode
# "make objs" sozinho pra ver so os erros de compilacao de cada uma).
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	-$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

objs: $(OBJS)

# Linka cada teste so com os .o que compilaram com sucesso ate agora.
# Se faltar alguma classe colaboradora ainda nao implementada, o erro
# de link aparece so para aquele teste, sem travar os demais.
test: objs | $(BUILD_DIR)
	@status=0; \
	for t in $(TEST_SRCS); do \
		name=$$(basename $$t .cpp); \
		avail_objs=$$(ls $(OBJ_DIR)/*.o 2>/dev/null); \
		if $(CXX) $(CXXFLAGS) $(INCLUDES) $$t $$avail_objs -o $(BUILD_DIR)/$$name 2>$(BUILD_DIR)/$$name.log; then \
			echo "=== $$name: PASSOU na compilacao/link, rodando ==="; \
			./$(BUILD_DIR)/$$name || status=1; \
		else \
			echo "=== $$name: nao foi possivel linkar (log em $(BUILD_DIR)/$$name.log) ==="; \
		fi; \
	done; \
	exit $$status

# Gera relatorio de cobertura (requer gcovr: pip install gcovr)
coverage: test
	gcovr --root . --filter 'src/' --filter 'include/' --print-summary -o $(BUILD_DIR)/coverage.txt
	gcovr --root . --filter 'src/' --filter 'include/' --html --html-details -o $(BUILD_DIR)/coverage.html
	@echo "Resumo em $(BUILD_DIR)/coverage.txt, relatorio em $(BUILD_DIR)/coverage.html"

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BUILD_DIR)/teste_* $(BUILD_DIR)/*.log $(BUILD_DIR)/coverage.*
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
