#include <raylib.h>
#include "config.h"

class Player {

    public:

    Vector2 position;

    float speed;

    float radius;

    Player();


    void Update();

    void Draw();

};