#include <raylib.h>
#include "config.h"

class Player {

    public:

    Vector2 position;

    float speed;

    float radius;

    int health;

    int xp;

    int level;

    int xpToNextLevel;

    float fireRate;

    Player();

    void Update();

    void Draw();

};