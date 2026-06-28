#include <iostream>
#include <string>
#include <string_view>
#include <span>
#include <thread>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <array>
#include <cstring>

using namespace std::chrono_literals;

// Enum State
enum class CombatMode : uint8_t {
    Attack,
    Defence,
    None
};

enum class StoryState : uint8_t {
    Intro,
    IntroPahlawan,
    IntroMusuh
};

enum class EntityType : uint8_t{
    player,
    enemy
};

enum class WeaponType : uint8_t{
    Melee,
    HeavyMelee,
    Range,
    HeavyRange,
    Staff,
    Wand
};

// Struct
struct Weapon{
    std::string_view name;
    WeaponType type;
    short level;
    short damage;
};

struct Stats{
    std::string_view name;
    EntityType type;
    Weapon useWeapon;
    short experience; 
    short level;
    short health;
    short damage; 
    short defence; 
};

// Stats Init
Stats player;
Stats enemy;

// Weapon Init
Weapon broadSword;
Weapon club;
Weapon heavySword;

// Combat Init
CombatMode selectMode;

StoryState state = StoryState::Intro;

// Core Function
void GameProcessing();
void LoadParagraph(StoryState state);
void Delay(short durations);
void Attack(short damage,Stats& target);
void AttackerInformation(Stats& entity);
void RandomSeedInitialized();
void CombatProcess(Stats& a, Stats& b);
void AttackTurn(Stats& a, Stats& b);
void TurnSelection();
void AdjustingStatsWithWeapon(Stats& p, Stats& e);

bool ResultCheck(Stats& a, Stats& b);

short RandomNumberGenerator();

std::array<char,12> CommandInput();

std::span<const std::string_view> TakingParagraph(StoryState state);

CombatMode CombatModeSelectionProcess(Stats& p, Stats& e);

// UI Display
void DisplayMainMenu();
void CombatDisplay(Stats& p, Stats& e);
void ModeSelectionDisplay();
void EnemyCombatInformationDisplay(Stats& e);
void PlayerCombatInformationDisplay(Stats& p); 

//Process
void MainMenuProcess();
void CombatProcess(Stats& p, Stats& e);

// Variabe
constinit bool gameState{true};
constinit unsigned short processID {0};

// Dictionary
std::unordered_map<StoryState, std::vector<std::string_view>> dialogue_game;

void ParagraphInitialized(){
   dialogue_game[StoryState::Intro]= {
         "EMPTY",
         "EMPTY"
    };
    dialogue_game[StoryState::IntroPahlawan]= {
         "EMPTY",
         "EMPTY"
    };
}

void WeaponInitialized(){
    broadSword = {"Broad Sword", WeaponType::Melee, 1, 7};
    heavySword = {"Heavy Sword", WeaponType::HeavyMelee,1, 23};
    club = {"Club", WeaponType::Melee, 1 , 4};
}

void EntityStatsInitialized(){
   player = {"Player", EntityType::player, broadSword,0,12, 100, 5, 21};
   enemy = {"Enemy", EntityType::enemy, club,0,8, 100, 5, 14};
}

void RandomSeedInitialized(){
    srand(static_cast<unsigned int>(time(nullptr)));
}

void DataInitialized(){
    WeaponInitialized();
    ParagraphInitialized();
    EntityStatsInitialized();
    RandomSeedInitialized();
}

int main(){
    DataInitialized();
    TurnSelection(); 
    // GameProcessing();
    return 0;
}

// Core Function
void GameProcessing(){         
    while(gameState){
        switch(processID){
            case 0:
                MainMenuProcess();
                break;
            case 1:
                
                break;
            case 2:
                // exit();
                break;
            default:
                std::cout<< "There is something wrong!" <<std::endl;
                processID = 0;
                break;
        }
    }
}

std::array<char,12> CommandInput(){  
    std::array<char,12> input;
    while(true){
        std::cout << "Input: ";
        std::cin.getline(input.data(),input.size());

        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(10000,'\n');
            return input; 
        }
        return input;
    }
}

std::span<const std::string_view> TakingParagraph(StoryState state){
    if(!dialogue_game.contains(state)) return {};
    return dialogue_game[state]; 
}

void LoadParagraph(StoryState state){
   std::span<const std::string_view> paragraph = TakingParagraph(state);
   for(short i {0} ; i < paragraph.size() ; ++i)
     {
        for(short j{0} ; j < paragraph[i].length() ;++j)
        {
            std::this_thread::sleep_for(20ms);
            std::cout << paragraph[i][j];
            std::cout.flush();
        }
        std::cout << std::endl;
     }
}
short RandomNumberGenerator(){
    short coin = static_cast<short>(rand() % 2);
    return coin;
}

void Attack(short damage, Stats& target){
   target.health-=damage;
}

CombatMode CombatModeSelection(Stats& a, Stats& b){
   CombatDisplay(a, b);
   return CombatMode::None;
}

// Process
void TurnSelection(){
    short turn = RandomNumberGenerator();
    if(turn == 0){
        CombatProcess(player, enemy);
    }
    else if(turn == 1){
        CombatProcess(enemy, player);
    }
}

