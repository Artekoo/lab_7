#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include <random>
#include <chrono>
#include <vector>
#include "head.h"
#include <istream>
#include <fstream>

void save_game(Character* character) {
    std::ofstream out ;

    out.open("save_game.txt", std::ios::out);
    if (out.is_open()) {
        out<<"Имя "<<character->getName()<<"\n";
        out<<"Уровень: "<<character->getLevel()<<"\n";
        out<<"Раса: "<<character->getRaceName()<<"\n"
        <<"Класс: "<<character->getClassName() <<"\n";
    }
    out.close();
}
 Weapon create_weapon(const std::string& name) {
    std::ifstream in("Weapons.txt");
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл Weapons.txt" << std::endl;
        // Возвращаем оружие по умолчанию или выбрасываем исключение
        return Weapon("default", 1.0, 0, 0, "none");
    }

    std::string line;
    while (std::getline(in, line)) {

        if (line.empty()) continue;


        size_t first_space = line.find(' ');
        if (first_space == std::string::npos) {
            std::cerr << "Ошибка формата в строке: " << line << std::endl;
            continue;
        }


        std::string weapon_name = line.substr(0, first_space);


        if (weapon_name != name) {
            continue;
        }

        try {

            std::string data = line.substr(first_space + 1);


            std::vector<std::string> tokens;
            std::istringstream iss(data);
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }


            if (tokens.size() < 4) {
                std::cerr << "Недостаточно данных для оружия: " << name << std::endl;
                continue;
            }


            double weapon_weight = std::stod(tokens[0]);
            int weapon_value = std::stoi(tokens[1]);
            int weapon_bonus_dmg = std::stoi(tokens[2]);


            std::string type_of_damage;
            for (size_t i = 3; i < tokens.size(); ++i) {
                if (i > 3) type_of_damage += " ";
                type_of_damage += tokens[i];
            }


            return Weapon(weapon_name, weapon_weight, weapon_value,
                         weapon_bonus_dmg, type_of_damage);

        } catch (const std::exception& e) {
            std::cerr << "Ошибка парсинга строки '" << line << "': " << e.what() << std::endl;
            continue;
        }
    }


    std::cerr << "Оружие с именем '" << name << "' не найдено." << std::endl;
    return Weapon("default", 1.0, 0, 0, "none");
}
int rollDice1d20() {

    static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> distribution(1, 20);

    return distribution(generator);
}
int rollDice1d6() {

    static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<int> distribution(1, 6);

    return distribution(generator);
}
int roll_for_ability() {
    int minimal = 7;
    int d1 = rollDice1d6();
    if (d1<minimal) { minimal = d1;}
    int d2 = rollDice1d6();
    if (d2<minimal) { minimal = d2;}
    int d3 = rollDice1d6();
    if (d3<minimal) { minimal = d3;}
    int d4 = rollDice1d6();
    if (d4<minimal) { minimal = d4;}
    int sum_of_3_max = d1+d2+d3+d4-minimal;
    return sum_of_3_max;
}
void print_abilities_info(Character* character) {
    std::cout<<"Характеристики вашего персонажа:\n";
    std::cout<<"Сила: "<<character->get_score("strength")<<"\n"
    <<"Ловкость: "<<character->get_score("dexterity")<<"\n"
    <<"Телосложение: "<<character->get_score("constitution")<<"\n"
    <<"Интеллект: "<<character->get_score("intelligence")<<"\n"
    <<"Мудрость: "<<character->get_score("wisdom")<<"\n"
    <<"Харизма: "<<character->get_score("charisma")<<"\n";
}
//(урон+модификаторы-класс брони
//добавить задавание характеристик
void fight(Character* you,Character* enemy,bool your_turn) {

    int dmg1 = rollDice1d20();
    int dmg2 = rollDice1d20();
    int damage_get = rollDice1d6();
    int damage_get2 = rollDice1d6();
    int dice_for_accurancy_for_you = dmg1 + you->get_bonus_damage() - enemy->get_ac();
    //на попадение
    int dice_for_accurancy_for_enemy = dmg2 + enemy->get_bonus_damage() - you->get_ac();

    if (your_turn) {
        if (dice_for_accurancy_for_you>0) {
            std::cout<<"Вы попали и нанесли "<<damage_get<<" урона противнику!\n";
            enemy->change_hp(damage_get);
        } else {std::cout<<"Вы не попали!\n";}
    }
        if (your_turn==false){
            if (dice_for_accurancy_for_enemy>0) {
                std::cout<<"Враг попал и нанес вам "<<damage_get2<<" урона!\n";
                you->change_hp(damage_get);
            } else {std::cout<<"Противник не попал!\n";}
        }
    }
