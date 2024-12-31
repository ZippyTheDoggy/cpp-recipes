#include <iostream>

#include "include/classes.h"

int main() {

	Item* steel_ingot = new Item("Steel Ingot");
	Item* glass = new Item("Glass");
	
	Item* iron_ingot = new Item("Iron Ingot");
	Item* iron_gear = new Item("Iron Gear");
	Item* machine_core = new Item("Machine Core");
	
	Item* machine_frame = new Item("Machine Frame");

	Recipe* iron_ingot__to__iron_gear = new Recipe({
		{iron_ingot, 4}
	});
	Recipe* recipe_machine_core = new Recipe({
		{iron_ingot, 3},
		{iron_gear, 3}
	});
	Recipe* recipe_machine_frame = new Recipe({
		{steel_ingot, 2},
		{glass, 3},
		{machine_core, 4},
	});

    iron_gear->addRecipe(iron_ingot__to__iron_gear);
    machine_core->addRecipe(recipe_machine_core);
    machine_frame->addRecipe(recipe_machine_frame);

	// EXPECTED RECIPE:
	/*
		Steel Ingot: 2
		Glass: 3
		Iron Ingot: 60
	*/

    // ONE ITEM:
    Counter::print(machine_frame);

    // MULTIPLE ITEMS:
    Counter counter(machine_frame, 5);
    Counter::print(counter);

    // OUTPUT FORMAT:
    /*
        Machine Frame (1):
            Steel Ingot: 2
            Glass: 3
            Iron Ingot: 60
        }
        Machine Frame (5):
            Steel Ingot: 10
            Glass: 15
            Iron Ingot: 300
        }
    */

    return EXIT_SUCCESS;
}