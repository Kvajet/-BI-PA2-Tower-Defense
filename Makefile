CXX=g++
CXXFLAGS=-std=c++17 -pedantic -Wall -Wextra -g
TARGET=tichyj12

BUILD_DIR=build
MKDIR=mkdir -p

.PHONY: all
all: compile doc
	@echo "all finish"

.PHONY: compile
compile: $(TARGET)
	@echo "compile finish"

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/ $(TARGET) doc/ 2>/dev/null

.PHONY: doc
doc:
	doxygen Doxyfile
	@echo "doc finish"

$(TARGET): $(BUILD_DIR)/CAstar.o $(BUILD_DIR)/CEmpty.o $(BUILD_DIR)/CEnemy.o $(BUILD_DIR)/CEntity.o $(BUILD_DIR)/CField.o $(BUILD_DIR)/CGameInfo.o $(BUILD_DIR)/CGameInstance.o $(BUILD_DIR)/CGameLoader.o $(BUILD_DIR)/CGameLogic.o $(BUILD_DIR)/CGameRenderer.o $(BUILD_DIR)/CGameSaver.o $(BUILD_DIR)/CGameStats.o $(BUILD_DIR)/CLoadGame.o $(BUILD_DIR)/CMainMenu.o $(BUILD_DIR)/CNewGame.o $(BUILD_DIR)/CRenderer.o $(BUILD_DIR)/CToken.o $(BUILD_DIR)/CTokenEnemy.o $(BUILD_DIR)/CTokenLoader.o $(BUILD_DIR)/CTokenTower.o $(BUILD_DIR)/CTower.o $(BUILD_DIR)/game_constants.o $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CAstar.o: src/CAstar.cpp src/CAstar.h src/CField.h src/game_constants.cpp
$(BUILD_DIR)/CEmpty.o: src/CEmpty.cpp src/CEmpty.h src/CField.h src/game_constants.cpp
$(BUILD_DIR)/CEnemy.o: src/CEnemy.cpp src/CEnemy.h src/CEntity.h src/CField.h src/game_constants.cpp src/CAstar.h
$(BUILD_DIR)/CEntity.o: src/CEntity.cpp src/CEntity.h src/CField.h src/game_constants.cpp
$(BUILD_DIR)/CField.o: src/CField.cpp src/CField.h src/game_constants.cpp
$(BUILD_DIR)/CGameInfo.o: src/CGameInfo.cpp src/CGameInfo.h src/CRenderer.h src/game_constants.cpp src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CField.h src/CEnemy.h src/CAstar.h src/CTokenEnemy.h
$(BUILD_DIR)/CGameInstance.o: src/CGameInstance.cpp src/CGameInstance.h src/game_constants.cpp src/CField.h src/CGameLogic.h src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CEnemy.h src/CAstar.h src/CTokenEnemy.h src/CGameStats.h src/CTokenLoader.h src/CGameLoader.h src/CEmpty.h src/CGameSaver.h src/CGameRenderer.h src/CRenderer.h src/CMainMenu.h src/CNewGame.h src/CLoadGame.h src/CGameInfo.h
$(BUILD_DIR)/CGameLoader.o: src/CGameLoader.cpp src/CGameLoader.h src/CField.h src/game_constants.cpp src/CTower.h src/CEntity.h src/CEnemy.h src/CAstar.h src/CEmpty.h src/CGameStats.h src/CTokenTower.h src/CToken.h src/CTokenEnemy.h src/CGameSaver.h
$(BUILD_DIR)/CGameLogic.o: src/CGameLogic.cpp src/CGameLogic.h src/game_constants.cpp src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CField.h src/CEnemy.h src/CAstar.h src/CTokenEnemy.h src/CGameStats.h src/CTokenLoader.h src/CGameLoader.h src/CEmpty.h src/CGameSaver.h
$(BUILD_DIR)/CGameRenderer.o: src/CGameRenderer.cpp src/CGameRenderer.h src/CRenderer.h src/game_constants.cpp src/CGameStats.h src/CField.h src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CEnemy.h src/CAstar.h
$(BUILD_DIR)/CGameSaver.o: src/CGameSaver.cpp src/CGameSaver.h src/CEntity.h src/CField.h src/game_constants.cpp
$(BUILD_DIR)/CGameStats.o: src/CGameStats.cpp src/CGameStats.h
$(BUILD_DIR)/CLoadGame.o: src/CLoadGame.cpp src/CLoadGame.h src/CRenderer.h src/game_constants.cpp src/CGameLoader.h src/CField.h src/CTower.h src/CEntity.h src/CEnemy.h src/CAstar.h src/CEmpty.h src/CGameStats.h src/CTokenTower.h src/CToken.h src/CTokenEnemy.h src/CGameSaver.h
$(BUILD_DIR)/CMainMenu.o: src/CMainMenu.cpp src/CMainMenu.h src/CRenderer.h src/game_constants.cpp
$(BUILD_DIR)/CNewGame.o: src/CNewGame.cpp src/CNewGame.h src/CRenderer.h src/game_constants.cpp
$(BUILD_DIR)/CRenderer.o: src/CRenderer.cpp src/CRenderer.h src/game_constants.cpp
$(BUILD_DIR)/CToken.o: src/CToken.cpp src/CToken.h src/CTower.h src/CEntity.h src/CField.h src/game_constants.cpp src/CEnemy.h src/CAstar.h
$(BUILD_DIR)/CTokenEnemy.o: src/CTokenEnemy.cpp src/CTokenEnemy.h src/CToken.h src/CTower.h src/CEntity.h src/CField.h src/game_constants.cpp src/CEnemy.h src/CAstar.h
$(BUILD_DIR)/CTokenLoader.o: src/CTokenLoader.cpp src/CTokenLoader.h src/CAstar.h src/CField.h src/game_constants.cpp src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CEnemy.h src/CTokenEnemy.h
$(BUILD_DIR)/CTokenTower.o: src/CTokenTower.cpp src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CField.h src/game_constants.cpp src/CEnemy.h src/CAstar.h
$(BUILD_DIR)/CTower.o: src/CTower.cpp src/CTower.h src/CEntity.h src/CField.h src/game_constants.cpp
$(BUILD_DIR)/game_constants.o: src/game_constants.cpp
$(BUILD_DIR)/main.o: src/main.cpp src/CGameInstance.h src/game_constants.cpp src/CField.h src/CGameLogic.h src/CTokenTower.h src/CToken.h src/CTower.h src/CEntity.h src/CEnemy.h src/CAstar.h src/CTokenEnemy.h src/CGameStats.h src/CTokenLoader.h src/CGameLoader.h src/CEmpty.h src/CGameSaver.h src/CGameRenderer.h src/CRenderer.h src/CMainMenu.h src/CNewGame.h src/CLoadGame.h src/CGameInfo.h