// Combat Process
void CombatProcess(Stats& a, Stats& b){
    
    bool yourTurn {false};
    bool oneTimeCheck {true};

    Stats* playerPtr = nullptr;
    Stats* enemyPtr = nullptr;

    if(oneTimeCheck){
          if(a.type == EntityType::player){ 
                playerPtr = &a;
                enemyPtr = &b;
                yourTurn = true;
          }
          else{
            playerPtr = &b; 
            enemyPtr = &a;
          }
    }
    while(true){
        AdjustingStatsWithWeapon(a,b);
        if(yourTurn){
            
            std::cout << "YOUR TURN" << std::endl;
            std::this_thread::sleep_for(1500ms);
            std::system("clear");

            CombatMode decision = CombatModeSelectionProcess(*playerPtr, *enemyPtr);
           
            if(decision == CombatMode::Attack){
                AttackTurn(*playerPtr, *enemyPtr);
            }
            else{
                std::system("clear");
                std::cout << "Defence mode!!" << std::endl; 
                std::this_thread::sleep_for(1500ms);
            }
            
            if(ResultCheck(a,b)){ break; }

            yourTurn = false;
            std::system("clear");
        }
       
        std::cout << "ENEMY TURN" << std::endl;
        std::this_thread::sleep_for(1500ms);       
        std::system("clear");
        AttackTurn(*enemyPtr, *playerPtr);

        if(ResultCheck(a,b)){ break; }
        yourTurn = true;
        std::system("clear");
    }
}

void AttackTurn(Stats& a, Stats& b){
    Attack(a.damage, b);
    if(a.health <= 0 || b.health <= 0){return;}
}

bool ResultCheck(Stats& a, Stats& b){
  if(a.type == EntityType::player && a.health <= 0){
    std::cout << "You lose!" <<std::endl;
    return true; 
  } else if(b.type == EntityType::player && b.health <= 0){ std::cout << "You lose!" << std::endl; return true; 
  }
  else if(a.type != EntityType::player && a.health <= 0){
    std::cout << "You Win!" << std::endl;
    return true;
  }
  else if(b.type != EntityType::player && b.health <= 0){
    std::cout << "You Win!" << std::endl;
    return true; 
  }
  return false;
}

// Main Menu Process, selection play,about, or exit.
void MainMenuProcess(){
   
    DisplayMainMenu();
    auto input = CommandInput();

    if(strcmp(input.data(), "p") == 0 || strcmp(input.data(), "1") == 0 || strcmp(input.data(), "play") == 0){
        std::cout << "Berhasil" << std::endl;
        processID = 1;
    }else if(strcmp(input.data(), "a") == 0 || strcmp(input.data(), "2") == 0 || strcmp(input.data(), "about") == 0){
        std::cout << "Berhasil" << std::endl;
        processID = 1;
    }else if(strcmp(input.data(), "e") == 0 || strcmp(input.data(), "3") == 0 || strcmp(input.data(), "exit") == 0){
        std::cout << "Berhasil" << std::endl;
        processID = 1;
    }
    // std::system("clear");
}

void AdjustingStatsWithWeapon(Stats& p, Stats& e){
   p.damage = p.useWeapon.damage; 
   e.damage = e.useWeapon.damage; 
}

CombatMode CombatModeSelectionProcess(Stats& p, Stats& e){

    CombatMode mode;
    while (true)
    {
        CombatDisplay(p,e);
        ModeSelectionDisplay();

        std::array<char, 12> inputRaw = CommandInput();

        std::string_view input = inputRaw.data();
        
        if(input == "1" || input == "attack"){
            return CombatMode::Attack; 
        }
        else if(input == "2" || input == "defence"){
            return CombatMode::Defence; 
        }else{
            std::system("clear");
            std::cout << "WRONG INPUT!!" << std::endl;
            std::this_thread::sleep_for(500ms);
            std::system("clear");
        }
    } 
   return CombatMode::None;
}

// Display Function
void DisplayMainMenu(){
    std::cout <<    "Main Menu" << std::endl;
    std::cout <<    "1. Play"    << std::endl;
    std::cout <<    "2. About"  <<std::endl;
    std::cout <<    "3. Exit"  <<std::endl;
}

void EnemyCombatInformationDisplay(Stats& e){
    std::cout << enemy.name << "Level: " << enemy.level << std::endl;
    std::cout << "Health: " << enemy.health << "\tdefence: "<< enemy.defence << std::endl; 
}

void PlayerCombatInformationDisplay(Stats& p){
    std::cout << "Health: " << p.health << "\tdefence: "<< p.defence << std::endl; 
    std::cout << p.name << "Level: " << p.level << std::endl; 
}

void ModeSelectionDisplay(){
    std::cout << "\nChoose:" << std::endl; 
    std::cout << "1. Attack" << std::endl; 
    std::cout << "2. Defence" << std::endl; // MASIH TAMPILAN ABILITY 
}

void CombatDisplay(Stats& p, Stats& e){
    EnemyCombatInformationDisplay(e); 
    std::cout << std::endl;
    std::cout << std::endl;
    PlayerCombatInformationDisplay(p);
}
