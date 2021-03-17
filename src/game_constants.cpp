// ------------------ CONSOLE COLORS ------------------

#define CONSOLE_FOREGROUND_BLACK    30
#define CONSOLE_FOREGROUND_RED      31
#define CONSOLE_FOREGROUND_GREEN    32
#define CONSOLE_FOREGROUND_YELLOW   33
#define CONSOLE_FOREGROUND_BLUE     34
#define CONSOLE_FOREGROUND_MAGENTA  35
#define CONSOLE_FOREGROUND_CYAN     36
#define CONSOLE_FOREGROUND_WHITE    37

#define CONSOLE_BACKGROUND_BLACK    40
#define CONSOLE_BACKGROUND_RED      41
#define CONSOLE_BACKGROUND_GREEN    42
#define CONSOLE_BACKGROUND_YELLOW   43
#define CONSOLE_BACKGROUND_BLUE     44
#define CONSOLE_BACKGROUND_MAGENTA  45
#define CONSOLE_BACKGROUND_CYAN     46
#define CONSOLE_BACKGROUND_WHITE    47

#define CONSOLE_TEXT_RESET          0
#define CONSOLE_TEXT_LIGHT          1
#define CONSOLE_TEXT_BLINK          5
#define CONSOLE_TEXT_STROKE         9
#define CONSOLE_TEXT_PREFIX         "\033["
#define CONSOLE_TEXT_POSTFIX        "\033[0m"

// ------------------ ENTITY ACTIONS ------------------

#define ACTION_SKIP                 100
#define ACTION_MOVE                 101
#define ACTION_DEATH                102
#define ACTION_ENEMY_END            103
#define ACTION_ATTACK               104

#define TOWER_ATTACK_AOE            110
#define TOWER_ATTACK_AOE_DESC       "Area of effect attack"
#define TOWER_ATTACK_CROSS          111
#define TOWER_ATTACK_CROSS_DESC     "Cross attack"

#define TOWER_SPECIAL_NONE          120
#define TOWER_SPECIAL_GENERATE      121
#define TOWER_SPECILA_GENERATE_DESC "Generates money"

#define SPECIAL_GENERATE_AMOUNT     5

#define THROUGH_FALSE               false
#define THROUGH_TRUE                true

// -------------------- LOADER --------------------

#define SAVE_INDEXER                "saves/save_indexer.txt"
#define MAP_INDEXER                 "maps/map_indexer.txt"
#define TOKENS                      "tokens/tokens.txt"
#define MAP_PREFIX                  "maps/"
#define SAVE_PREFIX                 "saves/"
#define SAVE_POSTFIX                ".txt"
#define FILE_OKAY                   10
#define FILE_ERROR                  11
#define ENEMY                       20
#define TOWER                       21
#define MIN_START_END_DISTANCE      1.5

// ------------------- BASE ENTITIES----------------

#define BASE_ENTITY_SPACE           ' '
#define BASE_ENTITY_WALL            '#'
#define BASE_ENTITY_START           'S'
#define BASE_ENTITY_END             'E'

// ------------------- MAIN MENU -------------------

#define MENU_SIZE                   4
#define MAX_MENU_ITEM_SIZE          10
#define MAIN_MENU_NEW_GAME_CONTENT  "NEW GAME"
#define MAIN_MENU_LOAD_GAME_CONTENT "LOAD GAME"
#define MAIN_MENU_GAME_INFO_CONTENT "GAME INFO"
#define MAIN_MENU_EXIT_CONTENT      "EXIT"
#define MAIN_MENU_LOGO_AUTHOR       "< GameEngine by Jan Tichý >"

// ------------------- GAME STATES -----------------

#define NUM_OF_ENEMIES_TO_DEPLOY    10
#define NUM_OF_ENEMIES_TO_BEAT      40

