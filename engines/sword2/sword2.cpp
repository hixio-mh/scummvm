/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * Additional copyright for this file:
 * Copyright (C) 1994-1998 Revolution Software Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "base/plugins.h"

#include "common/config-manager.h"
#include "common/textconsole.h"
#include "common/translation.h"
#include "common/error.h"

#include "engines/util.h"

#include "sword2/sword2.h"
#include "sword2/defs.h"
#include "sword2/detection.h"
#include "sword2/header.h"
#include "sword2/console.h"
#include "sword2/controls.h"
#include "sword2/logic.h"
#include "sword2/maketext.h"
#include "sword2/memory.h"
#include "sword2/mouse.h"
#include "sword2/resman.h"
#include "sword2/router.h"
#include "sword2/screen.h"
#include "sword2/sound.h"

namespace Sword2 {

Common::Platform Sword2Engine::_platform;

Sword2Engine::Sword2Engine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst), _rnd("sword2") {
	// Add default file directories
	const Common::FSNode gameDataDir(ConfMan.getPath("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "clusters");
	SearchMan.addSubDirectoryMatching(gameDataDir, "sword2");
	SearchMan.addSubDirectoryMatching(gameDataDir, "video");
	SearchMan.addSubDirectoryMatching(gameDataDir, "smacks");
	SearchMan.addSubDirectoryMatching(gameDataDir, "extras"); // GOG.com
	SearchMan.addSubDirectoryMatching(gameDataDir, "streams"); // PSX video

	_features = gameDesc->flags;
	Sword2Engine::_platform = gameDesc->platform;

	_bootParam = ConfMan.getInt("boot_param");
	_saveSlot = ConfMan.getInt("save_slot");

	_memory = nullptr;
	_resman = nullptr;
	_sound = nullptr;
	_screen = nullptr;
	_mouse = nullptr;
	_logic = nullptr;
	_fontRenderer = nullptr;
	_isRTL = Common::parseLanguage(ConfMan.get("language")) == Common::HE_ISR;
	_debugger = nullptr;

	_keyboardEvent.pending = false;
	_mouseEvent.pending = false;

	_wantSfxDebug = false;

	_gameCycle = 0;
	_gameSpeed = 1;

	_gmmLoadSlot = -1; // Used to manage GMM Loading

	_isKorTrs = gameDesc->language == Common::KO_KOR;
}

Sword2Engine::~Sword2Engine() {
	//_debugger is deleted by Engine
	delete _sound;
	delete _fontRenderer;
	delete _screen;
	delete _mouse;
	delete _logic;
	delete _resman;
	delete _memory;
}

void Sword2Engine::registerDefaultSettings() {
	ConfMan.registerDefault("gfx_details", 2);
	ConfMan.registerDefault("reverse_stereo", false);
}

void Sword2Engine::syncSoundSettings() {
	Engine::syncSoundSettings();

	bool mute = ConfMan.getBool("mute");

	setSubtitles(ConfMan.getBool("subtitles"));

	// Our own settings dialog can mute the music, speech and sound effects
	// individually. ScummVM's settings dialog has one master mute setting.

	if (ConfMan.hasKey("mute")) {
		ConfMan.setBool("music_mute", ConfMan.getBool("mute"));
		ConfMan.setBool("speech_mute", ConfMan.getBool("mute"));
		ConfMan.setBool("sfx_mute", ConfMan.getBool("mute"));

		if (!mute) // it is false
			// So remove it in order to let individual volumes work
			ConfMan.removeKey("mute", ConfMan.getActiveDomainName());
	}

	_sound->muteMusic(ConfMan.getBool("music_mute"));
	_sound->muteSpeech(ConfMan.getBool("speech_mute"));
	_sound->muteFx(ConfMan.getBool("sfx_mute"));
	_sound->setReverseStereo(ConfMan.getBool("reverse_stereo"));
}

void Sword2Engine::readSettings() {
	syncSoundSettings();
	_mouse->setObjectLabels(ConfMan.getBool("object_labels"));
	_screen->setRenderLevel(ConfMan.getInt("gfx_details"));
}

void Sword2Engine::writeSettings() {
	ConfMan.setInt("music_volume", _mixer->getVolumeForSoundType(Audio::Mixer::kMusicSoundType));
	ConfMan.setInt("speech_volume", _mixer->getVolumeForSoundType(Audio::Mixer::kSpeechSoundType));
	ConfMan.setInt("sfx_volume", _mixer->getVolumeForSoundType(Audio::Mixer::kSFXSoundType));
	ConfMan.setBool("music_mute", _sound->isMusicMute());
	ConfMan.setBool("speech_mute", _sound->isSpeechMute());
	ConfMan.setBool("sfx_mute", _sound->isFxMute());
	ConfMan.setInt("gfx_details", _screen->getRenderLevel());
	ConfMan.setBool("subtitles", getSubtitles());
	ConfMan.setBool("object_labels", _mouse->getObjectLabels());
	ConfMan.setInt("reverse_stereo", _sound->isReverseStereo());

	// If even one sound type is unmuted, we can't say that all sound is
	// muted.

	if (!_sound->isMusicMute() || !_sound->isSpeechMute() || !_sound->isFxMute()) {
		ConfMan.setBool("mute", false);
	}

	ConfMan.flushToDisk();
}

int Sword2Engine::getFramesPerSecond() {
	return _gameSpeed * FRAMES_PER_SECOND;
}

/**
 * The global script variables and player object should be kept open throughout
 * the game, so that they are never expelled by the resource manager.
 */

void Sword2Engine::setupPersistentResources() {
	_logic->_scriptVars = _resman->openResource(1) + ResHeader::size();
	_resman->openResource(CUR_PLAYER_ID);
}

Common::Error Sword2Engine::run() {
	// Get some falling RAM and put it in your pocket, never let it slip
	// away

	_debugger = nullptr;
	_sound = nullptr;
	_fontRenderer = nullptr;
	_screen = nullptr;
	_mouse = nullptr;
	_logic = nullptr;
	_resman = nullptr;
	_memory = nullptr;

	initGraphics(640, 480);
	_screen = new Screen(this, 640, 480);

	// Create the debugger as early as possible (but not before the
	// screen object!) so that errors can be displayed in it. In
	// particular, we want errors about missing files to be clearly
	// visible to the user.

	_debugger = new Debugger(this);
	setDebugger(_debugger);

	_memory = new MemoryManager();
	_resman = new ResourceManager(this);

	if (!_resman->init())
		return Common::kUnknownError;

	_logic = new Logic(this);
	_fontRenderer = new FontRenderer(this);
	_sound = new Sound(this);
	_mouse = new Mouse(this);
	
	_fontRenderer->loadTranslations();

	registerDefaultSettings();
	readSettings();

	initStartMenu();

	// During normal gameplay, we care neither about mouse button releases
	// nor the scroll wheel.
	setInputEventFilter(RD_LEFTBUTTONUP | RD_RIGHTBUTTONUP | RD_WHEELUP | RD_WHEELDOWN);

	setupPersistentResources();
	initializeFontResourceFlags();

	if (_features & ADGF_DEMO)
		_logic->writeVar(DEMO, 1);
	else
		_logic->writeVar(DEMO, 0);

	if (_saveSlot != -1) {
		if (saveExists(_saveSlot))
			restoreGame(_saveSlot);
		else {
			RestoreDialog dialog(this);
			if (!dialog.runModal())
				startGame();
		}
	} else if (!_bootParam && saveExists() && !isPsx()) { // Initial load/restart panel disabled in PSX
		int32 pars[2] = { 221, FX_LOOP };                 // version because of missing panel resources
		bool result;

		_mouse->setMouse(NORMAL_MOUSE_ID);
		_logic->fnPlayMusic(pars);

		StartDialog dialog(this);

		result = (dialog.runModal() != 0);

		// If the game is started from the beginning, the cutscene
		// player will kill the music for us. Otherwise, the restore
		// will either have killed the music, or done a crossfade.

		if (shouldQuit())
			return Common::kNoError;

		if (result)
			startGame();
	} else
		startGame();

	_screen->initializeRenderCycle();

	while (1) {
		// Handle GMM Loading
		if (_gmmLoadSlot != -1) {

			// Hide mouse cursor and fade screen
			_mouse->hideMouse();
			_screen->fadeDown();

			// Clean up and load game
			_logic->_router->freeAllRouteMem();

			// TODO: manage error handling
			restoreGame(_gmmLoadSlot);

			// Reset load slot
			_gmmLoadSlot = -1;

			// Show mouse
			_mouse->addHuman();
		}

		KeyboardEvent *ke = keyboardEvent();

		if (ke) {
			if (ke->kbd.hasFlags(0) || ke->kbd.hasFlags(Common::KBD_SHIFT)) {
				switch (ke->kbd.keycode) {
				case Common::KEYCODE_p:
					if (isPaused()) {
						_screen->dimPalette(false);
						_gamePauseToken.clear();
					} else {
						_gamePauseToken = pauseEngine();
						_screen->dimPalette(true);
					}
					break;
#if 0
				// Disabled because of strange rumors about the
				// credits running spontaneously every few
				// minutes.
				case Common::KEYCODE_c:
					if (!_logic->readVar(DEMO) && !_mouse->isChoosing()) {
						ScreenInfo *screenInfo = _screen->getScreenInfo();
						_logic->fnPlayCredits(nullptr);
						screenInfo->new_palette = 99;
					}
					break;
#endif
				default:
					break;
				}
			}
		}

		// skip GameCycle if we're paused
		if (!isPaused()) {
			_gameCycle++;
			gameCycle();
		}

		// We can't use this as termination condition for the loop,
		// because we want the break to happen before updating the
		// screen again.

		if (shouldQuit())
			break;

		// creates the debug text blocks
		_debugger->buildDebugText();

		_screen->buildDisplay();
	}

	return Common::kNoError;
}

void Sword2Engine::restartGame() {
	ScreenInfo *screenInfo = _screen->getScreenInfo();
	uint32 temp_demo_flag;

	_mouse->closeMenuImmediately();

	// Restart the game. To do this, we must...

	// Stop music instantly!
	_sound->stopMusic(true);

	// In case we were dead - well we're not anymore!
	_logic->writeVar(DEAD, 0);

	// Restart the game. Clear all memory and reset the globals
	temp_demo_flag = _logic->readVar(DEMO);

	// Remove all resources from memory, including player object and
	// global variables
	_resman->removeAll();

	// Reopen global variables resource and player object
	setupPersistentResources();

	_logic->writeVar(DEMO, temp_demo_flag);

	// Free all the route memory blocks from previous game
	_logic->_router->freeAllRouteMem();

	// Call the same function that first started us up
	startGame();

	// Prime system with a game cycle

	// Reset the graphic 'BuildUnit' list before a new logic list
	// (see fnRegisterFrame)
	_screen->resetRenderLists();

	// Reset the mouse hot-spot list (see fnRegisterMouse and
	// fnRegisterFrame)
	_mouse->resetMouseList();

	_mouse->closeMenuImmediately();

	// FOR THE DEMO - FORCE THE SCROLLING TO BE RESET!
	// - this is taken from fnInitBackground
	// switch on scrolling (2 means first time on screen)
	screenInfo->scroll_flag = 2;

	if (_logic->processSession())
		error("restart 1st cycle failed??");

	// So palette not restored immediately after control panel - we want
	// to fade up instead!
	screenInfo->new_palette = 99;
}

bool Sword2Engine::checkForMouseEvents() {
	return _mouseEvent.pending;
}

MouseEvent *Sword2Engine::mouseEvent() {
	if (!_mouseEvent.pending)
		return nullptr;

	_mouseEvent.pending = false;
	return &_mouseEvent;
}

KeyboardEvent *Sword2Engine::keyboardEvent() {
	if (!_keyboardEvent.pending)
		return nullptr;

	_keyboardEvent.pending = false;
	return &_keyboardEvent;
}

uint32 Sword2Engine::setInputEventFilter(uint32 filter) {
	uint32 oldFilter = _inputEventFilter;

	_inputEventFilter = filter;
	return oldFilter;
}

/**
 * OSystem Event Handler. Full of cross platform goodness and 99% fat free!
 */

void Sword2Engine::parseInputEvents() {
	Common::Event event;

	while (_eventMan->pollEvent(event)) {
		switch (event.type) {
		case Common::EVENT_KEYDOWN:
			if (event.kbd.hasFlags(Common::KBD_CTRL)) {
				if (event.kbd.keycode == Common::KEYCODE_f) {
					if (_gameSpeed == 1)
						_gameSpeed = 2;
					else
						_gameSpeed = 1;
				}
			}
			if (!(_inputEventFilter & RD_KEYDOWN)) {
				_keyboardEvent.pending = true;
				_keyboardEvent.kbd = event.kbd;
			}
			break;
		case Common::EVENT_LBUTTONDOWN:
			if (!(_inputEventFilter & RD_LEFTBUTTONDOWN)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_LEFTBUTTONDOWN;
			}
			break;
		case Common::EVENT_RBUTTONDOWN:
			if (!(_inputEventFilter & RD_RIGHTBUTTONDOWN)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_RIGHTBUTTONDOWN;
			}
			break;
		case Common::EVENT_LBUTTONUP:
			if (!(_inputEventFilter & RD_LEFTBUTTONUP)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_LEFTBUTTONUP;
			}
			break;
		case Common::EVENT_RBUTTONUP:
			if (!(_inputEventFilter & RD_RIGHTBUTTONUP)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_RIGHTBUTTONUP;
			}
			break;
		case Common::EVENT_WHEELUP:
			if (!(_inputEventFilter & RD_WHEELUP)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_WHEELUP;
			}
			break;
		case Common::EVENT_WHEELDOWN:
			if (!(_inputEventFilter & RD_WHEELDOWN)) {
				_mouseEvent.pending = true;
				_mouseEvent.buttons = RD_WHEELDOWN;
			}
			break;
		default:
			break;
		}
	}
}

void Sword2Engine::gameCycle() {
	// Do one game cycle, that is run the logic session until a full loop
	// has been performed.

	if (_logic->getRunList()) {
		do {
			// Reset the 'BuildUnit' and mouse hot-spot lists
			// before each new logic list. The service scripts
			// will fill thrm through fnRegisterFrame() and
			// fnRegisterMouse().

			_screen->resetRenderLists();
			_mouse->resetMouseList();

			// Keep going as long as new lists keep getting put in
			// - i.e. screen changes.
		} while (_logic->processSession());
	} else {
		// Start the console and print the start options perhaps?
		_debugger->attach("AWAITING START COMMAND: (Enter 's 1' then 'q' to start from beginning)");
	}

	// If this screen is wide, recompute the scroll offsets every cycle
	ScreenInfo *screenInfo = _screen->getScreenInfo();

	if (screenInfo->scroll_flag)
		_screen->setScrolling();

	_mouse->mouseEngine();
	_sound->processFxQueue();
}

void Sword2Engine::startGame() {
	// Boot the game straight into a start script. It's always George's
	// script #1, but with different ScreenManager objects depending on
	// if it's the demo or the full game, or if we're using a boot param.

	int screen_manager_id = 0;

	debug(5, "startGame() STARTING:");

	if (!_bootParam) {
		if (_logic->readVar(DEMO))
			screen_manager_id = 19;		// DOCKS SECTION START
		else
			screen_manager_id = 949;	// INTRO & PARIS START
	} else {
		// FIXME this could be validated against startup.inf for valid
		// numbers to stop people shooting themselves in the foot

		if (_bootParam != 0)
			screen_manager_id = _bootParam;
	}

	_logic->runResObjScript(screen_manager_id, CUR_PLAYER_ID, 1);
}

// FIXME: Move this to some better place?

void Sword2Engine::sleepUntil(uint32 time) {
	while (getMillis() < time) {
		// Make sure menu animations and fades don't suffer, but don't
		// redraw the entire scene.
		_mouse->processMenu();
		_screen->updateDisplay(false);
		_system->delayMillis(10);
	}
}

void Sword2Engine::pauseEngineIntern(bool pause) {
	Engine::pauseEngineIntern(pause);

	if (pause) {
		_screen->pauseScreen(true);
	} else {
		_screen->pauseScreen(false);
	}
}

uint32 Sword2Engine::getMillis() {
	return _system->getMillis();
}

Common::Error Sword2Engine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
	uint32 saveVal = saveGame(slot, (const byte *)desc.c_str());

	if (saveVal == SR_OK)
		return Common::kNoError;
	else if (saveVal == SR_ERR_WRITEFAIL || saveVal == SR_ERR_FILEOPEN)
		return Common::kWritingFailed;
	else
		return Common::kUnknownError;
}

bool Sword2Engine::canSaveGameStateCurrently(Common::U32String *msg) {
	bool canSave = true;

	// No save if dead
	if (_logic->readVar(DEAD))
		canSave = false;

	// No save if mouse not shown
	else if (_mouse->getMouseStatus())
		canSave = false;
	// No save if inside a menu
	else if (_mouse->getMouseMode() == MOUSE_system_menu)
		canSave = false;

	// No save if fading
	else if (_screen->getFadeStatus())
		canSave = false;

	return canSave;
}

Common::Error Sword2Engine::loadGameState(int slot) {

	// Prepare the game to load through GMM
	_gmmLoadSlot = slot;

	// TODO: error handling.
	return Common::kNoError;
}

bool Sword2Engine::canLoadGameStateCurrently(Common::U32String *msg) {
	bool canLoad = true;

	// No load if mouse is disabled
	if (_mouse->getMouseStatus())
		canLoad = false;
	// No load if mouse is in system menu
	else if (_mouse->getMouseMode() == MOUSE_system_menu)
		canLoad = false;
	// No load if we are fading
	else if (_screen->getFadeStatus())
		canLoad = false;

	// But if we are dead, ignore previous conditions
	if (_logic->readVar(DEAD))
		canLoad = true;

	return canLoad;
}

} // End of namespace Sword2
