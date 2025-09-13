# PlayerModelManager for Majora's Mask: Recompiled

This is an API mod for Majora's Mask: Recompiled that simplifies Link model 
replacement and adds a menu that lets the user swap  between models for each 
form.

If you want to make a model mod that is compatible with this API, add 
[this header file](https://github.com/YAZ64MT/PlayerModelManager/blob/main/include/playermodelmanager_api.h) 
to your mod's includes and the mod to your dependencies in your toml:

```toml
dependencies = [
    "yazmt_mm_playermodelmanager:0.4.0",
]
```

Note that this mod does not have any built-in models or allow models to be 
read in directly from the user's PC. If you have no mods that use this mod
as a dependency, then no models will appear in the menu.

![](https://github.com/YAZ64MT/PlayerModelManager/blob/main/images/menu_demo.gif?raw=true)

THIS MOD DOES NOT PROVIDE ANY MODELS ON ITS OWN. YOU MUST SOURCE YOUR MODELS
FROM OTHER MODS.

If you are looking to load ".zobj" models, then see this mod's [filesystem
addon](https://thunderstore.io/c/zelda-64-recompiled/p/YAZMT/Player_Model_Manager_Filesystem_Addon/).
