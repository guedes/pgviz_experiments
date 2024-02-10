/* #include "pg_shmem_visualizer.h" */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include "raylib/include/raylib.h"
#include "libpq-fe.h"

#define WIDTH  600
#define HEIGHT 700
#define WINDOW_SIZE WIDTH * HEIGHT

#define BLOCKS_LEN 100000

#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))

// https://www.color-hex.com/color-palette/41808
static Color colors[] = {
    BLACK, // empty block
    // Heat Metal Color Palette
    CLITERAL(Color){ 94,   0,   0, 255},
    CLITERAL(Color){189,  55,  10, 255},
    CLITERAL(Color){255, 155,  53, 255},
    CLITERAL(Color){249, 243, 124, 255},
    CLITERAL(Color){118,  93,  93, 255},

    CLITERAL(Color){29,  72,  119, 255},
    CLITERAL(Color){27,  138, 90 , 255},
    CLITERAL(Color){251, 176, 33 , 255},
    CLITERAL(Color){246, 136, 56 , 255},
    CLITERAL(Color){238, 62,  50 , 255},

    /* BLUE, */
    /* DARKBROWN, */
    /* YELLOW, */
    /* GREEN, */
    /* RED */
};

typedef struct BlockInfo {
  int bufferid;
  int relfilenode;
  int reltablespace;
  int reldatabase;
  int relforknumber;
  int relblocknumber;
  bool isdirty;
  int usagecount;
  int pinning_backends;
  char *relname;
} BlockInfo;

typedef struct Block {
    BlockInfo* block_info;
    Vector2 pos;
    Color color;
} Block;

BlockInfo* generate_blocks_info(size_t size)
{
    BlockInfo* blocks = (BlockInfo*) malloc(sizeof(BlockInfo) * size);
    if (blocks == NULL) {
        printf("%s:%d: ERRO: nao foi possivel alocar 'BlockInfo'", __FILE__, __LINE__);
        exit(1);
    };

    for (int i = 0; i < size; i++) {
        blocks[i].bufferid = i;
        blocks[i].relfilenode = (i % 2 == 0 ? 1000 : 2000);
        blocks[i].isdirty = (i % 4 == 0 ? true : false);
        blocks[i].usagecount = 1 + (rand() % 5);
    };

    return blocks;
};

Block* generate_blocks(size_t size)
{
    Block* blocks = (Block*) malloc(sizeof(Block) * size);
    if (blocks == NULL) {
        printf("%s:%d: ERRO: nao foi possivel alocar 'blocks'", __FILE__, __LINE__);
        exit(1);
    };

    BlockInfo* blocksinfo = generate_blocks_info(size);

    for (int i = 0; i < size; i++) {
        blocks[i].block_info = &blocksinfo[i];
        blocks[i].color = colors[blocksinfo[i].usagecount];
    };

    return blocks;
};

size_t calculate_total_pixels(int w, int h)
{
    return w * h;
}

size_t calculate_blocks_per_pixel(size_t pixels, size_t nblocks)
{
    assert(pixels > 0 && nblocks > 0);

    if (nblocks <= pixels) {
        return 1;
    }

    return nblocks / pixels;
}

void show_mouse_label(Block *blocks, size_t size)
{
    const int FONT_SIZE = 10;
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    char
        label[100]; // "id=1000000000, x=1111, y=1111, b1=11111111, b2=111111111"

    int b1 = (mouse_x / size);
    int b2 = (mouse_y / size);
    int id = (b1 + 1) * (b2 + 1);

    sprintf(label,
            "bufferid=%d\n"
            "relfilenode=%d\n"
            "usagecount=%d\n"
            "relname=%s\n",
            blocks[id].block_info->bufferid,
            blocks[id].block_info->relfilenode,
            blocks[id].block_info->usagecount,
            blocks[id].block_info->relname);

    DrawRectangle(mouse_x + 2, mouse_y + 2, 110 + MeasureText(blocks[id].block_info->relname, FONT_SIZE), 70, BLACK);
    DrawText(label, mouse_x + 10, mouse_y + 10, FONT_SIZE, WHITE);
}

int main(int argc, char *argv[])
{
	const char *conninfo;
	PGconn	   *conn;
	PGresult   *res;
    size_t      size;

    if (argc < 2) {
        size = 2;
    } else {
        size = strtoul(argv[1], NULL, 10);
    }

    srand(time(0));

    InitWindow(WIDTH, HEIGHT, "PG Shared Buffer Visualizer!");
    SetTargetFPS(10);
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    conninfo = "host=/var/run/postgresql port=5437";
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "%s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x161616AA));
        int w = GetScreenWidth();
        int h = GetScreenHeight();

        int pixels = calculate_total_pixels(w, h);
        size_t blocks_per_pixel = calculate_blocks_per_pixel(pixels, BLOCKS_LEN);

        bool has_blocks = true;

        res = PQexec(conn,
                     "select bufferid, coalesce(usagecount, 0) as usagecount, relfilenode, coalesce(cls.relname, '-') as relname "
                     "from pg_buffercache buf "
                     "left join pg_catalog.pg_class cls using (relfilenode);");

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            fprintf(stderr, "%s", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn);
            exit(1);
        }

        Block *blocks = malloc(sizeof(Block) * PQntuples(res));

        for (int i=0; i < PQntuples(res); i++) {
            char *bufferid_ptr = PQgetvalue(res, i, 0);
            char *usagecount_ptr = PQgetvalue(res, i, 1);
            char *relfilenode_ptr = PQgetvalue(res, i, 2);
            char *relname_ptr = PQgetvalue(res, i, 3);

            int bufferid = strtol(bufferid_ptr, NULL, 10);
            int usagecount = strtol(usagecount_ptr, NULL, 10);
            int relfilenode = strtol(relfilenode_ptr, NULL, 10);

            int x = bufferid % (w / size);
            int y = bufferid / (w / size);

            BlockInfo *block_info = malloc(sizeof(BlockInfo));
            block_info->bufferid = bufferid;
            block_info->usagecount = usagecount;
            block_info->relfilenode = relfilenode;
            block_info->relname = relname_ptr;

            blocks[i].block_info = block_info;
            blocks[i].pos.x = x * size;
            blocks[i].pos.y = y * size;

            if (size > 1) {
                DrawRectangle(blocks[i].pos.x, blocks[i].pos.y, size, size, colors[blocks[i].block_info->usagecount - 1]);
            } else {
                DrawPixel(blocks[i].pos.x, blocks[i].pos.y, colors[blocks[i].block_info->usagecount - 1]);
            }
        }

        show_mouse_label(blocks, size);
        EndDrawing();

        for (int i=0; i < PQntuples(res); i++) {
            free(blocks[i].block_info);
        }
        free(blocks);
        PQclear(res);
    }
    CloseWindow();
    return 0;
}
