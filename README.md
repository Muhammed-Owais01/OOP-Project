# Project Proposal

## Overview
This project is a platformer game where the player navigates through levels, avoiding or defeating enemies to reach the end.

## Project Description

### Parts
1. **States**: 
   - Abstract classes like MainMenuState, GameState, EditorState, and SettingsState manage different game states. These states are stacked, with the top one being active.
2. **Entities**: 
   - Abstract class for Player and Enemy entities, responsible for player and enemy behaviors respectively.
3. **Animation**: 
   - Manages sprite animations through the updateAnimations function.
4. **TileMap**: 
   - Handles the game map, including creation, modification, and loading/saving functions.

### States
- **MainMenuState**: Initial state with options like New Game, Settings, Editor, and Quit.
- **GameState**: Manages gameplay, loading maps, entities, and animations.
- **EditorState**: Allows map editing and saving.
- **SettingsState**: Manages game settings and saves them.

### Entities
- **Enemy**: Manages enemy behaviors and collision detection.
- **Player**: Controls player movement and collision detection with tiles.

### Animation
- Manages sprite animations, moving rectangles to simulate animation frames.

### TileMap
- Manages the game map, including loading, saving, and modifying tiles.

## Object-Oriented Programming Concepts Utilized
- Encapsulation
- Polymorphism
- Composition
- Inheritance
- Virtual Functions
- Abstraction

## Hurdles
1. **Player Collision**: Ensuring correct player-tile collision.
2. **Player Movement**: Stopping player movement accurately.
3. **Animations**: Setting up accurate animations.
4. **File Management**: Fixing issues with loading/saving map data.

## Possible Improvements
1. Fix player flying glitch and implement gravity properly.
2. Add checkpoints.
3. Introduce power-ups.
4. Allow player to jump on enemies.
5. Implement level endpoints.
6. Add enemy chase mechanics.
