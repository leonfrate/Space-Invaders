#include <iostream>
#include <vector>
#include "SpaceShip.h"
#include "GameController.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Barier.h"
#include "Location.h"
#include "FinalBoss.h"

int main()
{
    srand(time(NULL));
    bool gameRunning = true;
    sf::Event event;
    clock_t beginTime = clock();
    float duration;
    int executed = 0;
    Location shipLoc; shipLoc.x = 300; shipLoc.y = 565;
    SpaceShip ship(shipLoc);
    GameController controller;
    FinalBoss finalBoss;

    std::vector<Projectile> shipProjectiles;
    std::vector<Projectile> enemyProjectiles;
    std::vector<Projectile> finalBossProjectiles;
    std::vector<Enemy> enemies;
    std::vector<Barier> bariers;

    controller.spawnEnemiesnR(enemies);
    controller.spawnBariers(bariers);

    sf::RenderWindow window(sf::VideoMode(600, 600), "Space Invaders");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        window.clear();
        window.draw(ship.getShape());

        if (not shipProjectiles.empty())
        {
            for (auto & p : shipProjectiles)
            {
                window.draw(p.getShape());
                p.shootShip();
                p.overboard(shipProjectiles);
            }
        }

        if (not bariers.empty())
        {
            for (auto & b : bariers)
                window.draw(b.getShape());
        }

        if (not enemyProjectiles.empty())
        {
            for (auto & ep : enemyProjectiles)
            {
                window.draw(ep.getShape());
                ep.shootEnemy();
                ep.overboard(enemyProjectiles);
            }
        }

        if (not enemies.empty())
        {
            for (auto & e : enemies)
            {
                window.draw(e.getShape());
            }
        }

        if (not enemies.empty())
        {
            int randNum = rand() % (enemies.size() * 10);
            if (randNum < enemies.size())
            {
                enemies[randNum].enemyShoot(enemies[randNum].getLocation(), enemyProjectiles);
            }
        }

        controller.controlEvents(window, event, ship, shipProjectiles);
        controller.projectilesHit(shipProjectiles, enemies);
        controller.bariesCollision(shipProjectiles, enemyProjectiles, finalBossProjectiles, bariers);
        controller.spaceshipHit(ship, enemyProjectiles, gameRunning);
        controller.moveBariers(bariers);
        controller.moveEnemies(enemies, duration, executed);

        if (enemies.empty())
        {
            window.draw(finalBoss.getShape());

            controller.finalBossHit(finalBoss, shipProjectiles);
            controller.bariesCollision(shipProjectiles, enemyProjectiles, finalBossProjectiles, bariers);

            int randNum = rand() % 300;
            if (randNum >= 150 and randNum <= 170)
                finalBoss.fBshootNormProj(finalBossProjectiles);

            finalBoss.fBmove();

            for (auto &p : finalBossProjectiles)
            {
                p.overboard(finalBossProjectiles);
                window.draw(p.getShape());
                p.shootEnemy();
            }

            if (finalBoss.getHP() == 0)
                gameRunning = false;
        }

        duration = (clock() - beginTime) / (double) CLOCKS_PER_SEC;

        window.display();
    }
}
