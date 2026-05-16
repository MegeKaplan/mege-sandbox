#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define GRID_W 420
#define GRID_H 240
#define TILE_SIZE 4

struct WindowConfig
{
  Vector2 size;
  const char *title;
};

typedef enum
{
  CELL_EMPTY,
  CELL_SAND,
  CELL_WATER,
  CELL_ACID,
  CELL_SOIL,
  CELL_ROCK,
  CELL_BLACK_HOLE
} CellType;

struct Cell
{
  CellType type;
  Color color;
  int density;
  int steps;
};

typedef enum
{
  FLOOR_STOP,
  FLOOR_DELETE,
  FLOOR_WRAP,
} FloorMode;

struct Cell createCell(CellType type, Color color, int density, int steps)
{
  struct Cell cell;
  cell.type = type;
  cell.color = color;
  cell.density = density;
  cell.steps = steps;
  return cell;
}

struct Cell createSand()
{
  return createCell(CELL_SAND, ColorBrightness(BEIGE, GetRandomValue(-20, 20) / 100.f), 20, 2);
}

struct Cell createEmpty()
{
  return createCell(CELL_EMPTY, ColorBrightness(GRAY, GetRandomValue(-20, 20) / 100.0f), 0, 1);
}

struct Cell createWater()
{
  return createCell(CELL_WATER, ColorBrightness(BLUE, GetRandomValue(-10, 10) / 100.0f), 10, 30);
}

struct Cell createAcid()
{
  return createCell(CELL_ACID, ColorBrightness(GREEN, GetRandomValue(-10, 10) / 100.0f), 5, 20);
}

struct Cell createSoil()
{
  return createCell(CELL_SOIL, ColorBrightness(BROWN, GetRandomValue(-20, 20) / 100.0f), 20, 1);
}

struct Cell createRock()
{
  return createCell(CELL_ROCK, ColorBrightness(GRAY, GetRandomValue(-20, 20) / 100.0f), 100, 0);
}

struct Cell createBlackHole()
{
  return createCell(CELL_BLACK_HOLE, BLACK, 1000, 0);
}

