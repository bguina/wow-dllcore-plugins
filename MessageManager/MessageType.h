#pragma once

enum class MessageType : uint8_t {
	// wtf defining the unknown
	UNKNOWN=0,
	
	// Notify a new incoming message
	MESSAGE_TYPE = 2,

	// Request server for available processes and dlls.
	GET_SERVER_OPTIONS = 3,
	
	// Request injection with a given pair of dll+pid.
	// Client is implicitly associated to the resulting process injection.
	POST_SERVER_INJECTION = 4,

	// Request self-ejection by the associated associated dll.
	POST_SERVER_EJECTION = 5,

	// Respond dll injection success from associated dll.
	DLL_RESPONSE_INJECTED = 6,

	// Request for a given labeled data to the associated dll.
	DATA_PROMPT_DLL = 7,
	
	// Toggle subscription to a data observer from associated dll.
	SUBSCRIBE_DLL_UPDATES = 8,
	UNSUBSCRIBE_DLL_UPDATES = 9,

	SELECT_PLUGIN = 10, // NOT IMPLEMENTED
	// Toggle paused / resumed state of the currently selected plugin in the associated dll plugin.
	RESUME_PLUGIN = 11,
	PAUSE_PLUGIN = 12,

	// Submit a 3D path to the associated injected dll to be handled by the currently set up plugin.
	POST_DLL_DATA_3DPATH = 13,
};
