
// prevent recursive includes
#ifndef VSDRVR_H__A184E926_0461_11D7_BBC5_444553540001__INCLUDED_
#define VSDRVR_H__A184E926_0461_11D7_BBC5_444553540001__INCLUDED_ 

// The following ifdef block enables exporting and importing the entry points
// All files within this DLL are compiled with the VSDRVR_EXPORTS
//	symbol defined on the command line. 
// This symbol should not be defined on any project that uses this DLL. 
//	This way any other project whose source files include this file see 
//	VSDRVR_API functions as being imported from a DLL, wheras this DLL sees symbols
//	defined with this macro as being exported.

#ifdef VSDRVR_EXPORTS
#define VSDRVR_API __declspec(dllexport)
#else
#define VSDRVR_API __declspec(dllimport)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//	type declarations, as necessary
#ifndef Int32
#define Int32 __int32
#endif


typedef struct filterInfo
{
	char intfcName[32];
	int serNo;
	int version;
	double minWl;
	double maxWl;
} VS_SETUP;

enum VSSTAT {
	VSD_ERR_NOERROR = 0,
	VSD_ERR_NOT_OPEN= 100,
	VSD_ERR_BAD_HANDLE,
	VSD_ERR_NOT_INITIALIZED,
	VSD_ERR_ALREADY_OPEN,
	VSD_ERR_INTERFACE_NOT_AVAILABLE,
	VSD_ERR_COMMUNICATION_ERROR,
	VSD_ERR_NO_DEVICE_PRESENT,
	VSD_ERR_COMMUNICATION_SEND_ERROR,
	VSD_ERR_COMMUNICATION_REPLY_ERROR,
	VSD_ERR_COMMUNICATION_GENERAL_ERROR,
	VSD_ERR_BLOCK_SEND_ERROR,
	VSD_ERR_BLOCK_RECEIVE_ERROR,
	VSD_ERR_NO_READY_REPLY,
	VSD_ERR_COMMAND_TOO_LONG,
	VSD_ERR_SET_PAR_FAILED,
	VSD_ERR_GET_PAR_FAILED,
	VSD_ERR_DEVICE_BUSY,
	VSD_ERR_NO_CHARACTER_ECHO,
	VSD_ERR_SET_BLOCK_TIMEOUT_FAILED,
	VSD_ERR_SET_FLOAT_FAILED,
	VSD_ERR_INITIALIZATION_FAILED,
	VSD_ERR_GETERRORFAILED,
	VSD_ERR_VERSION_READBACK_ERROR,
	VSD_ERR_CONFIG_READBACK_ERROR,
	VSD_ERR_ATTEMPT_TO_SET_RO_PARAMETER,
	VSD_ERR_NULL_RETURN_POINTER,
	VSD_ERR_INTERNAL_RANGE_ERROR,
	VSD_ERR_TOO_MANY_DEVICES_OPEN,

	VSD_ERR_NO_WAVEPLATES_PRESENT=256,

	VSD_ERR_INT_OBJECT_CREATION_FAILED=1000,
	VSD_ERR_INT_WAVEPLATESTAGESINVALID
};



enum	VSMODES 
	{
	VSMODE_NORMAL = 0,
	VSMODE_ADV_PALETTE,
	VSMODE_JUMP_WAVELENGTH,
	VSMODE_SW_WAIT_FOR_TRIGGER=6
	};


#define	VS_HANDLE Int32



