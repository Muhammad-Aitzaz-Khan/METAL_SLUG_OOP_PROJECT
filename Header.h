#pragma once

#include <SFML/Graphics.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

void intToCharArray(int num, char result[7]);

class Entity;
class GameStateManager;
class FusionCompanion;
class PlayerSoldier;
class EntityManager;
class FlyingTara;
class Tara_Grenade;
class Missile;
class Enemy_Sub_Missile;
class Bullet;
class Vehicle;
class MetalSlug;
class SlugFlyer;
class LevelSelectState;
class BossLevelState;
class AmmoCrate;
class Perlin;

void Drop_Food(float ex, float ey, EntityManager* mgr);



struct VECTOR {

    float X_comp;
    float Y_comp;

};

VECTOR giverandomgradient(int ix, int iy) {

    unsigned int hash = (ix * 1619u) ^ (iy * 31337u);
    hash = hash * 73256093u;
    unsigned int h = hash & 3;
    VECTOR gradient;
    gradient.X_comp = ((h & 1) == 0) ? 1.0f : -1.0f;
    gradient.Y_comp = ((h & 2) == 0) ? 1.0f : -1.0f;
    return gradient;


}

float computeDOT(int x, int y, float a, float b) {


    float distanceX = a - (float)x;
    float distanceY = b - (float)y;
    VECTOR myone = giverandomgradient(x, y);
    return (distanceX * myone.X_comp) + (distanceY * myone.Y_comp);


}

float interpolateME(float x, float y, float distanceWEIGHT) {


    return (y - x) * (3.0 - distanceWEIGHT * 2.0) * distanceWEIGHT * distanceWEIGHT + x;


}

float MY_PERLIN(float a, float b) {


    int newA = (int)std::floor(a);
    int newB = (int)std::floor(b);
    int x1 = newA + 1;
    int y1 = newB + 1;
    float myXweight = a - (float)newA;
    float myYweight = b - (float)newB;
    float n1 = computeDOT(newA, newB, a, b);
    float n2 = computeDOT(x1, newB, a, b);
    float res1 = interpolateME(n1, n2, myXweight);
    float n3 = computeDOT(x1, y1, a, b);
    float n4 = computeDOT(newA, y1, a, b);
    float res2 = interpolateME(n4, n3, myXweight);
    return interpolateME(res1, res2, myYweight);


}

class Perlin {


protected:


    float MYFRACTALNOISE(int x, int numoctaves, float freqMUL, float ampMUL) {


        float tot = 0.0f;
        float myfeq = freqMUL;
        float myamp = ampMUL;
        float myMAXAMP = 0.0f;
        for (int i = 0; i < numoctaves; i++) {


            tot += MY_PERLIN(x * myfeq, 0.3f) * myamp;
            myMAXAMP += myamp;
            myfeq *= 2.0f;
            myamp *= 0.5f;
        }


        return tot / myMAXAMP;



    }


public:


    virtual ~Perlin() {}
    virtual float getheight(float fa) = 0;


};



class Amplified : public Perlin {


public:


    virtual float getheight(float fa) override {

        float tot = 0.0f;
        float freq = 0.05f;
        float amp = 0.9f;
        float maxAmp = 0.0f;
        for (int i = 0; i < 4; i++) {

            tot += MY_PERLIN(fa * freq, 0.3f) * amp; maxAmp += amp;
            freq *= 2.0f;
            amp *= 0.5f;

        }
        return (tot / maxAmp) * 2.5f;

    }

};


class Flat : public Perlin {

public:

    virtual float getheight(float fa) override {

        float tot = 0.0f;
        float freq = 0.05f;
        float amp = 0.9f;
        float maxAmp = 0.0f;

        for (int i = 0; i < 4; i++) {

            tot += MY_PERLIN(fa * freq, 0.3f) * amp;
            maxAmp += amp;
            freq *= 2.0f;
            amp *= 0.5f;

        }

        return (tot / maxAmp) * 0.4f;

    }

};



class Normal : public Perlin {

public:

    virtual float getheight(float fa) override {

        float tot = 0.0f;
        float freq = 0.05f;
        float amp = 0.9f;
        float maxAmp = 0.0f;

        for (int i = 0; i < 4; i++) {

            tot += MY_PERLIN(fa * freq, 0.3f) * amp;
            maxAmp += amp;
            freq *= 2.0f;
            amp *= 0.5f;

        }

        return (tot / maxAmp) * 1.0f;

    }

};



class PLAYERPROFILE {

public:

    Perlin* getMYMAP(int choice) {

        if (choice == 1)
            return new Normal();
        else if (choice == 2) 
            return new Amplified();
        else if (choice == 3)
            return new Flat();

        return new Normal();
    }

};





class Entity {

protected:
    
    float y;
    float velocityX;
    float velocityY;
    int width;
    int height;
    bool isDead;
    int* scorePtr;

public:

    float x;
    Entity(float startX, float startY) {

        x = startX;
        y = startY;
        velocityX = 0.0f;
        velocityY = 0.0f;
        width = 100;
        height = 100;
        isDead = false;
        scorePtr = nullptr;


    }

    virtual ~Entity() {}
    virtual void update(char** lvl, int cell_size) = 0;
    virtual void draw(sf::RenderWindow& window, float camX, float camY) = 0;

    bool getIsDead() const { return isDead; }
    void kill() { isDead = true; }

    virtual float getX() const { return x; }
    virtual float getY() const { return y; }
    float getwidth() const { return (float)width; }
    float getheight() const { return (float)height; }

    virtual bool isEnemy() { return false; }
    virtual bool isPlayerVehicle() { return false; }
    void setScorePtr(int* ptr) { scorePtr = ptr; }

    virtual void takeDamage(double damageAmount) {}
    virtual PlayerSoldier* asPlayer() { return nullptr; }
    virtual Vehicle* asVehicle() { return nullptr; }
    virtual bool isMetalSlug() { return false; }
    virtual bool isSlugFlyer() { return false; }
    virtual bool isSlugMariner() { return false; }
    virtual bool isAmphibious() { return false; }
    virtual bool isBullet() { return false; }
    virtual bool isGrenade() { return false; }
    virtual void setAsEnemyBullet() {}
    virtual void setAsShooterAirborne() {}
    virtual bool getIsEnemyGrenade() const { return false; }
    virtual bool getIsEnemyBullet() const { return false; }

    bool Check_Touch(float enemy_x, float enemy_y, int enemyWidth, int enemyHeight, float player_x, float player_y, int PlayerWidth, int PlayerHeight, int scale, int P_D) {
        int temp1 = 0;
        int temp2 = 0;


        if (scale == -1)
            temp1 = -enemyWidth;
        else
            temp1 = enemyWidth;


        if (P_D == -1)
            temp2 = -PlayerWidth;
        else
            temp2 = PlayerWidth;


        bool a = Touching(player_x, player_y, enemy_x, enemy_y, temp1, enemyHeight);
        bool b = Touching(player_x + temp2 / 2.0f, player_y, enemy_x, enemy_y, temp1, enemyHeight);
        bool c = Touching(player_x + temp2, player_y, enemy_x, enemy_y, temp1, enemyHeight);
        bool d = Touching(player_x, player_y + PlayerHeight / 2.0f, enemy_x, enemy_y, temp1, enemyHeight);
        bool e = Touching(player_x + temp2, player_y + PlayerHeight / 2.0f, enemy_x, enemy_y, temp1, enemyHeight);
        bool f = Touching(player_x, player_y + PlayerHeight, enemy_x, enemy_y, temp1, enemyHeight);
        bool g = Touching(player_x + temp2 / 2.0f, player_y + PlayerHeight, enemy_x, enemy_y, temp1, enemyHeight);
        bool h = Touching(player_x + temp2, player_y + PlayerHeight, enemy_x, enemy_y, temp1, enemyHeight);

        if ((a || b || c || d || e || f || g || h) == true)
            return true;


        return false;


    }

    bool Touching(float player_x, float player_y, float enemy_x, float enemy_y, int enemyWidth, int enemyHeight) {

        if ((player_x >= enemy_x) && (player_x <= (enemy_x + enemyWidth)) && (player_y >= enemy_y) && (player_y <= (enemy_y + enemyHeight))) 
            return true;
        else if ((player_x <= enemy_x) && (player_x >= (enemy_x + enemyWidth)) && (player_y >= enemy_y) && (player_y <= (enemy_y + enemyHeight)))
            return true;
        else
            return false;


    }

    void applyCollision(char** lvl, int cell_size, int entityWidth, int entityHeight,bool& onGround, bool& canJump, bool wantToFall, int PD) {
        if (PD == -1)
            entityWidth = -entityWidth;
        float prevFeetY = y + entityHeight;

        x += velocityX;
        float tempY = y + velocityY;
        int Map_Height = (int)(18 * 1.2f);
        int Map_Width = 128 * 3;
        int feet_row = (int)(tempY + entityHeight) / cell_size;
        int left_col = (int)(x) / cell_size;
        int middle_col = (int)(x + entityWidth / 2.0f) / cell_size;
        int right_col = (int)(x + entityWidth) / cell_size;



        if (feet_row >= Map_Height)
            feet_row = Map_Height - 1;


        if (feet_row < 0)
            feet_row = 0;

        if (left_col < 0)
            left_col = 0;

        if (left_col >= Map_Width)
            left_col = Map_Width - 1;

        if (right_col < 0)
            right_col = 0;

        if (right_col >= Map_Width)
            right_col = Map_Width - 1;

        if (middle_col < 0)
            middle_col = 0;

        if (middle_col >= Map_Width)
            middle_col = Map_Width - 1;

        char below_left = lvl[feet_row][left_col];
        char below = lvl[feet_row][middle_col];
        char below_right = lvl[feet_row][right_col];
        int head_row = (int)(tempY) / cell_size;


        if (head_row < 0)
            
            head_row = 0;
        if (head_row >= Map_Height) head_row = Map_Height - 1;
        char above_left = lvl[head_row][left_col];
        char above = lvl[head_row][middle_col];
        char above_right = lvl[head_row][right_col];
        onGround = false;
        float platform_top = (float)(feet_row * cell_size);
        if (below_left == '#' || below == '#' || below_right == '#') {
            onGround = true; velocityY = 0; y = platform_top - entityHeight;
        }
        else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
            if (wantToFall) { y = tempY + 5.0f; onGround = false; }
            else {
                if (prevFeetY <= platform_top) { onGround = true; velocityY = 0; y = platform_top - entityHeight; }
                else { y = tempY; }
            }
        }
        else if (velocityY < 0 && (above_left == 'P' || above == 'P' || above_right == 'P' ||
            above_left == 'S' || above == 'S' || above_right == 'S')) {
            y = tempY;
        }
        else { y = tempY; }
        canJump = onGround;
    }
};

class DamageableEntity : public Entity {
protected:
    double health;
public:
    DamageableEntity(float startX, float startY, double startHealth) : Entity(startX, startY) {
        health = startHealth;
    }
    virtual ~DamageableEntity() {}
    double getHealth() const { return health; }
    virtual void takeDamage(double damageAmount) {
        health = health - damageAmount;
        if (health <= 0) kill();
    }
};

// ─────────────────────────────────────────────
// WEAPONS
// ─────────────────────────────────────────────

class Weapon {
protected:
    int damage; int ammo; bool isinfinite;
    sf::Clock mytimer; float fireRate; float bulletSpeed; int bulletType;
public:
    Weapon(int dmg, int startAmmo, bool t, float rate, float speed, int type) {
        damage = dmg; ammo = startAmmo; isinfinite = t;
        fireRate = rate; bulletSpeed = speed; bulletType = type;
    }
    virtual ~Weapon() {}
    virtual void refillAmmo() {}
    int getAmmo() const { return ammo; }
    bool isInfinite() const { return isinfinite; }
    void modifyFireRate(float multiplier) { fireRate = fireRate * multiplier; }
    virtual void fire(float originX, float originY, float aimAngleDegrees,
        EntityManager* manager, bool specialActive = false) = 0;
};

class ProjectileWeapon : public Weapon {
public:
    ProjectileWeapon(int dmg, int startAmmo, bool t, float rate, float speed, int type)
        : Weapon(dmg, startAmmo, t, rate, speed, type) {
    }
    void fire(float originX, float originY, float aimAngleDegrees,
        EntityManager* manager, bool specialActive = false) override;
};

class TransformationState {
public:
    virtual ~TransformationState() {}
    virtual void handleInput(PlayerSoldier* player) = 0;
};

class Soldier : public DamageableEntity {
protected:
    bool onGround;
public:
    Soldier(float startX, float startY, double hp) : DamageableEntity(startX, startY, hp) {
        onGround = false;
    }
    bool isJumping() const { return !onGround; }
};

class pistol : public ProjectileWeapon {
public:
    pistol() : ProjectileWeapon(3, 0, true, 0.25f, 5.0f, 1) {}
    void refillAmmo() override {}
};
class HMG : public ProjectileWeapon {
public:
    HMG() : ProjectileWeapon(3, 10000, false, 0.1f, 25.0f, 2) {}
    void refillAmmo() override { ammo = 10000; }
};
class RocketLauncher : public ProjectileWeapon {
public:
    RocketLauncher() : ProjectileWeapon(5, 100000, false, 2.0f, 10.0f, 3) {}
    void refillAmmo() override { ammo = 100000; }
};
class FlameShot : public ProjectileWeapon {
public:
    FlameShot() : ProjectileWeapon(2, 10000, false, 0.016f, 10.0f, 4) {}
    void refillAmmo() override { ammo = 10000; }
};
class LaserGun : public ProjectileWeapon {
public:
    LaserGun() : ProjectileWeapon(9999, 0, false, 0.3f, 30.0f, 5) {}
    void refillAmmo() override { ammo = 500; }
};

// ─────────────────────────────────────────────
// ENTITY MANAGER
// ─────────────────────────────────────────────

class EntityManager {
private:
    int currentCount; int maxCapacity;
    void expandArray() {
        int newCapacity = maxCapacity * 2;
        Entity** newArray = new Entity * [newCapacity];
        for (int i = 0; i < currentCount; i++) newArray[i] = entityArray[i];
        delete[] entityArray; entityArray = newArray; maxCapacity = newCapacity;
    }
    Entity** entityArray;
public:
    EntityManager() { maxCapacity = 50; currentCount = 0; entityArray = new Entity * [maxCapacity]; }
    ~EntityManager() {
        for (int i = 0; i < currentCount; i++) delete entityArray[i];
        delete[] entityArray;
    }
    bool contains(Entity* target) {
        for (int i = 0; i < currentCount; i++) if (entityArray[i] == target) return true;
        return false;
    }
    Entity* getEntity(int index) {
        if (index >= 0 && index < currentCount) return entityArray[index];
        return nullptr;
    }
    void addEntity(Entity* newEntity) {
        if (currentCount >= maxCapacity) expandArray();
        entityArray[currentCount] = newEntity; currentCount++;
    }
    void updateAll(char** lvl, int cell_size) {
        for (int i = 0; i < currentCount; i++) {
            if (entityArray[i]->getIsDead()) {
                delete entityArray[i];
                for (int j = i; j < currentCount - 1; j++) entityArray[j] = entityArray[j + 1];
                currentCount--; i--;
            }
            else { entityArray[i]->update(lvl, cell_size); }
        }
    }
    void drawAll(sf::RenderWindow& window, float camX, float camY) {
        for (int i = 0; i < currentCount; i++) entityArray[i]->draw(window, camX, camY);
    }
    Entity* getPlayerEntity(int idx) { return entityArray[idx]; }
    int Get_Current_Count() { return currentCount; }
    Entity* getPlayerEntity();
    Entity* getVehicleEntity();
    Entity* getMetalSlugEntity();
    Entity* getSlugFlyerEntity();
    Entity* getSlugMarinerEntity();
    Entity* getAmphibiousSlugEntity();
};

// ─────────────────────────────────────────────
// PROJECTILES & GRENADES
// ─────────────────────────────────────────────

class PlayerGrenade : public Entity {
private:
    float x, y; bool exploded;
    Texture grenadeTexture; Sprite grenadeSprite;
    Texture explosionTexture; Sprite explosionSprite;
    EntityManager* manager;
    int frame, maxFrames, frameWidth, frameHeight;
    Clock animClock; float gravity; bool isEnemyGrenade;
public:
    PlayerGrenade(float startX, float startY, float launchVx, float launchVy, EntityManager* mgr)
        : Entity(startX, startY) {
        this->x = startX; this->y = startY;
        Entity::width = 50; Entity::height = 50;
        velocityX = launchVx; velocityY = launchVy; gravity = 0.9f;
        exploded = false; isEnemyGrenade = false;
        frame = 0; maxFrames = 3; frameWidth = 225; frameHeight = 225; manager = mgr;
        grenadeTexture.loadFromFile("Player_Grenade.png"); grenadeSprite.setTexture(grenadeTexture);
        grenadeSprite.setScale(0.5f, 0.67f);
        explosionTexture.loadFromFile("Grenade_Explosion.png"); explosionSprite.setTexture(explosionTexture);
    }
    void update(char** lvl, int cell_size) override;
    void setEnemyGrenade(bool val) { isEnemyGrenade = val; }
    bool getIsEnemyGrenade() const override { return isEnemyGrenade; }
    bool isGrenade() override { return true; }
    void draw(RenderWindow& window, float camX, float camY) override {
        if (!exploded) { grenadeSprite.setPosition(x - camX, y - camY); window.draw(grenadeSprite); }
        else { explosionSprite.setPosition(x - camX - 60, y - camY - 100); window.draw(explosionSprite); }
    }
};

class FireBombGrenade : public Entity {
private:
    float x, y; bool exploded, firePoolActive;
    float firePoolTimer, fireTickTimer, poolRadius;
    Texture grenadeTexture; Sprite grenadeSprite;
    Texture explosionTexture; Sprite explosionSprite;
    EntityManager* manager;
    int frame, maxFrames, frameWidth, frameHeight;
    Clock animClock; float gravity;
public:
    FireBombGrenade(float startX, float startY, float launchVx, float launchVy, EntityManager* mgr)
        : Entity(startX, startY) {
        this->x = startX; this->y = startY;
        Entity::width = 50; Entity::height = 50;
        velocityX = launchVx; velocityY = launchVy; gravity = 0.9f;
        exploded = false; firePoolActive = false; firePoolTimer = 0.0f; fireTickTimer = 0.0f;
        poolRadius = 3 * 50.0f; frame = 0; maxFrames = 3; frameWidth = 225; frameHeight = 225; manager = mgr;
        grenadeTexture.loadFromFile("Fire_Grenade.png"); grenadeSprite.setTexture(grenadeTexture);
        grenadeSprite.setScale(0.5f, 0.67f);
        explosionTexture.loadFromFile("Boss_Bombs_2.png"); explosionSprite.setTexture(explosionTexture);
    }
    bool isGrenade() override { return true; }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (!exploded) {
            velocityY += gravity; x += velocityX; y += velocityY;
            for (int i = 0; i < manager->Get_Current_Count(); i++) {
                Entity* e = manager->getEntity(i);
                if (e->getIsDead() || !e->isEnemy()) continue;
                bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1);
                if (hit) { e->takeDamage(20); exploded = true; firePoolActive = true; frame = 0; animClock.restart(); return; }
            }
            int check_col = (int)(x + width / 2.0f) / cell_size;
            int check_row = (int)(y + height) / cell_size;
            int Map_Height = (int)(18 * 1.2f); int Map_Width = 128 * 3;
            if (check_row >= 0 && check_row < Map_Height && check_col >= 0 && check_col < Map_Width) {
                if (lvl[check_row][check_col] == '#' || lvl[check_row][check_col] == 'P') {
                    exploded = true; firePoolActive = true; frame = 0; animClock.restart();
                }
            }
        }
        else {
            if (animClock.getElapsedTime().asMilliseconds() > 90) { frame++; animClock.restart(); }
            explosionSprite.setTextureRect(sf::IntRect((frame % 3) * frameWidth, 0, frameWidth, frameHeight));
            if (firePoolActive) {
                firePoolTimer += 0.016f; fireTickTimer += 0.016f;
                if (fireTickTimer >= 1.0f) {
                    fireTickTimer = 0.0f;
                    for (int i = 0; i < manager->Get_Current_Count(); i++) {
                        Entity* e = manager->getEntity(i);
                        if (e->getIsDead() || !e->isEnemy()) continue;
                        float dx = e->getX() - x; float dy = e->getY() - y;
                        if (sqrt(dx * dx + dy * dy) <= poolRadius) e->takeDamage(2);
                    }
                }
                if (firePoolTimer >= 2.0f) { firePoolActive = false; kill(); }
            }
            else if (frame >= maxFrames * 2) { kill(); }
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (!exploded) { grenadeSprite.setPosition(x - camX, y - camY); window.draw(grenadeSprite); }
        else { explosionSprite.setPosition(x - camX - 60, y - camY - 100); window.draw(explosionSprite); }
    }
};

class Tara_Grenade : public Entity {
private:
    float fuseTime, timer; int dir; float x, y; bool exploded;
    Texture grenadeTexture; Sprite grenadeSprite;
    Texture explosionTexture; Sprite explosionSprite;
    EntityManager* manager;
    int frame, maxFrames, frameWidth, frameHeight; Clock animClock;
public:
    Tara_Grenade(float startX, float startY, int direction, EntityManager* mgr) : Entity(startX, startY) {
        this->x = startX; this->y = startY; dir = direction;
        width = 100; height = 100; velocityX = (float)dir; velocityY = -1.0f;
        fuseTime = 1.1f; timer = 0.0f; exploded = false; frame = 0; maxFrames = 3;
        frameWidth = 225; frameHeight = 225; manager = mgr;
        grenadeTexture.loadFromFile("Tara_Grenade.png"); grenadeSprite.setTexture(grenadeTexture); grenadeSprite.setScale(2.0f, 2.0f);
        explosionTexture.loadFromFile("Grenade_Explosion.png"); explosionSprite.setTexture(explosionTexture);
    }
    void update(char** lvl, int cell_size) override;
    void draw(RenderWindow& window, float camX, float camY) override {
        if (!exploded) { grenadeSprite.setPosition(x - camX, y - camY); window.draw(grenadeSprite); }
        else { explosionSprite.setPosition(x - camX, y - camY); window.draw(explosionSprite); }
    }
};

class Missile : public Entity {
private:
    EntityManager* manager; float x, y, vx, vy; bool exploded; float fuseTime, timer; int dir;
    Texture Missile_Texture; Sprite Missile_Sprite;
    int frame, maxFrames, frameWidth, frameHeight; Clock animClock;
    Texture explosionTexture; Sprite explosionSprite;
    int frame2, maxFrames2, frameWidth2, frameHeight2; Clock animClock2; Clock explosionClock;
public:
    Missile(float startX, float startY, int dir, EntityManager* mgr) : Entity(startX, startY) {
        x = startX; y = startY; this->dir = dir; width = 40; height = 40;
        vx = dir * 12.0f; vy = -17.0f; fuseTime = 0.95f; timer = 0.0f; exploded = false;
        frame = 0; maxFrames = 8; frameWidth = 100; frameHeight = 80;
        frameWidth2 = 225; frameHeight2 = 225; frame2 = 0; maxFrames2 = 3; manager = mgr;
        Missile_Texture.loadFromFile("Bradley_Missile.png"); Missile_Sprite.setTexture(Missile_Texture);
        Missile_Sprite.setScale((float)(2 * -dir), 2.0f);
        explosionTexture.loadFromFile("Grenade_Explosion.png"); explosionSprite.setTexture(explosionTexture);
    }
    void update(char** lvl, int cell_size) override;
    void draw(RenderWindow& window, float camX, float camY) override {
        if (!exploded) { Missile_Sprite.setPosition(x - camX, y - camY); window.draw(Missile_Sprite); }
        else { explosionSprite.setPosition(x - camX, y - camY); window.draw(explosionSprite); }
    }
};

class Enemy_Sub_Missile : public Entity {
private:
    float fuseTime, timer; int dir; float x, y; bool exploded;
    Texture grenadeTexture; Sprite grenadeSprite;
    Texture explosionTexture; Sprite explosionSprite;
    EntityManager* manager; int frame, maxFrames, frameWidth, frameHeight; Clock animClock;
public:
    Enemy_Sub_Missile(float startX, float startY, int direction, EntityManager* mgr) : Entity(startX, startY) {
        this->x = startX; this->y = startY; dir = direction; width = 100; height = 100;
        velocityX = dir * 1.3f; velocityY = -1.0f; fuseTime = 2.0f; timer = 0.0f; exploded = false;
        frame = 0; maxFrames = 3; frameWidth = 150; frameHeight = 150; manager = mgr;
        grenadeTexture.loadFromFile("Enemy_Sub_Rocket.png"); grenadeSprite.setTexture(grenadeTexture);
        grenadeSprite.setScale((float)(1 * dir), 1.0f);
        explosionTexture.loadFromFile("Rocket_Explosion.png"); explosionSprite.setTexture(explosionTexture);
    }
    void update(char** lvl, int cell_size) override;
    void draw(RenderWindow& window, float camX, float camY) override {
        if (!exploded) { grenadeSprite.setPosition(x - camX, y - camY); window.draw(grenadeSprite); }
        else { explosionSprite.setPosition(x - camX, y - camY); window.draw(explosionSprite); }
    }
};

class Marine_Missiles : public Entity {
private:
    int M_Type, dir; float gravity; bool exploded; float timer, fuseTime;
    Texture Missile_Texture; Sprite Missile_Sprite;
    Texture explosionTex; Sprite explosionSpr;
    int frame, maxFrames, frameW, frameH; Clock animClock; EntityManager* manager;
public:
    Marine_Missiles(float sx, float sy, int type, int direction, EntityManager* mgr)
        : Entity(sx, sy), M_Type(type), dir(direction), exploded(false), timer(0.f),
        frame(0), maxFrames(3), frameW(150), frameH(150), manager(mgr) {
        width = 100; height = 75;
        switch (type) {
        case 1: velocityX = dir * 14.f; velocityY = 0.f; gravity = 0.f; fuseTime = 2.5f; break;
        case 2: velocityX = 0.f; velocityY = 14.f; gravity = 0.f; fuseTime = 2.5f; break;
        case 3: velocityX = dir * 8.f; velocityY = 6.f; gravity = -0.45f; fuseTime = 3.0f; break;
        }
        Missile_Texture.loadFromFile("Enemy_Sub_Rocket.png"); Missile_Sprite.setTexture(Missile_Texture); Missile_Sprite.setScale((float)dir, 1.0f);
        explosionTex.loadFromFile("Rocket_Explosion.png"); explosionSpr.setTexture(explosionTex);
    }
    bool isEnemy() override { return false; }
    void update(char** lvl, int cell_size) override;
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (!exploded) { Missile_Sprite.setPosition(x - camX, y - camY); window.draw(Missile_Sprite); }
        else { explosionSpr.setPosition(x - camX - 40, y - camY - 40); window.draw(explosionSpr); }
    }
};

class FlyerMissile : public Entity {
private:
    float fuseTime, timer; int dir; bool exploded;
    Texture missileTexture; Sprite missileSprite;
    Texture explosionTexture; Sprite explosionSprite;
    int frame, maxFrames, frameWidth, frameHeight, frameE; Clock animClock;
public:
    FlyerMissile(float startX, float startY, int direction) : Entity(startX, startY) {
        dir = direction; width = 100; height = 100; velocityX = dir * 8.0f; velocityY = 0.0f;
        fuseTime = 2.0f; timer = 0.0f; exploded = false; frame = 0; maxFrames = 3;
        frameWidth = 100; frameHeight = 75; frameE = 225;
        missileTexture.loadFromFile("Enemy_Sub_Rocket.png"); missileSprite.setTexture(missileTexture); missileSprite.setScale((float)dir, 1.0f);
        explosionTexture.loadFromFile("Grenade_Explosion.png"); explosionSprite.setTexture(explosionTexture);
    }
    void update(char** lvl, int cell_size) override;
    void draw(RenderWindow& window, float camX, float camY) override {
        if (!exploded) { missileSprite.setPosition(x - camX, y - camY); window.draw(missileSprite); }
        else { explosionSprite.setPosition(x - camX, y - camY); window.draw(explosionSprite); }
    }
};

// ─────────────────────────────────────────────
// BULLET
// ─────────────────────────────────────────────

class Bullet : public Entity {
private:
    sf::Texture bulletTexture; sf::Sprite bulletSprite;
    int damage, bulletType; EntityManager* manager;
    float spawnX, spawnY, maxRange;
    bool isEnemyBullet, shooterWasAirborne, isFlameShot;
    // Perlin for infinite mode collision
    Perlin* myPerlin; int myMode;
public:
    Bullet(float startX, float startY, float vx, float vy, int dmg, int bt, float aimAngle,
        EntityManager* mgr, Perlin* p = nullptr, int mode = 0) : Entity(startX, startY) {
        damage = dmg; width = 10; height = 10; isFlameShot = false; bulletType = bt;
        velocityX = vx; velocityY = vy; manager = mgr;
        isEnemyBullet = false; spawnX = startX; spawnY = startY; maxRange = 1800;
        if (bulletType == 4) maxRange = 750.0f;
        shooterWasAirborne = false; myPerlin = p; myMode = mode;
        if (bulletType == 1) { bulletTexture.loadFromFile("pistol.png"); bulletSprite.setTexture(bulletTexture); }
        else if (bulletType == 2) { bulletTexture.loadFromFile("machine_gun.png"); bulletSprite.setTexture(bulletTexture); }
        else if (bulletType == 3) { bulletTexture.loadFromFile("rocket_L.png"); bulletSprite.setTexture(bulletTexture); }
        else if (bulletType == 4) { bulletTexture.loadFromFile("machine_gun.png"); bulletSprite.setTexture(bulletTexture); }
        else if (bulletType == 5) { bulletTexture.loadFromFile("laser_gun.png"); bulletSprite.setTexture(bulletTexture); }
        else if (bulletType == 6) { bulletTexture.loadFromFile("Metal_Slug_Gun.png"); bulletSprite.setTexture(bulletTexture); }
        if (velocityX < 0) bulletSprite.setScale(-1.0f, 1.0f);
        else bulletSprite.setScale(1.0f, 1.0f);
    }
    void update(char** lvl, int cell_size) override;
    void setEnemyBullet(bool val) { isEnemyBullet = val; }
    void setShooterAirborne(bool val) { shooterWasAirborne = val; }
    bool getShooterWasAirborne() const { return shooterWasAirborne; }
    bool isBullet() override { return true; }
    bool getIsEnemyBullet() const override { return isEnemyBullet; }
    void setAsShooterAirborne() override { shooterWasAirborne = true; }
    void setIsFlameShot(bool val) { isFlameShot = val; }
    bool getIsFlameShot() const { return isFlameShot; }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        bulletSprite.setPosition(x - camX, y - camY); window.draw(bulletSprite);
    }
};

