#pragma once
#include "Enemy.h"
#include "ParaTrooper.h"
#include <SFML/Graphics.hpp>
#include "Collectible.h"
#include "InteractableObject.h"
#include "EnemyVehicle.h"
#include "Vehicle.h"
#include "NoiseProfile.h"
#include "Boss.h"


using namespace sf;

class Enemy;
//class Vehicle;
//class SupplyCrate;

class Level {
 protected:
     int width, height, enemy_count, max_enemies,cell_size;
     Enemy** enemies;
     const char* biome_type;
     char** lvl;
     Collectible** collectibles;
     int collectible_count, max_collectibles;
     InteractableObject** interactables;
     int interactable_count, max_interactables;
     Vehicle** vehicles;
     int  vehicle_count, max_vehicles; // fahius prime
public:
     Texture grass_tex;
     Texture stone_tex;
     Texture bedrock_tex;
     Texture water_tex;
     Texture background_tex;
     Sprite block_sprite,background_sprite;
     
     int getWidth() const {
         return width;
     }

     int getHeight() const {
         return height;
     }

     int getCellSize() const {
         return cell_size;
     }

     int getEnemyCount() const {
         return enemy_count;
     }

     int getMaxEnemies() const {
         return max_enemies;
     }

     int& getEnemyCountRef() {
         return enemy_count;
     }

     Enemy** getEnemies() {
         return enemies;
     }

     int getInteractableCount() const {
         return interactable_count;
     }

     int getMaxInteractables() const {
         return max_interactables;
     }

     InteractableObject** getInteractables() {
         return interactables;
     }

     int getCollectibleCount() const {
         return collectible_count;
     }

     int getMaxCollectibles() const {
         return max_collectibles;
     }

     Collectible** getCollectibles() {
         return collectibles;
     }
     int  getVehicleCount()  const { return vehicle_count; } 
     int   getMaxVehicles()   const { return max_vehicles; }
     Vehicle**getVehicles() { return vehicles; }

