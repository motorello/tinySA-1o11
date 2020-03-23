
void markmap_all_markers(void);
static void menu_marker_type_cb(int item, uint8_t data);

void set_sweep_frequency(int type, uint32_t frequency);
uint32_t get_sweep_frequency(int type);
void clearDisplay(void);
//void ui_process_touch(void);
void SetPowerGrid(int);
void SetRefLevel(int);
void set_refer_output(int);
int get_refer_output(void);
void SetAttenuation(int);
void SetPowerLevel(int);
void SetGenerate(int);
void SetRBW(int);
extern int settingBandwidth;
void SetSpur(int);
int GetSpur(void);
void SetAverage(int);
int GetAverage(void);
extern int settingAverage;
void  SetStorage(void);
void  SetClearStorage(void);
void  SetSubtractStorage(void);
void toggle_waterfall(void);
void SetMode(int);
int GetMode(void);
void AllDirty(void);
void MenuDirty(void);
void redrawHisto(void);
void self_test(void);
extern int32_t frequencyExtra;
extern int extraVFO;
extern int settingDrive;
extern int settingLNA;
extern int settingAGC;
extern int settingSpeed;
extern int stepDelay;

enum {
  KM_START, KM_STOP, KM_CENTER, KM_SPAN, KM_CW, KM_REFPOS, KM_SCALE, KM_ATTENUATION, KM_ACTUALPOWER, KM_IF, KM_SAMPLETIME, KM_DRIVE
};


#define KP_X(x) (48*(x) + 2 + (320-BUTTON_WIDTH-192))
#define KP_Y(y) (48*(y) + 2)


#define KP_PERIOD 10
#define KP_MINUS 11
#define KP_X1 12
#define KP_K 13
#define KP_M 14
#define KP_G 15
#define KP_BS 16
#define KP_INF 17
#define KP_DB 18
#define KP_PLUSMINUS 19
#define KP_KEYPAD 20
#define KP_N 21
#define KP_P 22


typedef struct {
  uint8_t x:4;
  uint8_t y:4;
  int8_t  c;
} keypads_t;

static const keypads_t *keypads;

static uint8_t keypads_last_index;


static const keypads_t keypads_freq[] = {
  { 1, 3, KP_PERIOD },
  { 0, 3, 0 },
  { 0, 2, 1 },
  { 1, 2, 2 },
  { 2, 2, 3 },
  { 0, 1, 4 },
  { 1, 1, 5 },
  { 2, 1, 6 },
  { 0, 0, 7 },
  { 1, 0, 8 },
  { 2, 0, 9 },
  { 3, 0, KP_G },
  { 3, 1, KP_M },
  { 3, 2, KP_K },
  { 3, 3, KP_X1 },
  { 2, 3, KP_BS },
  { 0, 0, -1 }
};

static const keypads_t keypads_scale[] = {
  { 1, 3, KP_PERIOD },
  { 0, 3, 0 },
  { 0, 2, 1 },
  { 1, 2, 2 },
  { 2, 2, 3 },
  { 0, 1, 4 },
  { 1, 1, 5 },
  { 2, 1, 6 },
  { 0, 0, 7 },
  { 1, 0, 8 },
  { 2, 0, 9 },
  { 3, 3, KP_X1 },
  { 2, 3, KP_BS },
  { 0, 0, -1 }
};

static const keypads_t keypads_level[] = {
  { 1, 3, KP_PERIOD },
  { 0, 3, 0 },
  { 0, 2, 1 },
  { 1, 2, 2 },
  { 2, 2, 3 },
  { 0, 1, 4 },
  { 1, 1, 5 },
  { 2, 1, 6 },
  { 0, 0, 7 },
  { 1, 0, 8 },
  { 2, 0, 9 },
  { 3, 2, KP_MINUS },
  { 3, 3, KP_X1 },
  { 2, 3, KP_BS },
  { 0, 0, -1 }
};


static const keypads_t * const keypads_mode_tbl[] = {
  keypads_freq, // start
  keypads_freq, // stop
  keypads_freq, // center
  keypads_freq, // span
  keypads_freq, // cw freq
  keypads_level, // refpos
  keypads_scale, // scale
  keypads_scale, // attenuation
  keypads_level, // actual power
  keypads_freq, // IF
  keypads_level, // sample time
  keypads_scale, // drive
};

