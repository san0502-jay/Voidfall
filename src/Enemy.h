#include <raylib.h>

class Enemy{

public:

    Vector2 position{};

    float speed;

    bool active;

    Enemy();


    void Update(Vector2 playerPosition);

    void Draw();

};