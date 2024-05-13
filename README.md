## Project Proposal:

This is a platformer game where the player has to reach the end by avoiding or killing all the enemies.

## Project Description:

The project is divided into multiple parts:

**1. States**

It is an abstract class from which we are deriving several other classes such as MainMenuState, GameState, EditorState and SettingsState. Each of the derived classes are pushed onto a stack, and whichever state is on top that state object code is executed.

* **MainMenuState:** It consists of the New Game, Settings, Editor, Quit Button. This is the first state that is pushed onto the stack when you start the game.
* **GameState:** When you click the New Game button, the GameState object is pushed onto the stack. Any code inside GameState class is then executed such as loading of map, player, enemies, game engine, animation and much more.
* **EditorState:** When you click the Editor button, the EditorState object is pushed onto the stack. It basically contains the code for building a map. In it are functions of TileMap class that help us build the map and save them.
* **SettingsState:** When you click the Settings button, the SettingsState object is pushed onto the stack. It contains code to change the settings of your game and then save those settings in a config file so that next time your game loads with those settings.

**2. Entities**

It is an abstract class from which we are deriving two classes. Enemy and Player. These two derived classes are responsible for the enemies and the player you see on the screen.

* **Enemy:** It calls the constructor of entity to create an enemy with a texture. It consists of a few functions relating to the enemy such as moveEnemy which is responsible for moving the enemy by a certain amount in a certain direction. It also has the function playerAlive for seeing if player collides with the enemy.
* **Player:** It calls the constructor of the entity to create a Player with a texture. It consists of few function, such as updateMovement which is responsible for allows the user to move the player. This function implements the gravity and projectile motion on the player to allow for jumping correctly to the platform

**3. Animation**

This is a class that is responsible for managing all the animations of each sprite. It has a constructor that sets the rectangle to the first player sprite on the sprite sheet. It consists of updateAnimations function, this functions takes in parameters to define how far the rectangle is to be moved each time the function is called in classes such as Player and Enemy. All it does is moves the rectangle by a certain amount in an allocated time, giving the effect of animation.

**4. TileMap**

This is a class that uses the Tile object defined in the Tile class to create a 3d vector to contain all the tiles which is the map. It utilises the addToMap function to add tiles to a specific part on the vector, and removeFromMap to remove specific tiles from the vector. These functions are called to build a map. It also has filing functions such as saveToFile to save the drawn map in a file, and loadFromFile to load the map saved in a file to the vector. It also consists of functions that are responsible for moving or breaking the tiles depending upon the type of tile. And finally a function known as mapCulling, this function only loads the tiles in the vectors when those tiles are in the view, this saves alot of processing power as not all the tiles have to be loaded rather only some have to be loaded.

## OOP Utilized:

* Encapsulation
* Polymorphism
* Composition
* Inheritance
* Virtual Functions
* Abstraction

## Hurdles:

1. Player collision with each tile, so that player does not go through the tiles, rather only stands on it. The main issue that occured initially was that player kept teleporting to different locations on the tile when it collided with different areas. So fixing that took time, however this fix was done when the player was just a circle. When sprite was used instead of a circle the whole collision broke down, therefore had to fix that, and even now its not completely fixed, but fixed enough to work.
2. Player movement, moving the player correctly so that it lands on tiles. The biggest issues occured was that once a player started moving it did not stop so had to fix that.
3. Animations, settings up animations was difficult as hitboxes on the sprite sheet were not all equal, so had to use photoshop to near each sprite to lessen the hitbox. Had to also perform calculations to move the rectangle in an amount so that it covers each sprite on the sprite sheet correctly.
4. Filing, there was a time when there were vector issues, the file was not loading data to the vector correctly, therefore fixing it took some