#ifdef __VNA__
static const char * const keypad_mode_label[] = {
  "START", "STOP", "CENTER", "SPAN", "CW FREQ", "SCALE", "REFPOS", "EDELAY", "VELOCITY%", "DELAY"
};
#endif
#ifdef __SA__
static const char * const keypad_mode_label[] = {
  "START", "STOP", "CENTER", "SPAN", "CW FREQ", "REFPOS", "SCALE", "ATTENUATION", "ACTUALPOWER", "IF", "SAMPLE TIME", "DRIVE"
};
#endif


// ===[MENU CALLBACKS]=========================================================


int generator_enabled = false;

extern const menuitem_t  menu_lowoutputmode[];
extern const menuitem_t  menu_highoutputmode[];

static void menu_mode_cb(int item, uint8_t data)
{
  (void)data;
  switch (item) {
  case 4: // Change reference output, should not happen!!!
    break;
  default:
    SetMode(item);
    draw_cal_status();
    if (item == 2) { // Activate menu_lowoutputmode as input form
      set_sweep_frequency(ST_SPAN, 0);
      menu_push_submenu(menu_lowoutputmode);
    } else if (item == 3) { // Activate menu_highoutputmode as input form
      set_sweep_frequency(ST_SPAN, 0);
      menu_push_submenu(menu_highoutputmode);
    } else
      ui_mode_normal(); // Exit menu after setting the mode
    break;
  }

}

