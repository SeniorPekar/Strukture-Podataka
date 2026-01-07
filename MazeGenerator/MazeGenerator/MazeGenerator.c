#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>   /* _getch na Windowsu */
#include <stdbool.h>
#include <string.h>

typedef struct mazeNode mazeNode;
typedef mazeNode* position;

struct mazeNode {
    position left;
    position right;
    position above;
    position below;
    bool wallLeft;
    bool wallRight;
    bool wallAbove;
    bool wallBelow;
    bool visited;
    bool revealed; /* je li korisnik otkrio ovu æeliju */
    int x;
    int y;
};

static position** createGrid(int width, int height);
static int destroyGrid(position** grid, int width, int height);
static int linkGrid(position** grid, int width, int height);
static int generateMaze(position** grid, int width, int height);
static int printMaze(position** grid, int width, int height, position player, position goal, bool showAll, bool** pathmap);
static position neighborByDirection(position p, char dir);

static int findShortestPath(position** grid, int width, int height, position start, position goal, bool** outPath);
static int shortestPathLength(position** grid, int width, int height, position start, position goal);

static bool isAdjacent(position a, position b);
static bool cellVisible(position cell, position player, bool showAll);
/* oznaèi æeliju i njezine izravne susjede kao otkrivene */
static int revealCellAndNeighbors(position p);

static int runPrintMode(position** grid, int width, int height, position player, position goal);
static int runPlayMode(position** grid, int width, int height, position player, position goal);

int main() {
    int width = 20;
    int height = 10;
    int mode = 0;

    printf("Pocetna velicina labirinta: %dx%d \n", width, height);
    printf("Unesite sirinu i visinu (ili unesite bilo sta drugo za korištenje pocetnih vrijednosti): ");
    //fflush(stdout);

    /* ako korisnik unese nove vrijednosti */
    if (scanf("%d %d", &width, &height) != 2) {
        /* oèisti preostali unos */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        width = 20;
        height = 10;
    }

    if (width < 2) width = 2; /* minimalna širina */
    if (height < 2) height = 2; /* minimalna visina */

    printf("Odaberi mod:\n");
    printf("  1 - Igraj (WASD) s maglom (samo trenutni zidovi i otkrivene celije su vidljive)\n");
    printf("  2 - Ispiši cijeli labirint i najkraci put (bez navigacije)\n");
    printf("Unesite 1 ili 2: ");
    //fflush(stdout);
    if (scanf("%d", &mode) != 1) {
        mode = 1;
    }

    srand((unsigned int)time(NULL));

    position** grid = createGrid(width, height);
    if (!grid) {
        fprintf(stderr, "Alokacija neuspjela\n");
        return 1;
    }

    linkGrid(grid, width, height);
    generateMaze(grid, width, height);

    /* Odaberi sluèajan poèetak i cilj, pazeæi da najkraæi put ne bude premalen */
    position player = NULL;
    position goal = NULL;

    int total = width * height;
    /*barem (width+height)/2 ili 1/10 æelija, veæe od toga */
    int minLen = (width + height) / 2;
    int alt = (width * height) / 10;
    if (alt > minLen) minLen = alt;
    if (minLen < 2) minLen = 2;

    int maxAttempts = 2000;
    int attempts = 0;

    while (attempts++ < maxAttempts) {
        int sr = rand() % height;
        int sc = rand() % width;
        int gr = rand() % height;
        int gc = rand() % width;
        if (sr == gr && sc == gc) continue;
        position s = grid[sr][sc];
        position g = grid[gr][gc];
        int len = shortestPathLength(grid, width, height, s, g);
        if (len >= minLen) {
            player = s;
            goal = g;
            break;
        }
    }

    /* Ako se nije uspjelo pronaæi par, vratimo se na kutove */
    if (!player || !goal) {
        player = grid[0][0];
        goal = grid[height - 1][width - 1];
    }

    /* Otkrivanje poèetne pozicije i susjeda radi preglednosti */
    revealCellAndNeighbors(player);

    int rc = 0;
    if (mode == 2) {
        rc = runPrintMode(grid, width, height, player, goal);
    } else {
        rc = runPlayMode(grid, width, height, player, goal);
    }

    destroyGrid(grid, width, height);
    return rc == 0 ? 0 : 1;
}
static int runPrintMode(position** grid, int width, int height, position player, position goal) {
    bool** pathmap = (bool**)malloc(sizeof(bool*) * height);
    if (!pathmap) {
        fprintf(stderr, "Alokacija neuspjela\n");
        return -1;
    }
    for (int r = 0; r < height; ++r) {
        pathmap[r] = (bool*)malloc(sizeof(bool) * width);
        if (!pathmap[r]) {
            for (int i = 0; i < r; ++i) free(pathmap[i]);
            free(pathmap);
            fprintf(stderr, "Alokacija neuspjela\n");
            return -1;
        }
        memset(pathmap[r], 0, sizeof(bool) * width);
    }

    findShortestPath(grid, width, height, player, goal, pathmap);

    printMaze(grid, width, height, NULL, goal, true, pathmap);

    for (int r = 0; r < height; ++r) free(pathmap[r]);
    free(pathmap);
    return 0;
}

