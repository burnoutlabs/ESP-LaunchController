/**
 * @file main.cpp
 * @author Ian Gray @iangray1000
 *
 * Based on the ESPUI example examples/completeLambda/completeLambda.ino
 * (https://github.com/s00500/ESPUI), Copyright (c) 2017 Lukas Bachschwell
 * (s00500), MIT licence. Modified by Cameron Brooks, 2024.
 * See THIRD_PARTY_NOTICES.md for the full ESPUI licence notice.
 *
 *
 * When this program boots, it will load an SSID and password from the EEPROM.
 * The SSID is a null-terminated C string stored at EEPROM addresses 0-31
 * The password is a null-terminated C string stored at EEPROM addresses 32-95.
 * If these credentials do not work for some reason, the ESP will create an Access
 * Point wifi with the SSID HOSTNAME (defined below). You can then connect and use
 * the controls on the "Wifi Credentials" tab to store credentials into the EEPROM.
 *
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <ESPUI.h>

#include <WiFi.h>
#include <ESPmDNS.h>

// Settings
#define SLOW_BOOT 0
#define HOSTNAME "ESP-LaunchController"
#define FORCE_USE_HOTSPOT 0
#define SERIAL_WAIT_MS 2000 // Longest time setup() waits for a serial monitor

// EEPROM layout for the stored Wi-Fi credentials. Each field holds a
// null-terminated string, so it fits at most (size - 1) characters.
#define EEPROM_SSID_ADDR 0
#define EEPROM_SSID_SIZE 32
#define EEPROM_PASS_ADDR (EEPROM_SSID_ADDR + EEPROM_SSID_SIZE)
#define EEPROM_PASS_SIZE 64
#define EEPROM_SIZE (EEPROM_PASS_ADDR + EEPROM_PASS_SIZE)

// Function Prototypes
void connectWifi();
void setUpUI();
void textCallback(Control *sender, int type);
void generalCallback(Control *sender, int type);
void writeStringToEEPROM(const String &str, int baseaddress, int size);

// UI handles
uint16_t wifi_ssid_text, wifi_pass_text;
uint16_t styleButton2;

// This is the main function which builds our GUI
void setUpUI()
{

		// Turn off verbose debugging
		ESPUI.setVerbosity(Verbosity::Quiet);

		// Make sliders continually report their position as they are being dragged.
		ESPUI.sliderContinuous = true;

		/*
		 * Tab: Example UI
		 * An example UI for the documentation
		 *-----------------------------------------------------------------------------------------------------------*/
		auto exampletab = ESPUI.addControl(Tab, "Launch Control", "Launch Control");
		ESPUI.addControl(Separator, "Control and Status", "", None, exampletab);
		ESPUI.addControl(Switcher, "Power", "1", Alizarin, exampletab, generalCallback);
		ESPUI.addControl(Label, "Status", "System status: OK", Wetasphalt, exampletab, generalCallback);
		styleButton2 = ESPUI.addControl(Button, "Launch Button", "Launch", Alizarin, exampletab, generalCallback);
		ESPUI.setPanelStyle(styleButton2, "width: 88%; background: linear-gradient(90deg, rgba(255,0,0,1) 0%, rgba(255,165,0,1) 50%, rgba(255,223,0,1) 100%); border-bottom: #000;");
		ESPUI.setElementStyle(styleButton2, "border-radius: 1em; border: 4px solid black; width: 80%; background-color: #ff4500;");

		ESPUI.addControl(Separator, "Settings", "", None, exampletab);
		ESPUI.addControl(PadWithCenter, "Attitude Control", "", Dark, exampletab, generalCallback);
		auto examplegroup1 = ESPUI.addControl(Button, "Activate Lights", "Light 1", Carrot, exampletab, generalCallback);
		ESPUI.addControl(Button, "Activate Lights", "Light 1", Carrot, examplegroup1, generalCallback);
		ESPUI.addControl(Button, "Activate Lights", "Light 1", Carrot, examplegroup1, generalCallback);
		ESPUI.addControl(Slider, "Voltage Level", "45", Peterriver, exampletab, generalCallback);

		/*
		 * Tab: WiFi Credentials
		 * You use this tab to enter the SSID and password of a wifi network to autoconnect to.
		 *-----------------------------------------------------------------------------------------------------------*/
		auto wifitab = ESPUI.addControl(Tab, "", "WiFi Credentials");
		wifi_ssid_text = ESPUI.addControl(Text, "SSID", "", Alizarin, wifitab, textCallback);
		// Note that adding a "Max" control to a text control sets the max length
		ESPUI.addControl(Max, "", "31", None, wifi_ssid_text); // EEPROM_SSID_SIZE - 1
		wifi_pass_text = ESPUI.addControl(Text, "Password", "", Alizarin, wifitab, textCallback);
		ESPUI.addControl(Max, "", "63", None, wifi_pass_text); // EEPROM_PASS_SIZE - 1
		ESPUI.addControl(Button, "Save", "Save", Peterriver, wifitab,
						 [](Control *sender, int type)
						 {
							 if (type == B_UP)
							 {
								 // Never print the password: serial output ends up in logs.
								 Serial.print("Saving Wi-Fi credentials to EEPROM for SSID: ");
								 Serial.println(ESPUI.getControl(wifi_ssid_text)->value);
								 EEPROM.begin(EEPROM_SIZE);
								 writeStringToEEPROM(ESPUI.getControl(wifi_ssid_text)->value, EEPROM_SSID_ADDR, EEPROM_SSID_SIZE);
								 writeStringToEEPROM(ESPUI.getControl(wifi_pass_text)->value, EEPROM_PASS_ADDR, EEPROM_PASS_SIZE);
								 EEPROM.end();
							 }
						 });

		// Finally, start up the UI.
		// This should only be called once we are connected to WiFi.
		ESPUI.begin(HOSTNAME);
	}

	// Most elements in this test UI are assigned this generic callback which prints some
	// basic information. Event types are defined in ESPUI.h
	void generalCallback(Control * sender, int type)
	{
		Serial.print("CB: id(");
		Serial.print(sender->id);
		Serial.print(") Type(");
		Serial.print(type);
		Serial.print(") '");
		Serial.print(sender->label);
		Serial.print("' = ");
		Serial.println(sender->value);
	}

	void setup()
	{
		Serial.begin(115200);
		// Give a serial monitor a moment to attach, but carry on without one.
		unsigned long serialWaitStart = millis();
		while (!Serial && millis() - serialWaitStart < SERIAL_WAIT_MS)
			delay(10);
		if (SLOW_BOOT)
			delay(5000); // Delay booting to give time to connect a serial monitor

		// -------------------------ESPUI------------------------- //
		connectWifi();
		WiFi.setSleep(false); // Turn off Wi-Fi sleep to keep the UI responsive (at the cost of power use)
		setUpUI();
		// -------------------------------------------------------- //


	}

	void loop()
	{
		// Simple debug UART interface
		if (Serial.available())
		{
			switch (Serial.read())
			{
			case 'w': // Print IP details
				Serial.println(WiFi.localIP());
				break;
			case 'W': // Reconnect wifi
				connectWifi();
				break;
			default:
				Serial.print('#');
				break;
			}
		}
	}
	// -------------------------------------------------------- //

	// Utilities
	//
	// If you are here just to see examples of how to use ESPUI, you can ignore the following functions
	//------------------------------------------------------------------------------------------------
	void readStringFromEEPROM(String & buf, int baseaddress, int size)
	{
		buf.reserve(size);
		for (int i = baseaddress; i < baseaddress + size; i++)
		{
			char c = EEPROM.read(i);
			buf += c;
			if (!c)
				break;
		}
	}

	// Writes at most (size - 1) characters of str followed by a terminating
	// null, so the write stays within [baseaddress, baseaddress + size).
	void writeStringToEEPROM(const String &str, int baseaddress, int size)
	{
		int len = str.length();
		if (len > size - 1)
			len = size - 1;
		for (int i = 0; i < len; i++)
			EEPROM.write(baseaddress + i, str.charAt(i));
		EEPROM.write(baseaddress + len, '\0');
	}

	void connectWifi()
	{
		int connect_timeout;

		WiFi.setHostname(HOSTNAME);
		Serial.println("Begin wifi...");

		// Load credentials from EEPROM
		if (!(FORCE_USE_HOTSPOT))
		{
			yield();
			EEPROM.begin(EEPROM_SIZE);
			String stored_ssid, stored_pass;
			readStringFromEEPROM(stored_ssid, EEPROM_SSID_ADDR, EEPROM_SSID_SIZE);
			readStringFromEEPROM(stored_pass, EEPROM_PASS_ADDR, EEPROM_PASS_SIZE);
			EEPROM.end();

			// Try to connect with stored credentials, fire up an access point if they don't work.
			WiFi.begin(stored_ssid.c_str(), stored_pass.c_str());
			connect_timeout = 28; // 7 seconds
			while (WiFi.status() != WL_CONNECTED && connect_timeout > 0)
			{
				delay(250);
				Serial.print(".");
				connect_timeout--;
			}
		}

		if (WiFi.status() == WL_CONNECTED)
		{
			Serial.println(WiFi.localIP());
			Serial.println("Wifi started");

			if (!MDNS.begin(HOSTNAME))
			{
				Serial.println("Error setting up MDNS responder!");
			}
		}
		else
		{
			Serial.println("\nCreating access point...");
			WiFi.mode(WIFI_AP);
			WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
			WiFi.softAP(HOSTNAME);

			connect_timeout = 20;
			do
			{
				delay(250);
				Serial.print(",");
				connect_timeout--;
			} while (connect_timeout);
		}
	}

	void textCallback(Control * sender, int type)
	{
		// This callback is needed to handle the changed values, even though it doesn't do anything itself.
	}