// ─────────────────────────────────────────────
// FUSION COMPANION
// ─────────────────────────────────────────────

class FusionCompanion : public Entity {
private:
    EntityManager* manager; float speed; double damage; float fireRate; float shootTimer; int dir;
    Texture igrisTex; Sprite igrisSpr;
    Weapon* fusionWeapon; sf::Clock lifeClock; float maxLifetime;
    int P_Frame, Player_Frame;
public:
    FusionCompanion(float sx, float sy, EntityManager* mgr, float spd, double dmg, float rate, float meleeMult)
        : Entity(sx, sy), manager(mgr), speed(spd), damage(dmg), fireRate(rate),
        shootTimer(0.f), dir(1), P_Frame(0), Player_Frame(0) {
        Entity::width = 150; Entity::height = 150; maxLifetime = 30.f;
        igrisTex.loadFromFile("igris.png"); igrisSpr.setTexture(igrisTex); igrisSpr.setScale(0.4167f, 0.375f);
        fusionWeapon = new ProjectileWeapon((int)damage, 0, true, fireRate, 20.f, 2);
    }
    ~FusionCompanion() { delete fusionWeapon; }
    bool isEnemy() override { return false; }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (lifeClock.getElapsedTime().asSeconds() >= maxLifetime) { kill(); return; }
        Entity* pEnt = manager->getPlayerEntity();
        if (!pEnt) return;
        float px = pEnt->getX(), py = pEnt->getY();
        float targetX = px + dir * 120.f;
        float dx = targetX - x; float dy = py - y;
        if (abs(dx) > 50.f) { if (dx > 0) { x += speed; dir = 1; } else { x -= speed; dir = -1; } }
        if (abs(dy) > 50.f) { if (dy > 0) y += 3.f; else y -= 3.f; }
        shootTimer += 0.016f;
        if (shootTimer >= fireRate) {
            shootTimer = 0.f;
            Entity* target = nullptr; float minDist = 9999.f;
            for (int i = 0; i < manager->Get_Current_Count(); i++) {
                Entity* e = manager->getEntity(i);
                if (!e->isEnemy() || e->getIsDead()) continue;
                float edx = e->getX() - x; float edy = e->getY() - y;
                float dist = sqrt(edx * edx + edy * edy);
                if (dist < minDist) { minDist = dist; target = e; }
            }
            if (target && minDist < 800.f) {
                int sdir = (target->getX() > x) ? 1 : -1;
                float wee = (sdir == 1) ? -0.4167f : 0.4167f; igrisSpr.setScale(wee, 0.375f);
                Bullet* b = new Bullet(x + sdir * 60.f, y + 30.f, sdir * 20.f, 0.f, (int)damage, 2, (sdir == 1 ? 0.f : 180.f), manager);
                manager->addEntity(b);
            }
            else { float wee = (dir == 1) ? -0.4167f : 0.4167f; igrisSpr.setScale(wee, 0.375f); }
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return; igrisSpr.setPosition(x - camX, y - camY); window.draw(igrisSpr);
    }
};

// ─────────────────────────────────────────────
// PLAYER SOLDIER
// ─────────────────────────────────────────────

class PlayerSoldier : public Soldier {
protected:
    EntityManager* myManager; TransformationState* currentState;
    Weapon* activeWeapon; Weapon* weapons[5]; int grenadeType;
    int activeWeaponIndex; int startinggre;
    float player_x, player_y, gravity, terminal_Velocity, speed, jumpStrength;
    int grenadeCount; Clock grenadeTimer; float specialMaxTime;
    bool specialActive, specialUsed; Clock specialDurationTimer;
    bool onGround, canJump, wantToFall;
    int P_D, Fire_Direction, Pwidth, Pheight;
    sf::Texture upperTexture; Texture upperTextures[5]; sf::Sprite upperSprite;
    sf::Texture lowerTexture; sf::Sprite lowerSprite;
    int P_Frame, Player_Frame, chosenCharacter, lives;
    bool Is_Firing, Melee_Activated; int Melee_Frame;
    Texture meleeTexture; Sprite meleeSprite;
    Texture firingTexture; Texture firingTextures[5]; Sprite firingSprite;
    Clock meleeCooldownTimer;
    float manualAimAngle = 0.0f; bool isRiding;
    Clock gunAnimClock; int gunFrame;
    int saturation, maxSaturation;
    Clock fusionCooldownTimer; bool fusionUsed, fusionReady;
    // Perlin mode for infinite level
    Perlin* myPerlin; int myMode;
public:
    PlayerSoldier(float startX, float startY, int chosen, EntityManager* mymm) : Soldier(startX, startY, 100) {
        grenadeCount = 10; myManager = mymm; player_x = startX; player_y = startY;
        lives = 2; startinggre = 10; gravity = 1.0f; jumpStrength = -20.0f;
        terminal_Velocity = 40.0f; speed = 12.0f; specialMaxTime = 10.0f;
        gunFrame = 0; saturation = 10; maxSaturation = 10;
        fusionUsed = false; fusionReady = false; specialActive = false; specialUsed = false;
        onGround = false; grenadeType = 6; canJump = false; wantToFall = false;
        P_D = 1; Fire_Direction = 1; Pwidth = 100; Pheight = 100;
        P_Frame = 0; Player_Frame = 0; chosenCharacter = chosen;
        myPerlin = nullptr; myMode = 0;
        weapons[0] = new pistol(); weapons[1] = new HMG(); weapons[2] = new RocketLauncher();
        weapons[3] = new FlameShot(); weapons[4] = new LaserGun();
        activeWeaponIndex = 0; activeWeapon = weapons[0];
        currentState = nullptr; isRiding = false;
        if (chosen == 1) {
            upperTextures[0].loadFromFile("UB_Macro.png"); upperTextures[1].loadFromFile("UB_HMG_Macro.png");
            upperTextures[2].loadFromFile("UB_Rocket_Macro.png"); upperTextures[3].loadFromFile("UB_Flame_Macro.png");
            upperTextures[4].loadFromFile("UB_Laser_Macro.png"); lowerTexture.loadFromFile("LB_Macro.png");
            meleeTexture.loadFromFile("Melee_Macro.png"); firingTextures[0].loadFromFile("Pistol_Macro.png");
            firingTextures[1].loadFromFile("HMG_Macro.png"); firingTextures[2].loadFromFile("Rocket_Macro.png");
            firingTextures[3].loadFromFile("Flame_Macro.png"); firingTextures[4].loadFromFile("Laser_Macro.png");
        }
        else if (chosen == 2) {
            upperTextures[0].loadFromFile("UB_Tarma.png"); upperTextures[1].loadFromFile("UB_HMG_Tarma.png");
            upperTextures[2].loadFromFile("UB_Rocket_Tarma.png"); upperTextures[3].loadFromFile("UB_Flame_Tarma.png");
            upperTextures[4].loadFromFile("UB_Laser_Tarma.png"); lowerTexture.loadFromFile("LB_Tarma.png");
            meleeTexture.loadFromFile("Melee_Tarma.png"); firingTextures[0].loadFromFile("Pistol_Tarma.png");
            firingTextures[1].loadFromFile("HMG_Tarma.png"); firingTextures[2].loadFromFile("Rocket_Tarma.png");
            firingTextures[3].loadFromFile("Flame_Tarma.png"); firingTextures[4].loadFromFile("Laser_Tarma.png");
        }
        else if (chosen == 3) {
            upperTextures[0].loadFromFile("UB_Eri.png"); upperTextures[1].loadFromFile("UB_HMG_Eri.png");
            upperTextures[2].loadFromFile("UB_Rocket_Eri.png"); upperTextures[3].loadFromFile("UB_Flame_Eri.png");
            upperTextures[4].loadFromFile("UB_Laser_Eri.png"); lowerTexture.loadFromFile("LB_Eri.png");
            meleeTexture.loadFromFile("UB_Eri.png"); firingTextures[0].loadFromFile("Pistol_Eri.png");
            firingTextures[1].loadFromFile("HMG_Eri.png"); firingTextures[2].loadFromFile("Rocket_Eri.png");
            firingTextures[3].loadFromFile("Flame_Eri.png"); firingTextures[4].loadFromFile("Laser_Eri.png");
        }
        else {
            upperTextures[0].loadFromFile("UB_Fiolina.png"); upperTextures[1].loadFromFile("UB_HMG_Fiolina.png");
            upperTextures[2].loadFromFile("UB_Rocket_Fiolina.png"); upperTextures[3].loadFromFile("UB_Flame_Fiolina.png");
            upperTextures[4].loadFromFile("UB_Laser_Fiolina.png"); lowerTexture.loadFromFile("LB_Fiolina.png");
            meleeTexture.loadFromFile("Melee_Fiolina.png"); firingTextures[0].loadFromFile("Pistol_Fiolina.png");
            firingTextures[1].loadFromFile("HMG_Fiolina.png"); firingTextures[2].loadFromFile("Rocket_Fiolina.png");
            firingTextures[3].loadFromFile("Flame_Fiolina.png"); firingTextures[4].loadFromFile("Laser_Fiolina.png");
        }
        meleeSprite.setTexture(meleeTexture); firingTexture = firingTextures[activeWeaponIndex];
        firingSprite.setTexture(firingTexture); meleeSprite.setScale(1.2f, 1.2f); firingSprite.setScale(1.2f, 1.2f);
        upperTexture = upperTextures[activeWeaponIndex]; upperSprite.setTexture(upperTexture); lowerSprite.setTexture(lowerTexture);
        sf::IntRect Frame(0, 0, 100, 50); upperSprite.setTextureRect(Frame); lowerSprite.setTextureRect(Frame);
        upperSprite.setScale(1.2f, 1.2f); lowerSprite.setScale(1.0f, 1.0f);
        Is_Firing = false; Melee_Activated = false; Melee_Frame = 0;
    }
    ~PlayerSoldier() {
        if (currentState != nullptr) delete currentState;
        for (int i = 0; i < 5; i++) delete weapons[i];
    }
    void setPerlinMode(Perlin* p, int mode) { myPerlin = p; myMode = mode; }
    virtual float getAmmoMultiplier() { return 1.0f; }
    virtual float getMeleeMultiplier() { return 1.0f; }
    virtual void activateSPECIAL() {}
    PlayerSoldier* asPlayer() override { return this; }
    void changeState(TransformationState* newState) {
        if (currentState != nullptr) delete currentState;
        currentState = newState;
    }
    virtual void throwGRENADE() {
        if (grenadeCount > 0 && grenadeTimer.getElapsedTime().asSeconds() >= 0.6f) {
            float power = 22.0f; float angleMath = 0.7071f;
            float calcVx = power * angleMath * P_D; float calcVy = -power * angleMath;
            PlayerGrenade* g = new PlayerGrenade(player_x + (Pwidth / 2.0f), player_y + (Pheight / 2.0f), calcVx, calcVy, myManager);
            g->setScorePtr(scorePtr); myManager->addEntity(g);
            grenadeCount--; grenadeTimer.restart();
        }
    }
    void refillAllWeapons() { for (int i = 0; i < 5; i++) weapons[i]->refillAmmo(); }
    void takeDamage(double damageAmount) override {
        if (isRiding) return;
        health -= damageAmount;
        if (health <= 0) {
            lives--;
            if (lives <= 0) kill();
            else {
                health = 100; player_x = 200.0f; player_y = 100.0f;
                x = player_x; y = player_y; velocityX = 0; velocityY = 0;
                onGround = false; canJump = false; specialActive = false; specialUsed = false;
            }
        }
    }
    void heal(double amount) { health += amount; if (health > 100) health = 100; }
    void shoot() {
        if (activeWeapon != nullptr) {
            float angleToUse = getManualAimAngle();
            if (P_D == -1) angleToUse = 180.0f - angleToUse;
            bool shouldFireBackwards = (chosenCharacter == 1 && specialActive == true);
            int yyy = (chosenCharacter == 1 || chosenCharacter == 2)
                ? (int)(player_y + (Pheight / 3.0f) - 20.0f) : (int)(player_y + (Pheight / 3.0f) - 5.0f);
            int xxx = (P_D == 1) ? (int)(player_x + (Pwidth / 2.0f) + 30.0f) : (int)(player_x - (Pwidth / 2.0f) - 30.0f);
            activeWeapon->fire((float)xxx, (float)yyy, angleToUse, myManager, shouldFireBackwards);
            int last = myManager->Get_Current_Count() - 1;
            Entity* last_entity = myManager->getEntity(last);
            if (last_entity && last_entity->isBullet()) last_entity->setAsShooterAirborne();
        }
    }
    void switchWeapon(int index) {
        if (index >= 0 && index < 5) {
            activeWeaponIndex = index; activeWeapon = weapons[index];
            upperTexture = upperTextures[index]; upperSprite.setTexture(upperTexture);
            firingTexture = firingTextures[index]; firingSprite.setTexture(firingTexture);
        }
    }
    bool canSpawnFusion(int gameMode) {
        if (gameMode == 1) return fusionCooldownTimer.getElapsedTime().asSeconds() >= 180.f;
        else return !fusionUsed;
    }
    void onFusionSpawned(int gameMode) {
        if (gameMode == 2) fusionUsed = true;
        fusionCooldownTimer.restart();
    }
    void setGrenadeType(int type) { grenadeType = type; }
    int getGrenadeType() const { return grenadeType; }
    float getSpeed() { return speed; }
    void addX(float amount) { player_x += amount; }
    void setVelocityX(float vx) { velocityX = vx; }
    void setDirection(int dir) { P_D = dir; }
    float getX() const override { return player_x; }
    float getY() const override { return player_y; }
    int Get_P_D() const { return P_D; }
    int getLives() const { return lives; }
    void loseLife() { lives--; }
    void setPlayerX(float val) { player_x = val; x = val; }
    void setPlayerY(float val) { player_y = val; y = val; }
    void setVelocityY(float val) { velocityY = val; }
    void setOnGround(bool val) { onGround = val; }
    void setCanJump(bool val) { canJump = val; }
    bool isActive() const { return currentState != nullptr; }
    virtual float getVehicleFireRateBuff() { return 1.0f; }
    virtual float getVehicleHealthBuff() { return 1.0f; }
    virtual bool hasTotemOfUndying() { return false; }
    bool Get_Is_Firing() const { return Is_Firing; }
    void Set_Is_Firing(bool val) { Is_Firing = val; }
    bool Get_Melee_Activated() const { return Melee_Activated; }
    void Set_Melee_Activated(bool val) { Melee_Activated = val; }
    int Get_PlayerIndex() { return chosenCharacter; }
    int Get_Player_Frame() const { return Player_Frame; }
    void Set_Player_Frame(int val) { Player_Frame = val; }
    bool Get_Melee_Ready() const { return meleeCooldownTimer.getElapsedTime().asSeconds() >= 0.5f; }
    void Reset_Melee_Timer() { meleeCooldownTimer.restart(); }
    void setManualAimAngle(float angle) { manualAimAngle = angle; }
    float getManualAimAngle() const { return manualAimAngle; }
    void SetIsRiding(bool val) { isRiding = val; }
    bool GetIsRiding() const { return isRiding; }
    Weapon* getWeapon() { return activeWeapon; }
    int getSaturation() const { return saturation; }
    int getMaxSaturation() const { return maxSaturation; }
    int* getScorePtr() { return scorePtr; }
    void addSaturation(int amount) { saturation += amount; if (saturation > maxSaturation) saturation = maxSaturation; }
    void triggerJump() { if (canJump) { velocityY = jumpStrength; canJump = false; } }
    void setWantToFall(bool status) { wantToFall = status; }
    virtual bool isInvincible() { return false; }
    virtual float getVehicleFireRateBuff2() { return 1.0f; }

    void update(char** lvl, int cell_size) override {
        if (isRiding) return;
        if (specialActive && specialDurationTimer.getElapsedTime().asSeconds() > specialMaxTime) specialActive = false;
        float prev_feet_y = player_y + Pheight;
        if (currentState != nullptr) currentState->handleInput(this);
        P_Frame++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (P_Frame % 5 == 0) Player_Frame++;
            if (Player_Frame >= 3) { Player_Frame = 0; if (Melee_Activated) Melee_Activated = false; }
        }
        if (Melee_Activated && P_Frame % 5 == 0) {
            Player_Frame++;
            if (Player_Frame >= 3) { Melee_Activated = false; Player_Frame = 0; }
        }
        if (Melee_Activated) { upperSprite.setTexture(meleeTexture); gunFrame = 0; }
        else if (Is_Firing) {
            upperSprite.setTexture(firingTexture);
            if (gunAnimClock.getElapsedTime().asMilliseconds() > 50) { gunFrame = (gunFrame == 0) ? 1 : 0; gunAnimClock.restart(); }
        }
        else { upperSprite.setTexture(upperTexture); gunFrame = 0; }
        int t_Playerframe = Player_Frame, xx, yy = 100;
        if (Melee_Activated) xx = 100; else xx = 50;
        if (Is_Firing) { t_Playerframe = gunFrame; yy = 150; }
        int firex_Frame = t_Playerframe * 100;
        if (Is_Firing) firex_Frame = t_Playerframe * 150;
        upperSprite.setTextureRect(IntRect(firex_Frame, 0, yy, xx));
        lowerSprite.setTextureRect(IntRect(Player_Frame * 100, 0, 100, 50));
        if (P_D == 1) { upperSprite.setScale(1.2f, 1.2f); lowerSprite.setScale(1.0f, 1.0f); }
        else { upperSprite.setScale(-1.2f, 1.2f); lowerSprite.setScale(-1.0f, 1.0f); }
        if (P_Frame >= 360) P_Frame = 0;
        if (Melee_Activated && P_Frame % 15 == 2) {
            float knifeX = (P_D == 1) ? player_x + Pwidth : player_x - Pwidth;
            float knifeY = player_y;
            for (int i = 0; i < myManager->Get_Current_Count(); i++) {
                Entity* e = myManager->getEntity(i);
                if (e->getIsDead() || !e->isEnemy()) continue;
                bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), knifeX, knifeY, Pwidth, Pheight, 1, 1);
                if (hit) {
                    e->takeDamage(10.0 * getMeleeMultiplier());
                    if (e->getIsDead() && scorePtr) *scorePtr += 50;
                }
            }
        }
        player_x += velocityX;
        int temp2 = 0, temp1 = 0;
        if (!onGround) { velocityY += gravity; if (velocityY > terminal_Velocity) velocityY = terminal_Velocity; }
        if (P_D == -1) { temp1 = Pwidth; temp2 = -Pwidth; }
        else { temp1 = (int)player_x; temp2 = Pwidth; }
        float tempY = player_y + velocityY;
        int Map_Height = (int)(18 * 1.2f); int Map_Width = 128 * 3;
        int feet_row = (int)(tempY + Pheight) / cell_size;
        int left_col = (temp1) / cell_size;
        int middle_col = (int)(player_x + temp2 / 2.0f) / cell_size;
        int right_col = (int)(player_x + temp2) / cell_size;
        if (feet_row >= Map_Height) feet_row = Map_Height - 1; if (feet_row < 0) feet_row = 0;
        if (left_col < 0) left_col = 0; if (left_col >= Map_Width) left_col = Map_Width - 1;
        if (right_col < 0) right_col = 0; if (right_col >= Map_Width) right_col = Map_Width - 1;
        if (middle_col < 0) middle_col = 0; if (middle_col >= Map_Width) middle_col = Map_Width - 1;
        char below_left = lvl[feet_row][left_col]; char below = lvl[feet_row][middle_col]; char below_right = lvl[feet_row][right_col];
        int head_row = (int)(tempY) / cell_size;
        if (head_row < 0) head_row = 0; if (head_row >= Map_Height) head_row = Map_Height - 1;
        char above_left = lvl[head_row][left_col]; char above = lvl[head_row][middle_col]; char above_right = lvl[head_row][right_col];
        onGround = false;
        float platform_top = (float)(feet_row * cell_size);
        if (below_left == '#' || below_right == '#' || below == '#') {
            onGround = true; velocityY = 0; player_y = platform_top - Pheight;
        }
        else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
            if (wantToFall) { player_y = tempY + 5.0f; onGround = false; }
            else {
                if (prev_feet_y <= platform_top) { onGround = true; velocityY = 0; player_y = platform_top - Pheight; }
                else player_y = tempY;
            }
        }
        else if (velocityY < 0 && (above_left == 'P' || above_right == 'P' || above == 'P' || above_left == 'S' || above_right == 'S' || above == 'S')) {
            player_y = tempY;
        }
        else { player_y = tempY; }

        // Infinite mode: snap to Perlin ground if needed
        if (myMode == 1 && myPerlin != nullptr) {
            float groundY = (10.0f - (myPerlin->getheight(player_x / (float)cell_size) * 5.0f)) * cell_size;
            if (player_y + Pheight > groundY + 5.0f) {
                player_y = groundY - Pheight; velocityY = 0; onGround = true; canJump = true;
            }
        }

        wantToFall = false; canJump = onGround;
        if (player_x <= 50.0f) player_x = 50.0f;
        int temp = 0; if (P_D == -1) temp = 0; else temp = Pwidth;
        float maxMapWidth = (Map_Width * 50.0f) - 50.0f;
        if (player_x >= maxMapWidth - (float)temp) player_x = maxMapWidth - (float)temp;
        if (player_y < 0) { player_y = 0; velocityY = 0; }
        if (tempY > (18 * 50.0f + 50.0f)) { takeDamage(9999); return; }
        this->x = player_x; this->y = player_y;
    }

    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isRiding || currentState == nullptr) return;
        float drawX = player_x - camX; float drawY = player_y - camY;
        float upperOffsetY = 0.0f;
        if (chosenCharacter == 1) upperOffsetY = Melee_Activated ? -35.0f : 15.0f;
        else if (chosenCharacter == 2) upperOffsetY = Melee_Activated ? -40.0f : 10.0f;
        else if (chosenCharacter == 3) upperOffsetY = 20.0f;
        else upperOffsetY = Melee_Activated ? -30.0f : 20.0f;
        int y_Coordinate = (Get_Is_Firing()) ? (int)(drawY + upperOffsetY - 10.0f) : (int)(drawY + upperOffsetY);
        upperSprite.setPosition(drawX, (float)y_Coordinate);
        lowerSprite.setPosition(drawX, drawY + 50.0f);
        window.draw(upperSprite); window.draw(lowerSprite);
    }
};

// ─────────────────────────────────────────────
// NORMAL STATE
// ─────────────────────────────────────────────

class NormalState : public TransformationState {
public:
    void handleInput(PlayerSoldier* player) override {
        float speed = player->getSpeed(); float currentVx = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (player->Get_P_D() == 1) player->addX(100.0f);
            currentVx = -speed; player->setDirection(-1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (player->Get_P_D() == -1) player->addX(-100.0f);
            currentVx = speed; player->setDirection(1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player->setDirection(-1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player->setDirection(1);
        player->setVelocityX(currentVx);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->triggerJump();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->setWantToFall(true);
        if (Keyboard::isKeyPressed(Keyboard::G)) player->throwGRENADE();
        if (Keyboard::isKeyPressed(Keyboard::P)) player->activateSPECIAL();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { player->shoot(); player->Set_Is_Firing(true); }
        else player->Set_Is_Firing(false);
        if (Keyboard::isKeyPressed(Keyboard::Q) && !player->Get_Melee_Activated() && player->Get_Melee_Ready()) {
            if (player->Get_PlayerIndex() != 3) { player->Set_Melee_Activated(true); player->Set_Player_Frame(0); player->Reset_Melee_Timer(); }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) player->switchWeapon(0);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) player->switchWeapon(1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) player->switchWeapon(2);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) player->switchWeapon(3);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) player->switchWeapon(4);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) player->setGrenadeType(6);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) player->setGrenadeType(7);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player->setManualAimAngle(player->getManualAimAngle() - 3.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player->setManualAimAngle(player->getManualAimAngle() + 3.0f);
        player->setManualAimAngle(max(-90.0f, min(90.0f, player->getManualAimAngle())));
    }
};

// ─────────────────────────────────────────────
// PLAYER SUBCLASSES
// ─────────────────────────────────────────────

class Macro_Rossi : public PlayerSoldier {
private:
    Texture anotherone; Sprite anothergun;
public:
    Macro_Rossi(float x, float y, EntityManager* mgr) : PlayerSoldier(x, y, 1, mgr) {
        weapons[1]->modifyFireRate(0.75f); grenadeCount = startinggre - 2; switchWeapon(1);
        anotherone.loadFromFile("other_gun.png"); anothergun.setTexture(anotherone);
        anothergun.setOrigin(187.0f / 2.0f, 95.0f / 2.0f);
    }
    virtual void activateSPECIAL() override {
        if (!specialUsed) { specialActive = true; specialUsed = true; specialDurationTimer.restart(); }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        PlayerSoldier::draw(window, camX, camY);
        if (specialActive) {
            float drawX = player_x - camX; float drawY = player_y - camY;
            float gunScale = 0.4f; float xOffset; float yOffset = 60.0f;
            if (P_D == 1) { anothergun.setScale(-gunScale, gunScale); xOffset = 15.0f; }
            else { anothergun.setScale(gunScale, gunScale); xOffset = -20.0f; }
            anothergun.setPosition(drawX + xOffset, drawY + yOffset); window.draw(anothergun);
        }
    }
};

class Tarma : public PlayerSoldier {
public:
    Tarma(float x, float y, EntityManager* mgr) : PlayerSoldier(x, y, 2, mgr) {
        speed = 9.6f; health = 80; specialMaxTime = 20.0f; switchWeapon(0);
    }
    float getVehicleFireRateBuff() override { return 0.75f; }
    float getVehicleHealthBuff() override { return 1.2f; }
    bool hasTotemOfUndying() override { return true; }
    bool isInvincible() override { return specialActive; }
    virtual void activateSPECIAL() override {
        if (!specialUsed) { specialActive = true; specialUsed = true; specialDurationTimer.restart(); }
    }
    void takeDamage(double damageAmount) override {
        if (specialActive) return;
        PlayerSoldier::takeDamage(damageAmount);
        if (health == 100) health = 80;
    }
};

class Eri_kasamoto : public PlayerSoldier {
public:
    Eri_kasamoto(float x, float y, EntityManager* mgr) : PlayerSoldier(x, y, 3, mgr) {
        grenadeCount = startinggre * 2; weapons[0]->modifyFireRate(1.25f); switchWeapon(0);
    }
    virtual void activateSPECIAL() override {
        if (!specialUsed) { specialActive = true; specialUsed = true; specialDurationTimer.restart(); }
    }
    void throwGRENADE() override {
        if (grenadeCount > 0 && grenadeTimer.getElapsedTime().asSeconds() >= 0.6f) {
            float power = 22.0f; float angleMath = 0.7071f;
            float calcVx = power * angleMath * P_D; float calcVy = -power * angleMath;
            if (grenadeType == 7) {
                myManager->addEntity(new FireBombGrenade(player_x + (Pwidth / 2.0f), player_y + (Pheight / 2.0f), calcVx, calcVy, myManager));
                if (specialActive) myManager->addEntity(new FireBombGrenade(player_x + (Pwidth / 2.0f), player_y + (Pheight / 2.0f) - 20.0f, calcVx + (3.0f * P_D), calcVy, myManager));
            }
            else {
                myManager->addEntity(new PlayerGrenade(player_x + (Pwidth / 2.0f), player_y + (Pheight / 2.0f), calcVx, calcVy, myManager));
                if (specialActive) myManager->addEntity(new PlayerGrenade(player_x + (Pwidth / 2.0f), player_y + (Pheight / 2.0f) - 20.0f, calcVx + (3.0f * P_D), calcVy, myManager));
            }
            grenadeCount--; grenadeTimer.restart();
        }
    }
};

class Fiolina_germi : public PlayerSoldier {
private:
    bool rateFixed;
public:
    Fiolina_germi(float x, float y, EntityManager* mgr) : PlayerSoldier(x, y, 4, mgr) {
        weapons[0]->modifyFireRate(0.9f); grenadeCount = startinggre - 2; rateFixed = false; switchWeapon(0);
    }
    float getAmmoMultiplier() override { return 1.5f; }
    float getMeleeMultiplier() override { return 0.75f; }
    virtual void activateSPECIAL() override {
        if (!specialUsed) { specialActive = true; specialUsed = true; rateFixed = false; specialDurationTimer.restart(); activeWeapon->modifyFireRate(0.5f); }
    }
    void update(char** lvl, int cell_size) override {
        PlayerSoldier::update(lvl, cell_size);
        if (!specialActive && specialUsed && !rateFixed) { activeWeapon->modifyFireRate(2.0f); rateFixed = true; }
    }
    void takeDamage(double damageAmount) override {
        PlayerSoldier::takeDamage(damageAmount); if (health == 100) rateFixed = false;
    }
};

// ─────────────────────────────────────────────
// ENEMY BASE
// ─────────────────────────────────────────────

