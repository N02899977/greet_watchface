#include <pebble.h>

enum {
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS
};

Window *window;
TextLayer *hour_layer;
TextLayer *min_layer;
TextLayer *sec_layer;
TextLayer *ampm_layer;
TextLayer *date_layer;
static TextLayer *s_weather_layer;
TextLayer *greet_layer;
TextLayer *line_layer;
TextLayer *line_layer2;


void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
    // handle time and set to text layers
    static char hour_buffer[10];
    static char min_buffer[10];
    static char sec_buffer[10];
    static char ampm_buffer[10];
    static char date_buffer[14];
    strftime(hour_buffer, sizeof(hour_buffer), "%I", tick_time);
    strftime(min_buffer, sizeof(min_buffer), "%M", tick_time);
    strftime(sec_buffer, sizeof(sec_buffer), "%S", tick_time);
    strftime(ampm_buffer, sizeof(ampm_buffer), "%p", tick_time);
    text_layer_set_text(hour_layer, hour_buffer);
    text_layer_set_text(min_layer, min_buffer);
    text_layer_set_text(sec_layer, sec_buffer);
    text_layer_set_text(ampm_layer, ampm_buffer);
    
    strftime(date_buffer, sizeof(date_buffer), "%b %d", tick_time);
    text_layer_set_text(date_layer, date_buffer);

    // display a different greeting during certain hours
    static char hour[10];
    static char greet1[20] = "Why are you awake?";
    static char greet2[20] = "Good Morning";
    static char greet3[20] = "Good Afternoon";
    static char greet4[20] = "Good Evening";
    static char greet5[20] = "Good Night";
    strftime(hour, sizeof(hour), "%H", tick_time);
    int hour_int = atoi(hour);
    if(hour_int>=0 && hour_int<4)
      text_layer_set_text(greet_layer, greet1);
    else if(hour_int>=4 && hour_int<11)
      text_layer_set_text(greet_layer, greet2);
    else if(hour_int>=11 && hour_int<16)
      text_layer_set_text(greet_layer, greet3);
    else if(hour_int>=16 && hour_int<21)
      text_layer_set_text(greet_layer, greet4);
    else if(hour_int>=21 && hour_int<23)
      text_layer_set_text(greet_layer, greet5);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, KEY_CONDITIONS);
  
  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", (int)temp_tuple->value->int32);
//     snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);
 
  }
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

void handle_init(void) {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  
	// Create a window and text layer
	window = window_create();
  greet_layer = text_layer_create(GRect(0, 0, 144, 20));
	hour_layer = text_layer_create(GRect(0, 15, 144, 34));
  min_layer = text_layer_create(GRect(0, 49, 144, 34));
  sec_layer = text_layer_create(GRect(0, 83, 144, 54));
  ampm_layer = text_layer_create(GRect(95, 87, 49, 34));
	date_layer = text_layer_create(GRect(0, 135, 72, 68));
  s_weather_layer = text_layer_create(GRect(72, 135, 72, 45));
  line_layer = text_layer_create(GRect(10, 130, 124, 2));
  line_layer2 = text_layer_create(GRect(71, 130, 2, 32));
  
	// Set the text, font, and text alignment
	text_layer_set_text(greet_layer, "...");
	text_layer_set_font(hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_font(min_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_font(sec_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_font(ampm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_font(greet_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(min_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(sec_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(ampm_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(greet_layer, GTextAlignmentCenter);
  text_layer_set_background_color(hour_layer, GColorBlack);
  text_layer_set_background_color(min_layer, GColorBlack);
  text_layer_set_background_color(sec_layer, GColorBlack);
  text_layer_set_background_color(ampm_layer, GColorBlack);
  text_layer_set_background_color(greet_layer, GColorBlack);
  text_layer_set_background_color(date_layer, GColorBlack);
  text_layer_set_background_color(s_weather_layer, GColorBlack);
  text_layer_set_text_color(hour_layer, GColorWhite);
  text_layer_set_text_color(min_layer, GColorWhite);
  text_layer_set_text_color(sec_layer, GColorWhite);
  text_layer_set_text_color(ampm_layer, GColorWhite);
  text_layer_set_text_color(greet_layer, GColorWhite);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text(s_weather_layer, "temp...");
  	
  // Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(hour_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(min_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(sec_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(ampm_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(greet_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(line_layer2));
  
  // Subscribe to tick timer service
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Loggedzors");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(hour_layer);
  text_layer_destroy(min_layer);
  text_layer_destroy(sec_layer);
  text_layer_destroy(ampm_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(greet_layer);
  text_layer_destroy(line_layer);
  text_layer_destroy(line_layer2);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