static int runPlayMode(position** grid, int width, int height, position player, position goal) {
    printMaze(grid, width, height, player, goal, false, NULL);
    printf("Koristite WASD za kretanje. Doðite do cilja (oznaèenog G). Pritisnite Esc za izlaz.\n");

    for (;;) {
        int ch = _getch();
        if (ch == 27) return 0; /* ESC */

        char cmd = (char)ch;
        if (cmd >= 'A' && cmd <= 'Z') cmd = (char)(cmd - 'A' + 'a');

        position dest = NULL;
        if (cmd == 'w' && !player->wallAbove) dest = player->above;
        else if (cmd == 's' && !player->wallBelow) dest = player->below;
        else if (cmd == 'a' && !player->wallLeft) dest = player->left;
        else if (cmd == 'd' && !player->wallRight) dest = player->right;

        if (dest) {
            player = dest;
            /* otkrij novu æeliju i susjede */
            revealCellAndNeighbors(player);
        }

        system("cls");
        printMaze(grid, width, height, player, goal, false, NULL);

        if (player == goal) {
            printf("Stigli ste do cilja! Pritisnite bilo koju tipku za izlaz.\n");
            _getch();
            return 0;
        }
    }
}

/* Kreiraj mrežu èvorova, zidovi inicijalno zatvoreni */
static position** createGrid(int width, int height) {
    position** grid = (position**)malloc(sizeof(position*) * height);
    if (!grid) return NULL;

    for (int r = 0; r < height; ++r) {
        grid[r] = (position*)malloc(sizeof(position) * width);
        if (!grid[r]) {
            /* obriši veæ alocirano ako je došlo do greške */
            for (int i = 0; i < r; ++i) {
                if (grid[i]) {
                    for (int j = 0; j < width; ++j) free(grid[i][j]);
                    free(grid[i]);
                }
            }
            free(grid);
            return NULL;
        }
        for (int c = 0; c < width; ++c) {
            position p = (position)malloc(sizeof(mazeNode));
            if (!p) {
                /* obriši sve do sada alocirano */
                for (int i = 0; i <= r; ++i) {
                    int limit = (i == r) ? c : width;
                    for (int j = 0; j < limit; ++j) free((void*)grid[i][j]);
                    free(grid[i]);
                }
                free(grid);
                return NULL;
            }
            p->left = p->right = p->above = p->below = NULL; // inicijaliziraj pokazivaèe
            p->wallLeft = p->wallRight = p->wallAbove = p->wallBelow = true;
            p->visited = false;
            p->revealed = false;
            p->x = c;
            p->y = r;
            grid[r][c] = p; // spremi èvor u mrežu
        }
    }
    return grid;
}

/* Poveži pokazivaèe susjeda za svaki èvor */   
static int linkGrid(position** grid, int width, int height) {
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            position p = grid[r][c];
            p->left = (c > 0) ? grid[r][c - 1] : NULL;
            p->right = (c < width - 1) ? grid[r][c + 1] : NULL;
            p->above = (r > 0) ? grid[r - 1][c] : NULL;
            p->below = (r < height - 1) ? grid[r + 1][c] : NULL;
        }
    }
    return 0;
}

