#pragma once

#include "fruitbox.hpp"

#define BUTTON_QUIT              0
#define BUTTON_INSERT_COIN1      1
#define BUTTON_INSERT_COIN2      2
#define BUTTON_INSERT_COIN3      3
#define BUTTON_INSERT_COIN4      4
#define BUTTON_VOL_UP            5
#define BUTTON_VOL_DOWN          6
#define BUTTON_ADD_RANDOM        7
#define BUTTON_SELECT            8
#define BUTTON_SKIP              9
#define BUTTON_PAUSE             10
#define BUTTON_UP                11
#define BUTTON_DOWN              12
#define BUTTON_LEFT              13
#define BUTTON_RIGHT             14
#define BUTTON_LEFT_JUMP         15
#define BUTTON_RIGHT_JUMP        16
#define BUTTON_LEFT_ALPHA        17
#define BUTTON_RIGHT_ALPHA       18
#define BUTTON_AUTO_PLAY_TOG     19    


#define BUTTON_MASK              0x0000ffff
#define BUTTON_RELEASED          0x80000000
#define TOUCH_SONG               0x40000000
#define SONG_PAGE_POS            8
#define SONG_ENTRY_POS           16
#define SONG_POS_MASK            0xff

#define EVENT_PARAM_ID           0
#define EVENT_PARAM_VAL1         1
#define EVENT_PARAM_VAL2         2
#define EVENT_PARAM_PRESSED      3

enum class input_device_state_e
{
  idle,
  waiting_for_touch_abs_x,
  waiting_for_touch_abs_y,
  waiting_for_touch_release
};

struct input_device_t
{
  input_type_e type { input_type_e::None };
  string name {};
  string event_path {};
  input_device_state_e state {input_device_state_e::idle};
  bool joystick_moved { false };
  uint32_t joystick_moved_code { 0 };
  int fd;
  uint32_t js_num {0};
  // uint32_t mouse_num {0};
};

struct raw_t
{
  mutex mtx;
  bool enable { false };
  condition_variable valid;
  input_event_t event {};
  string str;
  volatile bool kbd_esc_pressed { false };
};

class InputClass
{
public:
  InputClass(bool i_config_buttons, bool i_test_buttons, bool i_calibrate_touch);
  void Start(void);
  ~InputClass();
  deque<uint32_t> queue { };
  std::mutex queue_mtx {};
  bool touch_installed {false};
  double touch_scale_x { 1.0 };
  double touch_scale_y { 1.0 };
  int32_t touch_offset_x { 0 };
  int32_t touch_offset_y { 0 };
  bool reversed_touch {false};
private:
  void openDevices(void);
  void scanGPIO(input_event_t &event);
  void scanDevice(input_device_t &device, input_event_t &event);
  vector <input_device_t> input_device {};
  // deque<config_buttons_queue_t> config_buttons_queue { };
  raw_t raw {};
  bool config_buttons {false};
  bool test_buttons {false};
  bool calibrate_touch {false};
  void ConfigButtons(void);
  void TestButtons(void);
  bool FindButton(const input_event_t &event);
  void CalibrateTouch(void);
  void InputThread(void);
};
