#include "raylib.h"
#include <math.h>
#include <stdio.h>

// Ukuran layar & parameter mekanisme
#define L_LAYAR 1200
#define T_LAYAR 800
#define JARI_JARI 100.0f
#define PANJANG_ROD 320.0f
#define HIST_SIZE 240

// Fungsi garis putus-putus untuk lintasan
void GambarGarisPutus(Vector2 awal, Vector2 akhir, float panjang, Color warna) {
    float dx = akhir.x - awal.x, dy = akhir.y - awal.y;
    float jarak = sqrtf(dx*dx + dy*dy);
    int jumlah = (int)(jarak / (panjang * 2));
    for (int i = 0; i < jumlah; i++) {
        float f1 = (i * 2) * panjang / jarak;
        float f2 = (i * 2 + 1) * panjang / jarak;
        DrawLineV((Vector2){awal.x + dx*f1, awal.y + dy*f1}, (Vector2){awal.x + dx*f2, awal.y + dy*f2}, warna);
    }
}

int main(void) {
    // Setup window awal
    InitWindow(L_LAYAR, T_LAYAR, "Simulasi Slider Crank - Mekanika Mesin");
    SetTargetFPS(60);

    float sudut = 0.0f;
    float kec_dasar = 2.5f;
    float riwayatX[HIST_SIZE] = { 0 };
    int idxRiwayat = 0;

    // Titik pusat kruk as
    Vector2 pusatA = { L_LAYAR / 4.0f + 60, T_LAYAR / 2.0f };

    while (!WindowShouldClose()) {
        // Kontrol kecepatan pakai panah keyboard
        if (IsKeyDown(KEY_UP)) kec_dasar += 0.05f;
        if (IsKeyDown(KEY_DOWN)) kec_dasar -= 0.05f;
        if (kec_dasar < 0) kec_dasar = 0;

        // Simulasi beban mesin (kecepatan berubah sedikit saat kompresi)
        float w = kec_dasar * (1.0f + 0.15f * sinf(sudut - PI/4.0f));
        sudut += w * GetFrameTime();
        if (sudut > 2*PI) sudut -= 2*PI;

        // Hitung posisi kruk pin (Titik B)
        Vector2 titikB = { pusatA.x + JARI_JARI * cosf(sudut), pusatA.y + JARI_JARI * sinf(sudut) };

        // Hitung posisi piston (Titik C) pakai Pythagoras
        float dy = titikB.y - pusatA.y;
        float L_datar = sqrtf(PANJANG_ROD * PANJANG_ROD - (dy * dy));
        Vector2 titikC = { titikB.x + L_datar, pusatA.y };

        // Simpan data untuk grafik x(t)
        riwayatX[idxRiwayat] = (titikC.x - (pusatA.x + PANJANG_ROD)) / JARI_JARI;
        idxRiwayat = (idxRiwayat + 1) % HIST_SIZE;

        BeginDrawing();
        ClearBackground((Color){ 10, 10, 12, 255 }); 

        // Gambar grid tipis
        for(int x=0; x<L_LAYAR; x+=50) DrawLine(x, 0, x, T_LAYAR, (Color){ 20, 20, 25, 255 });
        for(int y=0; y<T_LAYAR; y+=50) DrawLine(0, y, L_LAYAR, y, (Color){ 20, 20, 25, 255 });

        // Komponen Luar (Silinder & Sirip)
        float sX = pusatA.x + JARI_JARI + 40;
        for(int f=0; f<10; f++) DrawRectangle(sX + f*32, pusatA.y - 65, 4, 130, (Color){40, 40, 45, 255});
        DrawRectangle(sX, pusatA.y - 60, 340, 120, (Color){18, 18, 22, 255});
        DrawLineEx((Vector2){sX, pusatA.y - 60}, (Vector2){sX + 340, pusatA.y - 60}, 2, DARKGRAY);
        DrawLineEx((Vector2){sX, pusatA.y + 60}, (Vector2){sX + 340, pusatA.y + 60}, 2, DARKGRAY);

        // Bagian Kruk As (Crankshaft)
        Vector2 bobotPos = { pusatA.x - 60 * cosf(sudut), pusatA.y - 60 * sinf(sudut) };
        DrawLineEx(pusatA, bobotPos, 20, (Color){60, 60, 70, 255});
        DrawCircleV(bobotPos, 30, (Color){50, 50, 60, 255});
        DrawLineEx(pusatA, titikB, 12, SKYBLUE);
        DrawCircleV(pusatA, 30, (Color){40, 40, 45, 255});
        DrawCircleV(titikB, 8, BLUE);

        // Batang Piston (Connecting Rod)
        DrawLineEx(titikB, titikC, 8, DARKGRAY);

        // Piston & Efek Pembakaran
        Rectangle pistonBox = { titikC.x - 60, titikC.y - 45, 120, 90 };
        DrawRectangleRec(pistonBox, (Color){110, 110, 120, 255});
        for(int r=0; r<3; r++) DrawRectangle(pistonBox.x + 10 + r*12, pistonBox.y + 5, 4, 80, BLACK);
        
        // Efek Flash Busi (Ignition)
        if (fabsf(sudut) < 0.25f || fabsf(sudut) > PI*1.95f) {
            DrawCircleGradient(sX + 320, pusatA.y, 50, YELLOW, Fade(ORANGE, 0.0f));
            DrawText("IGNITION!", sX + 240, pusatA.y - 80, 20, GOLD);
        }
        DrawCircle(sX + 335, pusatA.y, 8, RED); // Lokasi busi

        // Katup (Valves) sederhana
        float vY = sinf(sudut) * 15.0f;
        DrawRectangle(sX + 300, pusatA.y - 65 - (vY > 0 ? vY : 0), 15, 10, LIME); 
        DrawRectangle(sX + 300, pusatA.y + 55 + (vY < 0 ? -vY : 0), 15, 10, MAROON);

        // Grafik x(t) di pojok
        DrawRectangle(840, 610, 320, 160, Fade(BLACK, 0.8f));
        DrawRectangleLines(840, 610, 320, 160, DARKGRAY);
        DrawText("Grafik Posisi Piston x(t)", 850, 585, 16, SKYBLUE);
        for (int i = 0; i < HIST_SIZE - 1; i++) {
            int a = (idxRiwayat + i) % HIST_SIZE, b = (idxRiwayat + i + 1) % HIST_SIZE;
            DrawLine(850 + i * (300.0f/HIST_SIZE), 690 - riwayatX[a]*60, 
                     850 + (i+1) * (300.0f/HIST_SIZE), 690 - riwayatX[b]*60, SKYBLUE);
        }

        // Panel Info (Tugas Kuliah Style)
        DrawRectangle(30, 30, 350, 100, Fade(BLACK, 0.5f));
        DrawText(TextFormat("Sudut Theta: %.1f deg", sudut * RAD2DEG), 45, 45, 18, RAYWHITE);
        DrawText(TextFormat("Kec. Sudut: %.2f rad/s", w), 45, 70, 18, SKYBLUE);
        DrawText("Gunakan Panah Atas/Bawah", 45, 135, 14, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
