#include <pebble.h>
#include "Inspiration.h"
#define AUTHOR 0
#define QUOTE 1
#define DISPLAY 2

static Window *s_main_window;

static ScrollLayer *s_scroll_layer;

static TextLayer *s_text_layer;

static InverterLayer *s_inverter_layer;

static char s_scroll_text[]= "Welcome to Brainy Pebble! Crunching famous pebble quotes just for you... Loading... Firing missiles... Petting unicorns... Awesomeness awaits... Programming the flux capacitor... Spinning up the hamster... ";

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  s_scroll_layer = scroll_layer_create(bounds);

  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);

  s_text_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(s_text_layer, s_scroll_text);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  GSize max_size = text_layer_get_content_size(s_text_layer);
  text_layer_set_size(s_text_layer, max_size);
  scroll_layer_set_content_size(s_scroll_layer, GSize(bounds.size.w, max_size.h + 4));

  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));

  s_inverter_layer = inverter_layer_create(GRect(0, 28, bounds.size.w, 28));
  scroll_layer_add_child(s_scroll_layer, inverter_layer_get_layer(s_inverter_layer));

  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));
}

static void main_window_unload(Window *window) {
  inverter_layer_destroy(s_inverter_layer);
  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
   Tuple *info = dict_read_first(iterator);

  while(info!= NULL) {
    switch(info->key) {
    case AUTHOR:
      break;
    
    case QUOTE:
      break;
      
    case DISPLAY:
      text_layer_set_text(s_text_layer, info->value->cstring);

      GSize max_size = text_layer_get_content_size(s_text_layer);
      text_layer_set_size(s_text_layer, max_size);
      scroll_layer_set_content_size(s_scroll_layer, GSize(144, max_size.h + 4));
      break;
      
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized", (int)info->key);
      break;
    }
    info = dict_read_next(iterator);
  }
  
  //APP_LOG(APP_LOG_LEVEL_ERROR, t->value->cstring);
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}