#include "VoiceBeneGesserit.h"
#include "Character.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Level.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameStateManager.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "RainSystem.h"
#include "Scoring_Bonuses.h"
#include "DevMode.h"
#include "GrudgeRegistry.h"
#include "CowardiceReserve.h"
#include "EchoChambers.h"
#include "Collectible.h"
#include "Boss.h"


using namespace std;
using namespace sf;

class Character;
class Level;
class Projectile;
class Weapon;
class GameStateManager;

class Game {
	private:
		RenderWindow window;
		Character* characters[4];
		Level* current_level;
		Projectile** projectiles;
		Vehicle* active_vehicle;
		int active_char_index,num_characters,projectile_count,max_projectiles,gamestate,current_level_num;; // Gamestate: 0=menu, 1=campaign, 2=playing, 3=gameover, 4=win for each gamestate
		bool dev_mode_active; //devmode on for true and off for false
		bool b_was = false;
		int game_mode;
		float cam_x,cam_y, switch_cooldown;
		float level_select_cooldown;
		float menu_input_cooldown,menu_input_cooldown_1;
		float pause_input_cooldown;
		float enter_exit_vehicle_cooldown;
		int campaign_kills[8];
		bool campaign_complete;
		int boss_phase;
		bool boss_minion_batch_active;
		int boss_minion_batch_size;
		float boss_next_batch_timer;
		bool boss_phase_1_done;
		bool boss_phase_2_done;
		bool boss_phase_3_done;
		bool boss_phase_4_done;
		bool voice_startup_done;
		bool terminal_open;
		char terminal_buf[64];
		int  terminal_len;
		bool term_key_prev[101];

		NoiseProfile* noise_profile;
		GameStateManager state_manager;
		EntityManager    entity_manager;
		LevelManager     level_manager;
		//Scoring
		Scoring* score;
		//Time
		Clock frame_clock;
		float dt;
		//Special Features And Functions (To Be Added)
		DevMode* dev_mode;
		//TimelineManager* timeline;
		RainSystem* rain;
		EchoChamber* echo;
		GrudgeRegistry* grudge;
		CowardiceReserve* cowardice;
		VoiceBeneGesserit* voice;
		FusionCompanion* fusion_companion;
		//------------------------------

		Texture bullet_tex;
		Texture grenade_tex;
		Texture enemy_bullet_tex;

		// HUD
		Font font;
		Text hp_text;
		Text score_text;
		Text lives_text;
		Text ammo_text;
		Text grenade_text;
		Text mode_text;
		Text menu_title;
		Text menu_survival;
		Text menu_campaign;
		Text game_over_text;
		Text bonus_text;
		Text  pause_text, pause_resume, pause_quit;
		Text grudge_text_hud;
		Text cowardice_text_hud;
		Text boss_hud_ground;
		Text boss_hud_aerial;
		Text boss_hud_aquatic;
		Text boss_phase_text;
		Text terminal_text;

		//Damage
		Texture red_overlay_tex;
		Sprite red_overlay_sprite;
		Texture menu_bg;
		Sprite menu_bg_sprite;
		Texture level_select_bg;
		Sprite level_bg_sprite;
		Texture noise_select_bg;
		Sprite noise_bg_sprite;
		//Music
		Music bgm;
		SoundBuffer ok_soundbuffer;
		Sound ok;



		//Boss Functions Start
		void spawnBossMinions(IronNokana* boss, Character* player) {
			int batch = 2 + rand() % 3;
			float bx = boss->getPosX();
			float by = boss->getPosY();
			float dir = (player->getPosX() < bx) ? -1.0f : 1.0f;

			Enemy** enemies= current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();
			for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				float ex = bx + dir * (float)(100 + i * 90);

				int type = rand() % 4;

				if (type == 0) {
					enemies[enemy_count++] = new RebelSoldier(ex, by);
				}
				else if (type == 1) {
					enemies[enemy_count++] = new ShieldedSoldier(ex, by);
				}
				else if (type == 2) {
					enemies[enemy_count++] = new BazookaSoldier(ex, by);
				}
				else {
					enemies[enemy_count++] = new GrenadeSoldier(ex, by);
				}
			}

