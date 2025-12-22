#ifndef PROJECT_7_HEAD_H
#define PROJECT_7_HEAD_H

#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <chrono>

int roll_for_ability();
int creating_hero();
enum  Race {
    HUMAN,
    DWARF,
    ELF,
    HALFLING,
    DRAGONBORN,
    TIEFLING
};
enum classes_of_characters {
    Fighter,
    Cleric,
    Druid,
    Wizard
};
class Item {
private:
    std::string name;

    double weight;
    int value;
    bool isMagic;

public:

    Item(std::string itemName,  double itemWeight, int itemValue, bool magic = false)
        : name(itemName),  weight(itemWeight), value(itemValue), isMagic(magic) {}

    std::string getName() const { return name; }
    double getWeight() const { return weight; }
    int getValue() const { return value; }
    bool getIsMagic() const { return isMagic; }

    void display() const {
        std::cout << name;
        if (isMagic) std::cout << " [МАГИЧЕСКИЙ]";
        std::cout << "\n  Вес: " << weight << " килограммов"<< "\n  Стоимость: " << value << " золотых" << std::endl;
    }
};

class Weapon : public Item {
private:
    int damage;

    std::string damageType;

public:
    Weapon(std::string name,  double weight, int value,
           int dmg, std::string dmgType)
        : Item(name,  weight, value), damage(dmg), damageType(dmgType) {}

    int getDamage() const { return damage; }
    std::string getDamageType() const { return damageType; }
    void display_Weapon() {
        std::cout<<"Урон оружия 1d6+"<<getDamage()<<
            "\nТип урона "<<getDamageType()<<"\n";
    }
};
class Armor : public Item {
private:
    int armorClass;

public:
    Armor(std::string name,  double weight, int value, int ac)
        : Item(name, weight, value), armorClass(ac) {}

    int getArmorClass() const { return armorClass; }
};
class Inventory {
private:
    std::vector<Item> items;
    double maxWeight;
    double currentWeight;

public:
    Inventory(double capacity = 100.0) : maxWeight(capacity), currentWeight(0.0) {}

    bool addItem(const Item& newItem) {
        if (currentWeight + newItem.getWeight() <= maxWeight) {
            items.push_back(newItem);
            currentWeight += newItem.getWeight();
            std::cout << "Предмет \"" << newItem.getName() << "\" добавлен в инвентарь." << std::endl;
            return true;
        } else {
            std::cout << "Слишком тяжело! Нельзя добавить \"" << newItem.getName();

            return false;
        }
    }



    void display() const {
        std::cout << "\n=== ИНВЕНТАРЬ ===" << std::endl;
        std::cout << "Вес: " << currentWeight << "/" << maxWeight << " килограммов" << std::endl;
        std::cout << "Слоты: " << items.size() << "\n" << std::endl;

        if (items.empty()) {
            std::cout << "Инвентарь пуст." << std::endl;
        } else {
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << i+1 << ". ";
                items[i].display();
                std::cout << std::endl;
            }
        }
    }


    double getCurrentWeight() const { return currentWeight; }
    double getMaxWeight() const { return maxWeight; }
};
class AbilityScores {
private:
    std::unordered_map<std::string, int> scores;

public:
    AbilityScores() {
        scores["strength"] = 10;
        scores["dexterity"] = 10;
        scores["constitution"] = 10;
        scores["intelligence"] = 10;
        scores["wisdom"] = 10;
        scores["charisma"] = 10;
    }

    void setScore(const std::string& ability, int value) {
        scores[ability] = value;
    }

    int getScore(const std::string& ability) const {
        return scores.at(ability);
    }

    // Модификатор = (значение - 10) / 2
    int getModifier(const std::string& ability) const {
        return (scores.at(ability) - 10) / 2;
    }

    void increaseScore(const std::string& ability, int amount = 1) {
        scores[ability] += amount;
    }
};

class Character {
protected:
    int XP;
    bool is_your_turn = false;
    Weapon* equiped_weapon;
    Armor* equiped_armor;
    Inventory Inventory;
    std::string name;
    Race race;
    AbilityScores abilities;
    int level;
    classes_of_characters clas;
    int hp;
    int increase_dmg_with_weapon = 0;
public:
    void set_score(std::string ability, int new_value) {
        abilities.setScore(ability,new_value);
    }
    int get_score(std::string ability) const { return abilities.getScore(ability); }
    bool get_your_turn() const { return is_your_turn; }
    void set_turn(bool it_it_your_turn) {
        is_your_turn = it_it_your_turn;
    }
    Character(const std::string& charName, Race charRace)
        : name(charName), race(charRace), level(1) {}