class Enemy : public DamageableEntity {
protected:
    EntityManager* myManager; Weapon* activeWeapon;
    float gravity, terminal_Velocity, speed, jumpStrength;
    bool onGround, canJump, wantToFall;
    int P_D, Pwidth, Pheight;
    sf::Sprite sprite; int P_Frame, Player_Frame;
    sf::Clock aiTimer; float moveDuration; int aiState;
public:
    Enemy(float startX, float startY, double hp, EntityManager* mgr) : DamageableEntity(startX, startY, hp) {
        myManager = mgr; gravity = 1.0f; jumpStrength = -18.0f; terminal_Velocity = 40.0f; speed = 1.5f;
        onGround = false; canJump = false; wantToFall = false;
        P_D = (rand() % 2 == 0) ? 1 : -1; Pwidth = 70; Pheight = 85;
        P_Frame = 0; Player_Frame = 0; activeWeapon = nullptr;
        aiState = 0; moveDuration = (float)(rand() % 6 + 2); aiTimer.restart();
    }
    virtual ~Enemy() { if (activeWeapon != nullptr) delete activeWeapon; }
    void triggerJump() { if (canJump) { velocityY = jumpStrength; canJump = false; } }
    void setWeapon(Weapon* w) { if (activeWeapon != nullptr) delete activeWeapon; activeWeapon = w; }
    virtual void animate() = 0;
    virtual void shoot();
    void update(char** lvl, int cell_size) override;
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        float drawX = x - camX + (Pwidth / 2.0f); float drawY = y - camY + (Pheight / 2.0f);
        sprite.setPosition(drawX, drawY); window.draw(sprite);
    }
    void checkEdge(char** lvl, int cell_size) {
        if (onGround && velocityX != 0.0f) {
            int aheadCol = (int)((x + (P_D == 1 ? Pwidth + 5 : -5 - Pwidth)) / cell_size);
            int belowRow = (int)((y + Pheight + 1) / cell_size);
            int Map_Height = (int)(18 * 1.2f); int Map_Width = 128 * 3;
            if (aheadCol >= 0 && aheadCol < Map_Width && belowRow >= 0 && belowRow < Map_Height) {
                if (lvl[belowRow][aheadCol] == ' ') { P_D = -P_D; velocityX = -velocityX; }
            }
        }
    }
    bool isEnemy() override { return true; }
    virtual void takeDamage(double damageAmount) override {
        health -= damageAmount;
        if (health <= 0) { if (scorePtr) *scorePtr += getKillScore(); kill(); }
    }
    virtual int getKillScore() { return 50; }
};

// ─────────────────────────────────────────────
// ENEMY TYPES
// ─────────────────────────────────────────────

