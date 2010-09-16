/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

void init_tune(tune_t **head, tune_t **cur, tune_t **new);
void store_tune(tune_t **cur, int x, int y, int i, int yoff);
void create_tune(tune_t **cur, tune_t **new);
void free_tune(tune_t **head, tune_t **cur);
void print_tune(tune_t **head, tune_t **cur);
tune_t* merge_tune(tune_t *left, tune_t *right);
tune_t* msort_tune(tune_t *head, int n)
void delete_tune(tune_t **head, tune_t **cur, int *n, int rx, int ry);
void save_tune(tune_t **head, tune_t **cur);
void load_tune(tune_t **head, tune_t **cur);
void play_tune(tune_t *cur, tune_t *head, int n, sound_t *sounds, int tempo);