			boss_minion_batch_size = batch;
			boss_minion_batch_active = true;
		}
		void spawnUltimateBossMinions(UltimateBoss* boss, Character* player) {
			int batch = 2 + rand() % 3;
			float bx = boss->getPosX();
			float by = boss->getPosY();
			float dir = (player->getPosX() < bx) ? -1.0f : 1.0f;

		 if(boss->getCurrentState() == 0){

			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();
			for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				float ex = bx + dir * (float)(100 + i * 90);

				int type = rand() % 4;

				if (type == 0) {
					enemies[enemy_count++] = new RebelSoldier(ex, by);
				}
				else if (type == 1) {
					enemies[enemy_count++] = new ShieldedSoldier(ex, by);
				}
				else if (type == 2) {
					enemies[enemy_count++] = new BazookaSoldier(ex, by);
				}
				else {
					enemies[enemy_count++] = new GrenadeSoldier(ex, by);
				}
			}
			}
		 else if (boss->getCurrentState() == 1) {
			 Enemy** enemies = current_level->getEnemies();
			 int& enemy_count = current_level->getEnemyCountRef();
			 for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				 float ex = bx + (float)(i * 220) - (float)(batch * 110);

				 int type = rand() % 4;

				 if (type == 0) {
					 enemies[enemy_count++] = new Paratrooper(ex, 0.0f);
				 }
				 else if (type == 1) {
					 enemies[enemy_count++] = new ShieldedParatrooper(ex, 0.0f);
				 }
				 else if (type == 2) {
					 enemies[enemy_count++] = new BazookaParatrooper(ex, 0.0f);
				 }
				 else {
					 enemies[enemy_count++] = new GrenadeParatrooper(ex, 0.0f);
				 }
			 }
		 }
		 else {
			 Enemy** enemies = current_level->getEnemies();
			 int& enemy_count = current_level->getEnemyCountRef();
			 for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				 float ex = bx + (float)(i * 220) - (float)(batch * 110);

				 int type = rand() % 4;

				 if (type == 0) {
					 enemies[enemy_count++] = new Paratrooper(ex, 0.0f);
				 }
				 else if (type == 1) {
					 enemies[enemy_count++] = new ShieldedParatrooper(ex, 0.0f);
				 }
				 else if (type == 2) {
					 enemies[enemy_count++] = new BazookaParatrooper(ex, 0.0f);
				 }
				 else {
					 enemies[enemy_count++] = new GrenadeParatrooper(ex, 0.0f);
				 }
			 }

		 }
			boss_minion_batch_size = batch;
			boss_minion_batch_active = true;
		}
		void spawnBoss2Minions(HairbusterRiberts* boss, Character* player) {
			int batch = 2 + rand() % 3;
			float bx = boss->getPosX();
			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();
			for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				float ex = bx + (float)(i * 220) - (float)(batch * 110);

				int type = rand() % 4;

				if (type == 0) {
					enemies[enemy_count++] = new Paratrooper(ex, 0.0f);
				}
				else if (type == 1) {
					enemies[enemy_count++] = new ShieldedParatrooper(ex, 0.0f);
				}
				else if (type == 2) {
					enemies[enemy_count++] = new BazookaParatrooper(ex, 0.0f);
				}
				else {
					enemies[enemy_count++] = new GrenadeParatrooper(ex, 0.0f);
				}
			}

			boss_minion_batch_size = batch;
			boss_minion_batch_active = true;
		}
		void spawnBoss3Minions(SeaSatan* boss, Character* player) {
			int h = current_level->getHeight();
			int cs = current_level->getCellSize();
			float wy = (float)((h - 8 + 2) * cs);

			int batch = 2 + rand() % 3;
			float bx = boss->getPosX();

			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();

			for (int i = 0; i < batch && enemy_count < current_level->getMaxEnemies(); i++) {
				float ex = bx + (float)(i * 220) - (float)(batch * 110);

				int type = rand() % 3;

				if (type == 0) {
					enemies[enemy_count++] = new Zombie(ex, wy);
				}
				else if (type == 1) {
					enemies[enemy_count++] = new EnemySub(ex, wy);
				}
				else {
					enemies[enemy_count++] = new Zombie(ex, wy - 30.0f);
				}
			}

			boss_minion_batch_size = batch;
			boss_minion_batch_active = true;

		}
		int countLiveMinions() {
			int count = 0;
			for (int i = 0; i < current_level->getMaxEnemies(); i++) {
				Enemy* e = current_level->getEnemies()[i];
				if (!e || !e->isAlive()) continue;
				int tid = e->getEnemyTypeID();
				if (tid != 8 && tid != 9 && tid != 10)
					count++;
			}
			return count;
		}
		void spawnPhase2Boss() {
			int w = current_level->getWidth();
			int cs = current_level->getCellSize();
			int plains_end = w / 3;
			int aerial_end = (2 * w) / 3;
			int aerial_mid = (plains_end + aerial_end) / 2;


			float boss_x = (float)(aerial_mid * cs);
			float boss_y = (float)(4 * cs);
			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();

			if (enemy_count < current_level->getMaxEnemies())
				enemies[enemy_count++] = new HairbusterRiberts(boss_x, boss_y);

		}
		void spawnPhase3Boss() {
			int w = current_level->getWidth();
			int h = current_level->getHeight();
			int cs = current_level->getCellSize();

			int aerial_end = (2 * w) / 3;
			int water_start = aerial_end + 4;
			int water_end = w - 3;
			int water_mid = (water_start + water_end) / 2;

			int sea_surface = h - 8;

			float boss_x = (float)(water_mid * cs);
			float boss_y = (float)((sea_surface + 2) * cs); //submerged
			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();

			if (enemy_count < current_level->getMaxEnemies())
				enemies[enemy_count++] = new SeaSatan(boss_x, boss_y);


		}
		void spawnPhase4Boss() {
			int w = current_level->getWidth();
			int h = current_level->getHeight();
			int cs = current_level->getCellSize();

			float plains_end = (float)(w / 3);
			float aerial_end = (float)((2 * w) / 3);
			int   water_start = (2 * w / 3) + 4;

			float mid_x = (plains_end + aerial_end) * 0.5f * cs;

			float ground_y = (float)((h - 4) * cs);

			float hover_y = (float)(4 * cs);

			float water_y = (float)((h - 8 + 2) * cs);

			Enemy** enemies = current_level->getEnemies();
			int& enemy_count = current_level->getEnemyCountRef();

			if (enemy_count < current_level->getMaxEnemies()) {
				UltimateBoss* ub = new UltimateBoss(mid_x, ground_y, hover_y, water_y);
				enemies[enemy_count++] = ub;
			}



		}
		void updateBossPhase() {
			if (!current_level) return;

			Character* player = characters[active_char_index];

			if (boss_phase == 1) {
				IronNokana* boss = nullptr;

				for (int i = 0; i < current_level->getMaxEnemies(); i++) {
					IronNokana* b = dynamic_cast<IronNokana*>(current_level->getEnemies()[i]);

					if (b) {
						boss = b;
						break;
					}
				}

				if (!boss) 
				  return;

				if ((boss->hasRetreated() || !boss->isAlive()) && !boss_phase_1_done) {
					boss_phase_1_done = true;
					boss_phase = 2;
					boss_minion_batch_active = false;
					boss_next_batch_timer = 6.0f;
					score->addScore(500, "Phase 1 Clear!");
					spawnPhase2Boss();
					return;
				}

				if (boss->isRetreating()) return;

				if (!boss_minion_batch_active) {
					boss_next_batch_timer -= dt;

					if (boss_next_batch_timer <= 0.0f) {
						spawnBossMinions(boss, player);
					}

					return;
				}

				if (countLiveMinions() == 0) {
					boss_minion_batch_active = false;
					boss_next_batch_timer = 8.0f;
					float cx = boss->getPosX() + 180.0f;
					float cy = boss->getPosY() - 64.0f;
					current_level->addInteractable(new SupplyCrate(cx, cy));
				}
			}
			else if (boss_phase == 2) {
				HairbusterRiberts* boss = nullptr;

				for (int i = 0; i < current_level->getMaxEnemies(); i++) {
					HairbusterRiberts* b = dynamic_cast<HairbusterRiberts*>(current_level->getEnemies()[i]);

					if (b) {
						boss = b;
						break;
					}
				}

				if (!boss)
					return;

				if ((boss->hasRetreated() || !boss->isAlive()) && !boss_phase_2_done) {
					boss_phase_2_done = true;
					boss_phase = 3;
					boss_minion_batch_active = false;
					boss_next_batch_timer = 6.0f;
					score->addScore(500, "Phase 2 Clear!");
					spawnPhase3Boss();
					return;
				}

				if (boss->isRetreating()) return;

				if (!boss_minion_batch_active) {
					boss_next_batch_timer -= dt;

					if (boss_next_batch_timer <= 0.0f) {
						spawnBoss2Minions(boss, player);
					}

					return;
				}

				if (countLiveMinions() == 0) {
					boss_minion_batch_active = false;
					boss_next_batch_timer = 8.0f;
					float cx = boss->getPosX() + 60.0f;
					float cy = boss->getPosY() + (float)200; ;
					current_level->addInteractable(new SupplyCrate(cx, cy));
				}

			}
			else if (boss_phase == 3) {
				SeaSatan* boss = nullptr;

				for (int i = 0; i < current_level->getMaxEnemies(); i++) {
					SeaSatan* b = dynamic_cast<SeaSatan*>(current_level->getEnemies()[i]);

					if (b) {
						boss = b;
						break;
					}
				}

				if (!boss)
					return;

				if ((boss->hasRetreated() || !boss->isAlive()) && !boss_phase_3_done) {
					boss_phase_3_done = true;
					boss_phase = 4;
					boss_minion_batch_active = false;
					boss_next_batch_timer = 6.0f;
					score->addScore(500, "Phase 3 Clear!");
					score->addScore(1500, "MAIN BOSSES DEFEATED!");
					spawnPhase4Boss();
					return;
				}

				if (boss->isRetreating()) return;

				if (!boss_minion_batch_active) {
					boss_next_batch_timer -= dt;

					if (boss_next_batch_timer <= 0.0f) {
						spawnBoss3Minions(boss, player);
					}

					return;
				}

				if (countLiveMinions() == 0) {
					boss_minion_batch_active = false;
					boss_next_batch_timer = 8.0f;
					float cx = boss->getPosX() - 100.0f;
					float cy = boss->getPosY() - 64.0f;
					current_level->addInteractable(new SupplyCrate(cx, cy));
				}


			}
			else if (boss_phase == 4) {
				UltimateBoss* boss = nullptr;

				for (int i = 0; i < current_level->getMaxEnemies(); i++) {
					UltimateBoss* b = dynamic_cast<UltimateBoss*>(current_level->getEnemies()[i]);

					if (b) {
						boss = b;
						break;
					}
				}

				if (!boss)
					return;

				if ((boss->isDefeated() || !boss->isAlive()) && !boss_phase_4_done) {
					boss_phase_4_done = true;
					boss_phase = -1;
					boss_minion_batch_active = false;
					boss_next_batch_timer = 6.0f;
					score->addScore(500, "Phase 4 Clear!");
					score->addScore(1500, "ULTIMATE BOSS DEFEATED!");
					gamestate = 4;
					return;
				}

				if (boss->isDefeated()) return;

				if (!boss_minion_batch_active) {
					boss_next_batch_timer -= dt;

					if (boss_next_batch_timer <= 0.0f) {
						spawnUltimateBossMinions(boss, player);
					}

					return;
				}

				if (countLiveMinions() == 0) {
					boss_minion_batch_active = false;
					boss_next_batch_timer = 8.0f;
					float cx = boss->getPosX() - 100.0f;
					float cy = boss->getPosY() - 64.0f;
					current_level->addInteractable(new SupplyCrate(cx, cy));
				}


			}
			  
		}
	    //Boss Functions End
		bool checkCollision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2) {
			return (x1 <= x2 + w2 && x1 + w1 >= x2 && y1 <= y2 + h2 && y1 + h1 >= y2);
		}

		int getLength(const char* str) {
			if (!str) {
				return 0;
			}
			int legnth = 0;
			char* str_len = const_cast<char*>(str);
			while (*str_len != '\0') {
				legnth++;
				str_len++;
			}

			return legnth;
		}
		char* copyString(const char* str) {
			if (!str) {
				return nullptr;
			}
			int legnth = getLength(str);

			char* copy = new char[legnth + 1];

			const char* read = str;
			char* write = copy;

			for (; *read != '\0';) {
				*write++ = *read++;
			}

			*write = '\0';

			return copy;
		}
		void int_to_str(char* copy, int n) {
			bool negative = false;
			if (n == 0) {
				copy[0] = '0';
				copy[1] = '\0';
				return;
			}
			long long int num = n;
			if (num < 0) {
				num = -num;
				negative = true;
			}
			char* temp = new char[20];
			int i = 0;

			for (;num > 0;) {
				temp[i++] = '0' + (num % 10);
				num /= 10;
			}

			if (negative == true)
				temp[i++] = '-';

			for (int i1 = 0; i1 < i / 2; i1++) {
				char c = *(temp + i1);
				*(temp + i1) = *(temp + i - i1 - 1);
				*(temp + i - i1 - 1) = c;
			}
			for (int i1 = 0; i1 < i;i1++) {
				*(copy + i1) = *(temp + i1);
			}
			copy[i] = '\0';
			delete[] temp;
		}
		char* concatenate(const char* str1, const char* str2) {
			if (!str1 && !str2) {
				return nullptr;
			}
			if (!str1) {
				return copyString(str2);
			}
			if (!str2) {
				return copyString(str1);
			}

			int len1 = getLength(str1);
			int len2 = getLength(str2);

			char* copy = new char[len1 + len2 + 1];
			char* write = copy;

			const char* p = str1;
			const char* p1 = str2;

			for (; *p != '\0';) {
				*write++ = *p;
				p++;
			}
			for (; *p1 != '\0';) {
				*write++ = *p1;
				p1++;
			}

			*write = '\0';

			return copy;
		}
		bool areEqual(const char* a, const char* b) {
			if (!a && !b) return true;
			if (!a || !b) return false;
			for (; *a != '\0' && *b != '\0'; a++, b++)
				if (*a != *b) return false;
			return *a == '\0' && *b == '\0';
		}

		char* substring(const char* str, int start, int length) {
			if (!str || length < 0 || start < 0) return nullptr;
			char* copy = new char[length + 1];
			for (int i = 0; i < length && str[start + i] != '\0'; i++)
				copy[i] = str[start + i];
			copy[length] = '\0';
			return copy;
		}

		int findSubstring(const char* str, const char* sub) {
			if (!str || !sub) return -1;
			int sublen = getLength(sub);
			for (int i = 0; str[i] != '\0'; i++) {
				bool match = true;
				for (int j = 0; j < sublen; j++)
					if (str[i + j] != sub[j]) { match = false; break; }
				if (match) return i;
			}
			return -1;
		}
		void checkEnemiesOffScreen() {

			if (current_level_num == 4) 
			 return;

			float off_left = cam_x - 64.0f;

			for (int e = 0; e < current_level->getEnemyCount(); e++) {
				Enemy* enemy = current_level->getEnemies()[e];

				if (!enemy || !enemy->isAlive()) {
					continue;
				}

				if (enemy->getPosX() + (float)enemy->getEnemyWidth() < off_left) {
					if (!enemy->shouldBeSweepable()) 
					continue;
					current_level->getEnemies()[e] = nullptr;

					if (enemy->get_wasattcked()) {
						grudge->registerEnemy(enemy);
					}
					else {
						cowardice->addEnemy(enemy);
					}
				}
			}
		}
		void parseAndExecuteUrdu() {
			int space_position = findSubstring(terminal_buf, " ");
			if (space_position == -1) return;

			int comma_position = findSubstring(terminal_buf, ",");
			if (comma_position == -1 || comma_position <= space_position) return;

			char* command = substring(terminal_buf, 0, space_position);
			char* variable = substring(terminal_buf, space_position + 1, comma_position - space_position - 1);
			char* number_string = substring(terminal_buf, comma_position + 1, getLength(terminal_buf) - comma_position - 1);

			if (!command || !variable || !number_string) {
				delete[] command;
				delete[] variable;
				delete[] number_string;
				return;
			}

			int value = 0;
			bool is_negative = (number_string[0] == '-');

			for (int i = is_negative ? 1 : 0; number_string[i] >= '0' && number_string[i] <= '9'; i++)
				value = value * 10 + (number_string[i] - '0');

			if (is_negative) value = -value;
			if (areEqual(command, "GHATAO")) value = -value;

			Character* player = characters[active_char_index];

			if (areEqual(variable, "LIVES"))    player->setLives(player->getLives() + value);
			else if (areEqual(variable, "HP"))       player->setHP(player->getHP() + value);
			else if (areEqual(variable, "GRENADES")) player->setGrenadeCount(player->getGrenadeCount() + value);
			else if (areEqual(variable, "AMMO") && player->getCurrentWeapon())
				player->getCurrentWeapon()->addAmmo(value);

			delete[] command;
			delete[] variable;
			delete[] number_string;
		}
		void handleCampaignProfileInput() {
			if (menu_input_cooldown_1 > 0.0f) {
				menu_input_cooldown_1 -= dt;
				return;
		 }
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				if (noise_profile) {
					delete noise_profile;
				}
				noise_profile = createNoiseProfile(0);
				loadLevel(1);
				bgm.play();
				gamestate = 2;
				ok.play();
				menu_input_cooldown_1 = 1.0f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				if (noise_profile) {
					delete noise_profile;
				}

				noise_profile = createNoiseProfile(1);

				loadLevel(1);
				bgm.play();
				gamestate = 2;
				ok.play();
				menu_input_cooldown_1 = 1.0f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
				if (noise_profile) {
					delete noise_profile;
				}

				noise_profile = createNoiseProfile(2);

				loadLevel(1);
				bgm.play();
				gamestate = 2;
				ok.play();
				menu_input_cooldown_1 = 1.0f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				gamestate = 1;
			}

		}
		void handleLevelSelectInput() {

			if (level_select_cooldown > 0.0f) {
				level_select_cooldown -= dt;
				return;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				loadLevel(1);
				bgm.play();
				gamestate = 2;
				ok.play();
				level_select_cooldown = 0.7f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				loadLevel(2);
				bgm.play();
				gamestate = 2;
				ok.play();
				level_select_cooldown = 0.7f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
				loadLevel(3);
				bgm.play();
				gamestate = 2;
				ok.play();
				level_select_cooldown = 0.7f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num4)) {
				loadLevel(4);
				bgm.play();
				gamestate = 2;
				ok.play();
				level_select_cooldown = 0.7f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				gamestate = 0;
			}
		}
		void handlePauseInput() {
			if (pause_input_cooldown > 0) {
				pause_input_cooldown -= dt;
				return;
			}
			if (Keyboard::isKeyPressed(Keyboard::R))      gamestate = 2;
			if (Keyboard::isKeyPressed(Keyboard::Escape)) gamestate = 2;
			if (Keyboard::isKeyPressed(Keyboard::Q)) { gamestate = 0; bgm.stop(); }
		}
		void updateTerminalInput() {
			if (!terminal_open) return;

			for (int letter_index = 0; letter_index < 26; letter_index++) {
				Keyboard::Key current_key = (Keyboard::Key)(Keyboard::A + letter_index);
				bool is_pressed_now = Keyboard::isKeyPressed(current_key);

				if (is_pressed_now && !term_key_prev[current_key] && terminal_len < 62) {
					terminal_buf[terminal_len++] = 'A' + letter_index;
					terminal_buf[terminal_len] = '\0';
				}

				term_key_prev[current_key] = is_pressed_now;
			}

			for (int digit_index = 0; digit_index < 10; digit_index++) {
				Keyboard::Key current_key = (Keyboard::Key)(Keyboard::Num0 + digit_index);
				bool is_pressed_now = Keyboard::isKeyPressed(current_key);

				if (is_pressed_now && !term_key_prev[current_key] && terminal_len < 62) {
					terminal_buf[terminal_len++] = '0' + digit_index;
					terminal_buf[terminal_len] = '\0';
				}

				term_key_prev[current_key] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::Comma);
				if (is_pressed_now && !term_key_prev[Keyboard::Comma] && terminal_len < 62) {
					terminal_buf[terminal_len++] = ',';
					terminal_buf[terminal_len] = '\0';
				}
				term_key_prev[Keyboard::Comma] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::Space);
				if (is_pressed_now && !term_key_prev[Keyboard::Space] && terminal_len < 62) {
					terminal_buf[terminal_len++] = ' ';
					terminal_buf[terminal_len] = '\0';
				}
				term_key_prev[Keyboard::Space] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::Dash);
				if (is_pressed_now && !term_key_prev[Keyboard::Dash] && terminal_len < 62) {
					terminal_buf[terminal_len++] = '-';
					terminal_buf[terminal_len] = '\0';
				}
				term_key_prev[Keyboard::Dash] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::BackSpace);
				if (is_pressed_now && !term_key_prev[Keyboard::BackSpace] && terminal_len > 0)
					terminal_buf[--terminal_len] = '\0';
				term_key_prev[Keyboard::BackSpace] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::Return);
				if (is_pressed_now && !term_key_prev[Keyboard::Return]) {
					parseAndExecuteUrdu();
					terminal_len = 0;
					terminal_buf[0] = '\0';
					terminal_open = false;
				}
				term_key_prev[Keyboard::Return] = is_pressed_now;
			}

			{
				bool is_pressed_now = Keyboard::isKeyPressed(Keyboard::Escape);
				if (is_pressed_now && !term_key_prev[Keyboard::Escape]) {
					terminal_open = false;
					terminal_len = 0;
					terminal_buf[0] = '\0';
				}
				term_key_prev[Keyboard::Escape] = is_pressed_now;
			}
		}
		void handleMenuInput() {
			if (menu_input_cooldown > 0.0f) {
				menu_input_cooldown -= dt;
				return;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				game_mode = 0;
				gamestate = 1; 
				ok.play();
				level_select_cooldown = 0.7f;
				menu_input_cooldown = 0.5f;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				ok.play();
				game_mode = 1; //Campaign chosen 
				gamestate = 6;
				menu_input_cooldown_1 = 0.5f;
				//Perlin Noise
			}
			else if(Keyboard::isKeyPressed(Keyboard::Escape)){
			   window.close();
			}

		}
		//Game Specific Functions in private
		void handleInput() {
			Character* player = characters[active_char_index]; //Current Active Player
			
			if (!player->isAlive()) {
				return;
			 }
			//Close Game
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				gamestate = 5; 
				pause_input_cooldown = 0.3f;
				return;
			}
			/// vehicle
			if (active_vehicle && active_vehicle->isAlive()) {
				if (Keyboard::isKeyPressed(Keyboard::Right)) 
				  active_vehicle->moveRight();
				else if (Keyboard::isKeyPressed(Keyboard::Left))
				   active_vehicle->moveLeft();
				else active_vehicle->stopHorizontal();

				if (Keyboard::isKeyPressed(Keyboard::Up)) {
					active_vehicle->moveUp();
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down)) {
					active_vehicle->moveDown();
				}
				else {
					active_vehicle->stopVertical();
				}


				if (Keyboard::isKeyPressed(Keyboard::X)) {
					Projectile* vp = active_vehicle->fire();
					if (vp) 
					  spawnProjectile(vp);
				}
				if (Keyboard::isKeyPressed(sf::Keyboard::C)) {
					Projectile* vp = active_vehicle->fireMissile();
					if (vp) 
					  spawnProjectile(vp);
				}
				if (Keyboard::isKeyPressed(sf::Keyboard::Z))
					active_vehicle->cycleMissile();

				// Exit vehicle
				if (Keyboard::isKeyPressed(Keyboard::E)) {
					if (enter_exit_vehicle_cooldown > 0.0f) {
						enter_exit_vehicle_cooldown -= dt;
						return;
					}
					active_vehicle->exit();
					enter_exit_vehicle_cooldown = 0.5f;
					active_vehicle = nullptr;
				}
				return;  // skip foot controls while in vehicle
			}
			//VEHICLE   UDKHUL
			if (Keyboard::isKeyPressed(Keyboard::E)) {
				if (enter_exit_vehicle_cooldown > 0.0f) {
					enter_exit_vehicle_cooldown -= dt;
					return;
			  }
				for (int i = 0; i < current_level->getMaxVehicles(); i++) {
					Vehicle* v = current_level->getVehicles()[i];
					if (v && v->isAlive() && !v->isOccupied() && v->isNearPlayer(player)) {
						v->enter(player); 
						active_vehicle = v; 
						enter_exit_vehicle_cooldown = 0.5f;
						break;
					}
				}
			}
			//Movement
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player->moveRight(dt);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player->moveLeft(dt);
			}
			else
			{
				player->stopHorizontal();
			}
			if (Keyboard::isKeyPressed(Keyboard::Up)){
				if (player->isInWater())
					player->swimUp();
				player->jump(dt);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				if (player->isInWater())
					player->sink();
				//player->crouch();
			}
			//Aiming
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player->setAimAngle(player->getAimAngle() + 60.0f * dt);
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player->setAimAngle(player->getAimAngle() - 60.0f * dt);
			}

			//Melee
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				bool airborne = !player->getOnground();
				player->melee(current_level->getEnemies(), current_level->getMaxEnemies(), score, airborne);
				if (game_mode == 1) {
					for (int i = 0; i < current_level->getMaxEnemies(); i++) {
						Enemy* e = current_level->getEnemies()[i];
						if (e && !e->isAlive() && e->get_wasattcked()) {
							int t = e->getEnemyTypeID();
							if (t >= 0 && t < 8 && campaign_kills[t] < 5)
								campaign_kills[t]++;
						}
					}
				}
			}
			//Weapon Use
			
			if (Keyboard::isKeyPressed(Keyboard::X)) {   
				player->shoot(projectiles, projectile_count, max_projectiles);
			}
			if (Keyboard::isKeyPressed(sf::Keyboard::C)) {
				player->throwGrenade(projectiles, projectile_count,max_projectiles,grenade_tex);
			}
			//Interactables
			if (Keyboard::isKeyPressed(Keyboard::F)) {
				for (int i = 0; i < current_level->getMaxInteractables(); i++) {
					InteractableObject* obj = current_level->getInteractables()[i];
					if (!obj || !obj->getIsActive()) continue;
					float dx = obj->getPosX() - player->getPosX();
					float dy = obj->getPosY() - player->getPosY();
					if (dx * dx + dy * dy < 90.0f * 90.0f) {
						// Supply crate
						SupplyCrate* crate = dynamic_cast<SupplyCrate*>(obj);
						if (crate) {
							// Give grenades
							int new_g = player->getGrenadeCount() + crate->getGrenadeCount();
							player->setGrenadeCount(new_g);
							// Give / replace weapon
							Weapon* new_wp = crate->createWeapon();
							Fio* fio_check = dynamic_cast<Fio*>(player);                           
							Weapon* old = player->getCurrentWeapon();
							if (fio_check && new_wp && new_wp->getAmmo() > 0) {
								int extra = new_wp->getAmmo() / 2;
								new_wp->addAmmo(extra);
								player->setGrenadeCount(player->getGrenadeCount() +(int)(crate->getGrenadeCount() * 0.5f));
							}
							if (old) delete old;
							player->setWeapon(new_wp);
							crate->onInteract();
							score->addScore(0, "Crate");
						}
						// POW Prisoner
						POWPrisoner* pow = dynamic_cast<POWPrisoner*>(obj);
						if (pow && !pow->getIsFreed()) {
							pow->onInteract();
							SupplyCrate* reward = new SupplyCrate(pow->getPosX(), pow->getPosY());
							current_level->addInteractable(reward);
							score->addScore(100, "POW");
						}
						break;
					}
				}
			}
			//Character Switching
			if (Keyboard::isKeyPressed(Keyboard::Z) && switch_cooldown <= 0) {

				float old_x = player->getPosX();
				float old_y = player->getPosY();


				for (int i = 1;i <= num_characters;i++) {
					int next_char = (active_char_index+i)% num_characters;
					if (characters[next_char]->getLives() > 0 && characters[next_char]->isAlive()) {
					    active_char_index = next_char;
						characters[active_char_index]->setPosition(old_x,old_y);
						break;
					}
				}
				switch_cooldown = 0.3f; //cooldown
			}
			if (Keyboard::isKeyPressed(Keyboard::V))
				player->activateSpecial();

				//Dev mode
			dev_mode->handleInput(Keyboard::isKeyPressed(Keyboard::D), dt, characters, num_characters);

			 //Urdu Terminal
			if (dev_mode->isActive() && !terminal_open && sf::Keyboard::isKeyPressed(sf::Keyboard::T))
				terminal_open = true;
			//Fusion Companion
			bool b_now = Keyboard::isKeyPressed(Keyboard::B);

			if (b_now && !b_was) {
				Character* player = characters[active_char_index];
				fusion_companion->activate(player->getPosX(),player->getPosY(),characters,num_characters);
			}

			b_was = b_now;
				//Rewind

				//POW PRISONER

		}
		void update(){
			Character* player = characters[active_char_index]; //Current Active Player


			//Check if player is alive
			if (!player->isAlive()) {
				checkPlayerDeath();
				return;
			  }

			  //UpdateRewind
			  
			  //Devmode

			if (dev_mode->isActive()) {

			}
			//Vehicle Update
			if (active_vehicle && active_vehicle->isAlive()) {
				active_vehicle->updateTimers(dt);
				active_vehicle->updateMode(current_level);
				active_vehicle->applyGravity(current_level);
				active_vehicle->applyHorizontalCollision(current_level);
				active_vehicle->Driver();
			}
			if (active_vehicle && !active_vehicle->isAlive()) {
				Tarma* t = dynamic_cast<Tarma*>(characters[active_char_index]);
				if (!t) active_vehicle = nullptr;  
				else {

					active_vehicle->exit();
					active_vehicle = nullptr;
				}
			}
			  //Timers
			player->updateTimers(dt);

			if (switch_cooldown > 0) 
			   switch_cooldown -= dt;

           //Player Position already updated in handleInput()

		   //Updating Gravity
			if (!active_vehicle || !active_vehicle->isAlive()) {
				player->applyHorizontalCollision(current_level);
				player->applyGravity(current_level);
			}
		   grudge->update(dt, player, current_level->getEnemies(),current_level->getEnemyCountRef(), current_level->getMaxEnemies());
		   cowardice->update(dt, player, current_level->getEnemies(),current_level->getEnemyCountRef(), current_level->getMaxEnemies());

		   echo->setUnderwater(player->isInWater(), bgm);

		   
		   //Updating Projectiles
			for (int i = 0;i < max_projectiles;i++) {
				if (projectiles[i] != 0 && projectiles[i]->isActive()) {
					projectiles[i]->update(current_level);
				}
				if (projectiles[i] != 0 && projectiles[i]->isActive()) {
					if (projectiles[i]->isGrenade()  && projectiles[i]->getVelocityY() < 10.0f) {
						projectiles[i]->addVelocityY(0.2f);
					}
				}

		   //Updating Enemies(OLD)
		  /* for(int i = 0;i < current_level->enemy_count;i++){
			   if (current_level->enemies[i] == nullptr) {
				   continue;
			 }
			   if (projectiles->checkHit((current_level->enemies[i]->getPosX(), current_level->enemies[i]->getPosY(), 64, 64)) {
				   current_level->enemies[i]->takeDamage(projectiles[i]->damage);
	
			   projectiles[i]->is_active = false;

				   if (current_level->enemies[i]->hp <= 0) {
				   score += current_level->enemies[i]->score_value;
				   delete current_level->enemies[i];
				   current_level->enemies[i] = nullptr;
			   }
			   break;

			   }

		     }*/

		   }

		   //Score
			if (score)
			  score->update(dt);
		   //Updating Enemies
			for (int i = 0; i < current_level->getEnemyCount(); i++) {
				if (current_level->getEnemies()[i] != 0 && current_level->getEnemies()[i]->isAlive())
					current_level->getEnemies()[i]->update(player, current_level,projectiles, projectile_count, max_projectiles,enemy_bullet_tex,dt);
			}

			checkEnemyCollisions();
			checkPickupCollisions();
			checkEnemiesOffScreen();
			if (current_level_num == 4) {
				updateBossPhase();
			}


           //Fusion Companion
			if (fusion_companion && fusion_companion->isActiveCompanion()) {
				fusion_companion->updateAI(characters[active_char_index], current_level, current_level->getEnemies(), current_level->getMaxEnemies(), projectiles, projectile_count, max_projectiles, score, dt);
			}

			//Rain
			if (rain && current_level)
			rain->update(dt, current_level);
			

			//Updating View
			updateCamera();

			//Check Level Completion

			if (game_mode == 1) {
				bool quota_met = true;

				for (int i = 0; i < 8; i++) {
					if (campaign_kills[i] < 5) {
						quota_met = false;
						break;
					}
				}

				if (quota_met) {
					score->addScore(3000, "Campaign Clear");
					gamestate = 4;
				}

				return;
			}

			bool all_dead = true;
			for (int i = 0; i < current_level->getEnemyCount(); i++) {
			if (current_level->getEnemies()[i] != 0 && current_level->getEnemies()[i]->isAlive()){
				all_dead = false;
				break;
				}
		}
		    
			if (all_dead && game_mode == 0 && current_level_num != 4) {
				bool flawless = true;
			for (int i = 0; i < num_characters; i++)
			{
				if (characters[i]->getLives() < 2)   // started with 2
				{
					flawless = false;
					break;
				}
			}

			if (flawless)
				score->addScore(5000, "FLAWLESS!");
			else
				score->addScore(1000, "LEVEL CLEAR");

			if (current_level_num < 3)
			{
				current_level_num++;
				loadLevel(current_level_num);
			}
			else if (current_level_num == 3)
			{
				gamestate = 4; 
			}
				
			}	
		}
		void renderHUD() {
		   Character* player = characters[active_char_index];

		   

		   char num[32];
		   char* temp;


		   ////HP
		   //int_to_str(num, (int)player->getHP());
		   //temp = concatenate("HP: ", num);
		   //hp_text.setString(temp);
		   //hp_text.setPosition(10, 180);
		   //window.draw(hp_text);
		   //delete[] temp;

		   //Scoring
		   int_to_str(num, score->getScore());
		   temp = concatenate("Score: ", num);
		   score_text.setString(temp);
		   score_text.setPosition(10, 10);
		   window.draw(score_text);
		   delete[] temp;

		   //Lives
		   int_to_str(num, player->getLives());
		   temp = concatenate("Lives: ", num);
		   lives_text.setString(temp);
		   window.draw(lives_text);
		   delete[] temp;

		   //Ammo
		   if (player->getCurrentWeapon()){
			   if (player->getCurrentWeapon()->getAmmo() < 0) {
				   temp = copyString("Ammo: INF");
			   }
			   else{
				   int_to_str(num, player->getCurrentWeapon()->getAmmo());
				   temp = concatenate("Ammo: ", num);
			   }
		   }
		   else{
			   temp = copyString("Ammo: ---");
		   }
		   ammo_text.setString(temp);
		   window.draw(ammo_text);
		   delete[] temp;

		   //Grenades
		   int_to_str(num, player->getGrenadeCount());
		    temp = concatenate("Grenades: ", num);
		   grenade_text.setString(temp);
		   window.draw(grenade_text);
		   delete[] temp;

		   //Dev Mode (Wait To Be Implemented)
		   if (dev_mode_active)
		   {
			   mode_text.setString("DEV MODE ON");
			   mode_text.setFillColor(sf::Color::Red);
			   mode_text.setPosition(700, 10);
			   window.draw(mode_text);
			   mode_text.setFillColor(sf::Color::Yellow);
		   }

		   //DevMode
		   dev_mode->renderHUD(window, mode_text);
		   //Rain

		   //Rewind

		   //Characters
		   const char* names[] = { "Marco", "Tarma", "Eri", "Fio" };
		    temp = concatenate("Char: ", names[active_char_index]);
		   mode_text.setString(temp);
		   delete[] temp;
		   mode_text.setPosition(1400, 10);
		   window.draw(mode_text);

		   if (grudge->getCount() > 0)
		   {
			   char num[16];
			   int_to_str(num, grudge->getCount());
			   char* temp = concatenate("Grudges: ", num);
			   grudge_text_hud.setString(temp);
			   window.draw(grudge_text_hud);
			   delete[] temp;
		   }

		   // Cowardice warning — flash red when counterattack is coming
		   if (cowardice->getCount() > 0)
		   {
			   char num[16];
			   int_to_str(num, cowardice->getCount());
			   char* prefix = concatenate("Reserve: ", num);
			   char* temp = concatenate(prefix, cowardice->isTriggered() ? " INCOMING!" : "");
			   cowardice_text_hud.setFillColor(
				   cowardice->isTriggered()  ? sf::Color::Red : sf::Color(200, 0, 200));
			   cowardice_text_hud.setString(temp);
			   window.draw(cowardice_text_hud);
			   delete[] prefix;
			   delete[] temp;
		   }

		   if (game_mode == 1) {
			   int total = 0;
			   for (int i = 0; i < 8; i++) total += (campaign_kills[i] >= 5 ? 1 : 0);
			   char num[4];
			   int_to_str(num, total);
			   char* prog = concatenate("Campaign: ", num);
			   char* prog2 = concatenate(prog, "/8 types");
			   mode_text.setString(prog2);
			   mode_text.setPosition(10, 155);
			   window.draw(mode_text);
			   delete[] prog;
			   delete[] prog2;
		   }

		}
		void renderBossHUD() {
			UltimateBoss* ub = nullptr;
			for (int i = 0; i < current_level->getMaxEnemies(); i++) {
				UltimateBoss* b = dynamic_cast<UltimateBoss*>(current_level->getEnemies()[i]);
				if (b) { ub = b; break; }
			}
			if (!ub) return;

			char num[16];
			char* temp;

			int_to_str(num, (int)ub->getGroundHP());
			temp = concatenate("Ground HP: ", num);
			boss_hud_ground.setString(temp);
			window.draw(boss_hud_ground);
			delete[] temp;

			int_to_str(num, (int)ub->getAerialHP());
			temp = concatenate("Aerial HP: ", num);
			boss_hud_aerial.setString(temp);
			window.draw(boss_hud_aerial);
			delete[] temp;

			int_to_str(num, (int)ub->getAquaticHP());
			temp = concatenate("Aquatic HP: ", num);
			boss_hud_aquatic.setString(temp);
			window.draw(boss_hud_aquatic);
			delete[] temp;

			const char* state_names[] = { "GROUND","AERIAL","AQUATIC" };
			temp = concatenate("Active: ", state_names[ub->getCurrentState()]);
			boss_phase_text.setString(temp);
			window.draw(boss_phase_text);
			delete[] temp;
		}
		void render(){
		 //All GameStates
			window.clear();
		//Decide What To Draw W.R.T Gamestate
			if (gamestate == 0) {
			//Menu
			    window.draw(menu_bg_sprite);
				window.draw(menu_title);
				window.draw(menu_survival);
				window.draw(menu_campaign);

			}
			else if (gamestate == 2 || gamestate == 4) {

			current_level->render(window, cam_x, cam_y);

				for (int i = 0; i < current_level->getMaxEnemies(); ++i){
					Enemy* enemy = current_level->getEnemies()[i];

					if (enemy == nullptr || !enemy->isAlive())
						continue;

					enemy->render(window, cam_x, cam_y);
				}
				for (int i = 0; i < current_level->getMaxCollectibles(); i++) {
					Collectible* c = current_level->getCollectibles()[i];
					if (c && !c->isCollected())
						c->render(window, cam_x, cam_y);
				}
				for (int i = 0; i < current_level->getMaxVehicles(); i++) {
					Vehicle* v = current_level->getVehicles()[i];
					if (v && v->isAlive())
						v->render(window, cam_x, cam_y);
				}
				for (int i = 0; i < max_projectiles; ++i) {
					Projectile* projectile = projectiles[i];

					if (projectile == nullptr)
						continue;

					if (!projectile->isActive())
						continue;

					projectile->render(window, cam_x, cam_y);
				}
				if (score->getPopupTimer() > 0.0f) {
					bonus_text.setString(score->getPopupText());
					window.draw(bonus_text);
				}
				Character* player = characters[active_char_index];
				if (player->isAlive() && !active_vehicle)
				player->render(window, cam_x, cam_y);

				if (fusion_companion && fusion_companion->isActiveCompanion())
					fusion_companion->render(window, cam_x, cam_y);

				 if (player->getDamageState() > 0) {
					red_overlay_sprite.setPosition(0,0);
					window.draw(red_overlay_sprite);
					if (player->getDamageState() == 2) {
						window.draw(red_overlay_sprite);
					  }
				}

				//Rain
				rain->render(window, cam_x, cam_y);

				if (current_level_num == 4 && boss_phase == 4)
					renderBossHUD();
				renderHUD();

				if (gamestate == 4) {
					game_over_text.setFillColor(Color::Green);
					game_over_text.setString("YOU WIN!");
					game_over_text.setPosition(600.f, 400.f);
					window.draw(game_over_text);
				}

				//Terminal
				if (terminal_open) {
					char* prompt = concatenate("[MetalSlugOOP2026]$ ", terminal_buf);
					terminal_text.setString(prompt);
					window.draw(terminal_text);
					delete[] prompt;
				}


			}
			else if (gamestate == 3) {
				window.draw(game_over_text);
				char num[32] = {'\0'};
				int_to_str(num, score->getScore());
				char* temp = concatenate("Final Score: ", num);
				score_text.setString(temp);
				delete[] temp;

				score_text.setPosition(600, 430);
				window.draw(score_text);
				score_text.setPosition(10, 10);

			}
			else if (gamestate == 5) {
				current_level->render(window, cam_x, cam_y);
				characters[active_char_index]->render(window, cam_x, cam_y);
				pause_text.setString("PAUSED");
				pause_text.setPosition(680.0f, 340.0f);
				window.draw(pause_text);
				pause_resume.setString("R - Resume   Q - Quit to Menu");
				pause_resume.setPosition(560.0f, 420.0f);
				window.draw(pause_resume);
			}
			else if (gamestate == 1) {
				window.draw(level_bg_sprite);
			}
			else if (gamestate == 6) {
			window.draw(noise_bg_sprite);
			}
			window.display();
		}
		void updateCamera() {
			Character* player = characters[active_char_index]; //Current Active Player
			

			//Appear Slightly To The Left
			float focus_x = (active_vehicle && active_vehicle->isAlive())? active_vehicle->getPosX() : player->getPosX();
			cam_x = focus_x - 400;

			if (cam_x < 0)
			  cam_x = 0;

			  //Prevent Right Edge
			float max_cam_x = (current_level->getWidth() * 64) - 1600.0f;
		if (max_cam_x < 0) 
			    max_cam_x = 0;
			if (cam_x > max_cam_x) 
			    cam_x = max_cam_x;

			   
			    
			float focus_y = (active_vehicle && active_vehicle->isAlive())? active_vehicle->getPosY() : player->getPosY();
			cam_y = focus_y - 450;
			if (cam_y < 0)
				cam_y = 0;

			float max_cam_y = (current_level->getHeight() * 64) - 881.0f;
			 if (max_cam_y < 0) 
			    max_cam_y = 0;
			if (cam_y > max_cam_y) 
			   cam_y = max_cam_y;
		}
		void checkEnemyCollisions() {
		//Enemy Collisions

			Character* player = characters[active_char_index];
			bool player_airborne = !player->getOnground();


			//Scoring should be resets

			for (int i = 0; i < max_projectiles; i++) {
			   
			   if(projectiles[i] == 0 || !projectiles[i]->isActive())
			     continue;

				 //Blast Radius Check
			   if (projectiles[i]->isExplosive() && projectiles[i]->justExploded()) {
				   float p_x = projectiles[i]->getExplodeX();
				   float p_y = projectiles[i]->getExplodeY();
				   float blast_r = projectiles[i]->getBlastRadius() * current_level->getCellSize();

				   int blast_kills = 0;
						 for (int e = 0; e < current_level->getMaxEnemies(); e++) {
							 Enemy* enemy = current_level->getEnemies()[e];
							 if (enemy == 0 || !enemy->isAlive()) 
							   continue;
							   float e_x = enemy->getPosX();
							   float e_y = enemy->getPosY();   
							   float dist_x = e_x - p_x;
							   float dist_y = e_y - p_y;
							   float dist_sq = dist_x * dist_x + dist_y*dist_y;

							   if (dist_sq <= (blast_r * blast_r)) {
								   enemy->onBlastHit(projectiles[i]->getDamage());
							   }
							   if (!enemy->isAlive()) {
								   score->addScore(enemy->getScorevalue(), "Kill");
								   if (rand() % 100 < 15) {
									   Collectible* food = (rand() % 2 == 0) ? (Collectible*)new FruitCollectible(enemy->getPosX(), enemy->getPosY()): (Collectible*)new TurkeyCollectible(enemy->getPosX(), enemy->getPosY());
									   current_level->addCollectible(food);
								   }
								   blast_kills++;
								   if (game_mode == 1) {
									        int t = enemy->getEnemyTypeID();
									       if (t>=0 && t<8 && campaign_kills[t]<5) 
										      campaign_kills[t]++;
									    }
							   }
						 if (blast_kills == 2) score->addScore(200, "Multi-Kill!");
						 else if (blast_kills >= 3) score->addScore(300 + (blast_kills - 3) * 50, "MASSACRE!");
						 }
						
						 if (projectiles[i]->isFromEnemy()){
							float dx = (player->getPosX() + player->getCharacterWidth() * 0.5f) - p_x;
							 float dy = (player->getPosY() + player->getCharacterHeight() * 0.5f) - p_y;
							 if (dx * dx + dy * dy <= blast_r * blast_r)
								 player->takeDamage(projectiles[i]->getDamage());
						 }
						 if (projectiles[i]->isFire()) {
							 for (int fd = 0; fd < 5 && projectile_count < max_projectiles; fd++) {
								 float fx = p_x + (float)(rand() % (int)(blast_r * 2)) - blast_r;
								 Projectile* fire = new Projectile(fx, p_y, 0.0f, 0.0f, 0.07f, 32, 16);
								 fire->setFire(true); fire->setLifespan(10.0f);
								 spawnProjectile(fire);
							 }
						 }
						 projectiles[i]->clearExploded();
				   }
			   if (!projectiles[i]->isActive()) continue;
			   if (!projectiles[i]->isFromEnemy()){
			   bool hit_enemy = false;
				 //Checking Enemies
				for (int e = 0;e < current_level->getMaxEnemies();e++) {
				        
						Enemy * enemy = current_level->getEnemies()[e];

						 if (enemy == 0 || !enemy->isAlive())
						   continue;

					if (projectiles[i]->checkHit(enemy->getPosX(), enemy->getPosY(),enemy->getEnemyWidth(), enemy->getEnemyHeight())) {
						if (enemy->blocksBullet(projectiles[i]->getVelocityX(), player->getPosX()) && !projectiles[i]->isExplosive()) {
							projectiles[i]->setActive(false);
							hit_enemy = true;
							break;
						}
						if (enemy->handleSpecialHit(projectiles[i])) { 
							projectiles[i]->setActive(false);
							hit_enemy = true;
							break;
						}
					 enemy->takeDamage(projectiles[i]->getDamage());
					 projectiles[i]->setActive(false);
					 hit_enemy = true;

					 if (!enemy->isAlive()) {
						 score->addScore(enemy->getScorevalue(), "Kill");
						 if (rand() % 100 < 15) {
							 Collectible* food = (rand() % 2 == 0) ? (Collectible*)new FruitCollectible(enemy->getPosX(), enemy->getPosY()) : (Collectible*)new TurkeyCollectible(enemy->getPosX(), enemy->getPosY());
							 current_level->addCollectible(food);
						 }
					 }
					 if (game_mode == 1) {                            
						 int t = enemy->getEnemyTypeID();           
						 if (t >= 0 && t < 8 && campaign_kills[t] < 5) 
							 campaign_kills[t]++;                     
					 }
					 break;
				 }
	

		 }
				if (hit_enemy)
				  continue;
		  }
			   //Checking Enemies
				if (projectiles[i]->isFromEnemy() && projectiles[i]->isActive()) {
					bool toward =(projectiles[i]->getVelocityX() > 0 && projectiles[i]->getPosX() < player->getPosX()) || (projectiles[i]->getVelocityX() < 0 && projectiles[i]->getPosX() > player->getPosX()) || (projectiles[i]->getVelocityY() != 0);
					if (toward && checkCollision(projectiles[i]->getPosX(), projectiles[i]->getPosY(), projectiles[i]->getWidth(), projectiles[i]->getHeight(), player->getPosX(), player->getPosY(), (int)player->getCharacterWidth(), (int)player->getCharacterHeight()))
					{
						player->takeDamage(projectiles[i]->getDamage());
						projectiles[i]->setActive(false);
					}
				}
		}
				//Checking Direct Enemies
				for (int e = 0;e < current_level->getMaxEnemies();e++) {
				   Enemy * enemy = current_level->getEnemies()[e];
				   if(enemy == 0 || !enemy->isAlive())
				     continue;
				   if (checkCollision(player->getPosX(), player->getPosY(), (int)player->getCharacterWidth(), (int)player->getCharacterHeight(), enemy->getPosX(), enemy->getPosY(), enemy->getEnemyWidth(), enemy->getEnemyHeight())) {
					   player->takeDamage(1.0f);
					   break;
				   }

				}
				
		}
		void checkPickupCollisions() {
			//Pickup Collisions
			Character* player = characters[active_char_index];
			for (int i = 0; i < current_level->getMaxCollectibles(); i++) {
				Collectible* c = current_level->getCollectibles()[i];
				if (!c || c->isCollected()) continue;
				float dx = c->getPosX() - player->getPosX();
				float dy = c->getPosY() - player->getPosY();
				if (dx * dx + dy * dy < 55.0f * 55.0f) {
					c->collect();
					player->addSaturation(c->getHealValue());
					score->addScore(5, "Food");
				}
			}
		}
		void checkPlayerDeath() {
			//Player Lives Check
			active_vehicle = nullptr;
				Character* player = characters[active_char_index];
				float old_x = player->getPosX();
				float old_y = player->getPosY();
				bool found = false;
			for (int i = 1;i <= num_characters;i++) {
				int next_char = (active_char_index + i) % num_characters;
			if (characters[next_char]->getLives() > 0 && characters[next_char]->isAlive()) {
					active_char_index = next_char;
					characters[active_char_index]->setPosition(old_x, old_y - 25.0f);  // CHANGED
					characters[active_char_index]->resetMovement();
				   found = true;
					break;	
				}
			}
			if (!found) {
				gamestate = 3; //game over
			}
			   
		}
		void spawnProjectile(Projectile* p) {
			for (int i = 0;i < max_projectiles;i++) {
				if (projectiles[i] == 0 || !projectiles[i]->isActive()) {
					if (projectiles[i]) {
						delete projectiles[i];
					}
					projectiles[i] = p;
					return;
			  }
		   }
		   delete p;
		}
		//void checkEnemiesOffScreen() {
		//  //Grudge
		//}
		void loadLevel(int level_num) {

			if (current_level) {
				delete current_level;
			}
			if (fusion_companion) {
				fusion_companion->resetCompanion();
			}
			for (int i = 0; i < max_projectiles; i++) {
				if (projectiles[i]) { delete projectiles[i]; projectiles[i] = nullptr; }
			}
			projectile_count = 0;
			active_vehicle = nullptr;

			grudge->clear();
			cowardice->clear();

			if (game_mode == 1) {
				if (!noise_profile) {
					noise_profile = createNoiseProfile(0);
				}
				for (int i = 0; i < 8; i++)
					campaign_kills[i] = 0;
				current_level = new Level(5000, 40, "campaign");
				current_level->buildCampaignLevel(noise_profile);
				current_level_num = 1;
				int cs = current_level->getCellSize();

				for (int i = 0; i < num_characters; i++)
				{
					float char_x = 200.0f + i * 70.0f;

					int spawn_col = (int)(char_x / cs);
					if (spawn_col < 0)                      
					   spawn_col = 0;
					if (spawn_col >= current_level->getWidth()) 
					   spawn_col = current_level->getWidth() - 1;

				
					float spawn_y = (float)((current_level->getHeight() - 2) * cs) - 130.0f;

					for (int row = 0; row < current_level->getHeight(); row++)
					{
						char block = current_level->getBlock(row, spawn_col);
						if (block == 'g' || block == 's' || block == 'b')
						{
						
							spawn_y = (float)(row * cs) - 130.0f;
							break;
						}
					}

					characters[i]->setPosition(char_x, spawn_y);
					characters[i]->resetMovement();
				}

			}
			else if (level_num == 4) {
				current_level = new Level(200, 35, "boss");
				 current_level->buildBossLevel();
				 current_level_num = 4;

				 boss_phase = 1;
				 boss_minion_batch_active = false;
				 boss_next_batch_timer = 5.0f;
				 boss_phase_1_done = false;
				 boss_phase_2_done = false;
				 boss_phase_3_done = false;

				 float spawn_y = (float)((35 - 4) * 64) - 96.0f;
				 for (int i = 0; i < num_characters; i++) {
					 characters[i]->setPosition(200.0f + i * 70.0f, spawn_y);
					 characters[i]->resetMovement();
				 }
			}
			else {
				//Survival Levels
				current_level = new Level(151 + rand() % 90, 21 + rand() % 30, "survival");
				current_level->buildSurvivalLevel(level_num);
				current_level_num = level_num;
				float above_ground = float((current_level->getHeight() - 2) * current_level->getCellSize());
				for (int i = 0; i < num_characters; i++) {
					characters[i]->setPosition(200.0f + i * 70.0f, above_ground - 128.0f);
					characters[i]->resetMovement();
				}
			}

	

			   cam_x = 0;
			   cam_y = 0;

			   level_manager.setLevel(nullptr, level_num);

		}