class rebel_soldier : public Enemy {
private:
    Texture texMove, texJump, texShoot; int frameCount; int RWidth, RHeight;
public:
    rebel_soldier(float x, float y, EntityManager* mgr) : Enemy(x, y, 2, mgr) {
        RWidth = 100; RHeight = 100; Entity::width = RWidth; Entity::height = RHeight;
        frameCount = 0; setWeapon(new pistol());
        texMove.loadFromFile("Rebel_Move.png"); texJump.loadFromFile("Rebel_Jump.png");
        texShoot.loadFromFile("Rebel_Shoot.png"); sprite.setTexture(texMove);
    }
    void animate() override;
    void shoot();
    int getKillScore() override { return 50; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class shieldsoldier : public Enemy {
private:
    Texture texMove, texJump, texShoot; int frameCount; int RWidth, RHeight;
public:
    shieldsoldier(float x, float y, EntityManager* mgr) : Enemy(x, y, 5, mgr) {
        frameCount = 0; RWidth = 100; RHeight = 100; Entity::width = RWidth; Entity::height = RHeight;
        setWeapon(new pistol()); texMove.loadFromFile("Shield_Soldier_Move.png");
        texJump.loadFromFile("Shield_Soldier_Jump.png"); texShoot.loadFromFile("Shield_Soldier_Shoot.png");
        sprite.setTexture(texMove); aiState = 1; moveDuration = (float)(rand() % 9 + 4); aiTimer.restart();
    }
    void animate() override;
    void shoot();
    int getKillScore() override { return 75; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class bazooka_soldier : public Enemy {
private:
    Texture texMove, texJump, texShootStd, texShootCrouch;
    int lastAiState; bool isCrouchShooting; int RWidth, RHeight;
public:
    bazooka_soldier(float x, float y, EntityManager* mgr) : Enemy(x, y, 2, mgr) {
        setWeapon(nullptr); RWidth = 100; RHeight = 100; Entity::width = RWidth; Entity::height = RHeight;
        texMove.loadFromFile("Bazooka_Soldier_Move.png"); texJump.loadFromFile("Bazooka_Soldier_Jump.png");
        texShootStd.loadFromFile("Bazooka_Soldier_Shoot_1.png"); texShootCrouch.loadFromFile("Bazooka_Soldier_Shoot_2.png");
        sprite.setTexture(texMove); lastAiState = -1; isCrouchShooting = false;
        aiState = rand() % 2; moveDuration = (float)(rand() % 5 + 3); aiTimer.restart();
    }
    void animate() override;
    void shoot() override;
    int getKillScore() override { return 100; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class grenade_soldier : public Enemy {
private:
    Texture texMove, texJump; int frameCount; bool hasThrownInAir; int RWidth, RHeight;
public:
    grenade_soldier(float x, float y, EntityManager* mgr) : Enemy(x, y, 2, mgr) {
        frameCount = 0; RWidth = 100; RHeight = 100; Entity::width = RWidth; Entity::height = RHeight;
        setWeapon(nullptr); texMove.loadFromFile("Rebel_Move.png"); texJump.loadFromFile("Rebel_Jump.png");
        sprite.setTexture(texMove); hasThrownInAir = false; speed = 4.5f;
        aiState = 0; moveDuration = (float)(rand() % 4 + 2); aiTimer.restart();
    }
    void animate() override;
    void shoot() override {}
    int getKillScore() override { return 100; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class paratrooper : public Enemy {
private:
    sf::Texture texPara; float glideTimer;
public:
    paratrooper(float x, float y, EntityManager* mgr);
    void animate() override;
    void update(char** lvl, int cell_size) override;
    void decideSTATE();
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class Mummy : public Enemy {
private:
    Texture mumMOVE, mumBEINGHIT, mumCRUMBLE;
    float spawnX, spawnY; int RWidth, RHeight; bool spawnSaved;
public:
    Mummy(float x, float y, EntityManager* mgr) : Enemy(x, y, 5, mgr) {
        RWidth = 100; RHeight = 100; Entity::width = RWidth; Entity::height = RHeight;
        setWeapon(nullptr); mumMOVE.loadFromFile("Mummy_Move.png");
        mumBEINGHIT.loadFromFile("Mummy_Death.png"); mumCRUMBLE.loadFromFile("Mummy_Crumble.png");
        sprite.setTexture(mumMOVE); spawnX = x; spawnY = y; speed = 1.0f; aiState = 0; spawnSaved = false;
    }
    void animate() override;
    void update(char** lvl, int cell_size) override;
    void takeDamage(double damageAmount) override;
    int getKillScore() override { return 150; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class Zombie : public Enemy {
private:
    Texture texMove1, texMove2, texAim, texJump; bool hasShot; int RWidth, RHeight;
public:
    Zombie(float x, float y, EntityManager* mgr);
    void animate() override;
    void update(char** lvl, int cell_size) override;
    void shoot() override;
    int getKillScore() override { return 100; }
    void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY - 10); window.draw(sprite);
    }
};

class Martian : public Enemy {
private:
    Texture MartianBASE, MartianIDLE, MartianMOVE, MartianFALLING, MartianSHOOT, MartianJUMP, MartianPOD, MartianTURN;
    Sprite podSprite; bool isFLYING; int MARTIANHP, PODHP, lastPD; int RWidth, RHeight;
public:
    Martian(float x, float y, EntityManager* mgr);
    void animate() override;
    void update(char** lvl, int cell_size) override;
    void shoot() override;
    void takeDamage(double damageAmount) override;
    int getKillScore() override { return 200; }
    void draw(sf::RenderWindow& window, float camX, float camY) override;
};

// ─────────────────────────────────────────────
// VEHICLES
// ─────────────────────────────────────────────

class Vehicle : public DamageableEntity {
protected:
    float speed; sf::Texture texture; sf::Sprite sprite;
    PlayerSoldier* meraRIDER; double baseHealth;
    float fireRateCooldown; sf::Clock vehicleFireTimer;
public:
    Vehicle(float x, float y, double hp) : DamageableEntity(x, y, hp) {
        speed = 6.0f; baseHealth = hp; meraRIDER = nullptr; fireRateCooldown = 1.0f;
    }
    virtual ~Vehicle() {}
    virtual void RIDEVehicle(PlayerSoldier* driver);
    void takeDamage(double damageAmount) override;
    bool hasRider() const { return meraRIDER != nullptr; }
    Vehicle* asVehicle() override { return this; }
    virtual void fire(EntityManager* mgr) {
        if (meraRIDER == nullptr) return;
        if (vehicleFireTimer.getElapsedTime().asSeconds() >= fireRateCooldown) vehicleFireTimer.restart();
    }
    virtual void moveLeft() { x -= speed; }
    virtual void moveRight() { x += speed; }
    virtual void moveUp() {}
    virtual void moveDown() {}
    virtual void update(char** lvl, int cell_size) {}
    virtual void draw(sf::RenderWindow& window, float camX, float camY) {
        sprite.setPosition(x - camX, y - camY); window.draw(sprite);
    }
};

class AerialVehicle : public Vehicle {
protected:
    float hoverY;
public:
    AerialVehicle(float x, float y, double hp) : Vehicle(x, y, hp) { hoverY = y; }
    void moveUp() override { y -= speed; }
    void moveDown() override { y += speed; }
    void update(char** lvl, int cell_size) override {}
};

class GroundVehicle : public Vehicle {
protected:
    int scale; EntityManager* manager; int width, height;
public:
    GroundVehicle(float x, float y, double hp, EntityManager* mgr, int w, int h) : Vehicle(x, y, hp) {
        width = w; height = h; manager = mgr; scale = 1;
    }
    virtual ~GroundVehicle() {}
    virtual void update(char** lvl, int cell_size) override { return; }
};

class AquaticVehicle : public Vehicle {
protected:
    int scale; EntityManager* manager;
public:
    AquaticVehicle(float x, float y, double hp, EntityManager* mgr) : Vehicle(x, y, hp) {
        manager = mgr; scale = 1;
    }
    virtual ~AquaticVehicle() {}
    virtual void update(char** lvl, int cell_size) override = 0;
};

class MetalSlug : public GroundVehicle {
private:
    Texture slugTexture, slugJumpTexture, slugGunTexture;
    Sprite slugSprite, slugGunSprite; bool isFiring;
    float jumpStrength, gravity, terminalVelocity;
    int PD; bool onGround, canJump, wantToFall;
    int PFrame, PlayerFrame, Pwidth, Pheight;
    sf::Clock gunAnimClock; int gunFrame;
    Weapon* slugWeapon; bool clampToBiome2;
    Perlin* myPerlin; int myMode;
public:
    MetalSlug(float x, float y, EntityManager* mgr) : GroundVehicle(x, y, 25, mgr, 200, 200) {
        speed = 15.0f; jumpStrength = 25.0f; gravity = 0.8f; terminalVelocity = 35.0f;
        PD = 1; onGround = false; canJump = true; wantToFall = false;
        PFrame = 0; PlayerFrame = 0; gunFrame = 0;
        slugWeapon = new ProjectileWeapon(10, 0, true, 0.15f, 25.0f, 6);
        slugTexture.loadFromFile("Metal_Slug.png"); slugJumpTexture.loadFromFile("Metal_Slug_Jump.png");
        slugSprite.setTexture(slugTexture); slugSprite.setTextureRect(IntRect(0, 0, 100, 100)); slugSprite.setScale(2.0f, 2.0f);
        slugGunTexture.loadFromFile("Metal_Slug_Shooter.png");
        slugGunSprite.setTextureRect(IntRect(0, 0, 100, 25)); slugGunSprite.setTexture(slugGunTexture); slugGunSprite.setScale(2.0f, 2.0f);
        isFiring = false; Pwidth = 200; Pheight = 200;
        Entity::width = Pwidth; Entity::height = Pheight;
        clampToBiome2 = true; myPerlin = nullptr; myMode = 0;
    }
    ~MetalSlug() { delete slugWeapon; }
    void update(char** lvl, int cellsize) override;
    void setVelocityX(float vx) { velocityX = vx; }
    void setVelocityY(float vy) { velocityY = vy; }
    int GetPD() const { return PD; }
    int GetPwidth() const { return Pwidth; }
    int GetPheight() const { return Pheight; }
    void setPlayerX(float val) { x = val; }
    void setPlayerY(float val) { y = val; }
    void takeDamage(double damageAmount) override;
    void fire(EntityManager* mgr) override;
    bool isPlayerVehicle() override { return true; }
    void setClamp(bool val) { clampToBiome2 = val; }
    bool isMetalSlug() override { return true; }
    void setPerlinMode(Perlin* p, int mode) { myPerlin = p; myMode = mode; }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        slugSprite.setPosition(x - camX, y - camY); window.draw(slugSprite);
        int xx = (PD == 1) ? 75 : -75;
        slugGunSprite.setPosition(x + xx - camX, y + 55.0f - camY); window.draw(slugGunSprite);
    }
};

class SlugFlyer : public AerialVehicle {
private:
    Texture slugFlyerTexture, slugFlyerGunTexture;
    Sprite slugFlyerSprite, slugFlyerGunSprite; bool isFiring; int gunFrame;
    sf::Clock gunAnimClock; Weapon* flyerWeapon; int missileCount;
    sf::Clock missileTimer; int PD, Pwidth, Pheight;
    float hoverTime, hoverSpeed; EntityManager* manager;
    Perlin* myPerlin; int myMode;
public:
    SlugFlyer(float x, float y, EntityManager* mgr) : AerialVehicle(x, y, 25) {
        manager = mgr; PD = 1; Pwidth = 288; Pheight = 140;
        Entity::width = Pwidth; Entity::height = Pheight;
        isFiring = false; gunFrame = 0; missileCount = 4; hoverTime = 0.0f; hoverSpeed = 0.03f; speed = 20.0f;
        flyerWeapon = new ProjectileWeapon(10, 0, true, 0.3f, 27.5f, 6);
        slugFlyerTexture.loadFromFile("Slug_Flyer.png"); slugFlyerSprite.setTexture(slugFlyerTexture);
        slugFlyerSprite.setTextureRect(IntRect(0, 0, 144, 70)); slugFlyerSprite.setScale(2.0f, 2.0f);
        slugFlyerGunTexture.loadFromFile("Metal_Slug_Shooter.png"); slugFlyerGunSprite.setTexture(slugFlyerGunTexture);
        slugFlyerGunSprite.setTextureRect(IntRect(0, 0, 100, 100)); slugFlyerGunSprite.setScale(2.0f, 2.0f);
        myPerlin = nullptr; myMode = 0;
    }
    ~SlugFlyer() { delete flyerWeapon; }
    void setPerlinMode(Perlin* p, int mode) { myPerlin = p; myMode = mode; }
    void fire(EntityManager* mgr) override;
    void fireMissile(EntityManager* mgr);
    void update(char** lvl, int cell_size) override;
    bool isPlayerVehicle() override { return true; }
    void setPlayerX(float val) { x = val; }
    void setPlayerY(float val) { y = val; }
    bool isSlugFlyer() override { return true; }
    void takeDamage(double damageAmount) override {
        health -= damageAmount;
        if (health <= 0) {
            if (meraRIDER != nullptr) {
                meraRIDER->SetIsRiding(false); meraRIDER->setPlayerY(getY() - 50);
                if (meraRIDER->hasTotemOfUndying()) meraRIDER = nullptr;
                else meraRIDER->takeDamage(9999);
            }
            kill();
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        slugFlyerSprite.setPosition(x - camX, y - camY); window.draw(slugFlyerSprite);
        int xx = (PD == 1) ? +95 : -95;
        slugFlyerGunSprite.setPosition(x + xx - camX, y - camY + 55); window.draw(slugFlyerGunSprite);
    }
};

class SlugMariner : public AquaticVehicle {
private:
    Texture Marine_Texture; Sprite Marine_Sprite;
    int PD, Pwidth, Pheight; float hoverTime, hoverSpeed;
    int Ammo_H, Ammo_V, Ammo_R;
    Clock H_Clock, V_Clock, R_Clock; float Cooldown = 0.35f;
public:
    SlugMariner(float x, float y, EntityManager* mgr) : AquaticVehicle(x, y, 75, mgr) {
        PD = 1; Pwidth = 200; Pheight = 100; hoverTime = 0.f; hoverSpeed = 0.025f;
        Ammo_H = 3; Ammo_V = 3; Ammo_R = 3; speed = 12.0f;
        Entity::width = Pwidth; Entity::height = Pheight;
        Marine_Texture.loadFromFile("Slug_Marine.png"); Marine_Sprite.setTexture(Marine_Texture);
        Marine_Sprite.setTextureRect(sf::IntRect(0, 0, 200, 100)); Marine_Sprite.setScale(1.0f, 1.0f);
    }
    bool isEnemy() override { return false; }
    int getAmmoH() const { return Ammo_H; }
    int getAmmoV() const { return Ammo_V; }
    int getAmmoA() const { return Ammo_R; }
    void setPlayerX(float val) { x = val; }
    void setPlayerY(float val) { y = val; }
    void update(char** lvl, int cell_size) override;
    bool isPlayerVehicle() override { return true; }
    bool isSlugMariner() override { return true; }
    void takeDamage(double damageAmount) override {
        health -= damageAmount;
        if (health <= 0) {
            if (meraRIDER != nullptr) {
                meraRIDER->SetIsRiding(false); meraRIDER->setPlayerY(getY() - 50);
                if (meraRIDER->hasTotemOfUndying()) meraRIDER = nullptr;
                else meraRIDER->takeDamage(9999);
            }
            kill();
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        Marine_Sprite.setPosition(x - camX, y - camY); window.draw(Marine_Sprite);
    }
};

class AmphibiousSlug : public Vehicle {
private:
    MetalSlug* groundSlug; SlugFlyer* airSlug; SlugMariner* waterSlug;
    Vehicle* activeVehicle; int currentMode; EntityManager* manager;
    Texture amphibiousTexture; Sprite amphibiousSprite; bool riderMounted;
public:
    AmphibiousSlug(float startX, float startY, EntityManager* mgr) : Vehicle(startX, startY, 250.0), currentMode(1), manager(mgr) {
        Entity::width = 288; Entity::height = 140;
        groundSlug = new MetalSlug(startX, startY, mgr); groundSlug->setClamp(false);
        airSlug = new SlugFlyer(startX, startY, mgr); waterSlug = new SlugMariner(startX, startY, mgr);
        activeVehicle = airSlug;
        amphibiousTexture.loadFromFile("Amphibious_Slug.png"); amphibiousSprite.setTexture(amphibiousTexture); amphibiousSprite.setScale(1.0f, 2.0f);
        riderMounted = false;
    }
    ~AmphibiousSlug() { delete groundSlug; delete airSlug; delete waterSlug; }
    bool isEnemy() override { return false; }
    void RIDEVehicle(PlayerSoldier* driver) override {
        meraRIDER = driver; riderMounted = (driver != nullptr); activeVehicle->RIDEVehicle(driver);
    }
    void switchMode(int newMode) {
        if (newMode == currentMode) return;
        float px = activeVehicle->getX(); float py = activeVehicle->getY();
        currentMode = newMode;
        float biome2Start = 128.f * 1.f * 50.f; float biome3Start = 128.f * 2.f * 50.f;
        if (currentMode == 0) {
            if (px < biome2Start) px = biome2Start;
            groundSlug->setPlayerX(px); groundSlug->setPlayerY(py); groundSlug->RIDEVehicle(meraRIDER); activeVehicle = groundSlug;
        }
        else if (currentMode == 1) {
            if (px > biome2Start - (float)airSlug->getwidth()) px = biome2Start - (float)airSlug->getwidth();
            airSlug->setPlayerX(px); airSlug->setPlayerY(py); airSlug->RIDEVehicle(meraRIDER); activeVehicle = airSlug;
        }
        else {
            if (px < biome3Start) px = biome3Start;
            waterSlug->setPlayerX(px); waterSlug->setPlayerY(py); waterSlug->RIDEVehicle(meraRIDER); activeVehicle = waterSlug;
        }
    }
    void update(char** lvl, int cell_size) override {
        if (meraRIDER == nullptr) return;
        float biome2Start = 128.f * 1.f * 50.f; float biome3Start = 128.f * 2.f * 50.f;
        int newMode = currentMode;
        if (x >= biome3Start) newMode = 2;
        else if (x >= biome2Start) newMode = 0;
        else newMode = 1;
        switchMode(newMode); activeVehicle->update(lvl, cell_size);
        x = activeVehicle->getX(); y = activeVehicle->getY();
        meraRIDER->setPlayerX(x); meraRIDER->setPlayerY(y);
    }
    void draw(RenderWindow& window, float camX, float camY) override {
        if (riderMounted) activeVehicle->draw(window, camX, camY);
        else { amphibiousSprite.setPosition(x - camX, y - camY); window.draw(amphibiousSprite); }
    }
    void fire(EntityManager* mgr) override { activeVehicle->fire(mgr); }
    float getX() const override { return activeVehicle->getX(); }
    float getY() const override { return activeVehicle->getY(); }
    bool isPlayerVehicle() override { return true; }
    bool isAmphibious() override { return true; }
};

class FlyingTara : public AerialVehicle {
private:
    int Hp; Texture texture; Sprite sprite; EntityManager* manager;
    float hoverTime, hoverSpeed; int scale, frame_F, frame_FT, Fire_Timer, Biome;
public:
    FlyingTara(float x, float y, EntityManager* mgr, int biome) : AerialVehicle(x, y, 75) {
        speed = 4.0f; this->x = x + 128 * 50 * (biome - 1); this->y = y;
        texture.loadFromFile("Flying Tara.png"); sprite.setTexture(texture); sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(x, y); sprite.setTextureRect(IntRect(0, 0, 200, 100));
        scale = 1; Entity::width = 200; Entity::height = 100;
        frame_F = 0; frame_FT = 0; hoverTime = 0.0f; hoverSpeed = 0.03f;
        Fire_Timer = 0; manager = mgr; Biome = biome; Hp = 25;
    }
    void update(char** lvl, int cell_size) override;
    void fire(EntityManager* mgr) override;
    bool isEnemy() override { return true; }
    void takeDamage(double damageAmount) override {
        Hp -= (int)damageAmount;
        if (Hp <= 0) { if (scorePtr) *scorePtr += 750; Drop_Food(x, y, manager); kill(); }
    }
    void draw(RenderWindow& window, float camX, float camY) override {
        sprite.setPosition(x - camX, y - camY); window.draw(sprite);
    }
};

class Bradley : public GroundVehicle {
private:
    EntityManager* manager; Texture texture; Sprite sprite;
    int fireCooldown, scale; float speed, range; int HP, Biome;
public:
    Bradley(float x, float y, EntityManager* mgr, int biome) : GroundVehicle(x, y, 7, mgr, 187, 187) {
        this->x = x + 128 * 50 * (biome - 1); this->y = y;
        range = 900.0f; speed = 2.5f; scale = 1; fireCooldown = 200;
        Entity::width = 187; Entity::height = 187;
        texture.loadFromFile("Bradley.png"); sprite.setTexture(texture); sprite.setScale(1.5f, 1.5f);
        Biome = biome; HP = 7; manager = mgr;
    }
    void update(char** lvl, int cell_size) override;
    void fire(EntityManager* mgr) override;
    bool isEnemy() override { return true; }
    void takeDamage(double damageAmount) override {
        HP -= (int)damageAmount;
        if (HP <= 0) { if (scorePtr) *scorePtr += 750; Drop_Food(x, y, manager); kill(); }
    }
    void draw(RenderWindow& window, float camX, float camY) override {
        sprite.setPosition(x - camX, y - camY); window.draw(sprite);
    }
};

class Enemy_Sub : public AquaticVehicle {
private:
    int Hp; Texture texture; Sprite sprite; EntityManager* manager;
    float hoverTime, hoverSpeed; int scale, frame_F, frame_FT, Fire_Timer, Biome;
public:
    Enemy_Sub(float x, float y, EntityManager* mgr, int biome) : AquaticVehicle(x, y, 150, mgr) {
        speed = 4.0f; this->x = x + 128 * 50 * (biome - 1); this->y = y;
        texture.loadFromFile("Enemy_Sub.png"); sprite.setTexture(texture); sprite.setScale(2.0f, 1.5f);
        sprite.setPosition(x, y); sprite.setTextureRect(IntRect(0, 0, 200, 100));
        scale = 1; Entity::width = 400; Entity::height = 150;
        frame_F = 0; frame_FT = 0; hoverTime = 0.0f; hoverSpeed = 0.03f;
        Fire_Timer = 0; manager = mgr; Biome = biome; Hp = 25;
    }
    void update(char** lvl, int cell_size) override;
    void fire(EntityManager* mgr) override;
    bool isEnemy() override { return true; }
    void takeDamage(double damageAmount) override {
        Hp -= (int)damageAmount;
        if (Hp <= 0) { if (scorePtr) *scorePtr += 750; Drop_Food(x, y, manager); kill(); }
    }
    void draw(RenderWindow& window, float camX, float camY) override {
        sprite.setPosition(x - camX, y - camY); window.draw(sprite);
    }
};

// ─────────────────────────────────────────────
// PICKUP / FOOD / POW
// ─────────────────────────────────────────────

class AmmoCrate : public Entity {
private:
    Texture crateTex; Sprite crateSpr; EntityManager* manager; float Up_Down_Time;
public:
    AmmoCrate(float sx, float sy, EntityManager* mgr) : Entity(sx, sy), manager(mgr), Up_Down_Time(0.0f) {
        width = 250; height = 250;
        crateTex.loadFromFile("Ammo_Crate.png"); crateSpr.setTexture(crateTex); crateSpr.setScale(0.5f, 0.5f);
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        Up_Down_Time += 0.05f; float drawY = y + sin(Up_Down_Time) * 4.0f;
        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead()) continue;
            PlayerSoldier* p = e->asPlayer();
            if (!p || !p->isActive()) continue;
            if (Check_Touch(p->getX(), p->getY(), (int)p->getwidth(), (int)p->getheight(), x, drawY, width, height, p->Get_P_D(), 1)) {
                p->heal(100); p->refillAllWeapons(); kill(); return;
            }
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return;
        float drawY = y + sin(Up_Down_Time) * 4.0f;
        crateSpr.setPosition(x - camX, drawY - camY); window.draw(crateSpr);
    }
};

class POW_Prisoner : public Entity {
private:
    Texture prisonerTexture; Sprite prisonerSprite; EntityManager* manager;
    bool freed; Clock freeClock, animClock; int frame, frameWidth, frameHeight;
public:
    POW_Prisoner(float sx, float sy, EntityManager* mgr) : Entity(sx, sy), manager(mgr), freed(false), frame(0) {
        Entity::width = 80; Entity::height = 80; frameWidth = 40; frameHeight = 40;
        prisonerTexture.loadFromFile("Pow_Prisoner.png"); prisonerSprite.setTexture(prisonerTexture);
        prisonerSprite.setScale(2, 2); prisonerSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (freed) {
            if (animClock.getElapsedTime().asMilliseconds() > 80) {
                frame++; animClock.restart();
                prisonerSprite.setTextureRect(sf::IntRect(frame * frameWidth, frameHeight, frameWidth, frameHeight));
            }
            if (frame >= 25) { manager->addEntity(new AmmoCrate(x, y - 50.f, manager)); if (scorePtr) *scorePtr += 500; kill(); }
            return;
        }
        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead()) continue;
            PlayerSoldier* p = e->asPlayer();
            if (p == nullptr || !p->isActive() || p->GetIsRiding()) continue;
            float dx = p->getX() - x; float dy = p->getY() - y;
            if (sqrt(dx * dx + dy * dy) <= 150.0f) { freed = true; frame = 0; animClock.restart(); return; }
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return; prisonerSprite.setPosition(x - camX, y - camY); window.draw(prisonerSprite);
    }
};

class Foods : public Entity {
protected:
    int S_Value; Texture Food_Texture; Sprite Food_Sprite; EntityManager* manager;
    float Up_Down_Time, Up_Down_Speed, baseY; int frameWidth, frameHeight; Clock animClock; int animFrame;
public:
    Foods(float sx, float sy, int satVal, EntityManager* mgr) : Entity(sx, sy) {
        S_Value = satVal; manager = mgr; Up_Down_Time = 0.f; Up_Down_Speed = 0.05f; baseY = sy;
        frameWidth = 50; frameHeight = 50; animFrame = 0; Entity::width = 50; Entity::height = 50;
    }
    void collect(PlayerSoldier* player) { player->heal(S_Value); kill(); }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        Up_Down_Time += Up_Down_Speed; y = baseY + sin(Up_Down_Time) * 5.0f;
        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i); if (e->getIsDead()) continue;
            PlayerSoldier* p = e->asPlayer(); if (p == nullptr || !p->isActive() || p->GetIsRiding()) continue;
            bool hit = Check_Touch(p->getX(), p->getY(), (int)p->getwidth(), (int)p->getheight(), x, y, width, height, p->Get_P_D(), 1);
            if (hit) { collect(p); return; }
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return; Food_Sprite.setPosition(x - camX, y - camY); window.draw(Food_Sprite);
    }
};

class Turkey : public Foods {
public:
    Turkey(float sx, float sy, EntityManager* mgr) : Foods(sx, sy, 3, mgr) {
        frameWidth = 50; frameHeight = 50; Entity::width = 50; Entity::height = 50;
        Food_Texture.loadFromFile("Turkey.png"); Food_Sprite.setTexture(Food_Texture); Food_Sprite.setScale(1.0f, 1.0f);
    }
};
class Fruit : public Foods {
public:
    Fruit(float sx, float sy, EntityManager* mgr) : Foods(sx, sy, 2, mgr) {
        frameWidth = 50; frameHeight = 50; Entity::width = 50; Entity::height = 50;
        Food_Texture.loadFromFile("Fruit.png"); Food_Sprite.setTexture(Food_Texture); Food_Sprite.setScale(1.0f, 1.0f);
    }
};

// ─────────────────────────────────────────────
// LEVEL MANAGER
// ─────────────────────────────────────────────

class LevelManager {
private:
    sf::Texture Biome1, Biome2, Biome3;
    sf::Sprite S_Biome1, S_Biome2, S_Biome3;
    sf::Texture Tile1, Tile2, Tile3; sf::Sprite TileSprite;
public:
    LevelManager() {
        Biome1.loadFromFile("Arial.png"); S_Biome1.setTexture(Biome1);
        Biome2.loadFromFile("Plains.png"); S_Biome2.setTexture(Biome2);
        Biome3.loadFromFile("Aquatic.png"); S_Biome3.setTexture(Biome3);
        Tile1.loadFromFile("Arial_Tile.png"); Tile2.loadFromFile("Plains_Tile.png"); Tile3.loadFromFile("Aquatic_Tile.png");
    }

    // Procedural terrain for infinite mode
    char** createTERRAIN(int height, int width, float playerCurrentX, Perlin* magic) {
        float mystretch = 5.0f; float base = height / 2;
        char** myterrain = new char* [height];
        for (int i = 0; i < height; i++) myterrain[i] = new char[width];
        for (int i = 0; i < height; i++) for (int j = 0; j < width; j++) myterrain[i][j] = ' ';
        for (int i = 0; i < width; i++) {
            float realX = playerCurrentX + i;
            float heightVAL = magic->getheight(realX);
            int myY = (int)(base - (heightVAL * mystretch));
            if (myY < 0) myY = 0; if (myY >= height) myY = height - 1;
            for (int j = myY; j < height; j++) {
                if (j == myY) myterrain[j][i] = 'P';
                else myterrain[j][i] = '#';
            }
        }
        return myterrain;
    }

    void drawInfiniteLevel(sf::RenderWindow& window, Perlin* profile, float cameraX, float cameraY, int screenWidth, int screenHeight, int cell_size) {
        float bgX = -std::fmod(cameraX * 0.2f, 1600.0f);
        S_Biome2.setPosition(bgX, 0); window.draw(S_Biome2);
        S_Biome2.setPosition(bgX + 1600.0f, 0); window.draw(S_Biome2);
        int startCol = (int)(cameraX / cell_size); int endCol = startCol + (screenWidth / cell_size) + 1;
        for (int col = startCol; col <= endCol; col++) {
            float globalX = (float)(col * cell_size);
            float noiseHeight = profile->getheight((float)col);
            int groundRow = (int)(10.0f - (noiseHeight * 5.0f));
            for (int row = groundRow; row < 22; row++) {
                float drawX = globalX - cameraX; float drawY = (float)(row * cell_size) - cameraY;
                if (row < 12) TileSprite.setTexture(Tile2); else TileSprite.setTexture(Tile3);
                TileSprite.setPosition(drawX, drawY); window.draw(TileSprite);
            }
        }
    }

    char** createLevel1(int height, int width) {
        char** lvl = new char* [height];
        for (int i = 0; i < height; i++) { lvl[i] = new char[width]; for (int j = 0; j < width; j++) lvl[i][j] = ' '; }
        for (int i = 0; i < height; i++) { lvl[i][0] = '#'; lvl[i][width - 1] = '#'; }
        for (int j = width / 3; j < width; j++) lvl[height - 1][j] = '#';
        for (int j = 3, i = 0; j < width / 3 - 6; i += 2, j += 6 + 2 * i) { lvl[height - 2][j] = 'P'; lvl[height - 2][j + 1] = 'P'; lvl[height - 2][j + 2] = 'P'; lvl[height - 2][j + 3] = 'P'; }
        for (int i = 8; i < height - 2; i += 4) for (int j = 2; j < width - 4; j += 10 + i / 3) { lvl[i][j] = 'P'; lvl[i][j + 1] = 'P'; lvl[i][j + 2] = 'P'; lvl[i][j + 3] = 'P'; }
        return lvl;
    }
    char** createLevel2(int height, int width) { return createLevel1(height, width); }
    char** createLevel3(int height, int width) { return createLevel1(height, width); }
    char** createBossLevel(int height, int width) {
        char** lvl = new char* [height];
        for (int i = 0; i < height; i++) { lvl[i] = new char[width]; for (int j = 0; j < width; j++) lvl[i][j] = ' '; }
        for (int i = 0; i < height; i++) { lvl[i][0] = '#'; lvl[i][width - 1] = '#'; }
        for (int j = 0; j < width / 3; j++) lvl[height - 1][j] = '#';
        for (int j = 2 * width / 3; j < width; j++) lvl[height - 1][j] = '#';
        for (int j = 3 + width / 3, i = 0; j < 2 * width / 3 - 6; i += 2, j += 6 + 2 * i) { lvl[height - 2][j] = 'P'; lvl[height - 2][j + 1] = 'P'; lvl[height - 2][j + 2] = 'P'; lvl[height - 2][j + 3] = 'P'; }
        for (int i = 8; i < height - 2; i += 4) for (int j = 2; j < width - 4; j += 10 + i / 3) { lvl[i][j] = 'P'; lvl[i][j + 1] = 'P'; lvl[i][j + 2] = 'P'; lvl[i][j + 3] = 'P'; }
        return lvl;
    }

    void drawLevel(sf::RenderWindow& window, char** lvl, int height, int width, int cell_size, float cameraY, float cameraX, bool bossLevel = false) {
        float totalWidth = (float)(width * cell_size); float segmentWidth = totalWidth / 3.0f;
        if (bossLevel) {
            S_Biome2.setPosition(0 - cameraX, 0 - cameraY); S_Biome2.setScale(4.0f, 1.2f); window.draw(S_Biome2);
            S_Biome1.setPosition(segmentWidth - cameraX, 0 - cameraY); S_Biome1.setScale(4.0f, 1.2f); window.draw(S_Biome1);
        }
        else {
            S_Biome1.setPosition(0 - cameraX, 0 - cameraY); S_Biome1.setScale(4.0f, 1.2f); window.draw(S_Biome1);
            S_Biome2.setPosition(segmentWidth - cameraX, 0 - cameraY); S_Biome2.setScale(4.0f, 1.2f); window.draw(S_Biome2);
        }
        S_Biome3.setPosition(2 * segmentWidth - cameraX, 0 - cameraY); S_Biome3.setScale(4.0f, 1.2f); window.draw(S_Biome3);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == '#' || lvl[i][j] == 'P' || lvl[i][j] == 'S') {
                    float X_C = (j * cell_size) - cameraX; float Y_C = (i * cell_size) - cameraY;
                    if (X_C + cell_size < 0 || X_C > width * 50) continue;
                    if (Y_C + cell_size < 0 || Y_C > height * 50) continue;
                    float Level_X = (float)(j * cell_size);
                    if (bossLevel) {
                        if (Level_X < (width * cell_size) / 3.0f) TileSprite.setTexture(Tile2);
                        else if (Level_X < 2.0f * (width * cell_size) / 3.0f) TileSprite.setTexture(Tile1);
                        else TileSprite.setTexture(Tile3);
                    }
                    else {
                        if (Level_X < (width * cell_size) / 3.0f) TileSprite.setTexture(Tile1);
                        else if (Level_X < 2.0f * (width * cell_size) / 3.0f) TileSprite.setTexture(Tile2);
                        else TileSprite.setTexture(Tile3);
                    }
                    TileSprite.setPosition(X_C, Y_C); window.draw(TileSprite);
                }
            }
        }
    }
};

// ─────────────────────────────────────────────
// GAME STATE SYSTEM
// ─────────────────────────────────────────────

class GameState {
public:
    virtual ~GameState() {}
    virtual void update(GameStateManager* gsm) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

class GameStateManager {
private:
    GameState* activeState;
public:
    GameStateManager();
    ~GameStateManager() { if (activeState != nullptr) delete activeState; }
    void changeState(GameState* newState) { if (activeState != nullptr) delete activeState; activeState = newState; }
    void update() { if (activeState != nullptr) activeState->update(this); }
    void draw(sf::RenderWindow& window) { if (activeState != nullptr) activeState->draw(window); }
};

class LevelSelectState : public GameState {
private:
    sf::Texture bgTexture; sf::Sprite bgSprite;
    sf::Texture arrowTexture; sf::Sprite arrowSprite;
    int chosenChar, selectedMode, selectedLevel;
    float arrowX, arrowY; sf::Clock inputDelay;
public:
    LevelSelectState(int character, int mode) {
        chosenChar = character; selectedMode = mode; selectedLevel = 1;
        arrowX = 240.0f; arrowY = 680.0f;
        bgTexture.loadFromFile("Lvl_Sel.png"); bgSprite.setTexture(bgTexture); bgSprite.setPosition(0, 0);
        arrowTexture.loadFromFile("Arrow2.png"); arrowSprite.setTexture(arrowTexture); arrowSprite.setScale(0.7f, 0.5f);
        arrowSprite.setTextureRect(sf::IntRect(0, 0, 219, 230));
    }
    void update(GameStateManager* gsm) override;
    void draw(sf::RenderWindow& window) override { window.draw(bgSprite); window.draw(arrowSprite); }
};

class EndScreenState : public GameState {
private:
    Font font; Text titleText, scoreText, promptText;
    int finalScore; char scoreChars[7]; Clock inputDelay; sf::Music endMusic;
public:
    EndScreenState(int score) : finalScore(score) {
       // endMusic.openFromFile("Boss.ogg");
       // endMusic.setLoop(true);
       // endMusic.play();
        font.loadFromFile("Font.ttf");
        titleText.setFont(font); titleText.setCharacterSize(80); titleText.setFillColor(sf::Color::Red);
        titleText.setString("GAME OVER"); titleText.setPosition(500.0f, 200.0f);
        scoreText.setFont(font); scoreText.setCharacterSize(60); scoreText.setFillColor(sf::Color::White);
        scoreText.setString("SCORE: " + std::to_string(finalScore)); scoreText.setPosition(550.0f, 350.0f);
        promptText.setFont(font); promptText.setCharacterSize(40); promptText.setFillColor(sf::Color::Yellow);
        promptText.setString("PRESS ENTER TO RETURN TO MENU"); promptText.setPosition(380.0f, 500.0f);
        inputDelay.restart();
    }
    ~EndScreenState() { endMusic.stop(); }
    void update(GameStateManager* gsm) override {
        if (inputDelay.getElapsedTime().asSeconds() > 1.0f)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) exit(0);
    }
    void draw(sf::RenderWindow& window) override {
        window.draw(titleText); window.draw(scoreText); window.draw(promptText);
    }
};

// ─────────────────────────────────────────────
// BOSS CLASSES (kept from partner's code)
// ─────────────────────────────────────────────

class Boss : public DamageableEntity {
public:
    bool phaseOver = false;
    Boss(float x, float y, double hp) : DamageableEntity(x, y, hp) {}
    virtual ~Boss() {}
    void setScorePtr(int* p) { scorePtr = p; }
    bool isEnemy() override { return true; }
};

class FlameShot_Projectile : public Entity {
private:
    float timer, fuseTime; bool exploded; int dir;
    Texture flameTex; Sprite flameSpr;
    Texture explosionTex; Sprite explosionSpr;
    int frame, maxFrames; Clock animClock; EntityManager* manager;
public:
    FlameShot_Projectile(float sx, float sy, int direction, EntityManager* mgr)
        : Entity(sx, sy), dir(direction), timer(0.f), fuseTime(1.5f), exploded(false), frame(0), maxFrames(3), manager(mgr) {
        width = 50; height = 50; velocityX = dir * 8.0f; velocityY = 0.0f;
        flameTex.loadFromFile("Flames.png"); flameSpr.setTexture(flameTex);
        explosionTex.loadFromFile("Grenade_Explosion.png"); explosionSpr.setTexture(explosionTex); explosionSpr.setScale(0.5, 0.5);
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (!exploded) {
            x += velocityX; timer += 0.016f;
            Entity* P = manager->getPlayerEntity();
            if (P == nullptr) { velocityX = 0; return; }
            PlayerSoldier* player = P->asPlayer();
            if (player && !player->getIsDead()) {
                if (Check_Touch(player->getX(), player->getY(), (int)player->getwidth(), (int)player->getheight(), x, y, width, height, player->Get_P_D(), dir)) {
                    player->takeDamage(2); exploded = true; frame = 0; animClock.restart(); return;
                }
            }
            if (timer >= fuseTime) { exploded = true; frame = 0; animClock.restart(); }
        }
        else {
            if (animClock.getElapsedTime().asMilliseconds() > 90) { frame++; animClock.restart(); }
            if (frame >= maxFrames * 2) kill();
            explosionSpr.setTextureRect(sf::IntRect((frame % 3) * 225, 0, 225, 225));
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (!exploded) { flameSpr.setPosition(x - camX, y - camY); window.draw(flameSpr); }
        else { int xx = (dir == -1) ? 0 : 100; explosionSpr.setPosition(x - camX - 60 + xx, y - camY - 40); window.draw(explosionSpr); }
    }
};

class FireBomb : public Entity {
private:
    float timer, fuseTime; bool exploded; int dir;
    Texture bombTex, explosionTex; Sprite bombSpr, explosionSpr;
    int frame, maxFrames; Clock animClock; EntityManager* manager;
public:
    FireBomb(float sx, float sy, float vx, float vy, EntityManager* mgr, float ft)
        : Entity(sx, sy), timer(0.f), fuseTime(ft), exploded(false), frame(0), maxFrames(3), manager(mgr) {
        width = 50; height = 50; velocityX = vx; velocityY = vy; dir = (vx > 0) ? 1 : -1;
        bombTex.loadFromFile("Enemy_rocket.png"); bombSpr.setTexture(bombTex); bombSpr.setScale(dir * 0.5f, 1.0f);
        explosionTex.loadFromFile("Boss_Bombs.png"); explosionSpr.setTexture(explosionTex);
    }
    void update(char** lvl, int cell_size) {
        if (isDead) return;
        if (!exploded) {
            velocityY += 0.5f; x += velocityX; y += velocityY; timer += 0.016f;
            Entity* P = manager->getPlayerEntity();
            if (P == nullptr) { velocityX = 0; return; }
            PlayerSoldier* player = P->asPlayer();
            if (player && !player->getIsDead()) {
                if (Check_Touch(player->getX(), player->getY(), (int)player->getwidth(), (int)player->getheight(), x, y, width, height, player->Get_P_D(), dir)) {
                    player->takeDamage(20); exploded = true; frame = 0; animClock.restart(); return;
                }
            }
            for (int i = 0; i < manager->Get_Current_Count(); i++) {
                Entity* e = manager->getEntity(i);
                if (e->getIsDead() || !e->isPlayerVehicle()) continue;
                Vehicle* v = e->asVehicle(); if (v && !v->hasRider()) continue;
                if (Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1)) {
                    e->takeDamage(20); exploded = true; frame = 0; animClock.restart(); return;
                }
            }
            if (timer >= fuseTime) { exploded = true; frame = 0; animClock.restart(); }
        }
        else {
            if (animClock.getElapsedTime().asMilliseconds() > 90) { frame++; animClock.restart(); }
            if (frame >= maxFrames * 2) kill();
            explosionSpr.setTextureRect(sf::IntRect((frame % 3) * 225, 0, 225, 225));
        }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (!exploded) { bombSpr.setPosition(x - camX, y - camY); window.draw(bombSpr); }
        else { explosionSpr.setPosition(x - camX - 60, y - camY - 60); window.draw(explosionSpr); }
    }
};

class IronNokana : public Boss {
private:
    EntityManager* manager; Texture bodyTex; Sprite bodySpr;
    float speed; int dir; float minX, maxX;
    Clock missileClock, flameClock, firebombClock, minionClock;
    bool retreating; float retreatDir;
public:
    IronNokana(float sx, float sy, EntityManager* mgr) : Boss(sx, sy, 30.0), manager(mgr), speed(1.5f), dir(1), retreating(false) {
        Entity::width = 500; Entity::height = 400; minX = 0 + 100.0f; maxX = 6400 - 300.0f;
        bodyTex.loadFromFile("Phase_1_Boss.png"); bodySpr.setTexture(bodyTex); bodySpr.setScale(1.0f, 1.0f);
    }
    void setScorePtr(int* p) { scorePtr = p; }
    bool isEnemy() override { return true; }
    void takeDamage(double dmg) override {
        if (retreating) return; health -= dmg;
        if (health <= 15.0) { retreating = true; phaseOver = true; }
        if (health <= 0) { if (scorePtr) *scorePtr += 3000; kill(); }
    }
    void spawnMinion() {
        int batch = (rand() % 3) + 2;
        for (int i = 0; i < batch; i++) {
            float mx = x + (rand() % 400) - 200.0f; int roll = rand() % 3; Enemy* minion = nullptr;
            if (roll == 0) minion = new rebel_soldier(mx, y - 50.f, manager);
            else if (roll == 1) minion = new shieldsoldier(mx, y - 50.f, manager);
            else minion = new bazooka_soldier(mx, y - 50.f, manager);
            minion->setScorePtr(scorePtr); manager->addEntity(minion);
        }
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (retreating) { x -= 20.0f; if (x < 400.f) kill(); bodySpr.setScale(1.0f, 1.0f); return; }
        x += speed * dir;
        if (x < minX + width && dir == -1) { x = minX; dir = 1; }
        if (x > maxX - width && dir == 1) { x = maxX; dir = -1; }
        bodySpr.setScale((dir == 1) ? 1 : -1, 1);
        Entity* P = manager->getPlayerEntity(); if (P == nullptr) { velocityX = 0; return; }
        PlayerSoldier* player = P->asPlayer(); if (!player) return;
        dir = (player->getX() > x) ? 1 : -1;
        if (missileClock.getElapsedTime().asSeconds() >= 3.0f) {
            int xxx = (dir == 1) ? 420 : 500 - 380;
            manager->addEntity(new Missile(x + xxx, y + 10, dir, manager)); missileClock.restart();
        }
        if (flameClock.getElapsedTime().asSeconds() >= 0.15f) {
            int xxx = (dir == 1) ? 200 : 500 - 300;
            manager->addEntity(new FlameShot_Projectile(x + xxx, y + height - 155, dir, manager)); flameClock.restart();
        }
        if (firebombClock.getElapsedTime().asSeconds() >= 4.0f) {
            int xxx = (dir == 1) ? 320 : 500 - 320;
            manager->addEntity(new FireBomb(x + xxx, y + 65, dir * 15.0f, 0.f, manager, 0.8)); firebombClock.restart();
        }
        if (minionClock.getElapsedTime().asSeconds() >= 12.0f) { spawnMinion(); minionClock.restart(); }
        if (Check_Touch(player->getX(), player->getY(), (int)player->getwidth(), (int)player->getheight(), x, y, width, height, player->Get_P_D(), dir))
            player->takeDamage(1);
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return;
        if (dir == -1) bodySpr.setPosition(x - camX + width, y - camY);
        else bodySpr.setPosition(x - camX, y - camY);
        window.draw(bodySpr);
    }
};

class HairbusterRiberts : public Boss {
private:
    EntityManager* manager; Texture bodyTex; Sprite bodySpr;
    float speed, hoverTime, minX, maxX, baseY; int dir;
    Clock rocketClock, firebombClock, grenadeClock, minionClock; bool retreating;
public:
    HairbusterRiberts(float sx, float sy, EntityManager* mgr) : Boss(sx, sy, 30.0), manager(mgr), speed(5.0f), dir(1), hoverTime(0.f), retreating(false) {
        Entity::width = 500; Entity::height = 300; minX = 6400.0f + 100.0f; maxX = 12800.0f - 300.0f; baseY = sy;
        bodyTex.loadFromFile("Phase_2_Boss.png"); bodySpr.setTexture(bodyTex); bodySpr.setScale(1, 1);
    }
    void setScorePtr(int* p) { scorePtr = p; }
    bool isEnemy() override { return true; }
    void takeDamage(double dmg) override {
        if (retreating) return; health -= dmg;
        if (health <= 15.0) { retreating = true; phaseOver = true; }
        if (health <= 0) { if (scorePtr) *scorePtr += 3000; kill(); }
    }
    void spawnMinion() {
        int batch = (rand() % 3) + 2;
        for (int i = 0; i < batch; i++) {
            float mx = minX + (float)(rand() % (int)(maxX - minX));
            paratrooper* pt = new paratrooper(mx, -50.f, manager); pt->setScorePtr(scorePtr); manager->addEntity(pt);
        }
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (retreating) { y -= 8.0f; if (y < -300.f) kill(); return; }
        hoverTime += 0.03f; y = baseY + sin(hoverTime) * 20.0f;
        x += speed * dir;
        if (x < minX + width && dir == -1) { x = minX; dir = 1; }
        if (x > maxX - width && dir == 1) { x = maxX; dir = -1; }
        bodySpr.setScale((dir == 1) ? 1 : -1, 1);
        Entity* P = manager->getPlayerEntity(); if (P == nullptr) { velocityX = 0; return; }
        PlayerSoldier* player = P->asPlayer(); if (!player) return;
        if (rocketClock.getElapsedTime().asSeconds() >= 0.3f) {
            int xx = (dir == 1) ? 450 : 500 - 450;
            PlayerGrenade* pg = new PlayerGrenade(x + xx, y + height - 10, 10 * dir, 0, manager);
            pg->setEnemyGrenade(true); manager->addEntity(pg); rocketClock.restart();
        }
        if (firebombClock.getElapsedTime().asSeconds() >= 5.0f) {
            int xx = (dir == 1) ? 450 : 500 - 450;
            float vx = (player->getX() - x > 0) ? 6.f : -6.f;
            manager->addEntity(new FireBomb(x + width / 2.f, y + height / 2, vx, -10.f, manager, 2.5)); firebombClock.restart();
        }
        if (grenadeClock.getElapsedTime().asSeconds() >= 2.0f) {
            int xx = (dir == 1) ? 100 : 500 - 100;
            manager->addEntity(new Tara_Grenade(x + xx, y + height - 50, dir, manager));
            manager->addEntity(new Tara_Grenade(x + xx + 100 * dir, y + height - 50, -dir, manager)); grenadeClock.restart();
        }
        if (minionClock.getElapsedTime().asSeconds() >= 12.0f) { spawnMinion(); minionClock.restart(); }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return;
        if (dir == -1) bodySpr.setPosition(x - camX + width, y - camY);
        else bodySpr.setPosition(x - camX, y - camY);
        window.draw(bodySpr);
    }
};

class SeaSatan : public Boss {
private:
    EntityManager* manager; Texture bodyTex; Sprite bodySpr;
    float speed, hoverTime, minX, maxX, baseY; int dir;
    Clock hmgLeftClock, hmgRightClock, rocketClock, minionClock; bool retreating;
public:
    SeaSatan(float sx, float sy, EntityManager* mgr) : Boss(sx, sy, 30.0), manager(mgr), speed(3.5f), dir(1), hoverTime(0.f), retreating(false) {
        Entity::width = 500; Entity::height = 300; minX = 12800.0f + 100.0f; maxX = 19200.0f - 500.0f; baseY = sy;
        bodyTex.loadFromFile("Phase_3_Boss.png"); bodySpr.setTexture(bodyTex);
    }
    void setScorePtr(int* p) { scorePtr = p; }
    bool isEnemy() override { return true; }
    void takeDamage(double dmg) override {
        if (retreating) return; health -= dmg;
        if (health <= 15.0) { retreating = true; phaseOver = true; }
        if (health <= 0) { if (scorePtr) *scorePtr += 3000; kill(); }
    }
    void spawnMinion() {
        int batch = (rand() % 3) + 2;
        for (int i = 0; i < batch; i++) {
            float mx = minX + (float)(rand() % (int)(maxX - minX));
            Zombie* zb = new Zombie(mx, baseY - 100.f, manager); zb->setScorePtr(scorePtr); manager->addEntity(zb);
        }
    }
    void fireHMG(float originX, float originY, int direction) {
        float vx = direction * 15.0f;
        Bullet* b = new Bullet(originX, originY, vx, 0.f, 3, 2, (direction == 1 ? 0.f : 180.f), manager);
        b->setEnemyBullet(true); manager->addEntity(b);
    }
    void update(char** lvl, int cell_size) override {
        if (isDead) return;
        if (retreating) { y += 8.0f; if (y > 19200.f) kill(); return; }
        hoverTime += 0.025f; y = baseY + sin(hoverTime) * 10.0f; x += speed * dir;
        if (x < minX + width && dir == -1) { x = minX; dir = 1; }
        if (x > maxX - width && dir == 1) { x = maxX; dir = -1; }
        bodySpr.setScale((dir == 1) ? 1 : -1, 1);
        if (hmgLeftClock.getElapsedTime().asSeconds() >= 0.1f) { fireHMG(x + 100, y + height - 80, -1); hmgLeftClock.restart(); }
        if (hmgRightClock.getElapsedTime().asSeconds() >= 0.1f) { fireHMG(x + width - 75, y + height - 80, 1); hmgRightClock.restart(); }
        if (rocketClock.getElapsedTime().asSeconds() >= 3.0f) {
            manager->addEntity(new Enemy_Sub_Missile(x + width / 2.f, y, dir, manager));
            manager->addEntity(new Enemy_Sub_Missile(x + width / 2.f, y, -dir, manager)); rocketClock.restart();
        }
        if (minionClock.getElapsedTime().asSeconds() >= 12.0f) { spawnMinion(); minionClock.restart(); }
    }
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead) return;
        if (dir == -1) bodySpr.setPosition(x - camX + width, y - camY);
        else bodySpr.setPosition(x - camX, y - camY);
        window.draw(bodySpr);
    }
};

class UltimateBoss;
class UltimateBossState {
public:
    virtual ~UltimateBossState() {}
    virtual void update(UltimateBoss* boss, EntityManager* mgr) = 0;
    virtual void draw(UltimateBoss* boss, sf::RenderWindow& window, float camX, float camY) = 0;
    virtual int getName() const = 0;
};

class UltimateBoss : public Boss {
private:
    EntityManager* manager; double groundHP, aerialHP, aquaticHP;
    UltimateBossState* currentState; Clock stateClock; float stateTime;
    Texture groundTex, aerialTex, aquaticTex; Sprite groundSpr, aerialSpr, aquaticSpr;
    Clock minionClock, attackClock; float speed; int dir; float hoverTime, baseY;
public:
    float x_get() { return x; } float y_get() { return y; }
    int width_get() { return width; } int height_get() { return height; }
    int& dirRef() { return dir; } float& hoverTimeRef() { return hoverTime; } float& baseYRef() { return baseY; }
    float getSpeed() { return speed; } EntityManager* getMgr() { return manager; } int* getScorePtr() { return scorePtr; }
    double getGroundHP() const { return groundHP; } double getAerialHP() const { return aerialHP; } double getAquaticHP() const { return aquaticHP; }
    void damageGroundHP(double d) { groundHP -= d; if (groundHP < 0) groundHP = 0; }
    void damageAerialHP(double d) { aerialHP -= d; if (aerialHP < 0) aerialHP = 0; }
    void damageAquaticHP(double d) { aquaticHP -= d; if (aquaticHP < 0) aquaticHP = 0; }
    Sprite& getGroundSpr() { return groundSpr; } Sprite& getAerialSpr() { return aerialSpr; } Sprite& getAquaticSpr() { return aquaticSpr; }
    void moveX(float dx) { x += dx; } void moveY(float dy) { y += dy; } void setY(float val) { y = val; }
    bool isEnemy() override { return true; }
    UltimateBoss(float sx, float sy, EntityManager* mgr) : Boss(sx, sy, 90.0), manager(mgr), groundHP(30.0), aerialHP(30.0), aquaticHP(30.0), speed(4.0f), dir(1), hoverTime(0.f) {
        Entity::width = 500; Entity::height = 400; baseY = sy; stateTime = 10.0f; currentState = nullptr;
        groundTex.loadFromFile("Phase_1_Boss.png"); groundSpr.setTexture(groundTex); groundSpr.setScale(1, 1);
        aerialTex.loadFromFile("Phase_2_Boss.png"); aerialSpr.setTexture(aerialTex); aerialSpr.setScale(1, 1);
        aquaticTex.loadFromFile("Phase_3_Boss.png"); aquaticSpr.setTexture(aquaticTex);
    }
    ~UltimateBoss() { delete currentState; }
    void setScorePtr(int* p) { scorePtr = p; }
    void setState(UltimateBossState* newState);
    void takeDamage(double dmg) override;
    void spawnMinion();
    void update(char** lvl, int cell_size) override;
    void draw(sf::RenderWindow& window, float camX, float camY) override {
        if (isDead || !currentState) return; currentState->draw(this, window, camX, camY);
    }
};

class UltGroundState : public UltimateBossState {
    Clock missileClock, flameClock;
public:
    int getName() const override { return 0; }
    void update(UltimateBoss* boss, EntityManager* mgr) override {
        boss->moveX(boss->getSpeed() * boss->dirRef());
        float lx = 100.0f, rx = 6400 - 300.0f;
        if (boss->x_get() < lx && boss->dirRef() == -1) { boss->moveX(lx - boss->x_get()); boss->dirRef() = 1; }
        if (boss->x_get() > rx - boss->width_get() && boss->dirRef() == 1) { boss->moveX(rx - boss->x_get()); boss->dirRef() = -1; }
        if (missileClock.getElapsedTime().asSeconds() >= 3.0f) {
            int xxx = (boss->dirRef() == 1) ? 420 : 500 - 380;
            mgr->addEntity(new Missile(boss->x_get() + xxx, boss->y_get() + 10, boss->dirRef(), mgr)); missileClock.restart();
        }
        if (flameClock.getElapsedTime().asSeconds() >= 0.15f) {
            int xxx = (boss->dirRef() == 1) ? 200 : 500 - 300;
            mgr->addEntity(new FlameShot_Projectile(boss->x_get() + xxx, boss->y_get() + boss->height_get() - 155, boss->dirRef(), mgr)); flameClock.restart();
        }
        boss->getGroundSpr().setScale((boss->dirRef() == 1 ? 1 : -1), 1);
    }
    void draw(UltimateBoss* boss, sf::RenderWindow& window, float camX, float camY) override {
        if (boss->dirRef() == -1) boss->getGroundSpr().setPosition(boss->x_get() - camX + boss->width_get(), boss->y_get() - camY);
        else boss->getGroundSpr().setPosition(boss->x_get() - camX, boss->y_get() - camY);
        window.draw(boss->getGroundSpr());
    }
};

class UltAerialState : public UltimateBossState {
    Clock rocketClock, bombClock;
public:
    int getName() const override { return 1; }
    void update(UltimateBoss* boss, EntityManager* mgr) override {
        boss->hoverTimeRef() += 0.03f; boss->setY(boss->baseYRef() + sin(boss->hoverTimeRef()) * 25.0f);
        boss->moveX(boss->getSpeed() * 1.3f * boss->dirRef());
        float lx = 6400 - 100, rx = 12800 - 300.0f;
        if (boss->x_get() < lx + boss->width_get() && boss->dirRef() == -1) { boss->moveX(lx - boss->x_get()); boss->dirRef() = 1; }
        if (boss->x_get() > rx - boss->width_get() && boss->dirRef() == 1) { boss->moveX(rx - boss->x_get()); boss->dirRef() = -1; }
        if (rocketClock.getElapsedTime().asSeconds() >= 2.0f) {
            int xx = (boss->dirRef() == 1) ? 100 : 500 - 100;
            mgr->addEntity(new FlyerMissile(boss->x_get() + xx, boss->y_get() + boss->height_get() - 150, 1));
            mgr->addEntity(new FlyerMissile(boss->x_get() + xx + 100 * boss->dirRef(), boss->y_get() + boss->height_get() - 150, -1)); rocketClock.restart();
        }
        if (bombClock.getElapsedTime().asSeconds() >= 4.5f) {
            Entity* P = mgr->getPlayerEntity(); if (!P) return;
            PlayerSoldier* player = P->asPlayer(); if (!player) return;
            float vx = (player->getX() - boss->x_get() > 0) ? 6.f : -6.f;
            mgr->addEntity(new FireBomb(boss->x_get() + boss->width_get() / 2.f, boss->y_get() + (boss->height_get() - 100) / 2, vx, -10.f, mgr, 2.5)); bombClock.restart();
        }
        boss->getAerialSpr().setScale((boss->dirRef() == 1 ? 1 : -1), 1);
    }
    void draw(UltimateBoss* boss, sf::RenderWindow& window, float camX, float camY) override {
        if (boss->dirRef() == -1) boss->getAerialSpr().setPosition(boss->x_get() - camX + boss->width_get(), boss->y_get() - camY);
        else boss->getAerialSpr().setPosition(boss->x_get() - camX, boss->y_get() - camY);
        window.draw(boss->getAerialSpr());
    }
};

class UltAquaticState : public UltimateBossState {
    Clock hmgL, hmgR, rocketClock;
public:
    int getName() const override { return 2; }
    void update(UltimateBoss* boss, EntityManager* mgr) override {
        boss->hoverTimeRef() += 0.025f; boss->setY(boss->baseYRef() + sin(boss->hoverTimeRef()) * 10.0f);
        boss->moveX(boss->getSpeed() * boss->dirRef());
        float lx = 12800.0f + 100.0f, rx = 19200.0f - 500.0f;
        if (boss->x_get() < lx + boss->width_get() && boss->dirRef() == -1) { boss->moveX(lx - boss->x_get()); boss->dirRef() = 1; }
        if (boss->x_get() > rx - boss->width_get() && boss->dirRef() == 1) { boss->moveX(rx - boss->x_get()); boss->dirRef() = -1; }
        if (hmgL.getElapsedTime().asSeconds() >= 0.1f) {
            Bullet* b = new Bullet(boss->x_get() + 100, boss->y_get() + boss->height_get() - 180, -15.0f, 0.f, 3, 2, 180.f, mgr);
            b->setEnemyBullet(true); mgr->addEntity(b); hmgL.restart();
        }
        if (hmgR.getElapsedTime().asSeconds() >= 0.1f) {
            Bullet* b = new Bullet(boss->x_get(), boss->y_get() + boss->height_get() - 180, 15.0f, 0.f, 3, 2, 0.f, mgr);
            b->setEnemyBullet(true); mgr->addEntity(b); hmgR.restart();
        }
        if (rocketClock.getElapsedTime().asSeconds() >= 3.0f) {
            mgr->addEntity(new Enemy_Sub_Missile(boss->x_get() + boss->width_get() / 2.f, boss->y_get(), boss->dirRef(), mgr));
            mgr->addEntity(new Enemy_Sub_Missile(boss->x_get() + boss->width_get() / 2.f, boss->y_get(), -boss->dirRef(), mgr)); rocketClock.restart();
        }
        boss->getAquaticSpr().setScale((boss->dirRef() == 1 ? 2.0f : -2.0f), 1.5f);
    }
    void draw(UltimateBoss* boss, sf::RenderWindow& window, float camX, float camY) override {
        if (boss->dirRef() == -1) boss->getAquaticSpr().setPosition(boss->x_get() - camX + boss->width_get(), boss->y_get() - camY);
        else boss->getAquaticSpr().setPosition(boss->x_get() - camX, boss->y_get() - camY);
        window.draw(boss->getAquaticSpr());
    }
};

void UltimateBoss::setState(UltimateBossState* newState) {
    delete currentState; currentState = newState; stateTime = 10.0f; stateClock.restart();
}

void UltimateBoss::takeDamage(double dmg) {
    if (!currentState) return;
    int state = currentState->getName();
    if (state == 0) damageGroundHP(dmg);
    else if (state == 1) damageAerialHP(dmg);
    else damageAquaticHP(dmg);
    health = groundHP + aerialHP + aquaticHP;
    if (health <= 0) { if (scorePtr) *scorePtr += 10000; kill(); }
}

void UltimateBoss::spawnMinion() {
    if (!currentState) return;
    int state = currentState->getName(); int batch = (rand() % 3) + 2;
    for (int i = 0; i < batch; i++) {
        float mx = x + (rand() % 600) - 300.0f; Enemy* minion = nullptr;
        if (state == 0) {
            int r = rand() % 3;
            if (r == 0) minion = new rebel_soldier(mx, y - 50.f, manager);
            else if (r == 1) minion = new shieldsoldier(mx, y - 50.f, manager);
            else minion = new bazooka_soldier(mx, y - 50.f, manager);
        }
        else if (state == 1) { minion = new paratrooper(mx, -50.f, manager); }
        else { minion = new Zombie(mx, y - 100.f, manager); }
        if (minion) { minion->setScorePtr(scorePtr); manager->addEntity(minion); }
    }
    manager->addEntity(new AmmoCrate(x + width / 2.f, y - 80.f, manager));
}

void UltimateBoss::update(char** lvl, int cell_size) {
    if (isDead) return;
    if (!currentState) setState(new UltGroundState());
    if (stateClock.getElapsedTime().asSeconds() >= stateTime) {
        int cur = currentState->getName(); UltimateBossState* next = nullptr;
        if (cur == 0) { if (aerialHP > 0) next = new UltAerialState(); else if (aquaticHP > 0) next = new UltAquaticState(); else if (groundHP > 0) next = new UltGroundState(); }
        else if (cur == 1) { if (aquaticHP > 0) next = new UltAquaticState(); else if (groundHP > 0) next = new UltGroundState(); else if (aerialHP > 0) next = new UltAerialState(); }
        else { if (groundHP > 0) next = new UltGroundState(); else if (aerialHP > 0) next = new UltAerialState(); else if (aquaticHP > 0) next = new UltAquaticState(); }
        if (next) setState(next); else { if (scorePtr) *scorePtr += 10000; kill(); return; }
    }
    else {
        int cur = currentState->getName();
        if (x < 6400.0f && cur != 0 && groundHP > 0) setState(new UltGroundState());
        else if (x >= 6400.0f && x < 12800.0f && cur != 1 && aerialHP > 0) setState(new UltAerialState());
        else if (x >= 12800.0f && cur != 2 && aquaticHP > 0) setState(new UltAquaticState());
    }
    currentState->update(this, manager);
    if (minionClock.getElapsedTime().asSeconds() >= 12.0f) { spawnMinion(); minionClock.restart(); }
}

// ─────────────────────────────────────────────
// BOSS LEVEL STATE
// ─────────────────────────────────────────────

class BossLevelState : public GameState {
private:
    EntityManager* eManager; LevelManager* lManager; PlayerSoldier* mainPlayer;
    Macro_Rossi* macro; Tarma* tarma; Eri_kasamoto* eri; Fiolina_germi* fiolina; int chosenCharacter;
    char** currentLevelMap; int cellSize, mapHeight, mapWidth;
    int score, carryScore; Font scoreFont; Text scoreText; char scoreChars[7];
    Texture Player_Textures[4]; Sprite Player_Sprites[4]; Texture Hearts_Texture; Sprite Hearts_Sprite;
    int phase; bool phaseTransition; Clock phaseTimer;
    Boss* currentBoss;
    bool fusionExploding; Clock fusionClock; Texture explosionTex; Sprite explosionSpr;
    bool zPressed, ePressed, isInMariner, levelCleared; Clock clearTimer; int currentMode;
    int fusionFrame; Clock fusionAnimClock; int fusionFrameWidth, fusionFrameHeight;
    Music bossMusic; bool fPressed;
public:
    BossLevelState(int chosenChar, int gameMode, int carryScr = 0)
        : chosenCharacter(chosenChar), carryScore(carryScr), currentMode(gameMode) {
        eManager = new EntityManager(); lManager = new LevelManager();
        cellSize = 50; mapHeight = (int)(18 * 1.2f); mapWidth = 128 * 3;
        score = carryScr; phase = 1; phaseTransition = false; currentBoss = nullptr;
        fusionExploding = false; zPressed = ePressed = false; isInMariner = false; levelCleared = false; fPressed = false;
        currentLevelMap = lManager->createBossLevel(mapHeight, mapWidth);
       // bossMusic.openFromFile("Boss.ogg");
       // bossMusic.setLoop(true);
       // bossMusic.play();
        float sx = 100.0f, sy = 100.0f;
        macro = new Macro_Rossi(sx, sy, eManager); tarma = new Tarma(sx, sy, eManager);
        eri = new Eri_kasamoto(sx, sy, eManager); fiolina = new Fiolina_germi(sx, sy, eManager);
        if (chosenCharacter == 1) mainPlayer = macro;
        else if (chosenCharacter == 2) mainPlayer = tarma;
        else if (chosenCharacter == 3) mainPlayer = eri;
        else mainPlayer = fiolina;
        mainPlayer->changeState(new NormalState());
        eManager->addEntity(macro); eManager->addEntity(tarma); eManager->addEntity(eri); eManager->addEntity(fiolina);
        score = carryScr; scoreFont.loadFromFile("Font.ttf");
        scoreText.setFont(scoreFont); scoreText.setCharacterSize(45); scoreText.setFillColor(sf::Color::White); scoreText.setPosition(1200.0f, 10.0f);
        for (int i = 0; i < 4; i++) { Player_Textures[i].loadFromFile("Players.png"); Player_Sprites[i].setTexture(Player_Textures[i]); Player_Sprites[i].setScale(0.5f, 0.5f); }
        Player_Sprites[0].setTextureRect(IntRect(100, 0, 100, 100)); Player_Sprites[1].setTextureRect(IntRect(200, 0, 100, 100));
        Player_Sprites[2].setTextureRect(IntRect(0, 0, 100, 100)); Player_Sprites[3].setTextureRect(IntRect(300, 0, 100, 100));
        Hearts_Texture.loadFromFile("Heart.png"); Hearts_Sprite.setTexture(Hearts_Texture); Hearts_Sprite.setScale(0.5f, 0.5f);
        fusionFrame = 0; fusionFrameWidth = 225; fusionFrameHeight = 225;
        explosionTex.loadFromFile("Boss_Bombs_2.png"); explosionSpr.setTexture(explosionTex); explosionSpr.setScale(3.0f, 3.0f);
        spawnPhase1();
    }
    ~BossLevelState() { delete eManager; delete lManager; 
   // bossMusic.stop();
    for (int i = 0; i < mapHeight; i++)
        delete[] currentLevelMap[i];
    delete[] currentLevelMap; }
    void spawnPhase1() { currentBoss = new IronNokana(4000.0f, 605.0f, eManager); currentBoss->setScorePtr(&score); eManager->addEntity(currentBoss); }
    void spawnPhase2() { currentBoss = new HairbusterRiberts(7000.0f, 5.0f, eManager); currentBoss->setScorePtr(&score); eManager->addEntity(currentBoss); }
    void spawnPhase3() { currentBoss = new SeaSatan(18000.0f, 482.0f, eManager); currentBoss->setScorePtr(&score); eManager->addEntity(currentBoss); eManager->addEntity(new SlugMariner(12900.0f, 600.0f, eManager)); }
    void spawnPhase4() {
        fusionExploding = true; fusionClock.restart();
        mainPlayer->setPlayerX(4000.0f); mainPlayer->setPlayerY(100.0f); mainPlayer->setVelocityX(0); mainPlayer->setVelocityY(0);
        currentBoss = new UltimateBoss(6000.0f, 605.0f, eManager); currentBoss->setScorePtr(&score); eManager->addEntity(currentBoss);
    }
    PlayerSoldier* getNextAlivePlayer() {
        int startChar = chosenCharacter;
        do {
            chosenCharacter++; if (chosenCharacter > 4) chosenCharacter = 1;
            PlayerSoldier* c = nullptr;
            if (chosenCharacter == 1) c = macro; else if (chosenCharacter == 2) c = tarma;
            else if (chosenCharacter == 3) c = eri; else c = fiolina;
            if (eManager->contains(c)) return c;
        } while (chosenCharacter != startChar);
        return nullptr;
    }
    void update(GameStateManager* gsm) override {
        if (mainPlayer == nullptr) return;
        if (phase == 1 && currentBoss != nullptr) {
            if (!eManager->contains(currentBoss) || currentBoss->phaseOver || currentBoss->getIsDead()) {
                phase = 2; currentBoss = nullptr; mainPlayer->setPlayerX(6500.0f); mainPlayer->setPlayerY(100.0f); mainPlayer->setVelocityX(0); mainPlayer->setVelocityY(0); spawnPhase2();
            }
        }
        else if (phase == 2 && currentBoss != nullptr) {
            if (!eManager->contains(currentBoss) || currentBoss->phaseOver || currentBoss->getIsDead()) {
                phase = 3; currentBoss = nullptr; mainPlayer->setPlayerX(12900.0f); mainPlayer->setPlayerY(100.0f); mainPlayer->setVelocityX(0); mainPlayer->setVelocityY(0); spawnPhase3();
            }
        }
        else if (phase == 3 && currentBoss != nullptr) {
            if (!eManager->contains(currentBoss) || currentBoss->phaseOver || currentBoss->getIsDead()) {
                currentBoss = nullptr; phase = 4; mainPlayer->setPlayerX(9700.0f); mainPlayer->setPlayerY(100.0f); mainPlayer->setVelocityX(0); mainPlayer->setVelocityY(0); spawnPhase4();
            }
        }
        else if (phase == 4 && currentBoss != nullptr) {
            if (currentBoss->getIsDead() || !eManager->contains(currentBoss)) {
                currentBoss = nullptr; levelCleared = true; score += 5000; clearTimer.restart();
            }
        }
        if (levelCleared && clearTimer.getElapsedTime().asSeconds() >= 4.0f) { gsm->changeState(new EndScreenState(score)); return; }
        eManager->updateAll(currentLevelMap, cellSize);
        if (mainPlayer == nullptr) {
            PlayerSoldier* next = getNextAlivePlayer();
            if (next) { mainPlayer = next; mainPlayer->changeState(new NormalState()); }
            else { gsm->changeState(new EndScreenState(score)); return; }
            return;
        }
        if (!eManager->contains(mainPlayer)) mainPlayer = nullptr;
        if (mainPlayer == nullptr) {
            PlayerSoldier* next = getNextAlivePlayer();
            if (next) { mainPlayer = next; mainPlayer->changeState(new NormalState()); }
            return;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (!zPressed) {
                zPressed = true; float sx = mainPlayer->getX(), sy = mainPlayer->getY(); mainPlayer->changeState(nullptr);
                PlayerSoldier* next = getNextAlivePlayer();
                if (next) { mainPlayer = next; mainPlayer->changeState(new NormalState()); mainPlayer->setPlayerX(sx); mainPlayer->setPlayerY(sy); mainPlayer->setVelocityY(0); mainPlayer->setOnGround(false); mainPlayer->setCanJump(false); mainPlayer->setWantToFall(false); }
            }
        }
        else { zPressed = false; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (!ePressed) {
                ePressed = true;
                Entity* marinerEnt = eManager->getSlugMarinerEntity();
                Vehicle* mariner = marinerEnt ? marinerEnt->asVehicle() : nullptr;
                if (mainPlayer->GetIsRiding()) { if (mariner) mariner->RIDEVehicle(nullptr); mainPlayer->SetIsRiding(false); isInMariner = false; }
                else if (mariner && mariner->Check_Touch(mariner->getX(), mariner->getY(), mariner->getwidth(), mariner->getheight(), mainPlayer->getX(), mainPlayer->getY(), mainPlayer->getwidth(), mainPlayer->getheight(), 1, mainPlayer->Get_P_D())) {
                    mariner->RIDEVehicle(mainPlayer); mainPlayer->SetIsRiding(true); isInMariner = true;
                }
            }
        }
        else { ePressed = false; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !mainPlayer->GetIsRiding()) {
            if (!fPressed) {
                fPressed = true;
                if (mainPlayer->canSpawnFusion(currentMode)) {
                    PlayerSoldier* alive[4] = { macro, tarma, eri, fiolina }; int count = 0;
                    float totalSpeed = 0, totalDmg = 0, totalRate = 0, totalMelee = 0;
                    for (int i = 0; i < 4; i++) {
                        if (eManager->contains(alive[i])) { totalSpeed += alive[i]->getSpeed(); totalDmg += 3.f; totalRate += 0.25f; totalMelee += alive[i]->getMeleeMultiplier(); count++; }
                    }
                    if (count >= 2) {
                        FusionCompanion* fc = new FusionCompanion(mainPlayer->getX() + 120.f, mainPlayer->getY(), eManager, totalSpeed / count, totalDmg / count, totalRate / count, totalMelee / count);
                        eManager->addEntity(fc); mainPlayer->onFusionSpawned(currentMode);
                    }
                }
            }
        }
        else fPressed = false;
    }
    void draw(sf::RenderWindow& window) override {
        if (!mainPlayer) return;
        float camX = mainPlayer->getX() - 800.0f; float camY = mainPlayer->getY() - 450.0f;
        float maxCamX = (mapWidth * cellSize) - 1600.0f; float maxCamY = (mapHeight * cellSize) - 900.0f;
        if (camX < 0) camX = 0; if (camY < 0) camY = 0; if (camX > maxCamX) camX = maxCamX; if (camY > maxCamY) camY = maxCamY;
        lManager->drawLevel(window, currentLevelMap, mapHeight, mapWidth, cellSize, camY, camX, true);
        eManager->drawAll(window, camX, camY);
        if (fusionExploding) {
            float t = fusionClock.getElapsedTime().asSeconds();
            if (t < 2.5f) {
                if (fusionAnimClock.getElapsedTime().asMilliseconds() > 50) { fusionFrame = (fusionFrame + 1) % 3; fusionAnimClock.restart(); }
                explosionSpr.setTextureRect(IntRect(fusionFrame * fusionFrameWidth, 0, fusionFrameWidth, fusionFrameHeight));
                explosionSpr.setPosition(4750.0f - camX - 300.f, 580.0f - camY - 200.f); window.draw(explosionSpr);
            }
            else { fusionExploding = false; fusionFrame = 0; }
        }
        PlayerSoldier* allPlayers[4] = { macro, tarma, eri, fiolina };
        for (int i = 0; i < 4; i++) {
            float rowX = 10.0f, rowY = 10.0f + i * 50.0f;
            Player_Sprites[i].setPosition(rowX, rowY); window.draw(Player_Sprites[i]);
            if (eManager->contains(allPlayers[i])) {
                for (int j = 0; j < allPlayers[i]->getLives(); j++) { Hearts_Sprite.setPosition(rowX + 70.0f + j * 35.0f, rowY + 10.0f); window.draw(Hearts_Sprite); }
            }
        }
        scoreText.setString("BOSS PHASE " + std::to_string(phase)); scoreText.setPosition(650.0f, 10.0f); window.draw(scoreText);
        intToCharArray(score, scoreChars);
        scoreText.setString("SCORE:"); scoreText.setPosition(1200.0f, 10.0f); window.draw(scoreText);
        scoreText.setString(scoreChars); scoreText.setPosition(1350.0f, 10.0f); window.draw(scoreText);
        if (levelCleared) { scoreText.setString("ALL BOSSES DEFEATED!"); scoreText.setPosition(500.0f, 400.0f); window.draw(scoreText); }
    }
};

// ─────────────────────────────────────────────
// PLAY STATE - with Infinite Mode (Perlin) + Campaign
// ─────────────────────────────────────────────

class PlayState : public GameState {
private:
    EntityManager* eManager; LevelManager* lManager; PlayerSoldier* mainPlayer;
    int chosenCharacter; Macro_Rossi* macro; Tarma* tarma; Eri_kasamoto* eri; Fiolina_germi* fiolina;
    char** currentLevelMap; int cellSize, mapHeight, mapWidth;
    bool zPressed, ePressed, fPressed, isInFlyer, isInMariner, isInAmphibious;
    Texture Player_Textures[4]; Sprite Player_Sprites[4]; Texture Hearts_Texture; Sprite Hearts_Sprite;
    int score; Font scoreFont; Text scoreText; char scoreChars[7];
    bool levelCleared; Clock clearTimer; int currentLevel, currentMode, startingScore;
    Music levelMusic;
    // Perlin / Infinite mode
    Perlin* myPerlin; float blockoffset;
    int howManyKilled; // tracks kills for infinite mode level clear
    Clock enemySpawnClock; // random spawn timer for infinite mode
    float nextSpawnTime; // random interval 0-10s
    Text levelClearText;
public:
    PlayState(int chosenChar, int gameMode, int gameLevel, int carryScore = 0) {
        eManager = new EntityManager(); lManager = new LevelManager();
        cellSize = 50; mapHeight = (int)(18 * 1.2f); mapWidth = 128 * 3;
        zPressed = false; chosenCharacter = chosenChar; ePressed = false; fPressed = false;
        isInFlyer = false; isInMariner = false; isInAmphibious = false;
        currentLevel = gameLevel; currentMode = gameMode;
        myPerlin = nullptr; blockoffset = 0.0f; howManyKilled = 0;
        nextSpawnTime = (float)(rand() % 11); // 0-10 seconds

        // Setup Perlin for infinite mode (mode 1 = infinite, mode 2 = campaign/gauntlet)
        if (currentMode == 1) {
            PLAYERPROFILE pf; myPerlin = pf.getMYMAP(gameLevel); // gameLevel used as profile choice
            currentLevelMap = lManager->createTERRAIN(mapHeight, mapWidth, blockoffset, myPerlin);
        }
        else {
            if (gameLevel == 1) currentLevelMap = lManager->createLevel1(mapHeight, mapWidth);
            else if (gameLevel == 2) currentLevelMap = lManager->createLevel2(mapHeight, mapWidth);
            else currentLevelMap = lManager->createLevel3(mapHeight, mapWidth);
        }

       /// levelMusic.openFromFile("level.ogg");
       // levelMusic.setLoop(true); 
       // levelMusic.play();
        float x = 100.0f, y = 100.0f;
        macro = new Macro_Rossi(x, y, eManager); tarma = new Tarma(x, y, eManager);
        eri = new Eri_kasamoto(x, y, eManager); fiolina = new Fiolina_germi(x, y, eManager);

        if (chosenCharacter == 1) mainPlayer = macro;
        else if (chosenCharacter == 2) mainPlayer = tarma;
        else if (chosenCharacter == 3) mainPlayer = eri;
        else mainPlayer = fiolina;
        mainPlayer->changeState(new NormalState());
        eManager->addEntity(macro); eManager->addEntity(tarma); eManager->addEntity(eri); eManager->addEntity(fiolina);

        // Set perlin on players for infinite mode
        if (currentMode == 1 && myPerlin != nullptr) {
            macro->setPerlinMode(myPerlin, 1); tarma->setPerlinMode(myPerlin, 1);
            eri->setPerlinMode(myPerlin, 1); fiolina->setPerlinMode(myPerlin, 1);
        }

        score = carryScore; startingScore = carryScore;
        scoreFont.loadFromFile("Font.ttf"); scoreText.setFont(scoreFont);
        scoreText.setCharacterSize(45); scoreText.setFillColor(Color::White); scoreText.setPosition(1200.0f, 10.0f);
        levelClearText.setFont(scoreFont); levelClearText.setCharacterSize(60); levelClearText.setFillColor(Color::Yellow);
        levelClearText.setString("LEVEL CLEARED!"); levelClearText.setPosition(500.0f, 400.0f);

        for (int i = 0; i < 4; i++) { Player_Textures[i].loadFromFile("Players.png"); Player_Sprites[i].setTexture(Player_Textures[i]); Player_Sprites[i].setScale(0.5f, 0.5f); }
        Player_Sprites[0].setTextureRect(IntRect(1 * 100, 0, 100, 100)); Player_Sprites[1].setTextureRect(IntRect(2 * 100, 0, 100, 100));
        Player_Sprites[2].setTextureRect(IntRect(0 * 100, 0, 100, 100)); Player_Sprites[3].setTextureRect(IntRect(3 * 100, 0, 100, 100));
        Hearts_Texture.loadFromFile("Heart.png"); Hearts_Sprite.setTexture(Hearts_Texture); Hearts_Sprite.setScale(0.5f, 0.5f);
        levelCleared = false; srand((unsigned int)time(NULL));

        // Campaign mode enemy spawning (mode 2)
        if (currentMode == 2) {
            if (gameLevel == 1) {
                for (int biome = 0; biome < 3; biome++) {
                    for (int batch = 0; batch < 2; batch++) {
                        float rx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); rebel_soldier* rs = new rebel_soldier(rx, 100.0f, eManager); rs->setScorePtr(&score); eManager->addEntity(rs);
                        float sx2 = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); shieldsoldier* ss = new shieldsoldier(sx2, 100.0f, eManager); ss->setScorePtr(&score); eManager->addEntity(ss);
                        float bx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); bazooka_soldier* bs = new bazooka_soldier(bx, 100.0f, eManager); bs->setScorePtr(&score); eManager->addEntity(bs);
                        float gx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); grenade_soldier* gs = new grenade_soldier(gx, 100.0f, eManager); gs->setScorePtr(&score); eManager->addEntity(gs);
                    }
                }
                paratrooper* pt = new paratrooper(500.0f + (float)(rand() % 5400), -50.0f, eManager); pt->setScorePtr(&score); eManager->addEntity(pt);
                Martian* mart = new Martian(500.0f + (float)(rand() % 5400), 50.0f, eManager); mart->setScorePtr(&score); eManager->addEntity(mart);
                for (int b = 0; b < 2; b++) {
                    Mummy* mm = new Mummy(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); mm->setScorePtr(&score); eManager->addEntity(mm);
                    Zombie* zb = new Zombie(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); zb->setScorePtr(&score); eManager->addEntity(zb);
                    Zombie* azb = new Zombie(12800.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); azb->setScorePtr(&score); eManager->addEntity(azb);
                }
                FlyingTara* ft1 = new FlyingTara(400.0f, 80.0f, eManager, 1); ft1->setScorePtr(&score); eManager->addEntity(ft1);
                FlyingTara* ft2 = new FlyingTara(2000.0f, 80.0f, eManager, 1); ft2->setScorePtr(&score); eManager->addEntity(ft2);
                Enemy_Sub* es = new Enemy_Sub(400.0f, 582.0f, eManager, 3); es->setScorePtr(&score); eManager->addEntity(es);
                POW_Prisoner* pow1 = new POW_Prisoner(5000.0f, 922.0f, eManager); pow1->setScorePtr(&score); eManager->addEntity(pow1);
                POW_Prisoner* pow2 = new POW_Prisoner(9000.0f, 922.0f, eManager); pow2->setScorePtr(&score); eManager->addEntity(pow2);
                POW_Prisoner* pow3 = new POW_Prisoner(15000.0f, 922.0f, eManager); pow3->setScorePtr(&score); eManager->addEntity(pow3);
                eManager->addEntity(new MetalSlug(128 * 50 + 15.0f, 815.0f, eManager));
            }
            else if (gameLevel == 2) {
                for (int biome = 0; biome < 3; biome++) for (int b = 0; b < 3; b++) {
                    float rx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); rebel_soldier* rs = new rebel_soldier(rx, 100.0f, eManager); rs->setScorePtr(&score); eManager->addEntity(rs);
                    float sx2 = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); shieldsoldier* ss = new shieldsoldier(sx2, 100.0f, eManager); ss->setScorePtr(&score); eManager->addEntity(ss);
                    float bx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); bazooka_soldier* bs = new bazooka_soldier(bx, 100.0f, eManager); bs->setScorePtr(&score); eManager->addEntity(bs);
                    float gx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); grenade_soldier* gs = new grenade_soldier(gx, 100.0f, eManager); gs->setScorePtr(&score); eManager->addEntity(gs);
                }
                for (int b = 0;b < 2;b++) { paratrooper* pt = new paratrooper(500.0f + (float)(rand() % 5400), -50.0f, eManager); pt->setScorePtr(&score); eManager->addEntity(pt); Martian* m = new Martian(500.0f + (float)(rand() % 5400), 50.0f, eManager); m->setScorePtr(&score); eManager->addEntity(m); }
                for (int b = 0;b < 3;b++) { Mummy* mm = new Mummy(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); mm->setScorePtr(&score); eManager->addEntity(mm); Zombie* zb = new Zombie(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); zb->setScorePtr(&score); eManager->addEntity(zb); Zombie* azb = new Zombie(12800.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); azb->setScorePtr(&score); eManager->addEntity(azb); }
                FlyingTara* ft21 = new FlyingTara(400.0f, 80.0f, eManager, 1); ft21->setScorePtr(&score); eManager->addEntity(ft21);
                FlyingTara* ft22 = new FlyingTara(2000.0f, 80.0f, eManager, 1); ft22->setScorePtr(&score); eManager->addEntity(ft22);
                FlyingTara* ft23 = new FlyingTara(1000.0f, 80.0f, eManager, 2); ft23->setScorePtr(&score); eManager->addEntity(ft23);
                Bradley* bl1 = new Bradley(800.0f, 822.0f, eManager, 2); bl1->setScorePtr(&score); eManager->addEntity(bl1);
                Bradley* bl2 = new Bradley(2000.0f, 822.0f, eManager, 2); bl2->setScorePtr(&score); eManager->addEntity(bl2);
                Enemy_Sub* es2 = new Enemy_Sub(400.0f, 582.0f, eManager, 3); es2->setScorePtr(&score); eManager->addEntity(es2);
                POW_Prisoner* pow1 = new POW_Prisoner(5000.0f, 922.0f, eManager); pow1->setScorePtr(&score); eManager->addEntity(pow1);
                POW_Prisoner* pow2 = new POW_Prisoner(9000.0f, 922.0f, eManager); pow2->setScorePtr(&score); eManager->addEntity(pow2);
                eManager->addEntity(new SlugFlyer(900.0f, 400.0f, eManager));
            }
            else {
                for (int biome = 0;biome < 3;biome++) for (int b = 0;b < 3;b++) {
                    float rx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); rebel_soldier* rs = new rebel_soldier(rx, 100.0f, eManager); rs->setScorePtr(&score); eManager->addEntity(rs);
                    float sx2 = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); shieldsoldier* ss = new shieldsoldier(sx2, 100.0f, eManager); ss->setScorePtr(&score); eManager->addEntity(ss);
                    float bx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); bazooka_soldier* bs = new bazooka_soldier(bx, 100.0f, eManager); bs->setScorePtr(&score); eManager->addEntity(bs);
                    float gx = (biome * 6400.0f) + 500.0f + (float)(rand() % 5400); grenade_soldier* gs = new grenade_soldier(gx, 100.0f, eManager); gs->setScorePtr(&score); eManager->addEntity(gs);
                }
                for (int b = 0;b < 3;b++) { paratrooper* pt = new paratrooper(500.0f + (float)(rand() % 5400), -50.0f, eManager); pt->setScorePtr(&score); eManager->addEntity(pt); Martian* m = new Martian(500.0f + (float)(rand() % 5400), 50.0f, eManager); m->setScorePtr(&score); eManager->addEntity(m); }
                for (int b = 0;b < 4;b++) { Mummy* mm = new Mummy(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); mm->setScorePtr(&score); eManager->addEntity(mm); Zombie* zb = new Zombie(6400.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); zb->setScorePtr(&score); eManager->addEntity(zb); Zombie* azb = new Zombie(12800.0f + 500.0f + (float)(rand() % 5400), 100.0f, eManager); azb->setScorePtr(&score); eManager->addEntity(azb); }
                FlyingTara* ft31 = new FlyingTara(400.0f, 80.0f, eManager, 1); ft31->setScorePtr(&score); eManager->addEntity(ft31);
                FlyingTara* ft32 = new FlyingTara(2000.0f, 80.0f, eManager, 1); ft32->setScorePtr(&score); eManager->addEntity(ft32);
                FlyingTara* ft33 = new FlyingTara(1000.0f, 80.0f, eManager, 2); ft33->setScorePtr(&score); eManager->addEntity(ft33);
                Bradley* bl31 = new Bradley(800.0f, 822.0f, eManager, 2); bl31->setScorePtr(&score); eManager->addEntity(bl31);
                Bradley* bl32 = new Bradley(2000.0f, 822.0f, eManager, 2); bl32->setScorePtr(&score); eManager->addEntity(bl32);
                Enemy_Sub* es31 = new Enemy_Sub(400.0f, 582.0f, eManager, 3); es31->setScorePtr(&score); eManager->addEntity(es31);
                Enemy_Sub* es32 = new Enemy_Sub(2000.0f, 582.0f, eManager, 3); es32->setScorePtr(&score); eManager->addEntity(es32);
                eManager->addEntity(new SlugMariner(13000.0f, 600.0f, eManager));
            }
        }
    }
    ~PlayState() { delete eManager; delete lManager; levelMusic.stop(); if (myPerlin) delete myPerlin; for (int i = 0; i < mapHeight; i++) delete[] currentLevelMap[i]; delete[] currentLevelMap; }

    PlayerSoldier* getNextAlivePlayer() {
        int startChar = chosenCharacter;
        do {
            chosenCharacter++; if (chosenCharacter > 4) chosenCharacter = 1;
            PlayerSoldier* candidate = nullptr;
            if (chosenCharacter == 1) candidate = macro; else if (chosenCharacter == 2) candidate = tarma;
            else if (chosenCharacter == 3) candidate = eri; else candidate = fiolina;
            if (eManager->contains(candidate)) return candidate;
        } while (chosenCharacter != startChar);
        return nullptr;
    }

    void update(GameStateManager* gsm) override {
        // Infinite mode: terrain scrolling
        if (currentMode == 1 && mainPlayer != nullptr && myPerlin != nullptr) {
            if (mainPlayer->getX() > 9600.0f) {
                int blocksShifted = 160; float shiftAmount = -(float)(blocksShifted * cellSize);
                blockoffset += (float)blocksShifted;
                for (int i = 0; i < mapHeight; i++) delete[] currentLevelMap[i];
                delete[] currentLevelMap;
                currentLevelMap = lManager->createTERRAIN(mapHeight, mapWidth, blockoffset, myPerlin);
                for (int i = 0; i < eManager->Get_Current_Count(); i++) {
                    Entity* e = eManager->getEntity(i);
                    if (e != nullptr && !e->getIsDead()) { e->x += shiftAmount; }
                }
            }
            // Dynamic enemy spawning every 0-10 seconds random interval
            if (enemySpawnClock.getElapsedTime().asSeconds() >= nextSpawnTime) {
                float px = mainPlayer->getX();
                float spawnOffset = (rand() % 2 == 0) ? 800.0f : -800.0f;
                float spawnX = px + spawnOffset + (float)(rand() % 400 - 200);
                int enemyType = rand() % 8; Enemy* newEnemy = nullptr;
                switch (enemyType) {
                case 0: newEnemy = new rebel_soldier(spawnX, 100.0f, eManager); break;
                case 1: newEnemy = new shieldsoldier(spawnX, 100.0f, eManager); break;
                case 2: newEnemy = new bazooka_soldier(spawnX, 100.0f, eManager); break;
                case 3: newEnemy = new grenade_soldier(spawnX, 100.0f, eManager); break;
                case 4: newEnemy = new paratrooper(spawnX, -50.0f, eManager); break;
                case 5: newEnemy = new Mummy(spawnX, 100.0f, eManager); break;
                case 6: newEnemy = new Zombie(spawnX, 100.0f, eManager); break;
                case 7: newEnemy = new Martian(spawnX, 50.0f, eManager); break;
                }
                if (newEnemy) { newEnemy->setScorePtr(&score); eManager->addEntity(newEnemy); }
                enemySpawnClock.restart(); nextSpawnTime = (float)(rand() % 11); // next 0-10s
            }
            // Check level clear: 5 of each of 8 types = 40 kills minimum tracked via score kills
            if (!levelCleared && howManyKilled >= 40) { levelCleared = true; score += 3000; clearTimer.restart(); }
        }

        // Vehicle controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (!ePressed) {
                ePressed = true;
                Entity* tankEnt = eManager->getMetalSlugEntity();
                Entity* flyerEnt = eManager->getSlugFlyerEntity();
                Entity* marinerEnt = eManager->getSlugMarinerEntity();
                Entity* amphibEnt = eManager->getAmphibiousSlugEntity();
                Vehicle* tank = tankEnt ? tankEnt->asVehicle() : nullptr;
                Vehicle* flyer = flyerEnt ? flyerEnt->asVehicle() : nullptr;
                Vehicle* mariner = marinerEnt ? marinerEnt->asVehicle() : nullptr;
                Vehicle* amphibious = amphibEnt ? amphibEnt->asVehicle() : nullptr;
                if (mainPlayer->GetIsRiding()) {
                    if (isInFlyer && flyer) flyer->RIDEVehicle(nullptr);
                    else if (isInMariner && mariner) mariner->RIDEVehicle(nullptr);
                    else if (isInAmphibious && amphibious) amphibious->RIDEVehicle(nullptr);
                    else if (tank) tank->RIDEVehicle(nullptr);
                    mainPlayer->SetIsRiding(false); isInFlyer = isInMariner = isInAmphibious = false;
                }
                else {
                    if (tank && tank->Check_Touch(tank->getX(), tank->getY(), tank->getwidth(), tank->getheight(), mainPlayer->getX(), mainPlayer->getY(), mainPlayer->getwidth(), mainPlayer->getheight(), 1, mainPlayer->Get_P_D())) { tank->RIDEVehicle(mainPlayer); mainPlayer->SetIsRiding(true); }
                    else if (flyer && flyer->Check_Touch(flyer->getX(), flyer->getY(), flyer->getwidth(), flyer->getheight(), mainPlayer->getX(), mainPlayer->getY(), mainPlayer->getwidth(), mainPlayer->getheight(), 1, mainPlayer->Get_P_D())) { flyer->RIDEVehicle(mainPlayer); mainPlayer->SetIsRiding(true); isInFlyer = true; }
                    else if (mariner && mariner->Check_Touch(mariner->getX(), mariner->getY(), mariner->getwidth(), mariner->getheight(), mainPlayer->getX(), mainPlayer->getY(), mainPlayer->getwidth(), mainPlayer->getheight(), 1, mainPlayer->Get_P_D())) { mariner->RIDEVehicle(mainPlayer); mainPlayer->SetIsRiding(true); isInMariner = true; }
                    else if (amphibious && amphibious->Check_Touch(amphibious->getX(), amphibious->getY(), amphibious->getwidth(), amphibious->getheight(), mainPlayer->getX(), mainPlayer->getY(), mainPlayer->getwidth(), mainPlayer->getheight(), 1, mainPlayer->Get_P_D())) { amphibious->RIDEVehicle(mainPlayer); mainPlayer->SetIsRiding(true); isInAmphibious = true; }
                }
            }
        }
        else { ePressed = false; }

        // Campaign mode level clear
        if (currentMode == 2 && !levelCleared) {
            bool anyEnemyAlive = false;
            for (int i = 0; i < eManager->Get_Current_Count(); i++) {
                Entity* e = eManager->getEntity(i);
                if (e->isEnemy() && !e->getIsDead()) { anyEnemyAlive = true; break; }
            }
            if (!anyEnemyAlive) { levelCleared = true; score += 3000; clearTimer.restart(); }
        }
        if (levelCleared && clearTimer.getElapsedTime().asSeconds() >= 3.0f) {
            if (currentMode == 1) { gsm->changeState(new EndScreenState(score)); return; } // infinite: just end
            int nextLevel = currentLevel + 1;
            if (nextLevel == 4) gsm->changeState(new BossLevelState(chosenCharacter, currentMode, score));
            else if (nextLevel > 4) gsm->changeState(new LevelSelectState(chosenCharacter, currentMode));
            else gsm->changeState(new PlayState(chosenCharacter, currentMode, nextLevel, score));
            return;
        }

        eManager->updateAll(currentLevelMap, cellSize);
        if (!eManager->contains(mainPlayer)) mainPlayer = nullptr;
        if (mainPlayer == nullptr) {
            PlayerSoldier* nextPlayer = getNextAlivePlayer();
            if (nextPlayer != nullptr) { mainPlayer = nextPlayer; mainPlayer->changeState(new NormalState()); }
            else { gsm->changeState(new EndScreenState(score)); return; }
        }

        // Z: character swap
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (!zPressed && mainPlayer) {
                zPressed = true; float sx = mainPlayer->getX(), sy = mainPlayer->getY();
                mainPlayer->changeState(nullptr); PlayerSoldier* next = getNextAlivePlayer();
                if (next) { mainPlayer = next; mainPlayer->changeState(new NormalState()); mainPlayer->setPlayerX(sx); mainPlayer->setPlayerY(sy); mainPlayer->setVelocityY(0); mainPlayer->setOnGround(false); mainPlayer->setCanJump(false); mainPlayer->setWantToFall(false); }
            }
        }
        else { zPressed = false; }

        // F: Fusion
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && mainPlayer && !mainPlayer->GetIsRiding()) {
            if (!fPressed) {
                fPressed = true;
                if (mainPlayer->canSpawnFusion(currentMode)) {
                    PlayerSoldier* alive[4] = { macro, tarma, eri, fiolina }; int count = 0;
                    float totalSpeed = 0, totalDmg = 0, totalRate = 0, totalMelee = 0;
                    for (int i = 0;i < 4;i++) { if (eManager->contains(alive[i])) { totalSpeed += alive[i]->getSpeed(); totalDmg += 3.f; totalRate += 0.25f; totalMelee += alive[i]->getMeleeMultiplier(); count++; } }
                    if (count >= 2) {
                        FusionCompanion* fc = new FusionCompanion(mainPlayer->getX() + 120.f, mainPlayer->getY(), eManager, totalSpeed / count, totalDmg / count, totalRate / count, totalMelee / count);
                        eManager->addEntity(fc); mainPlayer->onFusionSpawned(currentMode);
                    }
                }
            }
        }
        else fPressed = false;

        // Track kills for infinite mode
        if (currentMode == 1) {
            int deadEnemies = 0;
            for (int i = 0; i < eManager->Get_Current_Count(); i++) { /* tracked via score increments */ }
            // We use score-based kill tracking: each kill adds to score via scorePtr
            // howManyKilled is incremented in enemy takeDamage - set scorePtr to track
            // Simple approach: count by score difference / avg kill value
            howManyKilled = (score - startingScore) / 50; // approximate
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (mainPlayer == nullptr) return;
        float camX = mainPlayer->getX() - 800.0f; float camY = mainPlayer->getY() - 450.0f;
        if (currentMode == 1) {
            if (camX < 0) camX = 0;
            lManager->drawInfiniteLevel(window, myPerlin, camX, camY, 1600, 900, cellSize);
        }
        else {
            float maxCamX = (mapWidth * cellSize) - 1600.0f; float maxCamY = (mapHeight * cellSize) - 900.0f;
            if (camX < 0) camX = 0; if (camY < 0) camY = 0; if (camX > maxCamX) camX = maxCamX; if (camY > maxCamY) camY = maxCamY;
            lManager->drawLevel(window, currentLevelMap, mapHeight, mapWidth, cellSize, camY, camX);
        }
        eManager->drawAll(window, camX, camY);
        PlayerSoldier* allPlayers[4] = { macro, tarma, eri, fiolina };
        for (int i = 0; i < 4; i++) {
            float rowX = 10.0f, rowY = 10.0f + i * 50.0f;
            Player_Sprites[i].setPosition(rowX, rowY); window.draw(Player_Sprites[i]);
            if (eManager->contains(allPlayers[i])) {
                for (int j = 0; j < allPlayers[i]->getLives(); j++) { Hearts_Sprite.setPosition(rowX + 70.0f + j * 35.0f, rowY + 10.0f); window.draw(Hearts_Sprite); }
            }
        }
        intToCharArray(score, scoreChars);
        scoreText.setString("SCORE:"); scoreText.setPosition(1200.0f, 10.0f); window.draw(scoreText);
        scoreText.setString(scoreChars); scoreText.setPosition(1350.0f, 10.0f); window.draw(scoreText);
        if (currentMode == 1) {
            // Show kill progress
            scoreText.setString("KILLS: " + to_string(howManyKilled) + "/40"); scoreText.setPosition(700.0f, 10.0f); window.draw(scoreText);
        }
        if (levelCleared) { window.draw(levelClearText); }
    }
};

