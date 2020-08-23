// Copyright(C) 2018-2019 by Steven Adler
//
// This file is part of TwoCan, a plugin for OpenCPN.
//
// TwoCan is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TwoCan is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TwoCan. If not, see <https://www.gnu.org/licenses/>.
//
// NMEA2000® is a registered Trademark of the National Marine Electronics Association

#ifndef TWOCAN_PLUGIN_H
#define TWOCAN_PLUGIN_H

// Pre compiled headers 
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
      #include <wx/wx.h>
#endif

// Defines all of the OpenCPN plugin virtual methods we need to override
#include "ocpn_plugin.h"

// Preferences Dialog (Note this header also includes twocanversion.h which is automatically generated)
#include "twocansettings.h"

// TwoCan device, which is our implementation of a NMEA 2000 device
#include "twocandevice.h"

// Icons Use png2wx.pl perl script to convert png images to wxWidgets memory streams
#include "twocanicons.h"

// BUG BUG check which wxWidget includes we really need
// Arrays of Strings
#include <wx/arrstr.h> 
// Configuration
#include <wx/fileconf.h>
// For passing references to wxCharacter strings
#include <wx/wxchar.h>
// Frame received events
#include <wx/event.h>

// Plugin receives FrameReceived events from the TwoCan device
const wxEventType wxEVT_SENTENCE_RECEIVED_EVENT = wxNewEventType();

// Globally accessible variables used by the plugin, device and the settings dialog.
// OpenCPN Configuration Settings
wxFileConfig *configSettings;
// To support the new Plugin Manager which requires a new API to determine the plugin's data folder
wxString pluginDataFolder;
// The name of the TwoCan driver being used to access the CAN bus or the Log Files
wxString canAdapter;
// Bit mask determing what NMEA 2000 PGN's are to be converted to NMEA 183 sentences
int supportedPGN;
// Whether the TwoCan device is a passive device (just listens) or whether it fully participates on teh NMEA 2000 network
bool deviceMode;
// Indicate whether the TwoCan Preferences diaog displays real time NMEA sentences
bool debugWindowActive;
// If an Active Device, whether to periodically send heartbeats
bool enableHeartbeat;
// If any logging is to be performed and in what format (twocan raw, candump, canboat, yacht devices or csv)
int logLevel;
// A 29bit number that uniqiuely identifies the TwoCan device if it is an Active Device
unsigned long uniqueId;
// A 1 byte CAN bus network address for this device if it is an Active device (0-253)
int networkAddress;
// Maintain a map of the all of the NMEA 2000 devices on the network
NetworkInformation networkMap[CONST_MAX_DEVICES];

// The TwoCan plugin
class TwoCan : public opencpn_plugin_116, public wxEvtHandler {

public:
	// The constructor
	TwoCan(void *ppimgr);
	
	// and destructor
	~TwoCan(void);

	// Overridden OpenCPN plugin methods
	int Init(void);
	bool DeInit(void);
	int GetAPIVersionMajor();
	int GetAPIVersionMinor();
	int GetPlugInVersionMajor();
	int GetPlugInVersionMinor();
	void ShowPreferencesDialog(wxWindow* parent);
	wxString GetCommonName();
	wxString GetShortDescription();
	wxString GetLongDescription();
	void SetNMEASentence(wxString &sentence); // Not used yet...
	wxBitmap *GetPlugInBitmap();
	
private: 
	// NMEA 2000 device
	TwoCanDevice *twoCanDevice;

	// Load & Save settings
	bool LoadConfiguration(void);
	bool SaveConfiguration(void);

	// Preferences dialog
	TwoCanSettings *settingsDialog;

	// Plugin bitmap
	wxBitmap pluginBitmap;

	// Reference to the OpenCPN window handle
	wxWindow *parentWindow;

	// NMEA 0183 sentence received events
	void OnSentenceReceived(wxCommandEvent &event);

	// TwoCanDevice
	void StartDevice(void);
	void StopDevice(void);

};

#endif 

