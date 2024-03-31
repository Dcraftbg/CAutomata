#include <raylib.h>
#include <gifenc.h>
#include <assert.h>
#include <string.h>
#define SCALAR 70
#define W_RATIO 16
#define H_RATIO 9
#define WIDTH (W_RATIO * SCALAR)
#define HEIGHT (H_RATIO * SCALAR)
#define BACKGROUND_COLOR GetColor(0x212121ff)

typedef enum {
	CELL_DEAD=0,
	CELL_ALIVE=1,
} CellType;


#define BOARD_W_CELLS (16*10)
#define BOARD_H_CELLS (9*10)
#define GIF_SCALAR 10
#define CELL_COLOR_ALIVE GREEN
#define CELL_COLOR_DEAD GetColor(0)
CellType cells[BOARD_W_CELLS * BOARD_H_CELLS] = { 0 };
CellType temp[BOARD_W_CELLS * BOARD_H_CELLS] = { 0 };
void RandomizeBoardGOL() {
	for (size_t i = 0; i < BOARD_W_CELLS * BOARD_H_CELLS; ++i) {
		cells[i] = GetRandomValue(0, 1);
	}
}
CellType getCell(int x, int y) {
	x = (x < 0 ? BOARD_W_CELLS + x : x) % BOARD_W_CELLS;
	y = (y < 0 ? BOARD_H_CELLS + y : y) % BOARD_H_CELLS;
	return cells[y * BOARD_W_CELLS + x];
}
typedef struct {
	size_t alive;
	size_t dead;
} Neighbours;
Neighbours getNeibours(int x, int y) {
	Neighbours n = { 0 };
	CellType ncells[8] = {
		getCell(x - 1, y - 1), getCell(x    , y - 1), getCell(x + 1, y - 1),
		getCell(x - 1, y    ),                        getCell(x + 1, y    ),
		getCell(x - 1, y + 1), getCell(x    , y + 1), getCell(x + 1, y + 1),
	};
	for (size_t i = 0; i < 8; ++i) {
		switch(ncells[i]) {
		case CELL_ALIVE: n.alive++; break;
		case CELL_DEAD:  n.dead++ ; break;
		default: assert(false && "Unreachable");
		}
	}
	return n;
}