extern int dirty;
void menu_autosettings_cb(int item, uint8_t data)
{
  (void)item;
  (void)data;
  SetMode(M_LOW);
//  set_sweep_frequency(ST_START, (int32_t) 0);
//  set_sweep_frequency(ST_STOP, (int32_t) 300000000);

  int value = 10; // 10dB/
  set_trace_scale(0, value);
  set_trace_scale(1, value);
  set_trace_scale(2, value);

  value = -10; // Top at -10dB
  set_trace_refpos(0, - value / get_trace_scale(0) + NGRIDY);
  set_trace_refpos(1, - value / get_trace_scale(0) + NGRIDY);
  set_trace_refpos(2, - value / get_trace_scale(0) + NGRIDY);

  active_marker = 0;
  menu_marker_type_cb(M_REFERENCE,M_REFERENCE);

  set_refer_output(1);
  SetAttenuation(0);
  SetPowerLevel(100); // Reset
  SetClearStorage();
  SetRBW(0);
  dirty = true;
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_config_cb(int item, uint8_t data)
{
  (void)data;
  switch (item) {
  case 0:
      touch_cal_exec();
      redraw_frame();
      request_to_redraw_grid();
      draw_menu();
      break;
  case 1:
      touch_draw_test();
      redraw_frame();
      request_to_redraw_grid();
      draw_menu();
      break;
  case 2:
      menu_move_back();
      ui_mode_normal();
      self_test();
      break;
  case 3:
      show_version();
      redraw_frame();
      request_to_redraw_grid();
      draw_menu();
  }
}

static void menu_dfu_cb(int item, uint8_t data)
{
  (void)data;
  switch (item) {
  case 0:
      enter_dfu();
  }
}

int menu_refer_value[]={-1,0,1,2,3,4,5,6};
static void menu_refer_cb(int item, uint8_t data)
{
  (void)data;
//Serial.println(item);
  set_refer_output(menu_refer_value[item]); 
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_refer_cb2(int item, uint8_t data)
{
  (void)data;
//Serial.println(item);
  set_refer_output(menu_refer_value[item+5]); 
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_spur_cb(int item, uint8_t data)
{
  (void)data;
  (void)item;
  if (GetSpur())
    SetSpur(0);
  else
    SetSpur(1); // must be 0 or 1 !!!!
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_storage_cb(int item, uint8_t data)
{
  (void)data;
  switch(item) {
    case 0:
      SetStorage();
      break;
    case 1:
      SetClearStorage();
      break;
    case 2:
      SetSubtractStorage();
      break;
    case 3:
      toggle_waterfall();
      break;
  }
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_average_cb(int item, uint8_t data)
{
  (void)data;
  SetAverage(item);
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

static void menu_marker_type_cb(int item, uint8_t data)
{
  (void)data;
  if (markers[active_marker].enabled)
  {
    if (item == M_REFERENCE) {
      for (int i = 0; i<MARKER_COUNT; i++ ) {
        if (markers[i].mtype ==M_REFERENCE)
          markers[i].mtype = M_NORMAL;
      }
    }
    markers[active_marker].mtype = item;
  }
  markmap_all_markers();
//  redraw_marker(active_marker, TRUE);
  menu_move_back();
  draw_menu();
}


int rbwsel[]={0,3,10,30,100,300};

static void menu_rbw_cb(int item, uint8_t data)
{
  (void)data;
  SetRBW(rbwsel[item]); 
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

int menu_dBper_value[]={1,2,5,10,20};

static void menu_dBper_cb(int item, uint8_t data)
{
  (void)data;
  set_trace_scale(0, menu_dBper_value[item]);
  set_trace_scale(1, menu_dBper_value[item]);
  set_trace_scale(2, menu_dBper_value[item]);
  menu_move_back();
  ui_mode_normal();
  draw_cal_status();
}

#if 0
static void choose_active_trace(void)
{
  int i;
  if (trace[uistat.current_trace].enabled)
    // do nothing
    return;
  for (i = 0; i < TRACE_COUNT ; i++)
    if (trace[i].enabled) {
      uistat.current_trace = i;
      return;
    }
}
#endif
static void choose_active_marker(void)
{
  int i;
  for (i = 0; i < MARKER_COUNT; i++)
    if (markers[i].enabled) {
      active_marker = i;
      return;
    }
  active_marker = -1;
}

static void menu_scale_cb(int item, uint8_t data)
{
  (void)data;
  int status;
  int km = KM_REFPOS + item;
//  if (km == KM_SCALE && trace[uistat.current_trace].type == TRC_DELAY) {
//    km = KM_SCALEDELAY;
//  }
  status = btn_wait_release();
  if (status & EVT_BUTTON_DOWN_LONG) {
    ui_mode_numeric(km);
//    ui_process_numeric();
  } else {
    ui_mode_keypad(km);
    ui_process_keypad();
  }
  draw_cal_status();
}

static void menu_lowoutputmode_cb(int item, uint8_t data)
{
  int status;
  int km = data;
//  if (km == KM_SCALE && trace[uistat.current_trace].type == TRC_DELAY) {
//    km = KM_SCALEDELAY;
//  }
  status = btn_wait_release();
  if (status & EVT_BUTTON_DOWN_LONG) {
    ui_mode_numeric(km);
//    ui_process_numeric();
  } else {
    area_width = AREA_WIDTH_NORMAL - MENU_BUTTON_WIDTH;
    redraw_frame();         // Remove form numbers
    ui_mode_keypad(km);
    ui_process_keypad();
  }
  draw_cal_status();
}

static void menu_highoutputmode_cb(int item, uint8_t data)
{
  int status;
  int km = data;
//  if (km == KM_SCALE && trace[uistat.current_trace].type == TRC_DELAY) {
//    km = KM_SCALEDELAY;
//  }
  status = btn_wait_release();
  if (status & EVT_BUTTON_DOWN_LONG) {
    ui_mode_numeric(km);
//    ui_process_numeric();
  } else {
    area_width = AREA_WIDTH_NORMAL - MENU_BUTTON_WIDTH;
    redraw_frame();         // Remove form numbers
    ui_mode_keypad(km);
    ui_process_keypad();
  }
  draw_cal_status();
}


static void menu_settings_cb(int item, uint8_t data)
{
  (void)data;
  int status;
  int km = KM_ACTUALPOWER+item;
  status = btn_wait_release();
  if (status & EVT_BUTTON_DOWN_LONG) {
    ui_mode_numeric(km);
    //    ui_process_numeric();
  } else {
    ui_mode_keypad(km);
    ui_process_keypad();
  }
  draw_cal_status();
}

static void menu_settings2_cb(int item, uint8_t data)
{
  (void)data;
  switch(item) {
  case 0:
    settingAGC = !settingAGC;
    break;
  case 1:
    settingLNA = !settingLNA;
    break;
  case 2:
    extraVFO = !extraVFO;
    break;
  }
  draw_cal_status();
  draw_menu();
}

static void menu_stimulus_cb(int item, uint8_t data)
{
  (void) data;
  int status;
  switch (item) {
  case 0: /* START */
  case 1: /* STOP */
  case 2: /* CENTER */
  case 3: /* SPAN */
  case 4: /* CW */
    status = btn_wait_release();
    if (status & EVT_BUTTON_DOWN_LONG) {
      ui_mode_numeric(item);
//      ui_process_numeric();
    } else {
      ui_mode_keypad(item);
      ui_process_keypad();
    }
    break;
  case 5: /* PAUSE */
    toggle_sweep();
    menu_move_back();
    ui_mode_normal();
    draw_menu();
    break;
  }
  draw_cal_status();
}

//static void menu_marker_sel_cb(int);
//static void menu_marker_op_cb(int);

#if 0

#pragma pack(push, 2)
typedef struct {
  uint8_t type;
  int  *data;
  char *format;
} menuvalue_t;
#pragma pack(pop)

enum {
  MVT_INT, MVT_FLOAT, MVT_STRINGARRAY
};
enum {
MV_AVERAGE, MV_RBW, MV_DBPER, MV_REFER, MV_POWER, MVSAMPLETIME, MV_IFFREQ
};

static const char *average_text[] =
{
 "OFF", "MIN HOLD", "MAX HOLD", "2", "4", "8"
};

static const menuvalue_t menu_value[] = {
  { MVT_STRINGARRAY,&settingAverage,  (char *)average_text },
  { MVT_INT,        &settingBandwidth, "%dkHz" },
  { MVT_INT,        &settingScale,     "%ddB/" },
  { MVT_INT,        &settingRefer,     "%dB"   },
  { MVT_INT,        &settingPower,      "%dB"   },
  { MVT_INT,        &settingSampleTime, "%dmS"  },
  { MVT_INT,        %setting_IF,        "%dHz"  },
  }
};
#endif

// ===[MENU DEFINITION]=========================================================

const menuitem_t  menu_lowoutputmode[] = {
  { MT_CALLBACK, KM_CENTER,     "FREQUENCY",    menu_lowoutputmode_cb},
  { MT_CALLBACK, KM_ATTENUATION,"LEVEL",        menu_lowoutputmode_cb},
  { MT_CANCEL,   0,             S_LARROW" BACK",NULL },
  { MT_NONE, 0, NULL, NULL } // sentinel
};

const menuitem_t  menu_highoutputmode[] = {
  { MT_CALLBACK, KM_CENTER,     "FREQUENCY",    menu_highoutputmode_cb},
  { MT_CALLBACK, KM_DRIVE,      "LEVEL",        menu_highoutputmode_cb},
  { MT_CANCEL,   0,             S_LARROW" BACK",NULL },
  { MT_NONE, 0, NULL, NULL } // sentinel
};

static const menuitem_t  menu_average[] = {
  { MT_CALLBACK, 0, "OFF",   menu_average_cb},
  { MT_CALLBACK, 0, "MIN",   menu_average_cb},
  { MT_CALLBACK, 0, "MAX",   menu_average_cb},
  { MT_CALLBACK, 0, " 2 ",   menu_average_cb},
  { MT_CALLBACK, 0, " 4 ",   menu_average_cb},
  { MT_CALLBACK, 0, " 8 ",   menu_average_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE, 0, NULL, NULL } // sentinel
};

static const menuitem_t menu_storage[] = {
  { MT_CALLBACK, 0, "STORE",    menu_storage_cb},
  { MT_CALLBACK, 0, "CLEAR",    menu_storage_cb},
  { MT_CALLBACK, 0, "SUBTRACT", menu_storage_cb},
  { MT_CALLBACK, 0, "WATERFALL",menu_storage_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,      0, NULL, NULL } // sentinel
};

static const menuitem_t menu_rbw[] = {
  { MT_CALLBACK, 0, "  AUTO",   menu_rbw_cb},
  { MT_CALLBACK, 0, "  3kHz",   menu_rbw_cb},
  { MT_CALLBACK, 0, " 10kHz",   menu_rbw_cb},
  { MT_CALLBACK, 0, " 30kHz",   menu_rbw_cb},
  { MT_CALLBACK, 0, "100kHz",   menu_rbw_cb},
  { MT_CALLBACK, 0, "300kHz",   menu_rbw_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,      0, NULL, NULL } // sentinel
};


static const menuitem_t menu_dBper[] = {
  { MT_CALLBACK, 0, "  1dB/",   menu_dBper_cb},
  { MT_CALLBACK, 0, "  2dB/",   menu_dBper_cb},
  { MT_CALLBACK, 0, "  5dB/",   menu_dBper_cb},
  { MT_CALLBACK, 0, " 10dB/",   menu_dBper_cb},
  { MT_CALLBACK, 0, " 20dB/",   menu_dBper_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_refer2[] = {
  { MT_CALLBACK, 0, "3MHz" ,   menu_refer_cb2},
  { MT_CALLBACK, 0, "2MHz" ,   menu_refer_cb2},
  { MT_CALLBACK, 0, "1MHz" ,   menu_refer_cb2},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_refer[] = {
  { MT_CALLBACK, 0, "OFF"  ,   menu_refer_cb},
  { MT_CALLBACK, 0, "30MHz",   menu_refer_cb},
  { MT_CALLBACK, 0, "15MHz",   menu_refer_cb},
  { MT_CALLBACK, 0, "10MHz",   menu_refer_cb},
  { MT_CALLBACK, 0, "4MHz" ,   menu_refer_cb},
  { MT_SUBMENU,  0, S_RARROW" MORE", menu_refer2},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_scale[] = {
  { MT_CALLBACK, 0, "\2REF\0LEVEL",  menu_scale_cb},
  { MT_SUBMENU,  0, "\2SCALE/\0DIV",     menu_dBper},
  { MT_CALLBACK, 0, "ATTEN",         menu_scale_cb},
  { MT_SUBMENU,  0, "AVERAGE",       menu_average},
  { MT_CALLBACK, 0, "\2SPUR\0REDUCT.",menu_spur_cb},
  { MT_SUBMENU,  0, "RBW",           menu_rbw},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_stimulus[] = {
  { MT_CALLBACK, 0, "START",            menu_stimulus_cb},
  { MT_CALLBACK, 0, "STOP",             menu_stimulus_cb},
  { MT_CALLBACK, 0, "CENTER",           menu_stimulus_cb},
  { MT_CALLBACK, 0, "SPAN",             menu_stimulus_cb},
  { MT_CALLBACK, 0, "CW FREQ",          menu_stimulus_cb},
//  { MT_SUBMENU,  0, "RBW",              menu_rbw},
  { MT_CALLBACK, 0, "\2PAUSE\0SWEEP",   menu_stimulus_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};


static const menuitem_t menu_marker_type[] = {
  { MT_CALLBACK, 0, "REFERENCE",    menu_marker_type_cb},
  { MT_CALLBACK, 0, "NORMAL",       menu_marker_type_cb},
  { MT_CALLBACK, 0, "DELTA",        menu_marker_type_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_marker_sel[] = {
  { MT_CALLBACK, 0, "MARKER 1",     menu_marker_sel_cb},
  { MT_CALLBACK, 0, "MARKER 2",     menu_marker_sel_cb},
  { MT_CALLBACK, 0, "MARKER 3",     menu_marker_sel_cb},
  { MT_CALLBACK, 0, "MARKER 4",     menu_marker_sel_cb},
  { MT_CALLBACK, 0, "ALL OFF",      menu_marker_sel_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_marker[] = {
  { MT_SUBMENU,  0, "\2SELECT\0MARKER",     menu_marker_sel},
  { MT_SUBMENU,  0, "\2MARKER\0TYPE",       menu_marker_type},
  { MT_CALLBACK, 0, S_RARROW"START",        menu_marker_op_cb},
  { MT_CALLBACK, 0, S_RARROW"STOP",         menu_marker_op_cb},
  { MT_CALLBACK, 0, S_RARROW"CENTER",       menu_marker_op_cb},
  { MT_CALLBACK, 0, S_RARROW"SPAN",         menu_marker_op_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_dfu[] = {
  { MT_CALLBACK, 0, "ENTER DFU", menu_dfu_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_mode[] = {
  { MT_CALLBACK, 0, "\2LOW\0INPUT", menu_mode_cb},
  { MT_CALLBACK, 0, "\2HIGH\0INPUT",menu_mode_cb},
  { MT_CALLBACK, 0, "\2LOW\0OUTPUT", menu_mode_cb},
  { MT_CALLBACK, 0, "\2HIGH\0OUTPUT",menu_mode_cb},
  { MT_SUBMENU,  0, "\2REFER\0OUTPUT",menu_refer},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_settings2[] =
{
//  { MT_CALLBACK, 0, "TRACK",            menu_settings2_cb},
  { MT_CALLBACK, 0, "AGC",              menu_settings2_cb},
  { MT_CALLBACK, 0, "LNA",              menu_settings2_cb},
  { MT_CALLBACK, 0, "BPF",              menu_settings2_cb},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_settings[] =
{
  { MT_CALLBACK, 0, "\2ACTUAL\0POWER",  menu_settings_cb},
  { MT_CALLBACK, 0, "\2IF\0FREQ",       menu_settings_cb},
  { MT_CALLBACK, 0, "\2SAMPLE\0TIME",   menu_settings_cb},
  { MT_CALLBACK, 0, "\2LO\0DRIVE",      menu_settings_cb},
  { MT_SUBMENU,  0, S_RARROW" MORE",    menu_settings2},

//  { MT_SUBMENU,  0, "RBW", menu_rbw},
  { MT_CANCEL,   0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_config[] = {
  { MT_CALLBACK, 0, "\2TOUCH\0CAL",     menu_config_cb},
  { MT_CALLBACK, 0, "\2TOUCH\0TEST",    menu_config_cb},
  { MT_CALLBACK, 0, "\2SELF\0TEST",     menu_config_cb},
  { MT_CALLBACK, 0, "VERSION",          menu_config_cb},
  { MT_SUBMENU,  0, "SETTINGS",         menu_settings},
//  { MT_SUBMENU,  0, "RBW", menu_rbw},
  { MT_SUBMENU,  0, S_RARROW"DFU",  menu_dfu},
  { MT_CANCEL, 0, S_LARROW" BACK", NULL },
  { MT_NONE,     0, NULL, NULL } // sentinel
};

static const menuitem_t menu_top[] = {
  { MT_CALLBACK, 0, "AUTO",       menu_autosettings_cb},
  { MT_SUBMENU,  0, "SCAN",       menu_stimulus},
  { MT_SUBMENU,  0, "MARKER",     menu_marker},
  { MT_SUBMENU,  0, "DISPLAY",    menu_scale},
  { MT_SUBMENU,  0, "STORAGE",    menu_storage},
  { MT_SUBMENU,  0, "MODE",       menu_mode},
  { MT_SUBMENU,  0, "CONFIG",     menu_config},
  { MT_NONE,     0, NULL, NULL } // sentinel,
 // MENUITEM_CLOSE,
};

// ===[MENU DEFINITION END]======================================================

#undef BOARD_NAME
#define BOARD_NAME  "tinySA"

#define ACTIVE_COLOR RGBHEX(0x007FFF)

void frequency_string(char *buf, size_t len, int32_t freq);

int menu_is_form(const menuitem_t *menu)
{
  return(menu == menu_lowoutputmode ||
         menu == menu_highoutputmode);
}

static void menu_item_modify_attribute(
    const menuitem_t *menu, int item, uint16_t *fg, uint16_t *bg)
{
  if (menu == menu_mode) {
    if (item == GetMode()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_refer) {
    if (item < 5 && item == get_refer_output() + 1){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_refer2) {
    if (item == get_refer_output() - 4){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_stimulus) {
    if (item == 5 /* PAUSE */ && !(sweep_mode&SWEEP_ENABLE)) {
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_scale) {
    if (item == 4 /* Spur reduction */ && GetSpur()) {
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_average) {
    if (item == GetAverage()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_dBper) {
    if (menu_dBper_value[item] == get_trace_scale(1)){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_rbw) {
    if (rbwsel[item] == GetRBW()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }

  } else if (menu == menu_storage) {
    if (item ==0 && GetStorage()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
    if (item == 2 && GetSubtractStorage()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
    if (item == 3 && get_waterfall()){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  } else if (menu == menu_settings2) {
    if (item ==0 && settingAGC){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
    if (item == 1 && settingLNA){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
    if (item == 2 && extraVFO){
      *bg = DEFAULT_MENU_TEXT_COLOR;
      *fg = config.menu_normal_color;
    }
  }
  if (ui_mode == UI_MENU && menu_is_form(menu)) {
//    if (item == 0)
//      redraw_frame();
    if (item <= 1) {

    area_width = 0;
//    area_height = HEIGHT - 32;
    int y = MENU_BUTTON_HEIGHT*item;
    uint16_t bg = config.menu_normal_color;
    uint16_t fg = DEFAULT_MENU_TEXT_COLOR;
    //    ili9341_fill(320-MENU_BUTTON_WIDTH, y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT-2, bg);
    ili9341_set_foreground(fg);
    ili9341_set_background(bg);
    char buf[10];
    ili9341_fill(50+25, y, 170, MENU_BUTTON_HEIGHT-2, bg);
    if (menu == menu_lowoutputmode) {
    switch (item) {
    case 0:
      set_sweep_frequency(ST_SPAN, 0);          // For CW sweep mode
      frequency_string(buf, sizeof buf, frequency0);
      break;
    case 1:
      plot_printf(buf, sizeof buf, "%ddB", -10 - settingAttenuate);
      break;
    }
    }
    if (menu == menu_highoutputmode) {
    switch (item) {
    case 0:
      set_sweep_frequency(ST_SPAN, 0);          // For CW sweep mode
      frequency_string(buf, sizeof buf, frequency0);
      break;
    case 1:
      plot_printf(buf, sizeof buf, "%ddB", -10 - settingDrive);
      break;
    }
    }
    ili9341_drawstring_size(buf, 130, y+6, 2);
  }
  }else{
    area_width = AREA_WIDTH_NORMAL - MENU_BUTTON_WIDTH;
  }
}

static void fetch_numeric_target(void)
{
  switch (keypad_mode) {
  case KM_START:
    uistat.value = get_sweep_frequency(ST_START);
    break;
  case KM_STOP:
    uistat.value = get_sweep_frequency(ST_STOP);
    break;
  case KM_CENTER:
    uistat.value = get_sweep_frequency(ST_CENTER);
    break;
  case KM_SPAN:
    uistat.value = get_sweep_frequency(ST_SPAN);
    break;
  case KM_CW:
    uistat.value = get_sweep_frequency(ST_CW);
    break;
  case KM_SCALE:
    uistat.value = get_trace_scale(uistat.current_trace) * 1000;
    break;
  case KM_REFPOS:
    uistat.value = get_trace_refpos(uistat.current_trace) * 1000;
    break;
  case KM_ATTENUATION:
    uistat.value = settingAttenuate;
     break;
  case KM_ACTUALPOWER:
    uistat.value = settingLevelOffset();
    break;
  case KM_IF:
    uistat.value = frequency_IF;
    break;
  case KM_SAMPLETIME:
    uistat.value = settingSpeed;
    break;
  case KM_DRIVE:
    uistat.value = settingDrive;
    break;

  }
  
  {
    uint32_t x = uistat.value;
    int n = 0;
    for (; x >= 10 && n < 9; n++)
      x /= 10;
    uistat.digit = n;
  }
//  uistat.previous_value = uistat.value;
}

static void
set_numeric_value(void)
{
  switch (keypad_mode) {
  case KM_START:
    set_sweep_frequency(ST_START, uistat.value);
    break;
  case KM_STOP:
    set_sweep_frequency(ST_STOP, uistat.value);
    break;
  case KM_CENTER:
    set_sweep_frequency(ST_CENTER, uistat.value);
    break;
  case KM_SPAN:
    set_sweep_frequency(ST_SPAN, uistat.value);
    break;
  case KM_CW:
    set_sweep_frequency(ST_CW, uistat.value);
    break;
  case KM_SCALE:
    set_trace_scale(0, uistat.value / 1000.0);
    set_trace_scale(1, uistat.value / 1000.0);
    set_trace_scale(2, uistat.value / 1000.0);
    break;
  case KM_REFPOS:
    set_trace_refpos(0, NGRIDY - uistat.value / get_trace_scale(0));
    set_trace_refpos(1, NGRIDY - uistat.value / get_trace_scale(0));
    set_trace_refpos(2, NGRIDY - uistat.value / get_trace_scale(0));
    break;
  case KM_ATTENUATION:
    SetAttenuation(uistat.value);
    break;
  case KM_ACTUALPOWER:
    SetPowerLevel(uistat.value);
    config_save();
    break;
  case KM_IF:
    frequency_IF = uistat.value;
    config_save();
    break;
  case KM_SAMPLETIME:
    settingSpeed = uistat.value;
    break;
  case KM_DRIVE:
    settingDrive = uistat.value;
    break;
  }
}
