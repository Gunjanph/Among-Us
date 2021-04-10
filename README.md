# Among US
An 2D game in OpenGL(version 3+), inspired from the Among Us aiming to get comfortable with making objects of
greater complexity with OpenGL, working with many interacting components and understanding how applica-
tions are built with OpenGL.

## Instructions

### Run

```bash
mkdir build
cd build
cmake..
make
./Among-Us
```
### Play

- `LEFT ARROW` - Move player left
- `RIGHT ARROW` - Move player right
- `UP ARROW` - Move player up
- `DOWN ARROW` - Move player down
- `T` - To turn on or off lighting

## Game
This game’s objective is to exit a maze you have been dropped into after finishing two tasks. You must hurry
because reactor meltdowns do not fix themselves.
1. ### World
- A procedural 2D maze is designed having different layout in every game.
- Maze have a single exit.
- Collision with walls is taken into consideration.

2. ### Player
- A player having some assigned health.
- A player can only exit after complting the 2 tasks.

3. ### Light
- When light is turned off, player can say only limited part of the maze.
- The time spent in dark-mode increases the health of the player.

4. ### Heads Up Display
- #### Health:
>This indicates the player’s points, which can increase or decrease based on events/interactions.
- #### Tasks:
>Tasks completed / Total number of tasks available.
- #### Light:
>`On/Off` indicated the status of the central lighting system.
- #### Time:
>If player stays more than 100 sec, the game gets over.

5. ### Imposter
- Starts at the random position in maze.
- Move towards the player using shortest path algorithm.
- The blue button vapourises the imposter.
- If imposter reaches the player, the game ends.

6. ### Tasks
- A purple button needs to be pressed to get 2 powerups(yellow button) and a bomb(red button).
- Health increases on picking up powerups and decreases on picking up bomb. 

## Bonus

- Characters are drawn as real Among Us characters.
- Shortest path algorithm is used instead of brute force for the imposter to find the player.
- `You Won` / `Game Over` / `You Lost` screen is shown.