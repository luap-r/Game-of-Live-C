#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  
  int state;
} CELL;

typedef struct INNER_ARRAY{

  CELL *cell;
  struct INNER_ARRAY *next;

} INNER_ARRAY;

typedef struct OUTER_ARRAY{

  INNER_ARRAY *i_array;
  struct OUTER_ARRAY *next;

} OUTER_ARRAY;

CELL *get_array_cell(OUTER_ARRAY *grid, int x, int y, int width, int height){

  if(x>width-1 || x<0 || y>height-1 || y<0){
    CELL *c_null = malloc(sizeof(CELL));
    c_null->state = 0;
    return c_null;
  } 
  

  OUTER_ARRAY *current = grid;

  for(int i = 0; i < x; i++){

    current = current->next;
  }

  INNER_ARRAY *inner_current = current->i_array;

  for(int i = 0; i < y; i++){

    inner_current = inner_current->next;
  }

  return inner_current->cell;

}

void put_array_cell(OUTER_ARRAY *grid, int x, int y, int sstate, int width, int height){

  CELL *cell = get_array_cell(grid, x, y, width, height);

  cell->state = sstate;

}

OUTER_ARRAY *make_2D_array(int width, int height){

  OUTER_ARRAY *grid = malloc(sizeof(OUTER_ARRAY));
  OUTER_ARRAY *current = grid;

  for(int i = 0; i < width; i++){

    current->i_array = malloc(sizeof(INNER_ARRAY));
    INNER_ARRAY *col = current->i_array;

    for(int j = 0; j < height; j++){
      col->cell = malloc(sizeof(CELL));
      col->cell->state = 0;

      if (j < height - 1) {
        col->next = malloc(sizeof(INNER_ARRAY));
        col = col->next;
      } else {
        col->next = NULL; 
      }
    }

    if (i < width - 1) {
      current->next = malloc(sizeof(OUTER_ARRAY));
      current = current->next;
    } else {
      current->next = NULL;
    }
  }

  return grid;

}

void free_2D_array(OUTER_ARRAY *grid, int width, int height){
  
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      
      free(grid->i_array->cell);

      INNER_ARRAY *tmp = grid->i_array->next;
      grid->i_array = tmp;

    }
    free(grid->i_array);
    grid = grid->next;

  }
}

char state_to_ascii(int state){

  if(state == 1){
    return '@';
  } else {
    return ' ';
  }
}

void grid_show(OUTER_ARRAY *grid, int width, int height){

  for(int x = 0; x < width; x++){

    for(int y = 0; y < height; y++){
      printf("%c", state_to_ascii( get_array_cell(grid, x, y, width, height)->state) );
    }
    printf("\n");
  } 
}

void copy_to_array(OUTER_ARRAY *from, OUTER_ARRAY *to, int width, int height){

  OUTER_ARRAY *current_from = from;
  OUTER_ARRAY *current_to = to;

  for (int x = 0; x < width; x++) {

    INNER_ARRAY *inner_from = current_from->i_array;
    INNER_ARRAY *inner_to = current_to->i_array;

    for (int y = 0; y < height; y++) {

      inner_to->cell->state = inner_from->cell->state;

      inner_from = inner_from->next;
      inner_to = inner_to->next;
    }

    current_from = current_from->next;
    current_to = current_to->next;
  }
}

int count_n(OUTER_ARRAY *grid, int x, int y, int width, int height){

  int count = 0;

  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <=1; j++){

      if(i == 0 && j == 0){
        continue;
      }

      count += get_array_cell(grid, x + i , y + j, width, height)->state; 
    }
  }

  return count;
}

void update(OUTER_ARRAY *grid, OUTER_ARRAY *grid_old, int width, int height){

  copy_to_array(grid, grid_old, width, height);

  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){

      // Count live neighbours of every cell
      int neighbours = count_n(grid_old, x, y, width, height);
      
      if(get_array_cell(grid_old, x, y, width, height)->state == 0 && neighbours == 3){
        put_array_cell(grid, x, y, 1, width, height);
      }
      else if(get_array_cell(grid_old, x, y, width, height)->state == 1 && (neighbours < 2 || neighbours > 3)){
        put_array_cell(grid, x, y, 0, width, height);
      }
      else {
        put_array_cell(grid, x, y, get_array_cell(grid_old, x, y, width, height)->state, width, height); 
      }
      
    }
  }

}

int main(){
  
  // Array of cells in an array
  
  int width = 50;
  int height = 50;
  int running = 1;
  int starting_time = clock();

  OUTER_ARRAY *grid = make_2D_array(width, height);
  OUTER_ARRAY *grid_old = make_2D_array(width, height);

  put_array_cell(grid, 3, 3, 1, width, height);
  put_array_cell(grid, 4, 3, 1, width, height);
  put_array_cell(grid, 5, 2, 1, width, height);
  put_array_cell(grid, 5, 3, 1, width, height);
  put_array_cell(grid, 4, 1, 1, width, height);

  while (running) {

    system("clear");

    grid_show(grid, width, height);
    update(grid, grid_old, width, height);
    
    int time = clock();
    int time_e = time + 100000;
    while (time < time_e) {
      time = clock();
    }
    if(clock()-starting_time > 1000000){
      running = 0;
    }
  }

  free_2D_array(grid, width, height);
  free_2D_array(grid_old, width, height);
}