void StepGOL() {
	for (int y = 0; y < BOARD_H_CELLS; ++y) {
		for (int x = 0; x < BOARD_W_CELLS; ++x) {
			Neighbours n = getNeibours(x, y);
			switch (cells[y * BOARD_W_CELLS + x]) {
			case CELL_ALIVE:
				if (n.alive < 2) temp[y * BOARD_W_CELLS + x] = CELL_DEAD;
				else if (n.alive > 3) temp[y * BOARD_W_CELLS + x] = CELL_DEAD;
				else temp[y * BOARD_W_CELLS + x] = CELL_ALIVE;
			break;
			case CELL_DEAD:
				if (n.alive == 3) temp[y * BOARD_W_CELLS + x] = CELL_ALIVE;
				else temp[y * BOARD_W_CELLS + x] = CELL_DEAD;
				break;
			
			}
		}
	}
	memcpy(cells, temp, (BOARD_H_CELLS * BOARD_W_CELLS * sizeof(*cells)));
}
void DrawCells(Rectangle boundary) {
	float cellWidth = boundary.width / BOARD_W_CELLS;
	float cellHeight = boundary.height / BOARD_H_CELLS;
	for (size_t y = 0; y < BOARD_H_CELLS; ++y) {
		for (size_t x = 0; x < BOARD_W_CELLS; ++x) {
			Rectangle cellRec = { .x = x * cellWidth, .y = y * cellHeight, .width = cellWidth, .height = cellHeight };
			Color color = { 0 };
			switch (cells[y * BOARD_W_CELLS + x]) {
			case CELL_ALIVE: color = CELL_COLOR_ALIVE; break;
			case CELL_DEAD: color = CELL_COLOR_DEAD; break;
			default: assert(false && "Unreachable");
			}
			DrawRectangleRec(cellRec, color);
		}
	}
}
void fillGif(ge_GIF* gif, size_t x, size_t y, size_t w, size_t h, uint8_t flag) {
	assert(gif);
	assert(x <= gif->w && y <= gif->h);
	for (size_t fy = y; fy < gif->h && fy - y < h ; ++fy) {
		for (size_t fx = x; fx < gif->w && fy - y < w; ++fx) {
			gif->frame[fy * gif->w + fx] = flag;
		}
	}
}
int main(void) {
#if 0
	ge_GIF* gif = NULL;
	gif = ge_new_gif("output.gif", BOARD_W_CELLS, BOARD_H_CELLS, (uint8_t[]) {  /* palette */
		0x00,0x00,0x00,
		0xFF,0x00,0x00,
		0x00,0xFF,0x00,
		0x00,0x00,0xFF,
		0xFF,0xFF,0xFF
	}, 3, -1, 0);
	cells[1 * BOARD_W_CELLS + 1] = CELL_ALIVE;
	cells[2 * BOARD_W_CELLS + 2] = CELL_ALIVE;
	cells[2 * BOARD_W_CELLS + 3] = CELL_ALIVE;
	cells[3 * BOARD_W_CELLS + 1] = CELL_ALIVE;
	cells[3 * BOARD_W_CELLS + 2] = CELL_ALIVE;
	//gif->frame[0] = 1;
	//gif->frame[1] = 2;
	//gif->frame[2] = 3;
	//gif->frame[3] = 4;
	//gif->frame[1 * gif->w + 1] = 3;
	//fillGif(gif, 0, 0, 20, 20, 1);
	//fillGif(gif, 0         , 0         , gif->w / 2, gif->h / 2, 0);
	//fillGif(gif, gif->w / 2, 0         , gif->w / 2, gif->h / 2, 1);
	//fillGif(gif, 0         , gif->h / 2, gif->w / 2, gif->h / 2, 2);
	//fillGif(gif, gif->w / 2, gif->w / 2, gif->w / 2, gif->h / 2, 3);
	for (size_t y = 0; y < BOARD_H_CELLS; ++y) {
		for (size_t x = 0; x < BOARD_W_CELLS; ++x) {
			fillGif(gif, x, y, 1, 1, cells[y * BOARD_W_CELLS + x]);
		}
	}
	ge_add_frame(gif, 16);
	StepGOL();
	for (size_t y = 0; y < BOARD_H_CELLS; ++y) {
		for (size_t x = 0; x < BOARD_W_CELLS; ++x) {
			fillGif(gif, x, y, 1, 1, cells[y * BOARD_W_CELLS + x]);
		}
	}
	ge_add_frame(gif, 10);
	ge_close_gif(gif);
#endif
#if 1
	InitWindow(WIDTH, HEIGHT, "Cellular Automata");
	SetTargetFPS(60);
	float stepSpeed = 0.2f;
	float stepTime = 0.0f;
	cells[1*BOARD_W_CELLS+1] = CELL_ALIVE;
	cells[2*BOARD_W_CELLS+2] = CELL_ALIVE;
	cells[2*BOARD_W_CELLS+3] = CELL_ALIVE;
	cells[3*BOARD_W_CELLS+1] = CELL_ALIVE;
	cells[3*BOARD_W_CELLS+2] = CELL_ALIVE;
	bool paused = true;
	bool rendering = false;
	ge_GIF* gif=NULL;
	size_t gifCellWidth = 0, gifCellHeight=0;
	while (!WindowShouldClose()) {
		float w = GetScreenWidth(), h = GetScreenHeight(), dt = GetFrameTime();
		if (IsKeyPressed(KEY_F10)) {
			if (rendering) {
				SetWindowTitle("Cellular Automata");
				ge_close_gif(gif);
				TraceLog(LOG_INFO, "Stopped recording..");
				gif = NULL;
				rendering = false;
			}
			else {
				SetWindowTitle("Cellular Automata (Recording GIF)");
				gif = ge_new_gif("output.gif", GIF_SCALAR*BOARD_W_CELLS, GIF_SCALAR*BOARD_H_CELLS, (uint8_t[]) {  /* palette */
					CELL_COLOR_DEAD.r, CELL_COLOR_DEAD.g, CELL_COLOR_DEAD.b,
					CELL_COLOR_ALIVE.r, CELL_COLOR_ALIVE.g, CELL_COLOR_ALIVE.b,
				},2, -1, 0);	
				assert(gif);
				gifCellWidth  = gif->w / BOARD_W_CELLS;
				gifCellHeight = gif->h / BOARD_H_CELLS;
				TraceLog(LOG_INFO, "Recording... CellWidth: %zu, CellHeight: %zu. Resolution: %zux%zu", gifCellWidth, gifCellHeight, (size_t)gif->w, (size_t)gif->h);
				rendering = true;
			}
			
		}

		if (IsKeyPressed(KEY_R)) {
			RandomizeBoardGOL();
		}
		if (IsKeyPressed(KEY_SPACE)) {
			stepTime = 0;
			paused = !paused;
		}
		if (paused && IsKeyPressed(KEY_N)) {
			StepGOL();
		}
		if (!paused) {
			stepTime += dt;
			if (stepTime >= stepSpeed) {
				StepGOL();
				stepTime = 0;
			}
		}
		Rectangle screen = {
			0, 0, w, h,
		};

		if (rendering) {
#if 1
			for (size_t y = 0; y < BOARD_H_CELLS; ++y) {
				for (size_t x = 0; x < BOARD_W_CELLS; ++x) {
					fillGif(gif, x * gifCellWidth, y * gifCellHeight, gifCellWidth, gifCellHeight, cells[y*BOARD_W_CELLS+x]);
				}
			}
#endif
			//gif->frame[0*gifCellHeight*gif->w+0*gifCellWidth] = 1;
			//gif->frame[1*gifCellHeight*gif->w+1*gifCellWidth] = 1;
			ge_add_frame(gif, 2);
		}
		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);
		DrawCells(screen);
		EndDrawing();
	}
	if (gif) ge_close_gif(gif);

	CloseWindow();
#endif
}