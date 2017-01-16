#ifndef KEYBOARD
#define KEYBOARD
typedef enum {
  ESC=1,
  Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key0, KeyApostrophe, Plus, BackSpace,
  Tab=15, Q, W, E, R, T, Z, U, I, O, P, MS, MD, Return,
  LControl, A=30, S, D, F, G, H, J, K, L, MC, SC, Unknown2, LShift=42, SZ,
  Y=44, X, C, V, B, N, M, Colon, Dot, Dash, RShift=54, Alt=56, Space=57,
  F1=59, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  UpArrow = 72, DownArrow = 80, LeftArrow = 75, RightArrow = 77
} Key;

extern const char KeyMAPL[];
extern const char KeyMAPU[];
extern int ALT_DOWN;
extern int SHIFT_DOWN;

#define PRINTABLE(key) ((key > 1 && key < 14)||(key>15 && key<29)||(key>29 && key<41)||(key>43 && key<54)||(Key)key == Space)
#define RELEASED(key) (key&0x80)

char Keyboard_Scan();
#endif
