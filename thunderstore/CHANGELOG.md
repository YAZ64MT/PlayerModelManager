### 0.5.0
* Add support for replacing individual pieces of equipment
  * Can be specified on a per-form-model-type basis
* Added built-in equipment model entries
* Added ability to replace the ray emitted by the Mirror Shield
* Added support for model packs
  * Allows mod creators to have multiple model types equipped as a set
* Adult models built for this version or later fall back to adult-sized equipment
* Update UI to allow model categories to be directly selected
* Removed ModelReplacer optional dependency
* Concept of "optional models" formalized
  * These models are prefixed with PMM_DL_OPT
  * If an optional model isn't specified, it will fall back to another model before falling back to the vanilla models
  * e.g. if PMM_DL_OPT_FPS_BOW isn't specified, it will fall back to PMM_DL_BOW before falling back to the vanilla model
* Fix Blast Mask cooldown corrupting tunic color
* Mod will no longer attempt to write values to En_Torch2 or En_Boom variables while their overlays are not loaded
* Rewrote much of the mod under the hood for better extensibility

### 0.4.0
* Add support for custom Fierce Deity Elegy of Emptiness statues
* Add API call for changing the tunic color on a per form basis
* Add API call for overriding display lists used when no custom model is present
* Add API call for overriding matrixes when no custom matrix is present
* Add API call for checking if a custom player model is equipped to a particular form
* Bumped API version
* Fix Zora boomerangs not being replaced
* Fix Goron shielding skeleton not being replaced

### 0.3.1
* Fix vanilla Deku Link's flower propellers using the wrong model
* Fix mask matrix not being properly reset when changing models
* Fix custom arrow display lists not being used
* Bump minimum recomp version to 1.2.2

### 0.3.0
* ModelReplacer is now an optional dependency
* Add first person bow/slingshot replacement options
* Adult bow and arrow positions now match OoT
* Bump required version of YAZMT Core
* Fix Fierce Deity vanilla model using wrong bottle hand
* Fix Kafei using adult door animation when adult model is equipped
* More consistently use the right models when multiple Links are being drawn

### 0.2.1 (Pre-Thunderstore)
* Fix player form properties being applied too early when removing a transformation mask
* Fix equipment being drawn on the wrong limb if a custom model's fists were connected to the forearm
* Fix milk bar sound check cutscenes using incorrect face textures & display lists
* Player skeleton no longer interpolates between positions right after switching models

### 0.2.0 (Pre-Thunderstore)
* Add API function for other mods to override custom tunic color
  * Override can be ignored by user in mod config
* Update adult human running speed to match Zora Link
* Fix adult Link clipping into the floor for a frame when unpausing
* Fix equipment rendering on the wrong limb with certain models

### 0.1.0 (Pre-Thunderstore)
* Initial release
