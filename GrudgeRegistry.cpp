#include"GrudgeRegistry.h"


GrudgeRegistry::GrudgeRegistry() : count(0) {

	for (int i = 0; i < MAX_GRUDGE_ENEMIES ;i++) 
	{
		timers[i] = 0.0f;
		pool[i] = nullptr;


	}


}

GrudgeRegistry::~GrudgeRegistry() 
{
	clear();

}

int GrudgeRegistry::getCount() const 
{
	return count;


}



void GrudgeRegistry::registerEnemy(Enemy* E) 
{

	if (!E || count >= MAX_GRUDGE_ENEMIES)  // NO ENEMY if null or full
	{
		delete E; return;
	}

    E->setPowerBoost(1.5f);
    E->forceRevive(E->getHP() * 2.0f);
	pool[count] = E;
	timers[count] = 15.0f;
	count++;
}
void GrudgeRegistry::update(float dt, Character* player,
    Enemy** level_enemies, int& enemy_count, int max_enemies)
{
    for (int i = count - 1; i >= 0; i--)
    {
        if (!pool[i]) continue;

        timers[i] -= dt;

        if (timers[i] <= 0.0f)
        {

            float sx = player->getPosX() - 380.0f;
            if (sx < 50.0f) sx = 50.0f;
            pool[i]->setPosition(sx, player->getPosY());

            bool added = false;


            for (int j = 0; j < max_enemies; j++)
            {
                if (level_enemies[j] == nullptr || !level_enemies[j]->isAlive())
                {
                    if (level_enemies[j]) delete level_enemies[j];
                    level_enemies[j] = pool[i];
                    if (j >= enemy_count) enemy_count = j + 1;
                    pool[i] = nullptr;
                    added = true;
                    break;
                }
            }

            if (!added)
            {
                delete pool[i];
                pool[i] = nullptr;
            }
            for (int k = i; k < count - 1; k++)
            {
                pool[k] = pool[k + 1];
                timers[k] = timers[k + 1];
            }
            pool[count - 1] = nullptr;
            timers[count - 1] = 0.0f;
            count--;
        }
    }
}


void GrudgeRegistry::clear() {
	for (int i = 0; i < MAX_GRUDGE_ENEMIES; i++)
	{ 
		if (pool[i])
		{ 
            delete pool[i];
			pool[i] = nullptr;

		} 

		timers[i] = 0.0f;
	
	}

	count = 0;
}