// ─────────────────────────────────────────────
// PROFILE SELECT STATE (Infinite Mode profile picker)
// ─────────────────────────────────────────────

class ProfileSelectState : public GameState {
private:
    sf::Texture bgTexture; sf::Sprite bgSprite;
    sf::Texture arrowTexture; sf::Sprite arrowSprite;
    int chosenChar, selectedPosition; float arrowX, arrowY; sf::Clock inputDelay;
public:
    ProfileSelectState(int character) {
        chosenChar = character; selectedPosition = 2; arrowX = 360.0f; arrowY = 450.0f;
        bgTexture.loadFromFile("INFINITE_VOID_MENU_SELECTION.png"); bgSprite.setTexture(bgTexture); bgSprite.setPosition(0, 0); bgSprite.setScale(0.6f, 0.6f);
        arrowTexture.loadFromFile("R_Arrow2.png"); arrowSprite.setTexture(arrowTexture); arrowSprite.setScale(0.8f, 0.8f); arrowSprite.setTextureRect(sf::IntRect(0, 0, 219, 230));
    }
    void update(GameStateManager* gsm) override {
        if (inputDelay.getElapsedTime().asSeconds() > 0.2f) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (selectedPosition == 1) { arrowY = 600.0f; selectedPosition = 3; }
                else if (selectedPosition == 2) { arrowY = 300.0f; selectedPosition = 1; }
                else { arrowY = 450.0f; selectedPosition = 2; }
                inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (selectedPosition == 1) { arrowY = 450.0f; selectedPosition = 2; }
                else if (selectedPosition == 2) { arrowY = 600.0f; selectedPosition = 3; }
                else { arrowY = 300.0f; selectedPosition = 1; }
                inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                int factoryChoice = 1;
                if (selectedPosition == 1) factoryChoice = 2;
                else if (selectedPosition == 2) factoryChoice = 1;
                else factoryChoice = 3;
                gsm->changeState(new PlayState(chosenChar, 1, factoryChoice)); inputDelay.restart();
            }
        }
        arrowSprite.setPosition(arrowX, arrowY);
    }
    void draw(sf::RenderWindow& window) override { window.draw(bgSprite); window.draw(arrowSprite); }
};

