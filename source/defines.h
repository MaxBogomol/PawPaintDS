#include <nds.h>

inline u16 alphaColor = ARGB16(0, 0, 0, 0);
inline u16 whiteColor = ARGB16(1, 31, 31, 31);
inline u16 grayColor = ARGB16(1, 15, 15, 15);
inline u16 blackColor = ARGB16(1, 0, 0, 0);
inline u16 redColor = ARGB16(1, 31, 0, 0);
inline u16 greenColor = ARGB16(1, 0, 31, 0);
inline u16 blueColor = ARGB16(1, 0, 0, 31);

inline u16 pinkFoxThemeColor = ARGB16(1, 31, 24, 25); //#fec8cf
inline u16 maidThemeColor = ARGB16(1, 6, 5, 7); //#2e2939
inline u16 aceThemeColor = ARGB16(1, 20, 6, 20); //#a231a2

inline u16 pixelBufferMain[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferSub[SCREEN_WIDTH * SCREEN_HEIGHT];

inline u16 pixelBufferLayer0[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferLayer1[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferLayer2[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferLayer3[SCREEN_WIDTH * SCREEN_HEIGHT];

inline bool activeLayer0 = true;
inline bool activeLayer1 = true;
inline bool activeLayer2 = true;
inline bool activeLayer3 = true;

inline u16* bgMainDest;
inline u16* bgSubDest;

inline touchPosition touch;
inline int keysD;
inline int keysH;
inline int keysR;
inline int keysU;

inline int touchX = 0;
inline int touchY = 0;
inline int touchXOld = 0;
inline int touchYOld = 0;
inline int touchCount = 0;

inline const char* fatPath = "fat:";
inline const char* sdPath = "sd:";
inline const char* pawPaintPath = "PawPaintDS";
inline const char* paintsPath = "Paints";
inline const char* pawsPath = "Paws";
inline const char* languagesPath = "nitro:/languages";
inline const char* settingsFile = "settings.ini";

inline const char* paintVerstion = "v0.1";

inline int maxLanguages = 6;
inline int maxPaintThemes = 4;
inline int maxPaintIcons = 2;

inline const char* languageCodes[6] = {
    "en_us",
    "ru_ru",
    "be_by",
    "be_tar",
    "be_by_latn",
    "be_tar_latn"
};

struct HSV {
    int h;
    int s;
    int v;
};