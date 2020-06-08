## Radioactive donut

### Short description
2015 winter semester Computer Graphics course project.
Game was implemented in C using OpelGL 1.* (fixed pipeline).

Goal of the game is to protect atoms using shields, from radioactive beams emitted by the donut. Radiation 'ejects' electrons from their orbit and harms the atom. When atom losses all its electrons it becomes radioactive and starts complicating player's defence by emitting various negative effects (slow player down, reduce his range...).

### Build:
`make` inside src directory.

### Usage:
`./radioactive_donut` after build.

### Controls:
To start, move the player!

| Key | Description|
| :-------------: | :-----------------: |
| `W`,`A`,`S`,`D` | move player |
| `P` | pause game |
| `space` | 'jetpack' |
| `mouse move` | rotate the player |
| `mouse scroll` | adjust target length |
| `left click` | put shield |