// ─────────────────────────────────────────────
// MODE & PLAYER SELECT STATES
// ─────────────────────────────────────────────

class ModeSelectState : public GameState {
private:
    sf::Texture bgTexture; sf::Sprite bgSprite;
    sf::Texture arrowTexture; sf::Sprite arrowSprite;
    int chosenChar, selectedMode; float arrowX, arrowY; sf::Clock inputDelay;
public:
    ModeSelectState(int character) {
        chosenChar = character; selectedMode = 1; arrowX = 315.0f; arrowY = 360.0f;
        bgTexture.loadFromFile("Game_Mode.png"); bgSprite.setTexture(bgTexture); bgSprite.setPosition(0, 0);
        arrowTexture.loadFromFile("R_Arrow2.png"); arrowSprite.setTexture(arrowTexture); arrowSprite.setScale(0.8f, 0.8f); arrowSprite.setTextureRect(sf::IntRect(0, 0, 219, 230));
    }
    void update(GameStateManager* gsm) override {
        if (inputDelay.getElapsedTime().asSeconds() > 0.2f) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (selectedMode == 1) { arrowY = 560; selectedMode = 2; }
                else { arrowY = 360; selectedMode = 1; }
                inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (selectedMode == 1) gsm->changeState(new ProfileSelectState(chosenChar)); // infinite -> profile pick
                else gsm->changeState(new LevelSelectState(chosenChar, selectedMode)); // campaign -> level select
                inputDelay.restart();
            }
        }
        arrowSprite.setPosition(arrowX, arrowY);
    }
    void draw(sf::RenderWindow& window) override { window.draw(bgSprite); window.draw(arrowSprite); }
};

class PlayerSelectState : public GameState {
private:
    sf::Texture C1, C2, C3, C4; sf::Sprite C_1, C_2, C_3, C_4;
    sf::Texture bgTexture; sf::Sprite bgSprite;
    sf::Texture arrowTexture; sf::Sprite arrowSprite;
    int chosenChar; float arrowX, arrowY; sf::Clock inputDelay; bool enterPressed;
public:
    PlayerSelectState() {
        chosenChar = 1; arrowX = 150.0f; arrowY = 630.0f;
        C1.loadFromFile("Players.png"); C_1.setTexture(C1); C_1.setScale(2.5f, 2.5f); C_1.setTextureRect(sf::IntRect(100, 0, 100, 100)); C_1.setPosition(150, 355);
        C2.loadFromFile("Players.png"); C_2.setTexture(C2); C_2.setScale(2.5f, 2.5f); C_2.setTextureRect(sf::IntRect(200, 0, 100, 100)); C_2.setPosition(500, 355);
        C3.loadFromFile("Players.png"); C_3.setTexture(C3); C_3.setScale(2.5f, 2.5f); C_3.setTextureRect(sf::IntRect(0, 0, 100, 100)); C_3.setPosition(850, 355);
        C4.loadFromFile("Players.png"); C_4.setTexture(C4); C_4.setScale(2.45f, 2.45f); C_4.setTextureRect(sf::IntRect(300, 0, 100, 100)); C_4.setPosition(1225, 370);
        bgTexture.loadFromFile("Player_Selection.png"); bgSprite.setTexture(bgTexture); bgSprite.setPosition(0, 0);
        arrowTexture.loadFromFile("Arrow2.png"); arrowSprite.setTexture(arrowTexture); arrowSprite.setScale(0.8f, 0.8f); arrowSprite.setTextureRect(sf::IntRect(0, 0, 219, 230));
        enterPressed = false;
    }
    void update(GameStateManager* gsm) override {
        if (inputDelay.getElapsedTime().asSeconds() > 0.2f) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (chosenChar == 1) { arrowX = 1250;chosenChar = 4; }
                else if (chosenChar == 2) { arrowX = 150;chosenChar = 1; }
                else if (chosenChar == 3) { arrowX = 525;chosenChar = 2; }
                else { arrowX = 850;chosenChar = 3; } inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (chosenChar == 3) { arrowX = 1250;chosenChar = 4; }
                else if (chosenChar == 4) { arrowX = 150;chosenChar = 1; }
                else if (chosenChar == 1) { arrowX = 525;chosenChar = 2; }
                else { arrowX = 850;chosenChar = 3; } inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (!enterPressed) { enterPressed = true; gsm->changeState(new ModeSelectState(chosenChar)); inputDelay.restart(); }
            }
            else { enterPressed = false; }
        }
        arrowSprite.setPosition(arrowX, arrowY);
    }
    void draw(sf::RenderWindow& window) override {
        window.draw(bgSprite); window.draw(arrowSprite);
        window.draw(C_1); window.draw(C_2); window.draw(C_3); window.draw(C_4);
    }
};

inline GameStateManager::GameStateManager() { activeState = new PlayerSelectState(); }

// ─────────────────────────────────────────────
// GAME CLASS
// ─────────────────────────────────────────────

class Game {
private:
    sf::RenderWindow window; GameStateManager* gsm;
public:
    Game() : window(sf::VideoMode(1600, 900), "OOP 26 Metal Slug") {
        window.setFramerateLimit(60); gsm = new GameStateManager();
    }
    ~Game() { delete gsm; }
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
            }
            gsm->update(); window.clear(); gsm->draw(window); window.display();
        }
    }
};

// ─────────────────────────────────────────────
// FUNCTION DEFINITIONS
// ─────────────────────────────────────────────

void ProjectileWeapon::fire(float originX, float originY, float aimAngleDegrees, EntityManager* manager, bool specialActive) {
    if (mytimer.getElapsedTime().asSeconds() >= fireRate) {
        float pi = 3.14159265f; float radians = aimAngleDegrees * (pi / 180.0f);
        float calcVx = bulletSpeed * cos(radians); float calcVy = bulletSpeed * sin(radians);
        Bullet* newBullet = new Bullet(originX, originY, calcVx, calcVy, damage, bulletType, aimAngleDegrees, manager);
        manager->addEntity(newBullet);
        if (bulletType == 4) newBullet->setIsFlameShot(true);
        if (specialActive) {
            float backRadians = (aimAngleDegrees + 180.0f) * (pi / 180.0f);
            float backVx = bulletSpeed * cos(backRadians); float backVy = bulletSpeed * sin(backRadians);
            Bullet* backBullet = new Bullet(originX, originY + 15, backVx, backVy, damage, bulletType, aimAngleDegrees + 180.0f, manager);
            manager->addEntity(backBullet);
        }
        mytimer.restart();
    }
}

void Bullet::update(char** lvl, int cell_size) {
    if (bulletType == 3) if (velocityY < 1) velocityY += 0.2f;
    x += velocityX; y += velocityY;
    int Map_Height = (int)(18 * 1.2f); int Map_Width = 128 * 3;
    if (x < 0 || x > Map_Width * 50 || y < 0 || y > Map_Height * 50) { kill(); return; }
    float dx = x - spawnX; float dy = y - spawnY;
    if (dx * dx + dy * dy > maxRange * maxRange) { kill(); return; }
    // Perlin collision for infinite mode
    if (myMode == 1 && myPerlin != nullptr) {
        float groundY = (10.0f - (myPerlin->getheight(x / (float)cell_size) * 5.0f)) * cell_size;
        if (y > groundY) { kill(); return; }
    }
    else {
        int col = (int)(x / cell_size); int row = (int)(y / cell_size);
        if (row >= 0 && row < Map_Height && col >= 0 && col < Map_Width) {
            if (lvl[row][col] == '#' || lvl[row][col] == 'P' || lvl[row][col] == 'S') { kill(); return; }
        }
    }
    if (isEnemyBullet) {
        Entity* pEntity = manager->getPlayerEntity();
        if (pEntity == nullptr) { velocityX = 0; return; }
        PlayerSoldier* p = pEntity->asPlayer();
        if (p != nullptr && !p->getIsDead()) {
            if (Check_Touch(p->getX(), p->getY(), (int)p->getwidth(), (int)p->getheight(), x, y, width, height, 1, 1)) { p->takeDamage(damage); kill(); return; }
        }
        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i); if (e->getIsDead() || !e->isPlayerVehicle()) continue;
            Vehicle* v = e->asVehicle(); if (v != nullptr && !v->hasRider()) continue;
            if (Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1)) { e->takeDamage(damage); kill(); return; }
        }
    }
    else {
        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i); if (e->getIsDead() || !e->isEnemy()) continue;
            bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1);
            if (hit) {
                if (bulletType == 3) {
                    float blastRadius = 150.0f;
                    for (int j = 0; j < manager->Get_Current_Count(); j++) {
                        Entity* nearby = manager->getEntity(j); if (nearby->getIsDead() || !nearby->isEnemy()) continue;
                        float ddx = nearby->getX() - x; float ddy = nearby->getY() - y;
                        if (sqrt(ddx * ddx + ddy * ddy) <= blastRadius) nearby->takeDamage(5);
                    }
                }
                else {
                    e->takeDamage(damage);
                    if (e->getIsDead() && shooterWasAirborne) {
                        Entity* p = manager->getPlayerEntity(); if (!p) { velocityX = 0; return; }
                        PlayerSoldier* ps = p->asPlayer(); if (ps && ps->getScorePtr()) *ps->getScorePtr() += 100;
                    }
                }
                kill(); return;
            }
        }
    }
}

Entity* EntityManager::getPlayerEntity() {
    for (int i = 0; i < currentCount; i++) { PlayerSoldier* p = entityArray[i]->asPlayer(); if (p != nullptr && p->isActive()) return p; }
    return nullptr;
}
Entity* EntityManager::getMetalSlugEntity() { for (int i = 0; i < currentCount; i++) if (entityArray[i]->isMetalSlug()) return entityArray[i]; return nullptr; }
Entity* EntityManager::getVehicleEntity() { for (int i = 0; i < currentCount; i++) { Vehicle* v = entityArray[i]->asVehicle(); if (v) return v; } return nullptr; }
Entity* EntityManager::getSlugFlyerEntity() { for (int i = 0; i < currentCount; i++) if (entityArray[i]->isSlugFlyer()) return entityArray[i]; return nullptr; }
Entity* EntityManager::getSlugMarinerEntity() { for (int i = 0; i < currentCount; i++) if (entityArray[i]->isSlugMariner()) return entityArray[i]; return nullptr; }
Entity* EntityManager::getAmphibiousSlugEntity() { for (int i = 0; i < currentCount; i++) if (entityArray[i]->isAmphibious()) return entityArray[i]; return nullptr; }

void Vehicle::RIDEVehicle(PlayerSoldier* driver) {
    if (meraRIDER != nullptr) { meraRIDER->SetIsRiding(false); meraRIDER->setPlayerY(getY() - 50.0f); }
    meraRIDER = driver;
    if (meraRIDER == nullptr) health = baseHealth;
    else { health = baseHealth * meraRIDER->getVehicleHealthBuff(); fireRateCooldown = 1.0f * meraRIDER->getVehicleFireRateBuff(); }
}
void Vehicle::takeDamage(double damageAmount) {
    if (meraRIDER != nullptr && meraRIDER->isInvincible()) return;
    health -= damageAmount;
    if (health <= 0) {
        if (meraRIDER != nullptr) { if (meraRIDER->hasTotemOfUndying()) meraRIDER = nullptr; else meraRIDER->takeDamage(9999); }
        kill();
    }
}