    virtual ~Character() = default;
    bool pickUpItem(const Item& item) {
        return Inventory.addItem(item);
    }
    void equip_weapon(Weapon* weapon) {
        equiped_weapon = weapon;
    }
    void equip_Armor(Armor* armor) {
        equiped_armor = armor;
    }
    int get_XP(int receive_XP) {
        return XP += receive_XP;
    }
    int get_ac() {
        return equiped_armor->getArmorClass();
    }
    int get_bonus_damage() {
        return equiped_weapon->getDamage();
    }
    void showInventory() const {
        Inventory.display();
    }
    int getHP() const {return hp;}
    std::string getName() const { return name; }
    Race getRace() const { return race; }
    int getLevel() const { return level; }


    void change_hp(int damage) {
        hp -=damage;
    }
    void setHP(int newHp) { hp = newHp; }
    void setClass(const classes_of_characters newClass) { clas = newClass;}
    void setName(const std::string& newName) { name = newName; }
    void setRace(Race newRace) { race = newRace; }
    void setLevel(int newLevel) { level = newLevel; }
    std::string getClassName() const {
        switch(clas) {
            case Fighter: return "Воин";
            case Cleric: return "Жрец";
            case Druid: return "Друид";
            case Wizard: return "Чародей";
            default: return "Неизвестный класс";
        }
    }
    void print_char_info() {
        std::cout << "Информация о персонаже: " << std::endl;
        std::cout << "Имя: " << getName() << std::endl;
        std::cout << "Класс: " << getClassName() << std::endl;
        std::cout << "Раса: " << getRaceName() << std::endl;
        std::cout << "HP: " << getHP() << std::endl;
        std::cout << "Level: " << getLevel() << std::endl;
    }
    virtual void applyRaceBonuses() = 0;
    virtual std::string getRaceName() const = 0;


    void levelUp() {
        level++;
        // улучшения характеристик при повышении уровня
    }


};

class Dwarf : public Character {
public:
    Dwarf(const std::string& name) : Character(name, Race::DWARF) {
        applyRaceBonuses();
    }

    void applyRaceBonuses() override {
        abilities.increaseScore("constitution", 2);
    }

    std::string getRaceName() const override {
        return "Дварф";
    }
};

class Elf : public Character {
public:
    Elf(const std::string& name) : Character(name, Race::ELF) {
        applyRaceBonuses();
    }

    void applyRaceBonuses() override {
        abilities.increaseScore("dexterity", 2);
    }

    std::string getRaceName() const override {
        return "Эльф";
    }
};

class Human : public Character {
public:
    Human(const std::string& name) : Character(name, Race::HUMAN) {
        applyRaceBonuses();
    }

    void applyRaceBonuses() override {

        abilities.increaseScore("strength");
        abilities.increaseScore("dexterity");
        abilities.increaseScore("constitution");
        abilities.increaseScore("intelligence");
        abilities.increaseScore("wisdom");
        abilities.increaseScore("charisma");
    }

    std::string getRaceName() const override {
        return "Человек";
    }
};
class Tiefling : public Character {
    public:
    Tiefling(const std::string& name) : Character(name, Race::TIEFLING) {
        applyRaceBonuses();
    }
    void applyRaceBonuses() override {
        abilities.increaseScore("intelligence",1) ;
        abilities.increaseScore("charisma",2);
    }
    std::string getRaceName() const override {
        return "Тифлинг";
    }
};
class Halfling : public Character {
    public:
    Halfling(const std::string& name) : Character(name, Race::HALFLING) {
        applyRaceBonuses();
    }
    void applyRaceBonuses() override {
        abilities.increaseScore("dexterity",2) ;
    }
    std::string getRaceName() const override {
        return "Полурослик";
    }
};
class Dragonborn : public Character {
    public:
    Dragonborn(const std::string& name): Character(name, Race::DRAGONBORN) {
        applyRaceBonuses();
    }
    void applyRaceBonuses() override {
        abilities.increaseScore("strength",2) ;
        abilities.increaseScore("charisma",1) ;
    }
    std::string getRaceName() const override {
        return "Драконорожденный";
    }
};
void chapter_1(Character &Character);


#endif // PROJECT_7_HEAD_H