#define GAME_STATE_GAME_PHASE       400
#define GAME_STATE_BUY_PHASE_MAP    401
#define GAME_STATE_BUY_PHASE_ARRAY  402
#define GAME_STATE_MENU_PHASE       403
#define GAME_STATE_END_GAME         404
#define GAME_STATE_SAVE_GAME        405
#define GAME_STATE_GET_NAME         406
#define GAME_STATE_WIN              407             
#define GAME_STATE_LOST             408

#define GAME_LINE_WIN               "CONGRATULATIONS, YOU HAVE WON. GO PLAY FORTNITE NOW."
#define GAME_LINE_LOST              "OOPSIE, SEEMS THAT YOU LOST."
#define GAME_LINE_OBJECTIVE         "Number of enemies you have to kill: "

#define GAME_STATE_SAVE_SIZE        3
#define GAME_STATE_SAVE_ITEM_M_LEN  20
#define GAME_STATE_SAVE_GAME_ASK    "Do you want to save game?"
#define GAME_STATE_SAVE_GAME_YES    "YES, I DO"
#define GAME_STATE_SAVE_GAME_NO     "NO, THANKS"
#define GAME_STATE_SAVE_GAME_CANCEL "CANCEL"
#define GAME_STATE_SAVE_ENTER_NAME  "Enter name for your save:"

#define MAIN_MENU_DEFAULT           410
#define MAIN_MENU_NEW_GAME          411
#define MAIN_MENU_LOAD_GAME         412
#define MAIN_MENU_GAME_INFO         413

// -------------------- KEYS ------------------------

#define CONTROL_KEY_UP              'w'
#define CONTROL_KEY_DOWN            's'
#define CONTROL_KEY_LEFT            'a'
#define CONTROL_KEY_RIGHT           'd'
#define CONTROL_KEY_BACK            'q'
#define CONTROL_KEY_CONFIRM         'e'
#define CONTROL_KEY_WAVE            'f'
#define CONTROL_KEY_ERASE           'r'
#define CONTROL_KEY_MONEY_CHEAT     'm'

// ------------------- ERRORS -----------------------

#define ERROR_CODE_NO_ERROR         500
#define ERROR_CODE_FAILED_LOAD      501
#define ERROR_CODE_FAILED_PLACE_T   502
#define ERROR_CODE_WRONG_SAVE_NAME  505
#define ERROR_CODE_INS_FUNDS        506

#define ERROR_LINE_FAILED_LOAD      "I am sorry but I was not able to load your game. :("
#define ERROR_LINE_FAILED_PLACE_T   "You are not allowed to build here."
#define ERROR_LINE_WRONG_SAVE_NAME  "Sorry, save name may contain only [A-Za-z0-9_] with length 3-20. It also can't be named save_indexer."
#define ERROR_LINE_INS_FUNDS        "Sorry but you are poor. :("
#define ERROR_LINE_NO_TOKENS        "Sorry but I wasn't able to load towers or enemies. Game is unplayable. Sorry. :("

// ------------------- GAME STATS -------------------

#define GAME_STATS_INITIAL_HP       100
#define GAME_STATS_INITIAL_SCORE    0
#define GAME_STATS_INITIAL_MONEY    200

// ------------------- REGEX ------------------------

#define REGEX_NUMBER                "([0-9]+)"
#define REGEX_SAVE_PATH             "([A-Za-z_0-9.-]+)"
#define REGEX_MAP_PATH              "([A-Za-z_0-9.-]+)"
#define REGEX_NAME                  "([A-Za-z_0-9]{3,20})"
#define REGEX_TIME                  "([0-9.: ]+)"
#define REGEX_ENEMY_TOWER           "(ENEMY|TOWER)"
#define REGEX_ATTACK_TYPE           "ATTACK: (CROSS)$|^ATTACK: (AOE)"
#define REGEX_SPECIAL_TYPE          "SPECIAL: (GENERATE)$|^SPECIAL: (NONE)"
#define REGEX_LORE                  "([0-9A-Za-z '.:,?!]*)"
#define REGEX_CONTENT               "([!#-Z]{1})"
