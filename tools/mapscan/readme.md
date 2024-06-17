## Mapscan
Dumb attempt -*working* attempt- to convert a .png map from  [spriters-ressource.com](https://www.spriters-resource.com/game_boy_gbc/pokemoncrystal/) into a .dat in my map format.

Tried with bitmap but... I failed...

Convert the `png` into `css` in Aseprite, then remove the first lines of informations, and at the end change the `; }` to `,`.

```
$> ./mapscan <map>.css <tileset>.css <tile size>
```

The program parses the tileset's pixels channels (R,G,B), then compare it pixel by pixel to the map.
It outputs a `output.dat` file.

TODO:
- rewrite it with bitmap support because it's fun
- remove the necessity to erase the first lines of the `css` file and all
- automate it with a bash script and `aseprite-cli`
