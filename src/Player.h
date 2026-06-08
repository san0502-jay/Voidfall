#include <raylib.h>
#include "Weapon.h"
#include "config.h"

class Player {

    public:

    Vector2 position;

    float speed;

    float radius;

    int health;

    int maxhealth;

    int xp;

    int level;

    int xpToNextLevel;

    float damageCooldown;
    
    float damageTimer;

    Weapon weapon;

    Player();

    void Update();

    void Draw();

};