public:
		Game():window(VideoMode(1600,900),"Metal Slug", Style::Close) {
			window.setVerticalSyncEnabled(true);
			window.setFramerateLimit(60);
			score = new Scoring();
			game_mode = 0;
			active_char_index = 0;
			dev_mode_active = false;
			gamestate = 0; //at menu
			cam_x = 0.0;
			cam_y = 0.0;
			srand((unsigned int)time(0));

			//StateManager
			state_manager.setState(new PlayState());

			//Character Creations

			characters[0] = new Marco();
			characters[1] = new Tarma();
			characters[2] = new Eri();
			characters[3] = new Fio();
			fusion_companion = new FusionCompanion();
			fusion_companion->setWeapon(new Pistol());
			num_characters = 4;
			

			//Weapons
			for (int i = 0; i < 4; i++) {
				characters[i]->setWeapon(new Pistol());
			}


			//Campaign Level
			for (int i = 0;i < 8;i++) 
			  campaign_kills[i] = 0;
			campaign_complete = false;
			noise_profile = nullptr;
			//Level
			current_level_num = 1;	
			current_level = nullptr;

			//Projectile Creations

			max_projectiles =100;projectile_count = 0;
			projectiles = new Projectile * [max_projectiles];
			for (int i = 0;i < max_projectiles;i++) {
				projectiles[i] = 0;
			}

			//Vehicle
			active_vehicle = nullptr;
			//EntityManager
			entity_manager.initializer(characters, num_characters);

			//Boss
			boss_phase = 1;
			boss_minion_batch_active = false;
			boss_minion_batch_size = 0;
			boss_next_batch_timer = 5.0f;
			boss_phase_1_done = false;
			boss_phase_2_done = false;
			boss_phase_3_done = false;
			boss_phase_4_done = false;

			//Textures
			bullet_tex.loadFromFile("25I-0555_25I-1034_Assets/bullet.png");
			grenade_tex.loadFromFile("25I-0555_25I-1034_Assets/grenade_tex.png");
			enemy_bullet_tex.loadFromFile("25I-0555_25I-1034_Assets/enemy_bullet.png");

			//Menu
			menu_bg.loadFromFile("25I-0555_25I-1034_Assets/menu.png");
			menu_bg_sprite.setTexture(menu_bg);
			level_select_bg.loadFromFile("25I-0555_25I-1034_Assets/level_select.png");
			level_bg_sprite.setTexture(level_select_bg);
			noise_select_bg.loadFromFile("25I-0555_25I-1034_Assets/noise_select.png");
			noise_bg_sprite.setTexture(noise_select_bg);
			//HUD
			font.loadFromFile("25I-0555_25I-1034_Assets/arial.ttf");
			score_text.setFont(font);
			score_text.setCharacterSize(20);
			score_text.setFillColor(Color::White);
			score_text.setPosition(10, 10);
			lives_text.setFont(font);
			lives_text.setCharacterSize(20);
			lives_text.setFillColor(Color::White);
			lives_text.setPosition(10, 35);
			ammo_text.setFont(font);
			ammo_text.setCharacterSize(20);
			ammo_text.setFillColor(Color::White);
			ammo_text.setPosition(10, 60);
			grenade_text.setFont(font);
			grenade_text.setCharacterSize(20);
			grenade_text.setFillColor(Color::White);
			grenade_text.setPosition(10, 85);
			mode_text.setFont(font);
			mode_text.setCharacterSize(16);
			mode_text.setFillColor(Color::Yellow);
			mode_text.setPosition(1400, 10);
			menu_title.setFont(font);
			menu_title.setCharacterSize(60);
			menu_title.setFillColor(Color::White);
			menu_title.setPosition(500, 200);
			menu_title.setString("METAL SLUG");
			menu_survival.setFont(font);
			menu_survival.setCharacterSize(30);
			menu_survival.setFillColor(Color::White);
			menu_survival.setPosition(600, 400);
			menu_survival.setString("Press 1: Survival Mode");
			menu_campaign.setFont(font);
			menu_campaign.setCharacterSize(30);
			menu_campaign.setFillColor(Color::White);
			menu_campaign.setPosition(600, 460);
			menu_campaign.setString("Press 2: Campaign Mode");
			game_over_text.setFont(font);
			game_over_text.setCharacterSize(50);
			game_over_text.setFillColor(sf::Color::Red);
			game_over_text.setPosition(550, 350);
			game_over_text.setString("GAME OVER");
			bonus_text.setFont(font);
			bonus_text.setCharacterSize(28);
			bonus_text.setFillColor(sf::Color::Yellow);
			bonus_text.setPosition(700, 150);
			pause_text.setFont(font); pause_text.setCharacterSize(50);
			pause_text.setFillColor(sf::Color::Yellow);
			pause_resume.setFont(font); pause_resume.setCharacterSize(28);
			pause_resume.setFillColor(sf::Color::White);
			grudge_text_hud.setFont(font);
			grudge_text_hud.setCharacterSize(16);
			grudge_text_hud.setFillColor(sf::Color(255, 100, 0));
			grudge_text_hud.setPosition(10, 110);
			cowardice_text_hud.setFont(font);
			cowardice_text_hud.setCharacterSize(16);
			cowardice_text_hud.setFillColor(sf::Color(200, 0, 200));
			cowardice_text_hud.setPosition(10, 130);
			boss_hud_ground.setFont(font);
			boss_hud_ground.setCharacterSize(14);
			boss_hud_ground.setFillColor(sf::Color(255, 80, 80));
			boss_hud_ground.setPosition(600, 10);
			boss_hud_aerial.setFont(font);
			boss_hud_aerial.setCharacterSize(14);
			boss_hud_aerial.setFillColor(sf::Color(80, 180, 255));
			boss_hud_aerial.setPosition(800, 10);
			boss_hud_aquatic.setFont(font);
			boss_hud_aquatic.setCharacterSize(14);
			boss_hud_aquatic.setFillColor(sf::Color(80, 255, 200));
			boss_hud_aquatic.setPosition(1000, 10);
			boss_phase_text.setFont(font);
			boss_phase_text.setCharacterSize(18);
			boss_phase_text.setFillColor(sf::Color::Yellow);
			boss_phase_text.setPosition(680, 30);
			hp_text.setFont(font);
			hp_text.setCharacterSize(20);
			hp_text.setFillColor(sf::Color::White);
			// Damage overlay 
			red_overlay_tex.loadFromFile("25I-0555_25I-1034_Assets/red_overlay.png");
			red_overlay_sprite.setTexture(red_overlay_tex);
			float rx = 1600.0f / (float)red_overlay_tex.getSize().x;
			float ry = 900.0f / (float)red_overlay_tex.getSize().y;
			red_overlay_sprite.setScale(rx, ry);
			red_overlay_sprite.setPosition(0.0f, 0.0f);

			
			//Music
			bgm.openFromFile("25I-0555_25I-1034_Assets/bgm.ogg");
			bgm.setLoop(true);
			bgm.setVolume(50);
			if (ok_soundbuffer.loadFromFile("25I-0555_25I-1034_Assets/okay.ogg"))
				ok.setBuffer(ok_soundbuffer);
			//Time
			dt = 0;
			//Level Creation (OLD)
			//current_level = new Level(200,40,game_mode);

			//Urdu Terminal
			terminal_open = false;
			terminal_len = 0;
			terminal_buf[0] = '\0';
			terminal_text.setFont(font);
			for (int i = 0; i < 101; i++) term_key_prev[i] = false;
			terminal_text.setFont(font);
			terminal_text.setCharacterSize(18);
			terminal_text.setFillColor(sf::Color::Green);
			terminal_text.setPosition(300.f, 420.f);
			// Special features
			rain = new RainSystem(bullet_tex);
			dev_mode = new DevMode();
			echo = new EchoChamber();
			grudge = new GrudgeRegistry();
			cowardice = new CowardiceReserve();
			voice = new VoiceBeneGesserit();
			voice_startup_done = false;
			if (voice->loadReference("25I-0555_25I-1034_Assets/valar_morghulis.wav"))
				voice->startListening();
			echo->prepareAudio("25I-0555_25I-1034_Assets/bgm.wav");

			switch_cooldown = 0;
			level_select_cooldown = 0.7f;
		 menu_input_cooldown = 0.0f;
		 pause_input_cooldown = 0.0f;
		 enter_exit_vehicle_cooldown = 0.0f;
		 menu_input_cooldown_1 = 0.0f;
	  }
		~Game() {		
		for (int i = 0; i < num_characters; i++){
		 if (characters[i]->getCurrentWeapon())
			delete characters[i]->getCurrentWeapon();
               delete characters[i];
			   }
	 for (int i = 0; i < max_projectiles; i++) {
        if (projectiles[i]) {
		delete projectiles[i];
		}
    }
	delete[] projectiles;
	if (current_level) 
	  delete current_level;
		 
	if (rain) 
	  delete rain;

	if (echo) {
		delete echo;
		echo = nullptr;
	}
	if(grudge){
	delete grudge;
	grudge = nullptr;
	}
	if(cowardice){
	delete cowardice;
	cowardice = nullptr;
	}
	if (dev_mode) { delete dev_mode; 
	dev_mode = nullptr;
	}  
	if (score) { delete score;    
	score = nullptr; 
	} 
	if (voice) {
	   delete voice;
	   voice = nullptr;
	   }
	if (fusion_companion) {
		delete fusion_companion;
		fusion_companion = nullptr;
	}
	  }

		void run() {
			if (!voice_startup_done) {
				Text skip_hint;
				skip_hint.setFont(font);
				skip_hint.setCharacterSize(18);
				skip_hint.setFillColor(sf::Color(200, 200, 200));
				skip_hint.setPosition(350.0f, 450.0f);
				skip_hint.setString("Say \"Valar Morghulis\" to begin  |  Press Enter to skip");

				Clock gate_clock;
				while (window.isOpen() && !voice_startup_done) {
					float gate_dt = gate_clock.getElapsedTime().asSeconds();
					gate_clock.restart();
					if (gate_dt > 0.05f) 
					  gate_dt = 0.05f;
					Event ev;
					while (window.pollEvent(ev))
					{
						if (ev.type == Event::Closed)
						{
							window.close();
							return;
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Return))
						voice_startup_done = true;

					if (voice && voice->update(gate_dt))
						voice_startup_done = true;


					window.clear(Color::Black);
					window.draw(skip_hint);
					if (voice) 
					  voice->render(window);
					window.display();
				}

			}
			while (window.isOpen())
			{
			
				dt = frame_clock.getElapsedTime().asSeconds();
				frame_clock.restart();
				if (dt > 0.05f)
					dt = 0.05f;

				Event ev;
				while (window.pollEvent(ev))
				{
					if (ev.type == Event::Closed)
					{
						window.close();
					}
				}
				if (gamestate == 0)
				{
					handleMenuInput();
				}
				else if (gamestate == 2)  
				{
					if (!terminal_open)
					   handleInput(); 
					updateTerminalInput();
					update();
				}
				else if (gamestate == 3)
				{
					if (Keyboard::isKeyPressed(sf::Keyboard::Escape))
						window.close();
					if (Keyboard::isKeyPressed(sf::Keyboard::Return)) {
						for (int i = 0; i < num_characters; i++){						
						characters[i]->setLives(2);
						characters[i]->resetState();
						}
						delete score;
						score = new Scoring();
						active_char_index = 0;
						current_level_num = 1;
						if (current_level) {
							delete current_level;
							current_level = nullptr;
						}
						gamestate = 0;
						dev_mode->reset(characters, num_characters);
						for (int i = 0; i < 8; i++)
							campaign_kills[i] = 0;
					}

				}
				else if (gamestate == 1) { handleLevelSelectInput(); }
				else if (gamestate == 5) { handlePauseInput(); }
				else if (gamestate == 6) { handleCampaignProfileInput(); }
				render();
			}
		}

};