void MetalSlug::update(char** lvl, int cellsize) {
    if (meraRIDER == nullptr) return;
    PFrame++; float currentVx = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { if (PD == 1) x += 200.0f; currentVx = -speed; PD = -1; }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { if (PD == -1) x -= 200.0f; currentVx = speed; PD = 1; }
    setVelocityX(currentVx);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && canJump) { velocityY = -jumpStrength; canJump = false; onGround = false; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) wantToFall = true;
    if (!onGround) { velocityY += gravity; if (velocityY > terminalVelocity) velocityY = terminalVelocity; }
    applyCollision(lvl, cellsize, Pwidth, Pheight, onGround, canJump, wantToFall, PD);
    wantToFall = false;
    if (myMode == 1 && myPerlin != nullptr) { // infinite mode perlin ground
        float groundY = (10.0f - (myPerlin->getheight(x / (float)cellsize) * 5.0f)) * cellsize;
        if (y + Pheight > groundY) { y = groundY - Pheight; velocityY = 0; onGround = true; canJump = true; }
    }
    if (x < 128 * 50.0f + Pwidth) x = 128 * 50.0f + Pwidth;
    if (clampToBiome2) { float maxSlugX = 128 * 2 * 50.0f - Pwidth; if (x > maxSlugX) x = maxSlugX; }
    if (y < 0) { y = 0; velocityY = 0; }
    this->x = x; this->y = y;
    if (PD == 1) slugSprite.setScale(2.0f, 2.0f); else slugSprite.setScale(-2.0f, 2.0f);
    if (!onGround) { slugSprite.setTexture(slugJumpTexture); slugSprite.setTextureRect(sf::IntRect(0, 0, 100, 100)); }
    else if (currentVx != 0.0f) {
        slugSprite.setTexture(slugTexture);
        if (PFrame % 5 == 0) { PlayerFrame++; if (PlayerFrame >= 6) PlayerFrame = 0; }
        slugSprite.setTextureRect(sf::IntRect(PlayerFrame * 100, 0, 100, 100));
    }
    else { slugSprite.setTexture(slugTexture); PlayerFrame = 0; slugSprite.setTextureRect(sf::IntRect(0, 0, 100, 100)); }
    if (PFrame >= 360) PFrame = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) meraRIDER->setManualAimAngle(meraRIDER->getManualAimAngle() - 3.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) meraRIDER->setManualAimAngle(meraRIDER->getManualAimAngle() + 3.0f);
    meraRIDER->setManualAimAngle(max(-90.0f, min(90.0f, meraRIDER->getManualAimAngle())));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isFiring = true; fire(manager);
        if (gunAnimClock.getElapsedTime().asMilliseconds() > 50) { gunFrame = (gunFrame == 0) ? 1 : 0; gunAnimClock.restart(); }
    }
    else { isFiring = false; gunFrame = 0; }
    slugGunSprite.setTextureRect(sf::IntRect(gunFrame * 100, 0, 100, 100));
    if (PD == 1) slugGunSprite.setScale(2.0f, 2.0f); else slugGunSprite.setScale(-2.0f, 2.0f);
}
void MetalSlug::takeDamage(double damageAmount) {
    health -= damageAmount;
    if (health <= 0) {
        if (meraRIDER != nullptr) { meraRIDER->SetIsRiding(false); meraRIDER->setPlayerY(getY() - 50.0f); if (meraRIDER->hasTotemOfUndying()) meraRIDER = nullptr; else meraRIDER->takeDamage(9999); }
        kill();
    }
}
void MetalSlug::fire(EntityManager* mgr) {
    if (meraRIDER == nullptr) return;
    float angleToUse = meraRIDER->getManualAimAngle();
    if (PD == -1) angleToUse = 180.0f - angleToUse;
    float fireX = (PD == 1) ? x + Pwidth + 30.0f : x - Pwidth - 30.0f;
    float fireY = y + Pheight / 3.0f - 15.0f;
    slugWeapon->fire(fireX, fireY, angleToUse, mgr, false);
}

void SlugFlyer::fire(EntityManager* mgr) {
    if (meraRIDER == nullptr) return;
    float angleToUse = meraRIDER->getManualAimAngle();
    if (PD == -1) angleToUse = 180.0f - angleToUse;
    float fireX = (PD == 1) ? x + Pwidth + 30.0f : x - 30.0f;
    float fireY = y + Pheight / 2.0f;
    flyerWeapon->fire(fireX, fireY, angleToUse, mgr, false);
}
void SlugFlyer::fireMissile(EntityManager* mgr) {
    if (missileCount <= 0) return;
    if (missileTimer.getElapsedTime().asSeconds() >= 0.5f) { mgr->addEntity(new Enemy_Sub_Missile(x, y + Pheight / 2.0f, PD, mgr)); missileCount--; missileTimer.restart(); }
}
void SlugFlyer::update(char** lvl, int cell_size) {
    if (meraRIDER == nullptr) return;
    float currentVx = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { currentVx = -speed; PD = -1; }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { currentVx = speed; PD = 1; }
    x += currentVx;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y += speed;
    hoverTime += hoverSpeed; y += sin(hoverTime) * 1.5f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) meraRIDER->setManualAimAngle(meraRIDER->getManualAimAngle() - 3.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) meraRIDER->setManualAimAngle(meraRIDER->getManualAimAngle() + 3.0f);
    meraRIDER->setManualAimAngle(max(-90.0f, min(90.0f, meraRIDER->getManualAimAngle())));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isFiring = true; fire(manager);
        if (gunAnimClock.getElapsedTime().asMilliseconds() > 50) { gunFrame = (gunFrame == 0) ? 1 : 0; gunAnimClock.restart(); }
    }
    else { isFiring = false; gunFrame = 0; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) fireMissile(manager);
    if (PD == 1) { slugFlyerSprite.setScale(2.0f, 2.0f); slugFlyerGunSprite.setScale(2.0f, 2.0f); }
    else { slugFlyerSprite.setScale(-2.0f, 2.0f); slugFlyerGunSprite.setScale(-2.0f, 2.0f); }
    slugFlyerGunSprite.setTextureRect(sf::IntRect(gunFrame * 100, 0, 100, 100));
    float maxFlyerX = 128 * 2 * 50.0f - Pwidth; float maxMapHeight = (18 * 1.2f * 50.0f) - 50.0f;
    if (x < 50.0f) x = 50.0f; if (x > maxFlyerX) x = maxFlyerX;
    if (y < 0) y = 0; if (y > maxMapHeight - Pheight) y = maxMapHeight - Pheight;
    meraRIDER->setPlayerX(x); meraRIDER->setPlayerY(y);
}

void SlugMariner::update(char** lvl, int cell_size) {
    if (meraRIDER == nullptr) return;

    float vx = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        vx = -speed;
        PD = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        vx = +speed;
        PD = 1;
    }
    x += vx;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y += speed;

    hoverTime += hoverSpeed;
    y += sin(hoverTime) * 0.8f;

    float minX = 128.f * 2.f * 50.f + 50.f;
    float maxX = 128.f * 3.f * 50.f - Pwidth - 50.f;
    float maxY = 18.f * 1.2f * 50.f - Pheight - 10.f;

    if (x < minX) x = minX;
    if (x > maxX) x = maxX;
    if (y < 10.f) y = 10.f;
    if (y > maxY) y = maxY;

    meraRIDER->setPlayerX(x);
    meraRIDER->setPlayerY(y);

    Marine_Sprite.setScale((float)PD, 1.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (Ammo_H > 0 && H_Clock.getElapsedTime().asSeconds() >= Cooldown)
        {
            float xxx = (PD == 1) ? x + Pwidth + 10.f : x - Pwidth - 10.f;
            float yyy = y + Pheight / 2.f - 15.f;
            manager->addEntity(new Marine_Missiles(xxx, yyy, 1, PD, manager));
            Ammo_H--;
            H_Clock.restart();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        if (Ammo_V > 0 && V_Clock.getElapsedTime().asSeconds() >= Cooldown) {
            float xxx = x + Pwidth / 2.f;
            float yyy = y - 10.f;
            manager->addEntity(new Marine_Missiles(xxx, yyy, 2, PD, manager));
            Ammo_V--;
            V_Clock.restart();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        if (Ammo_R > 0 && R_Clock.getElapsedTime().asSeconds() >= Cooldown) {
            float xxx = (PD == 1) ? x + Pwidth + 10.f : x - 10.f;
            float yyy = y + Pheight / 2.f;
            manager->addEntity(new Marine_Missiles(xxx, yyy, 3, PD, manager));
            Ammo_R--;
            R_Clock.restart();
        }
    }
}

void FlyingTara::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (x < (Biome - 1) * 6400 + 250 && scale == -1) {
        x = x - 175;
        speed = -speed;
        sprite.setScale(1.0f, 1.0f);
        scale = 1;
    }
    else if (x > Biome * 6400 - 200 && scale == 1) {
        x = x + 175;
        speed = -speed;
        sprite.setScale(-1.0f, 1.0f);
        scale = -1;
    }

    hoverTime += hoverSpeed;
    y += sin(hoverTime) * 1.5f;

    frame_FT++;
    IntRect FFrame(frame_F * 200, 0, 200, 100);
    sprite.setTextureRect(FFrame);

    if (sin(hoverTime) < 0.2f && sin(hoverTime) > -0.2f) {
        frame_F = 0;
    }
    else if (sin(hoverTime) < -0.2f) {
        frame_F = 2;
    }
    else {
        frame_F = 1;
    }

    Fire_Timer--;
    if (Fire_Timer <= 0)
        fire(manager);

    x += speed;
    sprite.setPosition(x, y);
}

void FlyingTara::fire(EntityManager* mgr)
{
    Entity* P = mgr->getPlayerEntity();
    if (P == nullptr)
    {
        velocityX = 0;
        return;
    }
    PlayerSoldier* player = P->asPlayer();
    float px = 0, py = 0;
    int Dir = 0;
    if (player != nullptr) {
        px = player->getX();
        py = player->getY();
        Dir = player->Get_P_D();
    }

    int temp1;
    if (Dir == -1) temp1 = -100;
    else temp1 = 100;

    if (((x >= px) && (x <= (px + temp1)) || (x <= px) && (x >= (px + temp1))) && player != nullptr) {
        mgr->addEntity(new Tara_Grenade(x - 100.0f, y + 20.0f, scale, mgr));
        Fire_Timer = 200;
    }
}

void Missile::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (!exploded)
    {
        vy += 0.6f;
        x += vx;
        y += vy;
        timer += 0.016f;

        Entity* P = manager->getPlayerEntity();
        if (P == nullptr)
        {
            velocityX = 0;
            return;
        }
        PlayerSoldier* player = P->asPlayer();
        float px = 0, py = 0;
        int Dir = 0;
        if (player != nullptr) {
            px = player->getX();
            py = player->getY();
            Dir = player->Get_P_D();
        }

        bool Damage = Check_Touch(px, py, 100, 100, x, y, frameWidth, frameHeight, Dir, -dir);
        if (timer >= fuseTime || Damage) {
            exploded = true;
            frame = 0;
            animClock.restart();
            if (Damage == true && player != nullptr)
            {
                cout << "Damage from Missile";
                player->takeDamage(30);
            }
        }

        for (int i = 0; i < manager->Get_Current_Count(); i++)
        {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead()) continue;
            if (!e->isPlayerVehicle()) continue;

            bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1);
            if (hit)
            {
                e->takeDamage(20);
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }
    }

    if (!exploded) {
        if (animClock.getElapsedTime().asMilliseconds() > 270) {
            frame++;
            animClock.restart();
        }
        if (frame <= maxFrames) {
            Missile_Sprite.setTextureRect(IntRect(frame * frameWidth, 0, frameWidth, frameHeight));
        }
    }
    else {
        if (explosionClock.getElapsedTime().asMilliseconds() > 90) {
            frame2++;
            explosionClock.restart();
        }

        explosionSprite.setTextureRect(IntRect((int)((frame2 % 3) * frameWidth2), 0, frameWidth2, frameHeight2));
        if (frame2 >= maxFrames2) kill();
    }
}

void Bradley::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (x < (Biome - 1) * 6400 + 250 && scale == -1) {
        x = x - 175;
        speed = -speed;
        sprite.setScale(1.5f, 1.5f);
        scale = 1;
    }
    else if (x > Biome * 6400 - 200 && scale == 1) {
        x = x + 175;
        speed = -speed;
        sprite.setScale(-1.5f, 1.5f);
        scale = -1;
    }

    Entity* P = manager->getPlayerEntity();
    if (P == nullptr)
    {
        velocityX = 0;
        return;
    }
    PlayerSoldier* player = P->asPlayer();

    float px = 0, X_Check = 0;
    int d = 0;
    if (player != nullptr) {
        px = player->getX();
        d = player->Get_P_D();
        if (d == 1) px -= 100;
    }

    if (scale == 1) X_Check = x - 125 * 1.5f;
    else X_Check = x;

    if (X_Check > px && scale == 1) {
        sprite.setScale(-1.5f, 1.5f);
        scale = -1;
    }
    else if (X_Check < px && scale == -1) {
        sprite.setScale(1.5f, 1.5f);
        scale = 1;
    }

    fireCooldown--;
    fire(manager);

    if (x > ((Biome - 1) * 6400 + 250) && x < (Biome * 6400 - 200)) {
        float dx = px - X_Check;
        if (dx > range) x += speed;
        else if (dx < -range) x -= speed;
    }
}

void Bradley::fire(EntityManager* mgr) {
    if (fireCooldown <= 0) {
        fireCooldown = 200;

        Entity* P = manager->getPlayerEntity();
        if (P == nullptr)
        {
            velocityX = 0;
            return;
        }
        PlayerSoldier* player = P->asPlayer();

        int Throwing_point;
        if (scale == 1) Throwing_point = (int)(x + 125 * 1.8f);
        else Throwing_point = (int)(x - 125 * 1.7f);

        if (player != nullptr) {
            manager->addEntity(new Missile((float)Throwing_point, y - 30.0f, scale, manager));
        }
    }
}

void Enemy_Sub_Missile::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (!exploded) {
        if (velocityX > -15 && velocityX < 15) velocityX += 0.8f * dir;
        x += velocityX;
        timer += 0.016f;

        Entity* P = manager->getPlayerEntity();
        if (P == nullptr)
        {
            velocityX = 0;
            return;
        }
        PlayerSoldier* player = P->asPlayer();
        float px = 0, py = 0;
        int Dir = 0;
        if (player != nullptr) {
            px = player->getX();
            py = player->getY();
            Dir = player->Get_P_D();
        }

        bool Damage = Check_Touch(px, py, 100, 100, x, y, 100, 75, Dir, dir);
        if (timer >= fuseTime || Damage) {
            exploded = true;
            frame = 0;
            animClock.restart();
            if (Damage == true && player != nullptr)
            {
                cout << "Damage from Submarine Missile\n";
                player->takeDamage(20);
            }
        }

        for (int i = 0; i < manager->Get_Current_Count(); i++) {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead()) continue;
            if (!e->isPlayerVehicle()) continue;

            bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1);
            if (hit)
            {
                e->takeDamage(20);
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }
        return;
    }

    if (animClock.getElapsedTime().asMilliseconds() > 90) {
        frame++;
        animClock.restart();
    }

    if (frame >= maxFrames * 2) {
        kill();
    }

    explosionSprite.setTextureRect(IntRect((int)(frame % 3 * frameWidth), 0, frameWidth, frameHeight));
}

void Marine_Missiles::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (!exploded)
    {
        velocityY += gravity;
        x += velocityX;
        y += velocityY;
        timer += 0.016f;

        int Map_Height = (int)(18 * 1.2f);
        int MAP_Width = 128 * 3;

        if (x < 0 || x > MAP_Width * 50 || y < 0 || y > Map_Height * 50) {
            kill();
            return;
        }

        int col = (int)(x / cell_size);
        int row = (int)(y / cell_size);
        if (row >= 0 && row < Map_Height && col >= 0 && col < MAP_Width) {
            if (lvl[row][col] == '#' || lvl[row][col] == 'S') {
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }

        for (int i = 0; i < manager->Get_Current_Count(); i++)
        {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead() || !e->isEnemy()) continue;
            bool hit = Check_Touch(e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(), x, y, width, height, 1, 1);
            if (hit)
            {
                cout << "Damage from marine missile\n";
                e->takeDamage(25);
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }

        if (timer >= fuseTime) {
            exploded = true;
            frame = 0;
            animClock.restart();
        }
        return;
    }

    if (animClock.getElapsedTime().asMilliseconds() > 90) {
        frame++;
        animClock.restart();
    }
    if (frame >= maxFrames * 2) {
        kill();
        return;
    }

    explosionSpr.setTextureRect(sf::IntRect((frame % maxFrames) * frameW, 0, frameW, frameH));
}

void FlyerMissile::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (!exploded) {
        if (velocityX > -15 && velocityX < 15) velocityX += 0.8f * dir;
        x += velocityX;
        timer += 0.016f;

        int Map_Height = (int)(18 * 1.2f);
        int Map_Width = 128 * 3;

        if (x < 0 || x > Map_Width * 50 || y < 0 || y > Map_Height * 50) {
            kill();
            return;
        }

        int col = (int)(x / cell_size);
        int row = (int)(y / cell_size);

        if (row >= 0 && row < Map_Height && col >= 0 && col < Map_Width) {
            if (lvl[row][col] == '#') {
                exploded = true;
                frame = 0;
                animClock.restart();
            }
        }

        if (timer >= fuseTime) {
            exploded = true;
            frame = 0;
            animClock.restart();
        }
        return;
    }

    if (animClock.getElapsedTime().asMilliseconds() > 90) {
        frame++;
        animClock.restart();
    }

    if (frame >= maxFrames * 2) kill();

    explosionSprite.setTextureRect(IntRect((int)(frame % 3 * frameE), 0, frameE, frameE));
}

void Enemy_Sub::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (x < (Biome - 1) * 6400 + 250 && scale == -1) {
        x = x - 175;
        speed = -speed;
        sprite.setScale(2.0f, 1.5f);
        scale = 1;
    }
    else if (x > Biome * 6400 - 200 && scale == 1) {
        x = x + 175;
        speed = -speed;
        sprite.setScale(-2.0f, 1.5f);
        scale = -1;
    }

    int d = 0;
    Entity* P = manager->getPlayerEntity();
    if (P == nullptr)
    {
        velocityX = 0;
        return;
    }
    PlayerSoldier* player = P->asPlayer();
    if (player != nullptr) {
        d = player->Get_P_D();
    }

    hoverTime += hoverSpeed;
    y += sin(hoverTime);

    frame_FT++;
    IntRect FFrame(frame_F * 200, 0, 200, 100);
    sprite.setTextureRect(FFrame);

    if (frame_F == 6) frame_F = 0;
    if (frame_FT >= 25) {
        frame_FT = 0;
        frame_F++;
    }

    Fire_Timer--;
    if (Fire_Timer <= 0) fire(manager);

    x += speed;
    sprite.setPosition(x, y);
}

void Enemy_Sub::fire(EntityManager* mgr)
{
    Entity* P = mgr->getPlayerEntity();
    if (P == nullptr)
    {
        velocityX = 0;
        return;
    }
    PlayerSoldier* player = P->asPlayer();
    float px = 0, py = 0;
    if (player != nullptr) {
        px = player->getX();
        py = player->getY();
    }

    if (((y >= py - 30) && (y <= (py + 130))) && player != nullptr) {
        int Dir, location_Start;
        if (x < px) Dir = 1;
        else Dir = -1;

        if (Dir == 1) location_Start = (int)x + 100;
        else location_Start = (int)x + 100;

        mgr->addEntity(new Enemy_Sub_Missile((float)location_Start, y + 20.0f, Dir, mgr));
        Fire_Timer = 200;
    }
}

void Enemy::shoot() {
    if (activeWeapon != nullptr)
    {
        float currentAngle = (P_D == 1) ? 0.0f : 180.0f;
        activeWeapon->fire(x + (Pwidth / 2.0f), y + (Pheight / 4.0f), currentAngle, myManager, false);

        // mark the last added bullet as enemy bullet
        int last = myManager->Get_Current_Count() - 1;
        if (last >= 0)
        {
            Entity* last_entity = myManager->getEntity(last);
            if (last_entity && last_entity->isBullet())
                last_entity->setAsEnemyBullet();
        }
    }
}

void Enemy::update(char** lvl, int cell_size) {
    if (isDead) return;
    float prev_feet_y = y + (float)Pheight;

    if (aiState == 0) {
        velocityX = speed * (float)P_D;
        if (aiTimer.getElapsedTime().asSeconds() >= moveDuration) {
            aiState = 1;
            Player_Frame = 0;
            triggerJump();
        }
    }
    else if (aiState == 1)
    {
        velocityX = speed * (float)P_D;
        if (onGround == true && velocityY >= 0) {
            aiState = 2;
            velocityX = 0.0f;
            Player_Frame = 0;
            aiTimer.restart();
        }
    }
    else if (aiState == 2) {
        velocityX = 0.0f;
        if (aiTimer.getElapsedTime().asSeconds() > 1.5f) {
            shoot();
            aiState = 0;
            P_D = (rand() % 2 == 0) ? 1 : -1;
            moveDuration = (float)(rand() % 6 + 2);
            Player_Frame = 0;
            aiTimer.restart();
        }
    }

    animate();

    x = x + velocityX;
    checkEdge(lvl, cell_size);

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > terminal_Velocity) velocityY = terminal_Velocity;
    }

    float tempY = y + velocityY;
    int Map_Height = (int)(18 * 1.2f);
    int Map_Width = 128 * 3;

    int feet_row = (int)(tempY + Pheight) / cell_size;
    int left_col = (int)(x) / cell_size;
    int middle_col = (int)(x + Pwidth / 2.0f) / cell_size;
    int right_col = (int)(x + Pwidth) / cell_size;

    if (feet_row >= Map_Height) feet_row = Map_Height - 1;
    if (feet_row < 0) feet_row = 0;

    if (left_col < 0) left_col = 0;
    if (left_col >= Map_Width) left_col = Map_Width - 1;

    if (right_col < 0) right_col = 0;
    if (right_col >= Map_Width) right_col = Map_Width - 1;

    if (middle_col < 0) middle_col = 0;
    if (middle_col >= Map_Width) middle_col = Map_Width - 1;

    char below_left = lvl[feet_row][left_col];
    char below = lvl[feet_row][middle_col];
    char below_right = lvl[feet_row][right_col];

    int head_row = (int)(tempY) / cell_size;
    if (head_row < 0) head_row = 0;
    if (head_row >= Map_Height) head_row = Map_Height - 1;

    char above_left = lvl[head_row][left_col];
    char above = lvl[head_row][middle_col];
    char above_right = lvl[head_row][right_col];

    onGround = false;
    float platform_top = (float)(feet_row * cell_size);

    if (below_left == '#' || below_right == '#' || below == '#') {
        onGround = true;
        velocityY = 0;
        y = platform_top - Pheight;
    }
    else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
        if (wantToFall) {
            y = tempY + 5.0f;
            onGround = false;
        }
        else {
            if (prev_feet_y <= platform_top) {
                onGround = true;
                velocityY = 0;
                y = platform_top - (float)Pheight;
            }
            else {
                y = tempY;
            }
        }
    }
    else if (velocityY < 0 && (above_left == 'P' || above_right == 'P' || above == 'P' || above_left == 'S' || above_right == 'S' || above == 'S')) {
        y = tempY;
    }
    else {
        y = tempY;
    }

    wantToFall = false;
    canJump = onGround;

    if (x <= 50.0f) {
        x = 50.0f;
        P_D = 1;
    }

    float maxMapWidth = (Map_Width * 50.0f) - 50.0f;
    if (x >= maxMapWidth - (float)Pwidth) {
        x = maxMapWidth - (float)Pwidth;
        P_D = -1;
    }

    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y > (18 * 1.2f * 50.0f) + 100.0f) {
        kill();
        return;
    }
}

void rebel_soldier::animate()
{
    RWidth = 100;
    float xxx = 0;
    if (aiState == 0)
    {
        xxx = 5;
        sprite.setTexture(texMove);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 5)
                Player_Frame = 0;
        }
    }
    else if (aiState == 1)
    {
        sprite.setTexture(texJump);
        Player_Frame = 0;
    }
    else if (aiState == 2)
    {
        xxx = -10;
        RWidth = 120;
        sprite.setTexture(texShoot);
        if (P_Frame % 100 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 2)
                Player_Frame = 0;
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(sf::IntRect(Player_Frame * (RWidth + xxx), 0, RWidth, RHeight));
    if (P_D == 1)
        sprite.setScale(-1, 1);
    else
        sprite.setScale(1, 1);
}
void rebel_soldier::shoot()
{
    float spawnX = (P_D == 1) ? x + Pwidth : x;
    float spawnY = y + Pheight / 2.0f;
    float bVx = (P_D == 1) ? 15.0f : -15.0f;
    float angle = (P_D == 1) ? 0.0f : 180.0f;
    Bullet* b = new Bullet(spawnX, spawnY, bVx, 0.0f, 3, 1, angle, myManager);
    b->setEnemyBullet(true);
    myManager->addEntity(b);
}
void shieldsoldier::animate()
{
    if (aiState == 0)
    {
        sprite.setTexture(texMove);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 5)
                Player_Frame = 0;
        }
    }
    else if (aiState == 1)
    {
        sprite.setTexture(texJump);
        if (P_Frame % 10 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 4)
                Player_Frame = 0;
        }
    }
    else if (aiState == 2)
    {
        sprite.setTexture(texShoot);
        if (P_Frame % 50 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 4)
                Player_Frame = 0;
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(IntRect(Player_Frame * RWidth, 0, RWidth, RHeight));

    if (P_D == 1)
        sprite.setScale(-1, 1);
    else
        sprite.setScale(1, 1);
}
void shieldsoldier::shoot()
{
    float spawnX = (P_D == 1) ? x + Pwidth + 10.0f : x - 10.0f;
    float spawnY = y + (Pheight / 4.0f);
    float angle = (P_D == 1) ? 0.0f : 180.0f;
    float bVx = (P_D == 1) ? 15.0f : -15.0f;

    Bullet* b = new Bullet(spawnX, spawnY, bVx, 0.0f, 3, 1, angle, myManager);
    b->setEnemyBullet(true);
    myManager->addEntity(b);
}
void bazooka_soldier::animate()
{
    if (aiState != lastAiState)
    {
        if (aiState == 2)
        {
            isCrouchShooting = ((rand() % 100) < 40);
        }
        lastAiState = aiState;
    }

    int xxx = 0;
    if (aiState == 0)
    {
        sprite.setTexture(texMove);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 5)
                Player_Frame = 0;
        }
    }
    else if (aiState == 1)
    {
        sprite.setTexture(texJump);
        if (P_Frame % 10 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 4)
                Player_Frame = 0;
        }
    }
    else if (aiState == 2)
    {
        if (isCrouchShooting)
        {
            xxx = 5;
            sprite.setTexture(texShootCrouch);
            if (P_Frame % 20 == 0)
            {
                Player_Frame++;
                if (Player_Frame >= 5)
                    Player_Frame = 0;
            }
        }
        else
        {
            sprite.setTexture(texShootStd);
            if (P_Frame % 50 == 0)
            {
                Player_Frame++;
                if (Player_Frame >= 4)
                    Player_Frame = 0;
            }
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(sf::IntRect(Player_Frame * (xxx + RWidth), 0, RWidth, RHeight));

    if (P_D == 1)
        sprite.setScale(-1, 1);
    else
        sprite.setScale(1, 1);
}

void bazooka_soldier::shoot() {
    float spawnY = y;
    if (isCrouchShooting) {
        spawnY += 20.0f;
    }
    float spawnX = (P_D == 1) ? x + Pwidth : x - 40.0f;
    myManager->addEntity(new Missile(spawnX, spawnY, P_D, myManager));
}

void grenade_soldier::animate()
{
    float xxx = 0;
    if (aiState == 0 || aiState == 2)
    {
        xxx = 5;
        sprite.setTexture(texMove);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 5)
                Player_Frame = 0;
        }
        hasThrownInAir = false;
    }
    else if (aiState == 1)
    {
        sprite.setTexture(texJump);
        Player_Frame = 0;

        if (!hasThrownInAir)
        {
            float power = 12.0f;
            float angleMath = 0.7071f;
            float calcVx = power * angleMath * P_D;
            float calcVy = -power * angleMath;

            float spawnX = (P_D == 1) ? x + Pwidth : x;
            float spawnY = y - 50.0f;

            PlayerGrenade* pg = new PlayerGrenade(spawnX, spawnY, calcVx, calcVy, myManager);
            pg->setEnemyGrenade(true);
            myManager->addEntity(pg);
            hasThrownInAir = true;
        }
    }

    P_Frame++;

    if (P_Frame >= 360)
        P_Frame = 0;


    sprite.setTextureRect(sf::IntRect(Player_Frame * (RWidth + xxx), 0, RWidth, RHeight));

    if (P_D == 1)
        sprite.setScale(-1, 1);
    else
        sprite.setScale(1, 1);
}

paratrooper::paratrooper(float x, float y, EntityManager* mgr) : Enemy(x, y, 10, mgr) {
    setWeapon(nullptr);
    texPara.loadFromFile("Paratrooper.png");
    sprite.setTexture(texPara);
    glideTimer = 0.0f;
    gravity = 0.05f;
    terminal_Velocity = 2.5f;
    Pwidth = 70;
    Pheight = 100;
}

void paratrooper::animate()
{
    sprite.setTexture(texPara);
    int w = texPara.getSize().x;
    int h = texPara.getSize().y;

    sf::IntRect rect(0, 0, w, h);
    sprite.setTextureRect(rect);
    sprite.setOrigin((float)w / 2.0f, (float)h / 2.0f);

    if (velocityX > 0)
        sprite.setScale(1.5, 1.5);
    else
        sprite.setScale(-1.5, 1.5);
}

void paratrooper::decideSTATE()
{
    int roll = rand() % 100;
    Enemy* newInfantry = nullptr;

    if (roll < 40)
    {

        newInfantry = new rebel_soldier(x, y, myManager);
    }
    else if (roll < 70)
    {
        newInfantry = new grenade_soldier(x, y, myManager);
    }
    else if (roll < 90)
    {
        newInfantry = new shieldsoldier(x, y, myManager);
    }
    else
    {
        newInfantry = new bazooka_soldier(x, y, myManager);
    }

    newInfantry->setScorePtr(scorePtr);
    if (scorePtr) *scorePtr += 25;
    myManager->addEntity(newInfantry);
    kill();
}

void paratrooper::update(char** lvl, int cell_size) {
    if (isDead) return;

    glideTimer += 0.03f;
    velocityX = sin(glideTimer) * 3.5f;

    animate();

    x = x + velocityX;

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > terminal_Velocity)
            velocityY = terminal_Velocity;
    }

    float tempY = y + velocityY;
    int Map_Height = (int)(18 * 1.2f);
    int Map_Width = 128 * 3;

    int feet_row = (int)(tempY + Pheight) / cell_size;
    int left_col = (int)(x) / cell_size;
    int middle_col = (int)(x + Pwidth / 2.0f) / cell_size;
    int right_col = (int)(x + Pwidth) / cell_size;

    if (feet_row >= Map_Height) feet_row = Map_Height - 1;
    if (feet_row < 0) feet_row = 0;

    if (left_col < 0) left_col = 0;
    if (left_col >= Map_Width) left_col = Map_Width - 1;

    if (right_col < 0) right_col = 0;
    if (right_col >= Map_Width) right_col = Map_Width - 1;

    if (middle_col < 0) middle_col = 0;
    if (middle_col >= Map_Width) middle_col = Map_Width - 1;

    char below_left = lvl[feet_row][left_col];
    char below = lvl[feet_row][middle_col];
    char below_right = lvl[feet_row][right_col];

    int head_row = (int)(tempY) / cell_size;
    if (head_row < 0) head_row = 0;
    if (head_row >= Map_Height) head_row = Map_Height - 1;

    onGround = false;
    float platform_top = (float)(feet_row * cell_size);

    if (below_left == '#' || below_right == '#' || below == '#') {
        onGround = true;
        velocityY = 0;
        y = platform_top - Pheight;
    }
    else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
        if (wantToFall) {
            y = tempY + 5.0f;
            onGround = false;
        }
        else {
            if (y + Pheight <= platform_top) {
                onGround = true;
                velocityY = 0;
                y = platform_top - (float)Pheight;
            }
            else {
                y = tempY;
            }
        }
    }
    else if (velocityY < 0 && (below_left == 'P' || below_right == 'P' || below == 'P' || below_left == 'S' || below_right == 'S' || below == 'S')) {
        y = tempY;
    }
    else {
        y = tempY;
    }

    wantToFall = false;
    canJump = onGround;

    if (x <= 50.0f) {
        x = 50.0f;
        P_D = 1;
    }
    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y > (18 * 1.2f * 50.0f) + 100.0f) {
        kill();
        return;
    }
    float maxMapWidth = (Map_Width * 50.0f) - 50.0f;
    if (x >= maxMapWidth - (float)Pwidth) {
        x = maxMapWidth - (float)Pwidth;
        P_D = -1;
    }

    if (onGround) {
        decideSTATE();
    }
}

