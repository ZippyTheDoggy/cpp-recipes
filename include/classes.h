#pragma once
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

class Item;

class Recipe {
    private:
        std::vector<std::pair<Item*, int>> _recipe;
        std::map<Item*, int> _counts;
    public:
        Recipe(std::vector<std::pair<Item*, int>> recipe) {
            for(std::pair<Item*, int>& pair : recipe) {
                const auto& [item, count] = pair;
                if(std::find(_recipe.begin(), _recipe.end(), pair) == _recipe.end())
                    _recipe.push_back(std::make_pair(item, count));
                if(_counts.find(item) == _counts.end())
                    _counts[item] = count;
                else _counts[item] += count;
            }
        }
        std::vector<Item*> getItems() {
            std::vector<Item*> items;
            for(std::pair<Item*, int>& pair : _recipe) {
                if(std::find(items.begin(), items.end(), pair.first) == items.end())
                    items.push_back(pair.first);
            }
            return items;
        }
        int getCount(Item* item) {
            if(_counts.find(item) == _counts.end()) return -1;
            return _counts.at(item);
        }
};

class Item {
    private:
        int _id = -1;
        std::string _name;
        inline static int _next_id = 1;
    public:
        std::vector<Recipe*> recipes;
        Item(std::string name) {
            _name = name;
            _id = Item::NextId();
        }

        std::string getName() {
            return _name;
        }
        int getId() {
            return _id;
        }

        int getNeededItems(Item* item) {
            int count = 0;
            for(Recipe* recipe : recipes) {
                std::vector<Item*> items = recipe->getItems();
                if(std::find(items.begin(), items.end(), item) != items.end()) {
                    count += recipe->getCount(item);
                }
            }
            if(count == 0) return -1;
            return count;
        }
        std::vector<std::pair<Item*, int>> getItemCounts() {
            std::vector<std::pair<Item*, int>> counts;
            for(Recipe* recipe : recipes) {
                std::vector<Item*> items = recipe->getItems();
                for(Item* item : items) {
                    if(item->recipes.size() > 0) {
                        for(auto& [i, count] : item->getItemCounts()) {
                            std::vector<std::pair<Item*, int>>::iterator fi = std::find_if(counts.begin(), counts.end(), [i](std::pair<Item*, int> e) {
                                return e.first == i;
                            });
                            if(fi != counts.end()) {
                                fi->second += count * recipe->getCount(item);
                            } else {
                                counts.push_back(std::make_pair(i, count * recipe->getCount(item)));
                            }
                        }
                    } else
                        counts.push_back(std::make_pair(item, recipe->getCount(item)));
                }
            }
            return counts;
        }

        void addRecipe(Recipe* recipe) {
            recipes.push_back(recipe);
        }

        static int NextId() {
            return _next_id++;
        }
};

class Counter {
    private:
        Item* _item;
        int _count = 1;
    public:
        Counter(Item* item, int count = 1) {
            _item = item;
            _count = count;
        }
        Item* getItem() {
            return _item;
        }
        int getCount() {
            return _count;
        }

        std::vector<std::pair<Item*, int>> getItemCounts() {
            std::vector<std::pair<Item*, int>> counts;
            auto a = _item->getItemCounts();
            for(auto& [item, count] : a) {
                if(std::find_if(counts.begin(), counts.end(), [item](std::pair<Item*, int> c) {
                    return c.first == item;
                }) == counts.end())
                    counts.push_back(std::make_pair(item, count * _count));
            }
            return counts;
        }

        static void print(Counter counter) {
            std::cout << counter.getItem()->getName() << " (" << counter.getCount() << "): {\n";
            for(auto& [item, count] : counter.getItemCounts()) {
                std::cout << "\t" << item->getName() << ": " << count << "\n";
            }
            std::cout << "}" << std::endl;
        }
        static void print(Item* item) {
            print(Counter(item, 1));
        }
};