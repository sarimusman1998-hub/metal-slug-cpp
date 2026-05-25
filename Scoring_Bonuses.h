#pragma once
#include "SFML/Graphics.hpp"

class Scoring {
	private:
		int score;
		int kill_streak;
		float score_popup_timer;
		int score_popup_value;
		char score_popup_text[32];
		bool player_was_airborne;
	public:
		int getLength_1(const char* str) {
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
		char* copyString_1(const char* str) {
			if (!str) {
				return nullptr;
			}
			int legnth = getLength_1(str);

			char* copy = new char[legnth + 1];

			const char* read = str;
			char* write = copy;

			for (; *read != '\0';) {
				*write++ = *read++;
			}

			*write = '\0';

			return copy;
		}
		void int_to_str_1(char* copy, int n) {
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
		char* concatenate_1(const char* str1, const char* str2) {
			if (!str1 && !str2) {
				return nullptr;
			}
			if (!str1) {
				return copyString_1(str2);
			}
			if (!str2) {
				return copyString_1(str1);
			}

			int len1 = getLength_1(str1);
			int len2 = getLength_1(str2);

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
		int getScore() {
			return score;
	}
		float getPopupTimer() const {
			return score_popup_timer;
		}

		const char* getPopupText() const {
			return score_popup_text;
		}

		void update(float dt) {
			if (score_popup_timer > 0.0f) {
				score_popup_timer -= dt;
				if (score_popup_timer < 0.0f)
				   score_popup_timer = 0.0f;
			}
		}
		Scoring() {
			score = 0;
			kill_streak = 0;
			score_popup_timer = 0.0f;
			score_popup_value = 0;
			player_was_airborne = false;
			score_popup_text[0] = '\0';
	}
		void addScore(int amount, const char* label) {
			score += amount;

			score_popup_timer = 2.0f;
			score_popup_value = amount;

			char num[16];
			int_to_str_1(num, amount);
			char* full = concatenate_1(label, " +");
			char* with_num = concatenate_1(full, num);

			int len = getLength_1(with_num);
			if (len > 31) len = 31;
			for (int i = 0; i < len; i++)
				score_popup_text[i] = with_num[i];
			score_popup_text[len] = '\0';

			delete[] full;
			delete[] with_num;
		}



};