#include <raylib.h>


enum class EnemyType {
    Normal,Fast,Tank,Boss
};

class Enemy{

public:

    Vector2 position{};

    float speed;

    float radius;

    int health;

    bool active;

    EnemyType type;

    int xpReward;

    float hitFlashTimer;

    bool isHit;

    int damage;

    Color color;

    Enemy(EnemyType enemyType);


    void Update(Vector2 playerPosition);

    void Draw();

};