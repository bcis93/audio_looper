/**
 * @file Track.h
 * 
 * @brief Track class
 * 
 * This file implements the API for the Track class. The Track class simply
 * keeps track of if the track is currently playing, recording, or both (or
 * neither).
 * 
 * @author Bryan Cisneros
 */

#pragma once

class Track
{
public:
	/**
	 * @brief Track constructor
	 * 
	 * @return void
	 */
	Track();

	/**
	 * @brief Track destructor
	 * 
	 * @return void
	 */
	~Track();

	/**
	 * @brief is track playing?
	 * 
	 * @return true if playing, false otherwise
	 */
	bool isPlaying();

	/**
	 * @brief is track recording?
	 * 
	 * @return true if recording, false otherwise
	 */
	bool isRecording();

	/**
	 * @brief start playing the track
	 * 
	 * @return void
	 */
	void startPlaying();

	/**
	 * @brief start recording the track
	 * 
	 * @return void
	 */
	void startRecording();

	/**
	 * @brief stop playing the track
	 * 
	 * @return void
	 */
	void stopPlaying();

	/**
	 * @brief stop recording the track
	 * 
	 * @return void
	 */
	void stopRecording();

private:
	// variables to store playing and recording state
	bool playing;
	bool recording;
};

