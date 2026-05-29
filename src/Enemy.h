#include <raylib.h>

class Enemy{

public:

    Vector2 position{};

    float speed;

    int health;

    bool active;

    Enemy();


    void Update(Vector2 playerPosition);

    void Draw();

};