/* Generiraj labirint pomoæu nasumiènog DFS-a (iterativni stek) */
static int generateMaze(position** grid, int width, int height) {
    int total = width * height;
    position* stack = (position*)malloc(sizeof(position) * total);
    if (!stack) return -2;

    int top = 0;
    position start = grid[0][0];
    start->visited = true;
    stack[top++] = start;

    while (top > 0) {
        position current = stack[top - 1];

        /* prikupi neposjeæene susjede */
        position neighbors[4];
        char dirs[4];
        int ncount = 0;

        if (current->above && !current->above->visited) { neighbors[ncount] = current->above; dirs[ncount] = 'u'; ++ncount; }
        if (current->below && !current->below->visited) { neighbors[ncount] = current->below; dirs[ncount] = 'd'; ++ncount; }
        if (current->left && !current->left->visited) { neighbors[ncount] = current->left; dirs[ncount] = 'l'; ++ncount; }
        if (current->right && !current->right->visited) { neighbors[ncount] = current->right; dirs[ncount] = 'r'; ++ncount; }

        if (ncount == 0) {
            /* povratak u stogu */
            --top;
            continue;
        }

        int pick = rand() % ncount;
        position chosen = neighbors[pick];
        char dir = dirs[pick];

        /* ukloni zid izmeðu trenutnog i izabranog susjeda */
        if (dir == 'u') {
            current->wallAbove = false;
            chosen->wallBelow = false;
        } else if (dir == 'd') {
            current->wallBelow = false;
            chosen->wallAbove = false;
        } else if (dir == 'l') {
            current->wallLeft = false;
            chosen->wallRight = false;
        } else if (dir == 'r') {
            current->wallRight = false;
            chosen->wallLeft = false;
        }

        chosen->visited = true;
        stack[top++] = chosen;
    }

    /* oslobodi stek */
    free(stack);

    /* Resetiraj oznake posjeæenosti */
    for (int r = 0; r < height; ++r)
        for (int c = 0; c < width; ++c)
            grid[r][c]->visited = false;
    return 0;
}

/* provjeri je li æelija vidljiva prema pravilima magle */
static bool cellVisible(position cell, position player, bool showAll) {
    if (!cell) return false;
    if (showAll) return true;
    if (cell->revealed) return true;
    if (player && isAdjacent(cell, player)) return true;
    return false;
}

/* Ispiši ASCII labirint:
    ako je showAll true, otkrij cijeli labirint
    ako je pathmap dostavljen, oznaèi najkraæi put znakom '*' */
static int printMaze(position** grid, int width, int height, position player, position goal, bool showAll, bool** pathmap) {
    /* gornja granica */
    for (int c = 0; c < width; ++c) {
        position cell = grid[0][c];
        bool vis = cellVisible(cell, player, showAll);
        printf("+");
        if (vis) {
            if (cell->wallAbove) printf("---"); else printf("   ");
        } else {
            printf("   ");
        }
    }
    printf("+\n");

    for (int r = 0; r < height; ++r) {
        /* linija s lijevim zidovima i sadržajem æelija */
        for (int c = 0; c < width; ++c) {
            position cell = grid[r][c];
            bool vis = cellVisible(cell, player, showAll);

            /* lijevi zid */
            if (vis) {
                if (cell->wallLeft) printf("|"); else printf(" ");
            } else {
                printf(" ");
            }

            /* sadržaj æelije */
            if (pathmap && pathmap[r][c]) {
                printf(" * ");
            } else if (player && cell == player) {
                printf(" P ");
            } else if (goal && cell == goal) {
                printf(" G ");
            } else {
                printf("   ");
            }
        }

        /* desni rub retka: posljednja æelija */
        position last = grid[r][width - 1];
        bool lastVis = cellVisible(last, player, showAll);
        if (lastVis) {
            if (last->wallRight) printf("|");
            else printf(" ");
        } else {
            printf(" ");
        }
        printf("\n");

        /* granica s donjim zidovima */
        for (int c = 0; c < width; ++c) {
            position cell = grid[r][c];
            bool vis = cellVisible(cell, player, showAll);
            printf("+");
            if (vis) {
                if (cell->wallBelow) printf("---"); else printf("   ");
            } else {
                printf("   ");
            }
        }
        printf("+\n");
    }
    return 0;
}