//	function declarations 
#ifdef __cplusplus
extern "C"{ // ensure undecorated functions...
#endif // __cplusplus



// --------------------   C interfaces    --------------------
	VSDRVR_API Int32 VsGetAvailableDevices(Int32 *pnDev, VS_SETUP pConfigList[], Int32 maxListEntries);
	VSDRVR_API Int32 VsOpen(VS_HANDLE *phVs, LPCSTR port, Int32 *pErrorCode);
	VSDRVR_API Int32 VsClose(VS_HANDLE hVs);


	VSDRVR_API Int32 VsIsPresent(VS_HANDLE hVs);
	VSDRVR_API Int32 VsIsReady(VS_HANDLE hVs);
	VSDRVR_API Int32 VsInitialize(VS_HANDLE hVs, Int32 initType, BOOL synchronous);
	VSDRVR_API Int32 VsClearPalette(VS_HANDLE hVs);
	VSDRVR_API Int32 VsDefinePalette(VS_HANDLE hVs, Int32 palEntry, double wl, BOOL synchronous);
   VSDRVR_API Int32 VsDefinePaletteWavelengthAndWaves(VS_HANDLE hVs, Int32 palEntry, 
                                                   double wl, double *waves, BOOL synchronous);
	VSDRVR_API Int32 VsSetPalette(VS_HANDLE hVs, Int32 palEntry);
	VSDRVR_API Int32 VsGetPalette(VS_HANDLE hVs, Int32 *ppalEntry);
	VSDRVR_API Int32 VsSetWavelength(VS_HANDLE hVs, double wl, BOOL confirm);
	VSDRVR_API Int32 VsGetWavelength(VS_HANDLE hVs, double *pwl);
	VSDRVR_API Int32 VsDisconnect(VS_HANDLE hVs);
	VSDRVR_API Int32 VsClearPendingCommands(VS_HANDLE hVs);
	VSDRVR_API Int32 VsGetTemperature(VS_HANDLE hVs, double *ptemperature);
	VSDRVR_API Int32 VsClearError(VS_HANDLE hVs);
	VSDRVR_API Int32 VsGetError(VS_HANDLE hVs, Int32 *pErr);
	VSDRVR_API Int32 VsGetFilterIdentity(VS_HANDLE hVs, Int32 *pVer, Int32 *pSerno, double *pminWl, double *pmaxWl);
	VSDRVR_API Int32 VsSetMode(VS_HANDLE hVs, Int32 mode);
	VSDRVR_API Int32 VsGetMode(VS_HANDLE hVs, Int32 *pMode);
	VSDRVR_API Int32 VsSetWavelengthStep(VS_HANDLE hVs, double wl, BOOL confirm);
	VSDRVR_API Int32 VsGetWavelengthStep(VS_HANDLE hVs, double &pWl);

//	----------------------typically used only on rare occasion or for debugging------------------------
	VSDRVR_API Int32 VsSetRetries(VS_HANDLE hVs, Int32 retries);
	VSDRVR_API Int32 VsSetLatencyMs(VS_HANDLE hVs, Int32 latencyMs);

//	----------------------typically only used in VariLC / PolScope applications	-----------------------
	VSDRVR_API Int32 VsGetNstages(VS_HANDLE hVs, Int32 *pStages);
	VSDRVR_API Int32 VsGetWaveplateStages(VS_HANDLE hVS, Int32 *pwplStages);
	VSDRVR_API Int32 VsGetWaveplateLimits(VS_HANDLE hVs, double minWaveArr[], double maxWaveArr[]);
	VSDRVR_API Int32 VsSetWavelengthAndWaves(VS_HANDLE hVs, double wavelength, double waves[]);
	VSDRVR_API Int32 VsGetWavelengthAndWaves(VS_HANDLE hVs, double *pWavelength, double waves[]);


//	----------------------typically only used in Nuance applications -----------------------------------
	VSDRVR_API Int32 VsGetSettleMs(VS_HANDLE hVs, DWORD *pMillisec);
//	VSDRVR_API Int32 VsHasTrigger(VS_HANDLE hVs, BOOL *pHas);
	VSDRVR_API Int32 VsIsEngagedInBeam(VS_HANDLE hVs, BOOL *pIn);
    VSDRVR_API Int32 VsGetConfigInfo(VS_HANDLE hVs, Int32 *nConfigs, Int32 *configNow);
    VSDRVR_API Int32 VsSetConfigSel(VS_HANDLE hVs, Int32 sel);

//	-- DEPRECATED ---
//	use of the implicit pallete was beneficial for  older (RS232) units
//	however it confers no benefit for units made since 2004 (USB)
	VSDRVR_API Int32 VsEnableImplicitPalette(VS_HANDLE hVs, BOOL palettesEnabled);

// -----------------------THESE ARE OBSOLESCENT AND SHOULD NOT BE USED - USE VsOpen() instead
	VSDRVR_API Int32 VsConnect(VS_HANDLE *phVs, LPCSTR port, char eolChar, Int32 baud);
	VSDRVR_API Int32 VsIsConnected(VS_HANDLE hVs);


//	the next functions are useful for accessing specialized commands not provided above
	VSDRVR_API Int32 VsSendCommand(VS_HANDLE hVs, LPCSTR cmd, BOOL appendEol);
	VSDRVR_API Int32 VsGetReply(VS_HANDLE hVs, LPSTR reply, Int32 nChars, Int32 waitMs);
	VSDRVR_API Int32 VsGetPendingReply(VS_HANDLE hVs, LPSTR reply, Int32 nChars, Int32 *pQuit, Int32 firstMs, Int32 subsequentMs);

//	the following are only used during filter manufacture and calibration at CRI
//		and may be ignored by third-party programmers
	VSDRVR_API Int32 VsExercise(VS_HANDLE hVs, Int32 nCycles, BOOL synchronous);
	VSDRVR_API Int32 VsGetAllDrive(VS_HANDLE hVs, Int32 *pStages, Int32 drive[]);
	VSDRVR_API Int32 VsSendBinary(VS_HANDLE hVs, char *bin, Int32 nChars, BOOL clearEcho);
	VSDRVR_API Int32 VsSetStageDrive(VS_HANDLE hVs, Int32 stage, Int32 drive);
	VSDRVR_API Int32 VsThermistorCounts(VS_HANDLE hVs, Int32 *pCounts);
	VSDRVR_API Int32 VsIsDiagnostic(VS_HANDLE hVs, Int32 *pisDiag);


	//	can be used to speed up communication - safe for USB systems, not safe on RS-232
	VSDRVR_API Int32 VsSetWavelengthWavesConfirm(VS_HANDLE hVs, Int32 confirmCommands);

#ifdef __cplusplus
}
#endif // __cplusplus
// ------------------------------------------------------------


#endif // VSDRVR_H
