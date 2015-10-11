#include <pebble.h>

#define COLOR_KEY 1

static Window *s_main_window;
static TextLayer *s_question_layer, *s_prompt_layer;

static DictationSession *s_dictation_session;
static char s_last_text[256];

static GColor col;
static int color;

//not sure if i want to put more strings...
static char s_questions[1][64] = {"Say a color you want for the watchface"};

//dictation behaviour unknown, omit the first letter to match colors, in case of capitalizations
static char s_colors[11][32] = {
  "lue",       // blue/Blue
  "ink",      // pink/Pink
  "reen",    // green/Green
  "lack",    // black/Black
  "rown",    // Brown/brown
  "ray",     // Gray/gray
  "ed",      // red/Red
  "agenta",   // Magenta/magenta
  "hite",      // White/white
  "ellow",     // Yellow/yellow
  "range"     // orange/Orange
};

static bool s_speaking_enabled;

/******************************** TIME ****************************************/

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char date_buff[] = "MMM-DD";

  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  strftime(date_buff, sizeof("MMM-DD"), "%b-%d", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_layer, date_buff);
}

/********************************* Color Picker *********************************/

static void determine_color (char *answer) {
  int x;
  color = 3; //default color is black
  bool matched;
  for (x = 0; x<= 10; x++) {
     matched = strstr (answer, s_colors[x]);
     if (matched) {
      color = x;
      break;
     }
  }
}

static void pick_color() {
  col = GColorBlack;
  switch ( color ) {
  case 0:  
    col = GColorElectricUltramarine;          
    break;
  case 1:    
    col = GColorFolly;        
    break;
  case 2:  
    col = GColorMediumSpringGreen;  
    break;
  case 3: 
    col = GColorBlack;     
    break;
  case 4:  
    col = GColorWindsorTan;          
    break;
  case 5:   
    col = GColorDarkGray;         
    break;
  case 6: 
    col = GColorRed;   
    break;
  case 7:   
    col = GColorFashionMagenta;   
    break;
  case 8:  
    col = GColorWhite;          
    break;
  case 9: 
    col = GColorYellow;            
    break;
  case 10: 
    col = GColorOrange;   
    break;
  default:      
    break;
  }
}

/******************************* Dictation API ********************************/

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status, 
                                       char *transcription, void *context) {
  if(status == DictationSessionStatusSuccess) {
    determine_color(transcription);
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Transcription failed.\n\nError ID:\n%d", (int)status);
  }
}

/************************************ App *************************************/

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(s_speaking_enabled) {
    // Start voice dictation UI
    dictation_session_start(s_dictation_session);
    s_speaking_enabled = false;
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  pick_color();
  window_set_background_color(window, col);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 25, 167, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  s_date_layer = text_layer_create(GRect(5, 90, 167, 60));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, "MMM-DD");
  
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void question_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_question_layer = text_layer_create(GRect(16, 20, bounds.size.w - 45, bounds.size.h));
  text_layer_set_font(s_question_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(s_question_layer, GColorWhite);
  text_layer_set_text_alignment(s_question_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_question_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_question_layer));

  s_prompt_layer = text_layer_create(GRect(5, 120, bounds.size.w - 10, bounds.size.h));
  text_layer_set_text(s_prompt_layer, "Press Select to speak your answer!");
  text_layer_set_font(s_prompt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_color(s_prompt_layer, GColorWhite);
  text_layer_set_text_alignment(s_prompt_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_prompt_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_prompt_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_prompt_layer);
  text_layer_destroy(s_question_layer);
    //Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  //Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_click_config_provider(s_main_window, click_config_provider);
  if (persist_exists(COLOR_KEY)) {
    color = persist_read_int(COLOR_KEY);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });   
    window_stack_push(s_main_window, true);

    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  } else {
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = question_window_load,
      .unload = window_unload,
    });

    window_stack_push(s_main_window, true);

    // Create new dictation session
    s_dictation_session = dictation_session_create(sizeof(s_last_text), 
                                                   dictation_session_callback, NULL);
    dictation_session_enable_confirmation(s_dictation_session, false);
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_text(s_question_layer, s_questions[0]);
    s_speaking_enabled = true;
  }
}

static void deinit() {
  persist_write_int(COLOR_KEY, color);
  // Free the last session data
  dictation_session_destroy(s_dictation_session);
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}