     Level(int w, int h, const char* mode) : width(w),height(h),biome_type("plains") {
                
         this->cell_size = 64;

         lvl = new char* [height];
         for (int i = 0; i < height; i += 1)
         {
             lvl[i] = new char[width] {'\0'};
             for (int i1 = 0;i1 < width;i1++) {
                 lvl[i][i1] = ' ';
             }
         }

         for (int i = 0;i < width;i++) {
             lvl[height-1][i] = 'b';
             lvl[height - 2][i] = 'g';
         }

         ////Platforms (For Initial Start)
         //for (int j = 20; j < 30; j++)
         //    lvl[height - 8][j] = 'g';
         //for (int j = 40; j < 50; j++)
         //    lvl[height - 6][j] = 'g';


         max_enemies = 300;
         enemy_count = 0;
         max_collectibles = 100;
         collectible_count = 0;

         collectibles = new Collectible * [max_collectibles];
         for (int i = 0; i < max_collectibles; i++)
             collectibles[i] = nullptr;

         max_interactables = 300;
         interactable_count = 0;

         interactables = new InteractableObject * [max_interactables];
         for (int i = 0; i < max_interactables; i++)
             interactables[i] = nullptr;

         enemies = new Enemy * [max_enemies];
         for (int i = 0; i < max_enemies; i++)
             enemies[i] = 0;

         // vehicles
         max_vehicles = 10;
         vehicle_count = 0;
         vehicles = new Vehicle * [max_vehicles];
         for (int i = 0; i < max_vehicles; i++) {
             vehicles[i] = nullptr;
         }

             //load texture and sprite
         grass_tex.loadFromFile( "Assets/blocks/grass_block_side.png");
         stone_tex.loadFromFile( "Assets/blocks/stone.png");
         bedrock_tex.loadFromFile( "Assets/blocks/bedrock.png");
         water_tex.loadFromFile( "Assets/blocks/water.png");
         background_tex.loadFromFile("Assets/background.png");
         block_sprite.setTexture(grass_tex);
         background_sprite.setTexture(background_tex);
         background_sprite.setScale(3.0f,7.0f);


     }
     virtual ~Level() {
         for (int i = 0; i < height; i++)
             delete[] lvl[i];
         delete[] lvl;

         for (int i = 0; i < max_enemies; i++)
          
             if (enemies[i]) 
             delete enemies[i];
         delete[] enemies;

         for (int i = 0; i < max_interactables; i++) 
            if (interactables[i]) 
               delete interactables[i];
         delete[] interactables;
     }
     void addInteractable(InteractableObject* o) {
       if (!o || interactable_count >= max_interactables)  { 
     delete o;
      return;
         } 
           interactables[interactable_count++] = o;
           }
     void addCollectible(Collectible* c) {
         if (!c || collectible_count >= max_collectibles) {
             delete c;
             return;
         }

         collectibles[collectible_count++] = c;
     }
     void buildSurvivalLevel(int level_num) {
     if(level_num > 4)
       level_num = 4;
         //Build All Of The Levels
         for (int i = 0; i < height; i += 1)
         {
             for (int i1 = 0;i1 < width;i1++) {
                 lvl[i][i1] = ' ';
             }
         }
       
             for (int i1 = 0;i1 < width;i1++) {
                 lvl[height-1][i1] = 'b';
             }

             for (int i1 = 0;i1 < width;i1++) {
                 lvl[height - 2][i1] = 'g';
                 lvl[height - 3][i1] = 'g';
             }

             //Biomes

              int plain_end = width /3; //3 quarters
              int aerial_end = 2*(width/3); //ahead of the initial 3 quarters
              int water_end = width-5; //end of the level..the last 3 quarters
              int water_start = aerial_end + 5;
              int plain_start = 0;
              float plain_px_start = plain_start * (float)cell_size;
              float plain_px_end = plain_end * (float)cell_size;
              float aerial_px_start = plain_end * (float)cell_size;
              float aerial_px_end = aerial_end * (float)cell_size;
              float water_px_start = water_start * (float)cell_size;
              float water_px_end = water_end * (float)cell_size;


              //Plains
              int row_lo = height - 6;
              int row_hi = height - 9;

              if (row_lo > 0 && row_hi > 0) {
                  for (int j = 8; j < 14 && j < plain_end; j++) {
                      if (lvl[row_lo][j] == ' ') {
                          lvl[row_lo][j] = 'g';
                      }
                  }
                  for (int j = 24; j < 36 && j < plain_end; j++) {
                      if (lvl[row_lo][j] == ' ') {
                          lvl[row_lo][j] = 'g';
                      }
                  }

                  for (int j = 0; j < 8 && j < plain_end; j++) {
                      if (lvl[row_hi][j] == ' ') {
                          lvl[row_hi][j] = 'g';
                      }
                  }

                  for (int j = 16; j < 24 && j < plain_end; j++) {
                      if (lvl[row_hi][j] == ' ') {
                          lvl[row_hi][j] = 'g';
                      }
                  }
                  for (int j = 35;j < 48 && j < plain_end;j++) {
                      if (lvl[row_lo][j] == ' ') {
                          lvl[row_lo][j] = 'g';
                      }
                  }
                  for (int j = 48; j < plain_end; j++) {
                      if (lvl[row_hi][j] == ' ') {
                          lvl[row_hi][j] = 'g';
                      }
                  }
              }
              //Aerial
              int aerial_width = aerial_end - plain_end;
              int* air_h = new int[aerial_width + 2];
              int cur_h = height - 3; //Start At Ground


              for (int i = 0; i < aerial_width; i++) {
                  int change = (rand() % 5) - 3;
                  cur_h += change;
                  if (cur_h < height - 13) {
                      cur_h = height - 13;
                  }
                  if (cur_h > height - 1) {
                      cur_h = height - 1;
                  }
                  air_h[i] = cur_h;
              }
              for (int i = 1; i < aerial_width - 1; i++) {

                  int difference_r = air_h[i] - air_h[i + 1];
                  int difference_l = air_h[i] - air_h[i - 1];

                  if (difference_r <= -1 && difference_l <= -1) {
                      air_h[i] += 1;
                  }
              }
              for (int i = 0; i < aerial_width; i++) {
                  int j = plain_end + 1 + i;

                  int top = air_h[i];
                  if (j < 0 || j >= width) continue;

                  for (int k = top; k < height - 1; k++) {
                      if (lvl[k][j] == ' ')
                          lvl[k][j] = (k == top) ? 'g' : 's';
                  }

              }
              delete[] air_h;
              
              //Water
              for (int j = water_start; j < water_end; j++){           
                  lvl[height - 2][j] = 'w';
                  lvl[height - 3][j] = 'w';
              }
              for (int j = aerial_end; j < water_start; j++) {
                  lvl[height - 4][j] = 'g';
              }
              for (int j = water_end; j < width; j++){
                  lvl[height - 4][j] = 'g';
              }
              int sea_surface = height - 8;  
              for (int j = water_start; j < water_end; j++) {
                  for (int r = sea_surface; r < height - 1; r++) {
                      if (lvl[r][j] == ' ')    
                          lvl[r][j] = 'w';
                  }
              }

              int row_above = height - 9;
            

              if (row_above > 0) {
                  for (int j = water_start + 2; j < water_start + 10 && j < water_end; j++) {
                      if (lvl[row_above][j] == ' ') {
                          lvl[row_above][j] = 'g';
                      }
                  }

                  for (int j = water_start + 18; j < water_start + 26 && j < water_end; j++) {
                      if (lvl[row_above][j] == ' ') {
                          lvl[row_above][j] = 'g';
                      }
                  }
              }


              clearEnemies(); clearVehicles(); clearInteractables();

        //Batches
        int infantry_batch = (level_num == 1 )?2:3;
        int batches_per_biome = infantry_batch;
        int undead_batches = (level_num == 1) ? 2 : (level_num == 2) ? 3 : 4;
        int zombie_batches = (level_num == 1) ? 2 : (level_num == 2) ? 3 : 4;
        int para_batch = level_num;
        int alien_batches = level_num;
        float ground_surface_y = (float)((height - 2) * cell_size);
        float spawn_above_y = ground_surface_y - 96.0f;
        //Spawning Enemies In Biomes

        //OLD SPAWNING
       /* for (int batch = 0;batch < infantry_batch;batch++) {
          int batch_num =2 + rand() % 3;
           for(int i1 = 0;i1 < batch_num && enemy_count < max_enemies;i1++){
               float ex = plain_start * cell_size + 300.0f + batch * 400.0f + i1 * 80.0f;
               enemies[enemy_count++] = new RebelSoldier(ex, spawn_above_y);
           }                
         }
        for (int i1 = 0;i1 < infantry_batch &&  enemy_count < max_enemies;i1++) {
            float ex = plain_start * cell_size + 500.0f + i1 * 500.0f;
            enemies[enemy_count++] = new ShieldedSoldier(ex, spawn_above_y);
        }
        for (int i1 = 0;i1 < infantry_batch && enemy_count < max_enemies;i1++) {
            float ex = plain_start * cell_size + 600.0f + i1 * 600.0f;
            enemies[enemy_count++] = new BazookaSoldier(ex, spawn_above_y);
        }
        for (int i1 = 0;i1 < infantry_batch && enemy_count < max_enemies;i1++) {
            float ex = plain_start * cell_size + 700.0f + i1 * 600.0f;
            enemies[enemy_count++] = new GrenadeSoldier(ex, spawn_above_y);
        }*/

        float plain_zone = (plain_px_end - plain_px_start) / 4.0f;
        float aerial_zone = (aerial_px_end - aerial_px_start) / 4.0f;
        float water_zone = (water_px_end - water_px_start) / 4.0f;

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 2 + rand() % 3;
            float span = plain_zone / (batches_per_biome + 1);
            float bx_base = plain_px_start + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new RebelSoldier(ex * 3.45f, spawn_above_y);
            }
        }
        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = plain_zone / (batches_per_biome + 1);
            float bx_base = (plain_px_start + plain_zone) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new ShieldedSoldier(ex, spawn_above_y);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = plain_zone / (batches_per_biome + 1);
            float bx_base = (plain_px_start + plain_zone * 2.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new BazookaSoldier(ex, spawn_above_y);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = plain_zone / (batches_per_biome + 1);
            float bx_base = (plain_px_start + plain_zone * 3.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new GrenadeSoldier(ex, spawn_above_y);
            }
        }
        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 2 + rand() % 3;
            float span = aerial_zone / (batches_per_biome + 1);
            float bx_base = aerial_px_start + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new RebelSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = aerial_zone / (batches_per_biome + 1);
            float bx_base = (aerial_px_start + aerial_zone) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new ShieldedSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = aerial_zone / (batches_per_biome + 1);
            float bx_base = (aerial_px_start + aerial_zone * 2.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new BazookaSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = aerial_zone / (batches_per_biome + 1);
            float bx_base = (aerial_px_start + aerial_zone * 3.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new GrenadeSoldier(ex,100.0f);
            }
        }
        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 2 + rand() % 3;
            float span = water_zone / (batches_per_biome + 1);
            float bx_base = water_px_start + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new RebelSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = water_zone / (batches_per_biome + 1);
            float bx_base = (water_px_start + water_zone) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new ShieldedSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = water_zone / (batches_per_biome + 1);
            float bx_base = (water_px_start + water_zone * 2.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new BazookaSoldier(ex, 100.0f);
            }
        }

        for (int batch = 0; batch < batches_per_biome && enemy_count < max_enemies; batch++) {
            int batch_num = 1 + rand() % 2;
            float span = water_zone / (batches_per_biome + 1);
            float bx_base = (water_px_start + water_zone * 3.0f) + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new GrenadeSoldier(ex,100.0f);
            }
        }
        for (int batch = 0; batch < undead_batches && enemy_count < max_enemies; batch++) {
            float span = (plain_px_end - plain_px_start) / (undead_batches + 1);
            float bx_base = plain_px_start + span * (batch + 1);

            enemies[enemy_count++] = new MummyWarrior(bx_base,spawn_above_y);
        }
        for (int batch = 0; batch < zombie_batches && enemy_count < max_enemies; batch++) {
            int batch_num = 3 + rand() % 3;
            float span = (water_px_end - water_px_start) / (zombie_batches + 1);
            float bx_base = water_px_start + span * (batch + 1);

            for (int i1 = 0; i1 < batch_num && enemy_count < max_enemies; i1++) {
                float ex = bx_base + i1 * 220.0f;
                enemies[enemy_count++] = new Zombie(ex, 100.0f);
            }
        }
        for (int batch = 0; batch < para_batch && enemy_count < max_enemies; batch++) {
            float span = (aerial_px_end - aerial_px_start) / (para_batch + 1);
            float bx_base = aerial_px_start + span * (batch + 1);
            int r = rand() % 4;
            if (r == 0)      enemies[enemy_count++] = new Paratrooper(bx_base, 0.0f);
            else if (r == 1) enemies[enemy_count++] = new ShieldedParatrooper(bx_base, 0.0f);
            else if (r == 2) enemies[enemy_count++] = new BazookaParatrooper(bx_base, 0.0f);
            else             enemies[enemy_count++] = new GrenadeParatrooper(bx_base, 0.0f);
        }
        for (int batch = 0; batch < alien_batches && enemy_count < max_enemies; batch++) {
            float span = (aerial_px_end - aerial_px_start) / (alien_batches + 1);
            float bx_base = aerial_px_start + span * (batch + 1);
            float ey = (float)((height - 18) * cell_size);

            enemies[enemy_count++] = new Martian(bx_base, ey);
        }

        // Spawn Metal Slug vehicle in plains//plain_px_end * 0.35f
        if (level_num >= 1 && (vehicle_count < max_vehicles))
            vehicles[vehicle_count++] = new MetalSlug(plain_px_end * 0.5f, spawn_above_y);
        if (vehicle_count < max_vehicles) {
            vehicles[vehicle_count++] = new SlugFlyer( aerial_px_start + (aerial_px_end - aerial_px_start) * 0.5f, (float)((height - 16) * cell_size) );
        }
        if (vehicle_count < max_vehicles) {
            vehicles[vehicle_count++] = new SlugMariner(water_px_start + (water_px_end - water_px_start) * 0.5f, (float)((sea_surface + 1) * cell_size) );
        }
        if (vehicle_count < max_vehicles)vehicles[vehicle_count++] = new AmphibiousSlug(aerial_px_start + (aerial_px_end - aerial_px_start) * 0.3f, (float)((height - 16) * cell_size));


        int ft_b = level_num;
        for (int b = 0; b < ft_b && enemy_count < max_enemies; b++) {
            float bx = aerial_px_start + (aerial_px_end - aerial_px_start) / (ft_b + 1) * (b + 1);
            enemies[enemy_count++] = new FlyingTara(bx, (float)((height - 14) * cell_size));
            if (enemy_count < max_enemies)
                enemies[enemy_count++] = new FlyingTara(bx + 200, (float)((height - 14) * cell_size));
        }
        if (level_num >= 2) {
            if (level_num >= 2) {
                if (enemy_count < max_enemies)
                    enemies[enemy_count++] = new M15ABradley(plain_px_end * 0.4f, spawn_above_y);
                if (enemy_count < max_enemies)
                    enemies[enemy_count++] = new M15ABradley(plain_px_end * 0.8f, spawn_above_y);
            }
        }
        // Enemy Sub in aquatic zone
        int sub_count = (level_num >= 3) ? 2 : 1;
        for (int b = 0; b < sub_count && enemy_count < max_enemies; b++) {
            float bx = water_px_start + (water_px_end - water_px_start) / (sub_count + 1) * (b + 1);
            enemies[enemy_count++] = new EnemySub(bx, (float)((sea_surface+5) * cell_size));
        }
        if (interactable_count < max_interactables - 4) {
            interactables[interactable_count++] = new SupplyCrate(plain_px_end * 0.5f, spawn_above_y);
            float aerial_cx = aerial_px_start + (aerial_px_end - aerial_px_start) * 0.5f;
            int aerial_col = (int)(aerial_cx / cell_size);
            if (aerial_col >= width) aerial_col = width - 1;
            float aerial_y = spawn_above_y;
            for (int r = 0; r < height; r++) {
                if (lvl[r][aerial_col] == 'g' || lvl[r][aerial_col] == 's') {
                    aerial_y = (float)((r - 1) * cell_size);
                    break;
                }
            }
            interactables[interactable_count++] = new SupplyCrate(aerial_cx, aerial_y);
            float water_cx = water_px_start + (water_px_end - water_px_start) * 0.5f;
            int sea_row_crate = height - 8;
            float water_y = (float)((sea_row_crate - 2) * cell_size);
            interactables[interactable_count++] = new SupplyCrate(water_cx, water_y);
        }

        if (level_num >= 2 && interactable_count < max_interactables)
            interactables[interactable_count++] = new SupplyCrate(plain_px_end * 0.85f,spawn_above_y);
        if (level_num >= 3 && interactable_count < max_interactables) 
            interactables[interactable_count++] = new SupplyCrate(water_px_start + 300.0f, 100.0f);

        int pow_pb = (level_num >= 2) ? 2 : 1;
        for (int b = 0;b < pow_pb && interactable_count < max_interactables;b++) {
            float px = plain_px_start + (plain_px_end - plain_px_start) / (float)(pow_pb + 1) * (float)(b + 1);
            interactables[interactable_count++] = new POWPrisoner(px, 100.0f - 52.0f);
        }
        if (interactable_count < max_interactables) 
           interactables[interactable_count++] = new POWPrisoner(aerial_px_start+ (aerial_px_end - aerial_px_start) * 0.5f, 140.0f);
        if (interactable_count < max_interactables)
          interactables[interactable_count++] = new POWPrisoner((water_px_start + water_px_end) * 0.5f, 100.0f);


     }
     void loadSurvivalLevel(int level_num) {
        //Load All Of The Levels
        //Already Done With Build Survival Level At The Start
        //Run This Only For Debugging And Checking terrain (for muttal)
     }
     void buildCampaignLevel(NoiseProfile* profile) {
         for (int i = 0; i < height; i++) {
             for (int j = 0; j < width; j++) {
                 lvl[i][j] = ' ';
             }
         }

         for (int j = 0; j < width; j++) {
             lvl[height - 1][j] = 'b';
         }

         int sea_row = (int)(height * 0.72f);
         int deep_row = (int)(height * 0.85f);

         int* surf = new int[width];

         for (int j = 0; j < width; j++) {
             surf[j] = profile->getSurfaceRow((float)j, height);
         }

         for (int j = 0; j < width; j++) {
             int s = surf[j];

             if (s > sea_row) {
                 for (int r = sea_row; r < s && r < height - 1; r++) {
                     lvl[r][j] = 'w';
                 }

                 for (int r = s; r < height - 1; r++) {
                     lvl[r][j] = (r == height - 2) ? 'g' : 's';
                 }
             }
             else if (s > (int)(height * 0.50f)) {
                 lvl[s][j] = 'g';

                 for (int r = s + 1; r < height - 1; r++) {
                     lvl[r][j] = 's';
                 }
             }
             else {
                 lvl[s][j] = 'g';

                 for (int r = s + 1; r < height - 1; r++) {
                     lvl[r][j] = 's';
                 }
             }
         }

         for (int j = 0; j < width; j++) {
             if (surf[j] > sea_row) {
                 for (int r = 1; r < sea_row; r++) {
                     if (lvl[r][j] == ' ') {
                         lvl[r][j] = ' ';
                     }
                 }

                 if (lvl[sea_row][j] == ' ') {
                     lvl[sea_row][j] = 'w';
                 }
             }
         }

         delete[] surf;

         clearEnemies();
         clearVehicles();
         clearInteractables();


         float ground_y = (float)((height - 2) * cell_size);

         int spawn_interval = 300;

         for (int j = spawn_interval; j < width * cell_size; j += spawn_interval) {
             if (enemy_count >= max_enemies) {
                 break;
             }

             int col = j / cell_size;

             int row_spawn = height - 2;

             for (int r = 0; r < height - 1; r++) {
                 if (lvl[r][col] == 'g' || lvl[r][col] == 's') {
                     row_spawn = r;
                     break;
                 }
             }

             float ey = (float)((row_spawn - 1) * cell_size);

             bool underwater = (lvl[row_spawn][col] == 'w');

             int type = rand() % 8;

             if (type == 0) {
                 enemies[enemy_count++] = new RebelSoldier((float)j, ey);
             }
             else if (type == 1) {
                 enemies[enemy_count++] = new ShieldedSoldier((float)j, ey);
             }
             else if (type == 2) {
                 enemies[enemy_count++] = new BazookaSoldier((float)j, ey);
             }
             else if (type == 3) {
                 enemies[enemy_count++] = new GrenadeSoldier((float)j, ey);
             }
             else if (type == 4) {
                 enemies[enemy_count++] = new Paratrooper((float)j, 0.f);
             }
             else if (type == 5) {
                 enemies[enemy_count++] = new MummyWarrior((float)j, ey);
             }
             else if (type == 6) {
                 enemies[enemy_count++] = new Zombie((float)j, ey);
             }
             else {
                 enemies[enemy_count++] = new Martian((float)j, ey);
             }
         }

         for (int j = 2000; j < width * cell_size; j += 2000) {
             if (enemy_count >= max_enemies) {
                 break;
             }

             int col = j / cell_size;

             bool under = (col < width && lvl[(int)(height * 0.72f)][col] == 'w');

             float ey = (float)((height - 2) * cell_size);

             if (!under) {
                 if (rand() % 2 == 0 && enemy_count < max_enemies) {
                     enemies[enemy_count++] = new FlyingTara((float)j, ey - 300.f);
                 }
                 else if (enemy_count < max_enemies) {
                     enemies[enemy_count++] = new M15ABradley((float)j, ey);
                 }
             }
             else if (enemy_count < max_enemies) {
                 enemies[enemy_count++] = new EnemySub( (float)j,(float)((int)(height * 0.72f) * cell_size)
                 );
             }
         }

         for (int i = 0; i < max_interactables; i++) {
             if (interactables[i]) {
                 delete interactables[i];
                 interactables[i] = nullptr;
             }
         }

         interactable_count = 0;

         for (int j = 1500; j < width * cell_size; j += 1500) {
             if (interactable_count >= max_interactables) {
                 break;
             }

             int col = j / cell_size;

             if (col >= width) {
                 break;
             }

             int row_s = height - 2;

             for (int r = 0; r < height - 1; r++) {
                 if (lvl[r][col] == 'g' || lvl[r][col] == 's') {
                     row_s = r;
                     break;
                 }
             }

             float cy = (float)((row_s - 1) * cell_size);

             interactables[interactable_count++] = new SupplyCrate((float)j, cy);
         }

         for (int j = 3000; j < width * cell_size; j += 3000) {
             if (interactable_count >= max_interactables) {
                 break;
             }

             int col = j / cell_size;

             if (col >= width) {
                 break;
             }

             int row_s = height - 2;

             for (int r = 0; r < height - 1; r++) {
                 if (lvl[r][col] == 'g' || lvl[r][col] == 's') {
                     row_s = r;
                     break;
                 }
             }

             float py = (float)((row_s - 2) * cell_size);

             interactables[interactable_count++] = new POWPrisoner((float)j, py);
         }

         for (int i = 0; i < max_vehicles; i++) {
             if (vehicles[i]) {
                 delete vehicles[i];
                 vehicles[i] = nullptr;
             }
         }

         vehicle_count = 0;

         int vc = 10;
         int vr = height - 2;

         for (int r = 0; r < height - 1; r++) {
             if (lvl[r][vc] == 'g') {
                 vr = r;
                 break;
             }
         }

         vehicles[vehicle_count++] = new MetalSlug(  (float)(vc * cell_size), (float)((vr - 1) * cell_size)
         );
     }
     void buildBossLevel() {
     //Boss Level
         for (int i = 0; i < height; i++)
             for (int j = 0; j < width; j++)
                 lvl[i][j] = ' ';
         for (int j = 0; j < width; j++)
             lvl[height - 1][j] = 'b';

         int plains_end = width / 3;
         int aerial_end = (2 * width) / 3;
         int water_start = aerial_end + 4;

         for (int j = 0; j < plains_end; j++) {
             lvl[height - 2][j] = 'g';
             lvl[height - 3][j] = 'g';
         }
         int cur_h = height - 3;
         for (int j = plains_end; j < aerial_end; j++) {
             int change = (rand() % 5) - 2;
             cur_h += change;
             if (cur_h < height - 16) 
                cur_h = height - 16;
             if (cur_h > height - 3)  
                cur_h = height - 3;
             lvl[cur_h][j] = 'g';
             for (int r = cur_h + 1; r < height - 1; r++)
                 lvl[r][j] = 's';
         }
         for (int j = aerial_end; j < water_start; j++) {
             lvl[height - 2][j] = 'g';
             lvl[height - 3][j] = 'g';
         }
         int sea_surface = height - 8;
         for (int j = water_start; j < width - 3; j++) {
             lvl[height - 2][j] = 'w';
             for (int r = sea_surface; r < height - 1; r++)
                 if (lvl[r][j] == ' ')
                     lvl[r][j] = 'w';
         }
         for (int j = width - 3; j < width; j++) {
             lvl[height - 2][j] = 'g';
             lvl[height - 3][j] = 'g';
         }
         clearEnemies();   clearVehicles();  clearInteractables();

         float plains_ground_y = (float)((height - 4) * cell_size);

         float boss_x = (float)(plains_end / 2) * (float)cell_size;

         if(enemy_count < max_enemies)
         enemies[enemy_count++] = new IronNokana(boss_x, plains_ground_y);
         if (vehicle_count < max_vehicles) {
             vehicles[vehicle_count++] = new AmphibiousSlug((float)(200.0f), plains_ground_y - 64.0f);
         }
         if (interactable_count < max_interactables)
             interactables[interactable_count++] = new SupplyCrate( 200.0f, plains_ground_y - 64.0f );

     }
     void addVehicle(Vehicle* v) {
         if (!v || vehicle_count >= max_vehicles)
         {
             delete v; return;

         }
         vehicles[vehicle_count++] = v;

     }   //vehicle
     void clearEnemies() {
         for (int i = 0; i < max_enemies; i++) {
             if (enemies[i]) { delete enemies[i]; enemies[i] = nullptr; }
         }
         enemy_count = 0;
     }
     void clearVehicles() {
         for (int i = 0; i < max_vehicles; i++) {
             if (vehicles[i]) { delete vehicles[i]; vehicles[i] = nullptr; }
         }
         vehicle_count = 0;
     }
     void clearInteractables() {
         for (int i = 0; i < max_interactables; i++) {
             if (interactables[i]) { delete interactables[i]; interactables[i] = nullptr; }
         }
         interactable_count = 0;
     }

     void render(RenderWindow& window, float cam_x, float cam_y) {
         //Background Reendering
         float background_size = (float)(background_tex.getSize().x * 3.0f);
         float level_w = (float)(width * cell_size);
         float level_h = (float)(height * cell_size);
         float screen_w = 1600.0f;
         float screen_h = 900.0f;

         float max_cam_x = level_w - screen_w;
         float max_cam_y = level_h - screen_h;
         float max_cam_scrolls = background_size - screen_w;

         float base_offset_y = 190.0f;

         float parallax_x = (max_cam_x > 0.0f && max_cam_scrolls > 0.0f) ? (max_cam_scrolls / max_cam_x) : 0.5f;
         float max_cam_scrolls_y = background_size - screen_h;


         float min_cam = 500.0f;
         float max_cam = 3000.0f;

         float t = (max_cam_y - min_cam) / (max_cam - min_cam);

         
         t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

        
         float parallax_y = 1.2f - t * 0.9f;

         background_sprite.setPosition(-cam_x * parallax_x, base_offset_y-cam_y * parallax_y);
         window.draw(background_sprite);



         const int cell_size = 64;
          int start_x = (int)(cam_x/cell_size);
          int end_x = start_x + (1600 / cell_size);
          int start_y = (int)(cam_y / cell_size);
          int end_y = start_y + (900 / cell_size);;

          if(end_x > width)
            end_x = width;
            if(end_y > height)
             end_y = height;
             if(start_x < 0)
             start_x = 0;
             if(start_y < 0)
             start_y = 0;

             for (int i = start_y; i < end_y; i += 1)
             {
                 for (int j = start_x; j < end_x; j += 1)
                 {
                     if (lvl[i][j] == ' ')
                     {
                         continue;
                     }
                     if (lvl[i][j] == 'g')
                         block_sprite.setTexture(grass_tex);
                     else if (lvl[i][j] == 's')
                         block_sprite.setTexture(stone_tex);
                     else if (lvl[i][j] == 'b')
                         block_sprite.setTexture(bedrock_tex);
                     else if (lvl[i][j] == 'w')
                         block_sprite.setTexture(water_tex);
                     else
                         continue;

                     block_sprite.setPosition((float)(j * cell_size) - cam_x, (float)(i * cell_size) - cam_y);

                     window.draw(block_sprite);
                 }
             }

             for (int i = 0;i < max_interactables;i++)
                 if (interactables[i] && interactables[i]->getIsActive())
                     interactables[i]->render(window, cam_x, cam_y);
     }
     char getBlock(int row, int col) {
         if (row < 0 || row >= height || col < 0 || col >= width) {
              return ' ';
         }
        return lvl[row][col];
     }
     void destroyBlock(int row, int col) {
         if (row >= 0 && row < height && col >= 0 && col < width) {
             if (lvl[row][col] == 'g' || lvl[row][col] == 's'){
                 lvl[row][col] = ' ';
              }
         }
     } 
     void checkCraterWater(int row, int col) {
         if (getBlock(row, col) != ' ') {
             return;
          }
         if (row < height - 4) {
             return;
          }
          //measuring width
          int left = col;
          int right = col;


          for (;getBlock(row, left - 1) == ' ' && left > 0;) {
              left--;
          }
          for (;getBlock(row, right + 1) == ' ' && right < width-1;) {
              right++;
          }

          int c_width = right-left+1;

          //measure depth

          char below = getBlock(row + 1, col);
          bool has_floor = (below == 'g' || below == 's' || below == 'b' || below == 'w');

          if (c_width <= 3 && has_floor) {
              for (int i1 = left; i1 <= right; i1++)
                  lvl[row][i1] = 'w';
          }
     }

};