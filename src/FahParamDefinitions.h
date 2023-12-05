#pragma once
#define PID_LED_DAY_BRIGHTNESS 0x0001 // LED Brightness during the day
#define PID_LED_NIGHT_BRIGHTNESS 0x0002 // LED Brightness during the day
#define PID_DIMMING_ACTUATOR_TYPE1_LOAD_TYPE 0x0003 // Load type of dimmer
#define PID_DIMMING_ACTUATOR_MIN_BRIGHTNESS 0x0004 // Dimming actuator minimum brightness
#define PID_DIMMING_ACTUATOR_MAX_BRIGHTNESS_DAY 0x0005 // Dimming actuator maximum brightness during the day
#define PID_SENSOR_FORCE_ON_OFF 0x0006 // Sensor forces light to on or off
#define PID_LED_OPERATION_MODE 0x0007 // LED operation mode (status indication/orientation light)
#define PID_TOTAL_BLINDS_MOVEMENT_TIME 0x0008 // Total blinds movement time up
#define PID_TOTAL_BLINDS_MOVEMENT_TIME_CALIBRATED 0x0009 // Total blinds movement time up calibrated
#define PID_TOTAL_SLATS_MOVEMENT_TIME 0x000A // Total slats movement time
#define PID_TOTAL_SLATS_MOVEMENT_TIME_CALIBRATED 0x000B // Total slats movement time calibrated
#define PID_CHANNEL_SELECTOR_2_GANG 0x000C // Channel selector for 2 gang sensor
#define PID_SENSOR_FORCE_UP_DOWN 0x000D // Sensor Force blinds up or down
#define PID_CHANNEL_SELECTOR_BINARY_INPUT_2_GANG 0x000E // Channel selector for 2 gang binary input
#define PID_CHANNEL_SELECTOR_BINARY_INPUT_4_GANG 0x000F // Channel selector for 4 gang binary input
#define PID_BI_CONTACT_TYPE 0x0010 // binary input contact type
#define PID_BI_FORCE_ON_OFF 0x0011 // binary input sensor force light on or off
#define PID_DIMMING_ACTUATOR_MAX_BRIGHTNESS_NIGHT 0x0012 // Dimming actuator max brightness at night
#define PID_DIMMING_ACTUATOR_TYPE0_LOAD_TYPE 0x0013 // Dimming actuator load types (DimmingActuator_Type1)
#define PID_BEHAVIOUR_ON_MALFUNCTION 0x0014 // Behaviour On Malfunction
#define PID_AUTONOMOUS_SWITCH_OFF_TIME_DURATION 0x0015 // Autonomous Switch Off Time Duration
#define PID_DEFAULT_SETPOINT_TEMPERATURE 0x0018 // Default Set Point Temperature
#define PID_ECO_DELTA_TEMPERATURE 0x0019 // Eco Delta Temperature
#define PID_PRESENCE_DETECTOR_TIMED_ON_DURATION 0x001A // Presence Detector Timed On Duration
#define PID_TEMPERATURE_CORRECTION 0x001B // Temperature Correction
#define PID_TEMPERATURE_SEND_INTERVAL 0x001C // Current Temperature Send Interval
#define PID_MAX_TEMP_ADJ 0x001D // Max Temperature Adjustment
#define PID_MIN_TEMP_ADJ 0x001E // Min Temperature Adjustment
#define PID_RTC_HAS_FANCOIL 0x0020 // RTC has FanCoil
#define PID_DISPLAY_SWITCH_OFF_TIME 0x0021 // Display switch off time
#define PID_FAIR_MODE 0x0022 // Fair Mode
#define PID_SCENE_MODE 0x0024 // Store scene on long press or recall it
#define PID_SWITCH_ON_MODE_RGB 0x0025 // Switch On Mode RGB
#define PID_CHANNEL_SELECTOR_8_GANG 0x0026 // Channel selector for 8 gang sensor
#define PID_SWITCH_ACTUATOR_BEHAVIOR_ON_MALFUNCTION 0x0027 // SA-M: Behaviour On Malfunction for switch actuator
#define PID_DELAY_TIME 0x0028 // SA-M: Delay Time
#define PID_DIMMER_SWITCH_ON_MODE 0x0029 // Dimmer Switch On Mode
#define PID_TEMPERATURE_SOURCE 0x002A // Temperature Source
#define PID_BRIGHTNESS_ALERT_ACTIVATION_LEVEL 0x002B // Brightness alert activation level
#define PID_HYSTERESIS 0x002C // Hysteresis brightness alert
#define PID_FROST_ALARM_ACTIVATION_LEVEL 0x002D // Frost Alarm activation level
#define PID_WIND_FORCE 0x002E // Wind Alarm activation level
#define PID_CONTACT_TYPE 0x002F // Heating Actuator Contact Type
#define PID_BEHAVIOUR_ON_MALFUNCTION_HEATING 0x0030 // Heating Actuator Behaviour on Malfunction
#define PID_PWM_PERIOD 0x0031 // Heating Actuator PWM Period
#define PID_TRIGGER_DURATION 0x0032 // Trigger Duration (Wisch-Impuls)
#define PID_MOVEMENT_DETECTOR_SWITCH_ON_CONDITIONS 0x0034 // Movement Detector: Switch on condition
#define PID_TOTAL_BLINDS_MOVEMENT_TIME_DOWN 0x0035 // Total blinds movement time down
#define PID_TOTAL_BLINDS_MOVEMENT_TIME_CALIBRATED_DOWN 0x0036 // Total blinds movement time down calibrated
#define PID_ENGINE_SWITCH_ON_DELAY_TIME 0x0037 // Slat switch on delay (Somfy)
#define PID_DEVICE_CHANNEL_SELECTOR 0x0038 // Device channel selector
#define PID_TRANSMISSION_INTERVAL 0x0039 // Transmission interval
#define PID_FAN_COIL_FAN_SPEED 0x003A // FanCoil Actuator Fan speed
#define PID_FAN_COIL_NIGHT_MODE 0x003B // FanCoil Actuator Night mode
#define PID_FAN_COIL_FAN_MODE 0x003C // FanCoil Actuator Fan mode
#define PID_FAN_COIL_FAN_STARTUP_TIME 0x003D // FanCoil Actuator Fan startup time with full power
#define PID_FAN_COIL_FAN_CHANGEOVER_DELAY 0x003E // FanCoil Actuator Fan changeover delay
#define PID_CONTACT_TYPE_A 0x003F // FanCoil Actuator Contact type sensor (a)
#define PID_CONTACT_TYPE_B 0x0040 // FanCoil Actuator Contact type sensor (b)
#define PID_FAN_COIL_VALVE_OPENING_TIME 0x0041 // FanCoil Actuator Valve opening time
#define PID_FAN_COIL_PWM_PROFILE 0x0042 // FanCoil Actuator Valve PWM profile
#define PID_SENSOR_TYPE 0x0043 // BI: Sensor type (Pushbutton/switch toggle/switch direct )
#define PID_FAN_COIL_VALVE_3POINT 0x0044 // FCA: Valve typ (NC/NO/3-Piont)
#define PID_FAN_COIL_VALVE_NCNO 0x0045 // FCA: Valve typ (NC/NO)
#define PID_PRIMARY_FUNCTION 0x0046 // Primary function
#define PID_ALERT_ACTIVATION_DELAY 0x0047 // Alert activiation delay
#define PID_DEALERT_ACTIVATION_DELAY 0x0048 // Alert deactiviation delay
#define PID_RAIN_SENSOR_SENSITIVITY 0x0049 // Rain sensor sensitivity
#define PID_HOURS_PER_DAY 0x004A // Hours per day
#define PID_USER_LANGUAGE 0x004B // User language
#define PID_EFFECTIVE_USER_LANGUAGE 0x004C // Effective user language
#define PID_TEMP_SENSOR_USAGE 0x004D // Temperature sensor usage
#define PID_EXT_TEMP_OFFSET 0x004E // Temperature correction external sensor
#define PID_TEMP_LIMIT 0x004F // Temperature limit
#define PID_ACOUSTIC_FEEDBACK 0x0050 // Acoustic Feedback
#define PID_REFERENCE_DRIVE_BEFORE_POSITIONING 0x0054 // AlwaysCalibrateBeforeAbsolutePositionRequest
#define PID_PROVIDE_TEMPERATURE 0x0055 // Provide temperature
#define PID_HC_ACTIVE_THRESHOLD 0x0056 // HeatingCoolingActiveThreshold
#define PID_POWER_SAVING_MODE 0x0057 // Power Saving Mode
#define PID_HANDLE_WIND_ALARM 0x0058 // Handle wind alarm
#define PID_HANDLE_RAIN_ALARM 0x0059 // Handle rain alarm
#define PID_HANDLE_FROST_ALARM 0x005A // Handle frost alarm
#define PID_HANDLE_WINDOW_ALARM 0x005B // Handle window alarm
#define PID_BEHAVIOUR_DISABLE_FORCE 0x005C // Behaviour disable force
#define PID_SONOS_GROUP_BIT_POSITION 0x005D // Bit position for sonos player in group mask
#define PID_SONOS_SENSOR_BEHAVIOUR 0x005E // Sonos sensor behaviour
#define PID_SONOS_PLAYER_VOLUME_LIMIT 0x005F // Sonos player volume limit
#define PID_CEILING_FAN_SWITCH_ON_MODE 0x0060 // Ceiling fan switch on mode
#define PID_CFAT0_MINIMUM_FAN_SPEED 0x0061 // Minimum fan speed
#define PID_CFAT0_MAXIMUM_FAN_SPEED 0x0062 // Maximum fan speed
#define PID_TEMPERATURE_UNIT 0x0063 // Temperature unit
#define PID_EFFECTIVE_TEMPERATURE_UNIT 0x0064 // Effective temperature unit
#define PID_AWAKE_INDICATOR 0x0065 // Awake indicator
#define PID_HEATING_FIRST_STAGE 0x0066 // First stage heating system
#define PID_HEATING_SECOND_STAGE 0x0067 // Second stage heating system
#define PID_COOLING_FIRST_STAGE 0x0068 // First stage cooling system
#define PID_COOLING_SECOND_STAGE 0x0069 // Second stage cooling system
#define PID_REVERSING_VALVE_OPMODE 0x006A // Reversing valve is engaged
#define PID_AUX_TYPE 0x006B // Type of generic additional system (AUX)
#define PID_COMPRESSOR_DELAY 0x006C // Compressor delay
#define PID_TEMPERATURE_EXTRACT_HEAT 0x006D // Temperature extraction heat
#define PID_TEMPERATURE_EXTRACT_COOL 0x006E // Temperature extraction cool
#define PID_VALUES_IN_ACTIVE_MODE 0x006F // Values in Active Mode
#define PID_SHOW_TEMPERATURE_WHILE_OFF 0x0070 // Show temperature when switched off
#define PID_SHORTCUT_FOR_FAN 0x0071 // Shortcut for the fan symbol
#define PID_SHORTCUT_FOR_HEAT 0x0072 // Shortcut for heat enable
#define PID_SHORTCUT_FOR_COOL 0x0073 // Shortcut for cool enable
#define PID_SHORTCUT_FOR_AUX 0x0074 // Shortcut for aux enable
#define PID_SHORTCUT_FOR_ECO 0x0075 // Shortcut for eco enable
#define PID_SHORTCUT_FOR_OFF 0x0076 // Shortcut for off enable
#define PID_FAN_MANUAL_RUNTIME 0x0077 // Fan manual runtime
#define PID_DELAY_FOR_EXIT_MODE 0x0078 // Delay for active mode exit
#define PID_PROTECTION_HIGH_LIMIT 0x0079 // Protection mode upper temperature limit
#define PID_PROTECTION_LOW_LIMIT 0x007A // Protection mode lower temperature limit
#define PID_MINIMUM_ON_TIME 0x007B // Minimum on time for gas furnaces
#define PID_FAN_ON_TIME_LAG 0x007C // Fan ON time lag
#define PID_FAN_OFF_TIME_LAG 0x007D // Fan OFF time lag
#define PID_COMPRESSOR_LOCKOUT 0x007E // Compressor lockout
#define PID_HEAT_PUMP_AUX_HEAT_LOCKOUT 0x007F // Heat pump aux heat lockout
#define PID_FIRST_STAGE_COMP_CYCLE_RATE 0x0080 // 1st stage compressor cycle rate: max cycles per hour
#define PID_SECOND_STAGE_COMP_CYCLE_RATE 0x0081 // 2nd stage compressor cycle rate: max cycles per hour
#define PID_FIRST_STAGE_HEAT_CYCLE_RATE 0x0082 // 1st stage heat cycle rate: max cycles per hour
#define PID_SECOND_STAGE_HEAT_CYCLE_RATE 0x0083 // 2nd stage heat cycle rate: max cycles per hour
#define PID_EMERGENCY_HEAT_CYCLE_RATE 0x0084 // Emergency heat cycle rate: max cycles per hour
#define PID_FAN_CONTROL 0x0085 // Fan control
#define PID_HEAT_PUMP_TIME 0x0086 // Heat pump time between equipment stages
#define PID_HIGH_COOL_LATCH 0x0087 // High cool latch temperature
#define PID_HIGH_HEAT_LATCH 0x0088 // High Heat latch temperature
#define PID_COOLING_HYSTERESIS 0x0089 // Cooling hysteresis
#define PID_HEATING_HYSTERESIS 0x008A // Heating hysteresis
#define PID_HC_SWITCH_DELAY 0x008B // Switching delay between heating and cooling
#define PID_MAXIMUM_SETPOINT_TEMPERATURE 0x008D // Maximum setpoint tempeature
#define PID_MINIMUM_SETPOINT_TEMPERATURE 0x008E // Minimum setpoint temperature
#define PID_STAGING_TIMER_OVERRIDE 0x008F // Staging timer override time when demand diff greater five fahrenheit
#define PID_ARMING_OPTION 0x0090 // Arming option
#define PID_DAY_OF_WEEK 0x0091 // Day of week
#define PID_START_TIME 0x0092 // Start time
#define PID_END_TIME 0x0093 // End time
#define PID_ARM_DELAY 0x0094 // Arm delay in seconds
#define PID_SILENT_MODE 0x0097 // SilentMode
#define PID_PAIRING_PARTNER_PRODUCTION_NUMBER 0x0098 // The production number of the pairing partner. The value is set automatically by the SysAP.
#define PID_PAIRING_PARTNER_MANUFACTURER 0x0099 // The manufacturer of the pairing partner. The value is set automatically by the SysAP
#define PID_SWITCH_OFF_DELAY_MIN 0x009A // Switch off delay in minutes
#define PID_TWO_LEVEL_CONTROLLER_HYSTERESIS 0x009B // Hysteresis of a two level controller
#define PID_LED_BRIGHTNESS 0x009C // LED Brightness
#define PID_LED_OFF_AT_NIGHT 0x009D // LED off at night
#define PID_USER_MODE 0x009E // Normal / Power user mode
#define PID_INTERNAL_SIREN 0x00A3 // Enable or Disable internal siren
#define PID_WIZARD_COMPLETED 0x00D4 // Has the wizard been completed previously
#define PID_TWILIGHT_BRIGHTNESS 0x00D5 // Brightness level for twilight recognition
#define PID_WIRING 0x00D6 // Wiring
#define PID_WIND_SPEED_UNIT 0x00D8 // Wind speed unit
#define PID_EFFECTIVE_WIND_SPEED_UNIT 0x00D9 // Effective wind speed unit
#define PID_FAN_LEVEL_STEP_WIDTH 0x00DC // Fan level step width
#define PID_DIMMING_ACTUATOR_TYPE2_LOAD_TYPE 0x00DD // Loadtyp of multichannel led dimmer with led trailing edge support
#define PID_RESET_MANUAL_FAN_STAGE 0x00E1 // Reset manual fan stage
#define PID_USE_MASTER_MEASURED_TEMPERATURE 0x00E2 // Use master measured temperature
#define PID_LIMIT_TEMPERATURE 0x00E3 // Activates/Deactivates temperature limitation
#define PID_SEND_IR_TELEGRAM 0x00E4 // Always send ir telegrams
#define PID_LOCK_ON_SITE_OPERATION 0x00E5 // Lock on-site operation
#define PID_USE_TEMPLATE 0x00E6 // Use parameter data from template
#define PID_OPENER_CONTACT 0x00E7 // Type of contact (opener)
#define PID_OUTPUT_EXECUTES_CENTRAL_INPUT 0x00E8 // Output executes command from central input
#define PID_POWER_ON_LEVEL 0x00E9 // Power on level per gateway
#define PID_SYSTEM_FAILURE_LEVEL 0x00EA // System failure level per gateway
#define PID_CHANNEL_SELECTOR_AUX_INPUT_SHUTTER 0x00EB // Channel selector for shutter AUX input
#define PID_CHANNEL_SELECTOR_AUX_INPUT_SWITCH 0x00EC // Channel selector for switch AUX input
#define PID_CHANNEL_SELECTOR_1_GANG 0x00ED // Channel selector for 1 gang sensor
#define PID_CHANNEL_SELECTOR_3_GANG 0x00EE // Channel selector for 3 gang sensor
#define PID_VOLUME_SETUP 0x00EF // Volume setup
#define PID_AUTO_RELOCK_TIME 0x00F0 // Door auto relock time
#define PID_ALBUM_COVER_URL 0x00F1 // Album cover URL
#define PID_ZSTICK_CONTROL 0x00F2 // ZWave USB stick control
#define PID_TEMPERATURE_COLOR_PHYSICAL_COOLEST 0x00F5 // DALI DT8 TC physical coolest temperature limit
#define PID_TEMPERATURE_COLOR_PHYSICAL_WARMEST 0x00F6 // DALI DT8 TC physical warmest temperature limit
#define PID_DOMUS_SCENE_DISARMING 0x00F7 // Disarm via scene
#define PID_DOMUS_DISABLE_INTERNAL_BUZZER 0x00F8 // Disable internal buzzer
#define PID_DOMUS_ALARM_ON_BURGLARY 0x00F9 // Alarm on burglarie
#define PID_OPERATION_MODE 0x00FA // Operation mode
#define PID_DETECTION_AREA 0x00FB // Erfassungsbereich
#define PID_MOVEMENT_DETECTOR_TEST_MODE 0x00FC // Movement detector test mode
#define PID_BLIND_SENSOR_OPERATION_MODE 0x00FD // Operation mode
#define PID_SWITCH_OFF_PREWARNING 0x00FE // Prewarning before autonomous switch off
#define PID_HEATING_ACTUATOR_PAUSE_TIME 0x00FF // Pause time for heating actuators
#define PID_ACOUSTIC_FEEDBACK_VOLUME 0x0100 // Volume of acoustic feedback
#define PID_COLOR_THEME 0x0101 // Volume of acoustic feedback
#define PID_BRIGHTNESS_FOR_SWITCHING_TO_DARK_MODE 0x0102 // 
#define PID_AUTOMATIC_RETURN_TO_START_PAGE 0x0103 // 
#define PID_BRIGHTNESS_ADJUSTED_TO_AMBIENT_LIGHT 0x0104 // 
#define PID_SWITCH_DISPLAY_OFF_WHEN_ROOM_IS_DARK 0x0105 // 
#define PID_BRIGHTNESS_LEVEL_FOR_SWITCHING_TO_DARK 0x0106 // 
#define PID_SWITCH_OFF_TIME_AFTER_DARK 0x0107 // 
#define PID_USE_PROXIMITY_SENSOR 0x0108 // 
#define PID_PROXIMITY_SENSOR_SENSITIVITY 0x0109 // 
#define PID_HAPTIC_FEEDBACK 0x010A // 
#define PID_SCEEN_SAVER 0x010B // 
#define PID_SCEEN_SAVER_MODE 0x010C // 
#define PID_AUTOMATIC_RETURN 0x010D // 
#define PID_CHANNEL_SELECTOR_MOVEMENT_DETECTOR 0x010E // Channel selector for switch AUX input
#define PID_CHANNEL_SELECTOR_AUX_INPUT_SWITCH_1_GANG 0x010F // Channel selector for switch AUX input
#define PID_CHANNEL_SELECTOR_CONNECTED_VALVES 0x0110 // Channel selector for heating / fan coil actuator
#define PID_CHANNEL_SELECTOR_CHANNEL_1 0x0111 // Channel selector for 1st sensor channel
#define PID_CHANNEL_SELECTOR_CHANNEL_2 0x0112 // Channel selector for 2nd sensor channel
#define PID_CHANNEL_SELECTOR_CHANNEL_3 0x0113 // Channel selector for 3rd sensor channel
#define PID_CHANNEL_SELECTOR_CHANNEL_4 0x0114 // Channel selector for 4th sensor channel
#define PID_CHANNEL_SELECTOR_CHANNEL_5 0x0115 // Channel selector for 5th sensor channel
#define PID_CHANNEL_SELECTOR_CHANNEL_6 0x0116 // Channel selector for 6th sensor channel
#define PID_DOMUS_USER_INSTALLER 0x0117 // 
#define PID_DOMUS_USER_0 0x0118 // 
#define PID_DOMUS_USER_1 0x0119 // 
#define PID_DOMUS_USER_2 0x0121 // 
#define PID_DOMUS_USER_3 0x0122 // 
#define PID_DOMUS_USER_4 0x0123 // 
#define PID_DOMUS_USER_5 0x0124 // 
#define PID_DOMUS_USER_6 0x0125 // 
#define PID_DOMUS_USER_7 0x0126 // 
#define PID_DOMUS_USER_8 0x0127 // 
#define PID_DOMUS_KEY_0 0x0128 // 
#define PID_DOMUS_KEY_1 0x0129 // 
#define PID_DOMUS_KEY_2 0x0130 // 
#define PID_DOMUS_KEY_3 0x0131 // 
#define PID_DOMUS_KEY_4 0x0132 // 
#define PID_DOMUS_SIREN_INTRUSION_RINGTONE 0x0133 // 
#define PID_DOMUS_SIREN_INTRUSION_LED 0x0134 // 
#define PID_DOMUS_SIREN_SAFETY_RINGTONE 0x0135 // 
#define PID_DOMUS_SIREN_SAFETY_LED 0x0136 // 
#define PID_CHANNEL_SELECTOR_MWIRE_ROCKER_1GANG 0x0137 // Channel selector for MWIRE rockers 1-gang
#define PID_CHANNEL_SELECTOR_MWIRE_ROCKER_2GANG 0x0138 // Channel selector for MWIRE rockers 2-gang
#define PID_DOMUS_SELECTED_REPEATER 0x0139 // 
#define PID_CHANNEL_SELECTOR_MWIRE_MOVEMENT_DETECTOR 0x013A // Channel selector for MWIRE Movement detector
#define PID_CHANNEL_SELECTOR_FUNCTION_INPUT_A 0x013B // Channel selector for input function of channel A
#define PID_CHANNEL_SELECTOR_FUNCTION_INPUT_B 0x013C // Channel selector for input function of channel B
#define PID_BI_CONTACT_TYPE_INPUT_A 0x013D // binary input A contact type
#define PID_BI_CONTACT_TYPE_INPUT_B 0x013E // binary input B contact type
#define PID_DIMMING_ACTUATOR_MAX_BRIGHTNESS 0x013F // Dimming actuator maximum brightness
#define PID_HEIGHT_ABOVE_SEA 0x0140 // Mounting height above sea level
#define PID_CALDION_UI_DURING_NON_USE 0x0141 // Caldion UI: What to do if not in use
#define PID_IGNORE_LOCAL_TEMPERATURE 0x0142 // Ignore local temperature
#define PID_PRESENCE_DETECTOR_SWITCH_ON_CONDITION 0x0143 // Presence detector switch on condition
#define PID_PRESENCE_DETECTOR_SWITCH_ON_MODE 0x0144 // Presence detector switch on mode
#define PID_PRESENCE_DETECTOR_TARGET_BRIGHTNESS 0x0145 // Presence detector target brightness
#define PID_BASIC_SETPOINT_HEATING_COMFORT 0x0146 // Basic Set point for heating in comfort mode
#define PID_BASIC_SETPOINT_HEATING_ECO_MODE 0x0147 // Basic Set point for heating in eco mode
#define PID_BASIC_SETPOINT_COOLING_COMFORT 0x0148 // Basic Set point for cooling in comfort mode
#define PID_BASIC_SETPOINT_COOLING_ECO_MODE 0x0149 // Basic Set point for cooling in eco mode
#define PID_TEMPERATURE_LOWER_LIMIT_FLOOR 0x015A // Temperatur lower limit floor
#define PID_TEMPERATURE_UPPER_LIMIT_FLOOR 0x015B // Temperatur upper limit floor
#define PID_ADAPTIVE_TIMER_CONTROL 0x015C // Adaptive controller for room temperature control
#define PID_EXTERNAL_INPUT_FUNCTION 0x015D // External input function type
#define PID_CONTROLLER_TYPE 0x015E // Controller type
#define PID_BUILDING_PROTECTION 0x015F // Building protection
#define PID_VALVE_PROTECTION 0x0160 // Valve protection
#define PID_EXTERNAL_TEMPERATURE_SENSOR_TYPE 0x0161 // External temperature sensor type
#define PID_SWITCH_OVER_TO_HEATING 0x0162 // Switch over to heating date (every year)
#define PID_SWITCH_OVER_TO_COOLING 0x0163 // Switch over to cooling date (every year)
#define PID_BOOSTER_TIMER 0x0164 // Timespan for booster function
#define PID_SWITCHING_CAPACITY 0x0165 // Switching capacity
#define PID_PIR_1_SENSITIVITY 0x0166 // PIR 1 sensitivity
#define PID_PIR_2_SENSITIVITY 0x0167 // PIR 2 sensitivity
#define PID_PIR_3_SENSITIVITY 0x0168 // PIR 3 sensitivity
#define PID_PIR_4_SENSITIVITY 0x0169 // PIR 4 sensitivity
#define PID_PIR_1_ACTIVE 0x016A // PIR 1 active
#define PID_PIR_2_ACTIVE 0x016B // PIR 2 active
#define PID_PIR_3_ACTIVE 0x016C // PIR 3 active
#define PID_PIR_4_ACTIVE 0x016D // PIR 4 active
#define PID_SWITCH_ON_DELAY 0x016E // Switch on delay time
#define PID_CHANNEL_SELECTOR_RTC_ASSIGNED_ROCKER 0x016F // Channel selector for number of rockers assigned to rtc
#define PID_CO2_ALERT_ACTIVATION_LEVEL 0x0170 // CO2 alert activation level
#define PID_VOC_ALERT_ACTIVATION_LEVEL 0x0171 // VOC alert activation level
#define PID_DISPLAY_IDLE_TIME 0x0172 // Display idle time
#define PID_STD_RTC_UI_DURING_NON_USE 0x0173 // StdRTC UI: What to do if not in use
#define PID_HUMIDITY_ALERT_LOWER_LIMIT 0x0174 // Humidity alert lower limit
#define PID_HUMIDITY_ALERT_UPPER_LIMIT 0x0175 // Humidity alert upper limit
#define PID_ENABLE_CO2_ALERT 0x0176 // Enable co2 alert
#define PID_ENABLE_VOC_ALERT 0x0177 // Enable voc alert
#define PID_ENABLE_HUMIDITY_ALERT 0x0178 // Enable humidity alert
#define PID_FEATURE_SELECTOR 0x0179 // Feature selector
#define PID_REDUCED_LAYOUT 0x017A // 
#define PID_MOUNTED_IN_SOLID_WALL 0x017B // Specifies if the device is mounted on a solid wall
#define PID_ENGINE_SWITCH_ON_DELAY_TIME_NEW 0x017C // Slat switch on delay (Somfy)
#define PID_SCENE_BACKGROUND_IMG 0x017D // Scene background image
#define PID_LED_COLOR 0x017E // LED color
#define PID_SWITCH_ROCKER_BUTTONS 0x017F // Switch rocker buttons
#define PID_ROCKER_ORIENTATION 0x0180 // Rocker orientation
#define PID_FEATURE_ORIENTATION 0x0181 // Feature: Orientation
#define PID_FEATURE_RGB_SUPPORT 0x0182 // Feature: RGB support
#define PID_FEATURE_SWITCH_ROCKER_BUTTONS 0x0183 // Feature: Switch rocker buttons
#define PID_USER_DEFINED_COLOR 0x0184 // User defined color
#define PID_CHANNEL_SELECTOR_GENERIC_GANG 0x0185 // Channel selector for generic sensor like fts
#define PID_SWITCH_ACTUATOR_OPERATION_MODE 0x0186 // Operation mode of switch actuator
#define PID_FORCE_MODE_DURATION_TYPE 0x0187 // Spefifies if the force mode shall be deactivated after a certain time or not
#define PID_FORCE_MODE_AUTONOMOUS_SWITCH_OFF_TIME_DURATION 0x0188 // Autonomous Switch Off Time Duration for the force mode
#define PID_LOW_ENERGY_DEVICE_CHANNEL_SELECTOR 0x0189 // channel selector for a proxy client that represents the type of the low energy device
#define PID_LOW_ENERGY_DEVICE_ADDRESS 0x018A // the address of the associated low energy device
