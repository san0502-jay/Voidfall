#include <raylib.h>

class Enemy{

public:

    Vector2 position{};

    float speed;

    Enemy();


    void Update(Vector2 playerPosition);

    void Draw();

};