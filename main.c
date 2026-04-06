#include "raylib.h"
#include <math.h>
#include <stdio.h>

// Ukuran Layar
#define L_LAYAR 1280
#define T_LAYAR 800
#define HIST_SIZE 250

// Buffer Riwayat (Kalkulus)
float riwayatX[HIST_SIZE] = { 0 };
float riwayatV[HIST_SIZE] = { 0 };
float riwayatA[HIST_SIZE] = { 0 };
int idxRiwayat = 0;

// Variabel Kontrol
float R = 100.0f;
float L_ROD = 320.0f;
float kecepatanDasar = 2.0f;
bool isOutline = false;

// Fungsi menggambar panah vektor
void GambarVektor(Vector2 pos, float nilai, float skala, Color warna) {
    if (fabsf(nilai) < 0.05f) return;
    float panjang = nilai * skala;
    Vector2 ujung = { pos.x + panjang, pos.y };
    DrawLineEx(pos, ujung, 3.0f, warna);
    float arah = (nilai > 0) ? 1.0f : -1.0f;
    DrawTriangle((Vector2){ujung.x, ujung.y - 6}, (Vector2){ujung.x, ujung.y + 6}, (Vector2){ujung.x + 8 * arah, ujung.y}, warna);
}

// Fungsi grafik kalkulus
void GambarDashboardKalkulus(int x, int y, int w, int h, float* data, Color warna, char* label) {
    DrawRectangle(x - 5, y - 5, w + 10, h + 10, Fade(BLACK, 0.7f));
    DrawRectangleLines(x - 5, y - 5, w + 10, h + 10, DARKGRAY);
    DrawText(label, x, y - 20, 15, warna);
    DrawLine(x, y + h/2, x + w, y + h/2, DARKGRAY);

    for (int i = 0; i < HIST_SIZE - 1; i++) {
        float val1 = data[i] * (h / 2.2f);
        float val2 = data[i + 1] * (h / 2.2f);
        
        // Clamping: Batasi agar tidak keluar garis kotak (h/2)
        if (val1 > h/2) val1 = h/2; if (val1 < -h/2) val1 = -h/2;
        if (val2 > h/2) val2 = h/2; if (val2 < -h/2) val2 = -h/2;

        float y1 = (y + h/2) - val1;
        float y2 = (y + h/2) - val2;
        DrawLine(x + i * (w / (float)HIST_SIZE), y1, x + (i + 1) * (w / (float)HIST_SIZE), y2, warna);
    }
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(L_LAYAR, T_LAYAR, "DASHBOARD INTERAKTIF: KALKULUS & FISIKA [FIXED]");
    SetTargetFPS(60);

    float sudut = 0.0f;
    float x_sebelum = 0.0f;
    float v_sebelum = 0.0f;
    Vector2 pusatA = { L_LAYAR / 4.0f, T_LAYAR / 2.0f };

    while (!WindowShouldClose()) {
        bool variabelBerubah = false;
        Vector2 mPos = GetMousePosition();
        
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mPos, (Rectangle){30, 110, 200, 20})) {
                kecepatanDasar = (mPos.x - 30) / 20.0f; 
                if (kecepatanDasar < 0) kecepatanDasar = 0;
            }
            if (CheckCollisionPointRec(mPos, (Rectangle){30, 180, 200, 20})) {
                R = (mPos.x - 30) * 1.5f; variabelBerubah = true;
                if (R < 20.0f) R = 20.0f; if (R > 180.0f) R = 180.0f;
            }
            if (CheckCollisionPointRec(mPos, (Rectangle){30, 250, 200, 20})) {
                L_ROD = (mPos.x - 30) * 4.0f; variabelBerubah = true;
                if (L_ROD < R + 20.0f) L_ROD = R + 20.0f;
            }
        }
        if (IsKeyPressed(KEY_O)) isOutline = !isOutline;

        // Update Mekanik
        sudut += kecepatanDasar * GetFrameTime();
        if (sudut > 2*PI) sudut -= 2*PI;

        Vector2 titikB = { pusatA.x + R * cosf(sudut), pusatA.y + R * sinf(sudut) };
        float dy = titikB.y - pusatA.y;
        float x_pos = titikB.x + sqrtf(L_ROD * L_ROD - (dy * dy));
        Vector2 titikC = { x_pos, pusatA.y };

        // Kalkulus Numerik dengan Proteksi Lonjakan (Spike Protection)
        float dt = GetFrameTime();
        if (dt > 0) {
            float v_now = (x_pos - x_sebelum) / dt;
            float a_now = (v_now - v_sebelum) / dt;

            // Jika slider digeser, kita reset data agar tidak ada grafik "meledak"
            if (variabelBerubah) {
                v_now = 0; a_now = 0;
            }

            for(int i=0; i<HIST_SIZE-1; i++) {
                riwayatX[i] = riwayatX[i+1];
                riwayatV[i] = riwayatV[i+1];
                riwayatA[i] = riwayatA[i+1];
            }
            riwayatX[HIST_SIZE-1] = (x_pos - (pusatA.x + L_ROD)) / (R + 1.0f);
            riwayatV[HIST_SIZE-1] = v_now / 400.0f; 
            riwayatA[HIST_SIZE-1] = a_now / 5000.0f;

            x_sebelum = x_pos;
            v_sebelum = v_now;
        }

        // --- RENDERING ---
        BeginDrawing();
        ClearBackground((Color){ 10, 10, 12, 255 });

        // Background Grid
        for(int x=0; x<L_LAYAR; x+=50) DrawLine(x, 0, x, T_LAYAR, (Color){ 20, 20, 25, 255 });
        for(int y=0; y<T_LAYAR; y+=50) DrawLine(0, y, L_LAYAR, y, (Color){ 20, 20, 25, 255 });

        // 1. SILINDER & ENGINE BLOCK
        float sX = pusatA.x + R + 20;
        DrawRectangle(sX, pusatA.y - 70, 450, 140, (Color){18, 18, 22, 255});
        if (!isOutline) {
            for(int f=0; f<13; f++) DrawRectangle(sX + 10 + f*34, pusatA.y - 75, 4, 150, (Color){40, 40, 45, 255});
        }

        // 2. MEKANIK UTAMA
        if (isOutline) {
            DrawCircleLines(pusatA.x, pusatA.y, R, SKYBLUE);
            DrawLineEx(pusatA, titikB, 2.0f, SKYBLUE);
            DrawLineEx(titikB, titikC, 2.0f, GRAY);
            DrawRectangleLines(titikC.x - 60, titikC.y - 45, 120, 90, WHITE);
        } else {
            // Crank & Counterweight
            Vector2 bw = { pusatA.x - (R*0.7f) * cosf(sudut), pusatA.y - (R*0.7f) * sinf(sudut) };
            DrawLineEx(pusatA, bw, 20, (Color){60, 60, 70, 255});
            DrawCircleV(bw, 25, (Color){50, 50, 60, 255});
            DrawLineEx(pusatA, titikB, 12, SKYBLUE);
            DrawCircleV(pusatA, 32, (Color){40, 40, 45, 255});
            // Connecting Rod
            DrawLineEx(titikB, titikC, 8, DARKGRAY);
            DrawCircleV(titikB, 10, BLUE);
            // Piston
            DrawRectangleGradientH(titikC.x - 60, titikC.y - 45, 120, 90, GRAY, DARKGRAY);
            for(int r=0; r<3; r++) DrawRectangle(titikC.x - 50 + r*12, titikC.y - 40, 4, 80, BLACK);
            // Ignition Flash
            if (fabsf(sudut) < 0.2f || sudut > 2*PI - 0.2f) 
                DrawCircleGradient(sX + 440, pusatA.y, 70, YELLOW, Fade(GOLD, 0.0f));
        }

        // 3. VEKTOR GAYA (FISIKA)
        GambarVektor(titikC, riwayatA[HIST_SIZE-1], 15.0f, RED);
        if (!isOutline) DrawText("Vektor Inersia (F=ma)", titikC.x - 60, titikC.y - 65, 12, RED);

        // 4. GRAFIK DASHBOARD (KALKULUS)
        GambarDashboardKalkulus(960, 80,  280, 120, riwayatX, SKYBLUE, "1. Posisi [x]");
        GambarDashboardKalkulus(960, 260, 280, 120, riwayatV, LIME,    "2. Kecepatan [v = dx/dt]");
        GambarDashboardKalkulus(960, 440, 280, 120, riwayatA, RED,     "3. Percepatan [a = dv/dt]");

        // 5. PANEL PANEL KONTROL
        DrawRectangle(20, 20, 260, 360, Fade(BLACK, 0.7f));
        DrawRectangleLines(20, 20, 260, 360, DARKGRAY);
        DrawText("DASHBOARD KONTROL", 40, 40, 18, GOLD);
        
        char buf[128];
        sprintf(buf, "Kecepatan RPM: %.0f", kecepatanDasar * 9.55f); 
        DrawText(buf, 40, 85, 16, RAYWHITE);
        DrawRectangle(30, 110, 200, 8, DARKGRAY); DrawCircle(30 + kecepatanDasar*20, 114, 8, GOLD);

        sprintf(buf, "Radius Crank (r): %.1f", R); 
        DrawText(buf, 40, 155, 16, SKYBLUE);
        DrawRectangle(30, 180, 200, 8, DARKGRAY); DrawCircle(30 + (R/1.5f), 184, 8, SKYBLUE);

        sprintf(buf, "Panjang Rod (L): %.1f", L_ROD); 
        DrawText(buf, 40, 225, 16, GRAY);
        DrawRectangle(30, 250, 200, 8, DARKGRAY); DrawCircle(30 + (L_ROD/4.0f), 254, 8, RAYWHITE);

        DrawText("[O] Toggle Outline Mode", 40, 310, 15, isOutline ? GREEN : LIGHTGRAY);
        DrawText("Geser slider untuk kontrol", 40, 335, 14, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