void Mummy::takeDamage(double damageAmount)
{
    if (aiState > 0)
        return;

    if (damageAmount == 2 && aiState == 0)
    {
        //Za flame shot
        aiState = 2;
        Player_Frame = 0;
        P_Frame = 0;
        return;

    }
    if (damageAmount >= 15.0)
    {
        aiState = 2;
        Player_Frame = 0;
        P_Frame = 0;
        return;
    }

    health -= damageAmount;
    cout << "Mummy Health Now :" << health << endl;
    if (health <= 0) {
        aiState = 1;
        Player_Frame = 0;
        P_Frame = 0;
    }
}

void Mummy::animate()
{
    if (aiState == 3)
    {
        sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
        return;
    }
    int xxx = 0;
    if (aiState == 0) {
        if (!spawnSaved && onGround)
        {
            spawnX = x;
            spawnY = y;
            spawnSaved = true;
        }
        sprite.setTexture(mumMOVE);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 7)
                Player_Frame = 0;
        }
    }
    else if (aiState == 1)
    {
        xxx = 3;
        velocityX = 0.0f;
        velocityY = 0.0f;
        sprite.setTexture(mumCRUMBLE);
        if (P_Frame % 15 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 12)
            {
                aiState = 3;

                aiTimer.restart();
                return;
            }
        }
    }
    else if (aiState == 2)
    {
        xxx = 1;
        sprite.setTexture(mumBEINGHIT);
        if (P_Frame % 5 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 22)
            {
                kill();
                return;
            }
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(sf::IntRect(Player_Frame * (xxx + RWidth), 0, RWidth, RHeight));

    if (P_D == 1)
        sprite.setScale(-1, 1);
    else
        sprite.setScale(1, 1);
}

void Mummy::update(char** lvl, int cell_size)
{
    if (isDead)
        return;
    checkEdge(lvl, cell_size);
    if (aiState == 3)
    {
        if (aiTimer.getElapsedTime().asSeconds() >= 3.0f) {
            myManager->addEntity(new Mummy(spawnX, spawnY, myManager));
            kill();
        }
        return;
    }

    for (int i = 0; i < myManager->Get_Current_Count(); i++) {
        Entity* e = myManager->getEntity(i);
        if (e == nullptr || e->getIsDead()) continue;

        if (e->isGrenade())
        {
            if (!e->getIsEnemyGrenade())
            {
                if (Touching(e->getX(), e->getY(), x, y, Pwidth, Pheight))
                {
                    takeDamage(20);
                    e->kill();
                }
            }
        }
        else if (e->isBullet())
        {
            if (!e->getIsEnemyBullet())
            {
                if (Touching(e->getX(), e->getY(), x, y, Pwidth, Pheight))
                {
                    takeDamage(3);
                    e->kill();
                }
            }
        }
    }

    if (aiState == 0)
    {
        Entity* P = myManager->getPlayerEntity();
        if (P == nullptr)
        {
            velocityX = 0;
            return;
        }
        PlayerSoldier* player = P->asPlayer();

        if (player != nullptr) {
            bool touched = Check_Touch(x, y, Pwidth, Pheight, player->getX(), player->getY(), (int)player->getwidth(), (int)player->getheight(), P_D, player->Get_P_D());
            if (touched) {
                player->takeDamage(100);
            }

            if (player->getX() < x) P_D = -1;
            else P_D = 1;

            velocityX = speed * (float)P_D;
        }
        else {
            velocityX = 0;
        }
    }
    else {
        velocityX = 0.0f;
    }

    animate();

    x = x + velocityX;

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > terminal_Velocity) velocityY = terminal_Velocity;
    }

    float tempY = y + velocityY;
    int Map_Height = (int)(18 * 1.2f);
    int Map_Width = 128 * 3;

    int feet_row = (int)(tempY + Pheight) / cell_size;
    int left_col = (int)(x) / cell_size;
    int middle_col = (int)(x + Pwidth / 2.0f) / cell_size;
    int right_col = (int)(x + Pwidth) / cell_size;

    if (feet_row >= Map_Height) feet_row = Map_Height - 1;
    if (feet_row < 0) feet_row = 0;

    if (left_col < 0) left_col = 0;
    if (left_col >= Map_Width) left_col = Map_Width - 1;

    if (right_col < 0) right_col = 0;
    if (right_col >= Map_Width) right_col = Map_Width - 1;

    if (middle_col < 0) middle_col = 0;
    if (middle_col >= Map_Width) middle_col = Map_Width - 1;

    char below_left = lvl[feet_row][left_col];
    char below = lvl[feet_row][middle_col];
    char below_right = lvl[feet_row][right_col];

    int head_row = (int)(tempY) / cell_size;
    if (head_row < 0) head_row = 0;
    if (head_row >= Map_Height) head_row = Map_Height - 1;

    onGround = false;
    float platform_top = (float)(feet_row * cell_size);

    if (below_left == '#' || below_right == '#' || below == '#') {
        onGround = true;
        velocityY = 0;
        y = platform_top - Pheight;
    }
    else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
        if (wantToFall) {
            y = tempY + 5.0f;
            onGround = false;
        }
        else {
            if (y + Pheight <= platform_top) {
                onGround = true;
                velocityY = 0;
                y = platform_top - (float)Pheight;
            }
            else {
                y = tempY;
            }
        }
    }
    else if (velocityY < 0 && (below_left == 'P' || below_right == 'P' || below == 'P' || below_left == 'S' || below_right == 'S' || below == 'S')) {
        y = tempY;
    }
    else {
        y = tempY;
    }

    wantToFall = false;
    canJump = onGround;

    if (x <= 50.0f) {
        x = 50.0f;
        P_D = 1;
    }

    float maxMapWidth = (Map_Width * 50.0f) - 50.0f;
    if (x >= maxMapWidth - (float)Pwidth) {
        x = maxMapWidth - (float)Pwidth;
        P_D = -1;
    }

    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y > (18 * 1.2f * 50.0f) + 100.0f) {
        kill();
        return;
    }
}

Zombie::Zombie(float x, float y, EntityManager* mgr) : Enemy(x, y, 5, mgr)
{
    setWeapon(new pistol());
    texMove1.loadFromFile("Zombie_Move_1.png");
    texMove2.loadFromFile("Zombie_Move_2.png");
    texAim.loadFromFile("Zombie_Shoot.png");
    texJump.loadFromFile("Zombie_JUMP.png");

    RWidth = 100;
    RHeight = 100;
    Entity::width = RWidth;
    Entity::height = RHeight;

    sprite.setTexture(texMove1);
    speed = 0.6f;
    aiState = 0;
    hasShot = false;

    moveDuration = (float)(rand() % 4 + 3);
    aiTimer.restart();
}

void Zombie::animate()
{

    float xxx = 0;
    if (aiState == 0)
    {
        xxx = -17;
        sprite.setTexture(texMove1);
        if (P_Frame % 40 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 5)
            {
                aiState = 1;
                Player_Frame = 0;
            }
        }
    }
    else if (aiState == 1)
    {
        xxx = -1;
        sprite.setTexture(texMove2);
        if (P_Frame % 10 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 6)
            {
                aiState = 0;
                Player_Frame = 0;
            }
        }
    }
    else if (aiState == 2)
    {
        xxx = 1;
        sprite.setTexture(texAim);
        if (P_Frame % 15 == 0)
        {
            Player_Frame++;
            if (Player_Frame == 5 && !hasShot)
            {
                shoot();
                hasShot = true;
            }
            if (Player_Frame >= 5)
            {
                aiState = 0;
                Player_Frame = 0;
                aiTimer.restart();
                moveDuration = (float)(rand() % 4 + 3);
            }
        }
    }
    else if (aiState == 3)
    {
        xxx = -0.5;
        sprite.setTexture(texJump);
        if (P_Frame % 10 == 0)
        {
            Player_Frame++;
            if (Player_Frame >= 7)
            {
                aiState = 0;
                Player_Frame = 0;
                aiTimer.restart();
                moveDuration = (float)(rand() % 4 + 3);
            }
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(sf::IntRect(Player_Frame * (RWidth + xxx), 0, RWidth, RHeight));

    if (P_D == 1)
        sprite.setScale(1, 1);
    else
        sprite.setScale(-1, 1);
}

void Zombie::shoot() {
    float spawnX = (P_D == 1) ? x + Pwidth + 25.0f : x - 25.0f;
    float spawnY = y + (Pheight / 4.0f) + 15.0f;
    float bVx = (P_D == 1) ? 15.0f : -15.0f;
    float angle = (P_D == 1) ? 0.0f : 180.0f;

    Bullet* zombieBullet = new Bullet(spawnX, spawnY, bVx, 0.0f, 3, 1, angle, myManager);
    zombieBullet->setEnemyBullet(true);
    myManager->addEntity(zombieBullet);
}

void Zombie::update(char** lvl, int cell_size)
{
    if (isDead)
        return;
    checkEdge(lvl, cell_size);

    int playerDir = P_D;
    Entity* P = myManager->getPlayerEntity();
    if (P == nullptr)
    {
        velocityX = 0;
        return;
    }
    PlayerSoldier* player = P->asPlayer();

    if (player != nullptr) {
        if (player->getX() < x) playerDir = -1;
        else playerDir = 1;

        bool touched = Check_Touch(x, y, Pwidth, Pheight, player->getX(), player->getY(), (int)player->getwidth(), (int)player->getheight(), P_D, player->Get_P_D());
        if (touched)
        {
            cout << "Damage from Zombie\n";
            player->takeDamage(100);
        }
    }

    if (aiState == 0 || aiState == 1)
    {
        if (playerDir != P_D) {
            aiState = 2;
            Player_Frame = 0;
            P_Frame = 0;
            velocityX = 0.0f;
        }
        else
        {
            velocityX = speed * (float)P_D;

            if (aiTimer.getElapsedTime().asSeconds() > moveDuration)
            {
                if (rand() % 2 == 0) {
                    aiState = 2;
                    hasShot = false;
                }
                else {
                    aiState = 3;
                    velocityY = -10.0f;
                }
                Player_Frame = 0;
                velocityX = 0.0f;
            }
        }
    }
    else
    {
        velocityX = 0.0f;
    }

    animate();

    x = x + velocityX;

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > terminal_Velocity) velocityY = terminal_Velocity;
    }

    float tempY = y + velocityY;
    int Map_Height = (int)(18 * 1.2f);
    int Map_Width = 128 * 3;

    int feet_row = (int)(tempY + Pheight) / cell_size;
    int left_col = (int)(x) / cell_size;
    int middle_col = (int)(x + Pwidth / 2.0f) / cell_size;
    int right_col = (int)(x + Pwidth) / cell_size;

    if (feet_row >= Map_Height) feet_row = Map_Height - 1;
    if (feet_row < 0) feet_row = 0;

    if (left_col < 0) left_col = 0;
    if (left_col >= Map_Width) left_col = Map_Width - 1;

    if (right_col < 0) right_col = 0;
    if (right_col >= Map_Width) right_col = Map_Width - 1;

    if (middle_col < 0) middle_col = 0;
    if (middle_col >= Map_Width) middle_col = Map_Width - 1;

    char below_left = lvl[feet_row][left_col];
    char below = lvl[feet_row][middle_col];
    char below_right = lvl[feet_row][right_col];

    int head_row = (int)(tempY) / cell_size;
    if (head_row < 0) head_row = 0;
    if (head_row >= Map_Height) head_row = Map_Height - 1;

    onGround = false;
    float platform_top = (float)(feet_row * cell_size);

    if (below_left == '#' || below_right == '#' || below == '#') {
        onGround = true;
        velocityY = 0;
        y = platform_top - Pheight;
    }
    else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
        if (wantToFall) {
            y = tempY + 5.0f;
            onGround = false;
        }
        else {
            if (y + Pheight <= platform_top) {
                onGround = true;
                velocityY = 0;
                y = platform_top - (float)Pheight;
            }
            else {
                y = tempY;
            }
        }
    }
    else if (velocityY < 0 && (below_left == 'P' || below_right == 'P' || below == 'P' || below_left == 'S' || below_right == 'S' || below == 'S')) {
        y = tempY;
    }
    else {
        y = tempY;
    }

    wantToFall = false;
    canJump = onGround;

    if (x <= 50.0f) { x = 50.0f; P_D = 1; }

    float maxMapWidth = (Map_Width * 50.0f) - 50.0f;
    if (x >= maxMapWidth - (float)Pwidth) { x = maxMapWidth - (float)Pwidth; P_D = -1; }

    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y > (18 * 1.2f * 50.0f) + 100.0f) {
        kill();
        return;
    }
}

Martian::Martian(float x, float y, EntityManager* mgr) : Enemy(x, y, 3, mgr) {
    isFLYING = true;
    MARTIANHP = 3;
    PODHP = 30;
    RWidth = 150;
    RHeight = 150;
    Entity::width = RWidth;
    Entity::height = RHeight;
    moveDuration = 2.0f;

    MartianBASE.loadFromFile("Martian.png");
    MartianIDLE.loadFromFile("Martian_Stop.png");
    MartianMOVE.loadFromFile("Martian_Move.png");
    MartianFALLING.loadFromFile("Martian_Fall.png");
    MartianSHOOT.loadFromFile("Martian_Fire.png");
    MartianJUMP.loadFromFile("Martian_Jump.png");
    MartianPOD.loadFromFile("MARTIAN_POD.png");
    MartianTURN.loadFromFile("Martian_Rotate.png");

    podSprite.setTexture(MartianPOD);
    podSprite.setOrigin((float)MartianPOD.getSize().x / 4.0f, (float)MartianPOD.getSize().y);
    podSprite.setScale(0.10f, 0.10f);

    sprite.setTexture(MartianBASE);
    speed = 2.0f;
    velocityX = speed;
    aiState = 0;
    lastPD = 1;
}

void Martian::takeDamage(double damageAmount) {
    if (isFLYING)
    {
        PODHP -= (int)damageAmount;
        if (PODHP <= 0)
        {
            cout << "Maritan Air hp 0\n";
            isFLYING = false;
            aiState = 1;
            Player_Frame = 0;
            P_Frame = 0;
            velocityY = 0.0f;
        }
    }
    else
    {
        if (aiState == 1)
            return;
        MARTIANHP -= (int)damageAmount;
        cout << "Ground Martian Health Now :" << health << endl;
        if (MARTIANHP <= 0)
        {
            if (scorePtr) *scorePtr += 1500;
            kill();
        }
    }
}
void Martian::animate()
{
    if (aiState == 0 || aiState == 1) {
        sprite.setTexture(MartianBASE);
        if (P_Frame % 12 == 0) {
            Player_Frame++;
            if (Player_Frame >= 1)
                Player_Frame = 0;
        }
    }
    else if (aiState == 2) {
        sprite.setTexture(MartianMOVE);
        if (P_Frame % 6 == 0) {
            Player_Frame++;
            if (Player_Frame >= 16)
                Player_Frame = 0;
        }
    }
    else if (aiState == 3) {
        sprite.setTexture(MartianTURN);
        if (P_Frame % 6 == 0) {
            Player_Frame++;
            if (Player_Frame >= 8) {
                aiState = 2;
                Player_Frame = 0;
                P_D = (P_D == 1) ? -1 : 1;
                lastPD = P_D;
                aiTimer.restart();
                moveDuration = (float)(rand() % 4 + 2);
            }
        }
    }
    else if (aiState == 4) {
        sprite.setTexture(MartianSHOOT);
        if (P_Frame % 8 == 0) {
            Player_Frame++;
            if (Player_Frame >= 6) {
                aiState = 2;
                Player_Frame = 0;
                aiTimer.restart();
                moveDuration = (float)(rand() % 4 + 2);
            }
        }
    }
    else if (aiState == 5) {
        sprite.setTexture(MartianJUMP);
        if (P_Frame % 6 == 0) {
            Player_Frame++;
            if (Player_Frame >= 14) {
                aiState = 2;
                Player_Frame = 0;
                aiTimer.restart();
                moveDuration = (float)(rand() % 4 + 2);
            }
        }
    }

    P_Frame++;
    if (P_Frame >= 360)
        P_Frame = 0;

    sprite.setTextureRect(sf::IntRect(Player_Frame * 150, 0, 150, 120));

    if (P_D == 1)
        sprite.setScale(-1.5f, 1.5f);
    else
        sprite.setScale(1.5f, 1.5f);
}

void Martian::update(char** lvl, int cell_size) {
    if (isDead) return;

    int totalEntities = myManager->Get_Current_Count();
    for (int i = 0; i < totalEntities; i++)
    {
        Entity* e = myManager->getEntity(i);
        if (e == nullptr || e->getIsDead()) continue;

        if (e->isBullet() && !e->getIsEnemyBullet())
        {
            if (Touching(e->getX(), e->getY(), x, y, Pwidth, Pheight))
            {
                takeDamage(3);
                e->kill();
            }
        }
    }

    if (isFLYING) {
        x += velocityX;
        float maxMapWidth = (128 * 3 * 50.0f) - 50.0f;

        if (x <= 50.0f) {
            x = 50.0f;
            P_D = 1;
            velocityX = speed;
        }
        if (x >= maxMapWidth - Pwidth) {
            x = maxMapWidth - Pwidth;
            P_D = -1;
            velocityX = -speed;
        }

        if (aiTimer.getElapsedTime().asSeconds() > 1.5f) {
            shoot();
            aiTimer.restart();
        }

        animate();
        return;
    }

    Entity* P = myManager->getPlayerEntity();
    int playerDir = P_D;
    if (P != nullptr) {
        if (P->getX() < x) playerDir = -1;
        else playerDir = 1;
    }

    if (aiState == 1) {
        velocityX = 0.0f;
        if (onGround) {
            aiState = 2;
            Player_Frame = 0;
            aiTimer.restart();
        }
    }
    else if (aiState == 2) {
        if (playerDir != P_D) {
            aiState = 3;
            Player_Frame = 0;
            P_Frame = 0;
            velocityX = 0.0f;
        }
        else {
            velocityX = speed * (float)P_D;

            if (aiTimer.getElapsedTime().asSeconds() > moveDuration) {
                if (rand() % 2 == 0) { aiState = 4; shoot(); }
                else { aiState = 5; velocityY = -12.0f; }
                Player_Frame = 0;
                velocityX = 0.0f;
            }
        }
    }
    else {
        velocityX = 0.0f;
    }

    animate();

    x += velocityX;

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > terminal_Velocity) velocityY = terminal_Velocity;
    }

    float tempY = y + velocityY;
    int Map_Height = (int)(18 * 1.2f);
    int Map_Width = 128 * 3;
    int feet_row = (int)(tempY + Pheight) / cell_size;
    int left_col = (int)(x) / cell_size;
    int middle_col = (int)(x + Pwidth / 2.0f) / cell_size;
    int right_col = (int)(x + Pwidth) / cell_size;

    if (feet_row >= Map_Height) feet_row = Map_Height - 1;
    if (feet_row < 0) feet_row = 0;

    if (left_col < 0) left_col = 0;
    if (left_col >= Map_Width) left_col = Map_Width - 1;

    if (right_col < 0) right_col = 0;
    if (right_col >= Map_Width) right_col = Map_Width - 1;

    if (middle_col < 0) middle_col = 0;
    if (middle_col >= Map_Width) middle_col = Map_Width - 1;

    char below_left = lvl[feet_row][left_col];
    char below = lvl[feet_row][middle_col];
    char below_right = lvl[feet_row][right_col];

    int head_row = (int)(tempY) / cell_size;
    if (head_row < 0) head_row = 0;
    if (head_row >= Map_Height) head_row = Map_Height - 1;

    onGround = false;
    float platform_top = (float)(feet_row * cell_size);

    if (below_left == '#' || below_right == '#' || below == '#') {
        onGround = true;
        velocityY = 0;
        y = platform_top - Pheight;
    }
    else if (velocityY >= 0 && (below == 'P' || below == 'S')) {
        if (wantToFall) {
            y = tempY + 5.0f;
            onGround = false;
        }
        else
        {
            if (y + Pheight <= platform_top)
            {
                onGround = true;
                velocityY = 0;
                y = platform_top - (float)Pheight;
            }
            else {
                y = tempY;
            }
        }
    }
    else if (velocityY < 0 && (below_left == 'P' || below_right == 'P' || below == 'P' || below_left == 'S' || below_right == 'S' || below == 'S')) {
        y = tempY;
    }
    else {
        y = tempY;
    }

    wantToFall = false;
    canJump = onGround;

    if (x <= 50.0f) { x = 50.0f; P_D = 1; }
    float maxMapWidth2 = (Map_Width * 50.0f) - 50.0f;
    if (x >= maxMapWidth2 - Pwidth)
    {
        x = maxMapWidth2 - Pwidth;
        P_D = -1;
    }
    if (y < 0)
    {
        y = 0;
        velocityY = 0;
    }
    if (y > (18 * 1.2f * 50.0f) + 100.0f) {
        kill();
        return;
    }
}

void Martian::draw(sf::RenderWindow& window, float camX, float camY) {
    if (isDead) return;

    float drawX = (x + Pwidth / 2.0f) - camX;
    float drawY = (y + Pheight / 2.0f) - camY;

    float scaleX = (P_D == 1) ? -1.5f : 1.5f;
    if (aiState == 3) scaleX = (P_D == 1) ? 1.5f : -1.5f;

    sprite.setPosition(drawX, drawY);
    sprite.setScale(scaleX, 1.5f);

    if (isFLYING)
    {
        podSprite.setScale((P_D == 1) ? 0.3f : -0.3f, 0.3f);
        if (P_D == 1)
            drawX -= 170;
        else
            drawX += 170;
        podSprite.setPosition(drawX, drawY + 225.0f);
        window.draw(podSprite);
        window.draw(sprite);
    }
    else
    {
        sprite.setPosition(x - camX, y - camY - 100);
        window.draw(sprite);
    }
}

void Martian::shoot() {
    if (isFLYING) {
        float dropX = x + (Pwidth / 2.0f);
        float dropY = y + Pheight;
        Bullet* alienBomb = new Bullet(dropX, dropY, 0.0f, 10.0f, 3, 1, 90.0f, myManager);
        alienBomb->setEnemyBullet(true);
        myManager->addEntity(alienBomb);
    }
    else {
        float spawnX = (P_D == 1) ? x + Pwidth + 25.0f : x - 25.0f;
        float spawnY = y + (Pheight / 2.0f) + 10.0f;
        float bVx = (P_D == 1) ? 12.0f : -12.0f;
        float angle = (P_D == 1) ? 0.0f : 180.0f;

        Bullet* alienBullet = new Bullet(spawnX, spawnY, bVx, 0.0f, 3, 1, angle, myManager);
        alienBullet->setEnemyBullet(true);
        myManager->addEntity(alienBullet);
    }
}

void intToCharArray(int num, char result[7]) {

    for (int i = 0; i < 6; i++)
        result[i] = '0';
    result[6] = '\0';

    int i = 5;
    if (num == 0)
        return;
    while (num > 0 && i >= 0) {
        result[i] = '0' + (num % 10);
        num /= 10;
        i--;
    }
}

void Drop_Food(float ex, float ey, EntityManager* mgr) {

    int turkeyChance = 10;
    int fruitChance = 15;
    int roll = rand() % 100;

    if (roll < turkeyChance)
        mgr->addEntity(new Turkey(ex, ey, mgr));
    else if (roll < turkeyChance + fruitChance)
        mgr->addEntity(new Fruit(ex, ey, mgr));

}
void PlayerGrenade::update(char** lvl, int cell_size)
{
    if (isDead == true) return;

    if (exploded == false)
    {
        velocityY = velocityY + gravity;
        x = x + velocityX;
        y = y + velocityY;

        int check_col = (int)(x + (width / 2.0f)) / cell_size;
        int check_row = (int)(y + height) / cell_size;

        int Map_Height = (int)(18 * 1.2f);
        int Map_Width = 128 * 3;

        for (int i = 0; i < manager->Get_Current_Count(); i++)
        {
            Entity* e = manager->getEntity(i);

            if (e->getIsDead()) continue;
            if (isEnemyGrenade)
            {
                if (!e->isPlayerVehicle())
                {
                    PlayerSoldier* ps = e->asPlayer();
                    bool isVehicle = e->isPlayerVehicle();
                    if (ps == nullptr && !isVehicle) continue;
                }
            }
            else
            {
                if (!e->isEnemy()) continue;
            }

            bool hit = Check_Touch(
                e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(),
                x, y, width, height,
                1, 1
            );

            if (hit)
            {
                e->takeDamage(20);
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }

        if (check_row >= 0 && check_row < Map_Height && check_col >= 0 && check_col < Map_Width)
        {
            if (lvl[check_row][check_col] == '#' || lvl[check_row][check_col] == 'P' || lvl[check_row][check_col] == 'S')
            {
                exploded = true;
                frame = 0;
                animClock.restart();
                int killCount = 0;
                for (int i = 0; i < manager->Get_Current_Count(); i++)
                {
                    Entity* e = manager->getEntity(i);

                    if (e->getIsDead()) continue;
                    if (isEnemyGrenade)
                    {
                        if (!e->isPlayerVehicle())
                        {
                            PlayerSoldier* ps = e->asPlayer();
                            bool isVehicle = e->isPlayerVehicle();
                            if (ps == nullptr && !isVehicle) continue;
                        }
                    }
                    else
                    {
                        if (!e->isEnemy()) continue;
                    }

                    bool hit = Check_Touch(
                        e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(),
                        x, y, 150, 150,
                        1, 1
                    );

                    if (hit)
                    {
                        e->takeDamage(5);
                        if (e->getIsDead())
                            killCount++;
                    }
                }
                if (killCount == 2 && scorePtr)
                    *scorePtr += 200;
                else if (killCount >= 3 && scorePtr)
                    *scorePtr += 300 + (killCount - 3) * 50;
            }
        }

        if (y > Map_Height * 50 || x < 0 || x > Map_Width * 50) {
            kill();
        }
        return;
    }

    if (animClock.getElapsedTime().asMilliseconds() > 90) {
        frame = frame + 1;
        animClock.restart();
    }

    if (frame >= maxFrames * 2) {
        kill();
    }

    explosionSprite.setTextureRect(sf::IntRect((int)((frame % 3) * frameWidth), 0, frameWidth, frameHeight));
}

void Tara_Grenade::update(char** lvl, int cell_size) {
    if (isDead) return;

    if (!exploded) {
        if (velocityY <= 10)
            velocityY += 0.8f;

        x += velocityX;
        y += velocityY;
        timer += 0.016f;

        Entity* P = manager->getPlayerEntity();
        if (P == nullptr)
        {
            velocityX = 0;
            return;
        }
        PlayerSoldier* player = P->asPlayer();
        float px = 0, py = 0;
        int Dir = 0;
        if (player != nullptr) {
            px = player->getX();
            py = player->getY();
            Dir = player->Get_P_D();
        }

        bool Damage = Check_Touch(px, py, 100, 100, x, y, 100, 100, Dir, dir);
        if (timer >= fuseTime || Damage) {
            exploded = true;
            frame = 0;
            animClock.restart();
            if (Damage == true && player != nullptr) {
                cout << "Damage from Bomb\n";
                player->takeDamage(20);
            }
        }

        for (int i = 0; i < manager->Get_Current_Count(); i++)
        {
            Entity* e = manager->getEntity(i);
            if (e->getIsDead()) continue;
            if (!e->isPlayerVehicle()) continue;

            bool hit = Check_Touch(
                e->getX(), e->getY(), (int)e->getwidth(), (int)e->getheight(),
                x, y, width, height, 1, 1
            );

            if (hit)
            {
                e->takeDamage(20);;
                exploded = true;
                frame = 0;
                animClock.restart();
                return;
            }
        }
        return;
    }

    if (animClock.getElapsedTime().asMilliseconds() > 90) {
        frame++;
        animClock.restart();
    }

    if (frame >= maxFrames * 2) {
        kill();
    }

    explosionSprite.setTextureRect(sf::IntRect((int)((frame % 3) * frameWidth), 0, frameWidth, frameHeight));
}

void LevelSelectState::update(GameStateManager* gsm)
{
    if (inputDelay.getElapsedTime().asSeconds() > 0.2f) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (selectedLevel == 1) { arrowX = 1185; selectedLevel = 4; }
            else if (selectedLevel == 2) { arrowX = 240; selectedLevel = 1; }
            else if (selectedLevel == 3) { arrowX = 555; selectedLevel = 2; }
            else { arrowX = 875; selectedLevel = 3; }
            inputDelay.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (selectedLevel == 1) { arrowX = 555; selectedLevel = 2; }
            else if (selectedLevel == 2) { arrowX = 875; selectedLevel = 3; }
            else if (selectedLevel == 3) { arrowX = 1185; selectedLevel = 4; }
            else { arrowX = 240; selectedLevel = 1; }
            inputDelay.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            if (selectedLevel == 4)
                gsm->changeState(new BossLevelState(chosenChar, selectedMode, 0));
            else
                gsm->changeState(new PlayState(chosenChar, selectedMode, selectedLevel));
            inputDelay.restart();
        }
    }
    arrowSprite.setPosition(arrowX, arrowY);
}