void updateSand(struct Cell grid[GRID_W][GRID_H], int x, int y)
{
  int ny = y + 1;

  if (ny > GRID_H - 1)
    return;

  if (grid[x][ny].type == CELL_EMPTY)
  {
    grid[x][ny] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  int dir = GetRandomValue(0, 1);
  int nx = x + (dir ? 1 : -1);

  if (nx < 0 || nx > GRID_W - 1)
    return;

  if (grid[nx][ny].type == CELL_EMPTY)
  {
    grid[nx][ny] = grid[x][y];
    grid[x][y] = createEmpty();
  }

  if (grid[nx][ny].density < grid[x][y].density)
  {
    struct Cell temp = grid[x][y];
    grid[x][y] = grid[nx][ny];
    grid[nx][ny] = temp;
  }
}

void updateWater(struct Cell grid[GRID_W][GRID_H], int x, int y)
{
  int ny = y + 1;

  if (ny > GRID_H - 1)
    return;

  if (grid[x][ny].type == CELL_EMPTY)
  {
    grid[x][y + 1] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  int dir = GetRandomValue(0, 1);
  int nx = x + (dir ? 1 : -1);

  if (nx < 0 || nx > GRID_W - 1)
    return;

  if (grid[nx][ny].type == CELL_EMPTY)
  {
    grid[nx][ny] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  if (grid[nx][y].type == CELL_EMPTY)
  {
    grid[nx][y] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }
}

void updateAcid(struct Cell grid[GRID_W][GRID_H], int x, int y)
{
  int ny = y + 1;

  if (ny > GRID_H - 1)
    return;

  if (grid[x][ny].type == CELL_EMPTY)
  {
    grid[x][y + 1] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  int dir = GetRandomValue(0, 1);
  int nx = x + (dir ? 1 : -1);

  if (nx < 0 || nx > GRID_W - 1)
    return;

  if (grid[nx][ny].type == CELL_EMPTY)
  {
    grid[nx][ny] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  if (grid[nx][y].type == CELL_EMPTY)
  {
    grid[nx][y] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  if (grid[nx][ny].density < grid[x][y].density)
  {
    struct Cell temp = grid[x][y];
    grid[x][y] = grid[nx][ny];
    grid[nx][ny] = temp;
    return;
  }

  if (GetRandomValue(0, 1000) < 1)
  {
    grid[nx][ny] = createEmpty();
  }
}

void updateSoil(struct Cell grid[GRID_W][GRID_H], int x, int y)
{
  int ny = y + 1;

  if (ny > GRID_H - 1)
    return;

  if (grid[x][ny].type == CELL_EMPTY)
  {
    grid[x][y + 1] = grid[x][y];
    grid[x][y] = createEmpty();
    return;
  }

  int dir = GetRandomValue(0, 1);
  int nx = x + (dir ? 1 : -1);

  if (nx < 0 || nx > GRID_W - 1)
    return;

  if (grid[nx][ny].type == CELL_EMPTY)
  {
    grid[nx][ny] = grid[x][y];
    grid[x][y] = createEmpty();
  }

  if (grid[nx][ny].density < grid[x][y].density)
  {
    struct Cell temp = grid[x][y];
    grid[x][y] = grid[nx][ny];
    grid[nx][ny] = temp;
  }
}

void updateBlackHole(struct Cell grid[GRID_W][GRID_H], int x, int y)
{
  // black hole update logic
}

int main()
{
  struct WindowConfig windowConfig = {{GRID_W * TILE_SIZE, GRID_H * TILE_SIZE}, "Mege Sandbox"};
  InitWindow(windowConfig.size.x, windowConfig.size.y, windowConfig.title);
  SetTargetFPS(120);

  struct Cell grid[GRID_W][GRID_H] = {0};

  for (int y = 0; y < GRID_H; y++)
  {
    for (int x = 0; x < GRID_W; x++)
    {
      grid[x][y] = createEmpty();
    }
  }

  int maxBrushSize = 20;
  int minBrushSize = 3;
  int brushSize = (int)(maxBrushSize / 2);
  bool brushVisibility = false;
  float brushVisibilityTimer = 0.0f;

  CellType currentCellType = CELL_SAND;

  int clearing = 0;
  int clearRow = 0;

  FloorMode floorMode = FLOOR_STOP;

  Texture2D brushTexture = LoadTexture("assets/brush.png");
  Texture2D sandLogoTexture = LoadTexture("assets/sand-logo.png");

  int gameStarted = 0;
  float gameStartedTimer = 0.5f;
  int mainMenuButtonClicked = 0;
  int mainMenuLogoScale = 1.0f;
  char *mainMenuLogoText = "Mege Sandbox";
  char *mainMenuButtonText = "Start";

  int mouseButtonLeftDown = 0;

  int drawGrid = 0;

  float t = 0;

  char *hints[] = {
      "Press G to toggle grid",
      "Use mouse wheel to change brush size",
      "Press Delete to clear the grid",
      "Press S, D and W to change floor mode",
      "Press 1-7 to change cell type",
      "Press H to hide hints",
  };
  int hintsCount = sizeof(hints) / sizeof(hints[0]);
  int currentHint = 0;
  float hintTimer = 0.0f;
  float hintDisplayTime = 5.0f;
  bool showHints = true;

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    float dt = GetFrameTime();
    t += dt;

    Vector2 mousePos = GetMousePosition();
    int gx = mousePos.x / TILE_SIZE;
    int gy = mousePos.y / TILE_SIZE;

    if (!gameStarted)
    {
      DrawTextureEx(sandLogoTexture, (Vector2){windowConfig.size.x / 2 - sandLogoTexture.width / 2 * 2.5, 150 + -fabs(sin(t * 2)) * 20}, 0.0f, 2.5f, WHITE);
      DrawText(mainMenuLogoText, windowConfig.size.x / 2 - MeasureText(mainMenuLogoText, 50) / 2, 400, 50, BROWN);

      DrawRectangle(windowConfig.size.x / 2 - MeasureText(mainMenuButtonText, 50) / 2 - 20, windowConfig.size.y / 2 + 10, MeasureText(mainMenuButtonText, 50) + 40, 60, DARKBROWN);
      DrawRectangle(windowConfig.size.x / 2 - MeasureText(mainMenuButtonText, 50) / 2 - 20, windowConfig.size.y / 2 + (mainMenuButtonClicked ? 5 : 0), MeasureText(mainMenuButtonText, 50) + 40, 60, BROWN);
      DrawText(mainMenuButtonText, windowConfig.size.x / 2 - MeasureText(mainMenuButtonText, 50) / 2, windowConfig.size.y / 2 + 5 + (mainMenuButtonClicked ? 5 : 0), 50, WHITE);

      if (
          mousePos.x > windowConfig.size.x / 2 - MeasureText(mainMenuButtonText, 50) / 2 - 20 &&
              mousePos.x < windowConfig.size.x / 2 - MeasureText(mainMenuButtonText, 50) / 2 - 20 + MeasureText(mainMenuButtonText, 50) + 40 &&
              mousePos.y > windowConfig.size.y / 2 &&
              mousePos.y < windowConfig.size.y / 2 + 60 &&
              IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
          (IsKeyDown(KEY_SPACE)) || IsKeyDown(KEY_ENTER))
      {
        mainMenuButtonClicked = 1;
      }
    }
    if (mainMenuButtonClicked)
    {
      gameStartedTimer -= dt;
      if (gameStartedTimer < 0)
      {
        gameStarted = 1;
      }
    }

    if (gameStarted)
    {
      HideCursor();

      if (drawGrid)
      {
        for (int y = 0; y < GRID_H; y++)
        {
          for (int x = 0; x < GRID_W; x++)
          {
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
          }
        }
      }

      if (IsKeyPressed(KEY_G))
      {
        drawGrid = !drawGrid;
      }

      for (int y = -brushSize + 1; y < brushSize; y++)
      {
        for (int x = -brushSize + 1; x < brushSize; x++)
          if (brushVisibility)
          {
            if (x * x + y * y < brushSize * brushSize)
            {
              // DrawRectangle((gx + x) * TILE_SIZE, (gy + y) * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
              if (GetRandomValue(0, 100) < 20)
              {
                DrawRectangle((gx + x) * TILE_SIZE, (gy + y) * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
              }
            }
          }
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      {
        mouseButtonLeftDown = 1;
      }
      else
      {
        mouseButtonLeftDown = 0;
      }

      if (mouseButtonLeftDown)
      {
        for (int y = -brushSize + 1; y < brushSize; y++)
        {
          for (int x = -brushSize + 1; x < brushSize; x++)
          {
            if (!(gy + y > GRID_H - 1 || gx + x < 0 || gx + x > GRID_W - 1 || gy + y < 0))
              // if (!(gy + y > GRID_H - 1 || gx + x < 0 || gx + x > GRID_W - 1) && grid[gx + x][gy + y].type == CELL_EMPTY)
              if (x * x + y * y < brushSize * brushSize)
              {
                if (GetRandomValue(0, 100) < 3)
                {
                  switch (currentCellType)
                  {
                  case CELL_SAND:
                    grid[gx + x][gy + y] = createSand();
                    break;
                  case CELL_WATER:
                    grid[gx + x][gy + y] = createWater();
                    break;
                  case CELL_ACID:
                    grid[gx + x][gy + y] = createAcid();
                    break;
                  case CELL_SOIL:
                    grid[gx + x][gy + y] = createSoil();
                    break;
                  default:
                    break;
                  }
                }
                switch (currentCellType)
                {
                case CELL_EMPTY:
                  grid[gx + x][gy + y] = createEmpty();
                  break;
                case CELL_ROCK:
                  grid[gx + x][gy + y] = createRock();
                  break;
                case CELL_BLACK_HOLE:
                  grid[gx + x][gy + y] = createBlackHole();
                  break;
                default:
                  break;
                }
              }
          }
        }
      }

      float wheel = GetMouseWheelMove();
      if (wheel != 0)
      {
        brushVisibility = true;
        brushVisibilityTimer = 0.5f;
        brushSize += (int)wheel;
        if (brushSize < minBrushSize)
          brushSize = minBrushSize;
        if (brushSize > maxBrushSize)
          brushSize = maxBrushSize;
      }

      if (brushVisibility)
      {
        brushVisibilityTimer -= dt;
        if (brushVisibilityTimer <= 0)
          brushVisibility = false;
      }

      // if (IsKeyDown(KEY_DELETE))
      // {
      //   for (int y = 0; y < GRID_H; y++)
      //   {
      //     for (int x = 0; x < GRID_W; x++)
      //     {
      //       grid[x][y] = createEmpty();
      //     }
      //   }
      // }

      if (IsKeyPressed(KEY_DELETE))
      {
        clearing = 1;
        clearRow = 0;
      }

      if (clearing)
      {
        for (int x = 0; x < GRID_W; x++)
        {
          for (int i = 0; i < 3; i++)
          {
            if (clearRow < GRID_H - i)
            {
              grid[x][clearRow + i] = createAcid();
            }
          }

          grid[x][clearRow] = createEmpty();
        }

        clearRow++;

        if (clearRow >= GRID_H)
        {
          clearing = 0;
        }
      }

      for (int y = GRID_H - 1; y >= 0; y--)
      {
        int xScanDir = GetRandomValue(0, 1);
        for (int x = xScanDir ? (0) : (GRID_W - 1); xScanDir ? (x < GRID_W) : (x >= 0); x += xScanDir ? 1 : -1)
        {
          int steps = grid[x][y].steps;
          for (int s = 0; s < steps; s++)
          {
            switch (grid[x][y].type)
            {
            case CELL_SAND:
              updateSand(grid, x, y);
              break;
            case CELL_WATER:
              updateWater(grid, x, y);
              break;
            case CELL_ACID:
              updateAcid(grid, x, y);
              break;
            case CELL_SOIL:
              updateSoil(grid, x, y);
              break;
            case CELL_BLACK_HOLE:
              updateBlackHole(grid, x, y);
              break;
            default:
              break;
            }
            switch (floorMode)
            {
            case FLOOR_STOP:
              break;
            case FLOOR_DELETE:
              if (y == GRID_H - 1)
                grid[x][y] = createEmpty();
              break;
            case FLOOR_WRAP:
              if (y == GRID_H - 1 && grid[x][0].type == CELL_EMPTY)
              {
                grid[x][0] = grid[x][y];
                grid[x][y] = createEmpty();
              }
              break;
            }
          }
        }
      }

      switch (GetKeyPressed())
      {
      case KEY_ONE:
        currentCellType = CELL_EMPTY;
        break;
      case KEY_TWO:
        currentCellType = CELL_SAND;
        break;
      case KEY_THREE:
        currentCellType = CELL_WATER;
        break;
      case KEY_FOUR:
        currentCellType = CELL_ACID;
        break;
      case KEY_FIVE:
        currentCellType = CELL_SOIL;
        break;
      case KEY_SIX:
        currentCellType = CELL_ROCK;
        break;
      case KEY_SEVEN:
        currentCellType = CELL_BLACK_HOLE;
        break;

      case KEY_S:
        floorMode = FLOOR_STOP;
        break;
      case KEY_D:
        floorMode = FLOOR_DELETE;
        break;
      case KEY_W:
        floorMode = FLOOR_WRAP;
        break;

      default:
        break;
      }

      for (int y = 0; y < GRID_H; y++)
      {
        for (int x = 0; x < GRID_W; x++)
        {
          if (grid[x][y].type != CELL_EMPTY)
          {
            DrawRectangle(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                grid[x][y].color);
          }
        }
      }

      DrawText(TextFormat("(%d, %d)", gx, gy), mousePos.x + 40, mousePos.y + 0, 16, GRAY);

      DrawTextureEx(sandLogoTexture, (Vector2){10, 5}, 0.0f, 0.5f, WHITE);
      DrawText("Mege Sandbox", 80, 25, 30, BROWN);

      DrawTextureEx(brushTexture, (Vector2){10, 70}, 0.0f, 0.5f, WHITE);
      DrawText(TextFormat("%d%%", brushSize * 100 / maxBrushSize), 80, 90, 30, BROWN);

      DrawTextureEx(brushTexture, (Vector2){mousePos.x + (mouseButtonLeftDown ? -20 : (-10 + cos(t * 3) * -10)), mousePos.y + (mouseButtonLeftDown ? -45 : (-55 + sin(t * 3) * 10))}, 0.0f, 0.5f, WHITE);
      // DrawTextureEx(brushTexture, (Vector2){mousePos.x + (mouseButtonLeftDown ? -20 : (-20 + cos(t*3)*-10)), mousePos.y + (mouseButtonLeftDown ? -45 : (-45 + sin(t*3)*10))}, 0.0f, 0.5f, WHITE);

      if (showHints)
      {
        DrawText(hints[currentHint], windowConfig.size.x / 2 - MeasureText(hints[currentHint], 20) / 2, 30, 20, GRAY);
        hintTimer += dt;
      }
      if (hintTimer > hintDisplayTime)
      {
        hintTimer = 0.0f;
        currentHint = (currentHint + 1) % hintsCount;
      }
      if (IsKeyPressed(KEY_H))
      {
        showHints = !showHints;
      }
    }
    EndDrawing();
  }
  UnloadTexture(brushTexture);
  UnloadTexture(sandLogoTexture);
  CloseWindow();
}
