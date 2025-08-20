#pragma once
class Shop
{
Properties:
    List of Item objects called 'inventory'

        Function PopulateShop()
        // Adds a selection of items to the 'inventory' list.
        // Example:
        // Create new Item("Health Potion", "Restores health", 20)
        // Add to 'inventory'
        // Create new Item("Sword", "Increases damage", 50)
        // Add to 'inventory'

        Function DisplayItems()
        // Loop through the 'inventory' list.
        // For each item, print its name, description, and cost.

        Function Sell(Player player, Item item)
        // Check if the player has enough gold.
        If player.gold >= item.GetCost() then
        // Deduct the gold from the player.
        player.gold = player.gold - item.GetCost()
        // Give the item to the player.
        player.AddItem(item)
        // Provide a success message.
        Print "Transaction successful. Player bought " + item.name.
        Return true.
        Else
        // Provide a failure message.
        Print "Not enough gold!"
        Return false.
};