/* Oznaèi æeliju i njene susjede kao otkrivene (trajno) */
static int revealCellAndNeighbors(position p) {
    if (!p) return -1;
    p->revealed = true;
    if (p->above) p->above->revealed = true;
    if (p->below) p->below->revealed = true;
    if (p->left) p->left->revealed = true;
    if (p->right) p->right->revealed = true;
    return 0;
}

/* Oslobodi sve èvorove i polja mreže */
static int destroyGrid(position** grid, int width, int height) {
    if (!grid) return -1   ;
    for (int r = 0; r < height; ++r) {
        if (!grid[r]) continue;
        for (int c = 0; c < width; ++c) {
            free((void*)grid[r][c]);
        }
        free(grid[r]);
    }
    free(grid);
    return 0;
}
/*
static position neighborByDirection(position p, char dir) {
    if (!p) return NULL;
    if (dir == 'w' || dir == 'u') return p->above;
    if (dir == 's' || dir == 'd') return p->below;
    if (dir == 'a' || dir == 'l') return p->left;
    if (dir == 'r') return p->right;
    return NULL;
}*/

/* Vrati true ako je a ista kao b ili susjedna b (4-smjerova) */
static bool isAdjacent(position a, position b) {
    if (!a || !b) return false;
    if (a == b) return true;
    if (a == b->left || a == b->right || a == b->above || a == b->below) return true;
    return false;
}

/* "merged BFS" traži najskraæi put i ispunjuje ga */
static int findShortestPath(position** grid, int width, int height, position start, position goal, bool** outPath) {
    if (!grid || !start || !goal) return -1;

    int total = width * height;
    int *prev = (int*)malloc(sizeof(int) * total);
    if (!prev) return -1;
    for (int i = 0; i < total; ++i) prev[i] = -1;

    bool *seen = (bool*)malloc(sizeof(bool) * total);
    if (!seen) { free(prev); return -1; }
    for (int i = 0; i < total; ++i) seen[i] = false;

    int *queue = (int*)malloc(sizeof(int) * total);
    if (!queue) { free(prev); free(seen); return -1; }

    int qs = 0, qe = 0;
    int startIdx = start->y * width + start->x;
    int goalIdx = goal->y * width + goal->x;

    queue[qe++] = startIdx;
    seen[startIdx] = true;
    prev[startIdx] = -1;

    while (qs < qe) {
        int idx = queue[qs++];
        if (idx == goalIdx) break;

        int r = idx / width;
        int c = idx % width;
        position cell = grid[r][c];

        /* gore */
        if (!cell->wallAbove && cell->above) {
            int ni = (r - 1) * width + c;
            if (!seen[ni]) { seen[ni] = true; prev[ni] = idx; queue[qe++] = ni; }
        }
        /* dolje */
        if (!cell->wallBelow && cell->below) {
            int ni = (r + 1) * width + c;
            if (!seen[ni]) { seen[ni] = true; prev[ni] = idx; queue[qe++] = ni; }
        }
        /* lijevo */
        if (!cell->wallLeft && cell->left) {
            int ni = r * width + (c - 1);
            if (!seen[ni]) { seen[ni] = true; prev[ni] = idx; queue[qe++] = ni; }
        }
        /* desno */
        if (!cell->wallRight && cell->right) {
            int ni = r * width + (c + 1);
            if (!seen[ni]) { seen[ni] = true; prev[ni] = idx; queue[qe++] = ni; }
        }
    }

    /* Ako je zatražena mapa puta, inicijaliziraj je na false */
    if (outPath) {
        for (int rr = 0; rr < height; ++rr)
            for (int cc = 0; cc < width; ++cc)
                outPath[rr][cc] = false;
    }

    if (!seen[goalIdx]) {
        /* nedostupno */
        free(prev);
        free(seen);
        free(queue);
        return -1;
    }

    /* Rekonstruiraj put i prebroj èvorove */
    int len = 0;
    int cur = goalIdx;
    while (cur != -1) {
        int rr = cur / width;
        int cc = cur % width;
        if (outPath) outPath[rr][cc] = true;
        ++len;
        if (cur == startIdx) break;
        cur = prev[cur];
    }

    free(prev);
    free(seen);
    free(queue);
    return len;
}

static int shortestPathLength(position** grid, int width, int height, position start, position goal) {
    return findShortestPath(grid, width, height, start, goal, NULL);
}