int creating_hero() {
    Weapon club = create_weapon("club");
    Weapon knife = create_weapon("bloody_knife");
    Armor default_armor("Стандартная броня",1,0,10);
    Weapon default_weapon("Кулаки",0,0,0,"Дробящий");


    std::cout <<"Добро пожаловать в игру! Для начала введите имя персонажа: "<<std::endl;
    std::string char_name;
    std::getline(std::cin, char_name);
    char char_race;

    bool flag = true;
    while (flag) {
        std:: cout<<"Выберите расу\n1.Человек\n2.Эльф\n3.Дварф\n4.Тифлинг\n5.Полурослик\n6.Драконорожденный"<<std::endl;
        std::cin>>char_race;
        if (int(char_race)>48 && int(char_race)<55) {
            flag = false;
        }
    }

    Character* character;
    switch (char_race) {
        case '1':
            character = new Human(char_name);
            break;
        case '2':
            character = new Elf(char_name);
            break;
        case '3':
            character = new Dwarf(char_name);
            break;
        case '4':
            character = new Tiefling(char_name);
            break;
        case '5':
            character = new Halfling(char_name);
            break;
        case '6':
            character = new Dragonborn(char_name);
            break;
        default:
            std::cout<<"Error:expected race choice";
    }
    flag = true;
    char char_class;
    while (flag) {
        std:: cout<<"Выберите класс\n1.Жрец\n2.Воин\n3.Друид\n4.Чародей"<<std::endl;
        std::cin>>char_class;
        if (int(char_class)>48 && int(char_class)<53) {
            flag = false;
        }
    }
    flag = true;
    int ability_agree=0;
    bool first_iteration = true;
    while (flag) {

        if (first_iteration) {std::cout<<"Теперь определим значения характеристик броском костей 1к6\n";}
        if (!(first_iteration)) {std::cout<<"Перебрасываем..\n"; }
        character->set_score("strength",roll_for_ability());
        character->set_score("dexterity",roll_for_ability());
        character->set_score("constitution",roll_for_ability());
        character->set_score("intelligence",roll_for_ability());
        character->set_score("wisdom",roll_for_ability());
        character->set_score("charisma",roll_for_ability());

        print_abilities_info(character);
        std::cout<<"Вы согласны с характеристиками персонажа?\n1.Да\n2.Нет\n";
        std::cin>>ability_agree;
        if (ability_agree == 1) {
            flag = false;
        } else {first_iteration = false;}


    }
    switch(char_class) {
        case '1':
            character->setClass(Fighter);
            break;
        case '2':
            character->setClass(Cleric);
            break;
        case '3':
            character->setClass(Druid);
            break;
        case '4':
            character->setClass(Wizard);
            break;
        default:

            std::cout<<"Ошибка введения класса"<<std::endl;
    }
    character->setHP(14);
    character->setName(char_name);
    character->print_char_info();
    character->equip_Armor(&default_armor);
    character->equip_weapon(&default_weapon);
    std::cout<<"Теперь мы готовы начинать игру!\nВас отправили в город для закупки некоторых продуктов, необходимых вашему поселению\n ";
    save_game(character);

    flag = true;
    SetConsoleOutputCP(CP_UTF8);
    int first_choose;
    int second_choose;
    std::cout<<"Проходя по лесу, вы замечаете, что вокруг стало необыкновенно безлюдно\n"<<"Вы идете дальше и замечаете несколько трупов городских жителей";
    while (flag) {
        std::cout<<"\n1.Исследовать труп городского\n2.Пройти дальше в город\n3.Вернуться назад\n4.Посмотреть инвентарь\n";
        std::cin>>first_choose;
        if (first_choose>=1 && first_choose<=3) {
            flag = false;
        }
        if (first_choose==4) {
            character->showInventory();
        }
    }
    if (first_choose==1) {
        flag = true;
        std::cout<<"Вы обнаруживаете множество колотых ран на теле городского жителя. \nВы пытаетесь нащупать пульс, но понимаете что житель мертв";
        while (flag) {
            std::cout<<"Вы находите заморавшийся кровью клинок, лежащий возле трупа.\n1.Подобрать и экипировать\n2.Пойти дальше в город\n3.Посмотреть инвентарь\n";
            std::cin>>second_choose;
            if (second_choose>=1 && second_choose<=3) {
                flag = false;
            }
        }

        switch (second_choose) {
            case 1:
                character->pickUpItem(knife);
                character->equip_weapon(&knife);
                knife.display_Weapon();
                std::cout<<"Вы проходите в город и видите опустевший рынок, полный трупами городских жителей";
                return 1;
            case 2:
            std::cout<<"Вы проходите в город и видите опустевший рынок, полный трупами городских жителей";
                return 1;

            case 3:
                character->showInventory();
                break;
            default:
                std::cout<<"Ошибка";
        }
    }
    int third_choose;
    if (first_choose==2) {
        std::cout<<"Вы проходите в город и видите опустевший рынок, полный трупами городских жителей";
        return 1;
    }
    Character* enemy_goblin = new Halfling("Гоблин");
    enemy_goblin->setHP(9);
    enemy_goblin->setLevel(1);
    enemy_goblin->setClass(Fighter);
    Armor leather_armor("Кожаная броня",1.5,60,12);

    enemy_goblin->equip_Armor(&leather_armor);
    enemy_goblin->equip_weapon(&default_weapon);
    int fourth_choose;
    bool win = false;
    bool flag2 = true;
    save_game(character);
    if (first_choose==3) {
        std::cout<<"\nВы встречаете гоблина, возвращающегося из леса. Он немедленно становится враждебным.\n";

        flag = true;
        while (flag) {
            std::cout<<"1.Атаковать\n2.Посмотреть информацию о противнике\n3.Посмотреть инвентарь\n";
            std::cin>>third_choose;
            if (third_choose>=1 && third_choose<=3) {
                flag = false;
            }

            if (third_choose == 1) {
                flag = false;
                bool is_fighting = true;

                character->set_turn(true);
                enemy_goblin->set_turn(false);

                while (is_fighting) {
                    if (character->get_your_turn()) {
                        fight(character, enemy_goblin, true);

                        if (enemy_goblin->getHP()<=0) {
                            is_fighting = false;
                            std::cout<<" HP противника упало ниже нуля, вы выиграли!";
                            win = true;
                            break;

                        }
                        enemy_goblin->print_char_info();
                        character->set_turn(false);
                        enemy_goblin->set_turn(true);
                        flag2=true;
                    }
                    if (enemy_goblin->get_your_turn()) {

                        fight(character, enemy_goblin, false);
                        if (character->getHP()<=0) {
                            is_fighting = false;
                            std::cout<<"Ваше HP опустилось ниже нуля! Вы проиграли";
                            return 1;
                        }
                        std::cout<<"Информация о вас:\n";
                        character->print_char_info();
                        character->set_turn(true);
                        enemy_goblin->set_turn(false);
                        flag2=true;
                    }

                        while (flag2) {
                            std::cout<<"Вы желаете продолжить сражение?\n1.Продолжить сражение\n2.Сбежать с поля битвы\n3.Информация о противнике\n4.Информация о вас\n";
                            std::cin>>fourth_choose;
                            if (fourth_choose>=1 && fourth_choose<=2) {
                                flag2 = false;
                            }
                            if (fourth_choose==1) {
                                continue;
                            }


                            if (fourth_choose==2) {
                                is_fighting = false;
                                std::cout<<"Вы покинули бой";
                            }
                            if (fourth_choose==3) {
                                enemy_goblin->print_char_info();
                            }
                            if (fourth_choose==4) {
                                character->print_char_info();
                            }
                        }


                }
            }
                if (third_choose== 2) {
                    enemy_goblin->print_char_info();
                    flag=true;
                }
                if (third_choose== 3) {
                    character->showInventory();
                    flag = true;
                }

            }
        }
    if (win==true) {
        character->levelUp();
        std::cout<<"Ваш уровень повышен! Характеристики повысились\n";
        save_game(character);
        return 1;
        // character->getAbilities();
    }

    }